#ifndef __ZEEDANALYSISCUTSELECTOR_WMU
#define __ZEEDANALYSISCUTSELECTOR_WMU

////////////////////////////////////////////////////////
// Name    : ZeeDAnalysisCutSelectorWmu.h
////////////////////////////////////////////////////////

#include "ZeeDCuts/ZeeDCutSelector.h"
#include "TString.h"

/** Combination of cuts used in W->munu analysis
*/
class ZeeDAnalysisCutSelectorWmu : public ZeeDCutSelector {
public:
    explicit ZeeDAnalysisCutSelectorWmu(TString name = "Analysis Cuts");
    ~ZeeDAnalysisCutSelectorWmu();

private:
    void BookCuts();
};

#endif
