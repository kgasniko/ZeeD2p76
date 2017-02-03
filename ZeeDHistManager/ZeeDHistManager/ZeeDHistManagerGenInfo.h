#ifndef __ZEEDHISTMANAGERGENINFO_H
#define __ZEEDHISTMANAGERGENINFO_H

///////////////////////////////////////////////
//  Name ZeeDHistManagerGenInfo
//  Basic histograms for Generated information
//  Created by MK, 18 Feb 2011.
//
//////////////////////////////////////////////

#include "ZeeDHist/ZeeDHistManager.h"
#include "TString.h"

class ZeeDBoson;

/** ZeeDHistManagerGenInfo */
class ZeeDHistManagerGenInfo : public ZeeDHistManager {
public:

    explicit ZeeDHistManagerGenInfo(TString name)
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

#endif //ZeeDHistManagerGenInfo

