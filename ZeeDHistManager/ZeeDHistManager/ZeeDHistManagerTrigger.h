#ifndef __ZEEDHISTMANAGERTRIGGER_H
#define __ZEEDHISTMANAGERTRIGGER_H

///////////////////////////////////////////////
//  Name ZeeDHistManagerTrigger
//  Basic histograms for Trigger investigations 
//  Created by SSH, 29 Aug 2011.
//
//////////////////////////////////////////////

#include "ZeeDHist/ZeeDHistManager.h"
#include "TString.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

class ZeeDBosonZ;

/** ZeeDHistManagerTrigger */

class ZeeDHistManagerTrigger : public ZeeDHistManager {
  public:

    explicit ZeeDHistManagerTrigger(TString name)
      : ZeeDHistManager(name),
      fSvcHelper("ZeeDAnalysisCutHistManagerZCC"),
      fAnaOptions(fSvcHelper.GetAnaSvc())
  {}

    // Book all histograms:
    virtual void BookHistos();

    // Arrange loop over Z,W bosons:
    using ZeeDHistManager::Fill;
    virtual void Fill();

    // Fill tigger histos for individual boson
    virtual void FillTriggerHists(const ZeeDBosonZ* boson, Double_t Weight);


  private:

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;      //!

};

#endif //ZeeDHistManagerTrigger

