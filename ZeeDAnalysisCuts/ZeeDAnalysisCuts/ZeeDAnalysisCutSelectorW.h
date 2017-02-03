#ifndef __ZEEDANALYSISCUTSELECTOR_W
#define __ZEEDANALYSISCUTSELECTOR_W

////////////////////////////////////////////////////////
// Name    : ZeeDAnalysisCutSelectorW.h
////////////////////////////////////////////////////////

#include "ZeeDCuts/ZeeDCutSelector.h"
#include "TString.h"

/** Combination of cuts used in W analysis
  @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte, Sebastian Schmitt
  @date   Wed Jul 21 18:20:17 CEST 2010
*/
class ZeeDAnalysisCutSelectorW : public ZeeDCutSelector {
public:
    explicit ZeeDAnalysisCutSelectorW(TString name = "Analysis Cuts");
    ~ZeeDAnalysisCutSelectorW();

private:
    void BookCuts();
};

#endif
