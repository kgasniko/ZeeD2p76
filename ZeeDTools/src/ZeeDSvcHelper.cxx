#include "ZeeDTools/ZeeDSvcHelper.h"

#include "TSystem.h"

#include "GaudiKernel/ServiceHandle.h"
#include "GaudiKernel/ToolHandle.h"

#include "StoreGate/StoreGateSvc.h"
#include "GaudiKernel/ITHistSvc.h"

#include "MCTruthClassifier/IMCTruthClassifier.h"

#include "TrigObjectMatching/TrigMatchTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

//------------------------------------------------------------------------------
ZeeDSvcHelper::ZeeDSvcHelper()
    : fName("NONAME"),

      fAnaAlreadyRetrieved(false),
      fStoreGateAlreadyRetrieved(false),
      fTHistSvcAlreadyRetrieved(false),
      fTrigMatchToolAlreadyRetrieved(false),
      fTrigDecisionToolAlreadyRetrieved(false),
      fMCTruthClassifierAlreadyRetrieved(false)

{

    fHandleZeeDAnalysisSvc = new ServiceHandle<IZeeDAnalysisSvc>("ZeeDAnalysisSvc", "NONAME");
    fHandleStoreGate = new ServiceHandle<StoreGateSvc>("StoreGateSvc", "NONAME");
    fHandleTHistSvc = new ServiceHandle<ITHistSvc>("THistSvc", "NONAME");
    fHandleTrigMatchTool = new ToolHandle<TrigMatchTool>("TrigMatchTool");
    fHandleTrigDecisionTool = new ToolHandle<Trig::TrigDecisionTool>("Trig::TrigDecisionTool/TrigDecisionTool");
    fHandleMCTruthClassifier = new ToolHandle<IMCTruthClassifier>("MCTruthClassifier");

}


//------------------------------------------------------------------------------
ZeeDSvcHelper::ZeeDSvcHelper(const TString& name)
    : fName(name),
      fAnaAlreadyRetrieved(false),
      fStoreGateAlreadyRetrieved(false),
      fTHistSvcAlreadyRetrieved(false),
      fTrigMatchToolAlreadyRetrieved(false),
      fTrigDecisionToolAlreadyRetrieved(false),
      fMCTruthClassifierAlreadyRetrieved(false)
{

    fHandleZeeDAnalysisSvc = new ServiceHandle<IZeeDAnalysisSvc>("ZeeDAnalysisSvc", name.Data());
    fHandleStoreGate = new ServiceHandle<StoreGateSvc>("StoreGateSvc", name.Data());
    fHandleTHistSvc = new ServiceHandle<ITHistSvc>("THistSvc", name.Data());
    fHandleTrigMatchTool = new ToolHandle<TrigMatchTool>("TrigMatchTool");
    fHandleTrigDecisionTool = new ToolHandle<Trig::TrigDecisionTool>("Trig::TrigDecisionTool/TrigDecisionTool");
    fHandleMCTruthClassifier = new ToolHandle<IMCTruthClassifier>("MCTruthClassifier");

}

//------------------------------------------------------------------------------
ZeeDSvcHelper::~ZeeDSvcHelper()
{

    delete fHandleZeeDAnalysisSvc;
    delete fHandleStoreGate;
    delete fHandleTHistSvc;
    delete fHandleTrigMatchTool;
    delete fHandleTrigDecisionTool;
    delete fHandleMCTruthClassifier;
}

//------------------------------------------------------------------------------
void ZeeDSvcHelper::SetName(const TString& name)
{

    fName = name;

}

//------------------------------------------------------------------------------
StatusCode ZeeDSvcHelper::retrieveAnaSvc() const
{

    StatusCode sc = fHandleZeeDAnalysisSvc->retrieve();

    if ( sc.isFailure() ) {

        Error(fName, "ZeeDSvcHelper is unable to retrieve ZeeDAnalysisSvc");

        gSystem->Exit(EXIT_FAILURE);

    }

    return sc;

}

