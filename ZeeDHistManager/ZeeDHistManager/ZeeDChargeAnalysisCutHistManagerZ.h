#ifndef ZEED_CHARGE_ANALYSIS_CUTHISTMANAGER_Z
#define ZEED_CHARGE_ANALYSIS_CUTHISTMANAGER_Z

#include "ZeeDCuts/ZeeDCutHistManager.h"

#include "TString.h"

/** Cut hist manager for charge studies
    @author Sebastian Schmitt
    @date January 2011
*/
class ZeeDChargeAnalysisCutHistManagerZ : public ZeeDCutHistManager {
public:
    ZeeDChargeAnalysisCutHistManagerZ(TString name = "ChargeAnalysisCutHistManager") : ZeeDCutHistManager(name) {}
    void BookCutHistManager() ;
};

#endif // ZEED_CHARGE_ANALYSIS_CUTHISTMANAGER_Z
