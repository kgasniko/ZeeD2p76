#pragma once

////////////////////////////////////////////////////////
// Name    : ZeeDBosonZ.h
////////////////////////////////////////////////////////
//
//

// Root includes
#include "ZeeDEvent/ZeeDBoson.h"
#include "ZeeDEvent/ZeeDLepton.h"

/** Basic class for Z boson particles
 */
class ZeeDBosonZ : public ZeeDBoson {

  public:

    ZeeDBosonZ();
    ZeeDBosonZ(const ZeeDBosonZ& boson);
    ZeeDBosonZ& operator= (const ZeeDBosonZ& rhs);
    ~ZeeDBosonZ();

    // Getters
    const ZeeDLepton* GetFirstLep() const { return (ZeeDLepton*)(fPtPair.first); }

    const ZeeDLepton* GetSecondLep() const { return (ZeeDLepton*)(fPtPair.second); }

    const ZeeDLepton* GetCentralLep() const { return (ZeeDLepton*)(fCentralForwardPair.first); }

    const ZeeDLepton* GetForwardLep() const { return (ZeeDLepton*)(fCentralForwardPair.second); }

    const ZeeDLepton* GetLeptonPos() const { return (ZeeDLepton*)(fPosNegPair.first); }

    const ZeeDLepton* GetLeptonNeg() const { return (ZeeDLepton*)(fPosNegPair.second); }

};

// Operator to dump Z boson:

std::ostream& operator<< (std::ostream& s, const  ZeeDBosonZ& Z);