//------------------------------------------------------------------------------
StatusCode ZeeDSvcHelper::retrieveStoreGate() const
{

    StatusCode sc = fHandleStoreGate->retrieve();

    if ( sc.isFailure() ) {

        Error(fName, "ZeeDSvcHelper is unable to retrieve StoreGate");

        gSystem->Exit(EXIT_FAILURE);

    }

    return sc;

}

//------------------------------------------------------------------------------
StatusCode ZeeDSvcHelper::retrieveTHistSvc() const
{

    StatusCode sc = fHandleTHistSvc->retrieve();

    if ( sc.isFailure() ) {

        Error(fName, "ZeeDSvcHelper is unable to retrieve THistSvc");

        gSystem->Exit(EXIT_FAILURE);

    }

    return sc;

}

//------------------------------------------------------------------------------
StatusCode ZeeDSvcHelper::retrieveTrigMatchTool() const
{

    StatusCode sc = fHandleTrigMatchTool->retrieve();

    if ( sc.isFailure() ) {

        Error(fName, "ZeeDSvcHelper is unable to retrieve TrigMatchTool");

        gSystem->Exit(EXIT_FAILURE);

    }

    return sc;

}

//------------------------------------------------------------------------------
StatusCode ZeeDSvcHelper::retrieveTrigDecisionTool() const
{

    StatusCode sc = fHandleTrigDecisionTool->retrieve();

    if ( sc.isFailure() ) {

        Error(fName, "ZeeDSvcHelper is unable to retrieve TrigDecisionTool");

        gSystem->Exit(EXIT_FAILURE);

    }

    return sc;

}

//------------------------------------------------------------------------------
StatusCode ZeeDSvcHelper::retrieveMCTruthClassifier() const
{

    StatusCode sc = fHandleMCTruthClassifier->retrieve();

    if ( sc.isFailure() ) {

        Error(fName, "ZeeDSvcHelper is unable to retrieve MCTruthClassifier");

        gSystem->Exit(EXIT_FAILURE);

    }

    return sc;

}

//------------------------------------------------------------------------------
ServiceHandle<IZeeDAnalysisSvc>* ZeeDSvcHelper::GetAnaSvc() const
{

    if(fAnaAlreadyRetrieved) {

        return fHandleZeeDAnalysisSvc;

    } else {

        this->retrieveAnaSvc();
        fAnaAlreadyRetrieved = true;

        return fHandleZeeDAnalysisSvc;

    }

}

ServiceHandle<StoreGateSvc>* ZeeDSvcHelper::GetStoreGate() const
{

    if(fStoreGateAlreadyRetrieved) {

        return fHandleStoreGate;

    } else {

        this->retrieveStoreGate();
        fStoreGateAlreadyRetrieved = true;

        return fHandleStoreGate;

    }

}

ServiceHandle<ITHistSvc>* ZeeDSvcHelper::GetTHistSvc() const
{

    if(fTHistSvcAlreadyRetrieved) {

        return fHandleTHistSvc;

    } else {

        this->retrieveTHistSvc();
        fTHistSvcAlreadyRetrieved = true;

        return fHandleTHistSvc;

    }

}

ToolHandle<TrigMatchTool>* ZeeDSvcHelper::GetTrigMatchTool() const
{

    if(fTrigMatchToolAlreadyRetrieved) {

        return fHandleTrigMatchTool;

    } else {

        this->retrieveTrigMatchTool();
        fTrigMatchToolAlreadyRetrieved = true;

        return fHandleTrigMatchTool;

    }

}

ToolHandle<Trig::TrigDecisionTool>* ZeeDSvcHelper::GetTrigDecisionTool() const
{

    if(fTrigDecisionToolAlreadyRetrieved) {

        return fHandleTrigDecisionTool;

    } else {

        this->retrieveTrigDecisionTool();
        fTrigDecisionToolAlreadyRetrieved = true;

        return fHandleTrigDecisionTool;

    }

}

ToolHandle<IMCTruthClassifier>* ZeeDSvcHelper::GetMCTruthClassifier() const
{

    if(fMCTruthClassifierAlreadyRetrieved) {

        return fHandleMCTruthClassifier;

    } else {

        this->retrieveMCTruthClassifier();
        fMCTruthClassifierAlreadyRetrieved = true;

        return fHandleMCTruthClassifier;

    }

}

