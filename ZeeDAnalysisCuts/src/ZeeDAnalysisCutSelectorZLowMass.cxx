
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorZLowMass.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisZBosonCuts.h"

#include "ZeeDTools/ZeeDBags.h"

//------------------------------------------------------
ZeeDAnalysisCutSelectorZLowMass::ZeeDAnalysisCutSelectorZLowMass(TString name ) : ZeeDCutSelector(name)
{
    // Constructor
    Info("ZeeDAnalysisCutSelectorZLowMass::ZeeDAnalysisCutSelectorZLowMass", "Book Cuts");
    this->BookCuts();
}

//------------------------------------------------------
void ZeeDAnalysisCutSelectorZLowMass::BookCuts()
{

    //  ----------------------------------------
    // get handle to fast sim service
    std::string name = std::string("ZeeDAnalysisSvc");

    ServiceHandle<IZeeDAnalysisSvc> ZeeDAnaOptions(name, "ZeeDCalcZ");
    StatusCode sc = ZeeDAnaOptions.retrieve();

    if ( sc.isFailure() ) {
        throw std::runtime_error("ZeeDCalcZ: cannot retrieve " + name + " service");
    }

    Double_t elecPtCut   = ZeeDAnaOptions->Elec_PtCut();
    Double_t elecEtaCut  = ZeeDAnaOptions->Elec_EtaCut();
    //Double_t elecVtxCut  = ZeeDAnaOptions->Elec_DZVtxCut();
    //    Double_t ZPtCut      = ZeeDAnaOptions->Z_PtCut();

    Double_t ZMassCutMin = ZeeDAnaOptions->ZLowMassCutMin();
    Double_t ZMassCutMax = ZeeDAnaOptions->ZLowMassCutMax();

    Double_t EtaCrackMin = ZeeDAnaOptions->EtaCrackEMBEMEC_Min();
    Double_t EtaCrackMax = ZeeDAnaOptions->EtaCrackEMBEMEC_Max();

    Double_t vertexZCut = ZeeDAnaOptions->PriVertexZCut();
    Int_t    minNumTrkCut   = ZeeDAnaOptions->MinNumTrkCut();

    // declare cuts

    // Two electrons are present:
    this->AddCut(new ZeeDCutExistZ("ExistZ") );

    // Pt of both electrons is bigger than elecPtCut
    this->AddCut(new ZeeDCutPtMinBothLepZ("PtMinBothElecZ", elecPtCut));

    // Max |Eta| over two electrons is smaller than elecEtaCut
    this->AddCut(new ZeeDCutEtaMaxBothElecZ("EtaMaxBothElecZ", elecEtaCut));

    // Both Electrons are outside Eta crack defined by EtaCrackMin,EtaCrackMax
    this->AddCut(new ZeeDCutEtaCrackBothElecZ("EtaCrackBothElecZ", EtaCrackMin, EtaCrackMax));

    // Both electrons have track
    //this->AddCut(new ZeeDCutTrackBothElecZ("TrackBothElecZ"));

    // Both electrons in the good region of the calorimeter
    this->AddCut(new ZeeDCutOQMaps("OQMaps"));

    // Z Vertex of the electrons is separated by less than elecVtxCut
    //this->AddCut(new ZeeDCutVertexPresentBothElecZ("VertexPresentBothElecZ", elecVtxCut));

    // Electrons have opposite charge
    //this->AddCut(new ZeeDCutChargeBothElecZ("ChargeBothElecOppositeZ"));

    // Isolation cut for forward electron
    this->AddCut(new ZeeDCutBothCentralElecCaloIso("BothCaloIso98", &ZeeDElectronBags::CaloIso::CaloIso98));
    this->AddCut(new ZeeDCutBothCentralElecCaloIso("BothCaloIso95", &ZeeDElectronBags::CaloIso::CaloIso95));


    //this->AddCut(new ZeeDCutIsEMOneElecZ("IsEMLooseOneElecZ", &ZeeDElectronBags::IsEM::isLoose));
    //this->AddCut(new ZeeDCutIsEMBothElecZ("IsEMLooseBothElecZ", &ZeeDElectronBags::IsEM::isLoose));


    //this->AddCut(new ZeeDCutIsEMOneElecZ("IsEMMediumOneElecZ", &ZeeDElectronBags::IsEM::isMedium));
    //this->AddCut(new ZeeDCutIsEMBothElecZ("IsEMMediumBothElecZ", &ZeeDElectronBags::IsEM::isMedium));


    //this->AddCut(new ZeeDCutIsEMOneElecZ("IsEMTightOneElecZ", &ZeeDElectronBags::IsEM::isTight));
    //this->AddCut(new ZeeDCutIsEMBothElecZ("IsEMTightBothElecZ", &ZeeDElectronBags::IsEM::isTight));

    // Invariant mass of the two electrons falls into the mass window ZMassCutMin,ZMassCutMax
    this->AddCut(new ZeeDCutZMassZ("ZMassZ", ZMassCutMin, ZMassCutMax));

    // Author of both electons is from cluster algorithm
    this->AddCut(new ZeeDCutAuthorBothElecZ("AuthorBothElecZ"));

    // Z position of primary vertex |VtxZ| < 150mm
    this->AddCut(new ZeeDCutPriVtxZ("PriVtxZ", vertexZCut));

    // Cut on number of Tracks at Primary Vertex
    this->AddCut(new ZeeDCutPriVtxNtrack("NTracksAtPrimVtx", minNumTrkCut));

    // Use Trigger Cut
    if ( ZeeDAnaOptions->UseTrigger() ) {
        this->AddCut(new ZeeDCutTriggerWithMatching("TrigE10medOneElecZ",  ZeeDEnum::Trigger::EF_e10_medium));
        this->AddCut(new ZeeDCutTriggerWithMatching("TrigDefaultOneElecZ", ZeeDEnum::Trigger::DefaultSingleElec));
        this->AddCut(new ZeeDCutTrigger("TrigDefaultEvent", ZeeDEnum::Trigger::DefaultSingleElec));
    }

    // Veto on two medium electrons
    this->AddCut(new ZeeDCutTwoGoodElec("MaxTwoGoodElec"));




}
