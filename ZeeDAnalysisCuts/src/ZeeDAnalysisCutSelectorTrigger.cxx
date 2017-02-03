
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorTrigger.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisZBosonCuts.h"

#include "ZeeDTools/ZeeDBags.h"

//------------------------------------------------------
void ZeeDAnalysisCutSelectorTrigger::BookCuts()
{

    //  ----------------------------------------
    // get handle to fast sim service
    std::string name = std::string("ZeeDAnalysisSvc");

    ServiceHandle<IZeeDAnalysisSvc> ZeeDAnaOptions(name, "ZeeDCalcZ");
    StatusCode sc = ZeeDAnaOptions.retrieve();

    if ( sc.isFailure() ) {
        throw std::runtime_error("ZeeDCalcZ: cannot retrieve " + name + " service");
    }

   // Double_t elecPtCut   = ZeeDAnaOptions->Elec_PtCut();
    Double_t elecEtaCut  = ZeeDAnaOptions->Elec_EtaCut();

    Double_t ZMassCutMin = ZeeDAnaOptions->ZMassCutLooseMin();
    Double_t ZMassCutMax = ZeeDAnaOptions->ZMassCutLooseMax();

    Double_t EtaCrackMin = ZeeDAnaOptions->EtaCrackEMBEMEC_Min();
    Double_t EtaCrackMax = ZeeDAnaOptions->EtaCrackEMBEMEC_Max();

    Int_t    minNumTrkCut   = ZeeDAnaOptions->MinNumTrkCut();

    // declare cuts

    // Two electrons with Mee>30 are present:
    this->AddCut(new ZeeDCutExistZ("ExistZ") );

    // Max |Eta| over two electrons is smaller than elecEtaCut
    this->AddCut(new ZeeDCutEtaMaxBothElecZ("EtaMaxBothElecZ", elecEtaCut));

    // Both Electrons are outside Eta crack defined by EtaCrackMin,EtaCrackMax
    this->AddCut(new ZeeDCutEtaCrackBothElecZ("EtaCrackBothElecZ", EtaCrackMin, EtaCrackMax));

    // Both electrons have track
    this->AddCut(new ZeeDCutTrackBothLepZ("TrackBothElecZ"));

    // Isolation cut for forward electron
    this->AddCut(new ZeeDCutBothCentralElecCaloIso("BothCaloIso98", &ZeeDElectronBags::CaloIso::CaloIso98));
    this->AddCut(new ZeeDCutBothCentralElecCaloIso("BothCaloIso95", &ZeeDElectronBags::CaloIso::CaloIso95));

    // Both electrons in the good region of the calorimeter
    this->AddCut(new ZeeDCutOQMaps("OQMaps"));

    // Electrons have opposite charge
    this->AddCut(new ZeeDCutChargeBothLepZ("ChargeBothElecOppositeZ"));

    this->AddCut(new ZeeDCutIsEMBothLepZ("IsEMMediumBothElecZ", &ZeeDLeptonBags::IsEM::isMedium));

    this->AddCut(new ZeeDCutIsEMBothLepZ("IsEMTightBothElecZ", &ZeeDLeptonBags::IsEM::isTight));

    // Electrons have Same charge
    this->AddCut(new ZeeDCutSameChargeBothLepZ("ChargeBothElecSameZ"));

    // Invariant mass of the two electrons falls into the mass window ZMassCutMin,ZMassCutMax
    this->AddCut(new ZeeDCutZMassZ("ZMassZ", ZMassCutMin, ZMassCutMax));

    // Author of both electons is from cluster algorithm
    this->AddCut(new ZeeDCutAuthorBothElecZ("AuthorBothElecZ"));

    // Number of Tracks at Primary Vertex is > minNTrk 
    this->AddCut(new ZeeDCutPriVtxNtrack("NTracksAtPrimVtx", minNumTrkCut));

    if ( ZeeDAnaOptions->FillGenInfo() ) {
        // FIXME!
        // only a very little number of events survive with this cut ->
        // disabled for now
        //this->AddCut(new ZeeDCutTruthMatchBothElecZ());
    }

    if ( ZeeDAnaOptions->FillGenInfo() ) {

        this->AddCut(new ZeeDCutExistBornBoson("ExistBornBoson") );

        this->AddCut(new ZeeDCutExistBornElectrons("ExistTwoBornElectrons", 2) );

        this->AddCut(new ZeeDCutEtaMaxBothBornElecZ("EtaMaxBothBornElecZ", elecEtaCut));

        this->AddCut(new ZeeDCutMassBornBoson("MassBornBoson", ZMassCutMin, ZMassCutMax));

    }

}
