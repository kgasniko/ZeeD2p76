#include "ZeeDAnalysisCuts/ZeeDFinderZLowMass.h"

#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"


ZeeDFinderZLowMass::ZeeDFinderZLowMass(TString Name) :  ZeeDFinder(Name) {}


void ZeeDFinderZLowMass::BookCuts()
{
    // Books selection requirements

    std::string name = std::string("ZeeDAnalysisSvc");

    ServiceHandle<IZeeDAnalysisSvc> ZeeDAnaOptions(name, "ZeeDFinderZLowMass");
    StatusCode sc = ZeeDAnaOptions.retrieve();

    if ( sc.isFailure() ) {
        throw std::runtime_error("ZeeDFinderZLowMass: cannot retrieve " + name + " service");
    }

    Double_t elecPtCut  = ZeeDAnaOptions->Elec_PtCut();
    Double_t elecEtaCut = ZeeDAnaOptions->Elec_EtaCut();
    Double_t ZMassCutMin = ZeeDAnaOptions->ZLowMassCutMin();
    Double_t ZMassCutMax = ZeeDAnaOptions->ZLowMassCutMax();

    Double_t EtaCrackMin = ZeeDAnaOptions->EtaCrackEMBEMEC_Min();
    Double_t EtaCrackMax = ZeeDAnaOptions->EtaCrackEMBEMEC_Max();



    // Book cuts:

    // Invariant mass of the two electrons falls into the mass window ZMassCutMin,ZMassCutMax
    this->AddCut(new ZeeDCutZMassZ               ("ZMassZ", ZMassCutMin, ZMassCutMax));
    // Pt of both electrons is bigger than elecPtCut
    this->AddCut(new ZeeDCutPtMinBothLepZ       ("PtMinBothElecZ" , elecPtCut));
    // Max |Eta| over two electrons is smaller than elecEtaCut
    this->AddCut(new ZeeDCutEtaMaxBothElecZ      ("EtaMaxBothElecZ", elecEtaCut));
    // Both Electrons are outside Eta crack defined by EtaCrackMin,EtaCrackMax
    this->AddCut(new ZeeDCutEtaCrackBothElecZ    ("EtaCrackBothElecZ", EtaCrackMin, EtaCrackMax));
    // Both electrons pass author cut
    this->AddCut(new ZeeDCutAuthorBothElecZ      ("AuthorBothElecZ"));
    // Is EM medium with track match cut
    this->AddCut(new ZeeDCutIsEMBothLepZ        ("IsEMTightBothElecZ", &ZeeDLeptonBags::IsEM::isTight));
    // Latest recommindation for OQmaps.
    this->AddCut(new ZeeDCutOQMaps               ("OQMaps"));
    // Electron charge
    this->AddCut(new ZeeDCutChargeBothLepZ      ("ChargeBothElecOppositeZ"));
    // Both Electrons Isolation
    this->AddCut(new ZeeDCutBothCentralElecCaloIso("BothCaloIso98", &ZeeDElectronBags::CaloIso::CaloIso98));
    this->AddCut(new ZeeDCutBothCentralElecCaloIso("BothCaloIso95", &ZeeDElectronBags::CaloIso::CaloIso95));

    // Rank cuts differently:

    CutWeights.push_back(new CutWeight("ZMassZ",                      10., getBitMask("ZMassZ")));  // LARGE priority
    CutWeights.push_back(new CutWeight("PtMinBothElecZ",               1., getBitMask("PtMinBothElecZ")));
    CutWeights.push_back(new CutWeight("EtaMaxBothElecZ",             20., getBitMask("EtaMaxBothElecZ")));
    CutWeights.push_back(new CutWeight("EtaCrackBothElecZ",            1., getBitMask("EtaCrackBothElecZ")));
    CutWeights.push_back(new CutWeight("IsEMTightBothElecZ",           1., getBitMask("IsEMTightBothElecZ")));
    CutWeights.push_back(new CutWeight("ChargeBothElecOppositeZ",      1., getBitMask("ChargeBothElecOppositeZ")));
    CutWeights.push_back(new CutWeight("OQMaps",                       1., getBitMask("OQMaps")));
    CutWeights.push_back(new CutWeight("AuthorBothElecZ",              1., getBitMask("AuthorBothElecZ")));
    CutWeights.push_back(new CutWeight("BothCaloIso98",                1., getBitMask("BothCaloIso98")));
}

