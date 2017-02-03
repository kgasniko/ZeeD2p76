#include "ZeeDCalculator/ZeeDReadEventAOD.h"

// Athena includes
#include "EventInfo/EventInfo.h"
#include "EventInfo/EventID.h"
#include "EventInfo/EventType.h"
#include "TrigDecisionTool/TrigDecisionTool.h"
#include "StoreGate/StoreGateSvc.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"

// ZeeD analysis includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"

#include "ZeeDCalculator/ZeeDReadElecAOD.h"
#include "ZeeDCalculator/ZeeDReadEtMissAOD.h"
#include "ZeeDCalculator/ZeeDReadGenAOD.h"
#include "ZeeDCalculator/ZeeDReadVertexAOD.h"
#include "ZeeDCalculator/ZeeDReadMuonAOD.h"
#include "ZeeDCalculator/ZeeDReadMuonSpShowerAOD.h"
#include "ZeeDCalculator/ZeeDReadJetsAOD.h"
#include "ZeeDCalculator/ZeeDReadTracksAOD.h"
#include "ZeeDCalculator/ZeeDReadPhotonAOD.h"
#include "ZeeDCalculator/ZeeDReadTauAOD.h"

//#define USE_DEBUG_MESSAGES
#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDReadEventAOD::ZeeDReadEventAOD() :
    fStoreGate(0),
    fSvcHelper("ZeeDReadEventAOD"),
    fAnaOptions(fSvcHelper.GetAnaSvc()),
    fTrigDecisionTool(fSvcHelper.GetTrigDecisionTool())
{
  //    Info("ZeeDReadEventAOD::ZeeDReadEventAOD", " Initialisation ");

    toBeDeleted.SetOwner();

    // Calculators
    fReadElec = new ZeeDReadElecAOD();
    toBeDeleted.Add(fReadElec);

    if( (*fAnaOptions)->ReadEtMiss() ) {
        fReadEtMiss = new ZeeDReadEtMissAOD();
        toBeDeleted.Add(fReadEtMiss);
    }

    fReadGen = NULL;

    if ( (*fAnaOptions)->IsMC() && (*fAnaOptions)->FillGenInfo() ) {
        fReadGen  = new ZeeDReadGenAOD();
        toBeDeleted.Add(fReadGen);
    }

    fReadVertex = new ZeeDReadVertexAOD( );
    toBeDeleted.Add(fReadVertex);

    fReadMuon = new ZeeDReadMuonAOD();
    fReadMuonSpShower = new ZeeDReadMuonSpShowerAOD();

    fReadJets = NULL;

    fReadTracks = NULL;
    fReadJets   = NULL;

    if( (*fAnaOptions)->CalcJets()) {
        fReadJets = new ZeeDReadJetsAOD();
        toBeDeleted.Add(fReadJets);

	fReadTracks = new ZeeDReadTracksAOD( );
	toBeDeleted.Add(fReadTracks);
    }

    fReadPhoton = new ZeeDReadPhotonAOD();
    toBeDeleted.Add(fReadPhoton);    

    fReadTau = new ZeeDReadTauAOD();
    toBeDeleted.Add(fReadTau);   

    
    if ( (*fAnaOptions)->EtDensityTools().retrieve().isFailure() )
      {
	   Error("ZeeDReadEventAOD::ZeeDReadEventAOD", " Cannot retrieve Et density tools......... ");
      }
}

