#include "ZeeDHistManager/ZeeDAnalysisCutHistManagerZLowMass.h"

// ZeeD analysis includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorZLowMass.h"
#include "ZeeDAnalysisCuts/ZeeDCalcWeightCutDepZ.h"

// Histogram managers
#include "ZeeDHistManager/ZeeDHistManagerElectron.h"
#include "ZeeDHistManager/ZeeDHistManagerBoson.h"
#include "ZeeDHistManager/ZeeDHistManagerEvent.h"

// #include "ZeeDHistManager/ZeeDZLowMassHistManager.h"
#include "ZeeDHistManager/ZeeDCalibratorHM.h"
#include "ZeeDHistManager/ZeeDHistManagerPrint.h"

//#define DEBUG_ON 0
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDArrays/ZeeDArrayD.h"
#include "ZeeDArrays/ZeeDBinGrid.h"

//------------------------------------------------------
ZeeDAnalysisCutHistManagerZLowMass::ZeeDAnalysisCutHistManagerZLowMass(TString name) :
          ZeeDCutHistManager(name),
	  fSvcHelper("ZeeDAnalysisCutHistManagerZLowMass"),
          fAnaOptions(fSvcHelper.GetAnaSvc())
{
    // Constructor
}

//------------------------------------------------------
void ZeeDAnalysisCutHistManagerZLowMass::BookCutHistManager()
{
    // Analysis options
    ServiceHandle<IZeeDAnalysisSvc>* ZeeDAnaOptions = fSvcHelper.GetAnaSvc();

    // Create/book cut selector:
    ZeeDAnalysisCutSelectorZLowMass* AnalysisCuts = new ZeeDAnalysisCutSelectorZLowMass("Low Mass Z Analysis Selection");
    this->SetCutSelector(AnalysisCuts);

    std::vector<std::string> Stages;
    Stages.push_back("ExistZ");
    Stages.push_back(Stages.back() + "+PtMinBothElecZ");
    Stages.push_back(Stages.back() + "+EtaMaxBothElecZ");
    Stages.push_back(Stages.back() + "+EtaCrackBothElecZ");
    Stages.push_back(Stages.back() + "+ZMassZ");
    Stages.push_back(Stages.back() + "+AuthorBothElecZ");
    Stages.push_back(Stages.back() + "+TrackBothElecZ");
    Stages.push_back(Stages.back() + "+NTracksAtPrimVtx");
    Stages.push_back(Stages.back() + "+IsEMTightBothElecZ");
    Stages.push_back(Stages.back() + "+BothCaloIso98");
    Stages.push_back(Stages.back() + "+ChargeBothElecOppositeZ");
    Stages.push_back(Stages.back() + "+OQMaps");
    if ( (*ZeeDAnaOptions)->UseTrigger() ) {
        Stages.push_back(Stages.back() + "+TrigDefaultOneElecZ");
    }
    // FIXME: use the Stages for control purposes

    const std::string DefaultCuts = Stages.back();
    this->SetDefaultMask(DefaultCuts);

    // Main control plots:
    this->AddMaskLoose("DEFAULT", new ZeeDHistManagerBoson(this->getName() + "/" + "Boson"));
    this->AddMaskLoose("DEFAULT", new ZeeDHistManagerElectron(this->getName() + "/" + "Electron"));
    this->AddMaskLoose("DEFAULT", new ZeeDHistManagerEvent(this->getName() + "/" + "Event"));

    if ((*fAnaOptions)->FillBkgHistograms() ) {
        // Add background histograms
        this->AddMask("DEFAULT-ChargeBothElecOppositeZ", new ZeeDHistManagerBoson(this->getName() + "/" + "BosonSC"));
        this->AddMask("DEFAULT-ChargeBothElecOppositeZ", new ZeeDHistManagerElectron(this->getName() + "/" + "ElectronSC"));
        this->AddMask("DEFAULT-ChargeBothElecOppositeZ", new ZeeDHistManagerEvent(this->getName() + "/" + "EventSC"));
        this->AddMask("DEFAULT-ChargeBothElecOppositeZ-IsEMMediumBothElecZ-BothCaloIso98", new ZeeDHistManagerBoson(this->getName() + "/" + "BosonBG"));
        this->AddMask("DEFAULT-ChargeBothElecOppositeZ-IsEMMediumBothElecZ-BothCaloIso98", new ZeeDHistManagerElectron(this->getName() + "/" + "ElectronBG"));
        this->AddMask("DEFAULT-ChargeBothElecOppositeZ-IsEMMediumBothElecZ-BothCaloIso98", new ZeeDHistManagerEvent(this->getName() + "/" + "EventBG"));
    }

    if ((*fAnaOptions)->FillCalibrator() ) {
        // Calibrator:
        this->AddMaskLoose("DEFAULT", new ZeeDCalibratorHM(this->getName() + "/Calib"));
    }

}
