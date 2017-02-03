#pragma once

////////////////////////////////////////////////////////
// Name    : ZeeDAnalysisCutsW.h
////////////////////////////////////////////////////////

/* Cuts for W->enu analysis
*/

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDBags.h"
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDCuts/ZeeDCut.h"
#include "TString.h"

// Root includes
#include <TMath.h>
#include <TString.h>
#include <TObjArray.h>

// ZeeD Analysis includes
#include "ZeeDEvent/ZeeDBosonW.h"
#include "ZeeDEvent/ZeeDVertex.h"
#include "ZeeDEvent/ZeeDJet.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDMuon.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDGenParticle.h"

//#define USE_DEBUG_MESSAGES
//#define DEBUG_ON 1
#include "ZeeDMisc/ZeeDMisc.h"

#include "ZeeDEvent/ZeeDEvent.h"

class ZeeDCutMuonD0W : public ZeeDCut{
    public:
        ZeeDCutMuonD0W (TString cutName, Double_t cut) : ZeeDCut (cutName){
            m_cutval = cut;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private :
        Double_t m_cutval;
};

class ZeeDCutCombinedMuon: public ZeeDCut{
    public:
        ZeeDCutCombinedMuon (TString cutName, Bool_t dec = kTRUE): ZeeDCut(cutName){
            bDecision = dec;}
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Bool_t bDecision;
};

class ZeeDCutMuonHasIDTrack: public ZeeDCut{
    public:
        ZeeDCutMuonHasIDTrack (TString cutName): ZeeDCut(cutName) {;};
        Bool_t evaluate(const ZeeDEvent* event);
};

class ZeeDCutMuonTrackIso: public ZeeDCut{
    public:
        ZeeDCutMuonTrackIso(TString cutName, Double_t cut, Bool_t dec = kTRUE) : ZeeDCut (cutName){
            m_cutval = cut;
            bDecision = dec;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private :
        Double_t m_cutval;
        Bool_t bDecision;
};



/** Cut on isolation criteria for electron */
class ZeeDCutEtCone30overEtW : public ZeeDCut {
    public:
        ZeeDCutEtCone30overEtW (TString cutName, Double_t cut) : ZeeDCut (cutName){
            m_EtCut = cut;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t  m_EtCut;
};

class ZeeDCutCrackLepTrackEtaW : public ZeeDCut {
    public:
        ZeeDCutCrackLepTrackEtaW (TString cutName, Double_t min, Double_t max) : ZeeDCut (cutName){
            m_min = min;
            m_max = max;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t  m_min, m_max;
};

class ZeeDCutLepTrackEtaW : public ZeeDCut {
    public:
        ZeeDCutLepTrackEtaW (TString cutName, Double_t cut) : ZeeDCut (cutName){
            m_cutval = cut;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t  m_cutval;
};

class ZeeDCutLepTrackEtaWSim : public ZeeDCut {
    public:
        ZeeDCutLepTrackEtaWSim (TString cutName) : ZeeDCut (cutName){
        }
        Bool_t evaluate(const ZeeDEvent* event);
};




/** Cut on isolation criteria for electron */
class ZeeDCutEtCone20overEtW : public ZeeDCut {
    public:
        ZeeDCutEtCone20overEtW (TString cutName, Double_t cut) : ZeeDCut (cutName){
            m_EtCut = cut;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t  m_EtCut;
};

/** Cut on isolation criteria for electron */
class ZeeDCutEtCone30overEtCorW : public ZeeDCut {
    public:
        ZeeDCutEtCone30overEtCorW (TString cutName, Double_t cut) : ZeeDCut (cutName){
            m_EtCut = cut;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t  m_EtCut;
};

/** Cut on isolation criteria for electron */
class ZeeDCutEtCone20overEtCorW : public ZeeDCut {
    public:
        ZeeDCutEtCone20overEtCorW (TString cutName, Double_t cut) : ZeeDCut (cutName){
            m_EtCut = cut;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t  m_EtCut;
};


/** Cut on electron trigger */
class ZeeDCutTrigLepW : public ZeeDCut {
    public:
        ZeeDCutTrigLepW(TString cutName, ZeeDEnum::Trigger::Value chain) : ZeeDCut (cutName){
            fChain= chain;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        ZeeDEnum::Trigger::Value fChain;
};

/** Cut on transvers mass of W */
class ZeeDCutMassTransvW: public ZeeDCut{
    public: 
        ZeeDCutMassTransvW(TString cutName, Double_t massTransv) : ZeeDCut(cutName),
        m_MassTrCut(massTransv)
    {}
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t m_MassTrCut;
};

/** Cut on electron identification */
class ZeeDCutLepIdentW: public ZeeDCut {
    public: 
        ZeeDCutLepIdentW(TString CutName, 
                Bool_t ZeeDLeptonBags::IsEM::*IsEMType,
                Bool_t decision_IsEMType = kTRUE)
            :ZeeDCut(CutName),
            pIsEMType(IsEMType),
            bDecision_IsEMType(decision_IsEMType)
    {}

        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Bool_t ZeeDLeptonBags::IsEM::* pIsEMType;
        Bool_t bDecision_IsEMType;
};


//-----------------------------------------------
/**  Cut on minimal pt of the electron */
class ZeeDCutLepPtMinW : public ZeeDCut {
    public:
        ZeeDCutLepPtMinW(TString cutName, Double_t cutval) : ZeeDCut (cutName) {
            m_cutval = cutval;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t m_cutval;
};


//-----------------------------------------------
/**  Cut on minimal et of the electron cluster */
class ZeeDCutElecClustEtMinW : public ZeeDCut {
    public:
        ZeeDCutElecClustEtMinW(TString cutName, Double_t cutval) : ZeeDCut (cutName) {
            m_cutval = cutval;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t m_cutval;
};

//-----------------------------------------------
/**  Cut on minimal et miss */
/*class ZeeDCutEtMissMinW : public ZeeDCut {
  public:
  ZeeDCutEtMissMinW(TString cutName, Double_t cutval) : ZeeDCut (cutName) {
  m_cutval = cutval;
  }
  Bool_t evaluate(const ZeeDEvent* event);
  private:
  Double_t m_cutval;
  };
  */
//-----------------------------------------------
/**  Author requirement for electron  */
class ZeeDCutElecAuthorW: public ZeeDCut {
    public:
        ZeeDCutElecAuthorW(TString cutName) : ZeeDCut (cutName) {}
        Bool_t evaluate(const ZeeDEvent* event);
};

//-----------------------------------------------
/**  Cut away transition EMEC-EMB */
class ZeeDCutEtaCrackElecW : public ZeeDCut {
    public:
        ZeeDCutEtaCrackElecW(TString cutName, Double_t cutValMin, Double_t cutValMax) : ZeeDCut (cutName) {
            m_cutmin = cutValMin;
            m_cutmax = cutValMax;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t m_cutmin;
        Double_t m_cutmax;
};

//-----------------------------------------------
/**  Cut on maximal eta of the electron */
class ZeeDCutElecClustEtaMaxW : public ZeeDCut {
    public:
        ZeeDCutElecClustEtaMaxW(TString cutName, Double_t cutval) : ZeeDCut (cutName) {
            m_cutval = cutval;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t m_cutval;
};

//-----------------------------------------------
/**  Electron fired by default trigger */
class ZeeDCutDefaultTrigW : public ZeeDCut {
    public:
        ZeeDCutDefaultTrigW(TString CutName) : ZeeDCut (CutName) {}
        Bool_t evaluate(const ZeeDEvent* event);
};

//-----------------------------------------------
/**  Electron fiducial cuts */
class ZeeDCutOQMapsW : public ZeeDCut {
    public:
        ZeeDCutOQMapsW(TString cutName) : ZeeDCut (cutName) {}
        Bool_t evaluate(const ZeeDEvent* event);
};

/*class ZeeDCutTriggerWithMatchingW : public ZeeDCut {
  public: 
  ZeeDCutTriggerWithMatchingW(TString CutName, ZeeDEnum::Trigger::Value triggerChain = ZeeDEnum::Trigger::UNKNOWN) :
  ZeeDCut (CutName),
  fTriggerChain(triggerChain) {}
  Bool_t evaluate(const ZeeDEvent* event);

  private:
  ZeeDEnum::Trigger::Value fTriggerChain;
  };*/



//-----------------------------------------------
/**  Cut on the charge of the electron. */
class ZeeDCutLepChargePosW : public ZeeDCut {
    public:
        ZeeDCutLepChargePosW(TString CutName,
                Bool_t isPos = kTRUE) : ZeeDCut (CutName), bIsPos(isPos) {}

        Bool_t evaluate(const ZeeDEvent* event);

    private:

        // bool flag
        const Bool_t bIsPos;

};

