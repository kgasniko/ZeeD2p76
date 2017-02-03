//#define USE_DEBUG_MESSAGES 0
//#define DEBUG_ON 0
#include "ZeeDMisc/ZeeDMisc.h"

#include "ZeeDCalculator/ZeeDReadEventTTree.h"

#include "ZeeDEvent/ZeeDEvent.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include <TSystem.h>

#include "StoreGate/StoreGateSvc.h"

ZeeDReadEventTTree::ZeeDReadEventTTree() :
    fEventCounter(0),
    fSvcHelper("ZeeDReadEventTTree"),
    fAnaOptions(fSvcHelper.GetAnaSvc()),
    fStoreGate(fSvcHelper.GetStoreGate()),
    fROOTEvent(0)
{}

void ZeeDReadEventTTree::ReadInfo(RawEvent* event)
{

    if ((*fStoreGate)->retrieve(fROOTEvent, "events").isSuccess()) {


    } else {

        Error("ZeeDReadEventTTree::ReadInfo",
              "could not read events from StoreGate");
        gSystem->Exit(EXIT_FAILURE);

    }

    DEBUG_VAR(fROOTEvent);

    fROOTEvent->fillOut(event);

    DEBUG_VAR(event->RunNumber().IsSet());

    if(event->RunNumber().IsSet()) {
        DEBUG_VAR(event->RunNumber().Get());
    }

    DEBUG_VAR(event->EventNumber().IsSet());

    if(event->EventNumber().IsSet()) {
        DEBUG_VAR(event->EventNumber().Get());
    }

    DEBUG_VAR(event->GetWeight());

    DEBUG_VAR(event->GetEtDensity_3EM());
    DEBUG_VAR(event->GetEtDensity_4EM());
    DEBUG_VAR(event->GetEtDensity_3LC());
    DEBUG_VAR(event->GetEtDensity_4LC());

    DEBUG_VAR((*fAnaOptions)->IsData());

    DEBUG_VAR(event->IsData());

    const Bool_t anaOptions_IsData = (*fAnaOptions)->IsData();
    const Bool_t event_IsData = event->IsData();

    if (((*fAnaOptions)->MCDataAuto()) && (event_IsData != anaOptions_IsData)) {
        Error("ZeeDReadEventTTree::ReadInfo", "IsData stored in event and set in file's metadata don't match! event: %i metadata: %i", event_IsData, anaOptions_IsData);
        gSystem->Exit(EXIT_FAILURE);
    }

    DEBUG_VAR((*fAnaOptions)->IsData());

}
