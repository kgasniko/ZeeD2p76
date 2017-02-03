#include "ZeeDHistManager/ZeeDHistManagerZBaseline.h"

//#define USE_DEBUG_MESSAGES
#include "ZeeDMisc/ZeeDMisc.h"

// const char* ZeeDHistManagerZBaseline::fDefault_BinGridFile_dY = "Binning/ZYBaseline.txt";
const char* ZeeDHistManagerZBaseline::fDefault_BinGridFile_dY = "Binning/BosAbsY.txt";
const char* ZeeDHistManagerZBaseline::fDefault_BinGridFile_dM = "Binning/ZMassBaseline.txt";

//-----------------------------------------------------
ZeeDHistManagerZBaseline::ZeeDHistManagerZBaseline(TString name,
                                                   Bool_t rec,
                                                   Bool_t gen,
                                                   Bool_t genrec,
                                                   Bool_t absY,
                                                   TString binGridFile_dY,
                                                   TString binGridFile_dM
                                                   )
                                                   :
                                                   ZeeDHistManager(name),
                                                   bRec(rec),
                                                   bGen(gen),
                                                   bGenRec(genrec),
                                                   bAbsY(absY),
                                                   fSvcHelper(name),
                                                   fAnaOptions(fSvcHelper.GetAnaSvc()),
                                                   fEvent(NULL),
                                                   fEventWeight(0),
                                                   fGenWeight(0),
                                                   fZ_rec(NULL),
                                                   fZ_gen(NULL),
                                                   fBinGridFile_dY(ZeeDMisc::FindFile(binGridFile_dY)),
                                                   fBinGridFile_dM(ZeeDMisc::FindFile(binGridFile_dM)),
                                                   fBinGrid_dY("dY"),
                                                   fBinGrid_dM("dM"),
                                                   fZ_rec_fourVec(NULL),
                                                   fZ_gen_fourVec(NULL)

{

    fBinGrid_dY.ReadFromFile(fBinGridFile_dY, kFALSE);
    fBinGrid_dM.ReadFromFile(fBinGridFile_dM, kFALSE);

}


//-----------------------------------------------------
void ZeeDHistManagerZBaseline::BookHistos()
{
    DEBUG_MES_START;

    if(bRec) {
        this->BookRec();
    }

    if(bGen) {
        this->BookGen();
    }

    if(bGenRec) {
        this->BookGenRec();
    }

    DEBUG_MES_END;
}

void ZeeDHistManagerZBaseline::BookRec()
{
    DEBUG_MES_START;
    AddTH1D("ZMassRec", fBinGridFile_dM, "M_{Z->ee} / GeV", "");
    AddTH1D("ZYRec", fBinGridFile_dY, "y_{Z->ee} / GeV", "");
    DEBUG_MES_END;
}

void ZeeDHistManagerZBaseline::BookGen()
{
    DEBUG_MES_START;
    AddTH1D("ZMassGen", fBinGridFile_dM, "M_{Z->ee} / GeV", "");
    AddTH1D("ZYGen", fBinGridFile_dY, "Y_{Z->ee} / GeV", "");
    DEBUG_MES_END;
}

void ZeeDHistManagerZBaseline::BookGenRec()
{
    DEBUG_MES_START;

    AddTH2D("ZMGenMRec",
            fBinGrid_dM,
            fBinGrid_dM,
            "M_{Z->ee, rec} / GeV",
            "M_{Z->ee, gen} / GeV");

    AddTH2D("ZYGenYRec",
            fBinGrid_dY,
            fBinGrid_dY,
            "Y_{Z->ee, rec} / GeV",
            "Y_{Z->ee, gen} / GeV");

    AddTH1D("ZYGenAndRec",
            fBinGridFile_dY,
            "Y_{Z->ee, rec} / GeV","");

    DEBUG_MES_END;
}

//-----------------------------------------------------
void ZeeDHistManagerZBaseline::Fill()
{
    DEBUG_MES_START;
    DEBUG_MES(this->GetHMName());

    // Get event
    fEvent = GetEvent();
    CHECK_NULL_PTR(fEvent);

    fEventWeight = fEvent->GetWeight();
    fGenWeight = fEvent->GetGenWeight();

    if( bRec || bGenRec) {

        fZ_rec = fEvent->GetCurrentBoson();
        fZ_rec_fourVec = &fZ_rec->GetFourVector();
        fZ_rec_Y = bAbsY ? TMath::Abs(fZ_rec_fourVec->Rapidity()) : fZ_rec_fourVec->Rapidity();

    }

    if( bGen || bGenRec) {

        fZ_gen = fEvent->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);
        fZ_gen_fourVec = &fZ_gen->GetMCFourVector();
        fZ_gen_Y = bAbsY ? TMath::Abs(fZ_gen_fourVec->Rapidity()) : fZ_gen_fourVec->Rapidity();

    }

    if(bRec) {
        this->FillRec();
    }

    if(bGen) {
        this->FillGen();
    }

    if(bGenRec) {
        this->FillGenRec();
    }

    DEBUG_MES_END;
}

//-----------------------------------------------------
void ZeeDHistManagerZBaseline::FillRec()
{
    DEBUG_MES_START;

    FillTH1(fZ_rec_fourVec->M(), fEventWeight, "ZMassRec");
    FillTH1(fZ_rec_Y, fEventWeight, "ZYRec");

    DEBUG_MES_END;
}

//-----------------------------------------------------
void ZeeDHistManagerZBaseline::FillGen()
{
    DEBUG_MES_START;

    FillTH1(fZ_gen_fourVec->M(), fGenWeight, "ZMassGen");
    FillTH1(fZ_gen_Y, fGenWeight, "ZYGen");

    DEBUG_MES_END;
}

//-----------------------------------------------------
void ZeeDHistManagerZBaseline::FillGenRec()
{
    DEBUG_MES_START;

    FillTH2(fZ_rec_fourVec->M(), fZ_gen_fourVec->M(), fEventWeight, "ZMGenMRec");
    FillTH2(fZ_rec_Y, fZ_gen_Y, fEventWeight, "ZYGenYRec");


    const Int_t genYMBin = GetBin1D(fZ_gen_Y, "ZYGenAndRec");
    const Int_t recYMBin = GetBin1D(fZ_rec_Y, "ZYGenAndRec");

    if (recYMBin == genYMBin){

        FillTH1(fZ_rec_Y, fEventWeight, "ZYGenAndRec" );
    }


    DEBUG_MES_END;
}
