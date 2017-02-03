#ifndef ZEEDANALYSISCUTSELECTORZCCCutFlow
#define ZEEDANALYSISCUTSELECTORZCCCutFlow

////////////////////////////////////////////////////////
// Name    : ZeeDAnalysisCutSelectorZCCCutFlow.h
////////////////////////////////////////////////////////

#include "ZeeDCuts/ZeeDCutSelector.h"
#include "TString.h"

class ZeeDAnalysisCutSelectorZCCCutFlow : public ZeeDCutSelector {
public:
    ZeeDAnalysisCutSelectorZCCCutFlow(TString Name = "ZCCCutFlow") : ZeeDCutSelector(Name) {}
    void BookCuts();
};

#endif