//------------------------------------------------------
ZeeDReadEventAOD::~ZeeDReadEventAOD()
{
    // Destructor
    DEBUG_MES_START;

    // Remove all objects from the array
    // AND delete all heap based objects.
    toBeDeleted.Delete();

    DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDReadEventAOD::ReadInfo(RawEvent* event)
{

    // Get run and event numbers
    this->ReadEventInfo(event);

    // Read Electron  information from AOD
    fReadElec->ReadInfo(event);


    // Read Gen info from AOD
    if ( fReadGen != NULL && (*fAnaOptions)->FillGenInfo() ) {
        fReadGen->ReadInfo(event);
    }


    // Calculate Primary Vertex
    fReadVertex->ReadInfo(event);

    // Calculate Muons
    fReadMuon->ReadInfo(event);
    fReadMuonSpShower->ReadInfo(event);

    // Create Jets
    if( fReadJets != NULL) {

        fReadJets->ReadInfo(event);
        fReadTracks->ReadInfo(event);
    }

    fReadPhoton->ReadInfo(event);
    fReadTau->ReadInfo(event);

    // Read Trigger
    if( (*fAnaOptions)->UseTrigger() ) {
        this->ReadTrigger(event);
    }

    //
    // Missing ET should be the lase in the loop !!!! It needa the other objects to be defined...
    // Read missing et object
    //
    if( (*fAnaOptions)->ReadEtMiss() ) {
        fReadEtMiss->ReadInfo(event);
    }

}

//------------------------------------------------------
void ZeeDReadEventAOD::ReadEventInfo(ZeeDEvent* event)
{
    // Reads event run number, event number, lumi block number
    // and sets IsData
    DEBUG_MES_START;

    const EventInfo* evt;
    ServiceHandle<StoreGateSvc> *storeGate = fSvcHelper.GetStoreGate();

    StatusCode sc = (*storeGate)->retrieve(evt);

    if ( sc.isSuccess() ) {

        const Int_t eventNumber = evt->event_ID()->event_number();
        const Int_t runNumber   = evt->event_ID()->run_number();
        const Int_t LB          = evt->event_ID()->lumi_block();

        event->EventNumber().Set(eventNumber);
        event->RunNumber().Set(runNumber);
        event->OrigRunNumber().Set(runNumber);
        event->LB().Set(LB);


        const Double_t actualInteractionsPerCrossing  = evt->actualInteractionsPerCrossing();
        const Double_t averageInteractionsPerCrossing = evt->averageInteractionsPerCrossing();

        event->ActualInteractionsPerCrossing().Set(actualInteractionsPerCrossing);
        event->AverageInteractionsPerCrossing().Set(averageInteractionsPerCrossing);


        // start to set MCtype
        if((*fAnaOptions)->IsMC()) {

            /// dataset id is also known as mc channel number
            const Int_t datasetID = evt->event_type()->mc_channel_number();
            event->MCChannelNumber().Set(datasetID);

            const Double_t mc_event_weight = evt->event_type()->mc_event_weight();
            DEBUG_VAR(mc_event_weight);
	    if (ZeeDMisc::IsAlpgen(datasetID))
	      event->SetGenWeight(ZeeDMisc::AlpgenWeight(datasetID) * mc_event_weight);
	    else
	      event->SetGenWeight(mc_event_weight);

            DEBUG_VAR(event->GetGenWeight());

        }

        event->LArErrorState().Set(evt->errorState(EventInfo::LAr));
        event->LArErrorState().Lock();

    } else {
        Error("ZeeDReadEventAOD::GetEventInfo",
              "Can not get run and event numbers");
    }


    //
    //   Event Et density 
    //
    ToolHandleArray<IEventEtDensityProvider> etdTools = (*fAnaOptions)->EtDensityTools();
    ToolHandleArray<IEventEtDensityProvider>::iterator fTool( etdTools.begin() );
    ToolHandleArray<IEventEtDensityProvider>::iterator lTool( etdTools.end() );
    
    for ( ; fTool != lTool; ++fTool )
      {
	std::string toolName = (*fTool)->name();
	double rho = (*fTool)->rho()/ GeV ;
	if (  toolName.find("EM") != std::string::npos  && toolName.find("3") != std::string::npos )
	  event->SetEtDensity_3EM( rho );
	else if (  toolName.find("EM") != std::string::npos  && toolName.find("4") != std::string::npos )
	  event->SetEtDensity_4EM( rho );
	else if (  toolName.find("LC") != std::string::npos  && toolName.find("3") != std::string::npos )
	  event->SetEtDensity_3LC( rho );
	else if (  toolName.find("LC") != std::string::npos  && toolName.find("4") != std::string::npos )
	  event->SetEtDensity_4LC( rho );
      }


    event->SetIsData((*fAnaOptions)->IsData());

    DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDReadEventAOD::ReadTrigger(RawEvent* event)
{
    DEBUG_MES_START;

    /* do not depend on order of ReadTrigger/ReadEventInfo
       get run number independently
    */
    const EventInfo* evt;
    ServiceHandle<StoreGateSvc> *storeGate = fSvcHelper.GetStoreGate();
    StatusCode sc = (*storeGate)->retrieve(evt);

    if ( sc.isSuccess() == kFALSE) {
        Error("ZeeDReadEventAOD::ReadTrigger",
              "Can not get EventInfo");
        gSystem->Exit(EXIT_FAILURE);
    }

    const Int_t  runNumber = evt->event_ID()->run_number();
    const Bool_t isData    = (*fAnaOptions)->IsData();

    // always check default trigger chain
    const ZeeDEnum::Trigger::Value defaultTrigger =
        isData ? ZeeDMisc::GetDefaultSingleElectronTrigger(runNumber) :	// DATA
        ZeeDMisc::GetDefaultSingleElectronTrigger( runNumber  )		// MC
        ;

    if(defaultTrigger == ZeeDEnum::Trigger::UNKNOWN) {
        Error("ZeeDReadEventAOD::ReadTrigger","no default trigger found");
        gSystem->Exit(EXIT_FAILURE);
    }

    const std::string defaultTrigger_str = ZeeDEnum::Trigger::ToString(defaultTrigger);

    ZeeDEventBags::Trigger triggerBag;

    triggerBag.hasPassedChain[defaultTrigger] = (*fTrigDecisionTool)->isPassed(defaultTrigger_str);

    // additional chains
    std::vector<std::string> triggerChains = (*fAnaOptions)->TriggerEvent();

    for(std::vector<std::string>::const_iterator chain = triggerChains.begin();
            chain != triggerChains.end(); ++chain) {
        triggerBag.hasPassedChain[ZeeDEnum::Trigger::ToEnum(*chain)] = (*fTrigDecisionTool)->isPassed(*chain);
    }

    event->Trigger().Set(triggerBag);
    event->Trigger().Lock();

    DEBUG_MES_END;
}
