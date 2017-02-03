#ifndef __ZEEDHISTMANAGERYIELD_H
#define __ZEEDHISTMANAGERYIELD_H

///////////////////////////////////////////////
//  Name ZeeDHistManagerYield
//  Basic histograms for Yield control
//  Created by SSH, 03 Nov 2011.
//
//////////////////////////////////////////////

#include "ZeeDHist/ZeeDHistManager.h"
#include "TString.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

class ZeeDBoson;

class ZeeDHistManagerYield : public ZeeDHistManager {
  public:

    explicit ZeeDHistManagerYield(TString name)
      : ZeeDHistManager(name), 
      fSvcHelper("ZeeDAnalysisCutHistManagerYield"),
      fAnaOptions(fSvcHelper.GetAnaSvc())
  {}

    // Book all histograms:
    virtual void BookHistos();

    // Arrange loop over Z,W bosons:
    using ZeeDHistManager::Fill;
    virtual void Fill();

    // Fill tigger histos for individual boson
    virtual void FillYieldHists(Int_t Run, Int_t LB, Double_t Weight);


  private:
    
    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;

    std::map<int, float> lumies; // map<run, lumi>

};

#endif //ZeeDHistManagerYield

