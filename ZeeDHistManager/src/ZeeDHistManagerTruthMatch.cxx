#include "ZeeDHistManager/ZeeDHistManagerTruthMatch.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDGenParticle.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

//#define USE_DEBUG_MESSAGES
#include "ZeeDMisc/ZeeDMisc.h"


using std::cout;
using std::endl;

//-----------------------------------------------------
void ZeeDHistManagerTruthMatch::BookHistos()
{
    // Books histograms

    // Get analysis svc
    fZeeDAnaOptions = fSvcHelper.GetAnaSvc();

    const TString BosY      = ZeeDMisc::FindFile("Binning/BosY.txt");
    const TString ElecEta   = ZeeDMisc::FindFile("Binning/ElecEta.txt");
    const TString BosPt     = ZeeDMisc::FindFile("Binning/BosPt.txt");

    AddTH1D("ERecvsEGen",           100,    -0.4, 0.2,       " E_{Rec}/E_{Gen} -1 ", "");
    AddTH1D("ElecEtaRes",           100,    -0.05, 0.05,     " #eta_{Rec} - #eta_{Gen} ", "");
    AddTH1D("ElecPtRes",            100,    -0.5, 0.5,       " (P_{T}^{Rec}-P_{T}^{Gen})/P_{T}^{Gen} ", "");
    AddTH1D("ElecDeltaR",           100,     0.0, 0.2,       " DeltaR ", "");
    AddTH1D("ElecPt",               20,     0.0, 120.,      " P_{T} Rec&Gen ", "");
    AddTH1D("ElecEta",              ElecEta,      " #eta Rec&Gen ", "");

    AddTH1D("BosY",                 BosY,    "Y (Z->ee) ", "");
    AddTH1D("BosPt",                BosPt,   "P_{T} (Z->ee) ", "");

}

//-----------------------------------------------------
void ZeeDHistManagerTruthMatch::Fill()
{
    DEBUG_MES_START;
    DEBUG_MES(this->GetHMName());



    // Get event
    const ZeeDEvent* event = GetEvent();
    CHECK_NULL_PTR(event);

    // Event weight
    Double_t Weight = event->GetWeight();
    // How should I apply weight for this HM?


    const ZeeDGenParticle* GenBoson  = event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);
    const ZeeDBosonZ*      RecBoson  = event->GetCurrentBoson();
    TObjArray electrons;
    electrons.Add((TObject*)(RecBoson->GetFirstLep()));
    electrons.Add((TObject*)(RecBoson->GetSecondLep()));

    if ( (GenBoson == NULL) || (RecBoson == NULL) || (electrons.GetEntriesFast() == 0) ) {
        return;
    }

    TLorentzVector RecBosFV =  RecBoson->GetFourVector();
    TLorentzVector GenBosFV =  GenBoson->GetMCFourVector();

    if ( GetBin1D(RecBosFV.Rapidity(), "BosY") == GetBin1D(GenBosFV.Rapidity(), "BosY")  ) {
        FillTH1(GenBosFV.Rapidity(),  Weight, "BosY");
    }

    if ( GetBin1D(RecBosFV.Pt(), "BosPt") == GetBin1D(GenBosFV.Pt(), "BosPt")  ) {
        FillTH1(GenBosFV.Pt(),  Weight, "BosPt");
    }

    for ( Int_t i = 0; i < electrons.GetEntriesFast(); ++i ) {
        const ZeeDElectron*    RecElec = static_cast< ZeeDElectron* > (electrons.At(i));
        const ZeeDGenParticle* GenElec = RecElec->GetGenBornMatchedLepton();

        if (GenElec == NULL) {
            continue;
        }

        TLorentzVector RecElecFV =  RecElec->GetFourVector();
        TLorentzVector GenElecFV =  GenElec->GetMCFourVector();


        FillTH1((RecElecFV.Pt() - GenElecFV.Pt()) / GenElecFV.Pt(),  Weight, "ElecPtRes");
        FillTH1(RecElecFV.Eta() - GenElecFV.Eta(),  Weight, "ElecEtaRes");
        FillTH1((RecElecFV.E() / GenElecFV.E()) - 1,  Weight, "ERecvsEGen");
        FillTH1(RecElecFV.DeltaR(GenElecFV),  Weight, "ElecDeltaR");

        if ( GetBin1D(RecElecFV.Pt(), "ElecPt") == GetBin1D(GenElecFV.Pt(), "ElecPt")  ) {
            FillTH1(GenElecFV.Pt(),  Weight, "ElecPt");
        }

        if ( GetBin1D(RecElecFV.Eta(), "ElecEta") == GetBin1D(GenElecFV.Eta(), "ElecEta")  ) {
            FillTH1(GenElecFV.Eta(),  Weight, "ElecEta");
        }

    }


}
