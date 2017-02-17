#include "ZeeDHistManager/ZeeDAnalysisCutHistManagerWmu.h"

// std includes
#include <iostream>

// ZeeD analysis includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorWmu.h"
#include "ZeeDHistManager/ZeeDControlHistManagerW.h"


// Histogram managers
#include "ZeeDAnalysisCuts/ZeeDCalcWeightCutDepW.h"
#include "ZeeDHistManager/ZeeDHistManagerQCDBkgW.h"

#include "ZeeDHistManager/ZeeDHistManagerMuon.h"
#include "ZeeDHistManager/ZeeDHistManagerBoson.h"
#include "ZeeDHistManager/ZeeDHistManagerEvent.h"
#include "ZeeDHistManager/ZeeDHistManagerGenInfo.h"
#include "ZeeDHistManager/ZeeDHistManagerEtmiss.h"
#include "ZeeDHistManager/ZeeDHistManagerCut.h"

#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDAnalysisCutHistManagerWmu::ZeeDAnalysisCutHistManagerWmu(TString name) : ZeeDCutHistManager(name)
{
    // Constructor
}

//------------------------------------------------------
ZeeDAnalysisCutHistManagerWmu::~ZeeDAnalysisCutHistManagerWmu()
{
    // Destructor

}

//------------------------------------------------------
void ZeeDAnalysisCutHistManagerWmu::BookCutHistManager()
{
    TString selectionEvent = "NTracksAtPrimVtx+PriVtxZ+LAr";
    //TString selectionMuon = "eta+EFmu10+ptMu+MuonMedium+comb+trIso";
    TString selectionMuon = "ptMu+eta+MuonMedium+MuonTrig+comb+trIso+noGoodW+oneGoodWmu";
    TString selectionBoson = "WmuEtMiss+MtWmu+WmuEtMiss";
    TString selection = selectionEvent+"+"+selectionMuon+"+"+selectionBoson; 
    TString pos       = selection+"+pos";
    TString neg       = selection+"+neg";
    //TString selectionOld = "NTracksAtPrimVtx+PriVtxZ+LAr+eta+EFmu10+MtWmu+WmuEtMissOld+ptMu+MuonMedium";

    TString selUnf     = "MtWmu+WmuEtMiss+ptMu+eta";
    //selection based on generator level cuts
    TString genSel        = "GenEta+GenPt+GenMt+GenEt";
    //TString genSel = "GenMt"; 
    TString selUnfNEW     = "MassTransvWNEW+WmuEtMiss+ptMuNEW+etaNEW";
    //selection based on generator level cuts
    TString genSelNEW        = "GenEta+GenPtNEW+GenMtNEW+GenEtNEW";

    ZeeDCalcWeightCutDepW* cw = new ZeeDCalcWeightCutDepW();
    this->AddCalcWeighter(cw);

    ZeeDDoWeight doWeight;
    doWeight.TrigSFSingle = kTRUE;
    doWeight.IDSFBothMediumPP = kTRUE;	
    doWeight.HadrRecoil = kTRUE;
    doWeight.RecSF = kFALSE;
    
    ZeeDDoWeight doWeightQCD;
    doWeightQCD.TrigSFSingle = kTRUE;
    doWeightQCD.IDSFBothMediumPP = kFALSE;
    doWeightQCD.HadrRecoil = kTRUE;



    this->SetDefaultDoWeight(doWeight);

    ZeeDDoWeight doWeightNone;
    doWeightNone.RecSF = kFALSE;

    ServiceHandle<IZeeDAnalysisSvc>* ZeeDAnaOptions = fSvcHelper.GetAnaSvc();



    // Create/book control plots:
    ZeeDControlHistManagerW* analysisPlots = new ZeeDControlHistManagerW(this->getName() + "/" + "Boson");
    this->AddMaskLoose( selection, analysisPlots, doWeight);

    ZeeDControlHistManagerW* wpos = new ZeeDControlHistManagerW(this->getName()+"/"+"Plus");
    this->AddMaskLoose(pos, wpos, doWeight);

    ZeeDControlHistManagerW* wneg = new ZeeDControlHistManagerW(this->getName()+"/"+"Minus");
    this->AddMaskLoose(neg, wneg, doWeight);

    ZeeDControlHistManagerW* wposSim = new ZeeDControlHistManagerW(this->getName()+"/"+"PlusSim");
    this->AddMaskLoose(pos+"+EtaSim", wposSim, doWeight);

    ZeeDControlHistManagerW* wnegSim = new ZeeDControlHistManagerW(this->getName()+"/"+"MinusSim");
    this->AddMaskLoose(neg+"+EtaSim", wnegSim, doWeight);


    //TString QCD       = "NTracksAtPrimVtx+PriVtxZ+LAr+eta+EFmu10+MtWmu+ptMu+nTrIso";
    TString QCD         = "NTracksAtPrimVtx+PriVtxZ+LAr+eta+EFmu10+MtWmu+ptMu+notComb+hasID";
    TString QCDPos    = QCD+"+pos";
    TString QCDNeg    = QCD+"+neg";

    if ((*ZeeDAnaOptions)->FillBkgHistograms()) {
        //filling histograms for cutflow
        ZeeDHistManagerCut* eventCut = new ZeeDHistManagerCut(this->getName()+"/CutFlow/Event");
        ZeeDHistManagerCut* leptonCut = new ZeeDHistManagerCut(this->getName()+"/CutFlow/Lepton");
        ZeeDHistManagerCut* bosonCut = new ZeeDHistManagerCut(this->getName()+"/CutFlow/Boson");
        this->AddMaskLoose(selectionEvent, eventCut, doWeightNone);
        //this->AddMaskLoose("", eventCut, doWeightNone);
        this->AddMaskLoose(selectionEvent+"+"+selectionMuon, leptonCut, doWeightNone);
        //this->AddMaskLoose(selectionMuon, leptonCut, doWeightNone);
 
        this->AddMaskLoose(selection, bosonCut, doWeightNone);
        ZeeDHistManagerCut* bosonCutPlus = new ZeeDHistManagerCut(this->getName()+"/CutFlow/BosonPlus");
        ZeeDHistManagerCut* bosonCutMinus = new ZeeDHistManagerCut(this->getName()+"/CutFlow/BosonMinus");
        this->AddMaskLoose(pos, bosonCutPlus, doWeightNone);
        this->AddMaskLoose(neg, bosonCutMinus, doWeightNone);


        //QCD EtMiss fit
        // selection 
        //TString QCDEtsel    = "NTracksAtPrimVtx+PriVtxZ+LAr+eta+EFmu10+MtWmu+ptMu+trIso+MuonMedium+notComb";
        TString QCDEtsel    = "NTracksAtPrimVtx+PriVtxZ+LAr+eta+EFmu10+MtWmu+ptMu+MuonMedium+trIso+comb";
        TString QCDEtselPos = QCDEtsel+"+pos";
        TString QCDEtselNeg = QCDEtsel+"+neg";

        ZeeDHistManagerQCDBkgW* QCDEtselPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/etMiss/QCDFit/"+"Boson");
        this->AddMaskLoose(QCDEtsel, QCDEtselPlot);
        ZeeDHistManagerQCDBkgW* QCDEtselPosPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/etMiss/QCDFit/"+"Plus");
        this->AddMaskLoose(QCDEtselPos, QCDEtselPosPlot);
        ZeeDHistManagerQCDBkgW* QCDEtselNegPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/etMiss/QCDFit/"+"Minus");
        this->AddMaskLoose(QCDEtselNeg, QCDEtselNegPlot);

        //QCD Pt fit
        TString QCDFitPt = "NTracksAtPrimVtx+PriVtxZ+LAr+eta+EFmu10+MtWmu+WmuEtMiss+MuonMedium+trIso+notComb";
        ZeeDHistManagerQCDBkgW* QCDPtPlot = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/lepPt/QCDFit/"+"Boson");
        this->AddMaskLoose(QCDFitPt, QCDPtPlot);
        ZeeDHistManagerQCDBkgW* QCDPtPosPlot = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/lepPt/QCDFit/"+"Plus");
        this->AddMaskLoose(QCDFitPt+"+pos", QCDPtPosPlot);
        ZeeDHistManagerQCDBkgW* QCDPtNegPlot = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/lepPt/QCDFit/"+"Minus");
        this->AddMaskLoose(QCDFitPt+"+neg", QCDPtNegPlot);
    }


    if (fSys->isShiftInUse(ZeeDSystematics::eNoShift)) {

        ZeeDHistManagerMuon* muonPlots = new ZeeDHistManagerMuon(this->getName()+"/"+"Muon");
        this->AddMaskLoose(selection, muonPlots);
        muonPlots->SetW();


        ZeeDHistManagerEtmiss* etMissPlots = new ZeeDHistManagerEtmiss(this->getName() + "/" + "EtMiss");
        this->AddMaskLoose( selection, etMissPlots);
        ZeeDHistManagerEtmiss* etMissPlotsP = new ZeeDHistManagerEtmiss(this->getName() + "/" + "EtMissPlus");
        this->AddMaskLoose( selection+"+pos", etMissPlotsP);
        ZeeDHistManagerEtmiss* etMissPlotsM = new ZeeDHistManagerEtmiss(this->getName() + "/" + "EtMissMinus");
        this->AddMaskLoose( selection+"+neg", etMissPlotsM);

        //ZeeDHistManagerEtmiss* etMissPlotsOld = new ZeeDHistManagerEtmiss(this->getName() + "/" + "EtMiss");
        //this->AddMaskLoose( selectionOld, etMissPlotsOld);

        ZeeDHistManagerEvent* eventPlots = new ZeeDHistManagerEvent(this->getName() + "/" + "Event");
        this->AddMaskLoose( selection, eventPlots );

        //QCD Hist Managers 
        if ((*ZeeDAnaOptions)->FillBkgHistograms()) {

            //EtMiss QCD

            //QCD EtMiss template	
            ZeeDHistManagerQCDBkgW* QCDPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/etMiss/QCDTempl/"+"Boson");
            this->AddMaskLoose(QCD, QCDPlot);
            ZeeDHistManagerQCDBkgW* QCDPosPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/etMiss/QCDTempl/"+"Plus");
            this->AddMaskLoose(QCDPos, QCDPosPlot);
            ZeeDHistManagerQCDBkgW* QCDNegPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/etMiss/QCDTempl/"+"Minus");
            this->AddMaskLoose(QCDNeg, QCDNegPlot);

            //Check of Fit
            TString QCDCheckFit = "NTracksAtPrimVtx+PriVtxZ+LAr+eta+EFmu10+MtWmu+ptMu+trIso";
            ZeeDHistManagerQCDBkgW* QCDPlotsCh1 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/etMiss/QCDVar/"+"Plus/Tight");
            this->AddMaskLoose(QCDCheckFit+"+notTight+pos", QCDPlotsCh1, doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsCh2 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/etMiss/QCDVar/"+"Minus/Tight");
            this->AddMaskLoose(QCDCheckFit+"+notTight+neg", QCDPlotsCh2,  doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsCh3 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/etMiss/QCDVar/"+"Boson/Tight");
            this->AddMaskLoose(QCDCheckFit+"+notTight", QCDPlotsCh3, doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsCh4 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/etMiss/QCDVar/"+"Plus/Loose");
            this->AddMaskLoose(QCDCheckFit+"+notLoose+pos", QCDPlotsCh4, doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsCh5 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/etMiss/QCDVar/"+"Minus/Loose");
            this->AddMaskLoose(QCDCheckFit+"+notLoose+neg", QCDPlotsCh5,  doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsCh6 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/etMiss/QCDVar/"+"Boson/Loose");
            this->AddMaskLoose(QCDCheckFit+"+notLoose+pos", QCDPlotsCh6, doWeight);


            //QCD Pt 
            TString QCDSelectionPt = "NTracksAtPrimVtx+PriVtxZ+LAr+eta+EFmu10+MtWmu+WmuEtMiss+nTrIso+notComb";

            TString QCDPtVar = "NTracksAtPrimVtx+PriVtxZ+LAr+eta+EFmu10+MtWmu+WmuEtMiss+nTrIso+notComb";

            //QCD Pt template
            ZeeDHistManagerQCDBkgW* QCDPlotPt = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/lepPt/QCDTempl/"+"Boson");
            this->AddMaskLoose(QCDSelectionPt, QCDPlotPt, doWeight);
            ZeeDHistManagerQCDBkgW* QCDPlotPtPos = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/lepPt/QCDTempl/"+"Plus");
            this->AddMaskLoose(QCDSelectionPt+"+pos", QCDPlotPtPos, doWeight); 
            ZeeDHistManagerQCDBkgW* QCDPlotPtNeg = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/lepPt/QCDTempl/"+"Minus");
            this->AddMaskLoose(QCDSelectionPt+"+neg", QCDPlotPtNeg, doWeight);


            //template variation
            ZeeDHistManagerQCDBkgW* QCDPlotsPt1 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/lepPt/QCDVar/"+"Plus/Tight");
            this->AddMaskLoose(QCDPtVar+"+notTight+pos", QCDPlotsPt1, doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsPt2 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/lepPt/QCDVar/"+"Minus/Tight");
            this->AddMaskLoose(QCDPtVar+"+notTight+neg", QCDPlotsPt2,  doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsPt3 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/lepPt/QCDVar/"+"Boson/Tight");
            this->AddMaskLoose(QCDPtVar+"+notTight", QCDPlotsPt3, doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsPt4 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/lepPt/QCDVar/"+"Plus/Loose");
            this->AddMaskLoose(QCDPtVar+"+notLoose+pos", QCDPlotsPt4, doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsPt5 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/lepPt/QCDVar/"+"Minus/Loose");
            this->AddMaskLoose(QCDPtVar+"+notLoose+neg", QCDPlotsPt5,  doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsPt6 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/lepPt/QCDVar/"+"Boson/Loose");
            this->AddMaskLoose(QCDPtVar+"+notLoose", QCDPlotsPt6, doWeight);




        }
    }
    //Book plots for gen level information
    if ((*ZeeDAnaOptions)->IsMC() && (*ZeeDAnaOptions)->FillGenInfo()){

        ZeeDHistManagerGenInfo * gen = new ZeeDHistManagerGenInfo (this->getName()+"/"+"GenInfo");
        this->AddMaskLoose(genSel, gen, doWeightNone);

        ZeeDHistManagerGenInfo * genPlus = new ZeeDHistManagerGenInfo (this->getName()+"/"+"GenInfoPlus");
        this->AddMaskLoose(genSel+"+WGenPlus", genPlus, doWeightNone);

        ZeeDHistManagerGenInfo * genMinus = new ZeeDHistManagerGenInfo (this->getName()+"/"+"GenInfoMinus");
        this->AddMaskLoose(genSel+"+WGenMinus", genMinus, doWeightNone);

    /*    if (fSys->isShiftInUse(ZeeDSystematics::eNoShift)){
            ZeeDControlHistManagerW * unfMan = new ZeeDControlHistManagerW(this->getName()+"/"+"GenCut");
            this->AddMaskLoose(selUnf, unfMan, doWeightNone);
            ZeeDControlHistManagerW * unfManNEW = new ZeeDControlHistManagerW(this->getName()+"/"+"GenCutNEW");
            this->AddMaskLoose(selUnfNEW, unfManNEW);
            ZeeDHistManagerGenInfo * genNEW = new ZeeDHistManagerGenInfo (this->getName()+"/"+"GenNEW");
            this->AddMaskLoose(genSelNEW, genNEW, doWeightNone);
        }*/
    } 


}
