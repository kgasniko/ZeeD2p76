#include "ZeeDHistManager/ZeeDAnalysisCutHistManagerJet.h"

#include <iostream>

// ZeeD analysis includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorJet.h"

// Histogram managers
#include "ZeeDHistManager/ZeeDHistManagerJet.h"
#include "ZeeDHistManager/ZeeDHistManagerEvent.h"

#include "ZeeDMisc/ZeeDMisc.h"

ZeeDAnalysisCutHistManagerJet::ZeeDAnalysisCutHistManagerJet(TString Name) : ZeeDCutHistManager(Name)
{}

ZeeDAnalysisCutHistManagerJet::~ZeeDAnalysisCutHistManagerJet()
{}

void ZeeDAnalysisCutHistManagerJet::BookCutHistManager()
{
    //Define Cut masks
    TString selection="JetPtMin+JetEtaMax";
 
    // Trigger cut
    ServiceHandle<IZeeDAnalysisSvc>* ZeeDAnaOptions = fSvcHelper.GetAnaSvc();

    if ( (*ZeeDAnaOptions)->UseTrigger() ) {
        //Do something
    }

    ZeeDAnalysisCutSelectorJet * analysisCuts   = new ZeeDAnalysisCutSelectorJet(this->getName() + "/" + "Z+Jet Analysis Selection");
    this->SetCutSelector(analysisCuts);

    ZeeDHistManagerJet* jetPlots                = new ZeeDHistManagerJet(this->getName() + "/" + "Jet");
    this->AddMaskLoose( selection, jetPlots);
}

