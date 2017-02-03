#ifndef __ZEEDCALCWMU_H
#define __ZEEDCALCWMU_H

////////////////////////////////////////////////////////
// Name    : ZeeDCalcWmu.h
////////////////////////////////////////////////////////
//
// Contact persons: Andrei Nikiforov, George Sedov
//

// Root libraries
#include <TObject.h>
#include <TObjArray.h>

#include "ZeeDTools/ZeeDSvcHelper.h"

class MissingET;
class ZeeDEvent;
class ZeeDBosonW;
class ZeeDMuon;
class ZeeDEvent;
class ZeeDEtMiss;

/** Class for calculation of W boson quantities */
class ZeeDCalcWmu : public TObject {

public:

    ZeeDCalcWmu();
    ~ZeeDCalcWmu();

    /** Calculate W boson candidates for the given event */
    void Calculate(ZeeDEvent* event);

private:

    /** Reads array of electrons from event */
    Bool_t ReadMuons();

    /** Reads et miss object from event */
    void ReadEtMiss();
    
    ZeeDBosonW* CreateWmu (const ZeeDMuon* mu);

    /** Missing Et object */
    const MissingET* fMissingEtObj;

    /**  Array of electrons */
    const TObjArray* fMuons;

    /** Pointer to the current event */
    ZeeDEvent* fEventInProcess;

    /** Pointer to etMiss object */
    ZeeDEtMiss* fEtMiss;

    /** Service helper */
    ZeeDSvcHelper fSvcHelper;
};

#endif // ZeeDCalcW
