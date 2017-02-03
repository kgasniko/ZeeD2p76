#ifndef __ZeeDReadPhotonAOD_H
#define __ZeeDReadPhotonAOD_H

////////////////////////////////////////////////////////
// Name    : ZeeDReadPhotonAOD.h
////////////////////////////////////////////////////////

#include <TObject.h>

#include "StoreGate/StoreGateSvc.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

class ZeeDEvent;
class egammaContainer;

class ZeeDReadPhotonAOD: public TObject {
public:
	ZeeDReadPhotonAOD();
	~ZeeDReadPhotonAOD();

    /** Calculate photon candidates for the given event */
    void ReadInfo(ZeeDEvent* event);

private:

    /** Initialise necessary containers */
    void Initialize();

    ZeeDSvcHelper fSvcHelper;

    ServiceHandle<StoreGateSvc>* fStoreGate;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;

    const egammaContainer* fPhotonContainer ;

};

#endif /* __ZeeDReadPhotonAOD_H */
