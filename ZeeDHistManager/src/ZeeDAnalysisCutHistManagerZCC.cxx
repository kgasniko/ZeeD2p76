#include "ZeeDHistManager/ZeeDAnalysisCutHistManagerZCC.h"

// Root includes
#include <TString.h>

// ZeeD analysis includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

// Cut selector:
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorZCC.h"

// Weights:
#include "ZeeDAnalysisCuts/ZeeDCalcWeightCutDepZ.h"

// Hist managers:
#include "ZeeDHistManager/ZeeDHistManagerElectron.h"
#include "ZeeDHistManager/ZeeDHistManagerBoson.h"
#include "ZeeDHistManager/ZeeDHistManagerZjet.h"
#include "ZeeDHistManager/ZeeDHistManagerEvent.h"
#include "ZeeDHistManager/ZeeDHistManagerMuon.h"
#include "ZeeDHistManager/ZeeDHistManagerEtmiss.h"
#include "ZeeDHistManager/ZeeDCalibratorHM.h"
#include "ZeeDHistManager/ZeeDHistManagerElectronDetails.h"
#include "ZeeDHistManager/ZeeDHistManagerTruthMatch.h"
#include "ZeeDHistManager/ZeeDHistManagerWriter.h"
#include "ZeeDHistManager/ZeeDHistManagerPrint.h"
#include "ZeeDHistManager/ZeeDGetVariables.h"
#include "ZeeDHistManager/ZeeDHistManagerGenInfo.h"
#include "ZeeDHistManager/ZeeDHistManagerZjetAna.h"
#include "ZeeDHistManager/ZeeDHistManagerZplusJet.h"
#include "ZeeDHistManager/ZeeDHistManagerZplusJetUnfolding.h"
#include "ZeeDHistManager/ZeeDHistManagerJetCharge.h"
#include "ZeeDHistManager/ZeeDHistManagerCut.h"
#include "ZeeDHistManager/ZeeDHistManagerValidation.h"

#include "ZeeDHist/ZeeDObservable.h"
#include "ZeeDHist/ZeeDSimpleMeasurement.h"
#include "ZeeDHist/ZeeDGridHistManager.h"

//#define DEBUG_ON 0
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDArrays/ZeeDArrayD.h"
#include "ZeeDArrays/ZeeDBinGrid.h"

#include "ZeeDTools/ZeeDQuantity.h"

#include <iostream>

// Helper function
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDGenParticle.h"

