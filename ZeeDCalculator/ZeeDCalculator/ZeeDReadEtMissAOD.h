#ifndef __ZEEDREADETMISSAOD_H
#define __ZEEDREADETMISSAOD_H

////////////////////////////////////////////////////////
// Name    : ZeeDReadEtMissAOD.h
////////////////////////////////////////////////////////
//
// Contact persons: Andrei Nikiforov, George Sedov
//

#include <TObject.h>

#include "ZeeDTools/ZeeDSvcHelper.h"

#include "StoreGate/StoreGateSvc.h"
#include "tauEvent/TauJetContainer.h"
#include "muonEvent/MuonContainer.h"


class ZeeDEvent;
class ZeeDEtMiss;

class MissingET;
class MissingETComposition;
class JetCollection;
class ElectronContainer;
class egammaContainer;

/** Class for reading missing Et objects from AOD */
class ZeeDReadEtMissAOD : public TObject {

public:

    ZeeDReadEtMissAOD();
    ~ZeeDReadEtMissAOD();

    /** Reads et miss objects for given event */
    void ReadInfo(ZeeDEvent* event);

private:

    /** Initialise necessary containers */
    void Initialize();

    /** Reads et miss object from AOD */
    void ReadEtMiss(ZeeDEtMiss* etMiss);//, ZeeDEvent* event); // added event, since need to store MET composition information to all teh hard objetct

    /** Missing Et object */
    const MissingET* fMissingEtObj;
    const MissingET* fMissingEtSoftTermObj;
    const MissingETComposition* fMissingEtCompositionObj;

    const JetCollection* fJetCollection;
    const ElectronContainer* fElecContainer; 
    const egammaContainer* fPhotonContainer ;
    const Analysis::MuonContainer* fMuonContainer;
    const Analysis::TauJetContainer *fTauContainer;

    /** Service helper */
    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc> *fAnaOptions; //!
    ServiceHandle<StoreGateSvc>* fStoreGate;

};

#endif // ZeeDReadEtMissAOD
