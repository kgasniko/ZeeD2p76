#ifndef __ZEEDHISTMANAGERQCDBkg_H
#define __ZEEDHISTMANAGERQCDBkg_H

#include "ZeeDHist/ZeeDHistManager.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "TString.h"

class ZeeDHistManagerQCDBkg : public ZeeDHistManager {

public:

    explicit ZeeDHistManagerQCDBkg(TString name);

    // Book all histograms:
    void BookHistos();

    // Fill histos
    using ZeeDHistManager::Fill;
    void Fill();

private:

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;

};

#endif //ZeeDHistManagerQCDBkg

