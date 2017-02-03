#ifndef ZEEDFINDERZCF
#define ZEEDFINDERZCF

////////////////////////////////////////////////////////
// Name    : ZeeDFinderzcf.h
////////////////////////////////////////////////////////

#include "ZeeDAnalysisCuts/ZeeDFinder.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "TString.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisZBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"


/** Z finder for CF
    Re-structured by SG, 12 Feb 2011
*/
class ZeeDFinderZCF :  public ZeeDFinder {
public:
    ZeeDFinderZCF(TString Name = "Central-Forward Z Finder") : ZeeDFinder(Name) {}
    void BookCuts();

    virtual TObjArray SelectBosons(ZeeDEvent* event) {

        const ZeeDBoson* bestZBoson = ZeeDFinder::SelectBestBoson(event, event->GetZBosons());

        TObjArray bosonArray;
        bosonArray.Add(const_cast<ZeeDBoson*>(bestZBoson));

        return bosonArray;
    }

};

#endif
