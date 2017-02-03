#ifndef __ZeeDReadTauAOD_H
#define __ZeeDReadTauAOD_H

////////////////////////////////////////////////////////
// Name    : ZeeDReadTauAOD.h
////////////////////////////////////////////////////////

#include <TObject.h>

#include "StoreGate/StoreGateSvc.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "tauEvent/TauJetContainer.h"

class ZeeDEvent;
class ZeeDReadTauAOD: public TObject {
public:
	ZeeDReadTauAOD();
	~ZeeDReadTauAOD();

    /** Calculate tau candidates for the given event */
    void ReadInfo(ZeeDEvent* event);

private:

    /** Initialise necessary containers */
    void Initialize();

    ZeeDSvcHelper fSvcHelper;

    const Analysis::TauJetContainer *fTauContainer;
    ServiceHandle<StoreGateSvc>* fStoreGate;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;

};

#endif /* __ZeeDReadTauAOD_H */
