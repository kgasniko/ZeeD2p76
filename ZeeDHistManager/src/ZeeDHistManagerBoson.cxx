#include "ZeeDHistManager/ZeeDHistManagerBoson.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>
#include <math.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDBosonW.h"

#include "ZeeDTools/ZeeDBags.h"
#include "ZeeDTools/ZeeDSystematics.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include "ZeeDMisc/ZeeDMisc.h"

using std::cout;
using std::endl;


//-----------------------------------------------------
void ZeeDHistManagerBoson::BookHistos()
{
    // Books histograms

    // Do we run on data or MC?
    bIsMC = (*fAnaOptions)->IsMC();

    // basic distributions
    AddTH1D("BosPt",		50,	 0., 100.,	"P_{T}^{bos} [GeV]",	"");
    AddTH1D("BosY",		    20,	-3.,   3.,	"y",		"", ZeeDHistLevel::ToyMC);
    AddTH1D("BosAbsY",		20,	 0.,   3.,	"|y|",		"");
    AddTH1D("BosMass",		50,	 50., 120.,	"M^{Z} [GeV]",	"", ZeeDHistLevel::Systematics);
    AddTH1D("BosCosThStar",	50,	-1.,   1.,	"cos(#Theta *)",	"");
    AddTH1D("BosXVtx",		30,	-0.1,   0.1,	"X_{vtx}^{bos}",			"");
    AddTH1D("BosYVtx",		30,	-0.1,   0.1,	"Y_{vtx}^{bos}",			"");
    AddTH1D("BosZVtx",		100,    -20., 20.,	"Z_{vtx}^{bos}",			"");
    AddTH1D("BosCharge",	5,     -2.5,  2.5,	"BosCharge",		"");
    AddTH1D("mtW",     ZeeDMisc::FindFile("Binning/BosMassTransv.txt"), "M_{T}^{W} [GeV]",         "");
    AddTH1D("Elec12_deltaPhi",	100, -TMath::Pi(), TMath::Pi(), "#Delta #Phi_{elec12}", "");

    // cosTheta*
    AddTH1D("BosCosThStarCS",  20, -1., 1., "Cos(#Theta *)_{CS}",  "", ZeeDHistLevel::Analysis);
    AddTH1D("BosCosThStarZFD", 20, -1., 1., "Cos(#Theta *)_{ZFD}", "", ZeeDHistLevel::Analysis);
    AddTH1D("BosCosThStarBD",  20, -1., 1., "Cos(#Theta *)_{BD}",  "", ZeeDHistLevel::Analysis);

    AddTH2D("BosCosThStarCSvsZFD", 20, -1., 1., 20, -1., 1., "Cos(#Theta *)_{CS}",  "Cos(#Theta *)_{ZFD}", ZeeDHistLevel::Technical);
    AddTH2D("BosCosThStarCSvsBD",  20, -1., 1., 20, -1., 1., "Cos(#Theta *)_{CS}",  "Cos(#Theta *)_{BD}",  ZeeDHistLevel::Technical);
    AddTH2D("BosCosThStarZFDvsBD", 20, -1., 1., 20, -1., 1., "Cos(#Theta *)_{ZFD}", "Cos(#Theta *)_{BD}",  ZeeDHistLevel::Technical);
    AddTH1D("uDivPt", 50, -1, 2, "u_{||}/p_{T}^{leplep}" , "");
    AddTH1D("uMinPt", 50, -20, 20, "u_{||}-p_{T}^{leplep} [GeV]" , "");
    AddTH1D("uPar", 50, 0, 100., "u_{||} [GeV]", "");
    AddTH1D("uPerp", 50, -15, 15, "u_{#perp} [GeV]", ""); 
}

//-----------------------------------------------------
void ZeeDHistManagerBoson::Fill()
{
  // Fill HM for the selected boson only
  FillBosonHists(GetEvent()->GetCurrentBoson());

}

