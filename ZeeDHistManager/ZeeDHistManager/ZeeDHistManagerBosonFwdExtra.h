#ifndef __ZEEDHISTMANAGERBOSONFWDEXTRA_H
#define __ZEEDHISTMANAGERBOSONFWDEXTRA_H

///////////////////////////////////////////////
//  Name ZeeDHistManagerBosonFwdExtra
//  Extra histograms for Forward Boson candidates
//  Created by SG, 18 May 2011.
//
//////////////////////////////////////////////

#include "ZeeDHist/ZeeDHistManager.h"
#include "TString.h"


/** ZeeDHistManagerBosonFwdExtra */
class ZeeDHistManagerBosonFwdExtra : public ZeeDHistManager {
public:

    explicit ZeeDHistManagerBosonFwdExtra(TString name)
        : ZeeDHistManager(name)
    {}


    // Book all histograms:
    virtual void BookHistos();

    // Fill histos:
    using ZeeDHistManager::Fill;
    virtual void Fill();

};

#endif //ZeeDHistManagerBosonFwdExtra

