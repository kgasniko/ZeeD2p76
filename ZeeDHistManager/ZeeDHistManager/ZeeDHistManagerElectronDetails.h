#ifndef __ZEEDHISTMANAGERELECTRONDETAILS_H
#define __ZEEDHISTMANAGERELECTRONDETAILS_H

////////////////////////////////////////////////////////
// Name    : ZeeDHistManagerElectronDetails.h
//
//  SG: 15 Feb 2011, created based on original details hist manager.
//
////////////////////////////////////////////////////////

// Analysis includes
#include "ZeeDHistManager/ZeeDHistManagerLeptonBase.h"
#include "TString.h"

/** Control plots for electron "details"

*/

class ZeeDHistManagerElectronDetails : public ZeeDHistManagerLeptonBase {

public:

    ZeeDHistManagerElectronDetails(Double_t EtaMin = -10., Double_t EtaMax = 10.);
    explicit ZeeDHistManagerElectronDetails(TString name, Double_t EtaMin = -10., Double_t EtaMax = 10.);
    virtual void BookHistos();
    virtual void FillLeptonHists(const TObjArray* electrons, Double_t Weight);

private:

    // Eta range:
    Double_t mEtaMin;
    Double_t mEtaMax;

};

#endif // ZeeDHistmanagerelectrondetails
