// Root libraries
#include "TSystem.h"

// Athena includes
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"

// ZeeD includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorJet.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisJetCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"

//------------------------------------------------------
ZeeDAnalysisCutSelectorJet::ZeeDAnalysisCutSelectorJet(TString name ) : ZeeDCutSelector(name)
{
    // Constructor
    this->BookCuts();
}

//------------------------------------------------------
ZeeDAnalysisCutSelectorJet::~ZeeDAnalysisCutSelectorJet()
{
    // Destructor
}

//------------------------------------------------------
void ZeeDAnalysisCutSelectorJet::BookCuts()
{

    // Books cuts

    // Get handl to the service
    ServiceHandle<IZeeDAnalysisSvc> ZeeDAnaOptions("ZeeDAnalysisSvc", "");
    StatusCode sc = ZeeDAnaOptions.retrieve();

    if ( sc.isFailure() ) {
        Error("ZeeDCutSelectoJet::Initialize", "Unable to open ZeeDAnalysisSvc");
        gSystem->Exit(EXIT_FAILURE);
    }

    const Double_t jetPtMin     = 20.0; //GeV
    const Double_t jetEtaMax    = 2.5;

    //Jet Pt cut
    this->AddCut(new ZeeDCutPtMinJet("JetPtMin",jetPtMin));

    //Jet Eta cut
    this->AddCut(new ZeeDCutEtaMaxJet("JetEtaMax",jetEtaMax));
}
