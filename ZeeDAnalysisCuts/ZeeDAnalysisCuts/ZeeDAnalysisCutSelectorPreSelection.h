#ifndef ZEEDANALYSISCUTSELECTORPRESELECTION
#define ZEEDANALYSISCUTSELECTORPRESELECTION

#include "ZeeDCuts/ZeeDCutSelector.h"
#include "TString.h"


class ZeeDAnalysisCutSelectorPreSelection : public ZeeDCutSelector {
public:
    ZeeDAnalysisCutSelectorPreSelection(TString Name = "Preselection") : ZeeDCutSelector(Name) {}
    void BookCuts();
};

#endif
