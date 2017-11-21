#include "ZeeDAnalysisCuts/ZeeDFinderZCC.h"

#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"


ZeeDFinderZCC::ZeeDFinderZCC(TString Name) :
    ZeeDFinder(Name) {}

//------------------------------------------------------
TObjArray ZeeDFinderZCC::SelectAllGoodBosons( ZeeDEvent* event, const TObjArray* bosons)
{
    // Find all the bosons satysfying the selection
    TObjArray bosonArray;
    ZeeDCutBit Mask;

    // safe current boson - loop will change it for weight calculation
    const ZeeDBosonZ* currentBosonSafe = event->GetCurrentBoson();

    // bit mask for the case all the cuts passed, like: (ncuts == 3) ==> all_bits_1 = 0...0111
    unsigned int all_bits_1 = all_bits_1_lookup(CutWeights.size());

    for (Int_t i = 0; i < bosons->GetEntriesFast(); i++) {
        ZeeDBosonZ* boson = static_cast<ZeeDBosonZ*>(bosons->At(i));
        if (boson != NULL) {
            event->SetCurrentBoson(boson);
            this->evaluate(event, &Mask);
            if (all_bits_1==Mask.GetMask()){
                bosonArray.Add((TObject*)boson);
            }

        }
    }

    // restore current boson
    event->SetCurrentBoson(currentBosonSafe);
    TObjArray buf;
    if (bosonArray.GetEntriesFast()>1)
        return buf;
    else
        return bosonArray;



}


void ZeeDFinderZCC::BookCuts()
{
    // Books selection requirements

    std::string name = std::string("ZeeDAnalysisSvc");

    ServiceHandle<IZeeDAnalysisSvc> ZeeDAnaOptions(name, "ZeeDFinderZCC");
    StatusCode sc = ZeeDAnaOptions.retrieve();

    if ( sc.isFailure() ) {
        throw std::runtime_error("ZeeDFinderZCC: cannot retrieve " + name + " service");
    }

    //Double_t elecPtCut  = ZeeDAnaOptions->Elec_PtCut();

    Double_t elecEtaCut = ZeeDAnaOptions->Elec_EtaCut();

    Double_t EtaCrackMin = ZeeDAnaOptions->EtaCrackEMBEMEC_Min();
    Double_t EtaCrackMax = ZeeDAnaOptions->EtaCrackEMBEMEC_Max();

    Double_t fZMassCutMin = ZeeDAnaOptions->ZMassCutVeryLooseMin();
    Double_t fZMassCutMax = ZeeDAnaOptions->ZMassCutVeryLooseMax();


    // Book cuts:

    // Invariant mass of the two electrons falls into the mass window ZMassCutMin,ZMassCutMax
    //this->AddCut(new ZeeDCutZMassZ               ("ZMassZ", fZMassCutMin, fZMassCutMax));

    // Pt of both electrons is bigger than elecPtCut
    this->AddCut(new ZeeDCutAsymmetricalLepPt   ("PtMinBothElecZ" , 20., 20.));

    // Max |Eta| over two electrons is smaller than elecEtaCut
    this->AddCut(new ZeeDCutEtaMaxBothElecZ      ("EtaMaxBothElecZ", 2.47));

    // Both Electrons are outside Eta crack defined by EtaCrackMin,EtaCrackMax
    this->AddCut(new ZeeDCutEtaCrackBothElecZ    ("EtaCrackBothElecZ", 1.37, 1.52));

    // Both electrons pass author cut "isElectron"
//    this->AddCut(new ZeeDCutAuthorBothElecZ      ("AuthorBothElecZ"));


    // Set cuts for the boson selector
    CutWeights.push_back(new CutWeight("EtaMaxBothElecZ",  100., getBitMask("EtaMaxBothElecZ")));
    CutWeights.push_back(new CutWeight("PtMinBothElecZ",    10., getBitMask("PtMinBothElecZ")));
    //CutWeights.push_back(new CutWeight("ZMassZ",            10., getBitMask("ZMassZ")));
    CutWeights.push_back(new CutWeight("EtaCrackBothElecZ",  1., getBitMask("EtaCrackBothElecZ")));
//    CutWeights.push_back(new CutWeight("AuthorBothElecZ",    1., getBitMask("AuthorBothElecZ")));

}

