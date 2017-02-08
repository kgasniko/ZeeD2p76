// Root libraries
#include "TSystem.h"

// Athena includes
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"

// ZeeD includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorW.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisJetCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisWBosonCuts.h"

//------------------------------------------------------
ZeeDAnalysisCutSelectorW::ZeeDAnalysisCutSelectorW(TString name ) : ZeeDCutSelector(name)
{
    // Constructor
    this->BookCuts();
}

//------------------------------------------------------
ZeeDAnalysisCutSelectorW::~ZeeDAnalysisCutSelectorW()
{
    // Destructor
}

//------------------------------------------------------
void ZeeDAnalysisCutSelectorW::BookCuts()
{

    // Books cuts

    // Get handl to the service
    ServiceHandle<IZeeDAnalysisSvc> ZeeDAnaOptions("ZeeDAnalysisSvc", "");
    StatusCode sc = ZeeDAnaOptions.retrieve();

    if ( sc.isFailure() ) {
        Error("ZeeDCalcElec::Initialize", "Unable to open ZeeDAnalysisSvc");
        gSystem->Exit(EXIT_FAILURE);
    }

    const Double_t elecEtCut    = 20.0;  // GeV
    const Double_t etMissCut    = 25.0;  // GeV
    const Double_t etaCrackMin  = 1.37;
    const Double_t etaCrackMax  = 1.52;
    const Double_t elecEtaMax   = 2.47;
    const Double_t vertexZCut   = 300.0; // mm
    const Int_t    minNumTrkCut = 3;    //
    const Double_t massTransvCut   = 40;

    // Declare cuts


    // Et of  electron is larger than elecEtCut
    this->AddCut(new ZeeDCutElecClustEtMinW("ElecClustEtMinW", elecEtCut));
    this->AddCut(new ZeeDCutElecClustEtMinW("ElecClustEtMinWQCD", 10)); 
	this->AddCut(new ZeeDCutEtCone20overEtCorW("EtCone20", 0.1));
    this->AddCut(new ZeeDCutLepTrackEtaWSim("EtaSim"));

    //trigger cuts 
    this->AddCut(new ZeeDCutTrigger ("qcdTrig", ZeeDEnum::Trigger::EF_e7_loose1));
   	this->AddCut(new ZeeDCutTrigLepW("LepTrig", ZeeDEnum::Trigger::EF_e15_loose1)); 
    
    // Minium missing et requirement
    //this->AddCut(new ZeeDCutEventMinMissEt("EtMissMinW", etMissCut));
    this->AddCut(new ZeeDCutCorRecoilEtMissW ("EtMissMinW", etMissCut));

    // Cut away transition EMEC-EMB
    this->AddCut(new ZeeDCutEtaCrackElecW("EtaCrackElecW", etaCrackMin, etaCrackMax));

    // Cut on maximal eta of the electron
    this->AddCut(new ZeeDCutElecClustEtaMaxW("ElecClustEtaMaxW", elecEtaMax));

    // Cut on existence and quality cut of vertex
    this->AddCut(new ZeeDCutPriVtxNtrack("NTracksAtPrimVtx", minNumTrkCut));

    // Cut on primary vertex Z position
    this->AddCut(new ZeeDCutPriVtxZ("PriVtxZ", vertexZCut));
    
    
    this->AddCut(new ZeeDCutNumberGoodW("oneGoodW", 1));
    this->AddCut(new ZeeDCutNumberGoodWmu("noGoodWmu", 0)); 

    // Cuts on electrons identification
    this->AddCut(new ZeeDCutLepIdentW("ElMediumPP", &ZeeDLeptonBags::IsEM::isMediumPP));
    this->AddCut(new ZeeDCutLepIdentW("ElLoosePP", &ZeeDLeptonBags::IsEM::isLoosePP));
	this->AddCut(new ZeeDCutLepIdentW("nElLoosePP", &ZeeDLeptonBags::IsEM::isLoosePP, kFALSE));
    this->AddCut(new ZeeDCutLepIdentW("nElMediumPP", &ZeeDLeptonBags::IsEM::isMediumPP, kFALSE));
    this->AddCut(new ZeeDCutLepIdentW("nElTightPP", &ZeeDLeptonBags::IsEM::isTightPP, kFALSE));
    this->AddCut(new ZeeDCutLepIdentW("ElTightPP", &ZeeDLeptonBags::IsEM::isTightPP));
    
    // Cut on transverse mass of W
    this->AddCut(new ZeeDCutMassTransvW("MassTransvW", massTransvCut));

    // Cut on LAr veto
    this->AddCut(new ZeeDCutLArEventVeto("LAr"));
    
    // Cuts on charge of W
    this->AddCut(new ZeeDCutLepChargePosW("WPlus", true));
    this->AddCut(new ZeeDCutLepChargePosW("WMinus", false));

    // Electron in the good region of the calorimeter
    this->AddCut(new ZeeDCutOQMapsW("OQMaps"));
    

    // Cuts for generator level information
    if (ZeeDAnaOptions->IsMC() && ZeeDAnaOptions->FillGenInfo()) {
        this->AddCut(new ZeeDCutEtaMaxBornElecW("GenEta", elecEtaMax));
        this->AddCut(new ZeeDCutPtBornElectronW("GenPt", elecEtCut));
        this->AddCut(new ZeeDCutMassTransvBornW("GenMt", massTransvCut));
        this->AddCut(new ZeeDCutCrackBornElecW("GenCrack"));
        this->AddCut(new ZeeDCutEtMissBornW("GenEt", etMissCut));
/*      this->AddCut(new ZeeDCutEtaMaxBornElecW("GenEtaNEW", 2.5));
        this->AddCut(new ZeeDCutPtBornElectronW("GenPtNEW", 25));
        this->AddCut(new ZeeDCutMassTransvBornW("GenMtNEW", 50));
        this->AddCut(new ZeeDCutEtMissBornW("GenEtNEW", 25));
         
        this->AddCut(new ZeeDCutElecClustEtMinW("ElecClustEtMinWNEW", 30));
        this->AddCut(new ZeeDCutElecClustEtaMaxW("ElecClustEtaMaxWNEW", 2.5));
        this->AddCut(new ZeeDCutMassTransvW("MassTransvWNEW", 50));*/
        }
   



    
}
