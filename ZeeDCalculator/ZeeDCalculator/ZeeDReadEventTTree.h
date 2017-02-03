#ifndef ZEED_CALC_EVENT_TTREE_H
#define ZEED_CALC_EVENT_TTREE_H

#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDCalculator/IZeeDReadEvent.h"
#include "ZeeDROOTInterface/ZeeDROOTEvent.h"

class ZeeDEvent;
class StoreGateSvc;

/**
   @todo do we need calibrator and systematics for TTree?
 */
class ZeeDReadEventTTree : public TObject, public IZeeDReadEvent {

public:

    ZeeDReadEventTTree();

    virtual void ReadInfo(RawEvent* event);

private:

    int fEventCounter;

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;
    ServiceHandle<StoreGateSvc>* fStoreGate; //!

    const ZeeDROOTEvent* fROOTEvent;

};

#endif
