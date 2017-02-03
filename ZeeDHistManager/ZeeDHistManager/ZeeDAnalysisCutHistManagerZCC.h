#ifndef ZeeDAnalysisCutHistManager_ZCC
#define ZeeDAnalysisCutHistManager_ZCC

////////////////////////////////////////////////////////
// Name    : ZeeDAnalysisCutHistManagerZCC.h
///////////////////////////////////////////////////////

#include "ZeeDCuts/ZeeDCutHistManager.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "TString.h"

/** Cut/Histogram manager.
    @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
    @date 2008/08/30
*/
class ZeeDAnalysisCutHistManagerZCC : public ZeeDCutHistManager {
public:
    ZeeDAnalysisCutHistManagerZCC(TString name = "ZCC_selection")
        : ZeeDCutHistManager(name),
	  fSvcHelper("ZeeDAnalysisCutHistManagerZCC"),
          fAnaOptions(fSvcHelper.GetAnaSvc())
    {}

    void BookCutHistManager();

private:

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;      //!
};

#endif
