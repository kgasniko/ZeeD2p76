#include "ZeeDHistManager/ZeeDHistManagerQCDBkgW.h"

#include "ZeeDMisc/ZeeDMisc.h"

#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDElectron.h"

#include <algorithm>

//-----------------------------------------------------
ZeeDHistManagerQCDBkgW::ZeeDHistManagerQCDBkgW(TString name)
  : ZeeDHistManager(name),
    fSvcHelper(name),
    fAnaOptions(fSvcHelper.GetAnaSvc())
{}

//-----------------------------------------------------
void ZeeDHistManagerQCDBkgW::BookHistos()
{


	AddTH1D("etMiss",    ZeeDMisc::FindFile("Binning/etMiss.txt")   , "E_{T}^{miss} [GeV]",     "", ZeeDHistLevel::Systematics);
    AddTH1D("etMissFit",    ZeeDMisc::FindFile("Binning/etMiss.txt")   , "E_{T}^{miss} [GeV]",     "", ZeeDHistLevel::Systematics);
    AddTH1D("etMissBigRange",    ZeeDMisc::FindFile("Binning/etMissBigRange.txt")   , "E_{T}^{miss} [GeV]",     "");
    AddTH1D("etMissBigBin",    ZeeDMisc::FindFile("Binning/etMissBigBin.txt")   , "E_{T}^{miss} [GeV]",     "");
    AddTH1D("etMissSmallBin",    ZeeDMisc::FindFile("Binning/etMissSmallBin.txt")   , "E_{T}^{miss} [GeV]",     "");
	AddTH1D("etMissSmallRange",    ZeeDMisc::FindFile("Binning/etMissSmallRange.txt")   , "E_{T}^{miss} [GeV]",     "");


    AddTH1D("lepEta",   ZeeDMisc::FindFile("Binning/LepEta.txt"), "#eta^{lep}",          "");
    AddTH1D("lepPhi",   20,   -4.0,   4.0, "#varphi^{lep} [rad]", "");
    AddTH1D("dPhi" ,    20,   -4.0,   4.0, "d#varphi^{lep} [rad]", "");
    AddTH1D("lepPt",    ZeeDMisc::FindFile("Binning/LepPt.txt"), "P_{T}^{lep} [GeV]","" );
	AddTH1D("mtW",       ZeeDMisc::FindFile("Binning/BosMassTransv.txt"), "M_{T}^{W} [GeV]",         "" );
    AddTH1D("lepPtFit",    ZeeDMisc::FindFile("Binning/LepPt.txt"), "P_{T}^{lep} [GeV]","");
	AddTH1D("mtWFit",       ZeeDMisc::FindFile("Binning/BosMassTransvFit.txt"), "M_{T}^{W} [GeV]",         "", ZeeDHistLevel::Systematics );
    AddTH1D("ptW",            50,    0.0, 120.0, "P_{T}^{W} [GeV]",         "" );

    AddTH1D("ElecEtCone20overEt", 250,-1.,3.,"Et cone 20/ Et cluster","");
    AddTH1D("ElecEtCone30overEt", 250,-1.,3.,"Et cone 30/ Et cluster","");

    AddTH1D("ElecEtCone20", 250, -10.0, 100.,"Et cone 20","");
    AddTH1D("ElecEtCone30", 250, -10.0, 100.,"Et cone 30","");
    
    AddTH1D("ElecEtCone20_PtNPVCorrected", 250, -10.0, 100.,"Et cone 30","");
    AddTH1D("ElecEtCone30_PtNPVCorrected", 250, -10.0, 100.,"Et cone 30","");

    AddTH1D("sElecEtCone20_PtNPVCorrected", 250, -10.0, 100.,"Et cone 30","");
    AddTH1D("sElecEtCone30_PtNPVCorrected", 250, -10.0, 100.,"Et cone 30","");

    AddTH1D("ElecEtCone20_PtNPVCorrectedoverEt", 250,-1.,3.,"mean Et cone 20/ Et cluster","");
    AddTH1D("ElecEtCone30_PtNPVCorrectedoverEt", 250,-1.,3.,"mean Et cone 30/ Et cluster","");

    AddTH1D("sElecEtCone20_PtNPVCorrectedoverEt", 250,-1.,3.,"mean Et cone 20/ Et cluster","");
    AddTH1D("sElecEtCone30_PtNPVCorrectedoverEt", 250,-1.,3.,"mean Et cone 30/ Et cluster","");

	AddTH2D("etMissmtW", ZeeDMisc::FindFile("Binning/etMissBosMassTransv.txt"), "E_{T}^{miss} [GeV]", "M_{T}^{W} [GeV]");
	AddTH2D("etMisslepPt", ZeeDMisc::FindFile("Binning/etMissLepPt.txt"), "E_{T}^{miss} [GeV]", "P_{T}^{lep} [GeV]");
	AddTH2D("etMisslepEta", ZeeDMisc::FindFile("Binning/etMissLepEta.txt"), "E_{T}^{miss} [GeV]", "#eta^{lep}");
	AddTH2D("EtMisslepPhi", 50, 0.0, 100.0, 20, -4.0, 4.0,  "E_{T}^{miss} [GeV]", "#varphi^{lep} [rad]");
    AddTH2D("lepPtmtW", ZeeDMisc::FindFile("Binning/LepPtBosMassTransv.txt"), "P_{T}^{lep} [GeV]", "M_{T}^{W} [GeV]");	
  	AddTH2D("lepPtlepEta", ZeeDMisc::FindFile("Binning/LepPtLepEta.txt"), "P_{T}^{lep} [GeV]", "#eta^{lep}");
	//AddTH2D("lepPtlepPhi", , "P_{T}^{lep} [GeV]", 	"#varphi^{lep} [rad]");

    //--



}

