#include "ZeeDHistManager/ZeeDAnalysisCutHistManagerZCF.h"
#include <TString.h>

// ZeeD analysis includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDAnalysisCuts/ZeeDCalcWeightCutDepZ.h"

// Histogram managers
#include "ZeeDHistManager/ZeeDHistManagerElectron.h"
#include "ZeeDHistManager/ZeeDHistManagerElectronDetails.h"
#include "ZeeDHistManager/ZeeDHistManagerBoson.h"
#include "ZeeDHistManager/ZeeDHistManagerZjet.h"
#include "ZeeDHistManager/ZeeDHistManagerEvent.h"
#include "ZeeDHistManager/ZeeDHistManagerEtmiss.h"
#include "ZeeDHistManager/ZeeDHistManagerQCDBkg.h"
#include "ZeeDHistManager/ZeeDHistManagerYield.h"
#include "ZeeDHistManager/ZeeDHistManagerBosonFwdExtra.h"
#include "ZeeDHistManager/ZeeDGetVariables.h"
#include "ZeeDHistManager/ZeeDHistManagerGenInfo.h"
#include "ZeeDHistManager/ZeeDHistManagerZjetAna.h"
#include "ZeeDHistManager/ZeeDHistManagerBkgDscrZCF.h"
#include "ZeeDHistManager/ZeeDHistManagerControlCF.h"
#include "ZeeDHistManager/ZeeDHistManagerWeights.h"

#include "ZeeDHistManager/ZeeDCalibratorHM.h"
#include "ZeeDHistManager/ZeeDHistManagerPrint.h"
#include "ZeeDHistManager/ZeeDHistManagerEWUnfoldWriter.h"

#include "ZeeDHist/ZeeDGridHistManager.h"
#include "ZeeDHistManager/ZeeDGetVariables.h"
#include "ZeeDHist/ZeeDObservable.h"

//#define DEBUG_ON 0
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDArrays/ZeeDArrayD.h"
#include "ZeeDArrays/ZeeDBinGrid.h"

#include "ZeeDTools/ZeeDQuantity.h"

#include <boost/algorithm/string/join.hpp>
#include "ZeeDTools/ZeeDSystematics.h"

