#ifndef __ZEEDHISTMANAGERTRUTHMATCH_H
#define __ZEEDHISTMANAGERTRUTHMATCH_H

///////////////////////////////////////////////
//  Name ZeeDHistManagerTruthMatch
//  Basic histograms for Matched Reconstructed & generated information
//  Created by MK, 18 Feb 2011.
//
//////////////////////////////////////////////

#include "ZeeDHist/ZeeDHistManager.h"
#include "TString.h"

class ZeeDBoson;

/** ZeeDHistManagerTruthMatch */
class ZeeDHistManagerTruthMatch : public ZeeDHistManager {
public:

    explicit ZeeDHistManagerTruthMatch(TString name)
        : ZeeDHistManager(name), bIsMC(kFALSE), bIsAll(kFALSE), bIsW(kFALSE)
    {}

    void SetProperties(Bool_t isAll, Bool_t isW) {
        bIsAll = isAll;
        bIsW = isW;
    }

    // Book all histograms:
    virtual void BookHistos();

    // Fill All histograms
    using ZeeDHistManager::Fill;
    virtual void Fill();


private:
    Bool_t bIsMC;
    Bool_t bIsAll;
    Bool_t bIsW;

    ServiceHandle<IZeeDAnalysisSvc>* fZeeDAnaOptions;


};

#endif //ZeeDHistManagerTruthMatch