//-----------------------------------------------------
void ZeeDHistManagerBoson::FillBosonHists(const ZeeDBoson* boson)
{
    // Fill histograms for a single boson
    if (boson == NULL) {
        return;
    }

    // Event weight
    Double_t Weight = GetEvent()->GetWeight();


    // kinematics
    const TLorentzVector& fourVector = boson->GetFourVector();
    const double bosMass = fourVector.M();
    
    const ZeeDEvent* event= GetEvent();
    ZeeDEtMiss* etMiss=(ZeeDEtMiss*)event->GetEtMissArray()->At(0);
    double sf=(*fAnaOptions)->EtScaleFactor();
    if ((*fAnaOptions)->IsData()){
        sf=1.0;
    }
    

    TVector3 lCorRecoil3= TVector3(etMiss->GetCorRecoilEtX(), etMiss->GetCorRecoilEtY(), 0);
    TVector3 bosV3=TVector3(fourVector.Px(), fourVector.Py(), 0);
    bosV3.SetMag(1);
    //std::cout << etMiss->GetCorRecoilEt() << " "<< fourVector.Pt() << std::endl;
    double uPar = lCorRecoil3.Dot(bosV3);
    bosV3.SetMag(uPar);
    TVector3 uPerpV = lCorRecoil3-bosV3;
    //double angl = uPerpV.Angle(bosV3);
    //double uPerp=lCorRecoil3.Perp(bosV3);
    double uPerp= uPerpV.Mag();
    if (uPerpV.DeltaPhi(bosV3)<0){
        uPerp*=-1;
    }
    //std::cout << uPerp << std::endl;
    //std::cout << angl <<  uPerpV.DeltaPhi(bosV3) << std::endl;
    //std::cout << uPar << " " << lCorRecoil3.Mag() << " "<<bosV3.Mag() << std::endl;
    //std::cout << uPar/bosV3.Mag() << std::endl << std::endl;
    FillTH1(-uPar/fourVector.Pt(), Weight, "uDivPt");
    FillTH1(uPerp, Weight, "uPerp");
    FillTH1(uPar+fourVector.Pt(), Weight, "uMinPt");
    FillTH1(-uPar, Weight, "uPar");

    FillTH1(bosMass, Weight, "BosMass");
    FillTH1(fourVector.Pt(), Weight, "BosPt");
    FillTH1(fourVector.Rapidity(), Weight, "BosY");
    //FillTH1(TMath::Abs(fourVector.Rapidity()), Weight, "BosAbsY");

    TVector3 Vtx = boson->GetVertex();
    FillTH1(Vtx.Px(), Weight, "BosXVtx");
    FillTH1(Vtx.Py(), Weight, "BosYVtx");
    FillTH1(Vtx.Pz(), Weight, "BosZVtx");

    FillTH1(boson->GetCharge(), Weight, "BosCharge");

    FillTH1(boson->GetCosThStar(ZeeDEnum::CosThStar::Default), Weight, "BosCosThStar");

/*
    if (boson::BosonType::Value->GetBosonType() == BosonType::Value::Zee ) {
       const ZeeDElectron* e1 =(ZeeDBosonZ*) boson->GetFirstElec();
       const ZeeDElectron* e2 =(ZeeDBosonZ*) boson->GetSecondElec();

       const double elec12_deltaPhi = (e1->GetFourVector()).DeltaPhi(e2->GetFourVector());
       FillTH1(elec12_deltaPhi, Weight, "Elec12_deltaPhi");
    }
*/
    // cosTheta*
    const Double_t cosThStarZFD = boson->GetCosThStar(ZeeDEnum::CosThStar::ZFlightDirection);
    const Double_t cosThStarCS  = boson->GetCosThStar(ZeeDEnum::CosThStar::CollinsSoper);
    const Double_t cosThStarBD  = boson->GetCosThStar(ZeeDEnum::CosThStar::BeamDirection);

    FillTH1(cosThStarZFD, Weight, "BosCosThStarZFD");
    FillTH1(cosThStarCS,  Weight, "BosCosThStarCS");
    FillTH1(cosThStarBD,  Weight, "BosCosThStarBD");
    FillTH2(cosThStarCS,  cosThStarZFD, Weight, "BosCosThStarCSvsZFD");
    FillTH2(cosThStarCS,  cosThStarBD, Weight, "BosCosThStarCSvsBD");
    FillTH2(cosThStarZFD, cosThStarBD, Weight, "BosCosThStarZFDvsBD");

}

