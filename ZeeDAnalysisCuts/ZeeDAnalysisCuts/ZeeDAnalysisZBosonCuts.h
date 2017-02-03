#pragma once

////////////////////////////////////////////////////////
// Name    : ZeeDAnalysisCuts.h
////////////////////////////////////////////////////////

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDBags.h"
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDCuts/ZeeDCut.h"

// Root includes
#include <TMath.h>
#include <TString.h>
#include <TObjArray.h>

// ZeeD Analysis includes
#include "ZeeDEvent/ZeeDVertex.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDEvent.h"


// -----------------------------------------------------------------------------
//              Z boson analysis cuts
// -----------------------------------------------------------------------------

//-----------------------------------------------

/**  cut on minimal pt of the electrons */
class ZeeDCutPtMinCentralLepZ : public ZeeDCut {
public:
    ZeeDCutPtMinCentralLepZ(TString CutName, Double_t cutval)
        : ZeeDCut (CutName) {
        m_cutval = cutval;
    }
    Bool_t evaluate(const ZeeDEvent* event);

private:
    Double_t m_cutval;
};

//-----------------------------------------------

/**  cut on minimal pt of the electrons */
class ZeeDCutPtMinForwardLepZ : public ZeeDCut {
public:
    ZeeDCutPtMinForwardLepZ(TString CutName, Double_t cutval)
        : ZeeDCut (CutName) {
        m_cutval = cutval;
    }
    Bool_t evaluate(const ZeeDEvent* event);

private:
    Double_t m_cutval;
};


//-----------------------------------------------

/**  cut on maximal eta of the electrons */
class ZeeDCutEtaMaxBothElecZ : public ZeeDCut {
public:
    ZeeDCutEtaMaxBothElecZ(TString CutName, Double_t cutval) : ZeeDCut (CutName) {
        m_cutval = cutval;
    }
    Bool_t evaluate(const ZeeDEvent* event);
private:
    Double_t m_cutval;
};

/**  cut on maximal eta of the electrons */
class ZeeDCutEtaMaxBothLepZ : public ZeeDCut {
public:
    ZeeDCutEtaMaxBothLepZ(TString CutName, Double_t cutval) : ZeeDCut (CutName) {
        m_cutval = cutval;
    }
    Bool_t evaluate(const ZeeDEvent* event);
private:
    Double_t m_cutval;
};


//-----------------------------------------------

/**  cut on eta of the electrons in CF region */
class ZeeDCutEtaCFBothElecZ : public ZeeDCut {
public:
    ZeeDCutEtaCFBothElecZ(TString CutName, Double_t cutvalmin, Double_t cutvalmax, Double_t cutvalmaxEMEC) : ZeeDCut (CutName)  {
        m_cutvalmin = cutvalmin;
        m_cutvalmax = cutvalmax;
        m_cutvalmaxEMEC = cutvalmaxEMEC;
    }
    Bool_t evaluate(const ZeeDEvent* event);
private:
    Double_t m_cutvalmin;
    Double_t m_cutvalmax;
    Double_t m_cutvalmaxEMEC;
};

//-----------------------------------------------

/**  cut on eta of the electrons in FF region */
class ZeeDCutEtaFFBothElecZ : public ZeeDCut {
public:
    ZeeDCutEtaFFBothElecZ(TString CutName, Double_t cutval) : ZeeDCut (CutName) {
        m_cutval = cutval;
    }
    Bool_t evaluate(const ZeeDEvent* event);
private:
    Double_t m_cutval;
};

//-----------------------------------------------

/**  both electron tracks are found */
class ZeeDCutTrackBothLepZ : public ZeeDCut {
public:

    ZeeDCutTrackBothLepZ(TString CutName) : ZeeDCut (CutName) {}

    Bool_t evaluate(const ZeeDEvent* event);
};

//-----------------------------------------------

/**  Central electron tracks is found */
class ZeeDCutTrackCentralLepZ : public ZeeDCut {
public:

    ZeeDCutTrackCentralLepZ(TString CutName) : ZeeDCut (CutName) {}

    Bool_t evaluate(const ZeeDEvent* event);
};

//-----------------------------------------------
class ZeeDCutOQMaps : public ZeeDCut {
public:

    ZeeDCutOQMaps(TString CutName, Int_t type = 0) : ZeeDCut (CutName),
        m_type(type) {}

    Bool_t evaluate(const ZeeDEvent* event);

private:

    Int_t m_type;

};

//-----------------------------------------------

