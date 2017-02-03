#ifndef __ZEEDHISTMANAGERYieldAlt_H
#define __ZEEDHISTMANAGERYieldAlt_H

#include "ZeeDHist/ZeeDHistManager.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "TString.h"

#include <map>

class ZeeDHistManagerYieldAlt : public ZeeDHistManager {

public:

    explicit ZeeDHistManagerYieldAlt(TString name);

    // Book all histograms:
    void BookHistos();

    // Fill histos
    using ZeeDHistManager::Fill;
    void Fill();

private:

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;

    typedef std::map<UInt_t,Double_t> runLumiMap_t;
    typedef runLumiMap_t::const_iterator runLumiMap_const_it;
    runLumiMap_t fRunLumiMap;

};

#endif //ZeeDHistManagerYieldAlt

