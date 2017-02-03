#ifndef ZEEDANALYSISCUTSELECTORZLOWMASS
#define ZEEDANALYSISCUTSELECTORZLOWMASS

////////////////////////////////////////////////////////
// Name    : ZeeDAnalysisCutSelectorZLowMass.h
////////////////////////////////////////////////////////

#include "ZeeDCuts/ZeeDCutSelector.h"
#include "TString.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisZBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"

/** Combination of cuts used in Z analysis
    @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
    @date 2008/08/30
*/
class ZeeDAnalysisCutSelectorZLowMass : public ZeeDCutSelector {
public:
    explicit ZeeDAnalysisCutSelectorZLowMass(TString Name = "Z central central analysis cut selector");
    void BookCuts();
};

#endif