/**  both electron tracks are found */
class ZeeDCutVertexPresentBothLepZ : public ZeeDCut {
public:
    ZeeDCutVertexPresentBothLepZ(TString CutName, Double_t cutval) : ZeeDCut (CutName) {
        m_cutval = cutval;
    }
    Bool_t evaluate(const ZeeDEvent* event);
private:
    Double_t m_cutval;
};

//-----------------------------------------------

/**  Cut on the charge of the two electrons. */
class ZeeDCutChargeBothLepZ : public ZeeDCut {
public:
    /** Takes two boolean arguments. true correponds to a positive
        charge, false to a negative. The following combinations are possible:
        true/true (both electrons positive), false/false (both negative),
        true/false false/true (one electron positive, one negative). The last
        is the default. Note that false/true and true/false are equivalent.

        @param c_e1 - required charge of electron
        @param c_e2 - required charge other electron

     */
    ZeeDCutChargeBothLepZ(TString CutName,
                           Bool_t c_e1 = false,
                           Bool_t c_e2 = true
                          ) : ZeeDCut (CutName),
        bcharge_e1(c_e1),
        bcharge_e2(c_e2) {}

    Bool_t evaluate(const ZeeDEvent* event);

private:

    // true/false means positive/negative
    const Bool_t bcharge_e1;

    // true/false means positive/negative
    const Bool_t bcharge_e2;
};

//-----------------------------------------------
/**  Cut on the charge of the two electrons. */
class ZeeDCutSameChargeBothLepZ : public ZeeDCut {
public:

    ZeeDCutSameChargeBothLepZ(TString CutName) : ZeeDCut (CutName) {}

    Bool_t evaluate(const ZeeDEvent* event);

};

//-----------------------------------------------
/**  Isolation cut "caloIso" for one electron */
class ZeeDCutCentralElecCaloIso : public ZeeDCut {
public:

    ZeeDCutCentralElecCaloIso(TString CutName,
    			            Bool_t ZeeDElectronBags::CaloIso::*egammaIsolationType): ZeeDCut (CutName),
	pegammaIsolationType(egammaIsolationType){}

    Bool_t evaluate(const ZeeDEvent* event);

private:

    Bool_t ZeeDElectronBags::CaloIso::* pegammaIsolationType;
};

//-----------------------------------------------
/**  Isolation cut "caloIso" for one electron */
class ZeeDCutCentralElecIso : public ZeeDCut {
public:

    ZeeDCutCentralElecIso(TString CutName,
    			            Bool_t ZeeDElectronBags::EIso::*egammaIsolationType): ZeeDCut (CutName),
	pegammaIsolationType(egammaIsolationType){}

    Bool_t evaluate(const ZeeDEvent* event);

private:

    Bool_t ZeeDElectronBags::EIso::* pegammaIsolationType;
};

//-----------------------------------------------
/**  Isolation cut "caloIso" for one electron */
class ZeeDCutBothElecIso : public ZeeDCut {
public:

    ZeeDCutBothElecIso(TString CutName,
    			            Bool_t ZeeDElectronBags::EIso::*egammaIsolationType): ZeeDCut (CutName),
	pegammaIsolationType(egammaIsolationType){}

    Bool_t evaluate(const ZeeDEvent* event);

private:

    Bool_t ZeeDElectronBags::EIso::* pegammaIsolationType;
};

//-----------------------------------------------
/**  Isolation cut "caloIso" for both electron */
class ZeeDCutBothCentralElecCaloIso : public ZeeDCut {
public:

    ZeeDCutBothCentralElecCaloIso(TString CutName,
    			            Bool_t ZeeDElectronBags::CaloIso::*egammaIsolationType,
                                    Int_t type = 0) : ZeeDCut (CutName),
	pegammaIsolationType(egammaIsolationType),
        m_type(type) {}

    Bool_t evaluate(const ZeeDEvent* event);

private:

    Bool_t ZeeDElectronBags::CaloIso::* pegammaIsolationType;
    Int_t m_type;

};

//-----------------------------------------------
/**  invariant mass cut */
class ZeeDCutZMassZ : public ZeeDCut {
public:
    ZeeDCutZMassZ(TString CutName, Double_t mLow, Double_t mHigh) : ZeeDCut (CutName) {
        m_mwlow  = mLow;
        m_mwhigh = mHigh;
    }
    Bool_t evaluate(const ZeeDEvent* event);
private:
    Double_t m_mwlow;
    Double_t m_mwhigh;
};

//-----------------------------------------------

