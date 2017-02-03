
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorPreSelection.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"

#include "ZeeDTools/ZeeDBags.h"

void ZeeDAnalysisCutSelectorPreSelection::BookCuts()
{

    //At least 1 central electron in event
    
    //this->AddCut(new ZeeDCutTriggersW("WTriggers"));
    //this->AddCut(new ZeeDCutExistBornElectron("AtLeastOneElecInEvent", 1);
    //this->AddCut(new ZeeDCutCentralElectrons("CentralElectron");

    /*
    
    // one electron with cluster Et > 15 GeV and IsEM mediumPP
    this->AddCut(new ZeeDCutMinClusterEtNElec("OneMediumPPElecMinClusterEt15", 1, 5, true, true, &ZeeDElectronBags::IsEM::isLoosePP));

     //
    this->AddCut(new ZeeDCutMinClusterEtNElec("TwoMediumElecMinClusterEt15", 2, 15, true, true, &ZeeDElectronBags::IsEM::isMedium));

    //
    this->AddCut(new ZeeDCutMinClusterEtNElec("TwoLooseElecMinClusterEt15", 2, 15, true, true, &ZeeDElectronBags::IsEM::isLoose));

    // two electrons with cluster Et > 15 GeV but _no_ requirement on IsEM
    this->AddCut(new ZeeDCutMinClusterEtNElec("TwoElecMinClusterEt15", 2, 15, true, false));

    this->AddCut(new ZeeDCutMinClusterEtNElec("OneElecMinClusterEt14", 1, 14, true, false));

    this->AddCut(new ZeeDCutMinClusterEtNElec("OneElecMinClusterEt15", 1, 15, true, false));*/

}
