#ifndef __ZEEDHISTMANAGERWEIGHTS_H
#define __ZEEDHISTMANAGERWEIGHTS_H

#include "ZeeDHist/ZeeDHistManager.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "TString.h"

/** HistManager filling rec and gen weights histograms

    can be used at a "no cuts" stage in ZeeDAnalysis/ZeeDMeasurement to
    get sum of weights for e.g. MC luminosity

   @author Sebastian Schmitt
   @date September 2011
 */
class ZeeDHistManagerWeights : public ZeeDHistManager {

public:

    explicit ZeeDHistManagerWeights(TString name);

    // Book all histograms:
    void BookHistos();

    // Fill histos
    using ZeeDHistManager::Fill;
    void Fill();

private:

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;


};

#endif //ZeeDHistManagerWeights

