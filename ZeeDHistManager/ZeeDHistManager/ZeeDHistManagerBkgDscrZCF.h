
#ifndef __ZEEDHISTMANAGERBkgDscrZCF_H
#define __ZEEDHISTMANAGERBkgDscrZCF_H

#include "ZeeDHist/ZeeDHistManager.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "TString.h"

class ZeeDHistManagerBkgDscrZCF : public ZeeDHistManager {

public:

explicit ZeeDHistManagerBkgDscrZCF(TString name, bool doBS=false);

// Book all histograms:
void BookHistos();

// Fill histos
using ZeeDHistManager::Fill;
virtual void Fill();

private:

bool fdoBS;
ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;

};

#endif //ZeeDHistManagerBkgDscrZCF
