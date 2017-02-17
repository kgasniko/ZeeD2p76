#ifndef ZEEDFINDERZmumu
#define ZEEDFINDERZmumu

////////////////////////////////////////////////////////
// Name    : ZeeDFinderZmumu.h
////////////////////////////////////////////////////////

#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDAnalysisCuts/ZeeDFinder.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDTools/ZeeDBags.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisZBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"


/** Zmumu selector for central electrons

    Created based on ZeeDFinderZCC by SG on 15 Feb 2011.

*/
class ZeeDFinderZmumu : public ZeeDFinder {

public:
    ZeeDFinderZmumu(TString Name = "Zmumu Boson Finder");
    void BookCuts();  // Dummy book cuts

    virtual TObjArray SelectBosons(ZeeDEvent* event) {

        const TObjArray bosonArray = ZeeDFinderZmumu::SelectAllGoodBosons(event, event->GetZmumuBosons());

        /*TObjArray bosonArray;
        bosonArray.Add(const_cast<ZeeDBoson*>(bestZmumuBoson));*/

        return bosonArray;
    }
    
    virtual TObjArray SelectAllGoodBosons (ZeeDEvent*, const TObjArray*);

};

#endif
