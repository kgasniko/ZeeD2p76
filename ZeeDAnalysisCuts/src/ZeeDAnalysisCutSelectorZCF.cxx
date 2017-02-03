// Root libraries
#include "TSystem.h"

// Athena includes
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"

// ZeeD includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorZCF.h"
//#include "ZeeDAnalysisCuts/ZeeDAnalysisCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisZBosonCuts.h"

#include "ZeeDEvent/ZeeDEvent.h"

//------------------------------------------------------
ZeeDAnalysisCutSelectorZCF::ZeeDAnalysisCutSelectorZCF(TString name) :
        ZeeDCutSelector(name) {
    // Constructor
    //this->BookCuts();
}

//------------------------------------------------------
ZeeDAnalysisCutSelectorZCF::~ZeeDAnalysisCutSelectorZCF() {
    // Destructor
}

//------------------------------------------------------
void ZeeDAnalysisCutSelectorZCF::BookCuts() {

    // Books cuts

    // Get handl to the service
    ServiceHandle < IZeeDAnalysisSvc > ZeeDAnaOptions("ZeeDAnalysisSvc", "");
    StatusCode sc = ZeeDAnaOptions.retrieve();

    if (sc.isFailure()) {
        Error("ZeeDCalcElec::Initialize", "Unable to open ZeeDAnalysisSvc");
        gSystem->Exit(EXIT_FAILURE);
    }

    Double_t elecPtCut = ZeeDAnaOptions->Elec_PtCut();
    Double_t elecCentralPtCut = ZeeDAnaOptions->Elec_CentralPtMinCut();
    Double_t elecForwardPtCut = ZeeDAnaOptions->Elec_ForwardPtMinCut();

    Double_t elecEtaCntCut = ZeeDAnaOptions->Elec_EtaCut();
    Double_t elecEtaFwdMin = ZeeDAnaOptions->Elec_EtaFwdMin();
    Double_t elecEtaFwdMax = ZeeDAnaOptions->Elec_EtaFwdMax();

    Double_t ZMassCutMin = ZeeDAnaOptions->ZMassCutLooseMin();
    Double_t ZMassCutMax = ZeeDAnaOptions->ZMassCutLooseMax();

    Double_t EtaCrackMin = ZeeDAnaOptions->EtaCrackEMBEMEC_Min();
    Double_t EtaCrackMax = ZeeDAnaOptions->EtaCrackEMBEMEC_Max();

    Double_t EtaFwdCrackMin = ZeeDAnaOptions->EtaCrackEMECFCAL_Min();
    Double_t EtaFwdCrackMax = ZeeDAnaOptions->EtaCrackEMECFCAL_Max();

    Int_t minNumTrkCut = ZeeDAnaOptions->MinNumTrkCut();

    // Declare cuts

    // Two electrons with Mee>30 are present:
    this->AddCut(new ZeeDCutExistZ("ExistZ"));

    //2012 isolation
    this->AddCut(new ZeeDCutPtCone20overEt("PtCone20overEt", 0.1));   
  //  this->AddCut(new ZeeDCutPtCone30overEt("PtCone30overEt", 0.15));     

    // Pt of both electrons is bigger than elecPtCut
    this->AddCut(new ZeeDCutPtMinCentralLepZ("PtMinCentralElecZ", elecCentralPtCut));
    this->AddCut(new ZeeDCutPtMinForwardLepZ("PtMinForwardElecZ", elecForwardPtCut));

    // |Eta| of one electron in forwart region, anothe in central
    this->AddCut(new ZeeDCutEtaCFBothElecZ("EtaCFBothElecZ", elecEtaFwdMin, elecEtaFwdMax, elecEtaCntCut));

    // Both Electrons are outside Eta crack defined by EtaCrackMin,EtaCrackMax
    this->AddCut(new ZeeDCutEtaCrackBothElecZ("EtaCrackBothElecZ", EtaCrackMin, EtaCrackMax));
    if(ZeeDAnaOptions->ExtraCrackCut()) {
        this->AddCut(new ZeeDCutEtaCrackBothElecZ("EtaCrack2BothElecZ", ZeeDAnaOptions->ExtraCrackCutMin(), ZeeDAnaOptions->ExtraCrackCutMax()));
    }
 
    // Both Electrons are outside forward Eta crack defined by EtaFwdCrackMin,EtaFwdCrackMax
    this->AddCut(new ZeeDCutEtaCrackBothElecZ("EtaFwdCrackBothElecZ", EtaFwdCrackMin, EtaFwdCrackMax));

    // Both electrons in the good region of the calorimeter
    this->AddCut(new ZeeDCutOQMaps("OQMaps"));

    // Z Vertex of the electrons is separated by less than elecVtxCut
    //this->AddCut(new ZeeDCutVertexPresentBothElecZ("VertexPresentBothElecZ", elecVtxCut));

    // Invariant mass of the two electrons falls into the mass window ZMassCutMin,ZMassCutMax
    this->AddCut(new ZeeDCutZMassZ("ZMassZ", ZMassCutMin, ZMassCutMax));

    // New Forward ID cuts
    //this->AddCut(new ZeeDCutIsEMCentralForwardElecZ("IsEMNewFwdLooseElecZ",  &ZeeDElectronBags::IsEM::isNewFwdLoose, ZeeDEnum::Direction::Forward));
    this->AddCut(new ZeeDCutIsEMCentralForwardLepZ("IsEMNewFwdTightElecZ",  &ZeeDLeptonBags::IsEM::isNewFwdTight, ZeeDEnum::Direction::Forward));

    // One electron IsEM central Tight Cut
    this->AddCut(new ZeeDCutIsEMCentralForwardLepZ("IsEMTightPPCentralElecZ", &ZeeDLeptonBags::IsEM::isTightPP, ZeeDEnum::Direction::Central));

    // One electron IsEM central 
    this->AddCut(new ZeeDCutIsEMCentralForwardLepZ("IsEMMediumCentralElecZ", &ZeeDLeptonBags::IsEM::isMediumPP, ZeeDEnum::Direction::Central));

    // One electron has track
    this->AddCut(new ZeeDCutTrackCentralLepZ("ZeeDCutTrackCentralElecZ"));

    //this->AddCut(new ZeeDCutCentralForwardLooseGoodElec("MaxTwoGoodElecLoose"));
    this->AddCut(new ZeeDCutCentralForwardTightGoodElec("MaxTwoGoodElecTight"));

    // Author of both electons, one for cluster algorithm another for forward
    this->AddCut(new ZeeDCutAuthorCFElecZ("AuthorCFElecZ"));

    // Z position of primary vertex |VtxZ| < 150mm
    //this->AddCut(new ZeeDCutPriVtxZ("PriVtxZ", vertexZCut));

    // Cut on number of Tracks at Primary Vertex
    this->AddCut(new ZeeDCutPriVtxNtrack("NTracksAtPrimVtx", minNumTrkCut));

    this->AddCut(new ZeeDCutLArEventVeto("ZeeDCutLArEventVeto"));
    
    // Use Trigger Cut
    if (ZeeDAnaOptions->UseTrigger()) {
      this->AddCut(new ZeeDCutTwoTriggersCF("EF_e24vhi_medium1_OR_EF_e60_medium1"));
    }
    
    // Use Trigger Cut and Trigger Matching
    if (ZeeDAnaOptions->UseTrigger() && ZeeDAnaOptions->DoTriggerMatching()) {
      this->AddCut(new ZeeDCutTriggerWithMatching2012CF("Match_EF_e24vhi_medium1_OR_EF_e60_medium1"));
    }


    if (ZeeDAnaOptions->FillGenInfo()) {
        this->AddCut(new ZeeDCutExistBornBoson("ExistBornBoson"));
        this->AddCut(new ZeeDCutExistBornElectrons("ExistTwoBornElectrons", 2));
        this->AddCut(new ZeeDCutPtMinBothBornElecZ("PtMinBothBornElecZ", elecPtCut));
        this->AddCut(new ZeeDCutMassBornBoson("MassBornBoson", ZMassCutMin, ZMassCutMax));
		
	this->AddCut(new ZeeDCutEtaCrackBothBornElecZ("EtaCrackCentralBornElecZ", EtaCrackMin, EtaCrackMax));
	this->AddCut(new ZeeDCutEtaCrackBothBornElecZ("EtaCrackForwardBornElecZ", EtaFwdCrackMin, EtaFwdCrackMax));
	
	this->AddCut(new ZeeDCutEtaCFBothBornElecZ("EtaCFBothBornElecZ", elecEtaFwdMin, elecEtaFwdMax, elecEtaFwdMin));	
	this->AddCut(new ZeeDCutEtaCFBothBornElecZ("EtaExtCFBothBornElecZ", elecEtaFwdMin, elecEtaFwdMax, elecEtaCntCut));	
    }

}
