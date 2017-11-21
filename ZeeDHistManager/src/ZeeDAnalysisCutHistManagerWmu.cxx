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
    //TString selectionEvent="PriVtxZ+LAr";
    //TString selectionMuon = "eta+EFmu10+ptMu+MuonMedium+comb+trIso";
    TString selectionMuonBase2="eta+MuonMedium+MuonTrig";
    TString selectionMuonBase= "ptMu+"+selectionMuonBase2;
    TString selectionMuonIso = "comb+trIso";
    TString selectionMuon = selectionMuonBase+"+"+selectionMuonIso;
    TString selectionBoson = "WmuEtMiss+MtWmu";
    TString selectionBosonBase="MtWmu";
    TString selection = selectionEvent+"+"+selectionMuon+"+"+selectionBoson; 
    TString pos       = selection+"+pos";
    TString neg       = selection+"+neg";

    //selection based on generator level cuts
    TString genSel        = "GenEta+GenPt+GenMt+GenEt";
    //TString genSel = "GenMt"; 
    TString genSelNew        = "GenEta+GenPtNEW+GenMtNEW+GenEtNEW";

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

    ZeeDDoWeight doWeightGen;
    doWeightGen.HadrRecoil=kTRUE;
    doWeightGen.TrigSFSingle=kFALSE;
    doWeightGen.IDSFBothMediumPP = kFALSE;
    doWeightGen.RecSF = kFALSE;



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

    if ((*ZeeDAnaOptions)->IsMC() && (*ZeeDAnaOptions)->EvaluateSystematics()){

        ZeeDControlHistManagerW* wposSim = new ZeeDControlHistManagerW(this->getName()+"/"+"PlusSim");
        this->AddMaskLoose(pos+"+EtaSim", wposSim, doWeight);

        ZeeDControlHistManagerW* wnegSim = new ZeeDControlHistManagerW(this->getName()+"/"+"MinusSim");
        this->AddMaskLoose(neg+"+EtaSim", wnegSim, doWeight);
    }

    if ((*ZeeDAnaOptions)->FillBkgHistograms()) {
        //filling histograms for cutflow
        ZeeDHistManagerCut* eventCut = new ZeeDHistManagerCut(this->getName()+"/CutFlow/Event");
        ZeeDHistManagerCut* leptonCut = new ZeeDHistManagerCut(this->getName()+"/CutFlow/Lepton");
        ZeeDHistManagerCut* bosonCut = new ZeeDHistManagerCut(this->getName()+"/CutFlow/Boson");
        this->AddMaskLoose(selectionEvent, eventCut, doWeightNone);
        this->AddMaskLoose(selectionEvent+"+"+selectionMuon, leptonCut, doWeightNone);
        ZeeDHistManagerCut* bosonCut2 = new ZeeDHistManagerCut(this->getName()+"/CutFlow/Boson2");
        this->AddMaskLoose(selectionEvent+"+"+selectionMuon+"+WmuEtMiss", bosonCut2, doWeightNone);
        this->AddMaskLoose(selection, bosonCut, doWeightNone);
        ZeeDHistManagerCut* bosonCutPlus = new ZeeDHistManagerCut(this->getName()+"/CutFlow/BosonPlus");
        ZeeDHistManagerCut* bosonCutMinus = new ZeeDHistManagerCut(this->getName()+"/CutFlow/BosonMinus");
        this->AddMaskLoose(pos, bosonCutPlus, doWeightNone);
        this->AddMaskLoose(neg, bosonCutMinus, doWeightNone);


        //QCD EtMiss fit
        // selection 
        //TString QCDEtsel    = "NTracksAtPrimVtx+PriVtxZ+LAr+eta+EFmu10+MtWmu+ptMu+trIso+MuonMedium+notComb";
        //TString QCDEtsel    = "NTracksAtPrimVtx+PriVtxZ+LAr+eta+EFmu10+MtWmu+ptMu+MuonMedium+trIso+comb";
        TString QCDPtsel = selectionEvent+"+"+selectionMuonBase2+"+"+selectionMuonIso+"+"+selectionBoson+"+ptMuQCD";
        TString QCDPtselPos = QCDPtsel+"+pos";
        TString QCDPtselNeg = QCDPtsel+"+neg";
        TString QCDMtsel = selectionEvent+"+"+selectionMuon+"+WmuEtMiss";
        TString QCDMtselPos = QCDMtsel+"+pos";
        TString QCDMtselNeg = QCDMtsel+"+neg";
        TString QCDEtsel=selectionEvent+"+"+selectionMuon+"+"+selectionBosonBase;
        TString QCDEtselPos = QCDEtsel+"+pos";
        TString QCDEtselNeg = QCDEtsel+"+neg";

        ZeeDHistManagerQCDBkgW* QCDEtselPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/etMiss/QCDFit/"+"Boson");
        this->AddMaskLoose(QCDEtsel, QCDEtselPlot);
        ZeeDHistManagerQCDBkgW* QCDEtselPosPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/etMiss/QCDFit/"+"Plus");
        this->AddMaskLoose(QCDEtselPos, QCDEtselPosPlot);
        ZeeDHistManagerQCDBkgW* QCDEtselNegPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/etMiss/QCDFit/"+"Minus");
        this->AddMaskLoose(QCDEtselNeg, QCDEtselNegPlot);

        ZeeDHistManagerQCDBkgW* QCDPtselPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/lepPt/QCDFit/"+"Boson");
        this->AddMaskLoose(QCDPtsel, QCDPtselPlot);
        ZeeDHistManagerQCDBkgW* QCDPtselPosPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/lepPt/QCDFit/"+"Plus");
        this->AddMaskLoose(QCDPtselPos, QCDPtselPosPlot);
        ZeeDHistManagerQCDBkgW* QCDPtselNegPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/lepPt/QCDFit/"+"Minus");
        this->AddMaskLoose(QCDPtselNeg, QCDPtselNegPlot);

        ZeeDHistManagerQCDBkgW* QCDMtselPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/mtW/QCDFit/"+"Boson");
        this->AddMaskLoose(QCDMtsel, QCDMtselPlot);
        ZeeDHistManagerQCDBkgW* QCDMtselPosPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/mtW/QCDFit/"+"Plus");
        this->AddMaskLoose(QCDMtselPos, QCDMtselPosPlot);
        ZeeDHistManagerQCDBkgW* QCDMtselNegPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/mtW/QCDFit/"+"Minus");
        this->AddMaskLoose(QCDMtselNeg, QCDMtselNegPlot);


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


        ZeeDHistManagerEvent* eventPlots = new ZeeDHistManagerEvent(this->getName() + "/" + "Event");
        this->AddMaskLoose( selection, eventPlots );

        //QCD Hist Managers 
        if ((*ZeeDAnaOptions)->FillBkgHistograms()) {

            //EtMiss QCD
            TString QCD2       = selectionEvent+"+"+selectionMuonBase+"+"+selectionBosonBase+"+nTrIso";
            TString QCDPt      = selectionEvent+"+"+selectionMuonBase2+"+"+selectionBoson+"+nTrIso+comb+ptMuQCD";
            TString QCDPtPos   = QCDPt+"+pos";
            TString QCDPtNeg   = QCDPt+"+neg";
            TString QCDMt      = selectionEvent+"+"+selectionMuonBase+"+comb+nTrIso+WmuEtMiss";
            TString QCDMtPos   = QCDMt+"+pos";
            TString QCDMtNeg   = QCDMt+"+neg";
            //TString QCD         = "NTracksAtPrimVtx+PriVtxZ+LAr+eta+EFmu10+MtWmu+ptMu+notComb+hasID";
            TString QCD = selectionEvent+"+"+selectionMuonBase+"+"+selectionBosonBase+"+notComb+hasID";
            TString QCDPos    = QCD+"+pos";
            TString QCDNeg    = QCD+"+neg";


            //QCD EtMiss template	
            ZeeDHistManagerQCDBkgW* QCDPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/etMiss/QCDTempl/"+"Boson");
            this->AddMaskLoose(QCD, QCDPlot);
            ZeeDHistManagerQCDBkgW* QCDPosPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/etMiss/QCDTempl/"+"Plus");
            this->AddMaskLoose(QCDPos, QCDPosPlot);
            ZeeDHistManagerQCDBkgW* QCDNegPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/etMiss/QCDTempl/"+"Minus");
            this->AddMaskLoose(QCDNeg, QCDNegPlot);

            //QCD LepPt template	
            ZeeDHistManagerQCDBkgW* QCDPtPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/lepPt/QCDTempl/"+"Boson");
            this->AddMaskLoose(QCDPt, QCDPtPlot);
            ZeeDHistManagerQCDBkgW* QCDPtPosPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/lepPt/QCDTempl/"+"Plus");
            this->AddMaskLoose(QCDPtPos, QCDPtPosPlot);
            ZeeDHistManagerQCDBkgW* QCDPtNegPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/lepPt/QCDTempl/"+"Minus");
            this->AddMaskLoose(QCDPtNeg, QCDPtNegPlot);

            //QCD MtW template	
            ZeeDHistManagerQCDBkgW* QCDMtPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/mtW/QCDTempl/"+"Boson");
            this->AddMaskLoose(QCDMt, QCDMtPlot);
            ZeeDHistManagerQCDBkgW* QCDMtPosPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/mtW/QCDTempl/"+"Plus");
            this->AddMaskLoose(QCDMtPos, QCDMtPosPlot);
            ZeeDHistManagerQCDBkgW* QCDMtNegPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/mtW/QCDTempl/"+"Minus");
            this->AddMaskLoose(QCDMtNeg, QCDMtNegPlot);

            //Check of Fit
            TString QCDCheckFit = selectionEvent+"+"+selectionMuonBase+"+"+selectionBosonBase;

            ZeeDHistManagerQCDBkgW* QCDPlotsCh4 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/etMiss/QCDVar/"+"Plus/Medium");
            this->AddMaskLoose(QCDCheckFit+"+notMediumPP+pos", QCDPlotsCh4, doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsCh5 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/etMiss/QCDVar/"+"Minus/Medium");
            this->AddMaskLoose(QCDCheckFit+"+notMediumPP+neg", QCDPlotsCh5,  doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsCh6 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/etMiss/QCDVar/"+"Boson/Medium");
            this->AddMaskLoose(QCDCheckFit+"+notMediumPP", QCDPlotsCh6, doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsCh42 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/etMiss/QCDVar/"+"Plus/TrIso");
            this->AddMaskLoose(QCDCheckFit+"+nTrIso+pos", QCDPlotsCh42, doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsCh52 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/etMiss/QCDVar/"+"Minus/TrIso");
            this->AddMaskLoose(QCDCheckFit+"+nTrIso+neg", QCDPlotsCh52,  doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsCh62 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/etMiss/QCDVar/"+"Boson/TrIso");
            this->AddMaskLoose(QCDCheckFit+"+nTrIso", QCDPlotsCh62, doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsCh43 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/etMiss/QCDVar/"+"Plus/TrIsoComb");
            this->AddMaskLoose(QCDCheckFit+"+notComb+hasID+nTrIso+pos", QCDPlotsCh43, doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsCh53 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/etMiss/QCDVar/"+"Minus/TrIsoComb");
            this->AddMaskLoose(QCDCheckFit+"+notComb+hasID+nTrIso+neg", QCDPlotsCh53,  doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsCh63 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/etMiss/QCDVar/"+"Boson/TrIsoComb");
            this->AddMaskLoose(QCDCheckFit+"+notComb+hasID+nTrIso", QCDPlotsCh63, doWeight);




        }
    }
    //Book plots for gen level information
    if ((*ZeeDAnaOptions)->IsMC() && (*ZeeDAnaOptions)->FillGenInfo()){
        ZeeDHistManagerGenInfo * genNew = new ZeeDHistManagerGenInfo (this->getName()+"/"+"GenInfo13");
        this->AddMaskLoose(genSelNew, genNew, doWeightNone);

        ZeeDHistManagerGenInfo * gen = new ZeeDHistManagerGenInfo (this->getName()+"/"+"GenInfo");
        this->AddMaskLoose(genSel, gen, doWeightNone);
    } 


}
