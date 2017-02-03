#ifndef __ZEEDHISTMANAGERJETCHARGE_H
#define __ZEEDHISTMANAGERJETCHARGE_H

///////////////////////////////////////////////
//  Name ZeeDHistManagerJetCharge

//////////////////////////////////////////////

#include "ZeeDHist/ZeeDHistManager.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"


#include <TString.h>
#include <TLorentzVector.h>

#include <map>

class ZeeDBoson;
class ZeeDJet;

/** ZeeDHistManagerBoson */
class ZeeDHistManagerJetCharge : public ZeeDHistManager {
public:

  ZeeDHistManagerJetCharge(TString name ,  Double_t Ymin = 0., Double_t Ymax = 0.)
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
      
      // Fill histos for individual boson
      virtual void FillJetHists(const TObjArray* jets, Double_t Weight);
      
      
 private:
   
      std::string fCollectionName, fTruthCollectionName;

      Bool_t bIsMC;
      Bool_t bIsAll;
      Bool_t bIsW;
      Double_t fYmin;
      Double_t fYmax;

      ZeeDJet *leadingJet, *subLeadingJet, *matchedTruthJet;

      TLorentzVector bosonMomentum, Electron1Momentum, Electron2Momentum;

      std::map<std::string,Double_t> deltaRCutMap;
      std::map<std::string,Double_t> jetConeMap;

      Double_t deltaRcut, jetCone;
      Double_t bosonPtCut;

      ZeeDSvcHelper fSvcHelper;
      ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;      //!

      Bool_t isLeadingJet( ZeeDJet*& , Bool_t debugFlag = false);
      Bool_t isSubLeadingJet( ZeeDJet*& , Bool_t debugFlag = false);

      void GetMatchedTruth( ZeeDJet*& reco);

      std::vector<Double_t> deltaRBins;
      Double_t truncatedArea(double etaJ, double phiJ, double R, double eta0 = 2.5 );
      void  bookAnalysisCuts();



};

#endif //ZeeDHistManagerJetCharge
