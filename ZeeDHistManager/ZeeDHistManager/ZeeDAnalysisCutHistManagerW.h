#ifndef __ZEEDANALYSISCUTHISTMANAGER_W
#define __ZEEDANALYSISCUTHISTMANAGER_W

////////////////////////////////////////////////////////
// Name    : ZeeDAnalysisCutHistManagerW.h
///////////////////////////////////////////////////////

#include "ZeeDCuts/ZeeDCutHistManager.h"

#include "TString.h"

/** W boson cut-histmanager
  @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte, Sebastian Schmitt
  @date   Wed Jul 21 16:36:24 CEST 2010
*/
class ZeeDAnalysisCutHistManagerW : public ZeeDCutHistManager {
public:
    explicit ZeeDAnalysisCutHistManagerW(TString name = "W_selection");
    ~ZeeDAnalysisCutHistManagerW();
    void BookCutHistManager();
};

#endif
