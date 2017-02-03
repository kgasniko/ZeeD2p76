#include "ZeeDHistManager/ZeeDHistManagerZjet.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDJet.h"
#include "ZeeDEvent/ZeeDTrack.h"

#include "ZeeDTools/ZeeDBags.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

//#define USE_DEBUG_MESSAGES
//#define DEBUG_ON 1
#include "ZeeDMisc/ZeeDMisc.h"

using std::cout;
using std::endl;

//-----------------------------------------------------
void  ZeeDHistManagerZjet::bookAnalysisCuts()
{
  //std::cout <<__PRETTY_FUNCTION__<< std::endl;
  jetPtCut             = 10.0;
  subJetPtCut          = 10.0;
  
  jetJVFCut            = 0.25;
  subJetJVFCut         = 0.25;
  
  deltaJetPtCut        = 2.;
  deltaJVFCut          = 0.10;
  
  bosonPtCut           =  1.0;
  trackingAcceptance   =  2.5;
  
  deltaRCutMap.insert( std::pair<std::string,Double_t>("4" , 0.350) ) ;
  deltaRCutMap.insert( std::pair<std::string,Double_t>("5" , 0.425) ) ;
  deltaRCutMap.insert( std::pair<std::string,Double_t>("6" , 0.500) ) ;
  
  return;
}

