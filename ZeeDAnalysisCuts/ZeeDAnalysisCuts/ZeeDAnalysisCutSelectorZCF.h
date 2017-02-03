#ifndef __ZEEDANALYSISCUTSELECTOR_ZCF
#define __ZEEDANALYSISCUTSELECTOR_ZCF

////////////////////////////////////////////////////////
// Name    : ZeeDAnalysisCutSelectorZCF.h
////////////////////////////////////////////////////////

#include "ZeeDCuts/ZeeDCutSelector.h"
#include "TString.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisZBosonCuts.h"

/** Combination of cuts used in Z CF analysis
  @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte, Sebastian Schmitt
  @date   Wed Jul 21 18:20:17 CEST 2010
*/
class ZeeDAnalysisCutSelectorZCF : public ZeeDCutSelector {
public:
    explicit ZeeDAnalysisCutSelectorZCF(TString name = "Z central-forward analysis cuts");
    ~ZeeDAnalysisCutSelectorZCF();

private:
    void BookCuts();
};

#endif
