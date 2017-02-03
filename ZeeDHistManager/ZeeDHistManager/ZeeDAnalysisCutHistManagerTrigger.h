#ifndef ZEEDANALYSISCUTHISTMANAGERTRIGGER
#define ZEEDANALYSISCUTHISTMANAGERTRIGGER

////////////////////////////////////////////////////////
// Name    : ZeeDAnalysisCutHistManagerTrigger.h
///////////////////////////////////////////////////////

#include "ZeeDCuts/ZeeDCutHistManager.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "TString.h"

/** Cut/Histogram manager for Trigger investigations.
    @author SSh 
    @date 2011/10/30
*/
class ZeeDAnalysisCutHistManagerTrigger : public ZeeDCutHistManager {
public:
    ZeeDAnalysisCutHistManagerTrigger(TString name = "trigger_selection")
        : ZeeDCutHistManager(name),
	  fSvcHelper("ZeeDAnalysisCutHistManagerTrigger"),
          fAnaOptions(fSvcHelper.GetAnaSvc())
    {}

    void BookCutHistManager();

private:

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;      //!
};

#endif
