#ifndef ZEED_IDENTIFIERW_H
#define ZEED_IDENTIFIERW_H

/* Class for identifying best W bosons
 */

#include "ZeeDAnalysisCuts/ZeeDFinderW.h"
#include "ZeeDCuts/IZeeDCalcWeight.h"
#include "ZeeDEvent/ZeeDBosonW.h"
#include <TObjArray.h>
#include "ZeeDAnalysisCuts/ZeeDIdentifier.h"

#include <vector>
#include <utility>


class ZeeDIdentifierW: public ZeeDIdentifier {
public:
    ZeeDIdentifierW(TString name):ZeeDIdentifier(name) {};
    // Enforce event ID
    // @param idnumber - identification number
    void SetEventIdentification(ZeeDEvent* event, UInt_t idnumber) const {
      event->SetCurrentBosonW((ZeeDBosonW*)fBosonsAndWeights.at(idnumber).first);
      ZeeDBosonW* bos = (ZeeDBosonW*)fBosonsAndWeights.at(idnumber).first;
    }
    
    void IdentifyRecBosons (ZeeDEvent*);

private:


};


#endif
