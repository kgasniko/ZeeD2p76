#include "ZeeDHistManager/ZeeDHistManagerEWUnfoldWriter.h"

#define USE_DEBUG_MESSAGES
#define DEBUG_ON
#include "ZeeDMisc/ZeeDMisc.h"

#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDGenParticle.h"

#include <TLorentzVector.h>


//-----------------------------------------------------
ZeeDHistManagerEWUnfoldWriter::ZeeDHistManagerEWUnfoldWriter(TString ntuple_name,
                                                             TString binGridFile,
                                                             ZeeDQuantity<ZeeDArrayD> (*cVarREC)(const ZeeDEvent*),
                                                             ZeeDQuantity<ZeeDArrayD> (*cVarGEN)(const ZeeDEvent*)
                                                             )
  : ZeeDHistManager(ntuple_name),
    fName(ntuple_name),
    fTreeOut(NULL),
    fRecoWeight(1),
    fRecoIsReconstructed(dontcare),
    fTruthIsFiducial(dontcare),
    fForAcceptance(dontcare),
    fMCTruthWeight(1),
    fPileUpWeight(1),
    fChannel(3),
    fSvcHelper(ntuple_name),
    fAnaOptions(fSvcHelper.GetAnaSvc()),
    fMode(histograms),
    fBinGridFile(binGridFile),
    fBinGrid(NULL),
    fVarREC(cVarREC),
    fVarGEN(cVarGEN),
    fDim(0)
{

}

//-----------------------------------------------------
void ZeeDHistManagerEWUnfoldWriter::BookHistos()
{
    DEBUG_MES_START;

    switch (fMode) {

    case trees: {

        new TFile(fName+".root", "RECREATE");
        fTreeOut = new TTree("EWUnfold_ntuple", "EWUnfold ntuple");

        fTreeOut->Branch("RecoVar1", &fRecoVar1);
        fTreeOut->Branch("TruthVar1", &fTruthVar1);

        fTreeOut->Branch("RecoVar2", &fRecoVar2);
        fTreeOut->Branch("TruthVar2", &fTruthVar2);

        fTreeOut->Branch("RecoWeight", &fRecoWeight);

        fTreeOut->Branch("RecoIsReconstructed", &fRecoIsReconstructed);
        fTreeOut->Branch("TruthIsFiducial", &fTruthIsFiducial);

        fTreeOut->Branch("Channel", &fChannel);

        fTreeOut->Branch("MCTruthWeight", &fMCTruthWeight);

        fTreeOut->Branch("PileUpWeight", &fPileUpWeight);

        fTreeOut->Branch("runNumber"     , &fRunNumber);
        fTreeOut->Branch("origRunNumber" , &fOrigRunNumber);
        fTreeOut->Branch("eventNumber"   , &fEventNumber);
        fTreeOut->Branch("LB"            , &fLB);
        fTreeOut->Branch("eventWeight"   , &fEventWeight, "eventWeight/F");

        break;

    }

    case histograms:

        ZeeDBinGrid tmp_grid;
        tmp_grid.ReadFromFile(fBinGridFile, kFALSE);
        fDim = tmp_grid.GetBinEdges()->GetEntriesFast();

        if(fTruthIsFiducial == yes &&
           fRecoIsReconstructed == yes) {
            this->AddHistoFromGrid("fidCorrNum", fBinGridFile, ZeeDHistLevel::ToyMC);
            this->AddHistoFromGrid("effCorrNum", fBinGridFile, ZeeDHistLevel::ToyMC);
            this->AddHistoFromGrid("purityNum", fBinGridFile);

            this->AddResponseHistoFromGrid("ResponseMatrix_1", fBinGridFile, kTRUE, ZeeDHistLevel::ToyMC);
            if(fDim==2) {
                this->AddResponseHistoFromGrid("ResponseMatrix_2", fBinGridFile, kFALSE, ZeeDHistLevel::ToyMC);
            }

        }

        if(fRecoIsReconstructed == yes && fTruthIsFiducial == dontcare) {
            this->AddHistoFromGrid("fidCorrDenom", fBinGridFile, ZeeDHistLevel::ToyMC);
            this->AddHistoFromGrid("purityDenom", fBinGridFile);
            this->AddHistoFromGrid("stabilityDenom", fBinGridFile);
        }

        if(fTruthIsFiducial == yes && fRecoIsReconstructed == dontcare) {
            this->AddHistoFromGrid("effCorrDenom", fBinGridFile, ZeeDHistLevel::ToyMC);
        }

        if(fForAcceptance == yes && fRecoIsReconstructed == dontcare && fTruthIsFiducial == dontcare) {
            this->AddHistoFromGrid("gen", fBinGridFile);
        }

        break;

    }

    DEBUG_MES_END;
}

