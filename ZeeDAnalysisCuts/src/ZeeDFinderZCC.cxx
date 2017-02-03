#include "ZeeDAnalysisCuts/ZeeDFinderZCC.h"

#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"


ZeeDFinderZCC::ZeeDFinderZCC(TString Name) :
    ZeeDFinder(Name) {}

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
    this->AddCut(new ZeeDCutZMassZ               ("ZMassZ", fZMassCutMin, fZMassCutMax));

    // Pt of both electrons is bigger than elecPtCut
    this->AddCut(new ZeeDCutAsymmetricalLepPt   ("PtMinBothElecZ" , 15., 20.));

    // Max |Eta| over two electrons is smaller than elecEtaCut
    this->AddCut(new ZeeDCutEtaMaxBothElecZ      ("EtaMaxBothElecZ", elecEtaCut));

    // Both Electrons are outside Eta crack defined by EtaCrackMin,EtaCrackMax
    this->AddCut(new ZeeDCutEtaCrackBothElecZ    ("EtaCrackBothElecZ", EtaCrackMin, EtaCrackMax));

    // Both electrons pass author cut "isElectron"
    this->AddCut(new ZeeDCutAuthorBothElecZ      ("AuthorBothElecZ"));


    // Set cuts for the boson selector
    CutWeights.push_back(new CutWeight("EtaMaxBothElecZ",  100., getBitMask("EtaMaxBothElecZ")));
    CutWeights.push_back(new CutWeight("PtMinBothElecZ",    10., getBitMask("PtMinBothElecZ")));
    CutWeights.push_back(new CutWeight("ZMassZ",            10., getBitMask("ZMassZ")));
    CutWeights.push_back(new CutWeight("EtaCrackBothElecZ",  1., getBitMask("EtaCrackBothElecZ")));
    CutWeights.push_back(new CutWeight("AuthorBothElecZ",    1., getBitMask("AuthorBothElecZ")));

}

