#include "ZeeDHistManager/ZeeDHistManagerValidation.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDEtMiss.h"
#include "ZeeDEvent/ZeeDJet.h"
#include "ZeeDEvent/ZeeDTrack.h"
#include "ZeeDEvent/ZeeDGenParticle.h"
#include "ZeeDEvent/ZeeDVertex.h"

#include "ZeeDTools/ZeeDBags.h"
#include "ZeeDTools/ZeeDSystematics.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"


//#define USE_DEBUG_MESSAGES
//#define DEBUG_ON 1
#include "ZeeDMisc/ZeeDMisc.h"

using std::cout;
using std::endl;

//-----------------------------------------------------
void ZeeDHistManagerValidation::BookHistos()
{

  // Get analysis svc
  ServiceHandle<IZeeDAnalysisSvc>* ZeeDAnaOptions = fSvcHelper.GetAnaSvc();
      
  //********Boson histograms********
  const TString BosY	       = ZeeDMisc::FindFile("Binning/BosY.txt");
  const TString BosPt	       = ZeeDMisc::FindFile("Binning/BosPt.txt");
  const TString BosMass        = ZeeDMisc::FindFile("Binning/BosMass.txt");
  const TString BosCosTh       = ZeeDMisc::FindFile("Binning/BosCosTh.txt");
  double ZMassCutVeryLooseMin  = (*fAnaOptions)->ZMassCutVeryLooseMin();
  double ZMassCutVeryLooseMax  = (*fAnaOptions)->ZMassCutVeryLooseMax();
      
  AddTH1D("BosPt",             BosPt,	       "P_{T} (Z->ee) / GeV", "");
  AddTH1D("BosMassVeryLoose", (int)(ZMassCutVeryLooseMax-ZMassCutVeryLooseMin)*2, ZMassCutVeryLooseMin, ZMassCutVeryLooseMax, "M_{Z->ee} / GeV","");
  AddTH1D("BosCosThStarCS",    BosCosTh,       "Cos(Th*)_{CS}",       "");
  AddTH1D("BosY",              BosY,	       "y (Z->ee)",	      "");
  AddTH1D("BosZVtx",           200, -500, 500, "Zvtx",  	      "");
  AddTH1D("BosCharge",         5,   -2.5, 2.5, "BosCharge",           "");
  
  
  //********Electron histograms********
  const TString ElecEta   = ZeeDMisc::FindFile("Binning/ElecEta.txt");
  const TString ElecPt    = ZeeDMisc::FindFile("Binning/ElecPt.txt");  
  
  AddTH1D("ElecEta",      ElecEta,           "#eta^{elec}",	   "");
  AddTH1D("ElecPt",       ElecPt,            "P_{T}^{elec} / GeV", "");  
  AddTH1D("ElecPtMin",	   50,  0.0,  100.0, "P_{T}^{elec} / GeV", "");
  AddTH1D("ElecPtMax",	   50,  0.0,  100.0, "P_{T}^{elec} / GeV", "");
  AddTH1D("ElecEtaMinPt", 100, -5.0,    5.0, "#eta^{elec}",	   "");
  AddTH1D("ElecEtaMaxPt", 100, -5.0,    5.0, "#eta^{elec}",	   "");  
 
  
  //********Event histograms******** 
  AddTH1D("VtxZPos",      30,  -300., 300.,  "Z_{vtx} / mm",       "");
  AddTH1D("Period",        1,   0.0,     1,  "Period",             "");
  // Run range histogram
  Int_t runfirst = (*ZeeDAnaOptions)->FirstRunHistogram();
  Int_t runlast  = (*ZeeDAnaOptions)->LastRunHistogram();

  if ( (runfirst < runlast) && (runfirst > 0) ) {
      AddTH1D("RunNumber", runlast - runfirst + 1, runfirst, runlast + 1, " Run Number", "");
  }  
  AddTH1D("Nvtx", 	   70,	 0.5,  70.5,   "N_{vtx}",      "");
  AddTH1D("Mu",           101,  -0.5, 100.5,   "Mu",           "Average Interactions Per Crossing");
  AddTH1D("ActualMu",     101,  -0.5, 100.5,   "Mu(actual)",   "Actual  Interactions Per Crossing");
  
  UInt_t nChains = (*ZeeDAnaOptions)->TriggerEvent().size();
  AddTH1D("Trigger",  nChains,   0.0, nChains, "Chain",        "Passed");
  AddTH1D("EventWeight", 1010,   -2., 200.0,   "Event weight", "");    
      
      
  //********EtMiss histograms********
  AddTH1D("EtMiss",        20,   0.0, 200.0,   "E_{T}^{miss} [GeV]", "" );
  

  //********Jet histograms********
  std::vector<std::string> collections = (*fAnaOptions)->JetCollectionNames();
  for (unsigned int i = 0; i < collections.size(); i++)
    {
      //no pt cut
      AddTH1D(collections[i] + "_PtJet_NoPtCut" ,    300, 0.0, 300,   "P_{T} (jet) / GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_YJet_NoPtCut",      51, -5, 5,  "y (jet) ", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_PhiJet_NoPtCut",    180, -180, 180,  "#Phi (jet) [deg]", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_YDetJet_NoPtCut",   51, -5, 5,  "#eta (jet) ", "", ZeeDHistLevel::Analysis);

      AddTH1D(collections[i] + "_JVFJet_NoPtCut",    321, -1.1, 2.1,  "JVF (jet)"        , "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_EMFJet_NoPtCut",    321, -1.1, 2.1,  "EMF (jet) ", "", ZeeDHistLevel::Analysis);

      AddTH1D(collections[i] + "_nTrk_pv0_500MeV_Jet_NoPtCut",       31, -0.5, 30.5, "nTrk_pv0_500MeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_sumPtTrk_pv0_500MeV_Jet_NoPtCut",   100, 0, 100,    "sumPtTrk_pv0_500MeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_nTrk_pv0_1GeV_Jet_NoPtCut",         31, -0.5, 30.5, "nTrk_pv0_1GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_sumPtTrk_pv0_1GeV_Jet_NoPtCut",     100, 0, 100,    "sumPtTrk_pv0_1GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_trackWIDTH_pv0_1GeV_Jet_NoPtCut",   210, -1.1, 1.0, "trackWIDTH_pv0_1GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_nTrk_allpv_1GeV_Jet_NoPtCut",       31, -0.5, 30.5, "nTrk_allpv_1GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_sumPtTrk_allpv_1GeV_Jet_NoPtCut" ,  100, 0, 100,    "sumPtTrk_allpv_1GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_trackWIDTH_allpv_1GeV_Jet_NoPtCut", 210, -1.1, 1.0, "trackWIDTH_allpv_1GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_WIDTH_Jet_NoPtCut",                 110, -0.1, 1.0, "WIDTH (jet)", "", ZeeDHistLevel::Analysis);

      AddTH1D(collections[i] + "_EtaOriginJet_NoPtCut",   51, -5, 5,  "EtaOrigin", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_PhiOriginJet_NoPtCut",   51, -5, 5,  "PhiOrigin", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_MOriginJet_NoPtCut",     200, 0, 200,  "MOrigin", "", ZeeDHistLevel::Analysis);

      AddTH1D(collections[i] + "_ActiveAreaEJet_NoPtCut",     100, 0, 200,  "areaE", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_ActiveAreaPxJet_NoPtCut",    51, -5,5,  "areaPx", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_ActiveAreaPyJet_NoPtCut",    51, -5, 5,  "areaPy", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_ActiveAreaPzJet_NoPtCut",    100, -100, 100,  "areaPz", "", ZeeDHistLevel::Analysis);     

      AddTH1D(collections[i] + "_IsBadLooseMinus_NoPtCut",    2, -0.5, 1.5,  "IsBadLooseMinus", "", ZeeDHistLevel::Analysis);  
      AddTH1D(collections[i] + "_IsBadLoose_NoPtCut",    2, -0.5, 1.5,  "IsBadLoose", "", ZeeDHistLevel::Analysis);    
      AddTH1D(collections[i] + "_IsBadMedium_NoPtCut",    2, -0.5, 1.5,  "IsBadMedium", "", ZeeDHistLevel::Analysis);   
      AddTH1D(collections[i] + "_IsBadTight_NoPtCut",    2, -0.5, 1.5,  "IsBadTight", "", ZeeDHistLevel::Analysis);   
 
      AddTH1D(collections[i] + "_PtConstituentsJet_NoPtCut" ,        300, 0.0, 300,   "P_{T} (jet constituents) / GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_EnergyConstituentsJet_NoPtCut" ,    300, 0.0, 300,   "E (jet constituents) / GeV", "", ZeeDHistLevel::Analysis);

      // cut pT 10 GeV
      AddTH1D(collections[i] + "_PtJet_10GeV" ,    300, 0.0, 300,   "P_{T} (jet) / GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_YJet_10GeV",      51, -5, 5,  "y (jet) ", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_PhiJet_10GeV",    180, -180, 180,  "#Phi (jet) [deg]", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_YDetJet_10GeV",   51, -5, 5,  "#eta (jet) ", "", ZeeDHistLevel::Analysis);

      AddTH1D(collections[i] + "_JVFJet_10GeV",    321, -1.1, 2.1,  "JVF (jet)"        , "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_EMFJet_10GeV",    321, -1.1, 2.1,  "EMF (jet) ", "", ZeeDHistLevel::Analysis);

      AddTH1D(collections[i] + "_nTrk_pv0_500MeV_Jet_10GeV",       31, -0.5, 30.5, "nTrk_pv0_500MeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_sumPtTrk_pv0_500MeV_Jet_10GeV",   100, 0, 100,    "sumPtTrk_pv0_500MeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_nTrk_pv0_1GeV_Jet_10GeV",         31, -0.5, 30.5, "nTrk_pv0_1GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_sumPtTrk_pv0_1GeV_Jet_10GeV",     100, 0, 100,    "sumPtTrk_pv0_1GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_trackWIDTH_pv0_1GeV_Jet_10GeV",   210, -1.1, 1.0, "trackWIDTH_pv0_1GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_nTrk_allpv_1GeV_Jet_10GeV",       31, -0.5, 30.5, "nTrk_allpv_1GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_sumPtTrk_allpv_1GeV_Jet_10GeV" ,  100, 0, 100,    "sumPtTrk_allpv_1GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_trackWIDTH_allpv_1GeV_Jet_10GeV", 210, -1.1, 1.0, "trackWIDTH_allpv_1GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_WIDTH_Jet_10GeV",                 110, -0.1, 1.0, "WIDTH (jet)", "", ZeeDHistLevel::Analysis);

      AddTH1D(collections[i] + "_EtaOriginJet_10GeV",   51, -5, 5,  "EtaOrigin", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_PhiOriginJet_10GeV",   51, -5, 5,  "PhiOrigin", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_MOriginJet_10GeV",     200, 0, 200,  "MOrigin", "", ZeeDHistLevel::Analysis);

      AddTH1D(collections[i] + "_ActiveAreaEJet_10GeV",     100, 0, 200,  "areaE", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_ActiveAreaPxJet_10GeV",    51, -5,5,  "areaPx", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_ActiveAreaPyJet_10GeV",    51, -5, 5,  "areaPy", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_ActiveAreaPzJet_10GeV",    100, -100, 100,  "areaPz", "", ZeeDHistLevel::Analysis);  

      AddTH1D(collections[i] + "_IsBadLooseMinus_10GeV",    2, -0.5, 1.5,  "IsBadLooseMinus", "", ZeeDHistLevel::Analysis);  
      AddTH1D(collections[i] + "_IsBadLoose_10GeV",    2, -0.5, 1.5,  "IsBadLoose", "", ZeeDHistLevel::Analysis);    
      AddTH1D(collections[i] + "_IsBadMedium_10GeV",    2, -0.5, 1.5,  "IsBadMedium", "", ZeeDHistLevel::Analysis);   
      AddTH1D(collections[i] + "_IsBadTight_10GeV",    2, -0.5, 1.5,  "IsBadTight", "", ZeeDHistLevel::Analysis);   

      AddTH1D(collections[i] + "_PtConstituentsJet_10GeV" ,        300, 0.0, 300,   "P_{T} (jet constituents) / GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_EnergyConstituentsJet_10GeV" ,    300, 0.0, 300,   "E (jet constituents) / GeV", "", ZeeDHistLevel::Analysis);

      // cut pT 20 GeV
      AddTH1D(collections[i] + "_PtJet_20GeV" ,    300, 0.0, 300,   "P_{T} (jet) / GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_YJet_20GeV",      51, -5, 5,  "y (jet) ", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_PhiJet_20GeV",    180, -180, 180,  "#Phi (jet) [deg]", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_YDetJet_20GeV",   51, -5, 5,  "#eta (jet) ", "", ZeeDHistLevel::Analysis);

      AddTH1D(collections[i] + "_JVFJet_20GeV",    321, -1.1, 2.1,  "JVF (jet)"        , "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_EMFJet_20GeV",    321, -1.1, 2.1,  "EMF (jet) ", "", ZeeDHistLevel::Analysis);

      AddTH1D(collections[i] + "_nTrk_pv0_500MeV_Jet_20GeV",       31, -0.5, 30.5, "nTrk_pv0_500MeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_sumPtTrk_pv0_500MeV_Jet_20GeV",   100, 0, 100,    "sumPtTrk_pv0_500MeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_nTrk_pv0_1GeV_Jet_20GeV",         31, -0.5, 30.5, "nTrk_pv0_1GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_sumPtTrk_pv0_1GeV_Jet_20GeV",     100, 0, 100,    "sumPtTrk_pv0_1GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_trackWIDTH_pv0_1GeV_Jet_20GeV",   210, -1.1, 1.0, "trackWIDTH_pv0_1GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_nTrk_allpv_1GeV_Jet_20GeV",       31, -0.5, 30.5, "nTrk_allpv_1GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_sumPtTrk_allpv_1GeV_Jet_20GeV" ,  100, 0, 100,    "sumPtTrk_allpv_1GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_trackWIDTH_allpv_1GeV_Jet_20GeV", 210, -1.1, 1.0, "trackWIDTH_allpv_1GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_WIDTH_Jet_20GeV",                 110, -0.1, 1.0, "WIDTH (jet)", "", ZeeDHistLevel::Analysis);

      AddTH1D(collections[i] + "_EtaOriginJet_20GeV",   51, -5, 5,  "EtaOrigin", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_PhiOriginJet_20GeV",   51, -5, 5,  "PhiOrigin", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_MOriginJet_20GeV",     200, 0, 200,  "MOrigin", "", ZeeDHistLevel::Analysis);

      AddTH1D(collections[i] + "_ActiveAreaEJet_20GeV",     100, 0, 200,  "areaE", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_ActiveAreaPxJet_20GeV",    51, -5,5,  "areaPx", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_ActiveAreaPyJet_20GeV",    51, -5, 5,  "areaPy", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_ActiveAreaPzJet_20GeV",    100, -100, 100,  "areaPz", "", ZeeDHistLevel::Analysis);      

      AddTH1D(collections[i] + "_IsBadLooseMinus_20GeV",    2, -0.5, 1.5,  "IsBadLooseMinus", "", ZeeDHistLevel::Analysis);  
      AddTH1D(collections[i] + "_IsBadLoose_20GeV",    2, -0.5, 1.5,  "IsBadLoose", "", ZeeDHistLevel::Analysis);    
      AddTH1D(collections[i] + "_IsBadMedium_20GeV",    2, -0.5, 1.5,  "IsBadMedium", "", ZeeDHistLevel::Analysis);   
      AddTH1D(collections[i] + "_IsBadTight_20GeV",    2, -0.5, 1.5,  "IsBadTight", "", ZeeDHistLevel::Analysis);

      AddTH1D(collections[i] + "_PtConstituentsJet_20GeV" ,        300, 0.0, 300,   "P_{T} (jet constituents) / GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_EnergyConstituentsJet_20GeV" ,    300, 0.0, 300,   "E (jet constituents) / GeV", "", ZeeDHistLevel::Analysis);

    }

}
//-----------------------------------------------------
void ZeeDHistManagerValidation::Fill()
{
  DEBUG_MES_START;
  DEBUG_MES(this->GetHMName());

  // Get event
  const ZeeDEvent* event = GetEvent();
  CHECK_NULL_PTR(event);

  // Event weight
  Double_t Weight = event->GetWeight();

  // Arrange loop over bosons
  if (bIsAll) {
  
    // All bosons
    const TObjArray* bosons = (bIsW) ?  event->GetWBosons() : event->GetZBosons();

    if (bosons != NULL ) {

      for ( Int_t index = 0; index < bosons->GetEntriesFast(); ++index ) {
	const ZeeDBosonZ* boson = static_cast<ZeeDBosonZ*>( bosons->At(index) );
	FillBosonHists(boson, Weight);		        
	std::vector<std::string> collections = (*fAnaOptions)->JetCollectionNames();
	for (unsigned int i = 0; i < collections.size(); i++){
	  fCollectionName = collections[i];
	  TString _truthCollection( fCollectionName.c_str() );
	  _truthCollection.ReplaceAll("TopoEM","Truth");
	  _truthCollection.ReplaceAll("LCTopo","Truth");
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
      FillBosonHists(event->GetCurrentBoson(), Weight);
      
      std::vector<std::string> collections = (*fAnaOptions)->JetCollectionNames();
      for (unsigned int i = 0; i < collections.size(); i++){
	fCollectionName = collections[i];
	TString _truthCollection( fCollectionName.c_str() );
	_truthCollection.ReplaceAll("TopoEM","Truth");
	_truthCollection.ReplaceAll("LCTopo","Truth");  
	_truthCollection.ReplaceAll("New","");
	fTruthCollectionName = _truthCollection.Data();
	  
	const TObjArray *jets = event->GetJets( fCollectionName , kFALSE);
	if ( jets )  FillJetHists( jets,  Weight);
      }
    }
  }
  
  // Arrange loop over electrons:
  const TObjArray* electrons = event->GetElectrons();
  if (electrons != NULL ) FillElectronHists(electrons, Weight);

  FillEventHists(event, Weight);
  
  const TObjArray* etmissarr = event->GetEtMissArray();
  FillEtMissHists(etmissarr, Weight);

  DEBUG_MES_END;
}

//-----------------------------------------------------
void ZeeDHistManagerValidation::FillBosonHists(const ZeeDBosonZ* boson, Double_t Weight)
{
  // Fill histograms for a single boson
  if (boson == NULL) {
    return;
  }

  const TLorentzVector& fourVector = boson->GetFourVector();

  // Check boson Y range if fYmin != fYmax
  if (fYmin != fYmax ) {
    Double_t yBoson   = fourVector.Rapidity();

    if (yBoson >= fYmax ) {
	return;
    }

    if (yBoson <  fYmin ) {
	return;
    }
  }

  Double_t bosMass = fourVector.M();

  FillTH1(fourVector.Pt(),  Weight, "BosPt");
  FillTH1(bosMass,   Weight, "BosMassVeryLoose");
  

  const double cosThStarCS = boson->GetCosThStar(ZeeDEnum::CosThStar::CollinsSoper);
  FillTH1(cosThStarCS, Weight, "BosCosThStarCS");

  FillTH1(fourVector.Rapidity(), Weight, "BosY");
  
  TVector3 Vtx = boson->GetVertex();
  FillTH1(Vtx.Pz(), Weight, "BosZVtx");
  
  FillTH1(boson->GetCharge(), Weight, "BosCharge");

}

//-----------------------------------------------------
void ZeeDHistManagerValidation::FillElectronHists(const TObjArray* electrons, Double_t Weight)
{

  if (electrons == NULL) {
    return;
  }
  
  Double_t ptMax = 0;
  Double_t ptMin = 1000000.0;
  Double_t etaMinPt = 0;
  Double_t etaMaxPt = 0;  
  
  for ( Int_t i = 0; i < electrons->GetEntriesFast(); ++i ) {  
    const ZeeDElectron* electron = static_cast< ZeeDElectron* > (electrons->At(i));
    const TLorentzVector& fourVector = electron->GetFourVector();
  
    if (fourVector.Pt() < ptMin) {
      ptMin    = fourVector.Pt();
      etaMinPt = fourVector.Eta();
    }
  
    if (fourVector.Pt() > ptMax) {
      ptMax    = fourVector.Pt();
      etaMaxPt = fourVector.Eta();
    }
    
    FillTH1(fourVector.Pt(),	Weight, "ElecPt");
    FillTH1(fourVector.Eta(),	Weight, "ElecEta");
      
  } 
  
  FillTH1(ptMax,	      Weight, "ElecPtMax");
  FillTH1(etaMaxPt,	      Weight, "ElecEtaMaxPt");
  FillTH1(ptMin,	      Weight, "ElecPtMin");
  FillTH1(etaMinPt,	      Weight, "ElecEtaMinPt");  
   
}

//-----------------------------------------------------
void ZeeDHistManagerValidation::FillEventHists(const ZeeDEvent* event, Double_t Weight)
{

  // Get array of vertices
  const TObjArray* vertices = event->GetVertices();
  
  if ( vertices->GetEntriesFast() > 0 ) {
  
    ZeeDVertex* vertex = static_cast< ZeeDVertex* >( vertices->At(0) );
    TVector3 vtx3Vector = vertex->GetVertexCandidate();
    FillTH1(vtx3Vector.Z(), Weight, "VtxZPos"); 
  
  }
  
  const TString period = ZeeDEnum::Period::ToString(event->GetPeriod());

  FillTH1(period, Weight, "Period");
  
  // Fill Run number
  if ( event->RunNumber().IsSet() ) {
    Int_t runNumber = event->RunNumber().Get();
    FillTH1(runNumber, Weight, "RunNumber");
  }

  if ( event->Nvtx().IsSet() ) {
    Int_t NumVtx = event->Nvtx().Get();
    FillTH1(NumVtx, Weight, "Nvtx");
  }
  if ( event->AverageInteractionsPerCrossing().IsSet() ) {
    const Double_t Mu = event->AverageInteractionsPerCrossing().Get();
    FillTH1(Mu, Weight, "Mu");
  }

  if ( event->ActualInteractionsPerCrossing().IsSet() ) {
    const Double_t Mu = event->ActualInteractionsPerCrossing().Get();
    FillTH1(Mu, Weight, "ActualMu");
  }
  if ( event->Trigger().IsSet() ) {

    const ZeeDEventBags::Trigger triggerBag = event->Trigger().Get();
    ZeeDEventBags::Trigger::TriggerMap::const_iterator iter = triggerBag.hasPassedChain.begin();
    ZeeDEventBags::Trigger::TriggerMap::const_iterator iterEnd = triggerBag.hasPassedChain.end();

    for(; iter != iterEnd; ++iter) {
      TString chain	= ZeeDEnum::Trigger::ToString(iter->first);
      bool    hasPassed = iter->second;

      FillTH1(chain, hasPassed * Weight, "Trigger");
    }
  }    
    
  FillTH1(Weight, 1., "EventWeight");        
    
}

//-----------------------------------------------------
void ZeeDHistManagerValidation::FillEtMissHists(const TObjArray* etmissarr, Double_t Weight )
{

  for (int i = 0; i < etmissarr->GetEntriesFast(); ++i) {
  
    ZeeDEtMiss* etmiss = static_cast< ZeeDEtMiss* >( etmissarr->At(i) );
    FillTH1(etmiss->GetEt(),  Weight, "EtMiss");
    
  }	
      
}

//-----------------------------------------------------

void ZeeDHistManagerValidation::FillJetHists(const TObjArray* jets, Double_t Weight )
{
  const ZeeDEvent* event = GetEvent();
  CHECK_NULL_PTR(event);

  TLorentzVector Electron1Momentum = event->GetCurrentBoson()->GetFirstLep()->GetFourVector();
  TLorentzVector Electron2Momentum = event->GetCurrentBoson()->GetSecondLep()->GetFourVector();

  for (Int_t ijet = 0;  ijet< jets->GetEntries(); ++ijet)
    {
      ZeeDJet* jet =dynamic_cast<ZeeDJet*>( jets->At(ijet) ) ;
      CHECK_NULL_PTR( jet );

      TLorentzVector fourVector = jet -> GetFourVector();
      TLorentzVector fourVectorCS = jet->GetFourVectorCS();

      Double_t pt        = fourVector.Pt();
      Double_t y         = fourVector.Rapidity();
      Double_t phi       = fourVector.Phi();
      Double_t ydet      = fourVectorCS.Eta();

      Double_t emf       = jet->GetEmFraction();
      Double_t jvf       = jet->GetJVF();

      Double_t  nTrk_pv0_500MeV          = jet->GetnTrk_pv0_500MeV(); 
      Double_t  sumPtTrk_pv0_500MeV      = jet->GetsumPtTrk_pv0_500MeV(); 
      Double_t  nTrk_pv0_1GeV            = jet->GetnTrk_pv0_1GeV(); 
      Double_t  sumPtTrk_pv0_1GeV        = jet->GetsumPtTrk_pv0_1GeV();
      Double_t  trackWIDTH_pv0_1GeV      = jet->GettrackWIDTH_pv0_1GeV(); 
      Double_t  nTrk_allpv_1GeV          = jet->GetnTrk_allpv_1GeV();
      Double_t  sumPtTrk_allpv_1GeV      = jet->GetsumPtTrk_allpv_1GeV(); 
      Double_t  trackWIDTH_allpv_1GeV    = jet->GettrackWIDTH_allpv_1GeV();
      Double_t  width                    = jet->GetWIDTH();

      Double_t areaPx = jet->GetActiveAreaPx();
      Double_t areaPy = jet->GetActiveAreaPy();
      Double_t areaPz = jet->GetActiveAreaPz();
      Double_t areaE  = jet->GetActiveAreaE();

      Double_t EtaOrigin = jet->GetEtaOrigin();
      Double_t PhiOrigin = jet->GetPhiOrigin();
      Double_t MOrigin   = jet->GetMOrigin(); 

      Bool_t IsBadLooseMinus = jet->GetIsBadLooseMinus();
      Bool_t IsBadLoose      = jet->GetIsBadLoose();
      Bool_t IsBadMedium     = jet->GetIsBadMedium();
      Bool_t IsBadTight      = jet->GetIsBadTight();

      //**********new ttrees only**********//
      //std::vector<double> constituents_pt = jet->GetJetConstituentsPt();
      //std::vector<double> constituents_energy = jet->GetJetConstituentsEnergy();

      Double_t deltaRElec1 = fourVector.DeltaR( Electron1Momentum );
      Double_t deltaRElec2 = fourVector.DeltaR( Electron2Momentum );

      Bool_t isIsolated = true;
      if ( ( deltaRElec1 <= 0.35 ) || ( deltaRElec2 <= 0.35 ) ) isIsolated = false;
      if ( !isIsolated ) continue;

      FillTH1(pt,    Weight, (fCollectionName + "_PtJet_NoPtCut").c_str() );
      FillTH1(y,     Weight, (fCollectionName + "_YJet_NoPtCut").c_str() );
      FillTH1(phi* ZeeDMisc::Rad2Deg,   Weight, (fCollectionName + "_PhiJet_NoPtCut").c_str() );
      FillTH1(ydet,  Weight, (fCollectionName + "_YDetJet_NoPtCut").c_str() );
      FillTH1(jvf,   Weight, (fCollectionName + "_JVFJet_NoPtCut").c_str() );
      FillTH1(emf,   Weight, (fCollectionName + "_EMFJet_NoPtCut").c_str() );

      FillTH1(nTrk_pv0_500MeV, Weight, (fCollectionName +  "_nTrk_pv0_500MeV_Jet_NoPtCut").c_str() );
      FillTH1(sumPtTrk_pv0_500MeV/GeV, Weight, (fCollectionName +  "_sumPtTrk_pv0_500MeV_Jet_NoPtCut").c_str() );
      FillTH1(nTrk_pv0_1GeV, Weight, (fCollectionName +  "_nTrk_pv0_1GeV_Jet_NoPtCut").c_str() );
      FillTH1(sumPtTrk_pv0_1GeV/GeV, Weight, (fCollectionName +  "_sumPtTrk_pv0_1GeV_Jet_NoPtCut").c_str() );
      FillTH1(trackWIDTH_pv0_1GeV, Weight, (fCollectionName +  "_trackWIDTH_pv0_1GeV_Jet_NoPtCut").c_str() );
      FillTH1(nTrk_allpv_1GeV, Weight, (fCollectionName +  "_nTrk_allpv_1GeV_Jet_NoPtCut").c_str() );
      FillTH1(sumPtTrk_allpv_1GeV/GeV, Weight, (fCollectionName +  "_sumPtTrk_allpv_1GeV_Jet_NoPtCut").c_str() );
      FillTH1(trackWIDTH_allpv_1GeV, Weight, (fCollectionName +  "_trackWIDTH_allpv_1GeV_Jet_NoPtCut").c_str() );
      FillTH1(width,  Weight, (fCollectionName + "_WIDTH_Jet_NoPtCut").c_str()  );

      FillTH1(EtaOrigin,   Weight, (fCollectionName + "_EtaOriginJet_NoPtCut").c_str() );
      FillTH1(PhiOrigin,   Weight, (fCollectionName + "_PhiOriginJet_NoPtCut").c_str() );
      FillTH1(MOrigin,     Weight, (fCollectionName + "_MOriginJet_NoPtCut").c_str() );

      FillTH1(areaE,    Weight, (fCollectionName + "_ActiveAreaEJet_NoPtCut").c_str() );
      FillTH1(areaPx,   Weight, (fCollectionName + "_ActiveAreaPxJet_NoPtCut").c_str() );
      FillTH1(areaPy,   Weight, (fCollectionName + "_ActiveAreaPyJet_NoPtCut").c_str() );
      FillTH1(areaPx,   Weight, (fCollectionName + "_ActiveAreaPzJet_NoPtCut").c_str() );

      FillTH1(IsBadLooseMinus,   Weight, (fCollectionName + "_IsBadLooseMinus_NoPtCut").c_str() );
      FillTH1(IsBadLoose,        Weight, (fCollectionName + "_IsBadLoose_NoPtCut").c_str() );
      FillTH1(IsBadMedium,       Weight, (fCollectionName + "_IsBadMedium_NoPtCut").c_str() );
      FillTH1(IsBadTight,        Weight, (fCollectionName + "_IsBadTight_NoPtCut").c_str() );

      //**********new ttrees only**********//
      //       for (size_t i = 0; i < constituents_pt.size(); i++){
      //        	FillTH1(constituents_pt[i],        Weight, (fCollectionName + "_PtConstituentsJet_NoPtCut").c_str() );
      //       }
      //       for (size_t i = 0; i < constituents_energy.size(); i++){
      //        	FillTH1(constituents_energy[i],        Weight, (fCollectionName + "_EnergyConstituentsJet_NoPtCut").c_str() );
      //       }
      
      if (pt >= 10){

	FillTH1(pt,    Weight, (fCollectionName + "_PtJet_10GeV").c_str() );
	FillTH1(y,     Weight, (fCollectionName + "_YJet_10GeV").c_str() );
	FillTH1(phi* ZeeDMisc::Rad2Deg,   Weight, (fCollectionName + "_PhiJet_10GeV").c_str() );
	FillTH1(ydet,  Weight, (fCollectionName + "_YDetJet_10GeV").c_str() );
	FillTH1(jvf,   Weight, (fCollectionName + "_JVFJet_10GeV").c_str() );
	FillTH1(emf,   Weight, (fCollectionName + "_EMFJet_10GeV").c_str() );

	FillTH1(nTrk_pv0_500MeV, Weight, (fCollectionName +  "_nTrk_pv0_500MeV_Jet_10GeV").c_str() );
	FillTH1(sumPtTrk_pv0_500MeV/GeV, Weight, (fCollectionName +  "_sumPtTrk_pv0_500MeV_Jet_10GeV").c_str() );
	FillTH1(nTrk_pv0_1GeV, Weight, (fCollectionName +  "_nTrk_pv0_1GeV_Jet_10GeV").c_str() );
	FillTH1(sumPtTrk_pv0_1GeV/GeV, Weight, (fCollectionName +  "_sumPtTrk_pv0_1GeV_Jet_10GeV").c_str() );
	FillTH1(trackWIDTH_pv0_1GeV, Weight, (fCollectionName +  "_trackWIDTH_pv0_1GeV_Jet_10GeV").c_str() );
	FillTH1(nTrk_allpv_1GeV, Weight, (fCollectionName +  "_nTrk_allpv_1GeV_Jet_10GeV").c_str() );
	FillTH1(sumPtTrk_allpv_1GeV/GeV, Weight, (fCollectionName +  "_sumPtTrk_allpv_1GeV_Jet_10GeV").c_str() );
	FillTH1(trackWIDTH_allpv_1GeV, Weight, (fCollectionName +  "_trackWIDTH_allpv_1GeV_Jet_10GeV").c_str() );
	FillTH1(width,  Weight, (fCollectionName + "_WIDTH_Jet_10GeV").c_str()  );

	FillTH1(EtaOrigin,   Weight, (fCollectionName + "_EtaOriginJet_10GeV").c_str() );
	FillTH1(PhiOrigin,   Weight, (fCollectionName + "_PhiOriginJet_10GeV").c_str() );
	FillTH1(MOrigin,   Weight, (fCollectionName + "_MOriginJet_10GeV").c_str() );

	FillTH1(areaE,   Weight, (fCollectionName + "_ActiveAreaEJet_10GeV").c_str() );
	FillTH1(areaPx,   Weight, (fCollectionName + "_ActiveAreaPxJet_10GeV").c_str() );
	FillTH1(areaPy,   Weight, (fCollectionName + "_ActiveAreaPyJet_10GeV").c_str() );
	FillTH1(areaPz,   Weight, (fCollectionName + "_ActiveAreaPzJet_10GeV").c_str() );

	FillTH1(IsBadLooseMinus,   Weight, (fCollectionName + "_IsBadLooseMinus_10GeV").c_str() );
	FillTH1(IsBadLoose,        Weight, (fCollectionName + "_IsBadLoose_10GeV").c_str() );
	FillTH1(IsBadMedium,       Weight, (fCollectionName + "_IsBadMedium_10GeV").c_str() );
	FillTH1(IsBadTight,        Weight, (fCollectionName + "_IsBadTight_10GeV").c_str() );

	//**********new ttrees only**********//
	//  	for (size_t i = 0; i < constituents_pt.size(); i++){
	//  	  FillTH1(constituents_pt[i],        Weight, (fCollectionName + "_PtConstituentsJet_10GeV").c_str() );
	//  	}
	//  	for (size_t i = 0; i < constituents_energy.size(); i++){
	//  	  FillTH1(constituents_energy[i],        Weight, (fCollectionName + "_EnergyConstituentsJet_10GeV").c_str() );
	//  	}

      }
      if (pt >= 20){

	FillTH1(pt,    Weight, (fCollectionName + "_PtJet_20GeV").c_str() );
	FillTH1(y,     Weight, (fCollectionName + "_YJet_20GeV").c_str() );
	FillTH1(phi* ZeeDMisc::Rad2Deg,   Weight, (fCollectionName + "_PhiJet_20GeV").c_str() );
	FillTH1(ydet,  Weight, (fCollectionName + "_YDetJet_20GeV").c_str() );
	FillTH1(jvf,   Weight, (fCollectionName + "_JVFJet_20GeV").c_str() );
	FillTH1(emf,   Weight, (fCollectionName + "_EMFJet_20GeV").c_str() );

	FillTH1(nTrk_pv0_500MeV, Weight, (fCollectionName +  "_nTrk_pv0_500MeV_Jet_20GeV").c_str() );
	FillTH1(sumPtTrk_pv0_500MeV/GeV, Weight, (fCollectionName +  "_sumPtTrk_pv0_500MeV_Jet_20GeV").c_str() );
	FillTH1(nTrk_pv0_1GeV, Weight, (fCollectionName +  "_nTrk_pv0_1GeV_Jet_20GeV").c_str() );
	FillTH1(sumPtTrk_pv0_1GeV/GeV, Weight, (fCollectionName +  "_sumPtTrk_pv0_1GeV_Jet_20GeV").c_str() );
	FillTH1(trackWIDTH_pv0_1GeV, Weight, (fCollectionName +  "_trackWIDTH_pv0_1GeV_Jet_20GeV").c_str() );
	FillTH1(nTrk_allpv_1GeV, Weight, (fCollectionName +  "_nTrk_allpv_1GeV_Jet").c_str() );
	FillTH1(sumPtTrk_allpv_1GeV/GeV, Weight, (fCollectionName +  "_sumPtTrk_allpv_1GeV_Jet_20GeV").c_str() );
	FillTH1(trackWIDTH_allpv_1GeV, Weight, (fCollectionName +  "_trackWIDTH_allpv_1GeV_Jet_20GeV").c_str() );
	FillTH1(width,  Weight, (fCollectionName + "_WIDTH_Jet_20GeV").c_str()  );

	FillTH1(EtaOrigin,   Weight, (fCollectionName + "_EtaOriginJet_20GeV").c_str() );
	FillTH1(PhiOrigin,   Weight, (fCollectionName + "_PhiOriginJet_20GeV").c_str() );
	FillTH1(MOrigin,   Weight, (fCollectionName + "_MOriginJet_20GeV").c_str() );

	FillTH1(areaE,   Weight, (fCollectionName + "_ActiveAreaEJet_20GeV").c_str() );
	FillTH1(areaPx,   Weight, (fCollectionName + "_ActiveAreaPxJet_20GeV").c_str() );
	FillTH1(areaPy,   Weight, (fCollectionName + "_ActiveAreaPyJet_20GeV").c_str() );
	FillTH1(areaPz,   Weight, (fCollectionName + "_ActiveAreaPzJet_20GeV").c_str() );

	FillTH1(IsBadLooseMinus,   Weight, (fCollectionName + "_IsBadLooseMinus_20GeV").c_str() );
	FillTH1(IsBadLoose,        Weight, (fCollectionName + "_IsBadLoose_20GeV").c_str() );
	FillTH1(IsBadMedium,       Weight, (fCollectionName + "_IsBadMedium_20GeV").c_str() );
	FillTH1(IsBadTight,        Weight, (fCollectionName + "_IsBadTight_20GeV").c_str() );

	//**********new ttrees only**********//
	//  	for (size_t i = 0; i < constituents_pt.size(); i++){
	//  	  FillTH1(constituents_pt[i],        Weight, (fCollectionName + "_PtConstituentsJet_20GeV").c_str() );
	//  	}
	//  	for (size_t i = 0; i < constituents_energy.size(); i++){
	//  	  FillTH1(constituents_energy[i],        Weight, (fCollectionName + "_EnergyConstituentsJet_20GeV").c_str() );
	//  	}

      }

    }
}