/**  cut away transition EMEC-EMB */
class ZeeDCutEtaCrackBothElecZ : public ZeeDCut {
public:
    ZeeDCutEtaCrackBothElecZ(TString CutName, Double_t cutval1, Double_t cutval2)
        : ZeeDCut (CutName) {
        m_cutmin = cutval1;
        m_cutmax = cutval2;
    }
    Bool_t evaluate(const ZeeDEvent* event);
private:
    Double_t m_cutmin;
    Double_t m_cutmax;
};

//-----------------------------------------------
class ZeeDCutIsEMOneLepZ : public ZeeDCut {

public:

    ZeeDCutIsEMOneLepZ(TString CutName,
                        Bool_t ZeeDLeptonBags::IsEM::*IsEMType,
                        Bool_t decision_IsEMType = kTRUE)
  : ZeeDCut(CutName),
    pIsEMType(IsEMType),
    bDecision_IsEMType(decision_IsEMType)
    {}

    Bool_t evaluate(const ZeeDEvent* event);

private:

    Bool_t ZeeDLeptonBags::IsEM::* pIsEMType;
    Bool_t bDecision_IsEMType;

};

//-----------------------------------------------
class ZeeDCutIsEMOnlyOneLepZ : public ZeeDCut {

public:

    ZeeDCutIsEMOnlyOneLepZ(TString CutName,
                        Bool_t ZeeDLeptonBags::IsEM::*IsEMType)
  : ZeeDCut(CutName),
    pIsEMType(IsEMType)
    {}

    Bool_t evaluate(const ZeeDEvent* event);

private:

    Bool_t ZeeDLeptonBags::IsEM::* pIsEMType;

};

//-----------------------------------------------
class ZeeDCutCheckIDBitsBothElec : public ZeeDCut {

  public:

    ZeeDCutCheckIDBitsBothElec(TString CutName)

      : ZeeDCut(CutName)
      {}

    Bool_t evaluate(const ZeeDEvent* event);

};

//-----------------------------------------------
class ZeeDCutIsEMBothLepZ : public ZeeDCut {

public:

    ZeeDCutIsEMBothLepZ(TString CutName,
                         Bool_t ZeeDLeptonBags::IsEM::*IsEMType,
                         Int_t type = 0) :  ZeeDCut(CutName),
        pIsEMType(IsEMType),
        m_type(type) {}

    Bool_t evaluate(const ZeeDEvent* event);

private:

    Bool_t ZeeDLeptonBags::IsEM::* pIsEMType;
    Int_t m_type;

};

//-----------------------------------------------
class ZeeDCutIDCutOneElecZ : public ZeeDCut {

public:

    ZeeDCutIDCutOneElecZ(TString CutName,
                        Bool_t ZeeDElectronBags::IDCut::*IDCutType,
                        Bool_t decision_IDCutType = kTRUE)
  : ZeeDCut(CutName),
    pIDCutType(IDCutType),
    bDecision_IDCutType(decision_IDCutType)
    {}

    Bool_t evaluate(const ZeeDEvent* event);

private:

    Bool_t ZeeDElectronBags::IDCut::* pIDCutType;
    Bool_t bDecision_IDCutType;

};

//-----------------------------------------------
class ZeeDCutIDCutBothElecZ : public ZeeDCut {

public:

    ZeeDCutIDCutBothElecZ(TString CutName,
                          Bool_t ZeeDElectronBags::IDCut::*IDCutType,
                          Bool_t decision_IDCutType = kTRUE)
  : ZeeDCut(CutName),
    pIDCutType(IDCutType),
    bDecision_IDCutType(decision_IDCutType) {}

    Bool_t evaluate(const ZeeDEvent* event);

private:

    Bool_t ZeeDElectronBags::IDCut::* pIDCutType;
    Bool_t bDecision_IDCutType;

};

//-----------------------------------------------

/**  ISEM requirement for a single electron  */
class ZeeDCutIsEMIndexLeptonZ : public ZeeDCut {
public:
    ZeeDCutIsEMIndexLeptonZ(TString CutName,
                             Int_t LepronID,
                             Bool_t ZeeDLeptonBags::IsEM::*IsEMType)
        : ZeeDCut (CutName),
          m_LeptonIdx(LepronID),
          pIsEMType(IsEMType) {}

    Bool_t evaluate(const ZeeDEvent* event);

private:
    Int_t m_LeptonIdx;       // 0 or 1, for the first or second electron

    Bool_t ZeeDLeptonBags::IsEM::* pIsEMType;
};

