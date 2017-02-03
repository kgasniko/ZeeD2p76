#ifndef __ZEEDHISTMANAGEREVENT_H
#define __ZEEDHISTMANAGEREVENT_H

///////////////////////////////////////////////
//  Name ZeeDHistManagerEvent
//  Basic histograms for events
//  Created by SG, 13 Feb 2011.
//
//////////////////////////////////////////////

#include "ZeeDHist/ZeeDHistManager.h"
#include "TString.h"

class ZeeDHistManagerEvent : public ZeeDHistManager {
public:

    ZeeDHistManagerEvent(TString name)
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