void ZeeDAnalysisCutHistManagerZCF::BookCutHistManager() {
  DEBUG_MES_START;
  // Analysis options
  ServiceHandle < IZeeDAnalysisSvc > *ZeeDAnaOptions = fSvcHelper.GetAnaSvc();

  std::string str = this->getName().Data();
  bool isNoShift = (str.find("NoShift") > 1 && str.find("NoShift")<150);
  std::vector < std::string > Stages;
  
  //Z boson selection:
  Stages.push_back("ExistZ");
  Stages.push_back(Stages.back() + "+ZMassZ");
  
  //Event selection:
  Stages.push_back(Stages.back() + "+ZeeDCutLArEventVeto");
  Stages.push_back(Stages.back() + "+MaxTwoGoodElecTight");
  Stages.push_back(Stages.back() + "+NTracksAtPrimVtx");
  
  //Central electron selection:
  Stages.push_back(Stages.back() + "+PtMinCentralElecZ");
  Stages.push_back(Stages.back() + "+PtCone20overEt");
  Stages.push_back(Stages.back() + "+IsEMTightPPCentralElecZ");  
  Stages.push_back(Stages.back() + "+IsEMMediumCentralElecZ");
  Stages.push_back(Stages.back() + "+ZeeDCutTrackCentralElecZ"); //has track
  
  //Forward electron selection:
  Stages.push_back(Stages.back() + "+PtMinForwardElecZ");
  Stages.push_back(Stages.back() + "+EtaFwdCrackBothElecZ"); //crack between EMEC and FCal 3.16-3.35
  Stages.push_back(Stages.back() + "+EtaCFBothElecZ");       //center electron eta<2.47; fwd electron 2.5<eta<4.9
  Stages.push_back(Stages.back() + "+IsEMNewFwdTightElecZ"); 

  //For both electrons:
  Stages.push_back(Stages.back() + "+AuthorCFElecZ");
  Stages.push_back(Stages.back() + "+EtaCrackBothElecZ"); //crack for 1.37<eta<1.52   ...modify to 1.425-1.55? https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/Egamma2012Analyses#Egamma_Object_Identification
  
  if ((*ZeeDAnaOptions)->UseTrigger()) {
    Stages.push_back(Stages.back() + "+EF_e24vhi_medium1_OR_EF_e60_medium1");
  }
  if ((*ZeeDAnaOptions)->UseTrigger() && (*ZeeDAnaOptions)->DoTriggerMatching()) {
    Stages.push_back(Stages.back() + "+Match_EF_e24vhi_medium1_OR_EF_e60_medium1");
  }

  std::string DefaultCuts = Stages.back();
  
  this->SetDefaultMask(DefaultCuts);
  
  // Set event weights calculator
  ZeeDCalcWeightCutDepZ* cw = new ZeeDCalcWeightCutDepZ();
  this->AddCalcWeighter(cw);

  ZeeDDoWeight doWeight;
  doWeight.IDSFCentralTightPP = kTRUE;
  doWeight.IDSFNewFwdTight = kTRUE;
  doWeight.RecSFCentral = kTRUE;
  doWeight.IsoSFCentral = kTRUE;
  doWeight.TrigSFSingle = kTRUE;
  
  this->SetDefaultDoWeight(doWeight);

  ZeeDDoWeight doWeightNone;
  
  //find files with binning:
  const TString BosMass = ZeeDMisc::FindFile("Binning/BosMass.txt");
  const TString BosY = ZeeDMisc::FindFile("Binning/BosAbsY.txt");
  const TString TrippleGrid = ZeeDMisc::FindFile("Binning/BosYMassCosTh.txt");
    
  ////////////------- Main control plots -------/////////// 
  if ( isNoShift ) {
    this->AddMaskLoose("DEFAULT", new ZeeDHistManagerBoson(this->getName() + "/" + "Boson"));
    this->AddMaskLoose("DEFAULT", new ZeeDHistManagerElectron(this->getName() + "/" + "Electron"));
    this->AddMaskLoose("DEFAULT", new ZeeDHistManagerEvent(this->getName() + "/" + "Event"));
    this->AddMaskLoose("DEFAULT", new ZeeDHistManagerEtmiss(this->getName() + "/" + "Etmiss"));
    if ((*fAnaOptions)->FillBkgHistograms()) {
      this->AddMask("DEFAULT-IsEMNewFwdTightElecZ", new ZeeDHistManagerEtmiss(this->getName() + "/" + "Etmiss_bg"), "DEFAULT",doWeightNone); 
    }
  }
  //control plots in mass bins
  ZeeDGridHistManager* massGrid = new ZeeDGridHistManager(this->getName() + "/" + "BM", BosMass, &getBosonMass);
  massGrid->RegisterHM < ZeeDHistManagerBoson > (this->getName() + "/" + "BM");
  this->AddMaskLoose("DEFAULT", massGrid); 

  if ((*fAnaOptions)->FillGenInfo()) {
    this->AddMaskLoose("NONE", new ZeeDHistManagerWeights(this->getName() + "/" + "NoCuts"), doWeightNone);
  }
  
    ////////////-------Histograms for QCD background estimation-------///////////   
  if ((*fAnaOptions)->FillBkgHistograms() && isNoShift ) {

    // I. Signal selection for background extraction:
    ////Ia. Inclusive signal:
    this->AddMaskLoose("DEFAULT", new ZeeDHistManagerBkgDscrZCF(this->getName() + "/" + "Sig_def"));
    this->AddMaskLoose("DEFAULT-PtCone20overEt", new ZeeDHistManagerBkgDscrZCF(this->getName() + "/" + "Sig_NoIso", true));
    
    ////Ib. Single-differential, in mass bins, signal:
    ZeeDGridHistManager* massGrid1 = new ZeeDGridHistManager(this->getName() + "/" + "BM_Sig_def", BosMass, &getBosonMass);
    massGrid1->RegisterHM < ZeeDHistManagerBkgDscrZCF > (this->getName() + "/" + "BM_Sig_def");
    this->AddMaskLoose("DEFAULT", massGrid1);
    
    ZeeDGridHistManager* massGrid2 = new ZeeDGridHistManager(this->getName() + "/" + "BM_Sig_NoIso", BosMass, &getBosonMass);
    massGrid2->RegisterHM < ZeeDHistManagerBkgDscrZCF > (this->getName() + "/" + "BM_Sig_NoIso");
    this->AddMaskLoose("DEFAULT-PtCone20overEt", massGrid2);
    
    ////Ic. Single-differential, in rapidity bins, signal: 
    ZeeDGridHistManager* YGrid1 = new ZeeDGridHistManager(this->getName() + "/" + "Y_Sig_def", BosY, &getBosonAbsY);
    YGrid1->RegisterHM < ZeeDHistManagerBkgDscrZCF > (this->getName() + "/" + "Y_Sig_def");
    this->AddMaskLoose("DEFAULT", YGrid1);
    
    ZeeDGridHistManager* YGrid2 = new ZeeDGridHistManager(this->getName() + "/" + "Y_Sig_NoIso", BosY, &getBosonAbsY);
    YGrid2->RegisterHM < ZeeDHistManagerBkgDscrZCF > (this->getName() + "/" + "Y_Sig_NoIso");
    this->AddMaskLoose("DEFAULT-PtCone20overEt", YGrid2);  
    
    //Id. Triple-differential, in rapidity, mass and cosTh bins. signal:
    ZeeDGridHistManager* SignalGrid = new ZeeDGridHistManager(this->getName() + "/" + "BYMC_Sig", TrippleGrid, &getAbsY_M_cosTh_rec);
    SignalGrid->RegisterHM < ZeeDHistManagerBkgDscrZCF > (this->getName() + "/" + "BYMC_Sig");
    this->AddMaskLoose("DEFAULT", SignalGrid);
    
    //II. Template selection for background extraction:
    ////IIa. Inclusive bkg template:
    this->AddMask("DEFAULT-IsEMNewFwdTightElecZ-IsEMTightPPCentralElecZ", new ZeeDHistManagerBkgDscrZCF(this->getName() + "/" + "BG_def"), "DEFAULT",doWeightNone);    
    this->AddMask("DEFAULT-IsEMNewFwdTightElecZ-IsEMTightPPCentralElecZ", new ZeeDHistManagerBkgDscrZCF(this->getName() + "/" + "BG_NoIso"), "DEFAULT-PtCone20overEt", doWeightNone);
    this->AddMask("DEFAULT-IsEMNewFwdTightElecZ", new ZeeDHistManagerBkgDscrZCF(this->getName() + "/" + "BG_def_c"), "DEFAULT",doWeightNone);  
    this->AddMask("DEFAULT-IsEMNewFwdTightElecZ", new ZeeDHistManagerBkgDscrZCF(this->getName() + "/" + "BG_NoIso_c"), "DEFAULT-PtCone20overEt",doWeightNone);

    ////IIb. Single-differential, in mass bins, bkg template:
    ZeeDGridHistManager* massGrid3 = new ZeeDGridHistManager(this->getName() + "/" + "BM_BG_def", BosMass, &getBosonMass);
    massGrid3->RegisterHM < ZeeDHistManagerBkgDscrZCF > (this->getName() + "/" + "BM_BG_def");
    this->AddMask("DEFAULT-IsEMNewFwdTightElecZ-IsEMTightPPCentralElecZ", massGrid3, "DEFAULT",doWeightNone); 

    ZeeDGridHistManager* massGrid4 = new ZeeDGridHistManager(this->getName() + "/" + "BM_BG_NoIso", BosMass, &getBosonMass);
    massGrid4->RegisterHM < ZeeDHistManagerBkgDscrZCF > (this->getName() + "/" + "BM_BG_NoIso");
    this->AddMask("DEFAULT-IsEMNewFwdTightElecZ-IsEMTightPPCentralElecZ", massGrid4, "DEFAULT-PtCone20overEt",doWeightNone);	

      //the same, but only fwd electron inversion
    ZeeDGridHistManager* massGrid5 = new ZeeDGridHistManager(this->getName() + "/" + "BM_BG_def_c", BosMass, &getBosonMass);
    massGrid5->RegisterHM < ZeeDHistManagerBkgDscrZCF > (this->getName() + "/" + "BM_BG_def_c");
    this->AddMask("DEFAULT-IsEMNewFwdTightElecZ", massGrid5, "DEFAULT",doWeightNone); 

    ZeeDGridHistManager* massGrid6 = new ZeeDGridHistManager(this->getName() + "/" + "BM_BG_NoIso_c", BosMass, &getBosonMass);
    massGrid6->RegisterHM < ZeeDHistManagerBkgDscrZCF > (this->getName() + "/" + "BM_BG_NoIso_c");
    this->AddMask("DEFAULT-IsEMNewFwdTightElecZ", massGrid6, "DEFAULT-PtCone20overEt",doWeightNone);
    
    ////IIc. Single-differential, in rapidity bins, bkg template:  
    ZeeDGridHistManager* YGrid3 = new ZeeDGridHistManager(this->getName() + "/" + "Y_BG_def", BosY, &getBosonAbsY);
    YGrid3->RegisterHM < ZeeDHistManagerBkgDscrZCF > (this->getName() + "/" + "Y_BG_def");
    this->AddMask("DEFAULT-IsEMNewFwdTightElecZ-IsEMTightPPCentralElecZ", YGrid3, "DEFAULT",doWeightNone); 

    ZeeDGridHistManager* YGrid4 = new ZeeDGridHistManager(this->getName() + "/" + "Y_BG_NoIso", BosY, &getBosonAbsY);
    YGrid4->RegisterHM < ZeeDHistManagerBkgDscrZCF > (this->getName() + "/" + "Y_BG_NoIso");
    this->AddMask("DEFAULT-IsEMNewFwdTightElecZ-IsEMTightPPCentralElecZ", YGrid4, "DEFAULT-PtCone20overEt",doWeightNone); 

    //the same, but only fwd electron inversion  
    ZeeDGridHistManager* YGrid5 = new ZeeDGridHistManager(this->getName() + "/" + "Y_BG_def_c", BosY, &getBosonAbsY);
    YGrid5->RegisterHM < ZeeDHistManagerBkgDscrZCF > (this->getName() + "/" + "Y_BG_def_c");
    this->AddMask("DEFAULT-IsEMNewFwdTightElecZ", YGrid5, "DEFAULT",doWeightNone); 

    ZeeDGridHistManager* YGrid6 = new ZeeDGridHistManager(this->getName() + "/" + "Y_BG_NoIso_c", BosY, &getBosonAbsY);
    YGrid6->RegisterHM < ZeeDHistManagerBkgDscrZCF > (this->getName() + "/" + "Y_BG_NoIso_c");
    this->AddMask("DEFAULT-IsEMNewFwdTightElecZ", YGrid6, "DEFAULT-PtCone20overEt",doWeightNone);   
    
    //IId. Triple-differential, in rapidity, mass and cosTh bins. template:
    ZeeDGridHistManager* BgGrid_c = new ZeeDGridHistManager(this->getName() + "/" + "BYMC_BG_def_c", TrippleGrid, &getAbsY_M_cosTh_rec);
    BgGrid_c->RegisterHM < ZeeDHistManagerBkgDscrZCF > (this->getName() + "/" + "BYMC_BG_def_c");
    this->AddMask("DEFAULT-IsEMNewFwdTightElecZ", BgGrid_c, "DEFAULT",doWeightNone); 

    ZeeDGridHistManager* BgGrid = new ZeeDGridHistManager(this->getName() + "/" + "BYMC_BG_def", TrippleGrid, &getAbsY_M_cosTh_rec);
    BgGrid->RegisterHM < ZeeDHistManagerBkgDscrZCF > (this->getName() + "/" + "BYMC_BG_def");
    this->AddMask("DEFAULT-IsEMNewFwdTightElecZ-IsEMTightPPCentralElecZ", BgGrid, "DEFAULT",doWeightNone); 

    
    //////////------EtMiss check--------//////////
    ZeeDGridHistManager* massGrid_etmiss = new ZeeDGridHistManager(this->getName() + "/" + "BM_EtMiss", BosMass, &getBosonMass);
    massGrid_etmiss->RegisterHM < ZeeDHistManagerEtmiss > (this->getName() + "/" + "BM_EtMiss");
    this->AddMaskLoose("DEFAULT", massGrid_etmiss); 
    
    ZeeDGridHistManager* massGrid_etmiss_bg = new ZeeDGridHistManager(this->getName() + "/" + "BM_EtMiss_bg_c", BosMass, &getBosonMass);
    massGrid_etmiss_bg->RegisterHM < ZeeDHistManagerEtmiss > (this->getName() + "/" + "BM_EtMiss_bg_c");
    this->AddMask("DEFAULT-IsEMNewFwdTightElecZ", massGrid_etmiss_bg, "DEFAULT",doWeightNone);   
  
  }
  
  if((*fAnaOptions)->WriteEWUnfoldNtuple() & (*fAnaOptions)->FillGenInfo() ){
    std::string fGenCuts = "ExistBornBoson+ExistTwoBornElectrons+PtMinBothBornElecZ+EtaExtCFBothBornElecZ+EtaCrackCentralBornElecZ+EtaCrackForwardBornElecZ";
    std::string fRecCuts = "DEFAULT";
  
    std::string fGenCutsM = "ExistBornBoson+ExistTwoBornElectrons+MassBornBoson+PtMinBothBornElecZ+EtaExtCFBothBornElecZ+EtaCrackCentralBornElecZ+EtaCrackForwardBornElecZ";
    std::string fRecCutsM = "DEFAULT";
  
    ZeeDHistManagerEWUnfoldWriter* EWUnfoldWriter1;
    EWUnfoldWriter1 = new ZeeDHistManagerEWUnfoldWriter(this->getName()+"/RecOnly/EWUnfold",
    ZeeDMisc::FindFile("Binning/BosYMass.txt"),
    getAbsY_M_cosTh_rec,
    getAbsY_M_cosTh_gen
    );
    EWUnfoldWriter1->SetRecoIsReconstructed(ZeeDHistManagerEWUnfoldWriter::yes);
    //EWUnfoldWriter1->UseSystematics(this->GetSystematics());
    this->AddMaskLoose(fRecCuts, EWUnfoldWriter1, doWeight);
    EWUnfoldWriter1->SetLevelMask(0);
    
    ZeeDHistManagerEWUnfoldWriter* EWUnfoldWriter2;
    EWUnfoldWriter2 = new ZeeDHistManagerEWUnfoldWriter(this->getName()+"/GenOnly/EWUnfold",
    ZeeDMisc::FindFile("Binning/BosYMass.txt"),
    getAbsY_M_cosTh_rec,
    getAbsY_M_cosTh_gen
    );
    EWUnfoldWriter2->SetTruthIsFiducial(ZeeDHistManagerEWUnfoldWriter::yes);
    //EWUnfoldWriter2->UseSystematics(this->GetSystematics());
    this->AddMaskLoose(fGenCuts, EWUnfoldWriter2, doWeightNone);
    EWUnfoldWriter2->SetLevelMask(0);

    
    ZeeDHistManagerEWUnfoldWriter* EWUnfoldWriter3;
    EWUnfoldWriter3 = new ZeeDHistManagerEWUnfoldWriter(this->getName()+"/GenRec/EWUnfold",
    ZeeDMisc::FindFile("Binning/BosYMass.txt"),
    getAbsY_M_cosTh_rec,
    getAbsY_M_cosTh_gen
    );
    EWUnfoldWriter3->SetRecoIsReconstructed(ZeeDHistManagerEWUnfoldWriter::yes);
    EWUnfoldWriter3->SetTruthIsFiducial(ZeeDHistManagerEWUnfoldWriter::yes);
    //EWUnfoldWriter3->UseSystematics(this->GetSystematics());
    this->AddMaskLoose(fRecCuts+"+"+fGenCuts,
    EWUnfoldWriter3,
    doWeight);
    EWUnfoldWriter3->SetLevelMask(0);
  
  // -----------------------
    ZeeDHistManagerEWUnfoldWriter* EWUnfoldWriterM;
    EWUnfoldWriterM = new ZeeDHistManagerEWUnfoldWriter(this->getName()+"/RecOnly/EWUnfold_peak",
    ZeeDMisc::FindFile("Binning/BosAbsY.txt"),
    getBosonAbsY,
    getBornBosonAbsY
    );
    EWUnfoldWriterM->SetRecoIsReconstructed(ZeeDHistManagerEWUnfoldWriter::yes);
    //EWUnfoldWriterM->UseSystematics(this->GetSystematics());
    this->AddMask(fRecCutsM, EWUnfoldWriterM, fRecCutsM, doWeight);
    EWUnfoldWriterM->SetLevelMask(0);
    
    ZeeDHistManagerEWUnfoldWriter* EWUnfoldWriter2M;
    EWUnfoldWriter2M = new ZeeDHistManagerEWUnfoldWriter(this->getName()+"/GenOnly/EWUnfold_peak",
    ZeeDMisc::FindFile("Binning/BosAbsY.txt"),
    getBosonAbsY,
    getBornBosonAbsY
    );
    EWUnfoldWriter2M->SetTruthIsFiducial(ZeeDHistManagerEWUnfoldWriter::yes);
    //EWUnfoldWriter2M->UseSystematics(this->GetSystematics());
    this->AddMaskLoose(fGenCutsM, EWUnfoldWriter2M, doWeightNone);
    EWUnfoldWriter2M->SetLevelMask(0);
    
    ZeeDHistManagerEWUnfoldWriter* EWUnfoldWriter3M;
    EWUnfoldWriter3M = new ZeeDHistManagerEWUnfoldWriter(this->getName()+"/GenRec/EWUnfold_peak",
    ZeeDMisc::FindFile("Binning/BosAbsY.txt"),
    getBosonAbsY,
    getBornBosonAbsY
    );
    EWUnfoldWriter3M->SetRecoIsReconstructed(ZeeDHistManagerEWUnfoldWriter::yes);
    EWUnfoldWriter3M->SetTruthIsFiducial(ZeeDHistManagerEWUnfoldWriter::yes);
    //EWUnfoldWriter3M->UseSystematics(this->GetSystematics());
    this->AddMaskLoose(fRecCutsM+"+"+fGenCutsM,
    EWUnfoldWriter3M,
    doWeight);
    EWUnfoldWriter3M->SetLevelMask(0);
  }

  if ((*fAnaOptions)->FillObservable()) {
    // Define for all grids:
    TString SignalGrid = "Binning/BosYMassCosTh.txt";
    TString BgGrid = "Binning/ElecIsolationFraction.txt";
    TString BaseName = this->getName() + "/" + "Obs_AbsY_M_cosTh";
    ZeeDQuantity<ZeeDArrayD> (*cVarREC)(const ZeeDEvent*);
    cVarREC = &getAbsY_M_cosTh_rec;
    ZeeDQuantity<ZeeDArrayD> (*cVarGEN)(const ZeeDEvent*);
    cVarGEN = &getAbsY_M_cosTh_gen;
    ZeeDQuantity<ZeeDArrayD> (*cVarGENbare)(const ZeeDEvent*);
    cVarGENbare = &getAbsY_M_cosTh_genBare;
    ZeeDQuantity<ZeeDArrayD> (*cVarGENdressed)(const ZeeDEvent*);
    cVarGENdressed = &getAbsY_M_cosTh_genDressed;
    ZeeDQuantity<ZeeDArrayD> (*cVarBG)(const ZeeDEvent*);
    cVarBG = &getIsoFracMax;

    Bool_t isMC = (*fAnaOptions)->IsMC();
    this->AddMaskLoose("DEFAULT", new ZeeDObservable(BaseName + "_RecCut", SignalGrid, BgGrid, cVarREC, cVarGEN, cVarBG, kTRUE, isMC, isMC));
    this->AddMaskLoose("NONE",    new ZeeDObservable(BaseName + "_NoCut", SignalGrid, BgGrid, cVarREC, cVarGEN, cVarBG, kTRUE, isMC, isMC), doWeightNone);

    if ((*fAnaOptions)->FillGenInfo()) {
      this->AddMaskLoose("DEFAULT+ExistBornBoson+ExistTwoBornElectrons+MassBornBoson+PtMinBothBornElecZ",
  	    new ZeeDObservable(BaseName + "_CombCut", SignalGrid, BgGrid, cVarREC, cVarGEN, cVarBG, kTRUE, isMC, isMC), doWeightNone);
  	  
      this->AddMaskLoose("DEFAULT+ExistBornBoson+ExistTwoBornElectrons+MassBornBoson+PtMinBothBornElecZ+EtaCrackCentralBornElecZ+EtaCrackForwardBornElecZ+EtaCFBothBornElecZ+EtaExtCFBothBornElecZ",
            new ZeeDObservable(BaseName + "_FiduCut", SignalGrid, BgGrid, cVarREC, cVarGEN, cVarBG, kTRUE, isMC, isMC), doWeightNone);
	    	    		    
    }
  }

  if ((*fAnaOptions)->FillCalibrator()) {
    // Calibrator:
    this->AddMaskLoose("DEFAULT", new ZeeDCalibratorHM(this->getName() + "/Calib"));
  }
}
