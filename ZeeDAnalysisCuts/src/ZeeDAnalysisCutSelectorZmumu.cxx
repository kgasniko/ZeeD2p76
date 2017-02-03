
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorZmumu.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisZBosonCuts.h"

#include "ZeeDTools/ZeeDBags.h"


void ZeeDAnalysisCutSelectorZmumu::BookCuts()
{

    //  ----------------------------------------
    // get handle to fast sim service
    std::string name = std::string("ZeeDAnalysisSvc");

    ServiceHandle<IZeeDAnalysisSvc> ZeeDAnaOptions(name, "ZeeDCalcZ");
    StatusCode sc = ZeeDAnaOptions.retrieve();

    if ( sc.isFailure() ) {
        throw std::runtime_error("ZeeDCalcZ: cannot retrieve " + name + " service");
    }

    //Double_t elecPtCut   = ZeeDAnaOptions->Elec_PtCut();
    //Double_t elecEtaCut  = ZeeDAnaOptions->Elec_EtaCut();
    //Double_t elecVtxCut  = ZeeDAnaOptions->Elec_DZVtxCut();

    //Double_t ZMassCutMin = ZeeDAnaOptions->ZMassCutLooseMin();
    //Double_t ZMassCutMax = ZeeDAnaOptions->ZMassCutLooseMax();

    //Double_t EtaCrackMin = ZeeDAnaOptions->EtaCrackEMBEMEC_Min();
    //Double_t EtaCrackMax = ZeeDAnaOptions->EtaCrackEMBEMEC_Max();

    //Double_t vertexZCut = ZeeDAnaOptions->PriVertexZCut();

    Int_t    minNumTrkCut  = ZeeDAnaOptions->MinNumTrkCut();

    //Bool_t   bookExtraCuts = ZeeDAnaOptions->AnalysisCutSelectorBookExtraCuts();


    //********************************************
    //  BASELINE SELECTION
    //

    // Two electrons with Mee>30 are present:
    //this->AddCut(new ZeeDCutExistZ("ExistZ") );

    // Author of both electons is from cluster algorithm
    //this->AddCut(new ZeeDCutAuthorBothElecZ("AuthorBothElecZ"));

    // Pt of both electrons is bigger than elecPtCut
     this->AddCut(new ZeeDCutAsymmetricalLepPt("PtMinBothMuonZ", 20., 20.));
     this->AddCut(new ZeeDCutIsEMBothLepZ("IsEMMediumPPBothMuonZ", &ZeeDLeptonBags::IsEM::isMedium));
     this->AddCut(new ZeeDCutEtaMaxBothLepZ("EtaMaxBothMuonZ", 2.47));
  	 this->AddCut(new ZeeDCutTrigger("EF_mu10", ZeeDEnum::Trigger::EF_mu10));
     this->AddCut(new ZeeDCutTriggerWithMatching("MuonTrig", ZeeDEnum::Trigger::EF_mu10));	
     this->AddCut(new ZeeDCutOQMaps("OQMaps"));

    // Electrons have opposite charge
    this->AddCut(new ZeeDCutChargeBothLepZ("ChargeBothLepOppositeZ"));

    // Invariant mass of the two electrons falls into the mass window ZMassCutMin,ZMassCutMax
    this->AddCut(new ZeeDCutZMassZ("ZMassZ", 66, 116));
	this->AddCut(new ZeeDCutZMassZ("Jpsi", 10, 50));
	this->AddCut(new ZeeDCutAsymmetricalLepPt("PtMinBothMuonJpsi", 15., 15.));

    // Cut on number of Tracks at Primary Vertex
    this->AddCut(new ZeeDCutPriVtxNtrack("NTracksAtPrimVtx", minNumTrkCut));

    // Event missing Et
    this->AddCut(new ZeeDCutEventMaxMissEt("EventEtMiss", 25));

    // LAr hole veto:
    this->AddCut(new ZeeDCutLArEventVeto("ZeeDCutLArEventVeto"));
    if (ZeeDAnaOptions->IsMC() && ZeeDAnaOptions->FillGenInfo()) {
        this->AddCut(new ZeeDCutEtaMaxBothBornElecZ("GenEta", 2.47));
        this->AddCut(new ZeeDCutPtMinBothBornElecZ("GenPt", 20.));
        this->AddCut(new ZeeDCutMassBornBoson("GenMt", 66, 116));
        this->AddCut(new ZeeDCutEtaMaxBothBornElecZ("GenEtaNEW", 2.5));
        this->AddCut(new ZeeDCutPtMinBothBornElecZ("GenPtNEW", 25.));
         
        }
 
    
    //**************************************************************************
    //
    // Additional cuts for extra studies:
    //
    //*************************************************************************
    /*if ( bookExtraCuts) {
    // Both electrons have track
        this->AddCut(new ZeeDCutTrackBothElecZ("TrackBothElecZ"));
     
*/
    /*
    }
    */

}
