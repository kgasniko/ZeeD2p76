#ifndef __ZEEDANALYSISCUTHISTMANAGER_WMU
#define __ZEEDANALYSISCUTHISTMANAGER_WMU

////////////////////////////////////////////////////////
// Name    : ZeeDAnalysisCutHistManagerWmu.h
///////////////////////////////////////////////////////

#include "ZeeDCuts/ZeeDCutHistManager.h"

#include "TString.h"

class ZeeDAnalysisCutHistManagerWmu : public ZeeDCutHistManager {
public:
    explicit ZeeDAnalysisCutHistManagerWmu(TString name = "Wmu_selection");
    ~ZeeDAnalysisCutHistManagerWmu();
    void BookCutHistManager();
};

#endif
