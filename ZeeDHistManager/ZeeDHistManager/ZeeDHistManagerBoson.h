#pragma once

///////////////////////////////////////////////
//  Name ZeeDHistManagerBoson
//  Basic histograms for Boson candidates
//  Created by SG, 13 Feb 2011.
//
//////////////////////////////////////////////

#include "ZeeDHist/ZeeDHistManager.h"
#include "TString.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDBosonW.h"

/** ZeeDHistManagerBoson */
class ZeeDHistManagerBoson : public ZeeDHistManager {
public:

    ZeeDHistManagerBoson(TString name)
        : ZeeDHistManager(name), bIsMC(kFALSE), bIsW(kFALSE),
	  fSvcHelper("ZeeDAnalysisCutHistManagerZCC"),
          fAnaOptions(fSvcHelper.GetAnaSvc())
    {}

    void SetProperties(Bool_t isW) {
        bIsW = isW;
    }

    // Book all histograms:
    virtual void BookHistos();

    // Arrange loop over Z,W bosons:
    using ZeeDHistManager::Fill;
    virtual void Fill();

    // Fill histos for individual boson
    virtual void FillBosonHists(const ZeeDBoson* boson);


private:
    Bool_t bIsMC;
    Bool_t bIsW;
    
    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;      //!

};

