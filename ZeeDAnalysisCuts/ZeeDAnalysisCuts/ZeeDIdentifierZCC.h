#ifndef ZEED_IDENTIFIERZCC_H
#define ZEED_IDENTIFIERZCC_H

/* Class for identifyinf best Z->mumu bosons
*/

#include "ZeeDAnalysisCuts/ZeeDFinderZCC.h"
#include "ZeeDCuts/IZeeDCalcWeight.h"
#include <TObjArray.h>
#include "ZeeDAnalysisCuts/ZeeDIdentifier.h"
#include "ZeeDEvent/ZeeDBosonZ.h"

#include <vector>
#include <utility>

class ZeeDEvent;

class ZeeDIdentifierZCC : public ZeeDIdentifier 
{
public:
    ZeeDIdentifierZCC(TString name):ZeeDIdentifier(name) {};
    // Enforce event ID
    // @param idnumber - identification number
    void SetEventIdentification(ZeeDEvent* event, UInt_t idnumber) const {
      event->SetCurrentBoson((ZeeDBosonZ*)fBosonsAndWeights.at(idnumber).first);
    }
    
    void IdentifyRecBosons (ZeeDEvent*);

private:


};


#endif
