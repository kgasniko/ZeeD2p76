
#include "ZeeDCalculator/ZeeDCalcJets.h"

// Root libraries
#include <TSystem.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TObjArray.h>

// Athena includes
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"

// ZeeD analysis includes
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDMuonSpShower.h"
#include "ZeeDEvent/ZeeDJet.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSystematics.h"
// Functions:
#include "ZeeDCalculator/ZeeDFunctions.h"

//------------------------------------------------------
ZeeDCalcJets::ZeeDCalcJets() : fSvcHelper("ZeeDCalcJets"),
			       fAnaOptions(fSvcHelper.GetAnaSvc())

{

  fSys = ZeeDSystematics::getInstance();

  fJetCalibrationTool = NULL;
  fBCHCleaningTool    = NULL;
  fJetMultiJESUncTool = NULL;

  if ( (*fAnaOptions)->RecalibrateJets() )
    {
      TString calibrationConfigFile( (*fAnaOptions)->RecalibrateJetsConfig() );
      if (calibrationConfigFile.Length() ==    0) 
	{
	  Warning(__PRETTY_FUNCTION__," Config file is not specified : %s.", calibrationConfigFile.Data());
	  calibrationConfigFile = ".";
	}

      /// Different configs for data and MC should be used for jets calibration
      /// https://twiki.cern.ch/twiki/bin/view/AtlasProtected/ApplyJetCalibration2012
      if ( (*fAnaOptions)->IsData() && calibrationConfigFile.Contains("MC12b") ){
	calibrationConfigFile.ReplaceAll("_MC12b","");
      }
      if ( (*fAnaOptions)->IsData() && calibrationConfigFile.Contains("MC12c") ){
	calibrationConfigFile.ReplaceAll("_MC12c","");
      }	  

      std::vector<std::string> collections = (*fAnaOptions)->JetCollectionNames();
      for (unsigned int i = 0; i < collections.size(); i++)
	{
	  TString algName( collections[ i ]);
	  algName.ReplaceAll("Jets","");
	  algName.ReplaceAll("New","");
	  algName.ReplaceAll("ReTagged","");	  
	  if (algName.Contains("Truth")) continue;
	  
	  fJetCalibrationTool = new JetAnalysisCalib::JetCalibrationTool( algName , calibrationConfigFile ,  (*fAnaOptions)->IsData() );
	  std::cout<<algName<<"   "<<calibrationConfigFile<<"   "<<(*fAnaOptions)->IsData()<<std::endl;
	  fJetCalibrationTool ->UseGeV( true );
	  
	  fJetCalibrationToolMap[ collections[ i ] ] = fJetCalibrationTool;
	}

      if ( (*fAnaOptions)->DoBCHCleaning() ){
	/// Info on BCH cleaning here
	/// https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/BCHCleaningTool

	TString TTileTripReaderFile = (*fAnaOptions) -> TTileTripReaderFile();
	TString BCHCleaningFile = ZeeDMisc::FindFile((*fAnaOptions) -> BCHCleaningFile()) ;
  
	fTTileTripReader = new Root::TTileTripReader("myTripReader");
	fTTileTripReader -> setTripFile(TTileTripReaderFile.Data());
	fBCHCleaningTool = new BCHTool::BCHCleaningToolRoot();
	bool isData = (*fAnaOptions) -> IsData();
	fBCHCleaningTool -> InitializeTool(isData, fTTileTripReader, BCHCleaningFile.Data());
      }

      /// Info on JES uncertainties can be found here
      /// https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/JetUncertainties
      if ((*fAnaOptions)->EvaluateSystematics()){
	
	//JES
	TString JESUncertConfig = (*fAnaOptions)->JESUncertaintiesConfig() ;
	TString MultiJESUncerConfig = (*fAnaOptions)->MultiJESUncertaintiesConfig() ;
	TString _workDir = (*fAnaOptions)->PathDirUncertaintiesConfig() ;

	//JER
	TString JERInputFile = (*fAnaOptions) -> JERInputFile();

	std::vector<std::string> collections = (*fAnaOptions)->JetCollectionNames();
	for (unsigned int i = 0; i < collections.size(); i++)
	  {
	    TString algName( collections[ i ]);
	    algName.ReplaceAll("Jets","");
	    algName.ReplaceAll("New","");
	    algName.ReplaceAll("ReTagged","");	  
	    if (algName.Contains("Truth")) continue;
	    fJetMultiJESUncTool = new MultijetJESUncertaintyProvider( MultiJESUncerConfig, JESUncertConfig, algName, "MC12a", _workDir );
	    fJetMultiJESUncTool -> useGeV( true );
	    fJetMultiJESUncTool -> initJES();
	    fJetMultiJESUncTool -> initMultijet();
	    fJetMultiJESUncToolMap[ collections[ i ] ] = fJetMultiJESUncTool;

	    Ncomp = fJetMultiJESUncTool -> getNUncertaintyComponents();
	    cout << "Syst Ncomp: "<<Ncomp << endl;
	    m_jesUncertainty.reserve(Ncomp);
	    m_jesUncertaintyMap[ collections[ i ] ] = m_jesUncertainty;

	    /// Info on JER here
	    /// https://twiki.cern.ch/twiki/bin/view/AtlasProtected/JetEnergyResolutionProvider2012
	    /// https://twiki.cern.ch/twiki/bin/view/AtlasProtected/ApplyJetResolutionSmearing
	    fJetSmearTool = new JetSmearingTool(algName.Data(), JERInputFile.Data());
	    fJetSmearTool -> UseGeV(true);
	    fJetSmearTool -> init();
	    NPseudoExp = 100;
	  }	
      }
    }

  //Info(__PRETTY_FUNCTION__," stop");

//   fJetCalibrationTool = NULL;
//   fJetMultiJESUncTool = NULL;

    // Constructor
}

