#include "ZeeDHistManager/ZeeDHistManagerQCDBkgZCCee.h"

#include "ZeeDMisc/ZeeDMisc.h"

#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDElectron.h"

#include <algorithm>

//-----------------------------------------------------
  ZeeDHistManagerQCDBkgZCCee::ZeeDHistManagerQCDBkgZCCee(TString name)
: ZeeDHistManager(name),
  fSvcHelper(name),
  fAnaOptions(fSvcHelper.GetAnaSvc())
{}

//-----------------------------------------------------
void ZeeDHistManagerQCDBkgZCCee::BookHistos()
{

  // book all the hists

  AddTH1D("Zmass", 160., 40., 200., "Zmass", "");


  // _rXXb stands for "etconeXX/etcluster is large"
  // _rXXg ----> etconeXX/etcluster is small
  // _ib ----> isolation (Medium++) not passed
  // _ig ----> isolation (Medium++) passed

  AddTH1D("ElecEtCluster_r20g", 100,  0., 80., "Et cluser","small etcone20/et");
  AddTH1D("ElecEtCluster_r20b", 100,  0., 80., "Et cluster","large etcone20/et");
  AddTH1D("ElecEtCluster_r30g", 100,  0., 80., "Et cluser","small etcone30/et");
  AddTH1D("ElecEtCluster_r30b", 100,  0., 80., "Et cluster","large etcone30/et");
  AddTH1D("ElecEtCluster_ig",   100,  0., 80., "Et cluster","passed Medium++");
  AddTH1D("ElecEtCluster_ib",   100,  0., 80., "Et cluster","not passed Medium++");

  AddTH1D("ElecPt_r20g",	100,  0., 100.,"Pt elec","small etcone20/et");
  AddTH1D("ElecPt_r20b",	100,  0., 100.,"Pt elec","large etcone20/et");
  AddTH1D("ElecPt_r30g",	100,  0., 100.,"Pt elec","small etcone30/et");
  AddTH1D("ElecPt_r30b",	100,  0., 100.,"Pt elec","large etcone30/et");
  AddTH1D("ElecPt_ig",		100,  0., 100.,"Pt elec","passed Medium++");
  AddTH1D("ElecPt_ib",		100,  0., 100.,"Pt elec","not passed Medium++");

  AddTH1D("ElecEtCone20_r20g", 1000, -10., 40.,"Et cone 20","small etcone20/et");
  AddTH1D("ElecEtCone20_r20b", 1000, -10., 40.,"Et cone 20","large etcone20/et");
  AddTH1D("ElecEtCone20_ig",   1000, -10., 40.,"Et cone 20","passed Medium++");
  AddTH1D("ElecEtCone20_ib",   1000, -10., 40.,"Et cone 20","not passed Medium++");

  AddTH1D("ElecEtCone20overEt_r20g", 400, -0.5, 1.5,"Et cone 20 / Et cluster","small etcone20/et");
  AddTH1D("ElecEtCone20overEt_r20b", 400, -0.5, 1.5,"Et cone 20 / Et cluster","large etcone20/et");
  AddTH1D("ElecEtCone20overEt_ig",   400, -0.5, 1.5,"Et cone 20 / Et cluster","passed Medium++");
  AddTH1D("ElecEtCone20overEt_ib",   400, -0.5, 1.5,"Et cone 20 / Et cluster","not passed Medium++");

  AddTH1D("ElecEtCone30_r30g", 1000, -10., 40.,"Et cone 30","small etcone30/et");
  AddTH1D("ElecEtCone30_r30b", 1000, -10., 40.,"Et cone 30","large etcone30/et");
  AddTH1D("ElecEtCone30_ig",   1000, -10., 40.,"Et cone 30","passed Medium++");
  AddTH1D("ElecEtCone30_ib",   1000, -10., 40.,"Et cone 30","not passed Medium++");

  AddTH1D("ElecEtCone30overEt_r30g", 400, -0.5, 1.5,"Et cone 30 / Et cluster","small etcone30/et");
  AddTH1D("ElecEtCone30overEt_r30b", 400, -0.5, 1.5,"Et cone 30 / Et cluster","large etcone30/et");
  AddTH1D("ElecEtCone30overEt_ig",   400, -0.5, 1.5,"Et cone 30 / Et cluster","passed Medium++");
  AddTH1D("ElecEtCone30overEt_ib",   400, -0.5, 1.5,"Et cone 30 / Et cluster","not passed Medium++");

}

