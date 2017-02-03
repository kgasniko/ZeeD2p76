#ifndef __ZEEDHISTMANAGEREWUnfoldWriter_H
#define __ZEEDHISTMANAGEREWUnfoldWriter_H

#include "ZeeDHist/ZeeDHistManager.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#define USE_DEBUG_MESSAGES
#define DEBUG_ON


#include "TString.h"

// Root includes
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TObject.h>
#include "TString.h"
#include <TTree.h>
#include <TFile.h>

#include "ZeeDArrays/ZeeDBinGrid.h"
#include "ZeeDArrays/ZeeDArrayD.h"
#include "ZeeDTools/ZeeDQuantity.h"

class ZeeDHistManagerEWUnfoldWriter : public ZeeDHistManager {

public:

    enum mode {histograms, trees};

    enum require {yes, no, dontcare};

    explicit ZeeDHistManagerEWUnfoldWriter(TString name, 
                                           TString binGridFile="",
                                           ZeeDQuantity<ZeeDArrayD> (*cVarREC)(const ZeeDEvent*) = NULL,
                                           ZeeDQuantity<ZeeDArrayD> (*cVarGEN)(const ZeeDEvent*) = NULL
                                           );

    // Book all histograms:
    void BookHistos();

    // Fill histos
    using ZeeDHistManager::Fill;
    void Fill();

    void Finalize();

    void SetChannel(Int_t channel) {
        fChannel = channel;
    }

    void SetRecoIsReconstructed(require RecoIsReconstructed) {
        fRecoIsReconstructed = RecoIsReconstructed;
    }

    void SetTruthIsFiducial(require TruthIsFiducial) {
        fTruthIsFiducial = TruthIsFiducial;
    }

    void SetForAcceptance(require ForAcceptance) {
        fForAcceptance = ForAcceptance;
    }

    void SetMode(mode m) {
        fMode = m;
    }

private:

    TString fName;

    TTree* fTreeOut;

    Double_t fRecoVar1;
    Double_t fTruthVar1;

    Double_t fRecoVar2;
    Double_t fTruthVar2;

    Double_t fRecoWeight;

    require fRecoIsReconstructed;
    require fTruthIsFiducial;
    require fForAcceptance;

    Double_t fMCTruthWeight;
    Double_t fPileUpWeight;

    Int_t fChannel;

    Int_t fRunNumber;
    Int_t fOrigRunNumber;
    Int_t fLB;
    Int_t fEventNumber;
    Double_t fEventWeight;

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;

    mode fMode;

    TString fBinGridFile;

    ZeeDBinGrid* fBinGrid;

    ZeeDQuantity<ZeeDArrayD> (*fVarREC)(const ZeeDEvent*);
    ZeeDQuantity<ZeeDArrayD> (*fVarGEN)(const ZeeDEvent*);

    UInt_t fDim;

    /** current (set at each event) array of reconstructed quantities */
    ZeeDQuantity<ZeeDArrayD> fArrayRecQuantity;
    ZeeDArrayD fArrayRec;
    Bool_t bRecAvailable;

    /** current (set at each event) array of generated quantities */
    ZeeDQuantity<ZeeDArrayD> fArrayGenQuantity;
    ZeeDArrayD fArrayGen;
    Bool_t bGenAvailable;

};

#endif //ZeeDHistManagerEWUnfoldWriter

