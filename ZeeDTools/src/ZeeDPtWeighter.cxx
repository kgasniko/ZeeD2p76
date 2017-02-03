#include "ZeeDTools/ZeeDPtWeighter.h"
#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include <TROOT.h>
#include <TSystem.h>
#include "TFile.h"
#include "TH1D.h"

using namespace ZeeDPtWeighter;

static TH1D* wEMB1  = 0;
static TH1D* wEMB2  = 0;
static TH1D* wEMEC  = 0;

/**
  Provide weighting factors in Eta ranges to emulate Pt dependence for single
  particle simulation.
*/
double ZeeDPtWeighter::GetPtW(double pt, double eta)
{
    if (wEMEC == NULL ) {
        // Initialize
        ZeeDSvcHelper fSvcHelper("ZeeDPtWeighter");
        const TString ptWname = ZeeDMisc::FindFile("PtWeight.root");
        TFile* ptWfile = TFile::Open(ptWname, "READ");

        if (ptWfile == NULL) {
            // TODO
            //Error("ZeeDPtWeighter::GetRun",TString("Can not open file") + ptWname);
            gSystem->Exit(EXIT_FAILURE);
        }

        ptWfile->GetObject("emb1", wEMB1);
        wEMB1->SetDirectory(0);

        ptWfile->GetObject("emb2", wEMB2);
        wEMB2->SetDirectory(0);

        ptWfile->GetObject("emec", wEMEC);
        wEMEC->SetDirectory(0);

        ptWfile->Close();
        // TODO
        // Info("ZeeDPtWeighter::GetPtW",TString("Successfully initialized Pt weight simulation"));
    }

    // Calculate bin
    if (pt < 0 ) {
        pt = 0.;
    }

    if (pt > 100.) {
        pt = 99.9;
    }

    int ibin = pt / 2. + 1;
    double fWeight = 1;


    if ( TMath::Abs(eta) < 0.8) {
        fWeight = wEMB1->At(ibin);
    }

    if ( (TMath::Abs(eta) > 0.8) && (TMath::Abs(eta) < 1.37) ) {
        fWeight = wEMB2->At(ibin);
    }

    if ( (TMath::Abs(eta) > 1.52) && (TMath::Abs(eta) < 2.47) ) {
        fWeight = wEMEC->At(ibin);
    }



    return fWeight;
}