void ZeeDHistManagerZjet::BookHistos()
{
  bookAnalysisCuts();
  
    // Do we run on data or MC?
    if ( (*fAnaOptions)->IsMC() ) {
        bIsMC = kTRUE;
    }
    // Books histograms

    const TString ZjetBosPt     = ZeeDMisc::FindFile("Binning/ZjetBosPt.txt");
    
    std::vector<std::string> collections = (*fAnaOptions)->JetCollectionNames();
    for (unsigned int i = 0; i < collections.size(); i++)
      {
	AddTH1D(collections[i] + "_Jets_BosPt",   501,0,500,  "P_{T} (Z->ee) / GeV", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_Jets_BosY",    51,-5,5,     "Y (Z->ee) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_Jets_BosPhi",  180, -180, 180,  "#Phi (Z->ee) / [deg]", "", ZeeDHistLevel::Analysis);
	
	AddTH1D(collections[i] + "_PtJet" ,  1001, -0.5, 500.5,  "P_{T} (jet) / GeV", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_PtJet1",  1001, -0.5, 500.5,  "P_{T} (jet_{1}) / GeV", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_PtJet2",  1001, -0.5, 500.5,  "P_{T} (jet_{2}) / GeV", "", ZeeDHistLevel::Analysis);
	
	AddTH1D(collections[i] + "_EMFJet",    321, -1.1, 2.1,  "EMF (jet)"    , "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_EMFJet1",   321, -1.1, 2.1,  "EMF (jet_{1}) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_EMFJet2",   321, -1.1, 2.1,  "EMF (jet_{2})" , "", ZeeDHistLevel::Analysis);
	
	AddTH1D(collections[i] + "_JVFJet",    321, -1.1, 2.1,  "JVF (jet)"    , "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_JVFJet1",   321, -1.1, 2.1,  "JVF (jet_{1})", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_JVFJet2",   321, -1.1, 2.1,  "JVF (jet_{2})", "", ZeeDHistLevel::Analysis);
	
	AddTH1D(collections[i] + "_YDetJet",    51, -5, 5,  "#eta (jet) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_YDetJet1",   51, -5, 5,  "#eta (jet_{1}) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_YDetJet2",   51, -5, 5,  "#eta (jet_{2}) ", "", ZeeDHistLevel::Analysis);
	
	AddTH1D(collections[i] + "_YJet",    51, -5, 5,  "y (jet) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_YJet1",   51, -5, 5,  "y (jet_{1}) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_YJet2",   51, -5, 5,  "y (jet_{2}) ", "", ZeeDHistLevel::Analysis);
	
	AddTH1D(collections[i] + "_PhiJet",    180, -180, 180,  "#Phi (jet) [deg] ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_PhiJet1",   180, -180, 180,  "#Phi (jet_{1}) [deg] ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_PhiJet2",   180, -180, 180,  "#Phi (jet_{2}) [deg]", "", ZeeDHistLevel::Analysis);
	
	AddTH1D(collections[i] + "_PhiDetJet",    180, -180, 180,  "#Phi (jet) [deg] ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_PhiDetJet1",   180, -180, 180,  "#Phi (jet_{1}) [deg] ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_PhiDetJet2",   180, -180, 180,  "#Phi (jet_{2}) [deg]", "", ZeeDHistLevel::Analysis);
	
	AddTH1D(collections[i] + "_DeltaPhiJetZ",       180, -180, 180,  "#Delta#Phi (jet-Z) [deg] ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaPhiJet1Z",      180, -180, 180,  "#Delta#Phi (jet_{1}-Z) [deg] ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaPhiJet2Z",      180, -180, 180,  "#Delta#Phi (jet_{2}-Z) [deg] ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaPhiJet1Jet2",   180, -180, 180,  "#Delta#Phi (jet_{1}-jet_{2}) [deg]", "", ZeeDHistLevel::Analysis);

// 	AddTH1D(collections[i] + "_DeltaPttool",  200, -100, 100,  "#Delta p_{T} [GeV]", "", ZeeDHistLevel::Analysis);
// 	AddTH1D(collections[i] + "_DeltaEtool",   200, -100, 100,  "#Delta E [GeV]", "", ZeeDHistLevel::Analysis);
// 	AddTH1D(collections[i] + "_DeltaMtool",   200, -100, 100,  "#Delta M [GeV]", "", ZeeDHistLevel::Analysis);
// 	AddTH1D(collections[i] + "_DeltaEtatool",  200, -100, 100,  "#Delta #eta [GeV]", "", ZeeDHistLevel::Analysis);
	
	AddTH1D(collections[i] + "_DeltaRJetElec1",  314, 0, 3.14,  "#Delta R (jet-ele_{1}) "    , "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaRJetElec2",  314, 0, 3.14,  "#Delta R (jet-ele_{2})"     , "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaRJet1Elec1", 314, 0, 3.14,  "#Delta R (jet_{1}-ele_{1}) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaRJet1Elec2", 314, 0, 3.14,  "#Delta R (jet_{1}-ele_{2}) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaRJet2Elec1", 314, 0, 3.14,  "#Delta R (jet_{2}-ele_{1}) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaRJet2Elec2", 314, 0, 3.14,  "#Delta R (jet_{2}-ele_{2}) ", "", ZeeDHistLevel::Analysis);
	
	const TString ZjetBosPtBalance      = ZeeDMisc::FindFile("Binning/ZjetBosPtBalance.txt");
	const TString ZjetBosPtBalanceDPhi  = ZeeDMisc::FindFile("Binning/ZjetBosPtBalanceDPhi.txt");
	const TString ZjetDeltaPtNvtxMu     = ZeeDMisc::FindFile("Binning/ZjetDeltaPtNvtxMu.txt");
	const TString ZjetTrackPt           = ZeeDMisc::FindFile("Binning/ZjetTrackPt.txt");
	const TString ZjetJetEta            = ZeeDMisc::FindFile("Binning/ZjetJetEta.txt");


	
	//	AddTH2D(collections[i]  + "_BalanceSimple" , ZjetBosPtBalance, "P_{T} (ref) / GeV", "balance", ZeeDHistLevel::Analysis);
	//AddTH2D(collections[i]  + "_BalanceCosPhi" , ZjetBosPtBalance, "P_{T} (ref) / GeV", "balance", ZeeDHistLevel::Analysis);
	//	AddTH2D(collections[i]  + "_BalanceAllJets", ZjetBosPtBalance, "P_{T} (ref) / GeV", "balance", ZeeDHistLevel::Analysis);
	//	AddTH3D(collections[i]  + "_BalanceDPhiSimple" , ZjetBosPtBalanceDPhi, "P_{T} (ref) / GeV", "balance", "#pi - #Delta #varphi", ZeeDHistLevel::Analysis);
	//AddTH3D(collections[i]  + "_BalanceDPhiCosPhi" , ZjetBosPtBalanceDPhi, "P_{T} (ref) / GeV", "balance", "#pi - #Delta #varphi", ZeeDHistLevel::Analysis);
	//	AddTH3D(collections[i]  + "_BalanceDPhiAllJets", ZjetBosPtBalanceDPhi, "P_{T} (ref) / GeV", "balance", "#pi - #Delta #varphi", ZeeDHistLevel::Analysis);
	
	//	AddTH3DVector(collections[i]  + "_BalanceDPhiEtaSimple" , ZjetBosPtBalanceDPhi, ZjetJetEta, "P_{T} (ref) / GeV", "balance", "#pi - #Delta #varphi", "#eta_{jet1}", ZeeDHistLevel::Analysis);

	AddTH3DVector(collections[i]  + "_BalanceDPhiEtaCosPhi" , ZjetBosPtBalanceDPhi, ZjetJetEta, "P_{T} (ref) / GeV", "balance", "#pi - #Delta #varphi", "#eta_{jet1}", ZeeDHistLevel::Analysis);
	
	if ( !((*fAnaOptions)->EvaluateSystematics()) ||  fSys->isShiftInUse(ZeeDSystematics::eNoShift) )
	  {
	    
	    
	    AddTH3DVector(collections[i]  + "_CorrDeltaJet1PtBosPtvsNvtxMuPtRefEta21"   , ZjetDeltaPtNvtxMu, ZjetBosPt, "P_{T} jet1 - P_{T} Z/ GeV", "Nvtx", "#mu", "P_{T} (Z->ee) / GeV",  ZeeDHistLevel::Analysis);
	    AddTH3DVector(collections[i]  + "_CorrDeltaJet1PtBosPtvsNvtxMuPtRefEta28"   , ZjetDeltaPtNvtxMu, ZjetBosPt, "P_{T} jet1 - P_{T} Z/ GeV", "Nvtx", "#mu", "P_{T} (Z->ee) / GeV",  ZeeDHistLevel::Analysis);
	    AddTH3DVector(collections[i]  + "_CorrDeltaJet1PtBosPtvsNvtxMuPtRefEta45"   , ZjetDeltaPtNvtxMu, ZjetBosPt, "P_{T} jet1 - P_{T} Z/ GeV", "Nvtx", "#mu", "P_{T} (Z->ee) / GeV",  ZeeDHistLevel::Analysis);
	    
	    if ( (*fAnaOptions)->IsMC() && (collections[i].find("Truth") == std::string::npos ) )
	      {
		AddTH3DVector(collections[i]  + "_CorrDeltaJet1PtTruthPtvsNvtxMuPtRefEta21"   , ZjetDeltaPtNvtxMu, ZjetBosPt, "P_{T} jet1 - P_{T} Truth/ GeV", "Nvtx", "#mu", "P_{T} (Truth) / GeV",  ZeeDHistLevel::Analysis);
		AddTH3DVector(collections[i]  + "_CorrDeltaJet1PtTruthPtvsNvtxMuPtRefEta28"   , ZjetDeltaPtNvtxMu, ZjetBosPt, "P_{T} jet1 - P_{T} Truth/ GeV", "Nvtx", "#mu", "P_{T} (Truth) / GeV",  ZeeDHistLevel::Analysis);
		AddTH3DVector(collections[i]  + "_CorrDeltaJet1PtTruthPtvsNvtxMuPtRefEta45"   , ZjetDeltaPtNvtxMu, ZjetBosPt, "P_{T} jet1 - P_{T} Truth/ GeV", "Nvtx", "#mu", "P_{T} (Truth) / GeV",  ZeeDHistLevel::Analysis);
		
		AddTH3DVector(collections[i]  + "_CorrDeltaJet1PtMatchedBosPtvsNvtxMuPtRefEta21"   , ZjetDeltaPtNvtxMu, ZjetBosPt, "P_{T} jet1 - P_{T} Z/ GeV", "Nvtx", "#mu", "P_{T} (Z->ee) / GeV",  ZeeDHistLevel::Analysis);
		AddTH3DVector(collections[i]  + "_CorrDeltaJet1PtMatchedBosPtvsNvtxMuPtRefEta28"   , ZjetDeltaPtNvtxMu, ZjetBosPt, "P_{T} jet1 - P_{T} Z/ GeV", "Nvtx", "#mu", "P_{T} (Z->ee) / GeV",  ZeeDHistLevel::Analysis);
		AddTH3DVector(collections[i]  + "_CorrDeltaJet1PtMatchedBosPtvsNvtxMuPtRefEta45"   , ZjetDeltaPtNvtxMu, ZjetBosPt, "P_{T} jet1 - P_{T} Z/ GeV", "Nvtx", "#mu", "P_{T} (Z->ee) / GeV",  ZeeDHistLevel::Analysis);
		
	      }
	    
	    const TString  ZjetTrackPtBosonPtJetEta     = ZeeDMisc::FindFile("Binning/ZjetTrackPtBosonPtJetEta.txt");
	    const TString  ZjetTrackDeltaRBosonPtJetEta = ZeeDMisc::FindFile("Binning/ZjetTrackDeltaRBosonPtJetEta.txt");
	    
	    AddTH1D(collections[i] +  "_TrackPt", ZjetTrackPt, "p_{T}^{track}/GeV ", "", ZeeDHistLevel::Analysis);
	    
	    AddTH1D(collections[i] +  "_TrackSCTHits", 51,0,50, " SCT hits ", "", ZeeDHistLevel::Analysis);
	    AddTH1D(collections[i] +  "_TrackPixelHits",51,0,50, " Pixel hits ", "", ZeeDHistLevel::Analysis);
	    AddTH1D(collections[i] +  "_TrackD0", 201,0, 2, "track D0", "", ZeeDHistLevel::Analysis);
	    AddTH1D(collections[i] +  "_TrackZ0", 201,0, 2, " track Z0 *sin#theta", "", ZeeDHistLevel::Analysis);
	    AddTH1D(collections[i] +  "_TrackNDF", 101,0,100, " NoF ", "", ZeeDHistLevel::Analysis);
	    AddTH1D(collections[i] +  "_TrackChi2NDF", 101,0,10, "#chi^{2}/NoF ", "", ZeeDHistLevel::Analysis);
	    

	    AddTH3D(collections[i]  + "_TrackPtVsBosonPtJetEta"   ,ZjetTrackPtBosonPtJetEta, "P_{T}^{track}/ GeV", "p_T(ref)/GeV", "#eta_{jet1}",  ZeeDHistLevel::Analysis);

// 	    AddTH3D(collections[i]  + "_TrackDeltaRVsBosonPtJetEta"   ,ZjetTrackDeltaRBosonPtJetEta, "#Delta R(track-jet)", "p_T(ref)/GeV", "#eta_{jet1}",  ZeeDHistLevel::Analysis);
// 	    AddTH3D(collections[i]  + "_TrackDeltaRVsBosonPtJetEtaWeights"   ,ZjetTrackDeltaRBosonPtJetEta, "#Delta R(track-jet)", "p_T(ref)/GeV", "#eta_{jet1}",  ZeeDHistLevel::Analysis);

	    AddTProfile3D(collections[i]  + "_TrackDeltaRVsBosonPtJetEta"   ,ZjetTrackDeltaRBosonPtJetEta, "#Delta R(track-jet)", "p_T(ref)/GeV", "#eta_{jet1}",  ZeeDHistLevel::Analysis );

	    const TString ZjetRhoScaleJetEta = ZeeDMisc::FindFile("Binning/ZjetRhoScaleJetEta.txt");


	    AddTH3D(collections[i]  + "_RhoEMScalePtEta"   ,     ZjetRhoScaleJetEta, "#rho_{EM}/ GeV", "p_{T} jet1/GeV", " #eta_{jet1}",  ZeeDHistLevel::Analysis);	    
	    AddTH3D(collections[i]  + "_RhoEMScaleMassPtEta"   , ZjetRhoScaleJetEta, "#rho_{EM}/ GeV", "p_{T} jet1 +m_{Z} /GeV", " #eta_{jet1}",  ZeeDHistLevel::Analysis);	    
	    AddTH3D(collections[i]  + "_RhoLCScalePtEta"   ,     ZjetRhoScaleJetEta, "#rho_{EM}/ GeV", "p_{T} jet1/GeV", " #eta_{jet1}",  ZeeDHistLevel::Analysis);	    
	    AddTH3D(collections[i]  + "_RhoLCScaleMassPtEta"   , ZjetRhoScaleJetEta, "#rho_{EM}/ GeV", "p_{T} jet1 +m_{Z} /GeV", " #eta_{jet1}",  ZeeDHistLevel::Analysis);	    

	    /*

	    
	    AddTH2D(collections[i]  + "_CorrJet1PtvsMult",    31,0,30, 300,0,300,  " N_{jets}", "P_{T} jet/GeV", ZeeDHistLevel::Analysis);

	    AddTH2D(collections[i]  + "_CorrJetPtvsZ",        300,0,300, 300,0,300, "P_{T} (Z->ee) / GeV", "P_{T} jet/GeV", ZeeDHistLevel::Analysis);
	    AddTH2D(collections[i]  + "_CorrJet1PtvsZ",       300,0,300, 300,0,300, "P_{T} (Z->ee) / GeV", "P_{T} jet1/GeV", ZeeDHistLevel::Analysis);
	    AddTH2D(collections[i]  + "_CorrJet2PtvsZ",       300,0,300, 300,0,300, "P_{T} (Z->ee) / GeV", "P_{T} jet2/GeV", ZeeDHistLevel::Analysis);
	    AddTH2D(collections[i]  + "_CorrJet1PtvsJet2Pt",  300,0,300, 300,0,300, "P_{T} jet1 / GeV", "P_{T} jet2/GeV", ZeeDHistLevel::Analysis);


	    AddTH2D(collections[i]  + "_CorrJetJVFvsZ",       300,0,300, 110,-1.05,1.05, "P_{T} (Z->ee) / GeV", "JVF all", ZeeDHistLevel::Analysis);
	    AddTH2D(collections[i]  + "_CorrJet1JVFvsZ",      300,0,300, 110,-1.05,1.05, "P_{T} (Z->ee) / GeV", "JVF jet1", ZeeDHistLevel::Analysis);
	    AddTH2D(collections[i]  + "_CorrJet2JVFvsZ",      300,0,300, 110,-1.05,1.05, "P_{T} (Z->ee) / GeV", "JVF jet2", ZeeDHistLevel::Analysis);

	    AddTH2D(collections[i]  + "_CorrJetRvsZ",       300,0,300, 81, 0, 8.00, "P_{T} (Z->ee) / GeV", "#Delta R",      ZeeDHistLevel::Analysis);
	    AddTH2D(collections[i]  + "_CorrJet1RvsZ",      300,0,300, 81, 0, 8.00, "P_{T} (Z->ee) / GeV", "#Delta R jet1", ZeeDHistLevel::Analysis);
	    AddTH2D(collections[i]  + "_CorrJet2RvsZ",      300,0,300, 81, 0, 8.00, "P_{T} (Z->ee) / GeV", "#Delta R jet2", ZeeDHistLevel::Analysis);

	    AddTH2D(collections[i]  + "_CorrJet1RvsJet1Pt",      300,0,300, 81, 0, 8.00, "P_{T} jet1 / GeV", "#Delta R jet1", ZeeDHistLevel::Analysis);
	    AddTH2D(collections[i]  + "_CorrJet2RvsJet2Pt",      300,0,300, 81, 0, 8.00, "P_{T} jet2 / GeV", "#Delta R jet2", ZeeDHistLevel::Analysis);
	    */
	    AddTH2D(collections[i]  + "_CorrJetPtAllvsZ",        300,0,300, 300,0,300, "P_{T} (Z->ee) / GeV", "P_{T} all jets/GeV"    , ZeeDHistLevel::Analysis);
	    AddTH2D(collections[i]  + "_CorrJetPtAllFvsZ",       300,0,300, 110,0,10, "P_{T} (Z->ee) / GeV", "P_{T} jets/ P_{T} jet1" , ZeeDHistLevel::Analysis);
	    /*
	    AddTH2D(collections[i]  + "_CorrJetPtAllvsJet1Pt",        300,0,300, 300,0,300, "P_{T} jet1 / GeV", "P_{T} all jets/GeV"    , ZeeDHistLevel::Analysis);
	    AddTH2D(collections[i]  + "_CorrJetPtAllFvsJet1Pt",       300,0,300, 110,0,10, "P_{T} jet1 / GeV", "P_{T} jets/ P_{T} jet1" , ZeeDHistLevel::Analysis);
	    */
	  }
      }

    const TString ZjetTrackDeltaR       = ZeeDMisc::FindFile("Binning/ZjetTrackDeltaR.txt");
    ZeeDBinGrid tmp_grid( "tempGrid" );
    tmp_grid.ReadFromFile(ZjetTrackDeltaR, kFALSE);   
    TObjArray* BinEdges = tmp_grid.GetBinEdges();  
    if ( BinEdges->GetEntriesFast() != 1 ) {
      Error("ZeeDHistManagerZjet::BookHistos()", "Wrong array dimension for histogram");
    }
    ZeeDArrayD* array_x = static_cast<ZeeDArrayD*>(BinEdges->At(0));
    const Double_t* bins = array_x -> GetArray();     
    for (int i = 0; i<= array_x -> GetMaxElement(); i++) deltaRBins.push_back( bins[i] );	 
    
    
    return;
    
}

//-----------------------------------------------------
void ZeeDHistManagerZjet::Fill()
{
    DEBUG_MES_START;
    DEBUG_MES(this->GetHMName());

    // Get event
    const ZeeDEvent* event = GetEvent();
    CHECK_NULL_PTR(event);

    // Event weight
    Double_t Weight = event->GetWeight();

    // Info(__PRETTY_FUNCTION__, TString::Format(" \n\n\n  NbEvt = %d  HM = %s ",event->EventNumber().Get(),this->GetHMName().Data() ) );


    // Arrange loop over bosons
    if (bIsAll) {
        // All bosons
        const TObjArray* bosons = (bIsW) ?  event->GetWBosons() : event->GetZBosons();

        if ( bosons != NULL ) {

	  for ( Int_t index = 0; index < bosons->GetEntriesFast(); ++index ) {
	    
	    const ZeeDBosonZ* boson = static_cast<ZeeDBosonZ*>( bosons->At(index) );
	    
	    bosonMomentum     = boson->GetFourVector();
	    Electron1Momentum = boson->GetFirstLep()->GetFourVector();
	    Electron2Momentum = boson->GetSecondLep()->GetFourVector();
	    
	    
	    std::vector<std::string> collections = (*fAnaOptions)->JetCollectionNames();
	    for (unsigned int i = 0; i < collections.size(); i++)
	      {
		fCollectionName = collections[i];
		TString _truthCollection( fCollectionName.c_str() );
		_truthCollection.ReplaceAll("TopoEM","Truth");
		_truthCollection.ReplaceAll("LCTopo","Truth");
		_truthCollection.ReplaceAll("New","");
		fTruthCollectionName = _truthCollection.Data();
		const TObjArray *jets = event->GetJets( fCollectionName, kFALSE); // don't warn if missing, just continue
		
		//  std::cout <<" \t\t 11 coll = "<< fCollectionName <<" tr = "<<fTruthCollectionName<<"  "<<__PRETTY_FUNCTION__<< std::endl;
		//  std::cout <<" \n\n \t\t coll = "<< fCollectionName << std::endl;
		if ( jets ) FillJetHists(jets, Weight);
	      }
	    
	  }
        }
	
    } else {
      // Selected boson only
      if (event->GetCurrentBoson() != NULL) {
	
	bosonMomentum     = event->GetCurrentBoson()->GetFourVector();
	Electron1Momentum = event->GetCurrentBoson()->GetFirstLep()->GetFourVector();
	Electron2Momentum = event->GetCurrentBoson()->GetSecondLep()->GetFourVector();
	
	std::vector<std::string> collections = (*fAnaOptions)->JetCollectionNames();
	for (unsigned int i = 0; i < collections.size(); i++)
	  {
	    fCollectionName = collections[i];
	    TString _truthCollection( fCollectionName.c_str() );
	    _truthCollection.ReplaceAll("TopoEM","Truth");
	    _truthCollection.ReplaceAll("LCTopo","Truth");  
	    _truthCollection.ReplaceAll("New","");
	    fTruthCollectionName = _truthCollection.Data();
	    
	    //std::cout <<" \t\t 22 coll = "<< fCollectionName <<" tr = "<<fTruthCollectionName<< " "<<__PRETTY_FUNCTION__<< std::endl;
	    // std::cout <<" \n\n \t\t coll = "<< fCollectionName << std::endl;
	    
	    const TObjArray *jets = event->GetJets( fCollectionName , kFALSE);
	    if ( jets )  FillJetHists( jets,  Weight);
	  }
	
      }

    }

    //    Info(__PRETTY_FUNCTION__, "------------------------------------------------------------" );

    DEBUG_MES_END;
}

//-----------------------------------------------------
//-----------------------------------------------------
//-----------------------------------------------------

void ZeeDHistManagerZjet::FillJetHists(const TObjArray* jets,  Double_t Weight )
{
  Bool_t _debug_ = false;
  Double_t bosonPt = bosonMomentum.Pt();
  if ( bosonPt < bosonPtCut ) return;


  //  Info(__PRETTY_FUNCTION__, TString::Format(" \t\t new event  collection = %s ", fCollectionName.c_str()) );

  const ZeeDEvent* event = GetEvent();
  CHECK_NULL_PTR(event);
  Int_t nvtx = event->Nvtx().Get();
  Double_t mu   = event->AverageInteractionsPerCrossing().Get();
  // the variable below is not in use for now
  //Double_t actmu   = event->ActualInteractionsPerCrossing().Get();

  Double_t rhoEM = event->GetEtDensity_4EM() ;
  Double_t rhoLC = event->GetEtDensity_4LC() ;

  //  if (mu >10) return;
  
  const TObjArray *tracks = event->GetTracks();

  //  -----------------------------------------
  // zero pointer to leading jets


  leadingJet          =  NULL; 
  subLeadingJet       =  NULL;
  isSubLeadingJetVeto =  true;

  leadingJetIndex= -1;
  subLeadingJetIndex= -1;

  //
  //-----------------------------------------

  
  ptReferenceCosPhi     = 0. ;
  ptBalanceCosPhi       = 0. ;
  leadingJetDeltaPhi    = 0. ;
  subLeadingJetDeltaPhi = 0. ;
  
  
  deltaRcut = 3.0;
  for (std::map<std::string,Double_t>::iterator itr = deltaRCutMap.begin() ; itr != deltaRCutMap.end(); ++itr)
    if (fCollectionName.find(itr->first) != std::string::npos) 
      {
	deltaRcut = itr->second;
	break;
      }
  
  _jetPtCut = jetPtCut;
  if ( fSys->isShiftInUse(ZeeDSystematics::eZjetJetPtCutUp) )
    _jetPtCut += deltaJetPtCut;
  if ( fSys->isShiftInUse(ZeeDSystematics::eZjetJetPtCutDown))
    _jetPtCut -= deltaJetPtCut;


  _jetJVFCut = jetJVFCut, _subJetJVFCut = subJetJVFCut;
  if ( fSys->isShiftInUse(ZeeDSystematics::eZjetJVFCutUp))
    {
      //      _jetJVFCut += deltaJVFCut;
      _subJetJVFCut += deltaJVFCut;
    }
  if ( fSys->isShiftInUse(ZeeDSystematics::eZjetJVFCutDown))
    {
      //      _jetJVFCut -= deltaJVFCut;
      _subJetJVFCut -= deltaJVFCut;
    }

  if ( _debug_)
    {
      std::cout <<"\t\t\t  "<<fCollectionName<<" deltaR = "<<deltaRcut<< std::endl;
      std::cout <<"\t\t\t  pt = "<<jetPtCut
		<<"  jvf def = "<<jetJVFCut<<" jvf cur = "<<_jetJVFCut
		<<"  jvf sub def = "<<subJetJVFCut<<" sub jvf cur"<<_subJetJVFCut
		<<" fSys = "<<fSys<<std::endl;
    }
  Int_t jetCount = 0;
  Double_t ptAll = 0;

  for (Int_t ijet = 0;  ijet< jets->GetEntries(); ++ijet)
    {

      ZeeDJet* jet =dynamic_cast< ZeeDJet* >( jets->At(ijet) ) ;
      CHECK_NULL_PTR( jet );
      
      if ( _debug_ )
	{
	  Info(__PRETTY_FUNCTION__,TString::Format(" ***** Looping jets %d",ijet));
	}

      TLorentzVector fourVector   = jet->GetFourVector();
      TLorentzVector fourVectorCS = jet->GetFourVectorCS();


      //      Double_t e         = fourVector.E();
      //      Double_t m         = fourVector.M();
      Double_t pt        = fourVector.Pt();
      //      Double_t eta       = fourVector.Eta();
      Double_t y         = fourVector.Rapidity();
      Double_t phi       = fourVector.Phi();
      Double_t deltaPhi  = fourVector.DeltaPhi( bosonMomentum );

      Double_t ydet      = fourVectorCS.Eta();
      Double_t phidet    = fourVectorCS.Phi();
      
      //   if ( pt < _jetPtCut ) continue;

      
      //      Info(__PRETTY_FUNCTION__," jet momentum is there");
      
      Double_t emf  = jet->GetEmFraction();
      Double_t jvf  = jet->GetJVF();
      
      //   Info(__PRETTY_FUNCTION__," jet jvf is there");
      
      Double_t deltaRElec1 = fourVector.DeltaR( Electron1Momentum );
      Double_t deltaRElec2 = fourVector.DeltaR( Electron2Momentum );
      
      Bool_t isIsolated = true;
      if ( ( deltaRElec1 <= deltaRcut ) || ( deltaRElec2 <= deltaRcut ) ) isIsolated = false;
      
      if ( !isIsolated ) continue;
      jetCount++;

      //  std::cout <<"\t\t\t ISOLATION !!! "<< std::endl; 

      if ( _debug_ )
	{
	  fourVector.Print();
	  std::cout <<"pt = "<<pt<<" y = "<<y<<" jvf = "<<jvf<<" ptAll = "<<ptAll<<std::endl;
	}
      
      if (  isLeadingJet( jet ) )
	{
	  leadingJetIndex= ijet;

	  leadingJetDeltaPhi    = leadingJet->GetFourVector().DeltaPhi( bosonMomentum ) ;
	  ptReferenceCosPhi     = bosonPt * TMath::Abs( TMath::Cos(leadingJetDeltaPhi) );
	  ptBalanceCosPhi       = leadingJet->GetFourVector().Pt()/ptReferenceCosPhi ;

	  _subJetPtCut = TMath::Max( _jetPtCut , 0.20 * ptReferenceCosPhi );
	  if ( fSys->isShiftInUse(ZeeDSystematics::eZjetSubJetPtCutUp))
	    _subJetPtCut = TMath::Max( 0.20 * _jetPtCut , 0.10 * ptReferenceCosPhi );
	  if ( fSys->isShiftInUse(ZeeDSystematics::eZjetSubJetPtCutDown))
	    _subJetPtCut +=  0.10 * ptReferenceCosPhi;
	  
	}
      
      if ( isSubLeadingJet( jet ) )
	{
	  subLeadingJetIndex= ijet;
	  subLeadingJetDeltaPhi = subLeadingJet->GetFourVector().DeltaPhi( bosonMomentum ) ;
	}

      if ( leadingJet && ijet !=  leadingJetIndex )
 	ptAll += pt * TMath::Abs( TMath::Cos(deltaPhi) );
      
      //      if (subLeadingJet) isSubLeadingJetVeto = true;

      //
      //  all jets
      //
      FillTH1( emf,   Weight, (fCollectionName + "_EMFJet").c_str());
      FillTH1( jvf,   Weight, (fCollectionName + "_JVFJet").c_str());
      
      FillTH1(pt,   Weight, (fCollectionName + "_PtJet").c_str());
      FillTH1(y,    Weight, (fCollectionName + "_YJet").c_str());
      FillTH1(ydet, Weight, (fCollectionName + "_YDetJet").c_str());
      FillTH1(     phi * ZeeDMisc::Rad2Deg,  Weight, (fCollectionName + "_PhiJet").c_str());
      FillTH1(  phidet * ZeeDMisc::Rad2Deg,  Weight, (fCollectionName + "_PhiDetJet").c_str());
      FillTH1(deltaPhi * ZeeDMisc::Rad2Deg,  Weight, (fCollectionName + "_DeltaPhiJetZ").c_str());
      
      FillTH1( deltaRElec1,  Weight, (fCollectionName + "_DeltaRJetElec1").c_str());
      FillTH1( deltaRElec2,  Weight, (fCollectionName + "_DeltaRJetElec2").c_str());
    
      if ( !((*fAnaOptions)->EvaluateSystematics()) ||  fSys->isShiftInUse(ZeeDSystematics::eNoShift) )
	{  
	  FillTH2( bosonPt, pt,  Weight, (fCollectionName + "_CorrJetPtvsZ").c_str());
	  FillTH2( bosonPt, jvf, Weight, (fCollectionName + "_CorrJetJVFvsZ").c_str() );
	  FillTH2( bosonPt, jet->GetFourVector().DeltaR(bosonMomentum) , Weight, (fCollectionName + "_CorrJetRvsZ").c_str() );
	}
      
    } // jet loop
  

  //
  //   jet loop is over here. Filling histograms...
  //
  if ( _debug_ )
    {
      if ( leadingJet != NULL )
	{
	  Info(" Zjet ","leading jet found");
	  leadingJet->Print();
	  if ( subLeadingJet != NULL )
	    {
	      Info(" Zjet ","sub leading jet found");   
	      subLeadingJet->Print();
	      if ( !isSubLeadingJetVeto )  
		Info(" Zjet ","sub leading jet veto FAILED"); 
		}
	}
    }
  
  if ( (NULL == leadingJet ) || !isSubLeadingJetVeto ) return;
  // if ( ptAll > 2. * leadingJet->GetFourVector().Pt() ) isSubLeadingJetVeto = false;
  
  if ( _debug_ )
    {
      Info("Zjet"," \t\t FILLING !!!!! ");
      std::cout <<" ptAll = " << ptAll<<" jet1 pt = "<<leadingJet->GetFourVector().Pt() <<" boson = "<< bosonPt << std::endl;
    }

  if ( 0&& ( ptAll/leadingJet->GetFourVector().Pt()  && ( bosonPt < 35.) ) )
    {
      
      Info(" TESTER START  ******** ", fCollectionName.c_str() );
      cout << " Boson pt = "<< bosonPt 
	   << " ele1 = "<<Electron1Momentum.Pt()<<" ele1 rap = "<<Electron1Momentum.Rapidity()
	   << " ele2 = "<<Electron2Momentum.Pt()<<" ele2 rap = "<<Electron2Momentum.Rapidity()
	   << " \n\n ptAll = "<< ptAll
	   << endl; 
      bosonMomentum.Print();

      cout <<" jet ********************** lead index = "<<leadingJetIndex<<" subindex = "<<subLeadingJetIndex<<endl; 
      leadingJet->Print();
      if ( isSubLeadingJetVeto && subLeadingJet )
	{  
	  subLeadingJet->Print();
	}

      Info(" TESTER reco ", "----------------------------------------------" );
      for (Int_t ijet = 0;  ijet< jets->GetEntries(); ++ijet)
	{
	  
	  ZeeDJet* jet =dynamic_cast< ZeeDJet* >( jets->At(ijet) ) ;
	  CHECK_NULL_PTR( jet );

	  Double_t deltaRElec1 = jet->GetFourVector().DeltaR( Electron1Momentum );
	  Double_t deltaRElec2 = jet->GetFourVector().DeltaR( Electron2Momentum );
	  Bool_t isIsolated = true;
	  if ( ( deltaRElec1 <= deltaRcut ) || ( deltaRElec2 <= deltaRcut ) ) isIsolated = false;
	  if ( !isIsolated ) continue;

	  cout <<" N :  "<<ijet
	       <<" pt = "<<jet->GetFourVector().Pt()
	       <<" y =  "<<jet->GetFourVector().Rapidity()
	       <<" jvf = "<<jet->GetJVF()
	       << endl;
	  isSubLeadingJet(jet, true);

	}
      
      
      Info(" TESTER truth ", fTruthCollectionName.c_str() );
      const TObjArray *truthJets = event->GetJets( fTruthCollectionName, kTRUE); // don't warn if missing, just continue
      if ( truthJets == NULL ) return;
      CHECK_NULL_PTR( truthJets );
      
      for (Int_t ijet = 0;  ijet< truthJets->GetEntries(); ++ijet)
	{
	  
	  ZeeDJet* jet =dynamic_cast< ZeeDJet* >( truthJets->At(ijet) ) ;
	  CHECK_NULL_PTR( jet );

	  Double_t deltaRElec1 = jet->GetFourVector().DeltaR( Electron1Momentum );
	  Double_t deltaRElec2 = jet->GetFourVector().DeltaR( Electron2Momentum );
	  Bool_t isIsolated = true;
	  if ( ( deltaRElec1 <= deltaRcut ) || ( deltaRElec2 <= deltaRcut ) ) isIsolated = false;
	  if ( !isIsolated ) continue;

	  cout <<" N :  "<<ijet
	       <<" pt = "<<jet->GetFourVector().Pt()
	       <<" y =  "<<jet->GetFourVector().Rapidity()
	       <<" jvf = "<<jet->GetJVF()
	       << endl;
	}
      Info(" TESTER STOP ", "----------------------------------------------\n\n\n" );
      
      
    }


  if ( (leadingJet != NULL)   && isSubLeadingJetVeto )
    {
      // balancing boson 
      FillTH1(bosonPt,                                 Weight, (fCollectionName + "_Jets_BosPt").c_str());
      FillTH1(bosonMomentum.Rapidity(),                Weight, (fCollectionName + "_Jets_BosY").c_str());
      FillTH1(bosonMomentum.Phi() * ZeeDMisc::Rad2Deg, Weight, (fCollectionName + "_Jets_BosPhi").c_str());
      
      
      
      FillTH1( leadingJet->GetEmFraction() ,  Weight, (fCollectionName + "_EMFJet1").c_str() );
      FillTH1( leadingJet->GetJVF()        ,  Weight, (fCollectionName + "_JVFJet1").c_str() );
      
      FillTH1(leadingJet->GetFourVector().Pt()      , Weight, (fCollectionName + "_PtJet1").c_str() );
      FillTH1(leadingJet->GetFourVector().Rapidity(), Weight, (fCollectionName + "_YJet1").c_str() );
      FillTH1(leadingJet->GetFourVectorCS().Eta()   , Weight, (fCollectionName + "_YDetJet1").c_str() );
      
      FillTH1(leadingJet->GetFourVector().Phi()   * ZeeDMisc::Rad2Deg,  Weight, (fCollectionName + "_PhiJet1").c_str() );
      FillTH1(leadingJet->GetFourVectorCS().Phi() * ZeeDMisc::Rad2Deg,  Weight, (fCollectionName + "_PhiDetJet1").c_str());
      FillTH1(leadingJetDeltaPhi * ZeeDMisc::Rad2Deg                 ,  Weight, (fCollectionName + "_DeltaPhiJet1Z").c_str());
      
      FillTH1( leadingJet->GetFourVector().DeltaR( Electron1Momentum ),  Weight, (fCollectionName + "_DeltaRJet1Elec1").c_str() );
      FillTH1( leadingJet->GetFourVector().DeltaR( Electron2Momentum ),  Weight, (fCollectionName + "_DeltaRJet1Elec2").c_str() );
      

      FillTH3( rhoEM, leadingJet->GetFourVector().Pt(),                     leadingJet->GetFourVectorCS().Eta(),  Weight, (fCollectionName + "_RhoEMScalePtEta").c_str() );
      FillTH3( rhoEM, leadingJet->GetFourVector().Pt() + bosonMomentum.M(), leadingJet->GetFourVectorCS().Eta(),  Weight, (fCollectionName + "_RhoEMScaleMassPtEta").c_str() );
      FillTH3( rhoLC, leadingJet->GetFourVector().Pt(),                     leadingJet->GetFourVectorCS().Eta(),  Weight, (fCollectionName + "_RhoLCScalePtEta").c_str() );
      FillTH3( rhoLC, leadingJet->GetFourVector().Pt() + bosonMomentum.M(), leadingJet->GetFourVectorCS().Eta(),  Weight, (fCollectionName + "_RhoLCScaleMassPtEta").c_str() );


//       if ( !((*fAnaOptions)->EvaluateSystematics()) ||  fSys->isShiftInUse(ZeeDSystematics::eNoShift) )
// 	{
// 	  FillTH2( bosonPt, leadingJet->GetFourVector().Pt() , Weight, (fCollectionName + "_CorrJet1PtvsZ").c_str() );
	  
// 	  if ( TMath::Abs( leadingJet->GetFourVectorCS().Eta() ) < trackingAcceptance )
// 	    FillTH2( bosonPt, leadingJet->GetJVF()             , Weight, (fCollectionName + "_CorrJet1JVFvsZ").c_str() );
	  
	  
// 	  FillTH2( bosonPt, leadingJet->GetFourVector().DeltaR( bosonMomentum) , Weight, (fCollectionName + "_CorrJet1RvsZ").c_str() );
	  
// 	  if ( bosonPt < 35 )
// 	    { 
// 	      FillTH2( leadingJet->GetFourVector().Pt(), leadingJet->GetFourVector().DeltaR( bosonMomentum) , Weight, (fCollectionName + "_CorrJet1RvsJet1Pt").c_str() );
// 	      FillTH2( leadingJet->GetFourVector().Pt(), ptAll, Weight, (fCollectionName + "_CorrJetPtAllvsJet1Pt").c_str() );
// 	      FillTH2( leadingJet->GetFourVector().Pt(), ptAll/leadingJet->GetFourVector().Pt() , Weight, (fCollectionName + "_CorrJetPtAllFvsJet1Pt").c_str() );
	      
// 	    }
	  
// 	  FillTH2( jetCount, leadingJet->GetFourVector().Pt(), Weight, (fCollectionName + "_CorrJet1PtvsMult").c_str() );
	  
 	  FillTH2( bosonPt, ptAll, Weight, (fCollectionName + "_CorrJetPtAllvsZ").c_str() );
 	  FillTH2( bosonPt, ptAll/leadingJet->GetFourVector().Pt() , Weight, (fCollectionName + "_CorrJetPtAllFvsZ").c_str() );
// 	}
    }
  
  if ( (subLeadingJet != NULL)  && isSubLeadingJetVeto )
    {
      FillTH1( subLeadingJet->GetEmFraction(), Weight, (fCollectionName + "_EMFJet2").c_str());
      FillTH1( subLeadingJet->GetJVF()       , Weight, (fCollectionName + "_JVFJet2").c_str());
      
      FillTH1(subLeadingJet->GetFourVector().Pt()                                          , Weight, (fCollectionName + "_PtJet2").c_str());
      FillTH1(subLeadingJet->GetFourVector().Rapidity()                                    , Weight, (fCollectionName + "_YJet2").c_str());
      FillTH1(subLeadingJet->GetFourVectorCS().Eta()                                       , Weight, (fCollectionName + "_YDetJet2").c_str());
      FillTH1(subLeadingJet->GetFourVector().Phi()   * ZeeDMisc::Rad2Deg                   , Weight, (fCollectionName + "_PhiJet2").c_str());
      FillTH1(subLeadingJet->GetFourVectorCS().Phi() * ZeeDMisc::Rad2Deg                   , Weight, (fCollectionName + "_PhiDetJet2").c_str());
      FillTH1(subLeadingJetDeltaPhi * ZeeDMisc::Rad2Deg                                    , Weight, (fCollectionName + "_DeltaPhiJet2Z").c_str());
      
      FillTH1(subLeadingJet->GetFourVector().DeltaPhi( leadingJet->GetFourVector() ) * ZeeDMisc::Rad2Deg,  Weight, (fCollectionName + "_DeltaPhiJet1Jet2").c_str());
      FillTH1(subLeadingJet->GetFourVector().DeltaR( Electron1Momentum ),  Weight, (fCollectionName + "_DeltaRJet2Elec1").c_str());
      FillTH1(subLeadingJet->GetFourVector().DeltaR( Electron2Momentum ),  Weight, (fCollectionName + "_DeltaRJet2Elec2").c_str());
      
//       if ( !((*fAnaOptions)->EvaluateSystematics()) ||  fSys->isShiftInUse(ZeeDSystematics::eNoShift) )
// 	{
// 	  FillTH2( bosonPt, subLeadingJet->GetFourVector().Pt() , Weight, (fCollectionName + "_CorrJet2PtvsZ").c_str() );
// 	  FillTH2( bosonPt, subLeadingJet->GetJVF()             , Weight, (fCollectionName + "_CorrJet2JVFvsZ").c_str() );
// 	  FillTH2( bosonPt, subLeadingJet->GetFourVector().DeltaR( bosonMomentum), Weight, (fCollectionName + "_CorrJet2RvsZ").c_str() );
// 	  FillTH2( leadingJet->GetFourVector().Pt() ,subLeadingJet->GetFourVector().Pt(), Weight, (fCollectionName + "_CorrJet1PtvsJet2Pt").c_str());
	  
	  
// 	  if (bosonPt < 35 )
// 	    { 
// 	      FillTH2( subLeadingJet->GetFourVector().Pt(), subLeadingJet->GetFourVector().DeltaR( bosonMomentum ) , Weight, (fCollectionName + "_CorrJet2RvsJet2Pt").c_str() );
// 	    }
// 	}

    }
  
  //------------------------------------------------------------------------------------------
  //
  //  balance   histograms
  //
  
  if ( (leadingJet != NULL) && isSubLeadingJetVeto )
    {

      
//       Double_t ptSubLeadingJetBalance = 0;
//       if ( subLeadingJet ) ptSubLeadingJetBalance = subLeadingJet->GetFourVector().Pt()/ ptReferenceCosPhi;
      
      FillTH3Vector( ptReferenceCosPhi, ptBalanceCosPhi, 
		     TMath::Pi() - TMath::Abs(leadingJetDeltaPhi), TMath::Abs(leadingJet->GetFourVectorCS().Eta()),  Weight, (fCollectionName + "_BalanceDPhiEtaCosPhi").c_str() );

//       FillTH3Vector( ptReferenceCosPhi, ptBalanceCosPhi, 
// 		     ptSubLeadingJetBalance, TMath::Abs(leadingJet->GetFourVectorCS().Eta()),  Weight, (fCollectionName + "_BalanceDPhiEtaCosPhi").c_str() );
      
      



      if ( !((*fAnaOptions)->EvaluateSystematics())  ||  fSys->isShiftInUse(ZeeDSystematics::eNoShift))  // study of deltapt *** not needed for systematics....
	{

	  if ( ( ptReferenceCosPhi > 17. ) )
	    {
	      // delta pt
	      if ( TMath::Abs(leadingJet->GetFourVectorCS().Eta()) < 2.1 )
		FillTH3Vector( leadingJet->GetFourVector().Pt() - ptReferenceCosPhi, nvtx, mu, ptReferenceCosPhi,  Weight, (fCollectionName + "_CorrDeltaJet1PtBosPtvsNvtxMuPtRefEta21").c_str() );
	      else if ( std::abs(leadingJet->GetFourVectorCS().Eta()) < 2.8 )
		FillTH3Vector( leadingJet->GetFourVector().Pt() - ptReferenceCosPhi, nvtx, mu, ptReferenceCosPhi,  Weight, (fCollectionName + "_CorrDeltaJet1PtBosPtvsNvtxMuPtRefEta28").c_str() );
	      else if ( std::abs(leadingJet->GetFourVectorCS().Eta()) < 4.5 )
		FillTH3Vector( leadingJet->GetFourVector().Pt() - ptReferenceCosPhi, nvtx, mu, ptReferenceCosPhi,  Weight, (fCollectionName + "_CorrDeltaJet1PtBosPtvsNvtxMuPtRefEta45").c_str() );
	      
	      if  ( (*fAnaOptions)->IsMC() && (fCollectionName.find("Truth")  == std::string::npos) )
		{
		  GetMatchedTruth( leadingJet ); 
		  if ( matchedTruthJet )
		    {
		      Double_t ptMatch = matchedTruthJet->GetFourVector().Pt();
		      
		      if ( TMath::Abs(leadingJet->GetFourVectorCS().Eta()) < 2.1 )
			FillTH3Vector( leadingJet->GetFourVector().Pt() - ptMatch, nvtx, mu, ptReferenceCosPhi,  Weight, (fCollectionName + "_CorrDeltaJet1PtTruthPtvsNvtxMuPtRefEta21").c_str() );
		      else if ( std::abs(leadingJet->GetFourVectorCS().Eta()) < 2.8 )
			FillTH3Vector( leadingJet->GetFourVector().Pt() - ptMatch, nvtx, mu, ptReferenceCosPhi,  Weight, (fCollectionName + "_CorrDeltaJet1PtTruthPtvsNvtxMuPtRefEta28").c_str() );
		      else if ( std::abs(leadingJet->GetFourVectorCS().Eta()) < 4.5 )
			FillTH3Vector( leadingJet->GetFourVector().Pt() -  ptMatch, nvtx, mu, ptReferenceCosPhi,  Weight, (fCollectionName + "_CorrDeltaJet1PtTruthPtvsNvtxMuPtRefEta45").c_str() );
		      
		      if ( TMath::Abs(leadingJet->GetFourVectorCS().Eta()) < 2.1 )
			FillTH3Vector( leadingJet->GetFourVector().Pt() - ptReferenceCosPhi, nvtx, mu, ptReferenceCosPhi,  Weight, (fCollectionName + "_CorrDeltaJet1PtMatchedBosPtvsNvtxMuPtRefEta21").c_str() );
		      else if ( std::abs(leadingJet->GetFourVectorCS().Eta()) < 2.8 )
			FillTH3Vector( leadingJet->GetFourVector().Pt() - ptReferenceCosPhi, nvtx, mu, ptReferenceCosPhi,  Weight, (fCollectionName + "_CorrDeltaJet1PtMatchedBosPtvsNvtxMuPtRefEta28").c_str() );
		      else if ( std::abs(leadingJet->GetFourVectorCS().Eta()) < 4.5 )
			FillTH3Vector( leadingJet->GetFourVector().Pt() - ptReferenceCosPhi, nvtx, mu, ptReferenceCosPhi,  Weight, (fCollectionName + "_CorrDeltaJet1PtMatchedBosPtvsNvtxMuPtRefEta45").c_str() );
		    }
		}
	    }
	}
    }
  //-------------------------------------------------------------
  //
  //  tracks
  //
  //-------------------------------------------------------------

  if ( (leadingJet != NULL)  && isSubLeadingJetVeto && ( !((*fAnaOptions)->EvaluateSystematics())  ||  fSys->isShiftInUse(ZeeDSystematics::eNoShift)) ) // not considered for systematics as well
    {	      
      Double_t *ptSum = new Double_t[ deltaRBins.size() ], *areas = new Double_t[ deltaRBins.size() ];
      // cout <<"\t\t *****\t\t area = ";
      for (UInt_t i = 0 ; i < deltaRBins.size() - 1 ; i++)
	{
	  ptSum[i] = 0; 
	  Double_t annulusWidth = (deltaRBins[i+1] - deltaRBins[i]);
	  Double_t radiusInBin = deltaRBins[i] + annulusWidth/2.;
	  areas[i] = truncatedArea( leadingJet->GetFourVectorCS().Eta(), leadingJet->GetFourVectorCS().Phi(), radiusInBin);
	  
	  //  cout << i<<" : "<<deltaRBins[i]<<" R = "<<radiusInBin<<" : "<<deltaRBins[i+1]
	  //   <<" A = "<<areas[i]<<"  "<<TMath::Pi()*radiusInBin*radiusInBin<<"  ||| ";
	}
      
      //cout << " areas done \n\n"<<endl;
      
      //leadingJet->GetFourVectorCS().Print();
      for (Int_t itrack = 0;  itrack < tracks->GetEntries(); ++itrack)
	{
	  ZeeDTrack* track =dynamic_cast<ZeeDTrack*>( tracks->At(itrack) ) ;
	  CHECK_NULL_PTR( track );
	  
	  TLorentzVector trackLV = track->GetFourVector();
	  if ( TMath::Abs(trackLV.Eta()) > trackingAcceptance ) continue;

	  Double_t trackPt = trackLV.Pt();
	  if ( trackPt < 1.0 ) continue;
	  
	  Int_t nSCTHits = track->GetTrackNumberOfSCTHits();
	  if ( nSCTHits < 5  ) continue;

	  Int_t nPixelHits = track->GetTrackNumberOfPixelHits();
	  if ( nPixelHits < 1  ) continue;

	  Double_t trackD0 = TMath::Abs( track->GetTrackD0() ); 
	  if (  trackD0 > 1. ) continue;

	  Double_t trackZ0 = TMath::Abs( track->GetTrackZ0()*TMath::Sin(track->GetTrackTheta()) );
	  if ( trackZ0 > 1. ) continue;

	  Int_t trackNDF = track->GetTrackNumberDoF();
	  if ( trackNDF <= 0 ) continue;

	  Double_t trackChi2NDF = track->GetTrackChiSquared()/track->GetTrackNumberDoF();
	  if ( trackChi2NDF > 3. ) continue;

	  Double_t trDeltaElec1 = trackLV.DeltaR( Electron1Momentum );
	  Double_t trDeltaElec2 = trackLV.DeltaR( Electron2Momentum );
	  
	  if ( trDeltaElec1 < deltaRcut ) continue;
	  if ( trDeltaElec2 < deltaRcut ) continue;
	  
	  
	  Double_t deltaR = trackLV.DeltaR( leadingJet->GetFourVector() );
	  
	  // 	      cout <<" \t\t"<<trDeltaElec1<<" "<<trDeltaElec2
	  // 		   <<" deltaR = "<<deltaR<<" pt = "<<trackPt
	  // 		   <<" ptref = "<<ptReferenceCosPhi
	  // 		   <<" eta = " << TMath::Abs(leadingJet->GetFourVectorCS().Eta())
	  // 		   << endl;
	  
	  
	  FillTH3( trackPt, ptReferenceCosPhi,  TMath::Abs(leadingJet->GetFourVectorCS().Eta()) , 
		   Weight, (fCollectionName + "_TrackPtVsBosonPtJetEta").c_str() );
	  
	  FillTH1( trackPt,     Weight, (fCollectionName + "_TrackPt").c_str() );
	  FillTH1( nSCTHits,    Weight, (fCollectionName + "_TrackSCTHits").c_str() );
	  FillTH1( nPixelHits,  Weight, (fCollectionName + "_TrackPixelHits").c_str() );
	  FillTH1( trackD0,     Weight, (fCollectionName + "_TrackD0").c_str() );
	  FillTH1( trackZ0,     Weight, (fCollectionName + "_TrackZ0").c_str() );
	  FillTH1( trackNDF,    Weight, (fCollectionName + "_TrackNDF").c_str() );
	  FillTH1( trackChi2NDF,Weight, (fCollectionName + "_TrackChi2NDF").c_str() );
	  
	  
	  for (UInt_t i = 0 ; i < deltaRBins.size() - 1 ; i++)
	    {
	      if ( deltaR >deltaRBins[i] && deltaR < deltaRBins[i+1] )  ptSum[i] +=  trackLV.Pt() ;
	    }
	}
      
      Double_t areaM1 = 0;
      for (UInt_t i = 0 ; i < deltaRBins.size() - 1 ; i++)
	{ 
	  Double_t radiusInBin = deltaRBins[i] + (deltaRBins[i+1] - deltaRBins[i])/2.;
	  Double_t areaI = areas[i] - areaM1;
	  areaM1 = areaI;
	  //	  if (ptSum[i] != 0)
	  //	    {
	  Double_t density = ptSum[i]/areaI;
	  
	  FillTProfile3D(radiusInBin , ptReferenceCosPhi,  TMath::Abs(leadingJet->GetFourVectorCS().Eta()) , 
			 density, Weight, (fCollectionName + "_TrackDeltaRVsBosonPtJetEta").c_str() );
	  
// 	      FillTH3(radiusInBin , ptReferenceCosPhi,  TMath::Abs(leadingJet->GetFourVectorCS().Eta()) , 
// 		      density * Weight, (fCollectionName + "_TrackDeltaRVsBosonPtJetEta").c_str() );
// 	      FillTH3(radiusInBin , ptReferenceCosPhi,  TMath::Abs(leadingJet->GetFourVectorCS().Eta()) , 
// 		      Weight, (fCollectionName + "_TrackDeltaRVsBosonPtJetEtaWeights").c_str() );
	      
	      // 	      std::cout <<"\t\t *** density = "<<density
// 			<<" R = "<<radiusInBin
// 			<<" ref  = "<<ptReferenceCosPhi
// 			<< " eta = "<<TMath::Abs(leadingJet->GetFourVectorCS().Eta()) 
// 			<<" area = "<<areaI<<" ibin = "<<i<<std::endl;
//	    }
	}
      
      delete [] ptSum;
      delete [] areas;
      
    }
  
  //  Info(__PRETTY_FUNCTION__, TString::Format(" \t\t &&&&&&&&&&&&&&&&& new event  collection = %s DONE ", fCollectionName.c_str()) );
  return;
}

//-----------------------------------------------------
//-----------------------------------------------------
//-----------------------------------------------------

Bool_t ZeeDHistManagerZjet::isLeadingJet( ZeeDJet*&  j, Bool_t debugFlag)
{
  if ( NULL != leadingJet ) return false;

  Double_t pt =  j->GetFourVector().Pt(), y =  TMath::Abs(j->GetFourVectorCS().Eta()), jvf =  j->GetJVF();
  if ( pt < _jetPtCut ) return false;

  isSubLeadingJetVeto = true;

  if (debugFlag) cout << "\t\t\t"<<__PRETTY_FUNCTION__ <<" \t : passed pt"<< endl;
  
  if ( std::string::npos != fCollectionName.find("Truth") ) jvf = 1.0;
  if ( ( y < trackingAcceptance )  && ( TMath::Abs(jvf) < _jetJVFCut ) )  return false;
  
  
  if (debugFlag) cout << "\t\t\t"<<__PRETTY_FUNCTION__ <<" \t : passed jvf"<< endl;

  leadingJet = j;

  return true;
}

Bool_t ZeeDHistManagerZjet::isSubLeadingJet( ZeeDJet*& j, Bool_t debugFlag)
{
  if ( NULL == leadingJet )    return false;
  if ( j    == leadingJet )    return false;
  if ( NULL != subLeadingJet ) return false;
  if (debugFlag) cout << "\t\t\t"<<__PRETTY_FUNCTION__ <<" \t : passed pretag"<< endl;

  Double_t pt =  j->GetFourVector().Pt(), y =  TMath::Abs(j->GetFourVectorCS().Eta()), jvf =  j->GetJVF();
  if ( std::string::npos != fCollectionName.find("Truth") ) jvf = 1.0;

  //if ( pt < _jetPtCut ) return false;

  if (debugFlag) cout << "\t\t\t"<<__PRETTY_FUNCTION__ <<" \t : passed pt"<< endl;

  

  isSubLeadingJetVeto = true;

  if ( ( y < trackingAcceptance )  && ( TMath::Abs(jvf) < _subJetJVFCut ) ) 
    {
      return false;
    }

  //
  //   veto part
  //

  //  Double_t  _cos_ = TMath::Abs( TMath::Cos( subLeadingJet->GetFourVector().DeltaPhi( bosonMomentum )) ) ;
  if (  pt >  _subJetPtCut ) 
    {

      if (debugFlag) cout << "\t\t\t"<<__PRETTY_FUNCTION__ <<" \t : passed jvf"<< endl;

      isSubLeadingJetVeto = false;
    }
  //  if (  pt >  TMath::Max( _jetPtCut , _subJetRelPtCut * ptReferenceCosPhi )  ) isSubLeadingJetVeto = false;

  subLeadingJet = j;

  if (debugFlag) cout << "\t\t\t"<<__PRETTY_FUNCTION__ <<" \t : veto == "<<isSubLeadingJetVeto<< endl;

  return true;
}

//-----------------------------------------------------
//-----------------------------------------------------
//-----------------------------------------------------

void ZeeDHistManagerZjet::GetMatchedTruth(  ZeeDJet*& reco )
{

  matchedTruthJet = NULL;

  const ZeeDEvent* event = GetEvent();
  CHECK_NULL_PTR(event);

//   std::cout <<" coll = "<<fCollectionName<<" truth = "<<fTruthCollectionName
// 	    <<" pt = "<<reco->GetFourVector().Pt()
// 	    <<" y = "<<reco->GetFourVector().Rapidity()
// 	    << std::endl;

  const TObjArray *truthJets = event->GetJets( fTruthCollectionName, kTRUE); // don't warn if missing, just continue

  if ( truthJets == NULL ) return;
  CHECK_NULL_PTR( truthJets );

  Double_t minDR = deltaRcut;
  Int_t matchIndex = -1;
  
  for (Int_t ijet = 0;  ijet< truthJets->GetEntries(); ++ijet)
    {
      const ZeeDJet* jet =dynamic_cast<ZeeDJet*>( truthJets->At(ijet) ) ;
      CHECK_NULL_PTR( jet );

      Double_t deltaRElec1 = jet->GetFourVector().DeltaR( Electron1Momentum );
      Double_t deltaRElec2 = jet->GetFourVector().DeltaR( Electron2Momentum );
      Bool_t isIsolated = true;
      if ( ( deltaRElec1 <= deltaRcut ) || ( deltaRElec2 <= deltaRcut ) ) isIsolated = false;
      if ( !isIsolated ) continue;


      Double_t _deltaR = jet->GetFourVector().DeltaR( reco->GetFourVector() ); 
      if (  _deltaR <= minDR )
	{
	  minDR = _deltaR;
	  matchIndex = ijet;
	}
    }
  
  if (matchIndex >= 0 )
    matchedTruthJet =  dynamic_cast<ZeeDJet*>( truthJets->At( matchIndex ) ) ;

  return;
}
//-----------------------------------------------------
//-----------------------------------------------------
//-----------------------------------------------------


Double_t ZeeDHistManagerZjet::truncatedArea(double etaJ, double phiJ, double R, double eta0)
/*****************************************************************/
{
  if( R > TMath::Pi() )
    Error("Zjet::truncatedArea()","R > Pi  circle will overlap itself\n");

  double deltaEta1 = fabs(etaJ-eta0);
  double deltaEta2 = fabs(etaJ+eta0);
  double deltaEta1Square = deltaEta1*deltaEta1;
  double deltaEta2Square = deltaEta2*deltaEta2;
  bool intersect1 = (R*R-deltaEta1Square>0 ? true : false);
  bool intersect2 = (R*R-deltaEta2Square>0 ? true : false);
  double phi11 = 0.;
  double phi12 = 0.;
  double phi21 = 0.;
  double phi22 = 0.;
  if((R<=deltaEta1 && intersect1) || (R<=deltaEta2 && intersect2))
    Error("Zjet::truncatedArea()"," Found intersections while it shouldn't");
  if((R>deltaEta1 && !intersect1) || (R>deltaEta2 && !intersect2))
    Error("Zjet::truncatedArea()"," Didn't found intersections while it should");

  if(intersect1)
    {
      phi11 = phiJ + sqrt(R*R-deltaEta1Square);
      phi12 = phiJ - sqrt(R*R-deltaEta1Square);
      //msg()<<LogLevel_Info<<"Phi11="<<phi11<<" Phi12="<<phi12<<"\n";
      //msg()<<LogLevel_Info<<fabs((phi11-phiJ)/(phiJ-phi12)-1.)<<"\n";
      //if(phi11-phiJ != phiJ-phi12)
      //    throw MarException("StudyKTerm::truncatedArea(): non symmetric intersections");
    }
  if(intersect2)
    {
      phi21 = phiJ + sqrt(R*R-deltaEta2Square);
      phi22 = phiJ - sqrt(R*R-deltaEta2Square);
      //msg()<<LogLevel_Info<<"Phi21="<<phi21<<" Phi22="<<phi22<<"\n";
      //msg()<<LogLevel_Info<<(phi21-phiJ)/(phiJ-phi22)<<"\n";
      //msg()<<LogLevel_Info<<fabs((phi21-phiJ)/(phiJ-phi22)-1.)<<"\n";
      //if(phi21-phiJ != phiJ-phi22)
      //    throw MarException("StudyKTerm::truncatedArea(): non symmetric intersections");
    }

  double area = 0.;
  if(!intersect1 && !intersect2)
    area = M_PI*R*R;
  else if(intersect1 && !intersect2)
    {
      double deltaPhi = fabs(phi11-phi12);
      double alpha = 2.*asin(deltaPhi/(2.*R));
      double areaCircle = M_PI*R*R*(1.-alpha/(2.*M_PI));
      double areaTriangle = deltaEta1*deltaPhi/2.;
      area = areaCircle + areaTriangle;
      //msg()<<LogLevel_Info<<"Intersection1 : alpha="<<alpha<<" areaCircle="<<areaCircle<<" areaTriangle="<<areaTriangle<<"\n";
    }
  else if(!intersect1 && intersect2)
    {
      double deltaPhi = fabs(phi21-phi22);
      double alpha = 2.*asin(deltaPhi/(2.*R));
      double areaCircle = M_PI*R*R*(1.-alpha/(2.*M_PI));
      double areaTriangle = deltaEta2*deltaPhi/2.;
      area = areaCircle + areaTriangle;
      //msg()<<LogLevel_Info<<"Intersection2 : alpha="<<alpha<<" areaCircle="<<areaCircle<<" areaTriangle="<<areaTriangle<<"\n";
    }
  else if(intersect1 && intersect2)
    {
      double deltaPhi1 = fabs(phi11-phi12);
      double deltaPhi2 = fabs(phi21-phi22);
      double alpha1 = 2.*asin(deltaPhi1/(2.*R));
      double alpha2 = 2.*asin(deltaPhi2/(2.*R));
      double areaCircle = M_PI*R*R*(1.-(alpha1+alpha2)/(2.*M_PI));
      double areaTriangle = (deltaEta1*deltaPhi1 + deltaEta2*deltaPhi2)/2.;
      area = areaCircle + areaTriangle;
      //msg()<<LogLevel_Info<<"Intersection1+2 : alpha1+alpha2="<<alpha1+alpha2<<" areaCircle="<<areaCircle<<" areaTriangle="<<areaTriangle<<"\n";
    }
  //msg()<<LogLevel_Info<<"("<<etaJ<<", "<<phiJ<<") R="<<R<<" --> A="<<area<<"(Pi*R^2="<<M_PI*R*R<<")\n";
  return area;

}

//-----------------------------------------------------
//-----------------------------------------------------
//-----------------------------------------------------