//------------------------------------------------------
ZeeDCalcJets::~ZeeDCalcJets()
{
    // Destructor
  std::map<std::string,JetAnalysisCalib::JetCalibrationTool*>::iterator itr = fJetCalibrationToolMap.begin();
  for ( ; itr !=  fJetCalibrationToolMap.end(); ++itr) if ( itr->second ) delete itr->second;
  
  std::map<std::string, MultijetJESUncertaintyProvider*>::iterator itr_unc = fJetMultiJESUncToolMap.begin();
  for ( ; itr_unc !=  fJetMultiJESUncToolMap.end(); ++itr_unc) if ( itr->second ) delete itr_unc->second;

}

//------------------------------------------------------
void ZeeDCalcJets::Calculate(ZeeDEvent* event)
{
  //Info(__PRETTY_FUNCTION__," start");
  
  //std::cout <<"option = "<< (*fAnaOptions)->RecalibrateJets() << std::endl; 

  if ( !(*fAnaOptions)->RecalibrateJets() ) return;

  
  //  Info(__PRETTY_FUNCTION__," start2");

  const Int_t _nPV = event->NpvJets().Get();
  Double_t nPV(_nPV);
  const Double_t aveMu  = event->AverageInteractionsPerCrossing().Get();
  
  std::vector<std::string> collections = (*fAnaOptions)->JetCollectionNames();
  
  for (unsigned int i = 0; i < collections.size(); i++) {
    
    std::string collName = collections[ i ];

    if ( collName.find("Truth") != std::string::npos ) continue;

    fJetCalibrationTool = fJetCalibrationToolMap[ collName ];
    if ( (fJetCalibrationTool == NULL) && (collName.find("Truth") == std::string::npos)) {
      Error("ZeeDCalcJets::Calculate", "Jet calibration tool is not configured for collection ");
      Error("ZeeDCalcJets::Calculate",  collName.c_str());
      gSystem->Exit(EXIT_FAILURE);
    }
    
    if ( (fJetCalibrationTool == NULL)  ) continue;
    
    TObjArray* jets = event->GetJets( collName, kFALSE );

    const TObjArray* muonSpShowers = event->GetMuonSpShowers();
    
    if ( ( jets == NULL) ) continue;
    
    //     if ( jets == NULL) {
    //       Error("ZeeDCalcJets::Calculate", "Jet array is not initialised. For collection ");
    //       Error("ZeeDCalcJets::Calculate", collName.c_str() );
    //       Error("ZeeDCalcJets::Calculate", "Wrong jet collection !!! ");
    //       gSystem->Exit(EXIT_FAILURE);
    //     }
    //     else
    //     {
    //       std::cout <<"CalJets : coll "<< jets->GetName() <<" size = "<<jets->GetEntries()<< std::endl;
    
    //     }
    Double_t rho = 0;

//     if (  collName.find("EM") != std::string::npos  && collName.find("4") != std::string::npos )
//       rho = event->GetEtDensity_3EM() / GeV ;
    if (  collName.find("EM") != std::string::npos )
      rho = event->GetEtDensity_4EM() ;
    //     else if (  collName.find("LC") != std::string::npos  && collName.find("4") != std::string::npos )
    //       rho = event->GetEtDensity_3LC() / GeV ;
    else if (  collName.find("LC") != std::string::npos)
      rho = event->GetEtDensity_4LC() ;

    //std::cout <<"\n\n\n CalJets : coll "<< jets->GetName() <<" size = "<<jets->GetEntries()<<" rho = "<<rho<<" GeV \n\n\n  "<< std::endl;

    // Set JER seed
    if ((*fAnaOptions) -> EvaluateSystematics()){
      fJetSmearTool -> SetSeed(event -> EventNumber().Get());
    }

      for (Int_t ijet = 0;  ijet< jets->GetEntries(); ++ijet)
	{
	  
	  ZeeDJet* jet =dynamic_cast<ZeeDJet*>( jets->At(ijet) ) ;
	  
	  TLorentzVector fvCS      = jet->GetFourVectorCS();
	  Double_t EtaOrigin(jet->GetEtaOrigin());
	  Double_t PhiOrigin(jet->GetPhiOrigin());
	  Double_t MOrigin  (jet->GetMOrigin()); 
	  
	  Double_t areaPX(jet->GetActiveAreaPx());
	  Double_t areaPY(jet->GetActiveAreaPy());
	  Double_t areaPZ(jet->GetActiveAreaPz());
	  Double_t areaE (jet->GetActiveAreaE());
	  
	  Double_t nTrk       (jet->GetnTrk_pv0_1GeV());
	  Double_t trackWidth (jet->GettrackWIDTH_pv0_1GeV());
	  Double_t tileBar0   (jet->GetEnergy_TileBar0());
	  Double_t tileExt0   (jet->GetEnergy_TileExt0());
	  Double_t emb3       (jet->GetEnergy_EMB3());
	  Double_t eme3       (jet->GetEnergy_EME3());
	  //Double_t nSegments  (0.0);
	  nSegments=0.0;

	  Int_t muonsp_index = findMuspContainer( fvCS.Eta(), fvCS.Phi(), muonSpShowers );

	  if (muonsp_index >= 0 ) 
	    {
	      const ZeeDMuonSpShower* mspsh = (const ZeeDMuonSpShower*)muonSpShowers->At(muonsp_index);
	      nSegments = mspsh->finnerSegments + mspsh->fmiddleSegments + mspsh->fouterSegments ;
	    }
	  jet->SetNSegments(nSegments);

	  Double_t fvRAWE (jet->GetFourVectorRaw().E());
	  Double_t fTile0(0), fEM3(0);
	  if ( fvRAWE )
	    {
	      fTile0 = ( tileBar0 + tileExt0)/ fvRAWE;
	      fEM3   = ( emb3 + eme3 )/fvRAWE;
	    }
	  
	  int test=1;   // (-1) - no any tests // (0) - exclude 0 origin // (1) 1 origin=fvCS // (2) origin=closest jet
	  if ( 0==test && 0 == EtaOrigin && 0 == PhiOrigin && 0 == MOrigin){
	    //cout << EtaOrigin << "   " << PhiOrigin << "   " << MOrigin << endl;
	    jets->RemoveAt(ijet);
	    continue;
	  }

	  if ( 1==test && 0 == EtaOrigin && 0 == PhiOrigin && 0 == MOrigin){
	    EtaOrigin = fvCS.Eta();
	    PhiOrigin = fvCS.Phi();
	    MOrigin   = fvCS.M();
	    //cout << EtaOrigin << "   " << PhiOrigin << "   " << MOrigin << endl;
	  }

	  if ( 2==test &&  0 == EtaOrigin && 0 == PhiOrigin && 0 == MOrigin){
	    Double_t deltaR=100;
 	    Int_t closestJet=0;
 	    for (Int_t jjet = 0;  jjet< jets->GetEntries(); ++jjet){
	      ZeeDJet* tmpjet =dynamic_cast<ZeeDJet*>( jets->At(jjet) );
 	      if ( 0==tmpjet->GetEtaOrigin() && 0==tmpjet->GetPhiOrigin() && 0==tmpjet->GetMOrigin() ) continue;
	      TLorentzVector fvCStmp = tmpjet->GetFourVectorCS();
 	      Double_t deltaRtmp = std::sqrt( std::pow( tmpjet->GetEtaOrigin() - fvCStmp.Eta(),2 ) +  std::pow( tmpjet->GetPhiOrigin() - fvCStmp.Phi(),2 ) );
 	      if (deltaRtmp < deltaR){
 		deltaR=deltaRtmp;
 		closestJet=jjet;
 	      }
 	    }
	    
 	    ZeeDJet* tmpjet2 = dynamic_cast<ZeeDJet*>( jets->At(closestJet) );
	    EtaOrigin = tmpjet2->GetEtaOrigin();
	    PhiOrigin = tmpjet2->GetPhiOrigin();
	    MOrigin   = tmpjet2->GetMOrigin();
	  }
	  //cout << EtaOrigin << "   " << PhiOrigin << "   " << MOrigin << endl;
	  //cout << std::sqrt( std::pow( jet->GetEtaOrigin() - fvCS.Eta(),2 ) +  std::pow( jet->GetPhiOrigin() - fvCS.Phi(),2 ) ) <<endl;;
	  
	  // 	TLorentzVector jetFourVectorCal = fJetCalibrationTool->
	  // 	  ApplyJetAreaOffsetOriginEtaJES( fvCS.E(), fvCS.Eta(), EtaOrigin, PhiOrigin, MOrigin, 
	  // 					  areaPX, areaPY, areaPZ, areaE, rho,
	  // 					  aveMu, nPV );
	  
	  // 	  TLorentzVector jetFourVectorCal = fJetCalibrationTool->
	  // 	    ApplyJetAreaOffset( fvCS.E(), fvCS.Eta(),fvCS.Phi(),fvCS.M(),
	  // 				areaPX, areaPY, areaPZ, areaE, rho,
	  // 				aveMu, nPV );


	  TLorentzVector jetFourVectorCal = fJetCalibrationTool->
 	    ApplyJetAreaOffsetOriginEtaJESGSC( fvCS.E(), fvCS.Eta(), fvCS.Phi(), fvCS.M(),
            				       EtaOrigin, PhiOrigin, MOrigin,
 					       areaPX, areaPY, areaPZ, areaE, rho,
 					       trackWidth, nTrk, fTile0, fEM3, nSegments,
 					       aveMu, nPV 
 					       );

// 	  TLorentzVector jetFourVectorCal = fJetCalibrationTool->
//             ApplyJetAreaOffsetEtaJESGSC( fvCS.E(), fvCS.Eta(), fvCS.Phi(), fvCS.M(),
// 					 areaPX, areaPY, areaPZ, areaE, rho,
// 					 trackWidth, nTrk, fTile0, fEM3, nSegments,
// 					 aveMu, nPV
// 					 );


// 	    ApplyJetAreaOffsetEtaJES( fvCS.E(), fvCS.Eta(),fvCS.Phi(),fvCS.M(),
// 				      areaPX, areaPY, areaPZ, areaE, rho,
// 				      aveMu, nPV );
	  
	  //      TLorentzVector jetFourVectorCal = fJetCalibrationTool->
	  // 	ApplyOffsetEtaJES( fvCS.E(), fvCS.Eta(), EtaOrigin, PhiOrigin, MOrigin, aveMu, nPV );
	  
	  //	std::cout <<"\t\t\t *** "<<areaPX<<" "<<areaPY<<" "<<areaPZ<<" "<<areaE<< std::endl; 
	  
	  //	jet->Print();
	  
	  // jet->GetFourVector().Print();
// 	  std::cout << " etaOrigin = "<<EtaOrigin
// 		    << " phiOrigin = "<<PhiOrigin
// 		    << " masOrigin = "<<MOrigin 
// 		    << " mass CS = "<< jet->GetFourVectorCS().M()
// 		    <<" \n \t"
// 		    << " raw pt = "<<jet->GetFourVectorRaw().Pt()
// 		    << " cs pt = "<<jet->GetFourVectorCS().Pt()
// 		    << " cal pt = "<<jet->GetFourVector().Pt()
// 		    << " new cal pt = "<<jetFourVectorCal.Pt()
// 		    <<std::endl;
// 	  std::cout <<" areaE = "<< areaE
// 		    <<" areaX = "<< areaPX
// 		    <<" areaY = "<< areaPY
// 		    <<" areaZ = "<< areaPZ
// 		    <<std::endl;
// 	  std::cout <<" ntrk = "<< nTrk
// 		    <<" trackWidth = "<< trackWidth
// 		    <<" tileBar0 = "<< tileBar0
// 		    <<" tileExt0 = " <<tileExt0
// 		    <<" emb3 = " <<emb3
// 		    <<" eme3 = " <<eme3
// 		    <<" = " 
// 		    <<std::endl;
// 	  jetFourVectorCal.Print();

	  jet->SetFourVector(jetFourVectorCal);
	  
	  
	  // jet->Print();
	  //	jet->GetFourVectorCS().Print();
	  //	jet->GetFourVectorRAW().Print();
	  //std::cout<<"---------------"<<std::endl;

	  if ( (*fAnaOptions)->DoBCHCleaning() ){
	    if( event -> RunNumber().Get() != 0){

	      // std::cout<<"EN = "<<event -> EventNumber().Get() 
	      // 	     <<"; RN = "<<event -> RunNumber().Get() 
	      // 	     << "; LB = "<<event -> LB().Get()
	      // 	     <<"; jet eta = "<<jet->GetFourVector().Eta()
	      // 	     <<"; jet phi = "<<jet->GetFourVector().Phi()
	      // 	     <<"; jet BCH_CORR_CELL = "<<jet ->GetBCH_CORR_CELL()
	      // 	     <<"; jet emf = "<<jet ->GetEmFraction()
	      // 	     <<"; jet pt = "<<jet ->GetFourVector().Pt()
	      // 	     <<std::endl;
	    
	      Bool_t BadMediumBCH = fBCHCleaningTool -> IsBadMediumBCH( event -> RunNumber().Get(), event -> LB().Get(), jet->GetFourVector().Eta(), jet->GetFourVector().Phi(), jet ->GetBCH_CORR_CELL(), jet ->GetEmFraction(), jet->GetFourVector().Pt());
	      Bool_t BadTightBCH = fBCHCleaningTool -> IsBadTightBCH( event -> RunNumber().Get(), event -> LB().Get(), jet->GetFourVector().Eta(), jet->GetFourVector().Phi(), jet ->GetBCH_CORR_CELL(), jet ->GetEmFraction(), jet->GetFourVector().Pt()); 

	      jet -> SetIsBadMediumBCH(BadMediumBCH);
	      jet -> SetIsBadTightBCH(BadTightBCH);
	    }
	  }


	}
      
      jets->Sort();
      
      //  jets->Print("all");
      
      //std::cout<<"######################################"<<std::endl;
  }  // collections loop

  //Info(__PRETTY_FUNCTION__," stop2");



  Mu  = event->AverageInteractionsPerCrossing().Get();
  Nvtx = event->Nvtx().Get();

  
  if ((*fAnaOptions)->EvaluateSystematics()){
    for (unsigned int i = 0; i < collections.size(); i++) {
      std::string collName = collections[ i ]; 
      if ( collName.find("Truth") != std::string::npos ) continue;

      fJetMultiJESUncTool = fJetMultiJESUncToolMap[ collName ];
      if ( (fJetMultiJESUncTool == NULL) && (collName.find("Truth") == std::string::npos)) {
	Error("ZeeDCalcJets::Calculate", "Jet uncertainties tool is not configured ");
	Error("ZeeDCalcJets::Calculate",  collName.c_str());
	gSystem->Exit(EXIT_FAILURE);
      }

      TObjArray* jets = event -> GetJets( collName, kFALSE );
      if ( ( jets == NULL) ) continue;

      for (Int_t ijet = 0;  ijet < jets -> GetEntries(); ++ijet)
	{
	  ZeeDJet* jet = dynamic_cast<ZeeDJet*>( jets->At(ijet) ) ;

	  TLorentzVector fourVector = jet -> GetFourVector();

	  this -> provideUncertainties( jet );
	  jet -> SetFourVector(calculateUncertainties( fourVector ));

	  m_jesUncertainty.clear();
	  jets->Sort();
	}
 
    }
  }

  // Info(__PRETTY_FUNCTION__," stop");

  fJetCalibrationTool = NULL;
  fJetMultiJESUncTool = NULL;
  DEBUG_MES_END;
  
  return;
}

