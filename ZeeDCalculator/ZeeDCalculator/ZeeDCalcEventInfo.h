#ifndef __ZEEDCALCEVENTINFO_H
#define __ZEEDCALCEVENTINFO_H

////////////////////////////////////////////////////////
// Name    : ZeeDCalcEventInfo.h
////////////////////////////////////////////////////////
//
// Contact persons: Andrei Nikiforov, Sebastian Schmitt
//

// Root libraries
#include <TObject.h>

// Athena includes
#include "GaudiKernel/ToolHandle.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "GoodRunsLists/TGoodRunsList.h"

class ZeeDEvent;

namespace Root {
    class TGoodRunsListReader;
}

/** Class for calculation of addition event information */
class ZeeDCalcEventInfo : public TObject {

public:

    ZeeDCalcEventInfo();
    ~ZeeDCalcEventInfo();

    /** Calculate W boson candidates for the given event */
    void Calculate(ZeeDEvent* event);

    /** Calculates the number of good verticies */
    void CalcNGoodVerticies(ZeeDEvent* event);

private:

    /** Calculates manual GRL decision for TTree reading */
    void CalcManualGRL();

    void CalcGenBosons();

    void CalcGenLeptons();    

    void CalcDressing();    
 
    void CalcMCtype();

    /** Pointer to the current event */
    ZeeDEvent* fEventInProcess;

    /** Service helper */
    ZeeDSvcHelper fSvcHelper;

    /** Analysis options service */
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions; //!

    Root::TGoodRunsListReader* fGRLReader;

    Root::TGoodRunsList fGRL;

};

#endif // ZeeDCalcEventInfo
