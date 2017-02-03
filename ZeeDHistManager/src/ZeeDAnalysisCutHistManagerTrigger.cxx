#include "ZeeDHistManager/ZeeDAnalysisCutHistManagerTrigger.h"

// Root includes
#include <TString.h>

// ZeeD analysis includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

// Cut selector:
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorTrigger.h"

// Weights
#include "ZeeDAnalysisCuts/ZeeDCalcWeightCutDepZ.h"

// Hist managers:
#include "ZeeDHistManager/ZeeDHistManagerTrigger.h"
#include "ZeeDHistManager/ZeeDHistManagerYield.h"

void ZeeDAnalysisCutHistManagerTrigger::BookCutHistManager()
{

  // Get analysis options
  //ServiceHandle<IZeeDAnalysisSvc>* ZeeDAnaOptions = fSvcHelper.GetAnaSvc();

  // Start with "Cut flow" emulation
  std::vector<std::string> Stages;
/*
  Stages.push_back("ExistZ");
  Stages.push_back(Stages.back() + "+ZMassZ");
  Stages.push_back(Stages.back() + "+EtaMaxBothElecZ");
  Stages.push_back(Stages.back() + "+EtaCrackBothElecZ");
  Stages.push_back(Stages.back() + "+AuthorBothElecZ");

  Stages.push_back(Stages.back() + "+TrackBothElecZ");
  Stages.push_back(Stages.back() + "+NTracksAtPrimVtx");
  //Stages.push_back(Stages.back() + "+IsEMMediumBothElecZ");
  Stages.push_back(Stages.back() + "+IsEMTightBothElecZ");
  Stages.push_back(Stages.back() + "+ChargeBothElecOppositeZ");
  Stages.push_back(Stages.back() + "+OQMaps");
  // FIXME: use the Stages for control purposes
*/
  // Default cut
  const std::string DefaultCuts = Stages.back();

  // Create/book cut selector:
  ZeeDAnalysisCutSelectorTrigger* AnalysisCuts = new ZeeDAnalysisCutSelectorTrigger("Z Analysis Trigger Selection");
  AnalysisCuts->BookCuts();
  this->SetCutSelector(AnalysisCuts);

  // Set default mask
  this->SetDefaultMask(DefaultCuts);

  // Plots after all cuts
  this->AddMaskLoose("DEFAULT", new ZeeDHistManagerTrigger(this->getName() + "/" + "Trigger"));
  this->AddMaskLoose("DEFAULT", new ZeeDHistManagerYield(this->getName() + "/" + "Yield"));

  // Background plots
  this->AddMask("DEFAULT-ChargeBothElecOppositeZ+ChargeBothElecSameZ", new ZeeDHistManagerTrigger(this->getName() + "/" + "TriggerBkg"));

  // Book control plots for cut selector, book all histograms
  ZeeDCutHistManager::BookCutHistManager();

}
