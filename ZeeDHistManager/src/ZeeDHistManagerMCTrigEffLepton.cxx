#include "ZeeDHistManager/ZeeDHistManagerMCTrigEffLepton.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>
#include <TH1.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDLepton.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDTools/ZeeDBags.h"
#include "ZeeDTools/ZeeDCutBit.h"

using std::cout;
using std::endl;
//-----------------------------------------------------



void ZeeDHistManagerMCTrigEffLepton::BookHistos()
{     
    const TString LepEataPtTrigmatchBins = ZeeDMisc::FindFile("Binning/ElecEtaPt.txt");
    AddTH2D("LepEtaPtTrigMatch",    LepEataPtTrigmatchBins, "#eta", "P_{t} [MeV]");
    AddTH2D("LepEtaPtTrigMatchPlus", LepEataPtTrigmatchBins, "#eta", "P_{t} [MeV]");
    AddTH2D("LepEtaPtTrigMatchMinus", LepEataPtTrigmatchBins, "#eta", "P_{t} [MeV]");
    AddTH2D("LepEtaPtNoTrigMatch",  LepEataPtTrigmatchBins, "#eta", "P_{t} [MeV]"); 
    AddTH2D("LepEtaPtNoTrigMatchMinus",  LepEataPtTrigmatchBins, "#eta", "P_{t} [MeV]"); 
    AddTH2D("LepEtaPtNoTrigMatchPlus",  LepEataPtTrigmatchBins, "#eta", "P_{t} [MeV]"); 
    const TString ptBins=ZeeDMisc::FindFile("Binning/ElecPt.txt");
    const TString etaBins = ZeeDMisc::FindFile("Binning/ElecEta.txt"); 
    AddTH1D("LepPtNoTrigMatch", ptBins, "P_{t} [MeV]");
    AddTH1D("LepPtNoTrigMatchMinus", ptBins, "P_{t} [MeV]");
    AddTH1D("LepPtNoTrigMatchPlus", ptBins, "P_{t} [MeV]");
    AddTH1D("LepPtTrigMatch", ptBins, "P_{t} [MeV]");
    AddTH1D("LepPtTrigMatchMinus", ptBins, "P_{t} [MeV]");
    AddTH1D("LepPtTrigMatchPlus", ptBins, "P_{t} [MeV]");

    AddTH1D("LepPtNoTrigMatch2", 50, 20, 70, "P_{t} [MeV]");
    AddTH1D("LepPtNoTrigMatchMinus2", 50, 20, 70, "P_{t} [MeV]");
    AddTH1D("LepPtNoTrigMatchPlus2", 50, 20, 70, "P_{t} [MeV]");
    AddTH1D("LepPtTrigMatch2", 50, 20, 70, "P_{t} [MeV]");
    AddTH1D("LepPtTrigMatchMinus2", 50, 20, 70, "P_{t} [MeV]");
    AddTH1D("LepPtTrigMatchPlus2", 50, 20, 70, "P_{t} [MeV]");


    AddTH1D("LepEtaNoTrigMatch", etaBins, "#eta");
    AddTH1D("LepEtaNoTrigMatchMinus", etaBins, "#eta");
    AddTH1D("LepEtaNoTrigMatchPlus", etaBins, "#eta");
    AddTH1D("LepEtaTrigMatch", etaBins, "#eta");
    AddTH1D("LepEtaTrigMatchMinus", etaBins, "#eta");
    AddTH1D("LepEtaTrigMatchPlus", etaBins, "#eta");

}


void ZeeDHistManagerMCTrigEffLepton::FillLeptonHists(const TObjArray* leptons, Double_t Weight)
{   
    this->SetProperties(kFALSE,kFALSE);
    if (leptons == NULL) {
        return;
    }    

    for ( Int_t i = 0; i < leptons->GetEntriesFast(); ++i ) {
        const ZeeDLepton* lepton = static_cast< ZeeDLepton* > (leptons->At(i));
        const TLorentzVector& fourVector = lepton->GetFourVector();
        const Double_t Thouthand = 1000.0; 

        const Bool_t bTriggMatSet = lepton->Trigger().IsSet();
        const Bool_t bMatchToTrigg1 = bTriggMatSet && lepton->IsMatchedToTriggerChain(ZeeDEnum::Trigger::EF_mu10);
        const Double_t charge      = lepton->getCharge();

        if (bMatchToTrigg1) { // Fill histogram for lepton matched to one of two triggers
            FillTH2(fourVector.Eta(), fourVector.Pt(), Weight, "LepEtaPtTrigMatch");
            FillTH1(fourVector.Eta(), Weight, "LepEtaTrigMatch");
            FillTH1(fourVector.Pt(), Weight, "LepPtTrigMatch2");
            FillTH1(fourVector.Pt(), Weight, "LepPtTrigMatch");
            if (charge > 0){
                FillTH1(fourVector.Eta(), Weight, "LepEtaTrigMatchPlus");
                FillTH1(fourVector.Pt(), Weight, "LepPtTrigMatchPlus");
                FillTH1(fourVector.Pt(), Weight, "LepPtTrigMatchPlus2");

                FillTH2(fourVector.Eta(), fourVector.Pt(), Weight, "LepEtaPtTrigMatchPlus");
            } else {
                FillTH1(fourVector.Eta(), Weight, "LepEtaTrigMatchMinus");
                FillTH1(fourVector.Pt(), Weight, "LepPtTrigMatchMinus");
                FillTH1(fourVector.Pt(), Weight, "LepPtTrigMatchMinus2");
                FillTH2(fourVector.Eta(), fourVector.Pt(), Weight, "LepEtaPtTrigMatchMinus");
            }


        }
        FillTH1(fourVector.Eta(), Weight, "LepEtaNoTrigMatch");
        FillTH1(fourVector.Pt(), Weight, "LepPtNoTrigMatch");
        FillTH1(fourVector.Pt(), Weight, "LepPtNoTrigMatch2");

        // Fill histogram for all leptons without trigg matching cut
        FillTH2 ( fourVector.Eta(), fourVector.Pt(),Weight, "LepEtaPtNoTrigMatch");
        if (charge > 0){
            FillTH1(fourVector.Eta(), Weight, "LepEtaNoTrigMatchPlus");
            FillTH1(fourVector.Pt(), Weight, "LepPtNoTrigMatchPlus");
            FillTH1(fourVector.Pt(), Weight, "LepPtNoTrigMatchPlus2");
            FillTH2(fourVector.Eta(), fourVector.Pt() , Weight, "LepEtaPtNoTrigMatchPlus");
        } else {
            FillTH1(fourVector.Eta(), Weight, "LepEtaNoTrigMatchMinus");
            FillTH1(fourVector.Pt(), Weight, "LepPtNoTrigMatchMinus2");
            FillTH1(fourVector.Pt(), Weight, "LepPtNoTrigMatchMinus");
            FillTH2(fourVector.Eta(), fourVector.Pt() , Weight, "LepEtaPtNoTrigMatchMinus");
        }
    }

}


