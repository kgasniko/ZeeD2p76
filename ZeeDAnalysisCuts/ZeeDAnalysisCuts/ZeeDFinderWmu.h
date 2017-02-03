#ifndef ZEEDFINDERWMU
#define ZEEDFINDERWMU

////////////////////////////////////////////////////////
// Name    : ZeeDFinderWmu.h
////////////////////////////////////////////////////////

#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDAnalysisCuts/ZeeDFinder.h"

/** W->munu selector for central electrons

    Created based on ZeeDFinderZCC by SG on 15 Feb 2011.

*/
class ZeeDFinderWmu: public ZeeDFinder {

public:
    ZeeDFinderWmu(TString Name = "Wmu Boson Finder");
    void BookCuts();  // Dummy book cuts

    virtual TObjArray SelectBosons(ZeeDEvent* event) {

        const TObjArray bosonArray = ZeeDFinderWmu::SelectAllGoodBosons(event, event->GetWmuBosons());

        return bosonArray;
    }
    
    virtual TObjArray SelectAllGoodBosons (ZeeDEvent*, const TObjArray*);

};

#endif
