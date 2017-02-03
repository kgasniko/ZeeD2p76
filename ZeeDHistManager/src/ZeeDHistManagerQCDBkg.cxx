#include "ZeeDHistManager/ZeeDHistManagerQCDBkg.h"

#include "ZeeDMisc/ZeeDMisc.h"

#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDElectron.h"

#include <algorithm>

//-----------------------------------------------------
ZeeDHistManagerQCDBkg::ZeeDHistManagerQCDBkg(TString name)
  : ZeeDHistManager(name),
    fSvcHelper(name),
    fAnaOptions(fSvcHelper.GetAnaSvc())
{}

//-----------------------------------------------------
void ZeeDHistManagerQCDBkg::BookHistos()
{
    AddTH1D("QCDBosMass", 2000, 0, 1000, "M_{Z->ee} / GeV", "");

    AddTH2D("ElecEtaPtMinvsEtaPtMax", 200,  -5.0,  5.0, 200,  -5.0,  5.0, "", "", ZeeDHistLevel::Technical);
    AddTH2D("ElecPtMinvsEtaPtMin", 400,  0.0,  200, 200,  -5.0,  5.0, "", "", ZeeDHistLevel::Technical);
    AddTH2D("ElecPtMaxvsEtaPtMax", 800,  0.0,  400, 200,  -5.0,  5.0, "", "", ZeeDHistLevel::Technical);
    AddTH2D("ElecEtaPMinvsEtaPMax", 200,  -5.0,  5.0, 200,  -5.0,  5.0, "", "", ZeeDHistLevel::Technical);
    AddTH2D("ElecPMinvsEtaPMin", 400,  0.0,  200, 200,  -5.0,  5.0, "", "", ZeeDHistLevel::Technical);
    AddTH2D("ElecPMaxvsEtaPMax", 800,  0.0,  400, 200,  -5.0,  5.0, "", "", ZeeDHistLevel::Technical);

    AddTH1D("Elec12_deltaPhi", 100, -TMath::Pi(), TMath::Pi(), "", "");
    AddTH1D("Elec12_deltaPhi_abs", 100, 0, TMath::Pi(), "", "");

    AddTH1D("QCDElecPt", 400, 0, 200, "", "");
    AddTH1D("QCDElecPtMin", 400, 0, 200, "", "");
    AddTH1D("QCDElecPtMax", 800, 0, 400, "", "");

    AddTH1D("ElecPtEtCone20overEtMin", 400, 0, 200, "", "");
    AddTH1D("ElecPtEtCone20overEtMax", 400, 0, 200, "", "");

    AddTH1D("ElecPtEtCone30overEtMin", 400, 0, 200, "", "");
    AddTH1D("ElecPtEtCone30overEtMax", 400, 0, 200, "", "");

    AddTH1D("ElecEtCone20overEt", 4000,-1.,3.,"Et cone 20/ Et cluster","");
    AddTH1D("ElecEtCone30overEt", 4000,-1.,3.,"Et cone 30/ Et cluster","");

    AddTH1D("ElecEtCone20", 2200, -10, 100.,"Et cone 20","");
    AddTH1D("ElecEtCone30", 2200, -10, 100.,"Et cone 30","");

    AddTH1D("ElecEtCone20Min", 2200, -10, 100.,"Et cone 20","");
    AddTH1D("ElecEtCone30Min", 2200, -10, 100.,"Et cone 30","");

    AddTH1D("ElecEtCone20Max", 2200, -10, 100.,"Et cone 20","");
    AddTH1D("ElecEtCone30Max", 2200, -10, 100.,"Et cone 30","");

    AddTH1D("ElecEtCone20Mean", 2200, -10, 100.,"Et cone 30","");
    AddTH1D("ElecEtCone30Mean", 2200, -10, 100.,"Et cone 30","");

    AddTH1D("ElecEtCone20_PtNPVCorrectedMin", 2200, -10, 100.,"Et cone 20","");
    AddTH1D("ElecEtCone30_PtNPVCorrectedMin", 2200, -10, 100.,"Et cone 30","");

    AddTH1D("ElecEtCone20_PtNPVCorrectedMax", 2200, -10, 100.,"Et cone 20","");
    AddTH1D("ElecEtCone30_PtNPVCorrectedMax", 2200, -10, 100.,"Et cone 30","");

    AddTH1D("ElecEtCone20_PtNPVCorrectedMean", 2200, -10, 100.,"Et cone 30","");
    AddTH1D("ElecEtCone30_PtNPVCorrectedMean", 2200, -10, 100.,"Et cone 30","");

    AddTH1D("ElecEtCone20overEtMin", 4000,-1.,3.,"min. Et cone 20/ Et cluster","");
    AddTH1D("ElecEtCone30overEtMin", 4000,-1.,3.,"min. Et cone 30/ Et cluster","");

    AddTH1D("ElecEtCone20overEtMax", 4000,-1.,3.,"max. Et cone 20/ Et cluster","");
    AddTH1D("ElecEtCone30overEtMax", 4000,-1.,3.,"max. Et cone 30/ Et cluster","");

    AddTH1D("ElecEtCone20overEtMean", 4000,-1.,3.,"mean Et cone 20/ Et cluster","");
    AddTH1D("ElecEtCone30overEtMean", 4000,-1.,3.,"mean Et cone 30/ Et cluster","");

    //--

    AddTH1D("ElecEtCone20_PtNPVCorrectedoverEtMin", 4000,-1.,3.,"min. Et cone 20/ Et cluster","");
    AddTH1D("ElecEtCone30_PtNPVCorrectedoverEtMin", 4000,-1.,3.,"min. Et cone 30/ Et cluster","");

    AddTH1D("ElecEtCone20_PtNPVCorrectedoverEtMax", 4000,-1.,3.,"max. Et cone 20/ Et cluster","");
    AddTH1D("ElecEtCone30_PtNPVCorrectedoverEtMax", 4000,-1.,3.,"max. Et cone 30/ Et cluster","");

    AddTH1D("ElecEtCone20_PtNPVCorrectedoverEtMean", 4000,-1.,3.,"mean Et cone 20/ Et cluster","");
    AddTH1D("ElecEtCone30_PtNPVCorrectedoverEtMean", 4000,-1.,3.,"mean Et cone 30/ Et cluster","");

}

