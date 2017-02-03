#include "ZeeDHistManager/ZeeDAnalysisCutHistManagerWriter.h"

// Root includes
#include <TString.h>

// ZeeD analysis includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

// Cut selector:
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorPreSelection.h"

#include "ZeeDHistManager/ZeeDHistManagerWriter.h"

//#define DEBUG_ON 0
#include "ZeeDMisc/ZeeDMisc.h"

void ZeeDAnalysisCutHistManagerWriter::BookCutHistManager()
{



    const TString cuts = "";

    this->SetDefaultMask(cuts);

    if ( (*fAnaOptions)->IsData() ) {
        this->AddMaskLoose(cuts, new ZeeDHistManagerWriter((*fAnaOptions)->TTreeOutput(), (*fAnaOptions)->TTreeOutputMaxSize()));
    }

    if ( (*fAnaOptions)->IsMC() ) {
        this->AddMaskLoose("", new ZeeDHistManagerWriter((*fAnaOptions)->TTreeOutput(), (*fAnaOptions)->TTreeOutputMaxSize()));
    }

    /*
    const TString bkg_cuts = "TwoLooseElecMinClusterEt";
    const TString bkg_mask = "TwoLooseElecMinClusterEt";

    this->AddMask(bkg_cuts,
                  new ZeeDHistManagerWriter("TTree_bkg.root", (*fAnaOptions)->TTreeOutputMaxSize()),
                  bkg_mask);
   */
}
