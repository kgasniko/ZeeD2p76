#ifndef __ZEEDHISTMANAGERZJET_H
#define __ZEEDHISTMANAGERZJET_H

///////////////////////////////////////////////
//  Name ZeeDHistManagerZjet
//  Basic histograms for Z+jet balance candidates
//  Created by P.S.,  02-02-2012.
//
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
class ZeeDHistManagerZjet : public ZeeDHistManager {
public:

  ZeeDHistManagerZjet(TString name ,  Double_t Ymin = 0., Double_t Ymax = 0.)
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
      Bool_t isSubLeadingJetVeto;
      Int_t leadingJetIndex, subLeadingJetIndex;

      TLorentzVector bosonMomentum, Electron1Momentum, Electron2Momentum;

      Double_t ptReferenceCosPhi, ptBalanceCosPhi ;
      Double_t leadingJetDeltaPhi, subLeadingJetDeltaPhi ;


      std::map<std::string,Double_t> deltaRCutMap;

      Double_t deltaRcut;

      Double_t bosonPtCut;

      Double_t jetPtCut, _jetPtCut;
      Double_t subJetPtCut, _subJetPtCut;
      Double_t jetJVFCut, _jetJVFCut;
      Double_t subJetJVFCut, _subJetJVFCut;
      Double_t trackingAcceptance;

      Double_t deltaJetPtCut, deltaJVFCut;

      ZeeDSvcHelper fSvcHelper;
      ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;      //!

      Bool_t isLeadingJet( ZeeDJet*& , Bool_t debugFlag = false);
      Bool_t isSubLeadingJet( ZeeDJet*& , Bool_t debugFlag = false);

      void GetMatchedTruth( ZeeDJet*& reco);


      std::vector<Double_t> deltaRBins;
      Double_t truncatedArea(double etaJ, double phiJ, double R, double eta0 = 2.5 );
      void  bookAnalysisCuts();

};

#endif //ZeeDHistManagerZjet

