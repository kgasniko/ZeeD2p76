#ifndef ZEEDFINDERZLOWMASS
#define ZEEDFINDERZLOWMASS

////////////////////////////////////////////////////////
// Name    : ZeeDFinderZLowMass.h
////////////////////////////////////////////////////////

#include "ZeeDAnalysisCuts/ZeeDFinder.h"
#include "ZeeDEvent/ZeeDEvent.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisZBosonCuts.h"

/** Z selector for central electrons

    Created based on ZeeDZCutSelector by SG on 11 Feb 2011.

*/
class ZeeDFinderZLowMass: public ZeeDFinder {

public:
    ZeeDFinderZLowMass(TString Name = "Central Z Low Mass Finder");
    virtual void BookCuts();

    virtual TObjArray SelectBosons(ZeeDEvent* event) {

        const ZeeDBoson* bestZBoson = ZeeDFinder::SelectBestBoson(event, event->GetZBosons());

        TObjArray bosonArray;
        bosonArray.Add(const_cast<ZeeDBoson*>(bestZBoson));

        return bosonArray;
    }

};

#endif
