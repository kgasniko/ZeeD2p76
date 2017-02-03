#pragma once

////////////////////////////////////////////////////////
// Name    : ZeeDBosonEMu.h
////////////////////////////////////////////////////////
//
//

// Root includes
#include "ZeeDEvent/ZeeDBoson.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDMuon.h"

/** Basic class for fake Z boson made of e-mu
 */
class ZeeDBosonEMu : public ZeeDBoson {

  public:

    ZeeDBosonEMu();
    ZeeDBosonEMu(const ZeeDBosonEMu& boson);
    ZeeDBosonEMu& operator= (const ZeeDBosonEMu& rhs);
    ~ZeeDBosonEMu();

    // Getters
    const ZeeDElectron* GetElectron() const { return fElectron; }

    const ZeeDMuon* GetMuon() const { return fMuon; }

    // Setters
    void SetElectron(const ZeeDElectron* electron) { fElectron = electron; }

    void SetMuon(const ZeeDMuon* muon) { fMuon = muon; }

  private:

    const ZeeDElectron* fElectron;
    const ZeeDMuon*  	fMuon;

};

// Operator to dump Z boson:

std::ostream& operator<< (std::ostream& s, const ZeeDBosonEMu& Z);