//-----------------------------------------------------
void ZeeDHistManagerEWUnfoldWriter::Fill()
{
    DEBUG_MES_START;
    DEBUG_MES(this->GetHMName());

    // Get event

    const ZeeDEvent* event = this->GetEvent();

    CHECK_NULL_PTR(event);

    fRecoWeight = event->GetWeight();
    fMCTruthWeight = event->GetGenWeight();

    const ZeeDBosonZ* boson = event->GetCurrentBoson();
    const ZeeDGenParticle* gen_boson = event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);

    DEBUG_VAR(event->RunNumber().IsSet());
    fRunNumber     = event->RunNumber().Get();
    DEBUG_VAR(event->OrigRunNumber().IsSet());
    fOrigRunNumber = event->OrigRunNumber().Get();
    DEBUG_VAR(event->EventNumber().IsSet());
    fEventNumber   = event->EventNumber().Get();
    DEBUG_VAR(event->LB().IsSet());
    fLB            = event->LB().Get();

    fEventWeight = event->GetWeight();
    DEBUG_VAR(eventWeight);

    switch(fMode) {

    case trees: {

        if( boson && fRecoIsReconstructed == yes) {

            const TLorentzVector& boson_fourVec = boson->GetFourVector();

            fRecoVar1 = boson_fourVec.M();
            fRecoVar2 = boson_fourVec.Rapidity();

        } else {

            fRecoVar1 = 0;
            fRecoVar2 = 0;

        }

        if( gen_boson ) {

            const TLorentzVector& gen_boson_fourVec = gen_boson->GetMCFourVector();

            fTruthVar1 = gen_boson_fourVec.M();
            fTruthVar2 = gen_boson_fourVec.Rapidity();

        } else {

            fTruthVar1 = 0;
            fTruthVar2 = 0;

        }

        fTreeOut->Fill();

        break;

    }

    case histograms: {

        fArrayRecQuantity = (*fVarREC)(event);
        bRecAvailable = fArrayRecQuantity.IsSet();

        if (bRecAvailable) {
            fArrayRec = fArrayRecQuantity.Get();
        }

        fArrayGenQuantity = (*fVarGEN)(event);
        bGenAvailable = fArrayGenQuantity.IsSet();

        if (bGenAvailable) {
            fArrayGen = fArrayGenQuantity.Get();

            switch(fDim) {

            case 1:
                FillTH1(fArrayGen[0], fMCTruthWeight, "gen");
                break;

            case 2:
                FillTH2(fArrayGen[0], fArrayGen[1], fMCTruthWeight, "gen");
                break;

            }

        }

        switch(fDim) {

        case 1: {

            if(fTruthIsFiducial == yes &&
               fRecoIsReconstructed == yes) {

                const UInt_t recbin = FillTH1(fArrayRec[0], fEventWeight, "fidCorrNum");
                const UInt_t genbin = FillTH1(fArrayGen[0], fEventWeight, "effCorrNum");

                if(recbin == genbin) {

                    FillTH1(fArrayRec[0], fEventWeight, "purityNum");

                }

                FillTH2(fArrayRec[0], fArrayGen[0], fEventWeight, "ResponseMatrix_1");

            }

            if(fTruthIsFiducial == yes) {

                FillTH1(fArrayGen[0], fMCTruthWeight, "effCorrDenom");

            }

            if(fRecoIsReconstructed == yes) {

                FillTH1(fArrayRec[0], fEventWeight, "purityDenom");
                FillTH1(fArrayRec[0], fEventWeight, "fidCorrDenom");

                if(bGenAvailable) FillTH1(fArrayGen[0], fEventWeight, "stabilityDenom");

            }

            break;

        }

        case 2: {

            if(fTruthIsFiducial == yes &&
               fRecoIsReconstructed == yes) {

                const UInt_t recbin = FillTH2(fArrayRec[0], fArrayRec[1], fEventWeight, "fidCorrNum");
                const UInt_t genbin = FillTH2(fArrayGen[0], fArrayGen[1], fEventWeight, "effCorrNum");

                if(recbin == genbin) {

                    FillTH2(fArrayRec[0], fArrayRec[1], fEventWeight, "purityNum");

                }

                FillTH3(fArrayRec[1], fArrayGen[1], fArrayGen[0], fEventWeight, "ResponseMatrix_1");
                FillTH3(fArrayRec[0], fArrayGen[0], fArrayGen[1], fEventWeight, "ResponseMatrix_2");

            }

            if(fTruthIsFiducial == yes) {

                FillTH2(fArrayGen[0], fArrayGen[1], fMCTruthWeight, "effCorrDenom");

            }

            if(fRecoIsReconstructed == yes) {

                FillTH2(fArrayRec[0], fArrayRec[1], fEventWeight, "purityDenom");
                FillTH2(fArrayRec[0], fArrayRec[1], fEventWeight, "fidCorrDenom");
                if(bGenAvailable) FillTH2(fArrayGen[0], fArrayGen[1], fEventWeight, "stabilityDenom");

            }

            break;

        }

        }

    }

    }

    DEBUG_MES_END;

}

void ZeeDHistManagerEWUnfoldWriter::Finalize()
{

    switch(fMode) {

    case trees: {

        Info("ZeeDHistManagerEWUnfoldWriter::Finalize", " Close TTree ");

        fTreeOut->Print();
        TFile* tfile = fTreeOut->GetCurrentFile();
        tfile->Write();
        tfile->Close();

        break;

    }

    default:
        break;

    }

}
