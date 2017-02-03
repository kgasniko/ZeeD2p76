#ifndef __ZEEDHISTMANAGERETMISS_H
#define __ZEEDHISTMANAGERETMISS_H

///////////////////////////////////////////////
//  Name ZeeDHistManagerEtmiss
//  Basic histograms for Etmiss
//  Created by SSh, EY, 19 Jul 2013.
//
//////////////////////////////////////////////

#include "ZeeDHist/ZeeDHistManager.h"
#include "TString.h"

class ZeeDHistManagerEtmiss : public ZeeDHistManager {
public:

    ZeeDHistManagerEtmiss(TString name)
        : ZeeDHistManager(name), bIsMC(kFALSE)
    {}

    // Book all histograms:
    virtual void BookHistos();

    // Fill histos
    using ZeeDHistManager::Fill;
    virtual void Fill();

private:
    Bool_t bIsMC;
};

#endif //ZeeDHistManagerElectron

