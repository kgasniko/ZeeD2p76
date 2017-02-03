#include "ZeeDHistManager/ZeeDHistManagerZplusJet.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDJet.h"
#include "ZeeDEvent/ZeeDTrack.h"
#include "ZeeDEvent/ZeeDGenParticle.h"

#include "ZeeDTools/ZeeDBags.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

//#define USE_DEBUG_MESSAGES
//#define DEBUG_ON 1
#include "ZeeDMisc/ZeeDMisc.h"

using std::cout;
using std::endl;

//-----------------------------------------------------
void  ZeeDHistManagerZplusJet::bookAnalysisCuts()
{
  //std::cout <<__PRETTY_FUNCTION__<< std::endl;
  jetPtJVFCut          = 50.0;
  jetYCut              = 4.4;  
  jetJVFCut            = 0.25;
  jetPtCut             = 20.0;  

  bosonPtCut           =  1.0;
  trackingAcceptance   =  2.5;

  deltaRCutMap.insert( std::pair<std::string,Double_t>("4" , 0.350) ) ;
  deltaRCutMap.insert( std::pair<std::string,Double_t>("5" , 0.425) ) ;
  deltaRCutMap.insert( std::pair<std::string,Double_t>("6" , 0.500) ) ;

  jetCone = 1.0;
  jetConeMap.insert( std::pair<std::string,Double_t>("3" , 0.3) ) ;
  jetConeMap.insert( std::pair<std::string,Double_t>("4" , 0.4) ) ;
  jetConeMap.insert( std::pair<std::string,Double_t>("5" , 0.5) ) ;
  jetConeMap.insert( std::pair<std::string,Double_t>("6" , 0.6) ) ;
  jetConeMap.insert( std::pair<std::string,Double_t>("7" , 0.7) ) ;

  return;
}

