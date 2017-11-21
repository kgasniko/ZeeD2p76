#include "ZeeDHistManager/ZeeDAnalysisCutHistManagerZmumu.h"

// std includes
#include <iostream>
#include "ZeeDHistManager/ZeeDHistManagerCut.h"

// ZeeD analysis includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorZmumu.h"


// Histogram managers
#include "ZeeDAnalysisCuts/ZeeDCalcWeightCutDepZ.h"

#include "ZeeDHistManager/ZeeDHistManagerMuon.h"
#include "ZeeDHistManager/ZeeDHistManagerBoson.h"
#include "ZeeDHistManager/ZeeDHistManagerEvent.h"
#include "ZeeDHistManager/ZeeDHistManagerEtmiss.h"
#include "ZeeDHistManager/ZeeDHistManagerGenInfo.h"
#include "ZeeDHistManager/ZeeDHistManagerTagAndProbeLepton.h"
#include "ZeeDHistManager/ZeeDHistManagerMCTrigEffLepton.h"

#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDAnalysisCutHistManagerZmumu::ZeeDAnalysisCutHistManagerZmumu(TString name) : ZeeDCutHistManager(name)
{
    // Constructor
}

//------------------------------------------------------
ZeeDAnalysisCutHistManagerZmumu::~ZeeDAnalysisCutHistManagerZmumu()
{
    // Destructor

}

//------------------------------------------------------
void ZeeDAnalysisCutHistManagerZmumu::BookCutHistManager()
{   

    TString selectionEvent = "NTracksAtPrimVtx+ZeeDCutLArEventVeto+PriVtxZ+EF_mu10";
    TString selectionMuon = "ChargeBothLepOppositeZ+PtMinBothMuonZ+EtaMaxBothMuonZ+MuonTrig+IsEMMediumPPBothMuonZ+comb+trIso";
    TString selectionBoson="ZMassZ";
    TString selection=selectionEvent + "+" +selectionMuon+"+"+selectionBoson;
    //TString selection = "IsEMMediumPPBothMuonZ+PtMinBothMuonZ+EtaMaxBothMuonZ+ChargeBothLepOppositeZ+ZMassZ+NTracksAtPrimVtx+ZeeDCutLArEventVeto+EF_mu10+MuonTrig";
	TString genSel        = "GenEta+GenPt+GenMt";
//	TString genSel        = "GenMt";
	TString genSelNew     = "GenEta+GenPtNEW+GenMt";
   	TString jpsiSelection =  "PtMinBothMuonJpsi+EtaMaxBothMuonZ+ChargeBothLepOppositeZ+Jpsi+NTracksAtPrimVtx+EventEtMiss+ZeeDCutLArEventVeto";

 
	//selection = "ZMassZ+NTracksAtPrimVtx+EventEtMiss"; 
   
    ZeeDCalcWeightCutDepZ* cw = new ZeeDCalcWeightCutDepZ();
    this->AddCalcWeighter(cw);

    ZeeDDoWeight doWeight;
    doWeight.IDSFBothMediumPP = kTRUE;
	doWeight.TrigSFDiMediumPP = kTRUE;


   	
	ZeeDDoWeight doWeightTag;
    doWeightTag.IDSFBothMediumPP = kTRUE;
	doWeightTag.TrigSFDiMediumPP = kFALSE;

	ZeeDDoWeight doWeightNone;
    doWeightNone.IDSFBothMediumPP = kFALSE;
	doWeightNone.TrigSFDiMediumPP = kFALSE;


    this->SetDefaultDoWeight(doWeight);

    //this->SetDefaultDoWeight(doWeightNone);
     ZeeDHistManagerCut* eventCut = new ZeeDHistManagerCut(this->getName()+"/CutFlow/Event");
    ZeeDHistManagerCut* leptonCut = new ZeeDHistManagerCut(this->getName()+"/CutFlow/Lepton");
    ZeeDHistManagerCut* bosonCut = new ZeeDHistManagerCut(this->getName()+"/CutFlow/Boson");
    this->AddMaskLoose(selectionEvent, eventCut, doWeightNone);
    this->AddMaskLoose(selectionEvent+"+"+selectionMuon, leptonCut);
    this->AddMaskLoose(selection, bosonCut);




    ServiceHandle<IZeeDAnalysisSvc>* ZeeDAnaOptions = fSvcHelper.GetAnaSvc();
    
	
	if ((*ZeeDAnaOptions)->IsMC() && (*ZeeDAnaOptions)->FillGenInfo()){
        ZeeDHistManagerGenInfo * gen = new ZeeDHistManagerGenInfo (this->getName()+"/"+"GenInfo");
        this->AddMaskLoose(genSel, gen, doWeightNone);
        
        ZeeDHistManagerGenInfo * genNew = new ZeeDHistManagerGenInfo (this->getName()+"/"+"GenInfo13");
        this->AddMaskLoose(genSelNew, genNew, doWeightNone);


    }
    this->AddMaskLoose("PtMinBothMuonZ+EtaMaxBothMuonZ+ChargeBothLepOppositeZ+ZMassZ+NTracksAtPrimVtx+EventEtMiss+ZeeDCutLArEventVeto+EF_mu10+comb", new ZeeDHistManagerTagAndProbeLepton(this->getName() + "/" + "ProbeLeptons"), doWeightTag);

    this->AddMaskLoose("IsEMMediumPPBothMuonZ+PtMinBothMuonZ+EtaMaxBothMuonZ+ChargeBothLepOppositeZ+ZMassZ+NTracksAtPrimVtx+EventEtMiss+ZeeDCutLArEventVeto+EF_mu10+comb", new ZeeDHistManagerMCTrigEffLepton(this->getName()+"/"+"MCEffTrig"), doWeightTag);

   this->AddMaskLoose(jpsiSelection+"+EF_mu10", new ZeeDHistManagerMCTrigEffLepton(this->getName()+"/"+"MCEffTrigJpsi"), doWeightTag);
	this->AddMaskLoose(jpsiSelection+"+IsEMMediumPPBothMuonZ", new ZeeDHistManagerTagAndProbeLepton(this->getName()+"/"+"ProbeLeptonsJpsi"), doWeightNone);	

    ZeeDHistManagerBoson* bosonHist = new ZeeDHistManagerBoson (this->getName() + "/"+"Boson");
    this->AddMaskLoose(selection, bosonHist);
    ZeeDHistManagerEvent* eventHist = new ZeeDHistManagerEvent (this->getName() + "/" + "Event");   
    this->AddMaskLoose(selection, eventHist);
    ZeeDHistManagerEvent* eventHist2 = new ZeeDHistManagerEvent (this->getName() + "/" + "Event2");   
    this->AddMaskLoose("", eventHist2);
    ZeeDHistManagerMuon* muonHist = new ZeeDHistManagerMuon (this->getName()+"/"+"Muon");
    this->AddMaskLoose(selection, muonHist);   
    ZeeDHistManagerEtmiss* etMissHist = new ZeeDHistManagerEtmiss(this->getName()+"/"+"EtMiss");
    this->AddMaskLoose(selection, etMissHist);
}
