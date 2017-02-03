#ifndef __ZEEDANALYSISCUTHISTMANAGER_ZLOWMASS
#define __ZEEDANALYSISCUTHISTMANAGER_ZLOWMASS

////////////////////////////////////////////////////////
// Name    : ZeeDAnalysisCutHistManagerZLowMass.h
///////////////////////////////////////////////////////

#include "ZeeDCuts/ZeeDCutHistManager.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "TString.h"

/** Z CF boson cut-histmanager
  @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte, Sebastian Schmitt
  @date   Wed Jul 21 16:36:24 CEST 2010
*/
class ZeeDAnalysisCutHistManagerZLowMass : public ZeeDCutHistManager {
public:
    explicit ZeeDAnalysisCutHistManagerZLowMass(TString name = "ZLowMass_selection");

    void BookCutHistManager();
private:

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;      //!
};

#endif
