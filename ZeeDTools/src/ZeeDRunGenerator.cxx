#include "ZeeDTools/ZeeDRunGenerator.h"
#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include <TROOT.h>
#include <TSystem.h>
#include "TFile.h"
#include "TH1F.h"
#include <TObject.h>

using namespace ZeeDRunGenerator;

static TH1F* fRunHist  = 0;
static TH1F* fLumiHist = 0;

/** Return random run number following luminosity profile */
int ZeeDRunGenerator::GetRun()
{
    if (fLumiHist == NULL ) {
        // Initialize
        ZeeDSvcHelper fSvcHelper("ZeeDRunGenerator");
        const TString runLumiFile = ZeeDMisc::FindFile("RandomRunNumber/RunLumi.root");
        TFile* rootLumiFile = TFile::Open(runLumiFile, "READ");

        if (rootLumiFile == NULL) {
            // TODO
            //Error("ZeeDRunGenerator::GetRun",TString("Can not open file") + runLumiFile);
            gSystem->Exit(EXIT_FAILURE);
        }

        rootLumiFile->GetObject("h1", fRunHist);
        fRunHist->SetDirectory(0);
        rootLumiFile->GetObject("h2", fLumiHist);
        fLumiHist->SetDirectory(0);
        rootLumiFile->Close();
        // TODO
        //Info("ZeeDRunGenerator::GetRun",TString("Successfully initialized run simulation"));
    }

    int iBin = fLumiHist->GetRandom();
    int iRun = fRunHist->GetAt(iBin + 1);
    return iRun;
}
