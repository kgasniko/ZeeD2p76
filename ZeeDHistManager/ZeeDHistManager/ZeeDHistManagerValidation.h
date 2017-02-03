#ifndef __ZEEDHISTMANAGERVALIDATION_H
#define __ZEEDHISTMANAGERVALIDATION_H

///////////////////////////////////////////////
//  Name ZeeDHistManagerValidation
//  Basic histograms for validation purposes
//
//////////////////////////////////////////////

#include "ZeeDHist/ZeeDHistManager.h"
#include "TString.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

class ZeeDBosonZ;
class ZeeDJet;
class ZeeDSystematics;

/** ZeeDHistManagerBoson */
class ZeeDHistManagerValidation : public ZeeDHistManager {
public:

    ZeeDHistManagerValidation(TString name, Double_t Ymin = 0., Double_t Ymax = 0.)
        : ZeeDHistManager(name), bIsMC(kFALSE), bIsAll(kFALSE), bIsW(kFALSE), fYmin(Ymin), fYmax(Ymax),
	  fSvcHelper("ZeeDAnalysisCutHistManagerZCC"),
          fAnaOptions(fSvcHelper.GetAnaSvc())
    {}

    void SetProperties(Bool_t isAll, Bool_t isW) {
        bIsAll = isAll;
        bIsW = isW;
    }

    // Book all histograms:
    virtual void BookHistos();

    // Arrange loop over Z,W bosons:
    using ZeeDHistManager::Fill;
    virtual void Fill();

    // Fill histos for individual boson
    virtual void FillBosonHists(const ZeeDBosonZ* boson, Double_t Weight);
    
    // Fill histos for list of electrons
    virtual void FillElectronHists(const TObjArray* electrons, Double_t Weight); 
    
    // Fill histos for list of event
    virtual void FillEventHists(const ZeeDEvent* event, Double_t Weight);  
    
    // Fill histos for EtMiss
    virtual void FillEtMissHists(const TObjArray* etmissarr, Double_t Weight);
      
    // Fill histos for jets
    virtual void FillJetHists(const TObjArray* jets, Double_t Weight);


private:
    Bool_t bIsMC;
    Bool_t bIsAll;
    Bool_t bIsW;
    Double_t fYmin;
    Double_t fYmax;
    ZeeDSystematics* fSys;

    std::string fCollectionName, fTruthCollectionName;
    
    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;      //!

};

#endif //ZeeDHistManagerValidation

