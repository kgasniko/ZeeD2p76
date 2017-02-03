#ifndef ZEEDANALYSISCUTHISTMANAGER_CUTFLOW
#define ZEEDANALYSISCUTHISTMANAGER_CUTFLOW

#include "ZeeDCuts/ZeeDCutHistManager.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "TString.h"

class ZeeDAnalysisCutHistManagerCutFlow : public ZeeDCutHistManager {
public:
    ZeeDAnalysisCutHistManagerCutFlow(TString name = "CutHistManagerCutFlow")
        : ZeeDCutHistManager(name),
	  fSvcHelper("ZeeDAnalysisCutHistManagerCutFlow"),
          fAnaOptions(fSvcHelper.GetAnaSvc())
    {}

    void BookCutHistManager();

private:

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;      //!
};

#endif
