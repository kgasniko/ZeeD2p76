#ifndef ZEEDFINDERZCC
#define ZEEDFINDERZCC

////////////////////////////////////////////////////////
// Name    : ZeeDFinderZCC.h
////////////////////////////////////////////////////////

#include "ZeeDAnalysisCuts/ZeeDFinder.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDTools/ZeeDBags.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisZBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"

/** Z selector for central electrons

    Created based on ZeeDZCutSelector by SG on 11 Feb 2011.

*/
class ZeeDFinderZCC: public ZeeDFinder {

public:
    ZeeDFinderZCC(TString Name);

    virtual void BookCuts();

    virtual TObjArray SelectBosons(ZeeDEvent* event) {

        TObjArray bestZBoson = ZeeDFinder::SelectAllGoodBosons(event, event->GetZBosons());

        return bestZBoson;

    }
    virtual TObjArray SelectAllGoodBosons (ZeeDEvent*, const TObjArray*);

};

#endif
