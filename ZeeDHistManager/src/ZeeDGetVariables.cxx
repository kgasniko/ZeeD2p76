#include "ZeeDHistManager/ZeeDGetVariables.h"

#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDArrays/ZeeDArrayD.h"
#include "ZeeDCalculator/ZeeDFunctions.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDGenParticle.h"


// Helper functions

//*-------------- Returns gen-level cosTheta
double CosThGen(const ZeeDEvent* event, ZeeDEnum::CosThStar::Value CosType, ZeeDEnum::MCFSRLevel::Value elecType)
{
    ZeeDGenElectrons elecpos = event->GetGenElectrons(elecType);

    if ( elecpos.first != NULL && elecpos.second != NULL) {
        const TLorentzVector &ele = elecpos.first->GetMCFourVector();
        const TLorentzVector &pos = elecpos.second->GetMCFourVector();

        return ZeeDFunctions::cosThStar(CosType,&pos, &ele);
    } else {
        return -100;
    }

}


//------------------------------------------------------
// returns [y, cosThetaEl] rec
//------------------------------------------------------
ZeeDQuantity<ZeeDArrayD> getYcosTh_rec(const ZeeDEvent* event)
{

  ZeeDQuantity<ZeeDArrayD> r;

  const ZeeDBosonZ* boson = event->GetCurrentBoson();

  if (boson == NULL) return r;

  ZeeDArrayD varYPtcosTh(2);

  const double cosThetaStar = boson->GetCosThStar(ZeeDEnum::CosThStar::Default);
  const TLorentzVector& fourVector = boson->GetFourVector();

  Double_t Y  = fourVector.Rapidity();

  varYPtcosTh.AddAt(Y, 0);
  varYPtcosTh.AddAt(cosThetaStar, 1);

  r.Set(varYPtcosTh);
  r.Lock();


  return r;

}


//------------------------------------------------------
// returns [y, Pt, cosThetaEl] rec
//------------------------------------------------------
ZeeDQuantity<ZeeDArrayD> getYPtcosTh_rec(const ZeeDEvent* event)
{

  ZeeDQuantity<ZeeDArrayD> r;

  const ZeeDBosonZ* boson = event->GetCurrentBoson();

  if (boson == NULL) return r; 

  ZeeDArrayD varYPtcosTh(3);

  const TLorentzVector& fourVector = boson->GetFourVector();
  // calculate y
  varYPtcosTh.AddAt(fourVector.Rapidity(), 0);

  // calculate pt
  varYPtcosTh.AddAt(fourVector.Pt(), 1);

  // calculate cos(theta1)
  Double_t cosTh =  boson->GetCosThStar(ZeeDEnum::CosThStar::Default);

  varYPtcosTh.AddAt(cosTh, 2);

  r.Set(varYPtcosTh);
  r.Lock();


  return r;
}

//------------------------------------------------------
// returns [y, cosThetaEl] gen
//------------------------------------------------------
ZeeDQuantity<ZeeDArrayD> getYcosTh_gen(const ZeeDEvent* event)
{

    ZeeDQuantity<ZeeDArrayD> r;

    const ZeeDGenParticle* boson = event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);

    if (boson != NULL) {

        ZeeDArrayD varYcosTh(2);

        const TLorentzVector& fourVector = boson->GetMCFourVector();
        // calculate y
        varYcosTh.AddAt(fourVector.Rapidity(), 0);
        varYcosTh.AddAt(CosThGen(event,ZeeDEnum::CosThStar::Default,ZeeDEnum::MCFSRLevel::Born),1);

        r.Set(varYcosTh);
        r.Lock();

    }

    return r;
}

// Get absY, M, cos th* at reconstructed level:

ZeeDQuantity<ZeeDArrayD> getAbsY_M_cosTh_rec(const ZeeDEvent* event)
{

  ZeeDQuantity<ZeeDArrayD> r;

  const ZeeDBosonZ *boson = event->GetCurrentBoson();
  if (boson == NULL) return r;

  ZeeDArrayD array(3);

  const TLorentzVector& fourVector = boson->GetFourVector();

  Double_t absY  = TMath::Abs(fourVector.Rapidity());
  Double_t mass  = fourVector.M();
  Double_t cosTh = boson->GetCosThStar(ZeeDEnum::CosThStar::Default);
  array.AddAt(absY,0);
  array.AddAt(mass,1);
  array.AddAt(cosTh,2);

  r.Set(array);
  r.Lock();

  return r;

}

