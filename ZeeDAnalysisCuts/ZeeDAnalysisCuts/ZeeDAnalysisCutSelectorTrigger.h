#ifndef ZEEDANALYSISCUTSELECTORTRIGGER
#define ZEEDANALYSISCUTSELECTORTRIGGER

////////////////////////////////////////////////////////
// Name    : ZeeDAnalysisCutSelectorTrigger.h
////////////////////////////////////////////////////////

#include "ZeeDCuts/ZeeDCutSelector.h"
#include "TString.h"

/** Combination of cuts used in Z analysis tuned for trigger investigations
    @author SSh 
    @date 2011/10/30
*/
class ZeeDAnalysisCutSelectorTrigger : public ZeeDCutSelector {
public:
    ZeeDAnalysisCutSelectorTrigger(TString Name = "Z central central analysis cut trigger selector") : ZeeDCutSelector(Name) {}
    void BookCuts();
};

#endif
