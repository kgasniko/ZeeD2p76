#ifndef __ZEEDHISTMANAGERQCDBkgW_H
#define __ZEEDHISTMANAGERQCDBkgW_H

#include "ZeeDHist/ZeeDHistManager.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "TString.h"

class ZeeDHistManagerQCDBkgW : public ZeeDHistManager {

public:

    explicit ZeeDHistManagerQCDBkgW(TString name);

    // Book all histograms:
    void BookHistos();

    // Fill histos
    using ZeeDHistManager::Fill;
    void Fill();

private:

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;

};

#endif //ZeeDHistManagerQCDBkgW

