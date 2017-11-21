#include "ZeeDHistManager/ZeeDAnalysisCutHistManagerW.h"

// std includes
#include <iostream>

// ZeeD analysis includes
#include "ZeeDAnalysisCuts/ZeeDCalcWeightCutDepW.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorW.h"
#include "ZeeDHistManager/ZeeDHistManagerQCDBkgW.h"
#include "ZeeDHistManager/ZeeDHistManagerElectron.h"
#include "ZeeDHistManager/ZeeDHistManagerElectronDetails.h"
#include "ZeeDHistManager/ZeeDHistManagerEtmiss.h"
#include "ZeeDHistManager/ZeeDHistManagerEvent.h"
#include "ZeeDHistManager/ZeeDHistManagerGenInfo.h"
#include "ZeeDHistManager/ZeeDHistManagerCut.h"

// Histogram managers
#include "ZeeDHistManager/ZeeDControlHistManagerW.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorW.h"

ZeeDAnalysisCutHistManagerW::ZeeDAnalysisCutHistManagerW(TString name) : ZeeDCutHistManager(name)
{
    // Constructor
}

//------------------------------------------------------
ZeeDAnalysisCutHistManagerW::~ZeeDAnalysisCutHistManagerW()
{
    // Destructor

}


void ZeeDAnalysisCutHistManagerW::BookCutHistManager()
{
    //Selection cuts
    //basic event selection 
    TString selectionEvent = "PriVtxZ+LAr+NTracksAtPrimVtx+LepTrig";
    //electron selection
    TString selectionElectronBase= "ElecClustEtaMaxW+EtaCrackElecW+ElecClustEtMinW+EtCone20+OQMaps";
    TString selectionElectron=selectionElectronBase+"+ElMediumPP";
    //TString selectionElectron = "ElecClustEtaMaxW+EtaCrackElecW+ElecClustEtMinW+ElMediumPP+EtCone20+OQMaps";
    TString selectionBosonBase="MassTransvW";
    //boson selection
    TString selectionBoson = "EtMissMinW+"+selectionBosonBase;

    //full selection (also for W+-)
    TString selection = selectionEvent + "+" +selectionElectron+"+"+ selectionBoson;
    TString selectionPlus = selection + "+WPlus";
    TString selectionMinus= selection + "+WMinus";

    //fiducial phase-space
    TString genSel        = "GenEta+GenPt+GenMt+GenEt";
    TString genSelNew        = "GenEta+GenPtNEW+GenMtNEW+GenEtNEW";

    //end of booking selection

    //retrieving analysis options
    ServiceHandle<IZeeDAnalysisSvc>* ZeeDAnaOptions = fSvcHelper.GetAnaSvc();

    //Booking different weighs
    //default do weight
    ZeeDDoWeight doWeight;
    doWeight.TrigSFSingle = kTRUE;
    doWeight.IDSFBothMediumPP = kTRUE;	
    doWeight.RecSF = kTRUE;
    doWeight.HadrRecoil = kTRUE;

    ZeeDDoWeight doWeight2;
    doWeight2.TrigSFSingle = kTRUE;
    doWeight2.IDSFBothMediumPP = kTRUE;	
    doWeight2.RecSF = kTRUE;
    doWeight2.HadrRecoil = kFALSE;


    this->SetDefaultDoWeight(doWeight);

    //do weight for QCD
    ZeeDDoWeight doWeightQCD;
    doWeightQCD.RecSF = kTRUE;
    doWeightQCD.TrigSFSingle = kTRUE;
    doWeightQCD.IDSFBothMediumPP = kFALSE;
    doWeightQCD.HadrRecoil = kTRUE;

    //no additional weights
    ZeeDDoWeight doWeightNone;

    //adding calculator for weights
    ZeeDCalcWeightCutDepW* cw = new ZeeDCalcWeightCutDepW();
    this->AddCalcWeighter(cw);



    // Create/book control plots:
    ZeeDControlHistManagerW* analysisPlots = new ZeeDControlHistManagerW(this->getName() + "/" + "Boson");
    this->AddMaskLoose( selection, analysisPlots, doWeight );

    ZeeDControlHistManagerW* wpos = new ZeeDControlHistManagerW(this->getName()+"/"+"Plus");
    this->AddMaskLoose(selectionPlus, wpos, doWeight);

    ZeeDControlHistManagerW* wneg = new ZeeDControlHistManagerW(this->getName()+"/"+"Minus");
    this->AddMaskLoose(selectionMinus, wneg, doWeight);




    if ((*ZeeDAnaOptions)->FillBkgHistograms()) {
        //filling histograms for cutflow
        ZeeDHistManagerCut* eventCut = new ZeeDHistManagerCut(this->getName()+"/CutFlow/Event");
        ZeeDHistManagerCut* leptonCut = new ZeeDHistManagerCut(this->getName()+"/CutFlow/Lepton");
        ZeeDHistManagerCut* bosonCut = new ZeeDHistManagerCut(this->getName()+"/CutFlow/Boson");
        this->AddMaskLoose(selectionEvent, eventCut, doWeightNone);
        this->AddMaskLoose(selectionEvent+"+"+selectionElectron, leptonCut);
        this->AddMaskLoose(selection, bosonCut);
        ZeeDHistManagerCut* bosonCutPlus = new ZeeDHistManagerCut(this->getName()+"/CutFlow/Plus");
        ZeeDHistManagerCut* bosonCutMinus = new ZeeDHistManagerCut(this->getName()+"/CutFlow/Minus");
        this->AddMaskLoose(selectionPlus, bosonCutPlus);
        this->AddMaskLoose(selectionMinus, bosonCutMinus);



        TString QCDMtsel = selectionEvent+"+"+selectionElectron+"+EtMissMinW";
        TString QCDMtselPos = QCDMtsel+"+WPlus";
        TString QCDMtselNeg = QCDMtsel+"+WMinus";

        //QCD EtMiss fit
        // selection 
        TString QCDEtsel = selectionEvent+"+"+selectionElectron+"+"+selectionBosonBase;
        //TString QCDEtsel    = "PriVtxZ+EtaCrackElecW+ElecClustEtaMaxW+NTracksAtPrimVtx+ElecClustEtMinW+LepTrig+LAr+ElMediumPP+MassTransvW+EtCone20";
        TString QCDEtselPos = QCDEtsel+"+WPlus";
        TString QCDEtselNeg = QCDEtsel+"+WMinus";

        ZeeDHistManagerQCDBkgW* QCDEtselPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/etMiss/QCDFit/"+"Boson");
        this->AddMaskLoose(QCDEtsel, QCDEtselPlot);
        ZeeDHistManagerQCDBkgW* QCDEtselPosPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/etMiss/QCDFit/"+"Plus");
        this->AddMaskLoose(QCDEtselPos, QCDEtselPosPlot);
        ZeeDHistManagerQCDBkgW* QCDEtselNegPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/etMiss/QCDFit/"+"Minus");
        this->AddMaskLoose(QCDEtselNeg, QCDEtselNegPlot);

        ZeeDHistManagerQCDBkgW* QCDMtselPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/mtW/QCDFit/"+"Boson");
        this->AddMaskLoose(QCDMtsel, QCDMtselPlot);
        ZeeDHistManagerQCDBkgW* QCDMtselPosPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/mtW/QCDFit/"+"Plus");
        this->AddMaskLoose(QCDMtselPos, QCDMtselPosPlot);
        ZeeDHistManagerQCDBkgW* QCDMtselNegPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/mtW/QCDFit/"+"Minus");
        this->AddMaskLoose(QCDMtselNeg, QCDMtselNegPlot);

        TString QCDMt      = selectionEvent+"+"+selectionElectronBase+"+EtMissMinW+nElMediumPP";
            TString QCDMtPos   = QCDMt+"+WPlus";
            TString QCDMtNeg   = QCDMt+"+WMinus";
            //QCD MtW template	
            ZeeDHistManagerQCDBkgW* QCDMtPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/mtW/QCDTempl/"+"Boson");
            this->AddMaskLoose(QCDMt, QCDMtPlot);
            ZeeDHistManagerQCDBkgW* QCDMtPosPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/mtW/QCDTempl/"+"Plus");
            this->AddMaskLoose(QCDMtPos, QCDMtPosPlot);
            ZeeDHistManagerQCDBkgW* QCDMtNegPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/mtW/QCDTempl/"+"Minus");
            this->AddMaskLoose(QCDMtNeg, QCDMtNegPlot);



    }

    if ((*ZeeDAnaOptions)->FillBkgHistograms() && fSys->isShiftInUse(ZeeDSystematics::eNoShift)) {
        //EtMiss QCD
        TString QCD    = selectionEvent+"+"+selectionElectronBase+"+nElMediumPP+"+selectionBosonBase;
        //TString QCD    = "PriVtxZ+EtaCrackElecW+ElecClustEtaMaxW+NTracksAtPrimVtx+ElecClustEtMinW+LepTrig+LAr+nElMediumPP+MassTransvW";
        TString QCDPos = QCD+"+WPlus";
        TString QCDNeg = QCD+"+WMinus";


        //QCD EtMiss template	
        ZeeDHistManagerQCDBkgW* QCDPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/etMiss/QCDTempl/"+"Boson");
        this->AddMaskLoose(QCD, QCDPlot);
        ZeeDHistManagerQCDBkgW* QCDPosPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/etMiss/QCDTempl/"+"Plus");
        this->AddMaskLoose(QCDPos, QCDPosPlot);
        ZeeDHistManagerQCDBkgW* QCDNegPlot = new ZeeDHistManagerQCDBkgW(this->getName()+"/QCD/etMiss/QCDTempl/"+"Minus");
        this->AddMaskLoose(QCDNeg, QCDNegPlot);


        ZeeDHistManagerElectron* electronPlots = new ZeeDHistManagerElectron(this->getName()+"/"+"Electron");
        this->AddMaskLoose(selection, electronPlots);
        electronPlots->SetW();
        ZeeDHistManagerElectronDetails* electronPlots2 = new ZeeDHistManagerElectronDetails(this->getName()+"/"+"ElectronDet");
        this->AddMaskLoose(selection, electronPlots2);
        electronPlots2->SetW();

        ZeeDHistManagerEtmiss* etMissPlots = new ZeeDHistManagerEtmiss(this->getName() + "/" + "EtMiss");
        this->AddMaskLoose( selection, etMissPlots);
        ZeeDHistManagerEtmiss* etMissPlotsP = new ZeeDHistManagerEtmiss(this->getName() + "/" + "EtMissPlus");
        this->AddMaskLoose( selection+"+WPlus",  etMissPlotsP);
        ZeeDHistManagerEtmiss* etMissPlotsM = new ZeeDHistManagerEtmiss(this->getName() + "/" + "EtMissMinus");
        this->AddMaskLoose( selection+"+WMinus", etMissPlotsM);

        //ZeeDHistManagerEtmiss* etMissPlotsOld = new ZeeDHistManagerEtmiss(this->getName() + "/" + "EtMissOld");
        //this->AddMaskLoose( selectionEtMissOld, etMissPlotsOld);



        ZeeDHistManagerEvent* eventPlots = new ZeeDHistManagerEvent(this->getName() + "/" + "Event");
        this->AddMaskLoose( selection, eventPlots );

        //QCD Hist Managers 
        if ((*ZeeDAnaOptions)->FillBkgHistograms()) {
            //Check of Fit
            TString QCDCheckFit=selectionEvent+"+"+selectionElectronBase+"+"+selectionBosonBase;
            //TString QCDCheckFit = "PriVtxZ+EtaCrackElecW+ElecClustEtaMaxW+NTracksAtPrimVtx+ElecClustEtMinW+MassTransvW+LepTrig+LAr+EtCone20";
            ZeeDHistManagerQCDBkgW* QCDPlotsCh1 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/etMiss/QCDVar/"+"Plus/Tight");
            this->AddMaskLoose(QCDCheckFit+"+nElTightPP+WPlus", QCDPlotsCh1, doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsCh2 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/etMiss/QCDVar/"+"Minus/Tight");
            this->AddMaskLoose(QCDCheckFit+"+nElTightPP+WMinus", QCDPlotsCh2,  doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsCh3 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/etMiss/QCDVar/"+"Boson/Tight");
            this->AddMaskLoose(QCDCheckFit+"+nElTightPP", QCDPlotsCh3, doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsCh4 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/etMiss/QCDVar/"+"Plus/Loose");
            this->AddMaskLoose(QCDCheckFit+"+nElLoosePP+WPlus", QCDPlotsCh4, doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsCh5 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/etMiss/QCDVar/"+"Minus/Loose");
            this->AddMaskLoose(QCDCheckFit+"+nElLoosePP+WMinus", QCDPlotsCh5,  doWeight);

            ZeeDHistManagerQCDBkgW* QCDPlotsCh6 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/etMiss/QCDVar/"+"Boson/Loose");
            this->AddMaskLoose(QCDCheckFit+"+nElLoosePP+WPlus", QCDPlotsCh6, doWeight);
/*

            //QCD Pt 
            //TString QCDSelectionPt = "PriVtxZ+EtaCrackElecW+ElecClustEtaMaxW+NTracksAtPrimVtx+EtMissMinW+ElecClustEtMinW+LepTrig+nElMediumPP+LAr+EtCone20+ElLoosePP";
            TString QCDSelectionPt =  "PriVtxZ+EtaCrackElecW+ElecClustEtaMaxW+NTracksAtPrimVtx+EtMissMinW+ElecClustEtMinW+LepTrig+nElMediumPP+LAr+EtCone20+ElLoosePP";

            TString QCDPtVar = "PriVtxZ+EtaCrackElecW+ElecClustEtaMaxW+NTracksAtPrimVtx+EtMissMinW+ElecClustEtMinW+LepTrig+LAr+ElLoosePP+EtCone20";


            //QCD Pt template
            ZeeDHistManagerQCDBkgW* QCDPlotPt = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/mtW/QCDTempl/"+"Boson");
            this->AddMaskLoose(QCDSelectionPt, QCDPlotPt, doWeightQCD);
            ZeeDHistManagerQCDBkgW* QCDPlotPtPos = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/mtW/QCDTempl/"+"Plus");
            this->AddMaskLoose(QCDSelectionPt+"+WPlus", QCDPlotPtPos, doWeightQCD); 
            ZeeDHistManagerQCDBkgW* QCDPlotPtNeg = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/mtW/QCDTempl/"+"Minus");
            this->AddMaskLoose(QCDSelectionPt+"+WMinus", QCDPlotPtNeg, doWeightQCD);

            //template variation
            ZeeDHistManagerQCDBkgW* QCDPlotsPt1 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/mtW/QCDVar/"+"Plus/Tight");
            this->AddMaskLoose(QCDPtVar+"+nElTightPP+WPlus", QCDPlotsPt1, doWeightQCD);

            ZeeDHistManagerQCDBkgW* QCDPlotsPt2 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/mtW/QCDVar/"+"Minus/Tight");
            this->AddMaskLoose(QCDPtVar+"+nElTightPP+WMinus", QCDPlotsPt2,  doWeightQCD);

            ZeeDHistManagerQCDBkgW* QCDPlotsPt3 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/mtW/QCDVar/"+"Boson/Tight");
            this->AddMaskLoose(QCDPtVar+"+nElTightPP", QCDPlotsPt3, doWeightQCD);

            ZeeDHistManagerQCDBkgW* QCDPlotsPt4 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/mtW/QCDVar/"+"Plus/Loose");
            this->AddMaskLoose(QCDPtVar+"+nElLoosePP+WPlus", QCDPlotsPt4, doWeightQCD);

            ZeeDHistManagerQCDBkgW* QCDPlotsPt5 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/mtW/QCDVar/"+"Minus/Loose");
            this->AddMaskLoose(QCDPtVar+"+nElLoosePP+WMinus", QCDPlotsPt5,  doWeightQCD);

            ZeeDHistManagerQCDBkgW* QCDPlotsPt6 = new ZeeDHistManagerQCDBkgW (this->getName()+"/QCD/mtW/QCDVar/"+"Boson/Loose");
            this->AddMaskLoose(QCDPtVar+"+nElLoosePP", QCDPlotsPt6, doWeightQCD);
*/

        }
        //end of QCD plots    
    }

    if ((*ZeeDAnaOptions)->IsMC()){ 
        ZeeDHistManagerGenInfo * gen = new ZeeDHistManagerGenInfo (this->getName()+"/"+"GenInfo");
        this->AddMaskLoose(genSel, gen, doWeightNone);
        ZeeDHistManagerGenInfo * genNew = new ZeeDHistManagerGenInfo (this->getName()+"/"+"GenInfo13");
        this->AddMaskLoose(genSelNew, genNew, doWeightNone);


    }
    if ((*ZeeDAnaOptions)->IsMC() && (*ZeeDAnaOptions)->EvaluateSystematics()){
        
        //booking histograms for systematics checks
        ZeeDControlHistManagerW* wSim = new ZeeDControlHistManagerW(this->getName()+"/"+"BosonSim");
        this->AddMaskLoose(selection+"+EtaSim", wSim, doWeight);

        ZeeDControlHistManagerW* wposSim = new ZeeDControlHistManagerW(this->getName()+"/"+"PlusSim");
        this->AddMaskLoose(selectionPlus+"+EtaSim", wposSim, doWeight);

        ZeeDControlHistManagerW* wnegSim = new ZeeDControlHistManagerW(this->getName()+"/"+"MinusSim");
        this->AddMaskLoose(selectionMinus+"+EtaSim", wnegSim, doWeight);
    }
    /*
    //Book plots for gen level information    
    if ((*ZeeDAnaOptions)->IsMC() && (*ZeeDAnaOptions)->FillGenInfo()){
    if (fSys->isShiftInUse(ZeeDSystematics::eNoShift)){
    ZeeDControlHistManagerW * unfMan = new ZeeDControlHistManagerW(this->getName()+"/"+"GenCut");
    this->AddMaskLoose(selUnf, unfMan);
    ZeeDControlHistManagerW * unfManNEW = new ZeeDControlHistManagerW(this->getName()+"/"+"GenCutNEW");
    this->AddMaskLoose(selUnfNEW, unfManNEW);
    ZeeDHistManagerGenInfo * genNEW = new ZeeDHistManagerGenInfo (this->getName()+"/"+"GenNEW");
    this->AddMaskLoose(genSelNEW, genNEW, doWeightNone);
    }
    }*/
}
