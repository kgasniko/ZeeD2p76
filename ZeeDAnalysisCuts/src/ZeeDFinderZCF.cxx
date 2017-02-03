#include "ZeeDAnalysisCuts/ZeeDFinderZCF.h"

#include <TObjArray.h>

#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include "ZeeDEvent/ZeeDEvent.h"


//------------------------------------------------------
void ZeeDFinderZCF::BookCuts()
{
    // Books selection requirements

    std::string name = std::string("ZeeDAnalysisSvc");

    ServiceHandle<IZeeDAnalysisSvc> ZeeDAnaOptions(name, "ZeeDCalcZ");
    StatusCode sc = ZeeDAnaOptions.retrieve();

    if ( sc.isFailure() ) {
        throw std::runtime_error("ZeeDCalcZ: cannot retrieve " + name + " service");
    }

    Double_t elecEtaCut       = ZeeDAnaOptions->Elec_EtaCut();
    Double_t elecEtaFwdMin    = ZeeDAnaOptions->Elec_EtaFwdMin();
    Double_t elecEtaFwdMax    = ZeeDAnaOptions->Elec_EtaFwdMax();
    Double_t ZMassCutMin      = ZeeDAnaOptions->ZMassCutLooseMin();
    Double_t ZMassCutMax      = ZeeDAnaOptions->ZMassCutLooseMax();
    Double_t EtaCrackMin      = ZeeDAnaOptions->EtaCrackEMBEMEC_Min();
    Double_t EtaCrackMax      = ZeeDAnaOptions->EtaCrackEMBEMEC_Max();
    Double_t elecCentralPtCut = ZeeDAnaOptions->Elec_CentralPtMinCut();    
    Double_t elecForwardPtCut = ZeeDAnaOptions->Elec_ForwardPtMinCut();
    
    // Book cuts:

    this->AddCut(new ZeeDCutZMassZ                    ("ZMassZ", ZMassCutMin, ZMassCutMax));
    this->AddCut(new ZeeDCutPtMinCentralLepZ         ("PtMinCentralElecZ", elecCentralPtCut));
    this->AddCut(new ZeeDCutPtMinForwardLepZ         ("PtMinForwardElecZ", elecForwardPtCut));
    this->AddCut(new ZeeDCutEtaCFBothElecZ            ("EtaCFBothElecZ",  elecEtaFwdMin, elecEtaFwdMax, elecEtaCut));

    // Both Electrons are outside Eta crack defined by EtaCrackMin,EtaCrackMax
    this->AddCut(new ZeeDCutEtaCrackBothElecZ         ("EtaCrackBothElecZ", EtaCrackMin, EtaCrackMax));
    
    // One electron IsEM forward Loose Cut
    this->AddCut(new ZeeDCutIsEMCentralForwardLepZ   ("IsEMFwdLooseOneElecZ", &ZeeDLeptonBags::IsEM::isNewFwdLoose, ZeeDEnum::Direction::Forward));

    // One electron IsEM central Tight Cut
    this->AddCut(new ZeeDCutIsEMCentralForwardLepZ   ("IsEMTightOneElecZ", &ZeeDLeptonBags::IsEM::isTight, ZeeDEnum::Direction::Central));

    // Author of both electons, one for cluster algorithm another for forward
    this->AddCut(new ZeeDCutAuthorCFElecZ             ("AuthorCFElecZ"));
    
    // Isolation cut for forward electron
    this->AddCut(new ZeeDCutCentralElecIso            ("Iso98Etcone20", &ZeeDElectronBags::EIso::Iso98Etcone20));

    // Maps:
    this->AddCut(new ZeeDCutOQMaps                    ("OQMaps"));

    // Rank cuts differently:

    CutWeights.push_back(new CutWeight("ZMassZ",                    10., getBitMask("ZMassZ")));  // LARGE priority
    CutWeights.push_back(new CutWeight("PtMinCentralElecZ",          1., getBitMask("PtMinCentralElecZ")));
    CutWeights.push_back(new CutWeight("PtMinForwardElecZ",          1., getBitMask("PtMinForwardElecZ")));
    CutWeights.push_back(new CutWeight("EtaCFBothElecZ",            20., getBitMask("EtaCFBothElecZ"))); // LARGE priority
    CutWeights.push_back(new CutWeight("EtaCrackBothElecZ",          1., getBitMask("EtaCrackBothElecZ")));
    CutWeights.push_back(new CutWeight("OQMaps",                     1., getBitMask("OQMaps")));
    CutWeights.push_back(new CutWeight("IsEMFwdLooseOneElecZ",       1., getBitMask("IsEMFwdLooseOneElecZ")));
    CutWeights.push_back(new CutWeight("IsEMTightOneElecZ",          1., getBitMask("IsEMTightOneElecZ")));
    CutWeights.push_back(new CutWeight("AuthorCFElecZ",              1., getBitMask("AuthorCFElecZ")));
    CutWeights.push_back(new CutWeight("CaloIso98",                  1., getBitMask("Iso98Etcone20")));

}