ZeeDQuantity<ZeeDArrayD> getAbsY_M_Pt_rec(const ZeeDEvent* event)
{

    ZeeDQuantity<ZeeDArrayD> r;

    const ZeeDBosonZ *boson = event->GetCurrentBoson();
    if (boson != NULL) {

        ZeeDArrayD array(3);

        const TLorentzVector& fourVector = boson->GetFourVector();

        Double_t absY  = TMath::Abs(fourVector.Rapidity());
        Double_t mass  = fourVector.M();
        Double_t Pt =  fourVector.Pt();
        array.AddAt(absY,0);
        array.AddAt(mass,1);
        array.AddAt(Pt,2);

        r.Set(array);
        r.Lock();

    }

    return r;

}


ZeeDQuantity<ZeeDArrayD> getAbsY_M_cosThCS_rec(const ZeeDEvent* event)
{
  ZeeDArrayD array(3);

  const ZeeDBosonZ *boson = event->GetCurrentBoson();
  if (boson == NULL) return array;

  Double_t cosTh = boson->GetCosThStar(ZeeDEnum::CosThStar::CollinsSoper);
  Double_t absY  = TMath::Abs(boson->GetFourVector().Rapidity());
  Double_t mass  = boson->GetFourVector().M();
  array.AddAt(absY,0);
  array.AddAt(mass,1);
  array.AddAt(cosTh,2);


  return array;
}


// Get absY, M at reconstructed level:
ZeeDQuantity<ZeeDArrayD> getAbsY_M_rec(const ZeeDEvent* event)
{

    ZeeDQuantity<ZeeDArrayD> r;

    const ZeeDBosonZ *boson = event->GetCurrentBoson();

    if ( boson != NULL ) {

        ZeeDArrayD array(2);

        const TLorentzVector& fourVector = boson->GetFourVector();
        Double_t absY  = TMath::Abs(fourVector.Rapidity());
        Double_t mass  = fourVector.M();

        array.AddAt(absY,0);
        array.AddAt(mass,1);

        r.Set(array);
        r.Lock();

    }
    return r;

}

// Get absY, M, cos th* at generated level:
ZeeDQuantity<ZeeDArrayD> getAbsY_M_cosTh_gen(const ZeeDEvent* event)
{

    ZeeDQuantity<ZeeDArrayD> r;

    const ZeeDGenParticle *boson = event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);
    if (boson != NULL) {

        ZeeDArrayD array(3);

        const TLorentzVector& fourVector = boson->GetMCFourVector();

        Double_t absY  = TMath::Abs(fourVector.Rapidity());
        Double_t mass  = fourVector.M();
        Double_t cosTh = CosThGen(event,ZeeDEnum::CosThStar::Default,ZeeDEnum::MCFSRLevel::Default);

        array.AddAt(absY,0);
        array.AddAt(mass,1);
        array.AddAt(cosTh,2);

        r.Set(array);
        r.Lock();

    }
    return r;
}

ZeeDQuantity<ZeeDArrayD> getAbsY_M_Pt_gen(const ZeeDEvent* event)
{

    ZeeDQuantity<ZeeDArrayD> r;

    const ZeeDGenParticle *boson = event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);
    if (boson != NULL) {

        ZeeDArrayD array(3);

        const TLorentzVector& fourVector = boson->GetMCFourVector();

        Double_t absY  = TMath::Abs(fourVector.Rapidity());
        Double_t mass  = fourVector.M();
        Double_t Pt = fourVector.Pt();

        array.AddAt(absY,0);
        array.AddAt(mass,1);
        array.AddAt(Pt,2);

        r.Set(array);
        r.Lock();

    }
    return r;
}

// Get absY, M, cos th* at generated level, bare electrons:
ZeeDQuantity<ZeeDArrayD> getAbsY_M_cosTh_genBare(const ZeeDEvent* event)
{

    ZeeDQuantity<ZeeDArrayD> r;

    const ZeeDGenParticle *boson = event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);
    if (boson != NULL) {

        ZeeDArrayD array(3);

        const TLorentzVector& fourVector = boson->GetMCFourVector();

        Double_t absY  = TMath::Abs(fourVector.Rapidity());
        Double_t mass  = fourVector.M();
        Double_t cosTh = CosThGen(event,ZeeDEnum::CosThStar::Default,ZeeDEnum::MCFSRLevel::Bare);

        array.AddAt(absY,0);
        array.AddAt(mass,1);
        array.AddAt(cosTh,2);

        r.Set(array);
        r.Lock();

    }
    return r;
}

