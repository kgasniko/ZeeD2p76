#ifndef ZEED_IDENTIFIERWMU_H
#define ZEED_IDENTIFIERWMU_H

/* Class for identifyinf best W->munu bosons
*/

#include "ZeeDAnalysisCuts/ZeeDFinderWmu.h"
#include "ZeeDCuts/IZeeDCalcWeight.h"
#include <TObjArray.h>
#include "ZeeDAnalysisCuts/ZeeDIdentifier.h"
#include "ZeeDEvent/ZeeDBosonW.h"

#include <vector>
#include <utility>

class ZeeDEvent;

class ZeeDIdentifierWmu : public ZeeDIdentifier 
{
public:
    ZeeDIdentifierWmu(TString name):ZeeDIdentifier(name) {};
    // Enforce event ID
    // @param idnumber - identification number
    void SetEventIdentification(ZeeDEvent* event, UInt_t idnumber) const {
      event->SetCurrentBosonW((ZeeDBosonW*)fBosonsAndWeights.at(idnumber).first);
    }
    
    void IdentifyRecBosons (ZeeDEvent*);

private:


};


#endif
