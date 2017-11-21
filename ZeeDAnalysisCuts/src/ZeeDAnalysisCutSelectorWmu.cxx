// Root libraries
#include "TSystem.h"

// Athena includes
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"

// ZeeD includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorWmu.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisJetCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisWBosonCuts.h"

//------------------------------------------------------
ZeeDAnalysisCutSelectorWmu::ZeeDAnalysisCutSelectorWmu(TString name ) : ZeeDCutSelector(name)
{
   // Constructor
    this->BookCuts();
}

//------------------------------------------------------
ZeeDAnalysisCutSelectorWmu::~ZeeDAnalysisCutSelectorWmu()
{
    // Destructor
}

//------------------------------------------------------
void ZeeDAnalysisCutSelectorWmu::BookCuts()
{

    // Books cuts

    // Get handl to the service
    ServiceHandle<IZeeDAnalysisSvc> ZeeDAnaOptions("ZeeDAnalysisSvc", "");
    StatusCode sc = ZeeDAnaOptions.retrieve();

    if ( sc.isFailure() ) {
        Error("ZeeDCalcElec::Initialize", "Unable to open ZeeDAnalysisSvc");
        gSystem->Exit(EXIT_FAILURE);
    }

   // const Double_t EtCut    = 20.0;  // GeV
    const Double_t etMissCut    = 25.0;  // GeV
    const Double_t etaCrackMin  = 1.37;
    const Double_t etaCrackMax  = 1.52;
    const Double_t muEtaMax   = 2.5;
    const Double_t vertexZCut   = 300.0; // mm
    const Int_t    minNumTrkCut = 3;    //
    const Double_t massTransvCut= 40;
    const Double_t ptCut        = 20;   //GeV
    //const Double_t isoCut       = 0.05;
    const Double_t d0Cut        = 0.1;    //mm

	this->AddCut(new ZeeDCutLepPtMinW("ptMu", ptCut));
    this->AddCut(new ZeeDCutLepPtMinW("ptMuQCD", 10.));
    // Cut on existence and quality cut of vertex
    this->AddCut(new ZeeDCutPriVtxNtrack("NTracksAtPrimVtx", minNumTrkCut));

    // Cut on primary vertex Z position
    this->AddCut(new ZeeDCutPriVtxZ("PriVtxZ", vertexZCut));

    // Cut on mass transverse of W    
    this->AddCut(new ZeeDCutMassTransvW("MtWmu", massTransvCut));
    this->AddCut(new ZeeDCutLepTrackEtaWSim("EtaSim"));

    // Cut on LAr veto
    this->AddCut(new ZeeDCutLArEventVeto("LAr"));
	this->AddCut(new ZeeDCutCombinedMuon("comb"));
	this->AddCut(new ZeeDCutCombinedMuon("notComb", kFALSE));
    this->AddCut(new ZeeDCutMuonHasIDTrack("hasID"));
    // Cut on muon trigger
    this->AddCut(new ZeeDCutTrigger("EFmu10", ZeeDEnum::Trigger::EF_mu10));
    this->AddCut(new ZeeDCutTrigLepW("MuonTrig", ZeeDEnum::Trigger::EF_mu10));
    //this->AddCut(new ZeeDCutMediumID("MuonMedium"));
//    this->AddCut(new ZeeDCutMuonMediumID("NotMuonMedium", kFALSE));
    this->AddCut(new ZeeDCutLepIdentW("MuonLoose", &ZeeDLeptonBags::IsEM::isLoose));
    this->AddCut(new ZeeDCutLepIdentW("MuonMedium", &ZeeDLeptonBags::IsEM::isMedium));
	this->AddCut(new ZeeDCutLepIdentW("notLoose", &ZeeDLeptonBags::IsEM::isLoose, kFALSE));
	this->AddCut(new ZeeDCutLepIdentW("notMediumPP", &ZeeDLeptonBags::IsEM::isMedium, kFALSE));
	//Cut on boson charge 
    this->AddCut(new ZeeDCutLepChargePosW("pos"));
    this->AddCut(new ZeeDCutLepChargePosW("neg", kFALSE));
     
    // Cut on muon track isolation
    this->AddCut(new ZeeDCutMuonTrackIso ("trIso", 0.8));
	this->AddCut(new ZeeDCutMuonTrackIso ("nTrIso", 0.8, kFALSE));

    // Cut on muon impact parameter
    this->AddCut(new ZeeDCutMuonD0W ("d0", d0Cut));

   // Cut on muon eta
    this->AddCut(new ZeeDCutLepTrackEtaW  ("eta", muEtaMax));
    //this->AddCut(new ZeeDCutCrackLepTrackEtaW("etaCrack", etaCrackMin, etaCrackMax));
   // Cut on minimal EtMiss in event
    //this->AddCut(new ZeeDCutEventMinMissEt ("WmuEtMiss", etMissCut));
	this->AddCut(new ZeeDCutEventMinMissEt ("WmuEtMiss", etMissCut));

    if (ZeeDAnaOptions->IsMC() && ZeeDAnaOptions->FillGenInfo()) {
        this->AddCut(new ZeeDCutEtaMaxBornElecW("GenEta", muEtaMax));
        this->AddCut(new ZeeDCutPtBornElectronW("GenPt", ptCut));
        this->AddCut(new ZeeDCutMassTransvBornW("GenMt", massTransvCut));
        this->AddCut(new ZeeDCutEtMissBornW("GenEt", etMissCut));
 
        this->AddCut(new ZeeDCutPtBornElectronW("GenPtNEW", 25.));
        this->AddCut(new ZeeDCutMassTransvBornW("GenMtNEW", 50.));
        this->AddCut(new ZeeDCutEtMissBornW("GenEtNEW", 25.));
       
       }

}
