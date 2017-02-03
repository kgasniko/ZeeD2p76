#ifndef ZEEDFINDERW
#define ZEEDFINDERW

////////////////////////////////////////////////////////
// Name    : ZeeDFinderW.h
////////////////////////////////////////////////////////

#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDAnalysisCuts/ZeeDFinder.h"

/** W selector for central electrons

    Created based on ZeeDFinderZCC by SG on 15 Feb 2011.

*/
class ZeeDFinderW: public ZeeDFinder {

public:
    ZeeDFinderW(TString Name = "W Boson Finder");
    void BookCuts();  // Dummy book cuts

    virtual TObjArray SelectBosons(ZeeDEvent* event) {

        const TObjArray bosonArray = ZeeDFinderW::SelectAllGoodBosons(event, event->GetWBosons());

        /*TObjArray bosonArray;
        bosonArray.Add(const_cast<ZeeDBoson*>(bestWBoson));*/

        return bosonArray;
    }
    
    virtual TObjArray SelectAllGoodBosons (ZeeDEvent*, const TObjArray*);

};

#endif