//-----------------------------------------------------
void ZeeDHistManagerQCDBkg::Fill()
{
    const ZeeDEvent* event  = GetEvent();
    const Double_t   Weight = event->GetWeight();

    const ZeeDBosonZ*    boson = event->GetCurrentBoson();
    const ZeeDElectron* elec1 =(ZeeDElectron*) boson->GetFirstLep();
    const ZeeDElectron* elec2 =(ZeeDElectron*) boson->GetSecondLep();

    const TLorentzVector& boson_fourVec = boson->GetFourVector();
    const TLorentzVector& elec1_fourVec = elec1->GetFourVector();
    const TLorentzVector& elec2_fourVec = elec2->GetFourVector();


    //-------------------------------------------------------------------------
    const Double_t boson_M = boson_fourVec.M();
    FillTH1(boson_M, Weight, "BosMass");


    //-------------------------------------------------------------------------
    const Double_t elec1_pt = elec1_fourVec.Pt();
    const Double_t elec2_pt = elec2_fourVec.Pt();
    const Double_t elec1_et = elec1_fourVec.Et();
    const Double_t elec2_et = elec2_fourVec.Et();
    FillTH1(elec1_pt, Weight, "ElecPt");
    FillTH1(elec2_pt, Weight, "ElecPt");

    const Double_t elec_pt_min = std::min(elec1_pt, elec2_pt);
    const Double_t elec_pt_max = std::max(elec1_pt, elec2_pt);
    FillTH1(elec_pt_min, Weight, "ElecPtMin");
    FillTH1(elec_pt_max, Weight, "ElecPtMax");
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    const Double_t elec1_p = TMath::Abs(elec1_fourVec.E());
    const Double_t elec2_p = TMath::Abs(elec2_fourVec.E());

    const Double_t elec_p_min = std::min(elec1_p, elec2_p);
    const Double_t elec_p_max = std::max(elec1_p, elec2_p);
    //-------------------------------------------------------------------------


    //---

    const Double_t elec1_eta = elec1->Cluster().Get().fourVector.Eta();
    const Double_t elec2_eta = elec2->Cluster().Get().fourVector.Eta();


    const Double_t elec_eta_pt_min = elec1_eta < elec2_eta? elec1_pt: elec2_pt;
    const Double_t elec_eta_pt_max = elec1_eta > elec2_eta? elec1_pt: elec2_pt;

    FillTH2(elec_eta_pt_min, elec_eta_pt_max, Weight, "ElecEtaPtMinvsEtaPtMax");

    FillTH2(elec_pt_min, elec_eta_pt_min, Weight, "ElecPtMinvsEtaPtMin");
    FillTH2(elec_pt_max, elec_eta_pt_max, Weight, "ElecPtMaxvsEtaPtMax");

    //---

    const Double_t elec_eta_p_min = elec1_eta < elec2_eta? elec1_p : elec2_p;
    const Double_t elec_eta_p_max = elec1_eta > elec2_eta? elec1_p : elec2_p;

    FillTH2(elec_eta_p_min, elec_eta_p_max, Weight, "ElecEtaPMinvsEtaPMax");

    FillTH2(elec_p_min, elec_eta_p_min, Weight, "ElecPMinvsEtaPMin");
    FillTH2(elec_p_max, elec_eta_p_max, Weight, "ElecPMaxvsEtaPMax");

    //-------------------------------------------------------------------------


    //-------------------------------------------------------------------------
    const Double_t elec12_deltaPhi = elec1_fourVec.DeltaPhi(elec2_fourVec);
    FillTH1(elec12_deltaPhi, Weight, "Elec12_deltaPhi");
    FillTH1(TMath::Pi()-TMath::Abs(elec12_deltaPhi), Weight, "Elec12_deltaPhi_abs");
    //-------------------------------------------------------------------------

    if(elec1->TrackMatch().IsSet() && elec2->TrackMatch().IsSet()) {

    }

    if(elec1->Shower().IsSet() && elec2->Shower().IsSet() &&
       elec1->CaloIsoCor().IsSet() && elec2->CaloIsoCor().IsSet()) {

        const ZeeDElectronBags::Shower elec1_shower = elec1->Shower().Get();
        const ZeeDElectronBags::Shower elec2_shower = elec2->Shower().Get();

	double elec1_etcone30 = elec1_shower.etcone30/1000.;
	double elec2_etcone30 = elec2_shower.etcone30/1000.;
	
	if ((*fAnaOptions)->IsMC()) {	// do it according to the 2011 year strategy
	  elec1_etcone30 += 0.4;
	  elec2_etcone30 += 0.4;
	}


	const ZeeDElectronBags::CaloIsoCor elec1_CaloIsoCor = elec1->CaloIsoCor().Get();
        const ZeeDElectronBags::CaloIsoCor elec2_CaloIsoCor = elec2->CaloIsoCor().Get();

        //----------------------------------------------------------------------

        const Double_t elec1_etcone30_PtNPVCorrected = elec1_CaloIsoCor.etcone30_PtNPVCorrected/1000.;
        const Double_t elec2_etcone30_PtNPVCorrected = elec2_CaloIsoCor.etcone30_PtNPVCorrected/1000.;

        const Double_t elec_etcone30min = std::min(elec1_etcone30, elec2_etcone30);
        const Double_t elec_etcone30max = std::max(elec1_etcone30, elec2_etcone30);

        const Double_t elec_etcone30_PtNPVCorrectedmin = std::min(elec1_etcone30_PtNPVCorrected, elec2_etcone30_PtNPVCorrected);
        const Double_t elec_etcone30_PtNPVCorrectedmax = std::max(elec1_etcone30_PtNPVCorrected, elec2_etcone30_PtNPVCorrected);

        const Double_t elec1_etcone30_PtNPVCorrectedoverEt   =  elec1_etcone30_PtNPVCorrected/elec1_et;
        const Double_t elec2_etcone30_PtNPVCorrectedoverEt   =  elec2_etcone30_PtNPVCorrected/elec2_et;

        const Double_t elec12_etcone30_PtNPVCorrectedoverEt = elec1_etcone30_PtNPVCorrectedoverEt + elec2_etcone30_PtNPVCorrectedoverEt;

        const Double_t elec_etcone30_PtNPVCorrectedoverEtmin = std::min(elec1_etcone30_PtNPVCorrectedoverEt,elec2_etcone30_PtNPVCorrectedoverEt);
        const Double_t elec_etcone30_PtNPVCorrectedoverEtmax = std::max(elec1_etcone30_PtNPVCorrectedoverEt,elec2_etcone30_PtNPVCorrectedoverEt);


        const Double_t elec12_etcone30 = elec1_etcone30 + elec2_etcone30;
        const Double_t elec12_etcone30_PtNPVCorrected = elec1_etcone30_PtNPVCorrected + elec2_etcone30_PtNPVCorrected;

        const Double_t elec1_etcone30overEt   = elec1_etcone30/elec1_et;
        const Double_t elec2_etcone30overEt   = elec2_etcone30/elec2_et;

        const Double_t elec12_etcone30overEt = elec1_etcone30overEt + elec2_etcone30overEt;

        const Double_t elec_etcone30overEtmin = std::min(elec1_etcone30overEt,elec2_etcone30overEt);
        const Double_t elec_etcone30overEtmax = std::max(elec1_etcone30overEt,elec2_etcone30overEt);

        FillTH1(elec_etcone30_PtNPVCorrectedmin, Weight, "ElecEtCone30_PtNPVCorrectedMin");
        FillTH1(elec_etcone30_PtNPVCorrectedmax, Weight, "ElecEtCone30_PtNPVCorrectedMax");

        FillTH1(elec1_etcone30_PtNPVCorrected, Weight, "ElecEtCone30_PtNPVCorrected");
        FillTH1(elec2_etcone30_PtNPVCorrected, Weight, "ElecEtCone30_PtNPVCorrected");

        FillTH1(elec12_etcone30_PtNPVCorrected*0.5, Weight, "ElecEtCone30_PtNPVCorrectedMean");


        FillTH1(elec1_etcone30, Weight, "ElecEtCone30");
        FillTH1(elec2_etcone30, Weight, "ElecEtCone30");

        FillTH1(elec_etcone30min, Weight, "ElecEtCone30Min");
        FillTH1(elec_etcone30max, Weight, "ElecEtCone30Max");

        FillTH1(elec12_etcone30*0.5, Weight, "ElecEtCone30Mean");

        FillTH1(elec1_etcone30overEt, Weight, "ElecEtCone30overEt");
        FillTH1(elec2_etcone30overEt, Weight, "ElecEtCone30overEt");
        FillTH1(elec12_etcone30overEt*0.5, Weight, "ElecEtCone30overEtMean");
        FillTH1(elec_etcone30overEtmin, Weight, "ElecEtCone30overEtMin");
        FillTH1(elec_etcone30overEtmax, Weight, "ElecEtCone30overEtMax");

        FillTH1(elec1_etcone30_PtNPVCorrectedoverEt, Weight, "ElecEtCone30_PtNPVCorrectedoverEt");
        FillTH1(elec2_etcone30_PtNPVCorrectedoverEt, Weight, "ElecEtCone30_PtNPVCorrectedoverEt");
        FillTH1(elec12_etcone30_PtNPVCorrectedoverEt*0.5, Weight, "ElecEtCone30_PtNPVCorrectedoverEtMean");


        FillTH1(elec_etcone30_PtNPVCorrectedoverEtmin, Weight, "ElecEtCone30_PtNPVCorrectedoverEtMin");



        FillTH1(elec_etcone30_PtNPVCorrectedoverEtmax, Weight, "ElecEtCone30_PtNPVCorrectedoverEtMax");

        //----------------------------------------------------------------------

        //----------------------------------------------------------------------

        const Double_t elec1_etcone20 = elec1_shower.etcone20/1000.;
        const Double_t elec2_etcone20 = elec2_shower.etcone20/1000.;

        const Double_t elec1_etcone20_PtNPVCorrected = elec1_CaloIsoCor.etcone20_PtNPVCorrected/1000.;
        const Double_t elec2_etcone20_PtNPVCorrected = elec2_CaloIsoCor.etcone20_PtNPVCorrected/1000.;

        const Double_t elec_etcone20min = std::min(elec1_etcone20, elec2_etcone20);
        const Double_t elec_etcone20max = std::max(elec1_etcone20, elec2_etcone20);

        const Double_t elec12_etcone20 = elec1_etcone20 + elec2_etcone20;

        FillTH1(elec_etcone20min, Weight, "ElecEtCone20Min");
        FillTH1(elec_etcone20max, Weight, "ElecEtCone20Max");

        FillTH1(elec1_etcone20, Weight, "ElecEtCone20");
        FillTH1(elec2_etcone20, Weight, "ElecEtCone20");

        FillTH1(elec12_etcone20*0.5, Weight, "ElecEtCone20Mean");

        const Double_t elec_etcone20_PtNPVCorrectedmin = std::min(elec1_etcone20_PtNPVCorrected, elec2_etcone20_PtNPVCorrected);
        const Double_t elec_etcone20_PtNPVCorrectedmax = std::max(elec1_etcone20_PtNPVCorrected, elec2_etcone20_PtNPVCorrected);

        const Double_t elec12_etcone20_PtNPVCorrected = elec1_etcone20_PtNPVCorrected + elec2_etcone20_PtNPVCorrected;

        FillTH1(elec_etcone20_PtNPVCorrectedmin, Weight, "ElecEtCone20_PtNPVCorrectedMin");
        FillTH1(elec_etcone20_PtNPVCorrectedmax, Weight, "ElecEtCone20_PtNPVCorrectedMax");

        FillTH1(elec1_etcone20_PtNPVCorrected, Weight, "ElecEtCone20_PtNPVCorrected");
        FillTH1(elec2_etcone20_PtNPVCorrected, Weight, "ElecEtCone20_PtNPVCorrected");

        FillTH1(elec12_etcone20_PtNPVCorrected*0.5, Weight, "ElecEtCone20_PtNPVCorrectedMean");

        const Double_t elec1_etcone20overEt   = elec1_etcone20/elec1_et;
        const Double_t elec2_etcone20overEt   = elec2_etcone20/elec2_et;
        const Double_t elec12_etcone20overEt  = elec1_etcone20overEt + elec2_etcone20overEt;
        const Double_t elec_etcone20overEtmin = std::min(elec1_etcone20overEt,elec2_etcone20overEt);
        const Double_t elec_etcone20overEtmax = std::max(elec1_etcone20overEt,elec2_etcone20overEt);

        FillTH1(elec1_etcone20overEt, Weight, "ElecEtCone20overEt");
        FillTH1(elec2_etcone20overEt, Weight, "ElecEtCone20overEt");
        FillTH1(elec12_etcone20overEt*0.5, Weight, "ElecEtCone20overEtMean");
        FillTH1(elec_etcone20overEtmin, Weight, "ElecEtCone20overEtMin");
        FillTH1(elec_etcone20overEtmax, Weight, "ElecEtCone20overEtMax");
        //----------------------------------------------------------------------

        //----------------------------------------------------------------------
        const Double_t elec_ptetcone20overEtmin = elec1_pt < elec2_pt? elec1_etcone20overEt: elec2_etcone20overEt;
        const Double_t elec_ptetcone20overEtmax = elec1_pt > elec2_pt? elec1_etcone20overEt: elec2_etcone20overEt;

        FillTH1(elec_ptetcone20overEtmin, Weight, "ElecPtEtCone20overEtMin");
        FillTH1(elec_ptetcone20overEtmax, Weight, "ElecPtEtCone20overEtMax");
        //----------------------------------------------------------------------

        //----------------------------------------------------------------------
        const Double_t elec_ptetcone30overEtmin = elec1_pt < elec2_pt? elec1_etcone30overEt: elec2_etcone30overEt;
        const Double_t elec_ptetcone30overEtmax = elec1_pt > elec2_pt? elec1_etcone30overEt: elec2_etcone30overEt;

        FillTH1(elec_ptetcone30overEtmin, Weight, "ElecPtEtCone30overEtMin");
        FillTH1(elec_ptetcone30overEtmax, Weight, "ElecPtEtCone30overEtMax");
        //----------------------------------------------------------------------


    }

}