//------------------------------------------------------

//void ZeeDCalcJets::provideUncertainties(TLorentzVector jetFourVector){
void ZeeDCalcJets::provideUncertainties(ZeeDJet *Jet){
  TLorentzVector jetFourVector = Jet->GetFourVector();

  for (int icomp = 0;icomp < Ncomp; ++icomp) 
    {
      m_jesUncertainty.push_back(fJetMultiJESUncTool->getRelUncertComponent(icomp, jetFourVector.Pt(), jetFourVector.Eta()));
    }
  OffsetMuTerm = fJetMultiJESUncTool->getRelMuOffsetTerm(jetFourVector.Pt(), jetFourVector.Eta(), Mu );
  OffsetNPVTerm = fJetMultiJESUncTool->getRelNPVOffsetTerm(jetFourVector.Pt(), jetFourVector.Eta(), Nvtx );

  PtTerm= fJetMultiJESUncTool->getRelPileupPtTerm(jetFourVector.Pt(), jetFourVector.Eta(), Nvtx, Mu); 
  RhoTopo = fJetMultiJESUncTool->getRelPileupRhoTopology(jetFourVector.Pt(), jetFourVector.Eta()); 

  FlavourComp = fJetMultiJESUncTool->getRelFlavorCompUncert(jetFourVector.Pt(), jetFourVector.Eta(), true);
  FlavourResponce = fJetMultiJESUncTool->getRelFlavorResponseUncert(jetFourVector.Pt(), jetFourVector.Eta());
 
  PunchThrough=fJetMultiJESUncTool->getRelPunchThroughUncert(jetFourVector.Pt(), jetFourVector.Eta(),Jet->GetNSegments() );

}

