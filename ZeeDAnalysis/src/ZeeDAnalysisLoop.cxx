#include "ZeeDAnalysis/ZeeDAnalysisLoop.h"

// Std includes
//#include <iostream>
#include <string>

// Root includes
#include <TString.h>
#include <TObjArray.h>

#include <TSystem.h>

// ZeeD analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDCalculator/ZeeDEventFactory.h"

#include "ZeeDHist/ZeeDHistManager.h"
#include "ZeeDCuts/ZeeDCutHistManager.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDArrays/ZeeDHistTools.h"

#include "ZeeDAnalysisCuts/ZeeDIdentifierZCC.h"
#include "ZeeDAnalysisCuts/ZeeDFinderZCC.h"

#include "ZeeDAnalysisCuts/ZeeDIdentifierW.h"
#include "ZeeDAnalysisCuts/ZeeDFinderW.h"
#include "ZeeDAnalysisCuts/ZeeDIdentifierWmu.h"
#include "ZeeDAnalysisCuts/ZeeDFinderWmu.h"

//------------------------------------------------------
ZeeDAnalysisLoop::ZeeDAnalysisLoop(const std::string& name,
                                   ISvcLocator* pSvcLocator) :
    AthAlgorithm(name, pSvcLocator),
    fEventCounter(0),
    fEventInGRLCounter(0),
    fSvcHelper("ZeeDAnalysis"),
    fAnaOptions(fSvcHelper.GetAnaSvc())
{
    // Constructor

}

//------------------------------------------------------
ZeeDAnalysisLoop::~ZeeDAnalysisLoop()
{
  // Destructor
  for (std::vector<ZeeDHistManager*>::iterator it = fHistManagers.begin();
      it != fHistManagers.end(); ++it) delete (*it);

}

//-----------------------------------------------------


//------------------------------------------------------
StatusCode ZeeDAnalysisLoop::execute()
{

    if( (*fAnaOptions)->ExecuteAnalysisLoop() == kFALSE ) {

        return StatusCode::SUCCESS;

    }

    // See if we need to generate bootstrap numbers:
    if ( (*fAnaOptions)->genBootstrapRnd() ) {
        (*fAnaOptions)->GetPoissonRandomVault()->UpdateNumbers();
    }

    // Read event from input file
    const RawEvent* rawEvent = ZeeDEventFactory::getInstance()->ReadEvent();

    if(rawEvent == NULL) {
        Error("ZeeDAnalysis::execute", "Can't read an event. Input source: %s", ((*fAnaOptions)->Source()).c_str());
        return StatusCode::SUCCESS;
    }

    // Do analysis
    if ( (*fAnaOptions)->AnalyseEvent() && rawEvent->RunNumber().Get() != 0 )   {

        this->analyseEvent(rawEvent);

    }

    // log message
    if (++fEventCounter % 1000 == 0) {
        Info("ZeeDAnalysisLoop::execute", "passing %i", fEventCounter);
    }

    delete rawEvent;

    return StatusCode::SUCCESS;
}

//------------------------------------------------------
void ZeeDAnalysisLoop::finalizeReport()
{

    Info("ZeeDAnalysisLoop::finalizeReport", "total number of events = %i", fEventCounter);

    if( (*fAnaOptions)->DoManualGRL() ) {
        Info("ZeeDAnalysisLoop::finalizeReport", "events in GRL          = %i", fEventInGRLCounter);
    }

}

//------------------------------------------------------
void ZeeDAnalysisLoop::AddHistManager(ZeeDHistManager* histManager)
{

  // Adds histmanager to the analysis
  if (histManager != NULL) {
    histManager->SetLevel(ZeeDLevel::Analysis);
    fHistManagers.push_back(histManager);
  }

}

void ZeeDAnalysisLoop::AddHistManager(ZeeDHistManager* histManager, ZeeDNoCutsBundle* bundle)
{

    histManager->SetLevel(ZeeDLevel::Analysis);
    bundle->push_back(histManager);
    
}


// For all of new bosons you need to add AddIdentifier function. In other case it would use default one for Z-boson
//------------------------------------------------------
// Setting of identifier for W -> enu boson
ZeeDIdentifier* ZeeDAnalysisLoop::AddIdentifier(ZeeDFinderW* finder, TString name)
{

    ZeeDIdentifierW* identifier = new ZeeDIdentifierW(name);

    if( finder != NULL ) {
        finder->BookCuts();
        identifier->SetFinder(finder);
    }

    fIdentifiers[name] = (ZeeDIdentifier*)identifier;

    return (ZeeDIdentifier*)identifier;
}

//Setting of identifier for W -> mu nu  boson
ZeeDIdentifier* ZeeDAnalysisLoop::AddIdentifier(ZeeDFinderWmu* finder, TString name)
{

    ZeeDIdentifierWmu* identifier = new ZeeDIdentifierWmu(name);

    if( finder != NULL ) {
        finder->BookCuts();
        identifier->SetFinder(finder);
    }

    fIdentifiers[name] = (ZeeDIdentifier*)identifier;

    return (ZeeDIdentifier*)identifier;
}

// Setting of identifier for Z-> ee boson
//------------------------------------------------------
ZeeDIdentifier* ZeeDAnalysisLoop::AddIdentifier(ZeeDFinderZCC* finder, TString name)
{

    ZeeDIdentifierZCC* identifier = new ZeeDIdentifierZCC(name);

    if( finder != NULL ) {
        finder->BookCuts();
        identifier->SetFinder(finder);
    }

    fIdentifiers[name] = identifier;

    return (ZeeDIdentifier*)identifier;
}

ZeeDIdentifier* ZeeDAnalysisLoop::AddIdentifier(ZeeDFinderZmumu* finder, TString name)
{

    ZeeDIdentifierZmumu* identifier = new ZeeDIdentifierZmumu(name);

    if( finder != NULL ) {
        finder->BookCuts();
        identifier->SetFinder(finder);
    }

    fIdentifiers[name] = identifier;

    return (ZeeDIdentifier*)identifier;
}

//------------------------------------------------------
void ZeeDAnalysisLoop::AddCutHistManager( ZeeDCutHistManager* cutHM,  ZeeDCutCache* cutCache,  ZeeDAnalysisChainBundle* list)
{

    cutHM->SetCutSelector(cutCache->fCutSelector);

    ZeeDAnalysisChain* chain = new ZeeDAnalysisChain(cutCache, cutHM);

    list->push_back(chain);

}

//------------------------------------------------------
bool ZeeDAnalysisLoop::CheckManualGRL(const ZeeDEvent* event)
{

    if( (*fAnaOptions)->DoManualGRL() ) {

        if(event->IsInGRL().IsSet() == kFALSE) {

            Error("ZeeDAnalysisLoop::execute",
                  "Event has no info about the GoodRunList. Provide the info or switch the check off.");

            gSystem->Exit(EXIT_FAILURE);

        } else { // IsInGRL is set

            const Bool_t isInGRL = event->IsInGRL().Get();

            if( isInGRL == kTRUE ) {

                ++fEventInGRLCounter;

                return kTRUE;

            } else {

                return kFALSE;

            }

        }

    }

    return kTRUE;

}

