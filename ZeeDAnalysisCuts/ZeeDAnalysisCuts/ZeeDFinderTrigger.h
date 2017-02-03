#ifndef ZEEDFINDERTRIGGER
#define ZEEDFINDERTRIGGER

////////////////////////////////////////////////////////
// Name    : ZeeDFinderTrigger.h
////////////////////////////////////////////////////////

#include "ZeeDAnalysisCuts/ZeeDFinder.h"
#include "ZeeDEvent/ZeeDEvent.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisZBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"

/** Z selector for central electrons

  Created based on ZeeDZCutSelector by SSh on 30 Oct 2011.

 */
class ZeeDFinderTrigger: public ZeeDFinder {

  public:
    ZeeDFinderTrigger(TString Name = "Z Finder for the Trigger");
    virtual void BookCuts();

    virtual TObjArray SelectBosons(ZeeDEvent* event) {

      // Take all the preselected bosons
      TObjArray bosonArray = ZeeDFinder::SelectAllGoodBosons(event, event->GetZBosons());

      return bosonArray;
    }

};

#endif