void ZeeDHistManagerZplusJet::BookHistos()
{
  bookAnalysisCuts();

    // Do we run on data or MC?
    if ( (*fAnaOptions)->IsMC() ) {
        bIsMC = kTRUE;
    }
    // Books histograms

    const TString ZjetBosPt     = ZeeDMisc::FindFile("Binning/ZjetBosPt.txt");
    const TString ZplusJet_PtJet_YJet     = ZeeDMisc::FindFile("Binning/ZplusJet_PtJet_YJet.txt");
    const TString ZplusJet_PtZ_YZJet     = ZeeDMisc::FindFile("Binning/ZplusJet_PtZ_YZJet.txt");

    const TString ZplusJetdRPtTruthResponce =  ZeeDMisc::FindFile("Binning/ZplusJetdRPtTruthResponce.txt");
    const TString ZplusJetMuPtTruthResponce =  ZeeDMisc::FindFile("Binning/ZplusJetMuPtTruthResponce.txt");  
    const TString ZplusJetNpvPtTruthResponce =  ZeeDMisc::FindFile("Binning/ZplusJetNpvPtTruthResponce.txt");  

    const TString ZplusJetEta =  ZeeDMisc::FindFile("Binning/ZplusJetEta.txt");  
    const TString ZplusJetdRPtTruthResponceElec =  ZeeDMisc::FindFile("Binning/ZplusJetdRPtTruthResponceElec.txt");
      
    const TString ZplusJetPtRatioRecoilEta  =  ZeeDMisc::FindFile("Binning/ZplusJetPtRatioRecoilEta.txt");

    const TString ZplusJetBosMassdR1dR2 = ZeeDMisc::FindFile("Binning/ZplusJetBosMassdR1dR2.txt");
    const TString ZplusJetBosMassdRmin = ZeeDMisc::FindFile("Binning/ZplusJetBosMassdRmin.txt");

    std::vector<std::string> collections = (*fAnaOptions)->JetCollectionNames();
    for (unsigned int i = 0; i < collections.size(); i++)
      {


	//####################################### dR(jet, electron) cut studies #######################################//
	AddTH1D(collections[i] + "_DeltaRmin_Jet_El1Reco",   700, 0, 7.0,  "#Delta R (jet-el_{1}(reco)) "     , "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaRmin_Jet_El2Reco",   700, 0, 7.0,  "#Delta R (jet-el_{2}(reco)) "     , "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaRmin_Jet_El1Truth",  700, 0, 7.0,  "#Delta R (jet-el_{1}(truth)) "    , "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaRmin_Jet_El2Truth",  700, 0, 7.0,  "#Delta R (jet-el_{2}(truth)) "    , "", ZeeDHistLevel::Analysis);

	AddTH1D(collections[i] + "_DeltaRmin_Jet_ElReco_Data",  700, 0, 7.0,  "#Delta R_{min} (jet-el) "    , "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaRmin_Jet_ElReco",       700, 0, 7.0,  "#Delta R_{min} (jet-el(reco)) "    , "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaRmin_Jet_ElTruth",      700, 0, 7.0,  "#Delta R_{min} (jet-el(truth)) "    , "", ZeeDHistLevel::Analysis);

	AddTH1D(collections[i] + "_DeltaRJetElec1_before_iso",  314, 0, 3.14,  "#Delta R (jet-ele_{1}) "    , "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaRJetElec2_before_iso",  314, 0, 3.14,  "#Delta R (jet-ele_{2})"     , "", ZeeDHistLevel::Analysis);

	// truth responce of electron: pt_{reco electron}/pt_{truth electron}

	AddTH3D(collections[i] + "_dR1_PtTruthEl1_TruthRespEl1" ,   ZplusJetdRPtTruthResponceElec,     "P_{T} (el_{1,truth}) / GeV",  "P_{T, el_{1,reco}}/P_{T, el_{1,truth}}",   "#Delta R(jet, el)",     ZeeDHistLevel::Analysis); 
	AddTH3D(collections[i] + "_dR2_PtTruthEl2_TruthRespEl2" ,   ZplusJetdRPtTruthResponceElec,     "P_{T} (el_{2,truth}) / GeV",  "P_{T, el_{2,reco}}/P_{T, el_{2,truth}}",   "#Delta R(jet, el)",     ZeeDHistLevel::Analysis); 
	AddTH3D(collections[i] + "_dR_PtTruthEl_TruthRespEl" ,      ZplusJetdRPtTruthResponceElec,     "P_{T} (el_{truth}) / GeV",  "P_{T, el_{reco}}/P_{T, el_{truth}}",   "#Delta R(jet, el)",     ZeeDHistLevel::Analysis); 
	
	AddTH3D(collections[i] + "_dR1min_PtTruthEl1_TruthRespEl1" ,   ZplusJetdRPtTruthResponceElec,     "P_{T} (el_{1,truth}) / GeV",  "P_{T, el_{1,reco}}/P_{T, el_{1,truth}}",   "#Delta R_{min}(jet, el)",     ZeeDHistLevel::Analysis); 
	AddTH3D(collections[i] + "_dR2min_PtTruthEl2_TruthRespEl2" ,   ZplusJetdRPtTruthResponceElec,     "P_{T} (el_{2,truth}) / GeV",  "P_{T, el_{2,reco}}/P_{T, el_{2,truth}}",   "#Delta R_{min}(jet, el)",     ZeeDHistLevel::Analysis); 
	AddTH3D(collections[i] + "_dRmin_PtTruthEl_TruthRespEl" ,      ZplusJetdRPtTruthResponceElec,     "P_{T} (el_{truth}) / GeV",  "P_{T, el_{reco}}/P_{T, el_{truth}}",   "#Delta R_{min}(jet, el)",     ZeeDHistLevel::Analysis); 

	// truth responce of jet: pt_{reco jet}/pt_{truth jet}
	AddTH3D(collections[i] + "_dR1_PtTruth_TruthResp" ,   ZplusJetdRPtTruthResponce,     "P_{T} (jet_{truth}) / GeV",  "P_{T, jet_{reco}}/P_{T, jet_{truth}}",   "#Delta R(jet, el)",     ZeeDHistLevel::Analysis); 
	AddTH3D(collections[i] + "_dR2_PtTruth_TruthResp" ,   ZplusJetdRPtTruthResponce,     "P_{T} (jet_{truth}) / GeV",   "P_{T, jet_{reco}}/P_{T, jet_{truth}}",   "#Delta R(jet, el)",    ZeeDHistLevel::Analysis); 
 	AddTH3D(collections[i] + "_dRmin_PtTruth_TruthResp" ,   ZplusJetdRPtTruthResponce,     "P_{T} (jet_{truth}) / GeV",   "P_{T, jet_{reco}}/P_{T, jet_{truth}}",   "#Delta R_{min}(jet, el)",    ZeeDHistLevel::Analysis);

	AddTH1D(collections[i] + "_DeltaRJetRecoTruth",  700, 0, 7.0,  "#Delta R (jet(reco)-jet(truth))"     , "", ZeeDHistLevel::Analysis);

	AddTH3D(collections[i] + "_BosMass_dREl1_dREl2",   ZplusJetBosMassdR1dR2, "#Delta R(jet, el_{1})", "M (Z->ee) / GeV", "#Delta R(jet, el_{2})", ZeeDHistLevel::Analysis);
	AddTH2D(collections[i] + "_BosMass_dRElmin",   ZplusJetBosMassdRmin, "#Delta R_{min}(jet, el)", "M (Z->ee) / GeV", ZeeDHistLevel::Analysis);

	//###################################################################################################################//

	AddTH1D(collections[i] + "_Jets_BosPt",   501, 0, 500,  "P_{T} (Z->ee) / GeV", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_Jets_BosY",    51,-5, 5,     "Y (Z->ee) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_Jets_BosPhi",  180, -180, 180,  "#Phi (Z->ee) / [deg]", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_Jets_BosMass",  100, 66, 116,  "M (Z->ee) / GeV", "", ZeeDHistLevel::Systematics);

	AddTH1D(collections[i] + "_PtJet" , 48 , 20, 500,  "P_{T} (jet) / GeV", "", ZeeDHistLevel::Systematics);
	AddTH1D(collections[i] + "_PtJet1", 48 , 20, 500,  "P_{T} (jet_{1}) / GeV", "", ZeeDHistLevel::Systematics);
	AddTH1D(collections[i] + "_PtJet2", 48 , 20, 500,  "P_{T} (jet_{2}) / GeV", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_PtJet3", 48 , 20, 500,  "P_{T} (jet_{3}) / GeV", "", ZeeDHistLevel::Analysis);
	
	AddTH1D(collections[i] + "_EMFJet",    321, -1.1, 2.1,  "EMF (jet)"    , "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_EMFJet1",   321, -1.1, 2.1,  "EMF (jet_{1}) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_EMFJet2",   321, -1.1, 2.1,  "EMF (jet_{2})" , "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_EMFJet3",   321, -1.1, 2.1,  "EMF (jet_{3})" , "", ZeeDHistLevel::Analysis);
	
	AddTH1D(collections[i] + "_JVFJet",    321, -1.1, 2.1,  "JVF (jet)"    , "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_JVFJet1",   321, -1.1, 2.1,  "JVF (jet_{1})", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_JVFJet2",   321, -1.1, 2.1,  "JVF (jet_{2})", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_JVFJet3",   321, -1.1, 2.1,  "JVF (jet_{3})", "", ZeeDHistLevel::Analysis);
	
	AddTH1D(collections[i] + "_YDetJet",    50, -5, 5,  "#eta (jet) ", "", ZeeDHistLevel::Systematics);
	AddTH1D(collections[i] + "_YDetJet1",   50, -5, 5,  "#eta (jet_{1}) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_YDetJet2",   50, -5, 5,  "#eta (jet_{2}) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_YDetJet3",   50, -5, 5,  "#eta (jet_{3}) ", "", ZeeDHistLevel::Analysis);
	
	AddTH1D(collections[i] + "_YJet",    50, -5, 5,  "y (jet) ", "", ZeeDHistLevel::Systematics);
	AddTH1D(collections[i] + "_YJet1",   50, -5, 5,  "y (jet_{1}) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_YJet2",   50, -5, 5,  "y (jet_{2}) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_YJet3",   50, -5, 5,  "y (jet_{3}) ", "", ZeeDHistLevel::Analysis);
	
	AddTH1D(collections[i] + "_PhiJet",    180, -180, 180,  "#Phi (jet) [deg]", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_PhiJet1",   180, -180, 180,  "#Phi (jet_{1}) [deg]", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_PhiJet2",   180, -180, 180,  "#Phi (jet_{2}) [deg]", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_PhiJet3",   180, -180, 180,  "#Phi (jet_{3}) [deg]", "", ZeeDHistLevel::Analysis);
	
	AddTH1D(collections[i] + "_PhiDetJet",    180, -180, 180,  "#Phi (jet) [deg] ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_PhiDetJet1",   180, -180, 180,  "#Phi (jet_{1}) [deg] ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_PhiDetJet2",   180, -180, 180,  "#Phi (jet_{2}) [deg]", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_PhiDetJet3",   180, -180, 180,  "#Phi (jet_{3}) [deg]", "", ZeeDHistLevel::Analysis);
		
	AddTH1D(collections[i] + "_DeltaRJetElec1",  314, 0, 3.14,  "#Delta R (jet-ele_{1}) "    , "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaRJetElec2",  314, 0, 3.14,  "#Delta R (jet-ele_{2})"     , "", ZeeDHistLevel::Analysis);

	AddTH1D(collections[i] + "_DeltaRJet1Elec1", 314, 0, 3.14,  "#Delta R (jet_{1}-ele_{1}) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaRJet1Elec2", 314, 0, 3.14,  "#Delta R (jet_{1}-ele_{2}) ", "", ZeeDHistLevel::Analysis);

	AddTH1D(collections[i] + "_DeltaRJet2Elec1", 314, 0, 3.14,  "#Delta R (jet_{2}-ele_{1}) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaRJet2Elec2", 314, 0, 3.14,  "#Delta R (jet_{2}-ele_{2}) ", "", ZeeDHistLevel::Analysis);

	AddTH1D(collections[i] + "_DeltaRJet3Elec1", 314, 0, 3.14,  "#Delta R (jet_{3}-ele_{1}) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaRJet3Elec2", 314, 0, 3.14,  "#Delta R (jet_{3}-ele_{2}) ", "", ZeeDHistLevel::Analysis);

	AddTH1D(collections[i] + "_DeltaRJet1Jet2", 314, 0, 3.14,  "#Delta R (jet_{2}-jet_{1}) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaRJet1Jet3", 314, 0, 3.14,  "#Delta R (jet_{3}-jet_{1}) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaRJet2Jet3", 314, 0, 3.14,  "#Delta R (jet_{3}-jet_{2}) ", "", ZeeDHistLevel::Analysis);

	AddTH1D(collections[i] + "_DeltaPhiJet1Jet2", 180, -180, 180,  "#Delta #phi (jet_{2}-jet_{1}) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaPhiJet1Jet3", 180, -180, 180,  "#Delta #phi (jet_{3}-jet_{1}) ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_DeltaPhiJet2Jet3", 180, -180, 180,  "#Delta #phi (jet_{3}-jet_{2}) ", "", ZeeDHistLevel::Analysis);

	AddTH1D(collections[i] + "_MassJet1", 200, 0, 100,  "mass_{jet_{1}} ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_MassJet2", 200, 0, 100,  "mass_{jet_{2}} ", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_MassJet3", 200, 0, 100,  "mass_{jet_{3}} ", "", ZeeDHistLevel::Analysis);	

	AddTH2D(collections[i] + "_PtJet_YJet", ZplusJet_PtJet_YJet, "P_{T} (jet) / GeV", "y (jet) ", ZeeDHistLevel::Systematics);
	AddTH2D(collections[i] + "_PtJet_YZMinYJet", ZplusJet_PtJet_YJet, "P_{T} (jet) / GeV", "y (jet) ", ZeeDHistLevel::Systematics);

	AddTH2D(collections[i] + "_PtJet1_YJet1", ZplusJet_PtJet_YJet, "P_{T} (jet_{1}) / GeV", "y (jet_{1}) ", ZeeDHistLevel::Analysis);
	AddTH2D(collections[i] + "_PtZ_YJet1",    ZplusJet_PtJet_YJet, "P_{T,Z} / GeV",     "y (jet_{1}) ", ZeeDHistLevel::Analysis);
	AddTH2D(collections[i] + "_PtZ_YZ",       ZplusJet_PtJet_YJet, "P_{T,Z} / GeV",     "y_{Z} ", ZeeDHistLevel::Analysis);
	AddTH2D(collections[i] + "_PtZ_YZJet1",   ZplusJet_PtZ_YZJet, "P_{T,Z} / GeV",     "y_{Z}+y_{jet_{1}} ", ZeeDHistLevel::Analysis);

	AddTH1D(collections[i] + "_NumJets", 21, -0.5, 20.5,  "Number of jets", "", ZeeDHistLevel::Systematics);

	AddTH3D(collections[i] + "_PtZPtJet1_Recoil_EtaJet1" ,   ZplusJetPtRatioRecoilEta,     "P_{T} (Z)/ P_T (Jet_{1}",  "#Delta Y(recoil, Jet_{1})",   "#eta (jet_{1})",     ZeeDHistLevel::Analysis);


	// Reco-True matching studies
	AddTH2D(collections[i] + "_PtRecoJet_PtTruthJet",  300, 0, 300,  300, 0, 300,  "P_{T} (reco jet) / GeV",  "P_{T} (truth jet) / GeV", ZeeDHistLevel::Analysis);
	AddTH2D(collections[i] + "_YRecoJet_YTruthJet",   50, -5, 5, 50, -5, 5, "y (reco jet) ",  "y (truth jet) ", ZeeDHistLevel::Analysis);
	AddTH2D(collections[i] + "_NumRecoJet_NumTruthJet",   20, 0.0, 20, 20, 0.0, 20, "Number of reconstructed jets",  "Number of truth jets", ZeeDHistLevel::Analysis);

	AddTH1D(collections[i] + "_PtJetMatched" ,  1001, -0.5, 500.5,  "P_{T} (jet) / GeV", "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_YJetMatched",    51, -5, 5,  "y (jet) ", "", ZeeDHistLevel::Analysis);
      }
}

//-----------------------------------------------------
void ZeeDHistManagerZplusJet::Fill()
{
  DEBUG_MES_START;
  DEBUG_MES(this->GetHMName());

  // Get event
  const ZeeDEvent* event = GetEvent();
  CHECK_NULL_PTR(event);
  Double_t Weight = event->GetWeight();

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
	    _truthCollection.ReplaceAll("ReTagged","");
	    _truthCollection.ReplaceAll("New","");
	    fTruthCollectionName = _truthCollection.Data();
	    const TObjArray *jets = event->GetJets( fCollectionName, kFALSE); // don't warn if missing, just continue

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

      matched_el1 = false;
      matched_el2 = false;	

      // matching for reco-truth electrons, only if gen info is switched
/*      if  ( (*fAnaOptions)->IsMC() && (*fAnaOptions)->FillGenInfo() ){
	const ZeeDGenParticle* El    = event->GetGenDressedElectron();
	const ZeeDGenParticle* Pos   = event->GetGenDressedPositron();

	ElectronTruthMomentum = El -> GetMCFourVector();
	PositronTruthMomentum = Pos -> GetMCFourVector();
	
	Double_t dR1El = Electron1Momentum.DeltaR( ElectronTruthMomentum );
	Double_t dR1Pos = Electron1Momentum.DeltaR( PositronTruthMomentum );	
	Double_t dR2El = Electron2Momentum.DeltaR( ElectronTruthMomentum );
	Double_t dR2Pos = Electron2Momentum.DeltaR( PositronTruthMomentum );	  
	  
	if ((dR1El < 0.2) && (dR1El < dR1Pos)){
	  matched_el1 = true;
	  MatchedElectron1Momentum = ElectronTruthMomentum;
	}
	if ((dR1Pos < 0.2) && (dR1El > dR1Pos)){
	  matched_el1 = true;
	  MatchedElectron1Momentum = PositronTruthMomentum;
	}
	if ((dR2El < 0.2) && (dR2El < dR2Pos)){
	  matched_el2 = true;
	  MatchedElectron2Momentum = ElectronTruthMomentum;
	}
	if ((dR2Pos < 0.2) && (dR2El > dR2Pos)){
	  matched_el2 = true;
	  MatchedElectron2Momentum = PositronTruthMomentum;
	}
      }*/

      std::vector<std::string> collections = (*fAnaOptions)->JetCollectionNames();
      for (unsigned int i = 0; i < collections.size(); i++)
	{
	  fCollectionName = collections[i];
	  TString _truthCollection( fCollectionName.c_str() );
	  _truthCollection.ReplaceAll("TopoEM","Truth");
	  _truthCollection.ReplaceAll("LCTopo","Truth");  
	  _truthCollection.ReplaceAll("ReTagged","");
	  _truthCollection.ReplaceAll("New","");
	  fTruthCollectionName = _truthCollection.Data();
	  const TObjArray *jets = event->GetJets( fCollectionName , kFALSE);
	  if ( jets )  FillJetHists( jets,  Weight);
	}
    }
  }
  DEBUG_MES_END;
}

//-----------------------------------------------------

void ZeeDHistManagerZplusJet::FillJetHists(const TObjArray* jets,  Double_t Weight )
{

  Bool_t _debug_ = false;
  Double_t bosonPt = bosonMomentum.Pt();
  Double_t bosonY = bosonMomentum.Rapidity();
  Double_t bosonMass = bosonMomentum.M();
  if ( bosonPt < bosonPtCut ) return;

  const ZeeDEvent* event = GetEvent();
  CHECK_NULL_PTR(event);

  //  -----------------------------------------
  // zero pointer to leading jets

  leadingJet          =  NULL; 
  subLeadingJet1       =  NULL;
  subLeadingJet2       =  NULL;
  //-----------------------------------------

  deltaRcut = 3.0;
  for (std::map<std::string,Double_t>::iterator itr = deltaRCutMap.begin() ; itr != deltaRCutMap.end(); ++itr)
    if (fCollectionName.find(itr->first) != std::string::npos) 
      {
	deltaRcut = itr->second;
	break;
      }
  for (std::map<std::string,Double_t>::iterator itr = jetConeMap.begin() ; itr != jetConeMap.end(); ++itr)
    if (fCollectionName.find(itr->first) != std::string::npos) 
      {
	jetCone = itr->second;
	break;
      }
  if ( _debug_)
    {
      std::cout <<"\t\t\t  "<<fCollectionName<<" deltaR = "<<deltaRcut<< std::endl;
      std::cout <<"\t\t\t  pt = "<<jetPtCut
		<<"  jvf def = "<<jetJVFCut
		<<" fSys = "<<fSys<<std::endl;
    }

  Int_t NumTruthJetsInEvent = 0;

  const TObjArray *truthjets = NULL;

  //fill plots for electron truth response IF number of truth jets > 0
  if ( (*fAnaOptions)->IsMC() && (*fAnaOptions)->FillGenInfo()){ // start if MC and gen information filled
    truthjets = event->GetJets( "AntiKt4TruthJets" , kFALSE);
    if ( truthjets ) NumTruthJetsInEvent = truthjets -> GetEntries()-2;
  
    if (NumTruthJetsInEvent != 0 ){ // start if number of truth jets > 0
      if(matched_el1 && matched_el2){ // if both electrons are matched reco-truth
	
	GetMinDR(  Electron1Momentum, MatchedElectron1Momentum, fCollectionName );
	Double_t dR1 = minDRElReco;
	if (minDRElTruth != 10 && minDRElReco != 10){ // if dR between first electron and jet (on both truth and reco level) calculated

	  FillTH1(minDRElTruth,   Weight, (fCollectionName + "_DeltaRmin_Jet_El1Truth").c_str()  );
	  FillTH1(minDRElReco,    Weight, (fCollectionName + "_DeltaRmin_Jet_El1Reco").c_str()  );
	  
	  FillTH3( MatchedElectron1Momentum.Pt(), Electron1Momentum.Pt()/MatchedElectron1Momentum.Pt(), minDRElReco,  Weight, (fCollectionName + "_dR1min_PtTruthEl1_TruthRespEl1").c_str()  );
	  FillTH3( MatchedElectron1Momentum.Pt(), Electron1Momentum.Pt()/MatchedElectron1Momentum.Pt(), minDRElReco,  Weight, (fCollectionName + "_dRmin_PtTruthEl_TruthRespEl").c_str()  );
	} // end if dR between first electron and jet (on both truth and reco level) calculated
	
	GetMinDR(  Electron2Momentum, MatchedElectron2Momentum, fCollectionName );
	Double_t dR2 = minDRElReco;
	if (minDRElTruth != 10 && minDRElReco != 10){ // if dR between second electron and jet (on both truth and reco level) calculated

	  FillTH1(minDRElTruth,   Weight, (fCollectionName + "_DeltaRmin_Jet_El2Truth").c_str()  );
	  FillTH1(minDRElReco,   Weight, (fCollectionName + "_DeltaRmin_Jet_El2Reco").c_str()  );

	  FillTH3( MatchedElectron2Momentum.Pt(), Electron2Momentum.Pt()/MatchedElectron2Momentum.Pt(), minDRElReco,  Weight, (fCollectionName + "_dR2min_PtTruthEl2_TruthRespEl2").c_str()  );
	  FillTH3( MatchedElectron2Momentum.Pt(), Electron2Momentum.Pt()/MatchedElectron2Momentum.Pt(), minDRElReco,  Weight, (fCollectionName + "_dRmin_PtTruthEl_TruthRespEl").c_str()  );
	} // end if dR between second electron and jet (on both truth and reco level) calculated

	if (dR1 != 10 && dR2 != 10)
	  FillTH3( dR1, bosonMass,dR2, Weight, (fCollectionName + "_BosMass_dREl1_dREl2").c_str()  );
            
      } 

    }// end if number of truth jets > 0
  }// end if MC and gen information filled

  if ( !(*fAnaOptions)->IsMC()){ // if data match reco electron to reco jet
    GetMinDRInData( Electron1Momentum, fCollectionName );
    Double_t dR1 = minDRElReco;
    GetMinDRInData( Electron2Momentum, fCollectionName );
    Double_t dR2 = minDRElReco;
    FillTH3( dR1, bosonMass, dR2, Weight, (fCollectionName + "_BosMass_dREl1_dREl2").c_str()  );
  }  // end if data match reco electron to reco jet


 
  Int_t NumJets=0, NumTruthJets=0; // NumJets/NumTruthJets - number of reconstructed/truth jets which PASSED selection cuts. 
  for (Int_t ijet = 0;  ijet< jets->GetEntries(); ++ijet)  // start loop over jets
    {

      ZeeDJet* jet =dynamic_cast< ZeeDJet* >( jets->At(ijet) ) ;
      CHECK_NULL_PTR( jet );
      
      if ( _debug_ )
	{
	  Info(__PRETTY_FUNCTION__,TString::Format(" ***** Looping jets %d",ijet));
	}

      TLorentzVector fourVector   = jet->GetFourVector();
      TLorentzVector fourVectorCS = jet->GetFourVectorCS();

      Double_t pt        = fourVector.Pt();
      Double_t y         = fourVector.Rapidity();
      Double_t phi       = fourVector.Phi();
      Double_t mass      = fourVector.Mag();
      Double_t ydet      = fourVectorCS.Eta();
      Double_t phidet    = fourVectorCS.Phi();
      Double_t emf  = jet->GetEmFraction();

      
      Double_t jvf;

      if  ( (*fAnaOptions)->IsMC() && (fCollectionName.find("Truth")  != std::string::npos) ) {
	jvf = 1.0;
      }
      else {
	jvf  = jet->GetJVF();
      }

      Double_t deltaRElec1 = fourVector.DeltaR( Electron1Momentum );
      Double_t deltaRElec2 = fourVector.DeltaR( Electron2Momentum );
      Double_t deltaRElecMin, deltaRElecTruthMin;
      
      FillTH1( deltaRElec1,  Weight, (fCollectionName + "_DeltaRJetElec1_before_iso").c_str() );
      FillTH1( deltaRElec2,  Weight, (fCollectionName + "_DeltaRJetElec2_before_iso").c_str() );

      Bool_t isIsolated = true;
      if ( ( deltaRElec1 <= deltaRcut ) || ( deltaRElec2 <= deltaRcut ) ) isIsolated = false;
      if ( !isIsolated ) continue;

      if (( pt < jetPtJVFCut )&&( TMath::Abs(y) < trackingAcceptance )&&( TMath::Abs(jvf) < jetJVFCut )) continue;
      if ( TMath::Abs(y) > jetYCut ) continue;

      if ( pt < jetPtCut ) continue;


      deltaRElecMin = deltaRElec1 < deltaRElec2 ? deltaRElec1 : deltaRElec2;
      FillTH1( deltaRElecMin,    Weight, (fCollectionName + "_DeltaRmin_Jet_ElReco_Data").c_str()  );

      if  ( !(*fAnaOptions) -> IsMC() )
	FillTH2( deltaRElecMin, bosonMass, Weight, (fCollectionName + "_BosMass_dRElmin").c_str()  );

      if(matched_el1){
	FillTH1( MatchedElectron1Momentum.Pt(), Weight, (fCollectionName + "_PtEl1").c_str() );
	FillTH3( MatchedElectron1Momentum.Pt(), Electron1Momentum.Pt()/MatchedElectron1Momentum.Pt(), deltaRElec1,  Weight, (fCollectionName + "_dR1_PtTruthEl1_TruthRespEl1").c_str()  );
	FillTH3( MatchedElectron1Momentum.Pt(), Electron1Momentum.Pt()/MatchedElectron1Momentum.Pt(), deltaRElec1,  Weight, (fCollectionName + "_dR_PtTruthEl_TruthRespEl").c_str()  );
      }
      if(matched_el2){
	FillTH1( MatchedElectron2Momentum.Pt(), Weight, (fCollectionName + "_PtEl2").c_str() );
	FillTH3( MatchedElectron2Momentum.Pt(), Electron2Momentum.Pt()/MatchedElectron2Momentum.Pt(), deltaRElec2,  Weight, (fCollectionName + "_dR2_PtTruthEl2_TruthRespEl2").c_str()  );
	FillTH3( MatchedElectron1Momentum.Pt(), Electron1Momentum.Pt()/MatchedElectron1Momentum.Pt(), deltaRElec1,  Weight, (fCollectionName + "_dR_PtTruthEl_TruthRespEl").c_str()  );
      }

      
      Double_t pt_truth = 0.0;
      if  ( (*fAnaOptions)->IsMC() && (fCollectionName.find("Truth")  == std::string::npos) && (*fAnaOptions)->FillGenInfo() )
	{
	  GetMatchedTruth( jet ); 
	  if ( matchedTruthJet && matched_el1 && matched_el2 )
	    {
	      pt_truth = matchedTruthJet->GetFourVector().Pt();

 	      deltaRElecMin = deltaRElec1 < deltaRElec2 ? deltaRElec1 : deltaRElec2;
 	      deltaRElecTruthMin = deltaRElec1 < deltaRElec2 ? matchedTruthJet -> GetFourVector().DeltaR( MatchedElectron1Momentum ) : matchedTruthJet -> GetFourVector().DeltaR( MatchedElectron2Momentum );

	      FillTH1( minDR,    Weight, (fCollectionName + "_DeltaRJetRecoTruth").c_str()  );	      

	      FillTH3( pt_truth, pt/pt_truth, deltaRElec1,  Weight, (fCollectionName + "_dR1_PtTruth_TruthResp").c_str()  );
	      FillTH3( pt_truth, pt/pt_truth, deltaRElec2,  Weight, (fCollectionName + "_dR2_PtTruth_TruthResp").c_str()  );
	      FillTH3( pt_truth, pt/pt_truth, deltaRElecMin,  Weight, (fCollectionName + "_dRmin_PtTruth_TruthResp").c_str()  );

	      FillTH1( deltaRElecMin,    Weight, (fCollectionName + "_DeltaRmin_Jet_ElReco").c_str()  );
	      FillTH1( deltaRElecTruthMin,    Weight, (fCollectionName + "_DeltaRmin_Jet_ElTruth").c_str()  );	

 	      FillTH2( deltaRElecMin, bosonMass, Weight, (fCollectionName + "_BosMass_dRElmin").c_str()  );

	    }
	}

      if ( leadingJet == NULL ) 
        {
          leadingJet = jet;

	  TLorentzVector Recoil = -(bosonMomentum + fourVector);
	  FillTH3( bosonPt/pt, (Recoil.Rapidity() - y), y,  Weight, (fCollectionName + "_PtZPtJet1_Recoil_EtaJet1").c_str()  );

          FillTH1( pt,   Weight, (fCollectionName + "_PtJet1").c_str()  );
          FillTH1( emf,  Weight, (fCollectionName + "_EMFJet1").c_str()  );
          FillTH1( jvf,  Weight, (fCollectionName + "_JVFJet1").c_str()  );
          FillTH1( y,    Weight, (fCollectionName + "_YJet1").c_str() );
          FillTH1( ydet, Weight, (fCollectionName + "_YDetJet1").c_str()  );
	  FillTH1( mass, Weight, (fCollectionName + "_MassJet1").c_str()  );

          FillTH1(phi * ZeeDMisc::Rad2Deg,  Weight, (fCollectionName + "_PhiJet1").c_str()  );
          FillTH1(phidet * ZeeDMisc::Rad2Deg,  Weight, (fCollectionName + "_PhiDetJet1").c_str() );

	  FillTH1( deltaRElec1,  Weight, (fCollectionName + "_DeltaRJet1Elec1").c_str() );
	  FillTH1( deltaRElec2,  Weight, (fCollectionName + "_DeltaRJet1Elec2").c_str() );

	  FillTH2( pt, y,  Weight, (fCollectionName + "_PtJet1_YJet1").c_str() );
	  FillTH2( bosonPt, y,  Weight, (fCollectionName + "_PtZ_YJet1").c_str() );
	  FillTH2( bosonPt, bosonY,  Weight, (fCollectionName + "_PtZ_YZ").c_str() );
	  FillTH2( bosonPt, (bosonY + y),  Weight, (fCollectionName + "_PtZ_YZJet1").c_str() );

        }
      
      if (
          ( jet           != leadingJet)  &&
          ( leadingJet    != NULL)        &&
          ( subLeadingJet1 == NULL ))
        {
          subLeadingJet1 =  jet ;

	  FillTH1( pt,   Weight, (fCollectionName + "_PtJet2").c_str() );
	  FillTH1( emf,  Weight, (fCollectionName + "_EMFJet2").c_str()  );
	  FillTH1( jvf,  Weight, (fCollectionName + "_JVFJet2").c_str()  );
	  FillTH1( y,    Weight, (fCollectionName + "_YJet2").c_str() );
	  FillTH1( ydet, Weight, (fCollectionName + "_YDetJet2").c_str()  );
	  FillTH1( mass, Weight, (fCollectionName + "_MassJet2").c_str()  );

	  FillTH1( phi * ZeeDMisc::Rad2Deg,  Weight, (fCollectionName + "_PhiJet2").c_str()  );
	  FillTH1( phidet * ZeeDMisc::Rad2Deg,  Weight, (fCollectionName + "_PhiDetJet2").c_str() );

	  FillTH1( deltaRElec1,  Weight, (fCollectionName + "_DeltaRJet2Elec1").c_str() );
	  FillTH1( deltaRElec2,  Weight, (fCollectionName + "_DeltaRJet2Elec2").c_str() );

	  FillTH1( fourVector.DeltaR( leadingJet -> GetFourVector() ),  Weight, (fCollectionName + "_DeltaRJet1Jet2").c_str() );
	  FillTH1( fourVector.DeltaPhi(leadingJet -> GetFourVector()) * ZeeDMisc::Rad2Deg,  Weight, (fCollectionName + "_DeltaPhiJet1Jet2").c_str() );

        }
      if (
          ( jet           != leadingJet)  &&
	  ( jet           != subLeadingJet1)  &&
          ( leadingJet    != NULL)        &&
          ( subLeadingJet1 != NULL )     &&
	  ( subLeadingJet2 == NULL ))
        {
          subLeadingJet2 =  jet ;

	  FillTH1( pt,   Weight, (fCollectionName + "_PtJet3").c_str() );
	  FillTH1( emf,  Weight, (fCollectionName + "_EMFJet3").c_str()  );
	  FillTH1( jvf,  Weight, (fCollectionName + "_JVFJet3").c_str()  );
	  FillTH1( y,    Weight, (fCollectionName + "_YJet3").c_str() );
	  FillTH1( ydet, Weight, (fCollectionName + "_YDetJet3").c_str()  );
	  FillTH1( mass, Weight, (fCollectionName + "_MassJet3").c_str()  );

	  FillTH1( phi * ZeeDMisc::Rad2Deg,  Weight, (fCollectionName + "_PhiJet3").c_str()  );
	  FillTH1( phidet * ZeeDMisc::Rad2Deg,  Weight, (fCollectionName + "_PhiDetJet3").c_str() );

	  FillTH1( deltaRElec1,  Weight, (fCollectionName + "_DeltaRJet3Elec1").c_str() );
	  FillTH1( deltaRElec2,  Weight, (fCollectionName + "_DeltaRJet3Elec2").c_str() );

	  FillTH1( fourVector.DeltaR( leadingJet -> GetFourVector() ),  Weight, (fCollectionName + "_DeltaRJet1Jet3").c_str() );
	  FillTH1( fourVector.DeltaR( subLeadingJet1 -> GetFourVector() ),  Weight, (fCollectionName + "_DeltaRJet2Jet3").c_str() );

	  FillTH1( fourVector.DeltaPhi(leadingJet -> GetFourVector()) * ZeeDMisc::Rad2Deg,  Weight, (fCollectionName + "_DeltaPhiJet1Jet3").c_str() );
	  FillTH1( fourVector.DeltaPhi(subLeadingJet1 -> GetFourVector()) * ZeeDMisc::Rad2Deg,  Weight, (fCollectionName + "_DeltaPhiJet2Jet3").c_str() );

        }

      //*************** all jets **************//

      FillTH1( pt,   Weight, (fCollectionName + "_PtJet").c_str() );
      FillTH1( emf,  Weight, (fCollectionName + "_EMFJet").c_str()  );
      FillTH1( jvf,  Weight, (fCollectionName + "_JVFJet").c_str()  );
      FillTH1( y,    Weight, (fCollectionName + "_YJet").c_str() );
      FillTH1( ydet, Weight, (fCollectionName + "_YDetJet").c_str()  );
 
      FillTH1( phi * ZeeDMisc::Rad2Deg,  Weight, (fCollectionName + "_PhiJet").c_str()  );
      FillTH1( phidet * ZeeDMisc::Rad2Deg,  Weight, (fCollectionName + "_PhiDetJet").c_str() );

      FillTH1( deltaRElec1,  Weight, (fCollectionName + "_DeltaRJetElec1").c_str() );
      FillTH1( deltaRElec2,  Weight, (fCollectionName + "_DeltaRJetElec2").c_str() );

      FillTH2( pt, y,  Weight, (fCollectionName + "_PtJet_YJet").c_str() );
      FillTH2( pt, (bosonY - y)/2,  Weight, (fCollectionName + "_PtJet_YZMinYJet").c_str() );


     if  ( (*fAnaOptions)->IsMC() )
	{
	  GetMatchedTruth( jet ); 
	  if ( matchedTruthJet )
	    {
	      FillTH2( pt, matchedTruthJet -> GetFourVector().Pt(),  Weight, (fCollectionName + "_PtRecoJet_PtTruthJet").c_str()   );
	      FillTH2( y, matchedTruthJet -> GetFourVector().Rapidity(),  Weight, (fCollectionName + "_YRecoJet_YTruthJet").c_str()   );

	      FillTH1( pt,   Weight, (fCollectionName + "_PtJetMatched").c_str() );
	      FillTH1( y,    Weight, (fCollectionName + "_YJetMatched").c_str() );
	      
	      NumTruthJets++;
	    }
	}
    
      NumJets++;

    } // jet loop
  
  FillTH1( NumJets,  Weight, (fCollectionName + "_NumJets").c_str() );
  FillTH2( NumJets, NumTruthJets,  Weight, (fCollectionName + "_NumRecoJet_NumTruthJet").c_str()   );

  if ( NumJets !=0 ){ // fill boson hostogram only if jet multiplicity > 0

    FillTH1(bosonMomentum.M(),   Weight, (fCollectionName + "_Jets_BosMass").c_str()  );
    FillTH1(bosonPt,   Weight, (fCollectionName + "_Jets_BosPt").c_str()  );
    FillTH1(bosonMomentum.Rapidity(),   Weight, (fCollectionName + "_Jets_BosY").c_str()  );
    FillTH1(bosonMomentum.Phi(),   Weight, (fCollectionName + "_Jets_BosPhi").c_str()  );

  }// endif fill boson hostogram only if jet multiplicity > 0

  return;
}


// match reco jet to truth jet
void ZeeDHistManagerZplusJet::GetMatchedTruth(  ZeeDJet*& input_reco_jet )
{
  matchedTruthJet = NULL;

  const ZeeDEvent* event = GetEvent();
  CHECK_NULL_PTR(event);

  const TObjArray *truthJets = event->GetJets( fTruthCollectionName, kTRUE); 
  const TObjArray *recoJets  = event->GetJets( fCollectionName, kTRUE);

  if ( truthJets == NULL ) return;
  CHECK_NULL_PTR( truthJets );

  minDR = jetCone;
  Int_t matchRecoTruthIndex = -1;
  Int_t matchTruthRecoIndex = -1;

  ZeeDJet* selectedTruthJet, *selectedRecoJet;
  
  for (Int_t ijet = 0;  ijet < truthJets -> GetEntries(); ++ijet)
    {
      const ZeeDJet* truth_jet =dynamic_cast<ZeeDJet*>( truthJets->At(ijet) ) ;
      CHECK_NULL_PTR( truth_jet );

      Double_t deltaRElec1 = truth_jet -> GetFourVector().DeltaR( Electron1Momentum ); // should be truth electron?
      Double_t deltaRElec2 = truth_jet -> GetFourVector().DeltaR( Electron2Momentum ); // should be truth electron?
      Bool_t isIsolated = true;
      if ( ( deltaRElec1 <= deltaRcut ) || ( deltaRElec2 <= deltaRcut ) ) isIsolated = false;
      if ( !isIsolated ) continue;

      if ( TMath::Abs(truth_jet -> GetFourVector().Rapidity() ) > jetYCut ) continue;
      if ( truth_jet -> GetFourVector().Pt() < jetPtCut ) continue;

      Double_t _deltaR = truth_jet->GetFourVector().DeltaR( input_reco_jet -> GetFourVector() ); 
      if (  _deltaR <= minDR )
	{
	  minDR = _deltaR;
	  matchRecoTruthIndex = ijet;
	}
    }
  
  Double_t minDRTruthReco = jetCone;
  if (matchRecoTruthIndex >= 0 ){
    // check if the closest reco jet for selected truth jet is our input jet 
    selectedTruthJet =  dynamic_cast<ZeeDJet*>( truthJets->At( matchRecoTruthIndex ) ) ;

    for (Int_t ijet = 0;  ijet < recoJets -> GetEntries(); ++ijet)
      {
      const ZeeDJet* reco_jet =dynamic_cast<ZeeDJet*>( recoJets->At(ijet) ) ;
      CHECK_NULL_PTR( reco_jet );

      Double_t deltaRElec1 = reco_jet->GetFourVector().DeltaR( Electron1Momentum );
      Double_t deltaRElec2 = reco_jet->GetFourVector().DeltaR( Electron2Momentum );
      Bool_t isIsolated = true;
      if ( ( deltaRElec1 <= deltaRcut ) || ( deltaRElec2 <= deltaRcut ) ) isIsolated = false;
      if ( !isIsolated ) continue;

      if ( TMath::Abs(reco_jet -> GetFourVector().Rapidity() ) > jetYCut ) continue;
      if ( reco_jet -> GetFourVector().Pt() < jetPtCut ) continue;

      Double_t _deltaR = reco_jet->GetFourVector().DeltaR( selectedTruthJet->GetFourVector() ); 
      if (  _deltaR <= minDRTruthReco )
	{
	  minDRTruthReco = _deltaR;
	  matchTruthRecoIndex = ijet;
	}
    }
    
    selectedRecoJet = dynamic_cast<ZeeDJet*>( recoJets->At( matchTruthRecoIndex ) ) ;

    if (selectedRecoJet == input_reco_jet){
      matchedTruthJet = selectedTruthJet;
      //std::cout<<"works in both sides"<<std::endl;
    }

    //    matchedTruthJet =  dynamic_cast<ZeeDJet*>( truthJets->At( matchIndex ) ) ;
  } 
  return;
}

// function finds minimal dR between input reconstructed electron and jet from input collection, 
// returns dR minimum between reco electron and reco jet and dR between corresponding truth electron and truth jet.
// returns minDRElReco=10, minDRElTruth = 10 if no jet passing cuts was found.
void ZeeDHistManagerZplusJet::GetMinDR( TLorentzVector electronRecoMomentum, TLorentzVector electronTruthMomentum, TString collectionName )
{
  const ZeeDEvent* event = GetEvent();
  CHECK_NULL_PTR(event);

  const TObjArray *Jets = event->GetJets( collectionName, kTRUE); // don't warn if missing, just continue

  if ( Jets == NULL ) return;
  CHECK_NULL_PTR( Jets );

  minDRElReco = 10;
  minDRElTruth = 10;
  
  for (Int_t ijet = 0;  ijet< Jets->GetEntries(); ++ijet)
    {
      ZeeDJet* jet =dynamic_cast<ZeeDJet*>( Jets->At(ijet) ) ;
      CHECK_NULL_PTR( jet );

      Double_t deltaRElec1 = jet->GetFourVector().DeltaR( Electron1Momentum );
      Double_t deltaRElec2 = jet->GetFourVector().DeltaR( Electron2Momentum );
      Bool_t isIsolated = true;
 
      if ( ( deltaRElec1 <= deltaRcut ) || ( deltaRElec2 <= deltaRcut ) ) isIsolated = false;

      if ( !isIsolated ) continue;

      if (( jet -> GetFourVector().Pt() < jetPtJVFCut )&&( TMath::Abs(jet -> GetFourVector().Rapidity()) < trackingAcceptance )&&( TMath::Abs(jet -> GetJVF()) < jetJVFCut )) continue;
      if ( TMath::Abs(jet -> GetFourVector().Rapidity() ) > jetYCut ) continue;

      if (jet -> GetFourVector().Pt() < jetPtCut) continue;

      GetMatchedTruth( jet ); 
      if ( !matchedTruthJet ) continue;

      Double_t _deltaR = jet -> GetFourVector().DeltaR( electronRecoMomentum ); 

      if (  _deltaR <= minDRElReco )
	{
	  minDRElReco = _deltaR;
	  minDRElTruth = matchedTruthJet -> GetFourVector().DeltaR( electronTruthMomentum );
	}
    }
  return;
}

// function finds minimal dR between input reconstructed electron and jet from input collection. 
// returns minDRElReco=10 if no jet passing cuts was found.
void ZeeDHistManagerZplusJet::GetMinDRInData( TLorentzVector electronRecoMomentum, TString collectionName )
{
  const ZeeDEvent* event = GetEvent();
  CHECK_NULL_PTR(event);

  const TObjArray *Jets = event->GetJets( collectionName, kTRUE); // don't warn if missing, just continue

  if ( Jets == NULL ) return;
  CHECK_NULL_PTR( Jets );

  minDRElReco = 10;
  
  for (Int_t ijet = 0;  ijet< Jets->GetEntries(); ++ijet)
    {
      ZeeDJet* jet =dynamic_cast<ZeeDJet*>( Jets->At(ijet) ) ;
      CHECK_NULL_PTR( jet );

      Double_t deltaRElec1 = jet->GetFourVector().DeltaR( Electron1Momentum );
      Double_t deltaRElec2 = jet->GetFourVector().DeltaR( Electron2Momentum );
      Bool_t isIsolated = true;
 
      if ( ( deltaRElec1 <= deltaRcut ) || ( deltaRElec2 <= deltaRcut ) ) isIsolated = false;

      if ( !isIsolated ) continue;

      if (( jet -> GetFourVector().Pt() < jetPtJVFCut )&&( TMath::Abs(jet -> GetFourVector().Rapidity()) < trackingAcceptance )&&( TMath::Abs(jet -> GetJVF()) < jetJVFCut )) continue;
      if ( TMath::Abs(jet -> GetFourVector().Rapidity() ) > jetYCut ) continue;

      if (jet -> GetFourVector().Pt() < jetPtCut) continue;

      Double_t _deltaR = jet -> GetFourVector().DeltaR( electronRecoMomentum ); 

      if (  _deltaR <= minDRElReco )
	{
	  minDRElReco = _deltaR;	
	}
    }
  return;
}