//-----------------------------------------------------
void ZeeDHistManagerQCDBkgZCCee::Fill()
{
  const ZeeDEvent* event  = GetEvent();
  const double Weight = event->GetWeight();

  const ZeeDBosonZ*    boson = event->GetCurrentBoson();

  if (boson == NULL) return;

  FillTH1(boson->GetFourVector().M(), Weight, "Zmass");

  // by default boson keeps pt-ordered electrons, see ZeeDBosonZ.h/ZeeDCalculateZ.cxx
  const ZeeDElectron* ptmax_electron = (ZeeDElectron*)boson->GetFirstLep();	// pt max
  const ZeeDElectron* ptmin_electron = (ZeeDElectron*)boson->GetSecondLep();	// pt min


  //-------------------------------------------------------------------------
  if(ptmin_electron->Shower().IsSet() && ptmax_electron->Shower().IsSet() &&
      ptmin_electron->Cluster().IsSet()  && ptmin_electron->Cluster().IsSet()) {

    const double ptmax_pt = ptmax_electron->GetFourVector().Pt();
    const double ptmin_pt = ptmin_electron->GetFourVector().Pt();
    const double ptmax_etclus = ptmax_electron->Cluster().Get().fourVector.Et();
    const double ptmin_etclus = ptmin_electron->Cluster().Get().fourVector.Et();

    const ZeeDElectronBags::Shower ptmax_shower = ptmax_electron->Shower().Get();
    const ZeeDElectronBags::Shower ptmin_shower = ptmin_electron->Shower().Get();

    double ptmax_etcone20 = ptmax_shower.etcone20/1000.;
    double ptmin_etcone20 = ptmin_shower.etcone20/1000.;
    double ptmax_etcone30 = ptmax_shower.etcone30/1000.;
    double ptmin_etcone30 = ptmin_shower.etcone30/1000.;

    if ((*fAnaOptions)->IsMC()) {	// do it according to the 2011 year strategy
      ptmax_etcone30 += 0.4;
      ptmin_etcone30 += 0.4;
      ptmax_etcone20 += 0.4;
      ptmin_etcone20 += 0.4;
    }

    double ptmax_etcone20et = ptmax_etcone20 / ptmax_etclus;
    double ptmin_etcone20et = ptmin_etcone20 / ptmin_etclus;
    double ptmax_etcone30et = ptmax_etcone30 / ptmax_etclus;
    double ptmin_etcone30et = ptmin_etcone30 / ptmin_etclus;

    if (ptmax_etcone20et > ptmin_etcone20et) {  // case of ptmax_elec has "bad cluster" --> "jet", ptmin_elec --> "elec"

      FillTH1(ptmax_pt, Weight, "ElecPt_r20b");
      FillTH1(ptmin_pt, Weight, "ElecPt_r20g");
      FillTH1(ptmax_etclus, Weight, "ElecEtCluster_r20b");
      FillTH1(ptmin_etclus, Weight, "ElecEtCluster_r20g");
      FillTH1(ptmax_etcone20, Weight, "ElecEtCone20_r20b");
      FillTH1(ptmin_etcone20, Weight, "ElecEtCone20_r20g");
      FillTH1(ptmax_etcone20et, Weight, "ElecEtCone20overEt_r20b");
      FillTH1(ptmin_etcone20et, Weight, "ElecEtCone20overEt_r20g");
    } else {

      FillTH1(ptmax_pt, Weight, "ElecPt_r20g");
      FillTH1(ptmin_pt, Weight, "ElecPt_r20b");
      FillTH1(ptmin_etclus, Weight, "ElecEtCluster_r20b");
      FillTH1(ptmax_etclus, Weight, "ElecEtCluster_r20g");
      FillTH1(ptmin_etcone20, Weight, "ElecEtCone20_r20b");
      FillTH1(ptmax_etcone20, Weight, "ElecEtCone20_r20g");
      FillTH1(ptmin_etcone20et, Weight, "ElecEtCone20overEt_r20b");
      FillTH1(ptmax_etcone20et, Weight, "ElecEtCone20overEt_r20g");
    }

    if (ptmax_etcone30et > ptmin_etcone30et) {  // case of ptmax_elec has "bad cluster" --> "jet", ptmin_elec --> "elec"

      FillTH1(ptmax_pt, Weight, "ElecPt_r30b");
      FillTH1(ptmin_pt, Weight, "ElecPt_r30g");
      FillTH1(ptmax_etclus, Weight, "ElecEtCluster_r30b");
      FillTH1(ptmin_etclus, Weight, "ElecEtCluster_r30g");
      FillTH1(ptmax_etcone30, Weight, "ElecEtCone30_r30b");
      FillTH1(ptmin_etcone30, Weight, "ElecEtCone30_r30g");
      FillTH1(ptmax_etcone30et, Weight, "ElecEtCone20overEt_r30b");
      FillTH1(ptmin_etcone30et, Weight, "ElecEtCone20overEt_r30g");
    } else {

      FillTH1(ptmax_pt, Weight, "ElecPt_r30g");
      FillTH1(ptmin_pt, Weight, "ElecPt_r30b");
      FillTH1(ptmin_etclus, Weight, "ElecEtCluster_r30b");
      FillTH1(ptmax_etclus, Weight, "ElecEtCluster_r30g");
      FillTH1(ptmin_etcone30, Weight, "ElecEtCone30_r30b");
      FillTH1(ptmax_etcone30, Weight, "ElecEtCone30_r30g");
      FillTH1(ptmin_etcone30et, Weight, "ElecEtCone30overEt_r30b");
      FillTH1(ptmax_etcone30et, Weight, "ElecEtCone30overEt_r30g");
    }

    bool ptmax_passed = ptmax_electron->IsEM().IsSet() && (ptmax_electron->IsEM().Get().isMediumPP == kTRUE);
    bool ptmin_passed = ptmin_electron->IsEM().IsSet() && (ptmin_electron->IsEM().Get().isMediumPP == kTRUE);

    if (ptmax_passed) {	// case of ptmax_elec passed Medium++ cut

      FillTH1(ptmax_pt, Weight, "ElecPt_ig");
      FillTH1(ptmax_etclus, Weight, "ElecEtCluster_ig");
      FillTH1(ptmax_etcone20, Weight, "ElecEtCone20_ig");
      FillTH1(ptmax_etcone30, Weight, "ElecEtCone30_ig");
      FillTH1(ptmax_etcone20et, Weight, "ElecEtCone20overEt_ig");
      FillTH1(ptmax_etcone30et, Weight, "ElecEtCone30overEt_ig");
    } else {

      FillTH1(ptmax_pt, Weight, "ElecPt_ib");
      FillTH1(ptmax_etclus, Weight, "ElecEtCluster_ib");
      FillTH1(ptmax_etcone20, Weight, "ElecEtCone20_ib");
      FillTH1(ptmax_etcone30, Weight, "ElecEtCone30_ib");
      FillTH1(ptmax_etcone20et, Weight, "ElecEtCone20overEt_ib");
      FillTH1(ptmax_etcone30et, Weight, "ElecEtCone30overEt_ib");
    }

    if (ptmin_passed) {	// case of ptmax_elec passed Medium++ cut

      FillTH1(ptmin_pt, Weight, "ElecPt_ig");
      FillTH1(ptmin_etclus, Weight, "ElecEtCluster_ig");
      FillTH1(ptmin_etcone20, Weight, "ElecEtCone20_ig");
      FillTH1(ptmin_etcone30, Weight, "ElecEtCone30_ig");
      FillTH1(ptmin_etcone20et, Weight, "ElecEtCone20overEt_ig");
      FillTH1(ptmin_etcone30et, Weight, "ElecEtCone30overEt_ig");
    } else {

      FillTH1(ptmin_pt, Weight, "ElecPt_ib");
      FillTH1(ptmin_etclus, Weight, "ElecEtCluster_ib");
      FillTH1(ptmin_etcone20, Weight, "ElecEtCone20_ib");
      FillTH1(ptmin_etcone30, Weight, "ElecEtCone30_ib");
      FillTH1(ptmin_etcone20et, Weight, "ElecEtCone20overEt_ib");
      FillTH1(ptmin_etcone30et, Weight, "ElecEtCone30overEt_ib");
    }

  }

}