//------------------------------------------------------
TLorentzVector ZeeDCalcJets::calculateUncertainties(TLorentzVector jetFourVector){
  
  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetJES_P1_Up)) {
    //jetFourVector.SetPerp((jetFourVector.Pt())*(1.0 + m_jesUncertainty[ 0 ] ));
    jetFourVector*=(1.0 + m_jesUncertainty[ 0 ]);
    return jetFourVector;
  }
  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetJES_P1_Down)) {
    jetFourVector*=(1.0 - m_jesUncertainty[ 0 ]);
    return jetFourVector;
  }

  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetJES_P2_Up)) {
    jetFourVector*=(1.0 + m_jesUncertainty[ 1 ]);
    return jetFourVector;
  }
  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetJES_P2_Down)) {
    jetFourVector*=(1.0 - m_jesUncertainty[ 1 ]);
    return jetFourVector;
  }

  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetJES_P3_Up)) {
    jetFourVector*=(1.0 + m_jesUncertainty[ 2 ]);
    return jetFourVector;	    
  }
  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetJES_P3_Down)) {
    jetFourVector*=(1.0 - m_jesUncertainty[ 2 ]);
    return jetFourVector;	    
  }

  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetJES_P4_Up)) {
    jetFourVector*=(1.0 + m_jesUncertainty[ 3 ]);
    return jetFourVector;	    
  }
  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetJES_P4_Down)) {
    jetFourVector*=(1.0 - m_jesUncertainty[ 3 ]);
    return jetFourVector;	    
  }

  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetJES_P5_Up)) {
    jetFourVector*=(1.0 + m_jesUncertainty[ 4 ]);
    return jetFourVector;	    
  }
  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetJES_P5_Down)) {
    jetFourVector*=(1.0 - m_jesUncertainty[ 4 ]);
    return jetFourVector;	    
  }

  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetJES_P6_Up)) {
    jetFourVector*=(1.0 + m_jesUncertainty[ 5 ]);
    return jetFourVector;	    
  }
  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetJES_P6_Down)) {
    jetFourVector*=(1.0 - m_jesUncertainty[ 5 ]);
    return jetFourVector;	    
  }

  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetJES_P7_Up)) {
    jetFourVector*=(1.0 + m_jesUncertainty[ 6 ]);
    return jetFourVector;	    
  }
  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetJES_P7_Down)) {
    jetFourVector*=(1.0 - m_jesUncertainty[ 6 ]);
    return jetFourVector;	    
  }

  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetJES_P8_Up)) {
    jetFourVector*=(1.0 + m_jesUncertainty[ 7 ]);
    return jetFourVector;	    
  }
  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetJES_P8_Down)) {
    jetFourVector*=(1.0 - m_jesUncertainty[ 7 ]);
    return jetFourVector;	    
  }


  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetJES_P9_Up)) {
    jetFourVector*=(1.0 + m_jesUncertainty[ 8 ]);
    return jetFourVector;	    
  }
  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetJES_P9_Down)) {
    jetFourVector*=(1.0 - m_jesUncertainty[ 8 ]);
    return jetFourVector;	    
  }

  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetJES_P10_Up)) {
    jetFourVector*=(1.0 + m_jesUncertainty[ 9 ]);
    return jetFourVector;	    
  }
  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetJES_P10_Down)) {
    jetFourVector*=(1.0 - m_jesUncertainty[ 9 ]);
    return jetFourVector;	    
  }

  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetPU_OffsetMu_Up)) {
    jetFourVector*=(1.0 + OffsetMuTerm );
    return jetFourVector;	    
  }
  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetPU_OffsetMu_Down)) {
    jetFourVector*=(1.0 - OffsetMuTerm );
    return jetFourVector;	    
  }

  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetPU_OffsetNPV_Up)) {
    jetFourVector*=(1.0 + OffsetNPVTerm );
    return jetFourVector;	    
  }
  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetPU_OffsetNPV_Down)) {
    jetFourVector*=(1.0 - OffsetNPVTerm );
    return jetFourVector;	    
  }

  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetPU_PtTerm_Up)) {
    jetFourVector*=(1.0 + PtTerm );
    return jetFourVector;	    
  }
  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetPU_PtTerm_Down)) {
    jetFourVector*=(1.0 - PtTerm );
    return jetFourVector;	    
  }

  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetPU_Rho_Up)) {
    jetFourVector*=(1.0 + RhoTopo );
    return jetFourVector;   
  }
  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetPU_Rho_Down)) {
    jetFourVector*=(1.0 - RhoTopo );
    return jetFourVector;   
  }
    
  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetFlavor_Comp_Up)) {
    jetFourVector*=(1.0 + FlavourComp );
    return jetFourVector;	    
  }
  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetFlavor_Comp_Down)) {
    jetFourVector*=(1.0 - FlavourComp );
    return jetFourVector;	    
  }

  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetFlavor_Response_Up)) {
    jetFourVector*=(1.0 + FlavourResponce );
    return jetFourVector;	    
  }
  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetFlavor_Response_Down)) {
    jetFourVector*=(1.0 - FlavourResponce );
    return jetFourVector;	    
  }
  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetPunchThrough_Up)) {
    jetFourVector*=(1.0 + PunchThrough );
    return jetFourVector;	    
  }
  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetPunchThrough_Down)) {
    jetFourVector*=(1.0 - PunchThrough );
    return jetFourVector;	    
  }
  //JER
  if (fSys->isShiftInUse(ZeeDSystematics::eZplusJetJER)) {
    fJetSmearTool -> SmearJet_Syst( jetFourVector );
    return jetFourVector;
  }

  return jetFourVector;
}




Int_t ZeeDCalcJets::findMuspContainer(float jet_eta, float jet_phi ,  const TObjArray* muonSpShowers) 
{
  double delR = 0;
  double delta_phi;
  double delta_eta;
  

  // Match mspn container to jet delR<0.4
  delR = 100;
  Int_t index_musp=-1;
  UInt_t muspSize( muonSpShowers->GetEntries() );

  
  for (unsigned int i = 0; i < muspSize; i++) 
    {
      const ZeeDMuonSpShower* mspsh = (const ZeeDMuonSpShower*)muonSpShowers->At(i);

      delta_phi = fabs(jet_phi - mspsh->fphi); //calculate the distance in phi
      if (delta_phi > TMath::Pi()) delta_phi = (2.0 * TMath::Pi()) - delta_phi; // always take the smaller angle (below 180°)
      delta_eta = jet_eta - mspsh->feta; // distance in eta
      if (TMath::Sqrt( TMath::Power(delta_phi,2) + TMath::Power(delta_eta,2) ) < delR ) 
	{
	  delR = sqrt( pow(delta_phi,2) + pow(delta_eta,2));
	  index_musp = i;
	}
    }

  if (delR < 0.4)
    {
      return index_musp;
    }
  return -1;
}