//-----------------------------------------------

/**  Author requirement for electrons  */
class ZeeDCutAuthorBothElecZ : public ZeeDCut {
public:
    ZeeDCutAuthorBothElecZ(TString CutName) : ZeeDCut (CutName) {}
    Bool_t evaluate(const ZeeDEvent* event);
};

//-----------------------------------------------
/** ISEM for central or forward electron */
class ZeeDCutIsEMCentralForwardLepZ : public ZeeDCut {

public:

    ZeeDCutIsEMCentralForwardLepZ(TString CutName,
                                   Bool_t ZeeDLeptonBags::IsEM::*IsEMType,
                                   ZeeDEnum::Direction::Value type) :  ZeeDCut(CutName),
        pIsEMType(IsEMType),
        m_type(type) {}

    Bool_t evaluate(const ZeeDEvent* event);

private:

    Bool_t ZeeDLeptonBags::IsEM::* pIsEMType;
    ZeeDEnum::Direction::Value m_type;

};

//-----------------------------------------------

/**  Author central-forward for electrons  */
class ZeeDCutAuthorCFElecZ : public ZeeDCut {
public:
    ZeeDCutAuthorCFElecZ(TString CutName) : ZeeDCut (CutName) {}
    Bool_t evaluate(const ZeeDEvent* event);
};

//-----------------------------------------------



/**  both electrons match truth */
class ZeeDCutTruthMatchBothElecZ : public ZeeDCut {
public:
    ZeeDCutTruthMatchBothElecZ(TString CutName) : ZeeDCut (CutName) {}
    Bool_t evaluate(const ZeeDEvent* event);
};


//-----------------------------------------------
/**  Event passed 2012 CC trigger */
class ZeeDCutTriggerWithMatching2012CC: public ZeeDCut {
public:
    ZeeDCutTriggerWithMatching2012CC(TString CutName, Bool_t ismc) :
        ZeeDCut (CutName),
        isMC (ismc) {}
    Bool_t evaluate(const ZeeDEvent* event);

private:
    Bool_t isMC;
};


//-----------------------------------------------
/**  Event passed 2012 CF trigger */
class ZeeDCutTriggerWithMatching2012CF: public ZeeDCut {
public:
    ZeeDCutTriggerWithMatching2012CF(TString CutName) :
        ZeeDCut (CutName) {}
    Bool_t evaluate(const ZeeDEvent* event);
};


//-----------------------------------------------
/**  Event passed trigger */
class ZeeDCutTriggerWithMatching : public ZeeDCut {
public:
    ZeeDCutTriggerWithMatching(TString CutName,
                     ZeeDEnum::Trigger::Value triggerChain = ZeeDEnum::Trigger::UNKNOWN) :
        ZeeDCut (CutName),
        fTriggerChain(triggerChain) {}
    Bool_t evaluate(const ZeeDEvent* event);

private:
    ZeeDEnum::Trigger::Value fTriggerChain;
};


//-----------------------------------------------
class ZeeDCutIsEMIsEMCaloIsoOneElecZ : public ZeeDCut {

 public:

    ZeeDCutIsEMIsEMCaloIsoOneElecZ(TString CutName,
                                   Bool_t decision_IsEMType1, // if true, isEM is required, if false, isEM is forbidden
                                   Bool_t ZeeDLeptonBags::IsEM::*IsEMType1,
                                   Bool_t decision_IsEMType2,
                                   Bool_t ZeeDLeptonBags::IsEM::*IsEMType2,
                                   Bool_t decision_CaloIso,
                                   Bool_t ZeeDElectronBags::CaloIso::*egammaIsolationType) :
        ZeeDCut(CutName),
        bDecision_IsEMType1(decision_IsEMType1),
        fIsEMType1(IsEMType1),
        bDecision_IsEMType2(decision_IsEMType2),
        fIsEMType2(IsEMType2),
        bDecision_CaloIso(decision_CaloIso),
        fEgammaIsolationType(egammaIsolationType) {}

    Bool_t evaluate(const ZeeDEvent* event);

 private:

    Bool_t bDecision_IsEMType1;
    Bool_t ZeeDLeptonBags::IsEM::*fIsEMType1;
    Bool_t bDecision_IsEMType2;
    Bool_t ZeeDLeptonBags::IsEM::*fIsEMType2;
    Bool_t bDecision_CaloIso;
    Bool_t ZeeDElectronBags::CaloIso::*fEgammaIsolationType;

};

