#ifndef __ZEEDANALYSISCUTHISTMANAGER_ZMU
#define __ZEEDANALYSISCUTHISTMANAGER_ZMU

////////////////////////////////////////////////////////
// Name    : ZeeDAnalysisCutHistManagerZmumu.h
///////////////////////////////////////////////////////

#include "ZeeDCuts/ZeeDCutHistManager.h"

#include "TString.h"

class ZeeDAnalysisCutHistManagerZmumu : public ZeeDCutHistManager {
public:
    explicit ZeeDAnalysisCutHistManagerZmumu(TString name = "Zmumu_selection");
    ~ZeeDAnalysisCutHistManagerZmumu();
    void BookCutHistManager();
};

#endif
