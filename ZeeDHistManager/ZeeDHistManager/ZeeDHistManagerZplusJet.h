#ifndef __ZEEDHISTMANAGERZPLUSJET_H
#define __ZEEDHISTMANAGERZPLUSJET_H

///////////////////////////////////////////////
//  Name ZeeDHistManagerZplusJet

//////////////////////////////////////////////

#include "ZeeDHist/ZeeDHistManager.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "ZeeDEvent/ZeeDBoson.h"
#include "ZeeDEvent/ZeeDJet.h"

#include <TString.h>
#include <TLorentzVector.h>

#include <map>

class ZeeDBoson;
class ZeeDJet;

/** ZeeDHistManagerZplusJet */
class ZeeDHistManagerZplusJet : public ZeeDHistManager {
public:

  ZeeDHistManagerZplusJet(TString name ,  Double_t Ymin = 0., Double_t Ymax = 0.)
    : ZeeDHistManager(name), bIsMC(kFALSE), bIsAll(kFALSE), bIsW(kFALSE), fYmin(Ymin), fYmax(Ymax),
    fSvcHelper("ZeeDAnalysisCutHistManagerZjet"),
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
      
      // Fill histos for jets
      virtual void FillJetHists(const TObjArray* jets, Double_t Weight);
      
      
 private:
   
      std::string fCollectionName, fTruthCollectionName;

      Bool_t bIsMC;
      Bool_t bIsAll;
      Bool_t bIsW;
      Double_t fYmin;
      Double_t fYmax;

      ZeeDJet *leadingJet, *subLeadingJet1,*subLeadingJet2, *matchedTruthJet;
      TLorentzVector bosonMomentum, Electron1Momentum, Electron2Momentum, ElectronTruthMomentum, PositronTruthMomentum;

      TLorentzVector MatchedElectron1Momentum, MatchedElectron2Momentum;
      bool matched_el1;
      bool matched_el2;

      std::map<std::string,Double_t> deltaRCutMap;
      std::map<std::string,Double_t> jetConeMap;
      Double_t deltaRcut;
      Double_t jetCone;

      Double_t bosonPtCut;
      Double_t minDR;

      Double_t jetPtCut;
      Double_t jetJVFCut;
      Double_t jetPtJVFCut;
      Double_t jetYCut;
      Double_t trackingAcceptance;

      ZeeDSvcHelper fSvcHelper;
      ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;      //!


      void GetMatchedTruth( ZeeDJet*& reco );

      void GetMinDR(  TLorentzVector electronRecoMomentum,TLorentzVector electronTruthMomentum, TString collectionName );
      void GetMinDRInData(  TLorentzVector electronRecoMomentum, TString collectionName );

      Double_t minDRElReco, minDRElTruth;

      void  bookAnalysisCuts();
};

#endif //ZeeDHistManagerZplusJet