// Get absY, M, cos th* at generated level, bare electrons:
ZeeDQuantity<ZeeDArrayD> getAbsY_M_cosTh_genDressed(const ZeeDEvent* event)
{

    ZeeDQuantity<ZeeDArrayD> r;

    const ZeeDGenParticle *boson = event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);
    if (boson != NULL) {

        ZeeDArrayD array(3);

        const TLorentzVector& fourVector = boson->GetMCFourVector();

        Double_t absY  = TMath::Abs(fourVector.Rapidity());
        Double_t mass  = fourVector.M();
        Double_t cosTh = CosThGen(event,ZeeDEnum::CosThStar::Default,ZeeDEnum::MCFSRLevel::Dressed);

        array.AddAt(absY,0);
        array.AddAt(mass,1);
        array.AddAt(cosTh,2);

        r.Set(array);
        r.Lock();

    }
    return r;
}


//------------------------------------------------------
// returns [y, Pt, cosThetaEl] gen
//------------------------------------------------------
ZeeDQuantity<ZeeDArrayD> getYPtcosTh_gen(const ZeeDEvent* event)
{

    ZeeDQuantity<ZeeDArrayD> r;

    const ZeeDGenParticle* boson = event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);
    if (boson != NULL) {

        ZeeDArrayD varYcosTh(3);

        const TLorentzVector& fourVector = boson->GetMCFourVector();
        // calculate y
        varYcosTh.AddAt(fourVector.Rapidity(), 0);

        // calculate pt

        varYcosTh.AddAt(fourVector.Pt(), 1);

        Double_t cosTh = CosThGen(event,ZeeDEnum::CosThStar::Default,ZeeDEnum::MCFSRLevel::Default);

        varYcosTh.AddAt(cosTh, 2);

        r.Set(varYcosTh);
        r.Lock();

    }

    return r;
}

//------------------------------------------------------
// returns [Z mass] rec
//------------------------------------------------------
ZeeDQuantity<ZeeDArrayD> getBosonMass_bkg(const ZeeDEvent* event)
{

    ZeeDQuantity<ZeeDArrayD> r;

    const ZeeDBosonZ* boson = event->GetCurrentBoson();
    if (boson != NULL) {

        ZeeDArrayD varBkg(1);
        varBkg.AddAt((boson->GetFourVector()).M(), 0);

        r.Set(varBkg);
        r.Lock();

    }

    return r;
}


//------------------------------------------------------
// returns track isolation in 0.3 cone / Et
//------------------------------------------------------
ZeeDQuantity<ZeeDArrayD> getIsoFracMax(const ZeeDEvent* event)
{

    ZeeDQuantity<ZeeDArrayD> r;

    const ZeeDBosonZ* boson = event->GetCurrentBoson();

    if (boson != NULL) {

        ZeeDArrayD array(1);

        const ZeeDElectron* e1 = (ZeeDElectron*)boson->GetFirstLep(); 
        const ZeeDElectron* e2 = (ZeeDElectron*)boson->GetSecondLep();

	if (e1 != NULL && e2 != NULL) {
	  Double_t et1 = e1->GetFourVector().Et();
	  Double_t et2 = e2->GetFourVector().Et();

	  Double_t rat1 = (e1->Shower().IsSet()) ? e1->Shower().Get().ptcone30/et1*0.001 : 0;
	  Double_t rat2 = (e2->Shower().IsSet()) ? e2->Shower().Get().ptcone30/et2*0.001 : 0;

	  // Retrun max of the two
	  Double_t rmax = (rat1 > rat2) ? rat1 : rat2 ;
	  array.AddAt(rmax,0);

        r.Set(array);
        r.Lock();
	}

    }

    return r;
}

//------------------------------------------------------
// returns Z mass rec
//------------------------------------------------------
ZeeDQuantity<ZeeDArrayD> getBosonMass(const ZeeDEvent* event)
{

    ZeeDQuantity<ZeeDArrayD> r;

    const ZeeDBosonZ* boson = event->GetCurrentBoson();
    if (boson != NULL) {

        ZeeDArrayD array(1);
        array.AddAt((boson->GetFourVector()).M(),0);

        r.Set(array);
        r.Lock();


    }

    return r;

}


//------------------------------------------------------
// returns Z cosTheta rec
//------------------------------------------------------
ZeeDQuantity<ZeeDArrayD> getBosonCosTheta(const ZeeDEvent* event)
{

  ZeeDQuantity<ZeeDArrayD> r;

  const ZeeDBosonZ* boson = event->GetCurrentBoson();

  if (boson == NULL) return r;

  ZeeDArrayD varCosTh(1);

  Double_t cosTh = boson->GetCosThStar(ZeeDEnum::CosThStar::Default);

  varCosTh.AddAt(cosTh, 0);

  r.Set(varCosTh);
  r.Lock();


  return r;

}


