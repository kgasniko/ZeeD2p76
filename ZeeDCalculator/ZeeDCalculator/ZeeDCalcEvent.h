#ifndef ZEED_CALC_EVENT_H
#define ZEED_CALC_EVENT_H

////////////////////////////////////////////////////////
// Name    : ZeeDCalcEvent.h
////////////////////////////////////////////////////////
//
// Contact persons: Sebastian Schmitt, Andrei Nikiforov
//

#include <TObject.h>
#include <TObjArray.h>

#include "ZeeDTools/ZeeDSvcHelper.h"

class ZeeDEvent;

class ZeeDSystematics;

class ZeeDCalcElec;
class ZeeDCalcMuon;
class ZeeDCalcZ;
class ZeeDCalcW;
class ZeeDCalcWmu;
class ZeeDCalcEventInfo;
class ZeeDCalcJets;
class ZeeDCalcEtMiss;
class ZeeDCalibrator;

/** Class to manage sub-calculators preparing event
  information on the basis of raw event content
 */
class ZeeDCalcEvent : public TObject {

public:

    ZeeDCalcEvent();
    ~ZeeDCalcEvent();

    /** Fills the event
        @param event has to be valid pointer
     */
    void Calculate(ZeeDEvent* event);

private:

    /** Array of objects to be deleted in destructor */
    TObjArray toBeDeleted;

    /**  Pointer to systematics manager */
    ZeeDSystematics* fSys; //!

    /**  Pointer to electrons calculator */
    ZeeDCalcElec* fCalcElec; //!

	ZeeDCalcMuon* fCalcMuon;

    /** Pointer to the Z boson calculator */
    ZeeDCalcZ* fCalcZ; //!

    /** Pointer to the W boson calculator */
    ZeeDCalcW* fCalcW; //!
 
    /** Pointer to the W boson calculator */
    ZeeDCalcWmu* fCalcWmu;

    /** Pointer to eventInfo calculator */
    ZeeDCalcEventInfo* fCalcEventInfo; //!

    /** Pointer to jets calculator */
    ZeeDCalcJets* fCalcJets; //!

    /** Pointer to EtMiss calculator */
    ZeeDCalcEtMiss* fCalcEtMiss; //!

    /** Service helper */
    ZeeDSvcHelper fSvcHelper;

    /** Service handle to job options file */
    ServiceHandle<IZeeDAnalysisSvc> *fAnaOptions;
};

#endif
