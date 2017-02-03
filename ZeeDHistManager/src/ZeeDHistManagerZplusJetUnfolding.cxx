#include "ZeeDHistManager/ZeeDHistManagerZplusJetUnfolding.h"

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
void  ZeeDHistManagerZplusJetUnfolding::bookAnalysisCuts()
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

void ZeeDHistManagerZplusJetUnfolding::BookHistos()
{
  bookAnalysisCuts();

    // Do we run on data or MC?
    if ( (*fAnaOptions)->IsMC() ) {
        bIsMC = kTRUE;
    }
    // Books histograms

    const TString ZplusJet_PtJet_YJet     = ZeeDMisc::FindFile("Binning/ZplusJet_PtJet_YJet.txt");

    std::vector<std::string> collections = (*fAnaOptions)->JetCollectionNames();
    for (unsigned int i = 0; i < collections.size(); i++)
      {
	AddTH2D(collections[i] + "_PtJet_YJet", ZplusJet_PtJet_YJet, "P_{T} (jet) / GeV", "y (jet) ", ZeeDHistLevel::Systematics);
	AddTH2D(collections[i] + "_PtJet_YZMinYJet", ZplusJet_PtJet_YJet, "P_{T} (jet) / GeV", "#Delta y_{zjet} ", ZeeDHistLevel::Systematics);
	AddTH1D(collections[i] + "_PtJet_YJet_1D",   540, 0, 540,  " y_{jet} "     , "", ZeeDHistLevel::Analysis);
	AddTH1D(collections[i] + "_PtJet_YZMinYJet_1D",   540, 0, 540,  " #Delta y_{zjet} "     , "", ZeeDHistLevel::Analysis);
    }
}

//-----------------------------------------------------
void ZeeDHistManagerZplusJetUnfolding::Fill()
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
/*
      // matching for reco-truth electrons, only if gen info is switched
      if  ( (*fAnaOptions)->IsMC() && (*fAnaOptions)->FillGenInfo() ){
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
      }
	  */

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

void ZeeDHistManagerZplusJetUnfolding::FillJetHists(const TObjArray* jets,  Double_t Weight )
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


  Int_t NumJets=0; // NumJets/NumTruthJets - number of reconstructed/truth jets which PASSED selection cuts. 
  for (Int_t ijet = 0;  ijet< jets->GetEntries(); ++ijet)  // start loop over jets
    {

      ZeeDJet* jet =dynamic_cast< ZeeDJet* >( jets->At(ijet) ) ;
      CHECK_NULL_PTR( jet );
      
      if ( _debug_ )
	{
	  Info(__PRETTY_FUNCTION__,TString::Format(" ***** Looping jets %d",ijet));
	}

      TLorentzVector fourVector   = jet->GetFourVector();

      Double_t pt        = fourVector.Pt();
      Double_t y         = fourVector.Rapidity();

      if (!passedCuts( jet )) continue;

      //Searching pT bin to fill
      const Int_t n_pt_Bins = 6;
      const Int_t pt_Edges[n_pt_Bins+1] = { 20.,  50., 100., 200., 300., 600., 1500. };
      
      Int_t pt_Bin = 0;
      for (Int_t i_pt_Bin=0; i_pt_Bin < n_pt_Bins; i_pt_Bin++){
	if ( pt >= pt_Edges[i_pt_Bin] && pt < pt_Edges[i_pt_Bin+1] ){
	  pt_Bin = i_pt_Bin;
	  break;
	}
      }
      
      Double_t factor_mult = 10;
      Double_t factor_shift = 45+90*(pt_Bin);
      
      Double_t Obs1 = y;
      Double_t Obs2 = (bosonY - y)/2;

      //std::cout << Obs1 * factor_mult + factor_shift << "   " << Obs2 * factor_mult + factor_shift << std::endl;
      //Fill Histograms
      FillTH1( Obs1 * factor_mult + factor_shift,  Weight, (fCollectionName + "_PtJet_YJet_1D").c_str() );
      FillTH1( Obs2 * factor_mult + factor_shift,  Weight, (fCollectionName + "_PtJet_YZMinYJet_1D").c_str() );

      
      FillTH2( pt, y,  Weight, (fCollectionName + "_PtJet_YJet").c_str() );
      FillTH2( pt, (bosonY - y)/2,  Weight, (fCollectionName + "_PtJet_YZMinYJet").c_str() );
    
      NumJets++;

    } // jet loop

  return;
}

// analysis cuts
Bool_t ZeeDHistManagerZplusJetUnfolding::passedCuts( ZeeDJet*& selected_jet ){

  Bool_t passed = true;

  Double_t jvf;

  if  ( (*fAnaOptions)->IsMC() && (fCollectionName.find("Truth")  != std::string::npos) ) {
    jvf = 1.0;
  }
  else {
    jvf  = selected_jet->GetJVF();
  }

  Double_t deltaRElec1 = selected_jet -> GetFourVector().DeltaR( Electron1Momentum );
  Double_t deltaRElec2 = selected_jet -> GetFourVector().DeltaR( Electron2Momentum );

  Bool_t isIsolated = true;
  if ( (deltaRElec1 <= deltaRcut) || (deltaRElec2 <= deltaRcut) ) isIsolated = false;
  if ( !isIsolated ) passed = false; 

  if (TMath::Abs(selected_jet -> GetFourVector().Rapidity()) > jetYCut) passed = false;
  if ( selected_jet -> GetFourVector().Pt() < jetPtCut ) passed = false; 
  if ( ( selected_jet -> GetFourVector().Pt() < jetPtJVFCut ) && ( TMath::Abs(selected_jet -> GetFourVector().Rapidity()) < trackingAcceptance ) && ( TMath::Abs(jvf) < jetJVFCut ) ) 
    passed = false;
  
  return passed;
}


// match reco jet to truth jet
void ZeeDHistManagerZplusJetUnfolding::GetMatchedTruth(  ZeeDJet*& input_reco_jet )
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
      ZeeDJet* truth_jet =dynamic_cast<ZeeDJet*>( truthJets->At(ijet) ) ;
      CHECK_NULL_PTR( truth_jet );

      if (!passedCuts( truth_jet )) continue;

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
      ZeeDJet* reco_jet =dynamic_cast<ZeeDJet*>( recoJets->At(ijet) ) ;
      CHECK_NULL_PTR( reco_jet );

      if (!passedCuts( reco_jet )) continue;

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
