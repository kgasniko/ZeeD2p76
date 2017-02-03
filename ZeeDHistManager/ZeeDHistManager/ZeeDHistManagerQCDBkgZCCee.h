#pragma once

#include "ZeeDHist/ZeeDHistManager.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "TString.h"

class ZeeDHistManagerQCDBkgZCCee : public ZeeDHistManager {

public:

    explicit ZeeDHistManagerQCDBkgZCCee(TString name);

    // Book all histograms:
    void BookHistos();

    // Fill histos
    using ZeeDHistManager::Fill;
    void Fill();

private:

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;

};