//-----------------------------------------------------
void ZeeDHistManagerQCDBkgW::Fill()
{
    ServiceHandle<IZeeDAnalysisSvc>* ZeeDAnaOptions = fSvcHelper.GetAnaSvc();
 
    const ZeeDEvent* event  = GetEvent();
    const Double_t   Weight = event->GetWeight();

    const ZeeDBosonW*    boson = event->GetCurrentBosonW();
    const ZeeDLepton* lep = boson->GetLep();

    const TLorentzVector& boson_fourVec = boson->GetFourVector();
    const TLorentzVector& lep_fourVec = lep->GetFourVector();

    const Double_t etMiss = boson->GetEtMiss();
    FillTH1(etMiss, Weight, "etMiss");
    FillTH1(etMiss, Weight, "etMissFit");
        FillTH1(etMiss, Weight, "etMissBigRange");
   FillTH1(etMiss, Weight, "etMissBigBin");
   FillTH1(etMiss, Weight, "etMissSmallBin");
   FillTH1(etMiss, Weight, "etMissSmallRange");

    //-------------------------------------------------------------------------
    const Double_t boson_M = boson->GetMt();
    FillTH1(boson_M, Weight, "mtW");
    FillTH1(boson_M, Weight, "mtWFit");
	FillTH1(boson_fourVec.Pt(), Weight, "ptW");


    //-------------------------------------------------------------------------
    const Double_t lep_et = lep_fourVec.Et();
    const Double_t lep_pt = lep_fourVec.Pt();
	const Double_t lep_eta = lep_fourVec.Eta();
    const Double_t lep_phi = lep_fourVec.Phi();
	FillTH1(lep_pt, Weight, "lepPt");
    FillTH1(lep_pt, Weight, "lepPtFit");
	FillTH1(lep_phi, Weight, "lepPhi");
	FillTH1(lep_eta, Weight, "lepEta");
	FillTH2(etMiss, lep_eta, Weight, "etMisslepEta");
	FillTH2(etMiss, lep_pt, Weight, "etMisslepPt");
	FillTH2(etMiss, boson_M, Weight, "etMissmtW");
    FillTH2(etMiss, lep_phi, Weight, "etMisslepPhi");
	FillTH2(lep_pt, boson_M, Weight, "lepPtmtW");
	FillTH2(lep_pt, lep_eta, Weight, "lepPtlepEta");
	FillTH2(lep_pt, lep_phi, Weight, "lepPtlepPhi");	
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------


    //---


    //-------------------------------------------------------------------------

/*
    if(lep->Shower().IsSet() &&
       lep->CaloIsoCor().IsSet()){

        const ZeeDElectronBags::Shower elec1_shower = elec->Shower().Get();

	double elec1_etcone30 = elec1_shower.etcone30/1000.;

	const ZeeDElectronBags::CaloIsoCor elec1_CaloIsoCor = elec->CaloIsoCor().Get();

        //----------------------------------------------------------------------

        const Double_t elec1_etcone30_PtNPVCorrected = elec1_CaloIsoCor.etcone30_PtNPVCorrected/1000.;


        const Double_t elec1_etcone30_PtNPVCorrectedoverEt   =  elec1_etcone30_PtNPVCorrected/elec_et;

        const Double_t elec1_etcone30overEt   = elec1_etcone30/elec_et;

        FillTH1(elec1_etcone30_PtNPVCorrected, Weight, "ElecEtCone30_PtNPVCorrected");

        FillTH1(elec1_etcone30, Weight, "ElecEtCone30");

        FillTH1(elec1_etcone30overEt, Weight, "ElecEtCone30overEt");

        FillTH1(elec1_etcone30_PtNPVCorrectedoverEt, Weight, "ElecEtCone30_PtNPVCorrectedoverEt");
        
        if ((*ZeeDAnaOptions)->IsMC()) {
            const Double_t sElec1_etcone30_PtNPVCorrected = elec1_etcone30_PtNPVCorrected+0.4;
            const Double_t sElec1_etcone30_PtNPVCorrectedoverEt   =  sElec1_etcone30_PtNPVCorrected/elec_et;
            FillTH1(sElec1_etcone30_PtNPVCorrected, Weight, "sElecEtCone30_PtNPVCorrected");
            FillTH1(sElec1_etcone30_PtNPVCorrectedoverEt, Weight, "sElecEtCone30_PtNPVCorrectedoverEt");
            }


        //----------------------------------------------------------------------

        //----------------------------------------------------------------------

        const Double_t elec1_etcone20 = elec1_shower.etcone20/1000.;

        const Double_t elec1_etcone20_PtNPVCorrected = elec1_CaloIsoCor.etcone20_PtNPVCorrected/1000.;


        FillTH1(elec1_etcone20, Weight, "ElecEtCone20");


        FillTH1(elec1_etcone20_PtNPVCorrected, Weight, "ElecEtCone20_PtNPVCorrected");

        const Double_t elec1_etcone20overEt   = elec1_etcone20/elec_et;

        FillTH1(elec1_etcone20overEt, Weight, "ElecEtCone20overEt");
   
        const Double_t elec1_etcone20_PtNPVCorrectedoverEt =  elec1_etcone20_PtNPVCorrected/elec_et;

        FillTH1(elec1_etcone20_PtNPVCorrectedoverEt, Weight, "ElecEtCone30_PtNPVCorrectedoverEt");

        if ((*ZeeDAnaOptions)->IsMC()) {
            const Double_t sElec1_etcone20_PtNPVCorrected = elec1_etcone20_PtNPVCorrected+0.4;
            const Double_t sElec1_etcone20_PtNPVCorrectedoverEt   =  sElec1_etcone20_PtNPVCorrected/elec_et;
            FillTH1(sElec1_etcone20_PtNPVCorrected, Weight, "sElecEtCone20_PtNPVCorrected");
            FillTH1(sElec1_etcone20_PtNPVCorrectedoverEt, Weight, "sElecEtCone20_PtNPVCorrectedoverEt");
            }

    }
*/
}



