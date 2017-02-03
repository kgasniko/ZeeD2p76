#ifndef ZEEDSVCHELPER_H
#define ZEEDSVCHELPER_H

#include <TObject.h>
#include <TString.h>

#include <GaudiKernel/StatusCode.h>

#include "GaudiKernel/ServiceHandle.h"
#include "GaudiKernel/ToolHandle.h"

/*
template <class TYPE> class ServiceHandle;
template <class TYPE> class ToolHandle;
*/

class IZeeDAnalysisSvc;

class TrigMatchTool;

namespace Trig {

class TrigDecisionTool;

}

class StoreGateSvc;
class ITHistSvc;

class IMCTruthClassifier;

class ZeeDSvcHelper : public TObject {

public:

    ZeeDSvcHelper();
    ZeeDSvcHelper(const TString& name);
    ~ZeeDSvcHelper();

    ServiceHandle<IZeeDAnalysisSvc>* GetAnaSvc() const;
    ServiceHandle<StoreGateSvc>*     GetStoreGate() const;
    ServiceHandle<ITHistSvc>*        GetTHistSvc() const;
    ToolHandle<TrigMatchTool>*       GetTrigMatchTool() const;
    ToolHandle<Trig::TrigDecisionTool>*    GetTrigDecisionTool() const;
    ToolHandle<IMCTruthClassifier>*    GetMCTruthClassifier() const;

    void SetName(const TString& name);

private:

    StatusCode retrieveAnaSvc() const;
    StatusCode retrieveStoreGate() const;
    StatusCode retrieveTHistSvc() const;
    StatusCode retrieveTrigMatchTool() const;
    StatusCode retrieveTrigDecisionTool() const;
    StatusCode retrieveMCTruthClassifier() const;

    TString fName;

    mutable ServiceHandle<IZeeDAnalysisSvc>* fHandleZeeDAnalysisSvc;
    mutable bool fAnaAlreadyRetrieved;

    mutable ServiceHandle<StoreGateSvc>* fHandleStoreGate;
    mutable bool fStoreGateAlreadyRetrieved;

    mutable ServiceHandle<ITHistSvc>* fHandleTHistSvc;
    mutable bool fTHistSvcAlreadyRetrieved;

    mutable ToolHandle<TrigMatchTool>* fHandleTrigMatchTool;
    mutable bool fTrigMatchToolAlreadyRetrieved;

    mutable ToolHandle<Trig::TrigDecisionTool>* fHandleTrigDecisionTool;
    mutable bool fTrigDecisionToolAlreadyRetrieved;

    mutable ToolHandle<IMCTruthClassifier>* fHandleMCTruthClassifier;
    mutable bool fMCTruthClassifierAlreadyRetrieved;

};

#endif