void ZeeDAnalysisCutHistManagerZCC::BookCutHistManager() {
    // Define list of cuts to be used
    std::vector <std::string> CutNames;
    std::vector < std::string > Stages;

    CutNames.push_back("ExistZ");
    CutNames.push_back("PtMinBothElecZ");
    CutNames.push_back("EtaMaxBothElecZ");
    CutNames.push_back("EtaCrackBothElecZ");
    CutNames.push_back("ZMassZ");
	CutNames.push_back("EF_e15_loose1");
	CutNames.push_back("ElecTrig");

    //CutNames.push_back("AuthorBothElecZ");
    CutNames.push_back("IsEMMediumPPBothElecZ");
    CutNames.push_back("ChargeBothElecOppositeZ");
    CutNames.push_back("NTracksAtPrimVtx");
    CutNames.push_back("OQMaps");
    TString genSel        = "GenEta+GenPt+GenMt";
    TString genSelNEW     = "GenEtaNEW+GenPtNEW+GenMt";


    // Use Trigger Cut
   /* if ((*fAnaOptions)->UseTrigger()) {
      CutNames.push_back("Trigger_2e12loose1_OR_L2StarB");
    }*/

    //CutNames.push_back("MaxTwoGoodElec");
    CutNames.push_back("ZeeDCutLArEventVeto");

    Stages.push_back(CutNames[0]);
    for ( unsigned i=1; i<CutNames.size(); i++) {
        Stages.push_back(Stages.back()+"+"+CutNames[i]);
    }  
    // define kinematic selection  
    // const std::string KinematicSelection = Stages[4];

    
    // Default cut
    const std::string DefaultCuts = Stages.back();

    // Set default mask
    this->SetDefaultMask(DefaultCuts);

    // Set event weights calculator
    ZeeDCalcWeightCutDepZ* cw = new ZeeDCalcWeightCutDepZ();
    this->AddCalcWeighter(cw);

    // Set Default weight
    ZeeDDoWeight doWeight;
    doWeight.IDSFBothMediumPP = kTRUE;
    doWeight.TrigSFDiMediumPP = kTRUE;
	doWeight.RecSF = kTRUE;

    this->SetDefaultDoWeight(doWeight);

    ZeeDDoWeight doWeightIso;
    doWeightIso.IsoSFBoth = kTRUE;
    doWeightIso.RecSF = kTRUE;
    doWeightIso.IDSFBothMediumPP = kTRUE;

    ZeeDDoWeight doWeightNone;

    // Plots after all cuts
    this->AddMaskLoose("DEFAULT", new ZeeDHistManagerBoson(this->getName() + "/" + "Boson"));
    this->AddMaskLoose("DEFAULT", new ZeeDHistManagerElectron(this->getName() + "/" + "Electron"));
    this->AddMaskLoose("DEFAULT", new ZeeDHistManagerEvent(this->getName() + "/" + "Event"));
    this->AddMaskLoose("DEFAULT", new ZeeDHistManagerEtmiss(this->getName() + "/" + "Etmiss"));

//    this->AddMaskLoose("", new ZeeDHistManagerMuon(this->getName() + "/" + "Muon"));

    if ((*fAnaOptions)->IsMC() && (*fAnaOptions)->FillGenInfo()){
        ZeeDHistManagerGenInfo * gen = new ZeeDHistManagerGenInfo (this->getName()+"/"+"GenInfo");
        this->AddMaskLoose(genSel, gen);
    }

   

    
    
    /*
    if ((*fAnaOptions)->AddDetailHM()) {
        this->AddMaskLoose("DEFAULT", new ZeeDHistManagerElectronDetails(this->getName() + "/" + "ElectronDetails"));
    }

    if ((*fAnaOptions)->AddSingleRejectsHMs()) {
        for (unsigned i =0 ; i<Stages.size(); i++ ) {
            this->AddMask("DEFAULT-"+CutNames[i], new ZeeDHistManagerElectron(this->getName() + "/SingleReject/" + "Electron_"+CutNames[i]),"DEFAULT");
            this->AddMask("DEFAULT-"+CutNames[i], new ZeeDHistManagerBoson(this->getName() + "/SingleReject/" + "Boson_"+CutNames[i]),"DEFAULT");
            if ((*fAnaOptions)->AddDetailHM()) {
                this->AddMask("DEFAULT-"+CutNames[i], new ZeeDHistManagerElectronDetails(this->getName() + "/SingleReject/" + "ElectronDetails_"+CutNames[i]),"DEFAULT");
            }
            
        }        
    }
    
*/
 /*   
    if ((*fAnaOptions)->CalcJets()) {
	this->AddMaskLoose("DEFAULT", new ZeeDHistManagerValidation(this->getName() + "/" + "Validation"));
      if ((*fAnaOptions)->DoZjet()){
	this->AddMaskLoose("DEFAULT", new ZeeDHistManagerZjet(this->getName() + "/" + "Zjet"));
      }
      if ((*fAnaOptions)->DoZjetAna()){
        this->AddMaskLoose("DEFAULT", new ZeeDHistManagerZjetAna(this->getName() + "/" + "ZjetAna"));
      }
      if ((*fAnaOptions)->DoZplusJet()){
	this->AddMaskLoose("DEFAULT", new ZeeDHistManagerZplusJet(this->getName() + "/" + "ZplusJet"));
	this->AddMaskLoose("DEFAULT", new ZeeDHistManagerZplusJetUnfolding(this->getName() + "/" + "ZplusJetUnfolding"));
      }
      if ((*fAnaOptions)->CalcJetCharge()){
	this->AddMaskLoose("DEFAULT", new ZeeDHistManagerJetCharge(this->getName() + "/" + "JetCharge"));
      }

    }

     if ((*fAnaOptions)->IsMC() && (*fAnaOptions)->FillGenInfo()) {
        this->AddMaskLoose("ExistBornBoson+ExistTwoBornElectrons+MassBornBoson+PtMinBothBornElecZ",
                new ZeeDHistManagerGenInfo(this->getName() + "/" + "GenInfoComb"), doWeightNone);
        this->AddMaskLoose("ExistBornBoson+ExistTwoBornElectrons+MassBornBoson+PtMinBothBornElecZ+EtaMaxBothBornElecZ",
                new ZeeDHistManagerGenInfo(this->getName() + "/" + "GenInfoFidu"),
                doWeightNone);
    }

    if (((*fAnaOptions)->CalcJets()) && ((*fAnaOptions)->CalcJetCharge())){
        const TString BosMass = ZeeDMisc::FindFile("Binning/BosMassJetCharge.txt");
        ZeeDGridHistManager* massGrid = new ZeeDGridHistManager(this->getName() + "/" + "BM", BosMass, &getBosonMass);
        massGrid->RegisterHM < ZeeDHistManagerJetCharge > (this->getName() + "/" + "BM");
        this->AddMaskLoose("DEFAULT", massGrid);

    }

    if ((*fAnaOptions)->FillObservable()) {
        // Define for all grids:
        TString BaseName = this->getName() + "/" + "Signal";
        Bool_t isMC = (*fAnaOptions)->IsMC();*/
/*
        TString SignalGrid = "Binning/BosMass.txt";
	ZeeDCutBit* genCuts = this->DecodeName("ExistBornBoson+ExistTwoBornElectrons+MassBornBoson+PtMinBothBornElecZ");
	ZeeDCutBit* recCuts = this->DecodeName("DEFAULT");
	this->AddMaskLoose("NONE", new ZeeDSimpleMeasurement(BaseName + "_SimpleMeas",	SignalGrid, recCuts, &getBornBosonMass,
	     										SignalGrid, genCuts, &getBosonMass,
										       	isMC));
*/	
/*
        ZeeDQuantity<ZeeDArrayD> (*cVarREC)(const ZeeDEvent*);
        ZeeDQuantity<ZeeDArrayD> (*cVarGEN)(const ZeeDEvent*);
	ZeeDQuantity<ZeeDArrayD> (*cVarBG)(const ZeeDEvent*);

        TString SignalGrid_test = "Binning/BosYMassCosTh.txt";
	cVarBG = &getIsoFracMax;
        cVarREC = &getAbsY_M_cosTh_rec;
        cVarGEN = &getAbsY_M_cosTh_gen;
	TString BgGrid = "Binning/ElecIsolationFraction.txt";
	this->AddMaskLoose("DEFAULT", new ZeeDObservable(BaseName + "_RecCut", SignalGrid_test, BgGrid, cVarREC, cVarGEN, cVarBG, kTRUE, isMC, isMC));
*/

	/*
        // Add boson hist managers for BosMass bin grids:
        const TString BosMass = ZeeDMisc::FindFile("Binning/BosMass.txt");
        ZeeDGridHistManager* massGrid = new ZeeDGridHistManager(this->getName() + "/" + "BM", BosMass, &getBosonMass);
        massGrid->RegisterHM < ZeeDHistManagerBoson > (this->getName() + "/" + "BM");
        this->AddMaskLoose("DEFAULT", massGrid);
	*/
  /*  }

    if ((*fAnaOptions)->FillBkgHistograms()) {
        // Add background histograms
        this->AddMask("DEFAULT-ChargeBothElecOppositeZ", new ZeeDHistManagerBoson(this->getName() + "/" + "BosonSC"));
        this->AddMask("DEFAULT-ChargeBothElecOppositeZ", new ZeeDHistManagerElectron(this->getName() + "/" + "ElectronSC"));
        this->AddMask("DEFAULT-ChargeBothElecOppositeZ", new ZeeDHistManagerEvent(this->getName() + "/" + "EventSC"));
        this->AddMask("DEFAULT-ChargeBothElecOppositeZ-IsEMMediumPPBothElecZ", new ZeeDHistManagerBoson(this->getName() + "/" + "BosonBG"));
        this->AddMask("DEFAULT-ChargeBothElecOppositeZ-IsEMMediumPPBothElecZ", new ZeeDHistManagerElectron(this->getName() + "/" + "ElectronBG"));
        this->AddMask("DEFAULT-ChargeBothElecOppositeZ-IsEMMediumPPBothElecZ", new ZeeDHistManagerEvent(this->getName() + "/" + "EventBG"));
    }

    if ((*fAnaOptions)->AnalysisCutHistManagerZFillStages()) {
        // Book main control plots for stages:
        for (unsigned int s = 0; s != Stages.size(); ++s) {
            TString hmname = "_Stage";
            hmname += s;
            this->AddMaskLoose(Stages[s], new ZeeDHistManagerBoson(this->getName() + "/" + "Boson" + hmname));
            this->AddMaskLoose(Stages[s], new ZeeDHistManagerElectron(this->getName() + "/" + "Electron" + hmname));
            this->AddMaskLoose(Stages[s], new ZeeDHistManagerEvent(this->getName() + "/" + "Event" + hmname));
        }
    }

    if ((*fAnaOptions)->FillCalibrator()) {
        // Calibrator:
        this->AddMaskLoose("DEFAULT", new ZeeDCalibratorHM(this->getName() + "/Calib"));
    }
*/
}
