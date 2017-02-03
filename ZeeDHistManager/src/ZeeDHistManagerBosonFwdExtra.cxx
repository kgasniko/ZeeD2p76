#include "ZeeDHistManager/ZeeDHistManagerBosonFwdExtra.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDBosonZ.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

//#define USE_DEBUG_MESSAGES
#include "ZeeDMisc/ZeeDMisc.h"


using std::cout;
using std::endl;

//-----------------------------------------------------

void ZeeDHistManagerBosonFwdExtra::BookHistos()
{
  // Boson mass for different eta fwd elec regions
  AddTH1D("BosMassEMECIW", 100,   66.0, 116.0, "M_{Z->ee} / GeV",     "");
  AddTH1D("BosMassFCAL", 100,   66.0, 116.0, "M_{Z->ee} / GeV",     "");
  AddTH1D("BosMassEMECIWL", 100,   66.0, 116.0, "M_{Z->ee} / GeV",     "");
  AddTH1D("BosMassFCALL", 100,   66.0, 116.0, "M_{Z->ee} / GeV",     "");
  AddTH1D("BosMassEMECIWR", 100,   66.0, 116.0, "M_{Z->ee} / GeV",     "");
  AddTH1D("BosMassFCALR", 100,   66.0, 116.0, "M_{Z->ee} / GeV",     "");

  // Boson mass as a function of smth 
  Double_t bounds_nvtx[] = {-0.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5, 100.5};
  AddTH2D("BosMass_Nvtx", 10, bounds_nvtx, 100,   66.0, 116.0, "N_{vtx}", "M_{Z->ee} / GeV");  
  Double_t bounds_etafwd[] = {2.5, 2.7, 2.9, 3.2, 3.6, 4.0, 4.4, 4.9};
  AddTH2D("BosMass_EtaFwd", 7, bounds_etafwd, 100,   66.0, 116.0, "N_{vtx}", "M_{Z->ee} / GeV");  
  Double_t bounds_ptfwd[] = {0., 20., 25., 30., 35., 40., 45., 50., 60., 70., 80., 100.};
  AddTH2D("BosMass_PtFwd", 11, bounds_ptfwd, 100,   66.0, 116.0, "N_{vtx}", "M_{Z->ee} / GeV");

  // Hists, separated for forward/central electrons
  AddTH1D("FwdElecEtaFine",       50,  -5.0,  5.0, "#eta^{FWDelec}",           "" );
  AddTH1D("CntElecEtaFine",       50,  -5.0,  5.0, "#eta^{CNTelec}",           "" );


}

void ZeeDHistManagerBosonFwdExtra::Fill()
{

  // ------- ==== Get hist pointers back ======------------------------------------------

  // ---------===== Get variables =========------------------------------------------
  // Get event
  const ZeeDEvent* event = GetEvent();
  CHECK_NULL_PTR(event);

  // Event 
  Int_t numVtx = event->Nvtx().Get();
  Double_t Weight = event->GetWeight();

  // Boson:
  const ZeeDBosonZ* boson = event->GetCurrentBoson();
  TLorentzVector fourVector = boson->GetFourVector();


  const ZeeDElectron* e1 =(ZeeDElectron*) boson->GetFirstLep();
  const ZeeDElectron* e2 = (ZeeDElectron*)boson->GetSecondLep();

  if ( (e1 != NULL ) && (e2 != NULL ) ) {

    // Who is fwd?
    const ZeeDElectron* elecFwd = NULL;
    const ZeeDElectron* elecCnt = NULL;

    if (TMath::Abs(e1->GetFourVector().Eta()) > TMath::Abs(e2->GetFourVector().Eta())) {	// first elec is fwd
      elecFwd = e1;
      elecCnt = e2;
    } else {											// second electron is fwd
      elecFwd = e2;
      elecCnt = e1;
    }

    // ------------=========== Fill ===========---------------------------------------
    double etaFwdabs = TMath::Abs(elecFwd->GetFourVector().Eta());
    double etaFwd    = elecFwd->GetFourVector().Eta();
    double ptFwd     = elecFwd->GetFourVector().Pt();

    if ( (etaFwdabs > 2.50) && (etaFwdabs < 3.20) ) {
      FillTH1(fourVector.M(),   Weight, "BosMassEMECIW");
    }
    if ( (etaFwdabs > 3.20) && (etaFwdabs < 4.90)) {
      FillTH1(fourVector.M(),   Weight, "BosMassFCAL");
    }

    if ( (etaFwd > 2.50) && (etaFwd < 3.20) ) {
      FillTH1(fourVector.M(),   Weight, "BosMassEMECIWL");
    }
    if ( (etaFwd > 3.20) && (etaFwd < 4.90)) {
      FillTH1(fourVector.M(),   Weight, "BosMassFCALL");
    }

    if ( (etaFwd < -2.50) && (etaFwd > -3.20) ) {
      FillTH1(fourVector.M(),   Weight, "BosMassEMECIWR");
    }
    if ( (etaFwd < -3.20) && (etaFwd > -4.90)) {
      FillTH1(fourVector.M(),   Weight, "BosMassFCALR");
    }

    FillTH1(elecCnt->GetFourVector().Eta(),   Weight, "CntElecEtaFine");
    FillTH1(elecFwd->GetFourVector().Eta(),   Weight, "FwdElecEtaFine");

    FillTH2(numVtx, fourVector.M(), Weight, "BosMassNvtx");
    FillTH2(etaFwd, fourVector.M(), Weight, "BosMassEtaFwd");
    FillTH2(ptFwd,  fourVector.M(), Weight, "BosMassPtFwd");
  }

}

