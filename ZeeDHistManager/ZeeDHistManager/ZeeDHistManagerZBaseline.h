#ifndef __ZEEDHISTMANAGERZBaseline_H
#define __ZEEDHISTMANAGERZBaseline_H

#include "ZeeDHist/ZeeDHistManager.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDGenParticle.h"

#include "TString.h"

class ZeeDHistManagerZBaseline : public ZeeDHistManager {

public:

    // named constructors
    // see: http://www.parashift.com/c++-faq-lite/ctors.html#faq-10.8
    static ZeeDHistManagerZBaseline* Rec(TString name,
                                         Bool_t absY = kFALSE,
                                         TString binGridFile_dY = fDefault_BinGridFile_dY,
                                         TString binGridFile_dM = fDefault_BinGridFile_dM) {

        return new ZeeDHistManagerZBaseline(name, kTRUE, kFALSE, kFALSE, absY,
                                            binGridFile_dY,
                                            binGridFile_dM);

    }

    static ZeeDHistManagerZBaseline* Gen(TString name,
                                         Bool_t absY = kFALSE,
                                         TString binGridFile_dY = fDefault_BinGridFile_dY,
                                         TString binGridFile_dM = fDefault_BinGridFile_dM) {

        return new ZeeDHistManagerZBaseline(name, kFALSE, kTRUE, kFALSE, absY,
                                            binGridFile_dY,
                                            binGridFile_dM);

    }

    static ZeeDHistManagerZBaseline* GenRec(TString name,
                                            Bool_t absY = kFALSE,
                                            TString binGridFile_dY = fDefault_BinGridFile_dY,
                                            TString binGridFile_dM = fDefault_BinGridFile_dM) {

        return new ZeeDHistManagerZBaseline(name, kFALSE, kFALSE, kTRUE, absY,
                                            binGridFile_dY,
                                            binGridFile_dM);

    }

    // Book all histograms:
    void BookHistos();

    // Fill histos
    using ZeeDHistManager::Fill;
    void Fill();

    void SetAbsY(Bool_t absY) { bAbsY = absY; }
    Bool_t GetAbsY() { return bAbsY; }

private:

    // see ZeeDHistManagerZBaseline.cxx for defaults
    static const char* fDefault_BinGridFile_dY;
    static const char* fDefault_BinGridFile_dM;

    ZeeDHistManagerZBaseline(TString name,
                             Bool_t rec,
                             Bool_t gen,
                             Bool_t genrec,
                             Bool_t absY,
                             TString binGridFile_dY,
                             TString binGridFile_dM);

    void BookRec();
    void BookGen();
    void BookGenRec();

    void FillRec();
    void FillGen();
    void FillGenRec();

    const Bool_t bRec;
    const Bool_t bGen;
    const Bool_t bGenRec;

    Bool_t bAbsY;

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;

    const ZeeDEvent* fEvent;
    Double_t fEventWeight;
    Double_t fGenWeight;

    const ZeeDBosonZ* fZ_rec;
    const ZeeDGenParticle* fZ_gen;

    const TString fBinGridFile_dY;
    const TString fBinGridFile_dM;

    ZeeDBinGrid fBinGrid_dY;
    ZeeDBinGrid fBinGrid_dM;

    const TLorentzVector* fZ_rec_fourVec;
    const TLorentzVector* fZ_gen_fourVec;

    Double_t fZ_rec_Y;
    Double_t fZ_gen_Y;

};

#endif //ZeeDHistManagerZBaseline

