#ifndef __ZEEDREADEVENTAOD_H
#define __ZEEDREADEVENTAOD_H

////////////////////////////////////////////////////////
// Name    : ZeeDReadEventAOD
////////////////////////////////////////////////////////
//
// Contact persons: Andrei Nikiforov, Sebastian Schmitt
//

#include <TObject.h>
#include <TObjArray.h>

#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDCalculator/IZeeDReadEvent.h"


class ZeeDReadElecAOD;
class ZeeDReadEtMissAOD;
class ZeeDReadJetsAOD;
class ZeeDReadTracksAOD;
class ZeeDReadMuonAOD;
class ZeeDReadMuonSpShowerAOD;
class ZeeDReadVertexAOD;
class ZeeDReadGenAOD;
class ZeeDReadPhotonAOD;
class ZeeDReadTauAOD;

class StoreGateSvc;

namespace Trig {

class TrigDecisionTool;

}

/** Reader for ZeeDEvent with AOD as input source */
class ZeeDReadEventAOD : public TObject, public IZeeDReadEvent  {

public:

    ZeeDReadEventAOD();
    virtual ~ZeeDReadEventAOD();

    virtual void ReadInfo(RawEvent* event);

private:

    /** Receives event information like run and event numbers */
    void  ReadEventInfo(RawEvent* event);

    /** Read trigger

        https://twiki.cern.ch/twiki/bin/viewauth/Atlas/TrigDecisionTool

     */
    void ReadTrigger(RawEvent* event);

    /** Array of objects to be deleted in destructor */
    TObjArray toBeDeleted; //!

    /** Pointer to event */
    ZeeDEvent* fEventInProcess; //!

    /** Pointer to electrons reader */
    ZeeDReadElecAOD* fReadElec; //!

    /** Pointer to electrons reader */
    ZeeDReadTracksAOD* fReadTracks; //!

    /** Pointer to the reader of et miss objects */
    ZeeDReadEtMissAOD* fReadEtMiss; //!

    /** Pointer to reader of vertices */
    ZeeDReadVertexAOD* fReadVertex; //!

    /** Pointer to the reader of muons */
    ZeeDReadMuonAOD* fReadMuon;
    ZeeDReadMuonSpShowerAOD* fReadMuonSpShower;

    /** Jet reader */
    ZeeDReadJetsAOD* fReadJets; //!

    /** Pointer to reader of generated particles */
    ZeeDReadGenAOD* fReadGen; //!

    /** Pointer to reader of photons */
    ZeeDReadPhotonAOD* fReadPhoton; //!

    /** Pointer to reader of taus */
    ZeeDReadTauAOD* fReadTau; //!

    /** Store gate */
    StoreGateSvc* fStoreGate; //!

    /** Service helper */
    ZeeDSvcHelper fSvcHelper; //!

    /** Service handle to job options file */
    ServiceHandle<IZeeDAnalysisSvc> *fAnaOptions; //!

    /** Trigger decision tool */
    ToolHandle<Trig::TrigDecisionTool>* fTrigDecisionTool; //!

};

#endif