ZeeDQuantity<ZeeDArrayD> getElecPtMax(const ZeeDEvent* event){

    ZeeDQuantity<ZeeDArrayD> r;

    const ZeeDBosonZ* boson = event->GetCurrentBoson();
		
    if (boson != NULL) {
        ZeeDArrayD array(1);
		
        const ZeeDLepton* e1 = boson->GetFirstLep();

        Double_t pt1 = e1->GetFourVector().Pt();

        // Retrun max of the two
        array.AddAt(pt1, 0);

        r.Set(array);
        r.Lock();

    }
    return r;
}


ZeeDQuantity<ZeeDArrayD> getElecPtMin(const ZeeDEvent* event){

    ZeeDQuantity<ZeeDArrayD> r;

    const ZeeDBosonZ* boson = event->GetCurrentBoson();
		
    if (boson != NULL) {
        ZeeDArrayD array(1);
		
        const ZeeDLepton* e2 = boson->GetSecondLep();

        Double_t pt2 = e2->GetFourVector().Pt();

        // Retrun max of the two
        array.AddAt(pt2, 0);

        r.Set(array);
        r.Lock();

    }
    return r;
}


ZeeDQuantity<ZeeDArrayD> getBosonPt(const ZeeDEvent* event)
{

    ZeeDQuantity<ZeeDArrayD> r;

    const ZeeDBosonZ* boson = event->GetCurrentBoson();
    if (boson != NULL) {

        ZeeDArrayD array(1);
        array.AddAt((boson->GetFourVector()).Pt(),0);

        r.Set(array);
        r.Lock();


    }

    return r;

}

//------------------------------------------------------
// returns Z absolute rapidity
//------------------------------------------------------
ZeeDQuantity<ZeeDArrayD> getBosonAbsY(const ZeeDEvent* event)
{

    ZeeDQuantity<ZeeDArrayD> r;

    const ZeeDBosonZ* boson = event->GetCurrentBoson();
    if (boson != NULL) {

        ZeeDArrayD array(1);

        array.AddAt(TMath::Abs((boson->GetFourVector()).Rapidity()),0);

        r.Set(array);
        r.Lock();
    }

    return r;

}

//------------------------------------------------------
// returns Z rapidity
//------------------------------------------------------
ZeeDQuantity<ZeeDArrayD> getBosonY(const ZeeDEvent* event)
{

    ZeeDQuantity<ZeeDArrayD> r;



    const ZeeDBosonZ* boson = event->GetCurrentBoson();
    if (boson != NULL) {

        ZeeDArrayD array(1);

        array.AddAt((boson->GetFourVector()).Rapidity(),0);

        r.Set(array);
        r.Lock();

    }

    return r;
}

//------------------------------------------------------
// returns Z rapidity, Mass
//------------------------------------------------------
ZeeDQuantity<ZeeDArrayD> getBosonY_Mass(const ZeeDEvent* event)
{

    ZeeDQuantity<ZeeDArrayD> r;

    const ZeeDBosonZ* boson = event->GetCurrentBoson();
    if (boson != NULL) {
        ZeeDArrayD array(2);
        array.AddAt((boson->GetFourVector()).Rapidity(),0);
        array.AddAt((boson->GetFourVector()).M(),1);

        r.Set(array);
        r.Lock();

    }


    return r;

}

//------------------------------------------------------
// returns Z absolute rapidity of born boson
//------------------------------------------------------
ZeeDQuantity<ZeeDArrayD> getBornBosonAbsY(const ZeeDEvent* event)
{

    ZeeDQuantity<ZeeDArrayD> r;

    const ZeeDGenParticle* boson = event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);
    if (boson != NULL) {
    ZeeDArrayD array(1);
        array.AddAt(TMath::Abs((boson->GetMCFourVector()).Rapidity()),0);

        r.Set(array);
        r.Lock();
    }
    return r;

}

//------------------------------------------------------
// returns Z mass of born boson
//------------------------------------------------------
ZeeDQuantity<ZeeDArrayD> getBornBosonMass(const ZeeDEvent* event)
{

    ZeeDQuantity<ZeeDArrayD> r;

    const ZeeDGenParticle* boson = event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);
    if (boson != NULL) {
        ZeeDArrayD array(1);
        array.AddAt(TMath::Abs((boson->GetMCFourVector()).M()),0);
        r.Set(array);
        r.Lock();
    }
    return r;

}
