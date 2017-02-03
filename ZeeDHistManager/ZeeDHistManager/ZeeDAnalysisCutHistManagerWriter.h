#ifndef ZEEDANALYSISCUTHISTMANAGER_WRITER
#define ZEEDANALYSISCUTHISTMANAGER_WRITER

#include "ZeeDCuts/ZeeDCutHistManager.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "TString.h"

class ZeeDAnalysisCutHistManagerWriter : public ZeeDCutHistManager {
public:
    ZeeDAnalysisCutHistManagerWriter(TString name = "CutHistManagerWriter")
        : ZeeDCutHistManager(name),
	  fSvcHelper("ZeeDAnalysisCutHistManagerWriter"),
          fAnaOptions(fSvcHelper.GetAnaSvc())
    {}

    void BookCutHistManager();

private:

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;      //!
};

#endif
