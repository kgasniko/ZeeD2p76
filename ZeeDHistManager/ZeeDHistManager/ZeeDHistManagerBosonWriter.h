#ifndef __ZEEDHISTMANAGERBosonWriter_H
#define __ZEEDHISTMANAGERBosonWriter_H

#include "ZeeDHist/ZeeDHistManager.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "TString.h"

// Root includes
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TObject.h>
#include "TString.h"
#include <TTree.h>
#include <TFile.h>

class ZeeDHistManagerBosonWriter : public ZeeDHistManager {

public:

    explicit ZeeDHistManagerBosonWriter(TString name);

    // Book all histograms:
    void BookHistos();

    // Fill histos
    using ZeeDHistManager::Fill;
    void Fill();

    void Finalize();

private:

    TTree* fTreeOut;
    TLorentzVector fBoson4Vec;

    Int_t fRunNumber;
    Int_t fOrigRunNumber;
    Int_t fLB;
    Int_t fEventNumber;
    Float_t fEventWeight;

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;

};

#endif //ZeeDHistManagerBosonWriter

