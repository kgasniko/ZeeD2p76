#include "ZeeDHistManager/ZeeDHistManagerTrigger.h"

// Root includes
#include <TLorentzVector.h>
#include <TMath.h>

// Analysis includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDMisc/ZeeDMisc.h"

#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDBosonZ.h"

//#define USE_DEBUG_MESSAGES

using std::cout;
using std::endl;


//-----------------------------------------------------
void ZeeDHistManagerTrigger::BookHistos()
{
  // Books histograms

  Double_t pi2 = TMath::Pi()/2.;

  AddTH1D("Ptelec1",	30, 0., 30.,	"Ptelec1");
  AddTH1D("Ptelec2",	30, 0., 30.,	"Ptelec2");
  AddTH1D("Mee", 170.,	30., 200.,	"M_{ee}");
  AddTH1D("nZ",		20., -0.5, 19.5,"N(Z)");

  // Add efficiency for 1D variables
  AddEff1D("2e12", 20, -3., 3.,		"Eta");
  AddEff1D("2e12", 30,  0., 30.,	"Pt");
  AddEff1D("2e12", 30,  0., 30.,	"E");
  AddEff1D("2e12", 30, -pi2, pi2,	"Phi");

  // Add efficiency for 2D variables
  AddEff2D("2e12", 20, -3., 3., "Eta", 30, 0., 30.,	"Pt");
  AddEff2D("2e12", 20, -3., 3., "Eta", 30, -pi2, pi2,	"Phi");

}

//-----------------------------------------------------
void ZeeDHistManagerTrigger::Fill()
{
  DEBUG_MES_START;
  DEBUG_MES(this->GetHMName());

  // Get event
  const ZeeDEvent* event = GetEvent();
  CHECK_NULL_PTR(event);

  // Event weight
  Double_t Weight = event->GetWeight();

  // Take selected boson only
  const ZeeDBosonZ* boson = event->GetCurrentBoson();
  if (boson != NULL) {
    FillTriggerHists(boson, Weight);
  }

  DEBUG_MES_END;
}

//-----------------------------------------------------
void ZeeDHistManagerTrigger::FillTriggerHists(const ZeeDBosonZ* boson, Double_t Weight)
{

  // Get hist pointers back

  // ------------------======= Fill histograms for a single boson =========------------------------

  // Count number of bosons in the event
  const ZeeDEvent* event = GetEvent();
  static int currentRun		= 0;
  static int currentEvent	= 0;
  static int n_bosons		= 1;
  int Run = event->RunNumber().Get();
  int Event = event->EventNumber().Get();
  if (Run != currentRun || Event != currentEvent) {
    FillTH1(n_bosons, 1., "NZ");
    n_bosons = 1;
    currentRun = Run;
    currentEvent = Event;
  } else {
    ++n_bosons;
  }

  // Trigger names for investigations
  const ZeeDEnum::Trigger::Value triggerChainTag   = ZeeDEnum::Trigger::EF_e20_medium;		// Tag
  const ZeeDEnum::Trigger::Value triggerChainProbe = ZeeDEnum::Trigger::EF_2e12_medium;		// Probe

  // Get electrons from Z
  const ZeeDElectron* felec1 =(ZeeDElectron*) boson->GetFirstLep();
  const ZeeDElectron* felec2 =(ZeeDElectron*) boson->GetSecondLep();

  // get four-vectors of the Z-electrons
  TLorentzVector elec1 = felec1->GetFourVector();
  TLorentzVector elec2 = felec2->GetFourVector();
  FillTH1(elec1.Pt(), Weight, "Ptelec1");
  FillTH1(elec2.Pt(), Weight, "Ptelec2");

  // Get narrow mass window
  // FIXME: should be special-efficiency-investigation-mass-range passed through the python script
  Double_t MassZee = (elec1 + elec2).M();
  if (MassZee < 80. || MassZee > 100.) return;
  FillTH1(MassZee, Weight, "Mee");

  // ----------------------------------------- Fill the efficiency distributions -----------------------------------------------
  TLorentzVector elecProbe(0., 0., 0., 0.);
  Bool_t trigDecProbe = kFALSE;
  Bool_t trigDecTag = kFALSE;

  // Decide who is Tag/Probe
  if ((felec1->Trigger().Get().isMatchedToChain.find(triggerChainTag))->second) {  // if the first electron is Tag
    trigDecProbe = (felec2->Trigger().Get().isMatchedToChain.find(triggerChainProbe))->second;
    elecProbe = elec2;
    trigDecTag = kTRUE;
  } else if ((felec2->Trigger().Get().isMatchedToChain.find(triggerChainTag))->second) { // else if the second electron is Tag
    trigDecProbe = felec1->Trigger().Get().isMatchedToChain.find(triggerChainProbe)->second;
    elecProbe = elec1;
    trigDecTag = kTRUE;
  }


  // Fill the efficiency histogramms
  if (elecProbe.E() > 0.) {

    FillEffHist(elecProbe.Rapidity(),	trigDecTag, trigDecProbe, Weight, "2e12", "Eta");
    FillEffHist(elecProbe.Pt(),	trigDecTag, trigDecProbe, Weight, "2e12", "Pt");
    FillEffHist(elecProbe.E(),	trigDecTag, trigDecProbe, Weight, "2e12", "E");
    FillEffHist(elecProbe.Phi(),	trigDecTag, trigDecProbe, Weight, "2e12", "Phi");

    FillEffHist(elecProbe.Rapidity(), elecProbe.Pt(), trigDecTag, trigDecProbe, Weight, "2e12", "Eta", "Pt");
    FillEffHist(elecProbe.Rapidity(), elecProbe.Phi(),trigDecTag, trigDecProbe, Weight, "2e12", "Eta", "Phi");
  }

}

