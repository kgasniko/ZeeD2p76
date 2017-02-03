#include "ZeeDAnalysisCuts/ZeeDFinderTrigger.h"

#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"


ZeeDFinderTrigger::ZeeDFinderTrigger(TString Name) :  ZeeDFinder(Name) {}


void ZeeDFinderTrigger::BookCuts()
{
    // Books selection requirements

    std::string name = std::string("ZeeDAnalysisSvc");

    ServiceHandle<IZeeDAnalysisSvc> ZeeDAnaOptions(name, "ZeeDFinderTrigger");
    StatusCode sc = ZeeDAnaOptions.retrieve();

    if ( sc.isFailure() ) {
        throw std::runtime_error("ZeeDFinderTrigger: cannot retrieve " + name + " service");
    }

    Double_t elecEtaCut = ZeeDAnaOptions->Elec_EtaCut();
    Double_t ZMassCutMin = ZeeDAnaOptions->ZMassCutLooseMin();
    Double_t ZMassCutMax = ZeeDAnaOptions->ZMassCutLooseMax();

    Double_t EtaCrackMin = ZeeDAnaOptions->EtaCrackEMBEMEC_Min();
    Double_t EtaCrackMax = ZeeDAnaOptions->EtaCrackEMBEMEC_Max();



    // Book cuts:
    
    // Invariant mass of the two electrons falls into the mass window ZMassCutMin,ZMassCutMax
    this->AddCut(new ZeeDCutZMassZ               ("ZMassZ", ZMassCutMin, ZMassCutMax));
    // Max |Eta| over two electrons is smaller than elecEtaCut
    this->AddCut(new ZeeDCutEtaMaxBothElecZ      ("EtaMaxBothElecZ", elecEtaCut));
    // Both Electrons are outside Eta crack defined by EtaCrackMin,EtaCrackMax
    this->AddCut(new ZeeDCutEtaCrackBothElecZ    ("EtaCrackBothElecZ", EtaCrackMin, EtaCrackMax));
    // Both electrons pass author cut
    this->AddCut(new ZeeDCutAuthorBothElecZ      ("AuthorBothElecZ"));
    // Latest recommindation for OQmaps.
    this->AddCut(new ZeeDCutOQMaps               ("OQMaps"));

    // For the trigger efficiency investigations various cuts could be applied.
    // At this stage use loose selection conditions and accept all the bosons
    CutWeights.push_back(new CutWeight("ZMassZ",                       1., getBitMask("ZMassZ")));
    CutWeights.push_back(new CutWeight("EtaMaxBothElecZ",              1., getBitMask("EtaMaxBothElecZ")));
    CutWeights.push_back(new CutWeight("EtaCrackBothElecZ",            1., getBitMask("EtaCrackBothElecZ")));
    CutWeights.push_back(new CutWeight("AuthorBothElecZ",              1., getBitMask("AuthorBothElecZ")));
    CutWeights.push_back(new CutWeight("OQMaps",                       1., getBitMask("OQMaps")));
}

