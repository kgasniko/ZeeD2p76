#include "ZeeDCalculator/ZeeDEventFactory.h"

// Athena includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

// ZeeD includex
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDCalculator/ZeeDCalcEvent.h"
#include "ZeeDCalculator/ZeeDCalcEventWeight.h"
#include "ZeeDCalculator/ZeeDCalcRecWeight.h"
#include "ZeeDCalculator/ZeeDCalcRecWeightMuon.h"

#include "ZeeDCalculator/ZeeDReadEventAOD.h"
#include "ZeeDCalculator/ZeeDReadEventInlineZ.h"
#include "ZeeDCalculator/ZeeDReadEventTTree.h"
#include "ZeeDCalculator/ZeeDReadEventD3PD.h"
#include "ZeeDCalculator/IZeeDReadEvent.h"

#define DEBUG_ON 0
#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDEventFactory* ZeeDEventFactory::fInstance = NULL;

//------------------------------------------------------
ZeeDEventFactory::ZeeDEventFactory()
    : fSvcHelper("ZeeDReadElecAOD"),
      fAnaOptions(fSvcHelper.GetAnaSvc())
{
    // Constructor
    DEBUG_MES_START;

    Info("ZeeDEventFactory::ZeeDEventFactory", " Initialisation ");

    fCalcEvent = new ZeeDCalcEvent;

    if ((*fAnaOptions)->Source() == "AOD") {
        fReadEvent = new ZeeDReadEventAOD;
    } else if ( (*fAnaOptions)->Source() == "InlineZ") {
        fReadEvent = new ZeeDReadEventInlineZ;
    } else if ( (*fAnaOptions)->Source() == "TTree") {
        fReadEvent = new ZeeDReadEventTTree;
    } else if ( (*fAnaOptions)->Source() == "D3PD") {
        fReadEvent = new ZeeDReadEventD3PD;
    }

    fCalcGenWeight = new ZeeDCalcEventWeight();
    fCalcRecWeight = new ZeeDCalcRecWeight();
    fCalcRecWeightMuon = new ZeeDCalcRecWeightMuon();

    DEBUG_MES_END;
}

//------------------------------------------------------
ZeeDEventFactory::~ZeeDEventFactory()
{
    // Destructor
    DEBUG_MES_START;

    delete fCalcEvent;
    
    if (fReadEvent != NULL) {
        delete fReadEvent;
    }

    if (fCalcGenWeight != NULL) {
        delete fCalcGenWeight;
    }

    /*if (fCalcRecWeight != NULL) {
        delete fCalcRecWeight;
    }
    if (fCalcRecWeightMuon != NULL) {
        delete fCalcRecWeightMuon;
    }*/

    DEBUG_MES_END;
}


//------------------------------------------------------
const RawEvent* ZeeDEventFactory::ReadEvent()
{
    DEBUG_MES_START;
    RawEvent* rawEvent = new RawEvent;

    // Read information to raw event
    fReadEvent->ReadInfo(rawEvent);

    return rawEvent;

    DEBUG_MES_END;
}

//------------------------------------------------------
ZeeDEvent* ZeeDEventFactory::CalcEvent(const RawEvent* rawEvent)
{
    // Copy Raw to Standard Event
    ZeeDEvent* calcEvent = new ZeeDEvent(*rawEvent);

    // Calculate event
    fCalcEvent->Calculate(calcEvent);

    return calcEvent;
}

//------------------------------------------------------
void ZeeDEventFactory::CalcGenWeight(ZeeDEvent* event){

    // Calculate event Weight
	fCalcGenWeight->Calculate(event);
}

//------------------------------------------------------
void ZeeDEventFactory::CalcRecWeight(ZeeDEvent* event){

    // Calculate event Weight
	fCalcRecWeight->Calculate(event);
    fCalcRecWeightMuon->Calculate(event);
}

//------------------------------------------------------
ZeeDEvent* ZeeDEventFactory::Get()
{
    const RawEvent* rawEvent = ReadEvent();
    return CalcEvent(rawEvent);

}
