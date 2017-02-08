#pragma once

////////////////////////////////////////////////////////
// Name    : ZeeDAnalysisCuts.h
////////////////////////////////////////////////////////

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
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDJet.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDGenParticle.h"

//#define USE_DEBUG_MESSAGES
//#define DEBUG_ON 1
#include "ZeeDMisc/ZeeDMisc.h"

#include "ZeeDEvent/ZeeDEvent.h"

class ZeeDCutNumberGoodW: public ZeeDCut{
    public:
        ZeeDCutNumberGoodW(TString CutName, UInt_t numbW): ZeeDCut(CutName), m_numbW(numbW) 
    {}
       Bool_t evaluate(const ZeeDEvent* event);
    private:
        UInt_t m_numbW;
};

class ZeeDCutNumberGoodWmu: public ZeeDCut{
    public:
        ZeeDCutNumberGoodWmu(TString CutName, UInt_t numbW): ZeeDCut(CutName), m_numbW(numbW) 
    {}
       Bool_t evaluate(const ZeeDEvent* event);
    private:
        UInt_t m_numbW;
};


//-----------------------------------------------
/**  at least 1 loose Z */

class ZeeDCutExistElectrons: public ZeeDCut{
    public:
        ZeeDCutExistElectrons(TString CutName, UInt_t nElectrons) : ZeeDCut (CutName), fNElectrons(nElectrons) 
    {}

        Bool_t evaluate (const ZeeDEvent* event);
    private:
        UInt_t fNElectrons;
};

class ZeeDCutNumberVert: public ZeeDCut{
    public: 
        ZeeDCutNumberVert(TString CutName, UInt_t nVert): ZeeDCut(CutName), fNVert(nVert)
    {}
        Bool_t evaluate (const ZeeDEvent* event);
    private:
        UInt_t fNVert;

};

class ZeeDCutCentralElectrons: public ZeeDCut{
    public:
        ZeeDCutCentralElectrons(TString CutName): ZeeDCut(CutName)
    {}
        Bool_t evaluate (const ZeeDEvent* event);
};

class ZeeDCutExistZ : public ZeeDCut {
    public:
        ZeeDCutExistZ(TString CutName) : ZeeDCut (CutName) {}
        Bool_t evaluate(const ZeeDEvent* event);
};

//-----------------------------------------------
class ZeeDCutExistBornBoson : public ZeeDCut {
    public:
        ZeeDCutExistBornBoson(TString CutName) : ZeeDCut (CutName) {}
        Bool_t evaluate(const ZeeDEvent* event);
};

//-----------------------------------------------
class ZeeDCutExistBornElectrons : public ZeeDCut {
    public:
        /**
          @param nElectrons - minimum number of born electrons
          */
        ZeeDCutExistBornElectrons(TString CutName, UInt_t nElectrons) :
            ZeeDCut(CutName),
            fNElectrons(nElectrons)
    {}

        Bool_t evaluate(const ZeeDEvent* event);

    private:

        UInt_t fNElectrons;

};

//-----------------------------------------------
class ZeeDCutPtMinBothBornElecZ : public ZeeDCut {
    public:
        ZeeDCutPtMinBothBornElecZ(TString CutName,
                Double_t cutval) :
            ZeeDCut(CutName),
            m_cutval(cutval)
    {}

        Bool_t evaluate(const ZeeDEvent* event);

    private:
        Double_t m_cutval;
};

//-----------------------------------------------
class ZeeDCutEtaMaxBothBornElecZ : public ZeeDCut {

    public:

        ZeeDCutEtaMaxBothBornElecZ(TString CutName,
                Double_t cutval) :
            ZeeDCut(CutName),
            m_cutval(cutval)
    {}

        Bool_t evaluate(const ZeeDEvent* event);

    private:

        Double_t m_cutval;

};

class ZeeDCutGenWPlus: public ZeeDCut {
    public:
        ZeeDCutGenWPlus(TString CutName, bool IsPos):
            ZeeDCut(CutName), bIsPos(IsPos)
    {}
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Bool_t bIsPos;
};

//-----------------------------------------------
class ZeeDCutEtaCrackBothBornElecZ : public ZeeDCut {
    public:
        ZeeDCutEtaCrackBothBornElecZ(TString CutName,
                Double_t cutval1,
                Double_t cutval2) :
            ZeeDCut (CutName),
            m_cutmin(cutval1),
            m_cutmax(cutval2)

    {}

        Bool_t evaluate(const ZeeDEvent* event);

    private:

        Double_t m_cutmin;
        Double_t m_cutmax;

};


//-----------------------------------------------                                                                                                                                             

/**  cut on eta of the electrons in CF region */
class ZeeDCutEtaCFBothBornElecZ : public ZeeDCut {
    public:
        ZeeDCutEtaCFBothBornElecZ(TString CutName, Double_t cutvalmin, Double_t cutvalmax, Double_t cutvalmaxEMEC) : ZeeDCut (CutName)  {
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


//cut on eta of the electron for generator boson
class ZeeDCutEtaMaxBornElecW : public ZeeDCut {
    public:
        ZeeDCutEtaMaxBornElecW(TString CutName, Double_t etaMax) : ZeeDCut(CutName) {
            m_cutval = etaMax;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t m_cutval;
};

class ZeeDCutPtBornElectronW : public ZeeDCut {
    public:
        ZeeDCutPtBornElectronW(TString CutName, Double_t ptMin) : ZeeDCut(CutName) {
            m_cutval = ptMin;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t m_cutval;
};

class ZeeDCutMassTransvBornW : public ZeeDCut {
    public: 
        ZeeDCutMassTransvBornW(TString CutName, Double_t mtMin) : ZeeDCut(CutName) {
            m_cutval = mtMin;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t m_cutval;
};

class ZeeDCutCrackBornElecW : public ZeeDCut {
    public: 
        ZeeDCutCrackBornElecW (TString CutName) : ZeeDCut(CutName) {}
        Bool_t evaluate(const ZeeDEvent* event);
};

class ZeeDCutEtMissBornW : public ZeeDCut {
    public: 
        ZeeDCutEtMissBornW (TString CutName, Double_t etMin) : ZeeDCut(CutName) {
            m_cutval = etMin;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t m_cutval;
};


//-----------------------------------------------                                                                                                                                             

/**  cut on minimal pt of the electrons */
class ZeeDCutPtMinCentralBornElecZ : public ZeeDCut {
    public:
        ZeeDCutPtMinCentralBornElecZ(TString CutName, Double_t cutval)
            : ZeeDCut (CutName) {
                m_cutval = cutval;
            }
        Bool_t evaluate(const ZeeDEvent* event);

    private:

        Double_t m_cutval;
};

//-----------------------------------------------                                                                                                                                             

/**  cut on minimal pt of the electrons */
class ZeeDCutPtMinForwardBornElecZ : public ZeeDCut {
    public:
        ZeeDCutPtMinForwardBornElecZ(TString CutName, Double_t cutval)
            : ZeeDCut (CutName) {
                m_cutval = cutval;
            }
        Bool_t evaluate(const ZeeDEvent* event);

    private:
        Double_t m_cutval;
};


//-----------------------------------------------
class ZeeDCutMassBornBoson : public ZeeDCut {
    public:
        ZeeDCutMassBornBoson(TString CutName,
                Double_t mLow,
                Double_t mHigh,
                Bool_t mirror=kFALSE) :
            ZeeDCut(CutName),
            m_mwlow(mLow),
            m_mwhigh(mHigh),
            bMirror(mirror)

    {}

        Bool_t evaluate(const ZeeDEvent* event);

    private:

        Double_t m_mwlow;
        Double_t m_mwhigh;
        Int_t  m_type;
        Bool_t bMirror;
};

//-----------------------------------------------
class ZeeDCutYBornBoson : public ZeeDCut {
    public:
        ZeeDCutYBornBoson(TString CutName,
                Double_t yLow,
                Double_t yHigh) :
            ZeeDCut(CutName),
            m_ywlow(yLow),
            m_ywhigh(yHigh)

    {}

        Bool_t evaluate(const ZeeDEvent* event);

    private:

        Double_t m_ywlow;
        Double_t m_ywhigh;
        Int_t  m_type;

};

//-----------------------------------------------
/**  Event passed trigger */
class ZeeDCutTrigger : public ZeeDCut {
    public:
        ZeeDCutTrigger(TString CutName,
                ZeeDEnum::Trigger::Value triggerChain = ZeeDEnum::Trigger::UNKNOWN) :
            ZeeDCut (CutName),
            fTriggerChain(triggerChain) {}
        Bool_t evaluate(const ZeeDEvent* event);

    private:
        ZeeDEnum::Trigger::Value fTriggerChain;
};

//-----------------------------------------------
/**  Event passed one of two triggers, made with CF triggers */
class ZeeDCutTwoTriggersCF : public ZeeDCut {
    public:
        ZeeDCutTwoTriggersCF(TString CutName) :
            ZeeDCut (CutName) {}
        Bool_t evaluate(const ZeeDEvent* event);
};

class ZeeDCutTriggersW : public ZeeDCut {
    public:
        ZeeDCutTriggersW (TString CutName) :
            ZeeDCut (CutName) {}
        Bool_t evaluate(const ZeeDEvent* event);
};


//-----------------------------------------------
/**  select predefined run range **/
class ZeeDCutRunNumber : public ZeeDCut {
    public:
        ZeeDCutRunNumber(TString CutName,
                Int_t runnumber_min, Int_t runnumber_max) :
            ZeeDCut(CutName)
    {
        m_runnumber_min = runnumber_min;
        m_runnumber_max = runnumber_max;
    }

        Bool_t evaluate(const ZeeDEvent* event);

    private:
        Int_t m_runnumber_min;
        Int_t m_runnumber_max;
};


//-----------------------------------------------

/**  cut on minimum vertices in the event */
class ZeeDCutMinNVertex : public ZeeDCut {
    public:
        ZeeDCutMinNVertex(TString CutName, Int_t cutval) : ZeeDCut (CutName) {
            m_cutval = cutval;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t m_cutval;
};


//-----------------------------------------------
/**  cut on minimum Z position of primary vertex */
class ZeeDCutPriVtxZ : public ZeeDCut {
    public:
        ZeeDCutPriVtxZ(TString CutName, Double_t cutval) : ZeeDCut (CutName) {
            m_cutval = cutval;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t m_cutval;
};

//-----------------------------------------------

/**  cut on minimum tracks for the primary vertex */
class ZeeDCutPriVtxNtrack : public ZeeDCut {
    public:
        ZeeDCutPriVtxNtrack(TString CutName, Int_t cutval) : ZeeDCut (CutName) {
            m_cutval = cutval;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t m_cutval;
};


//-----------------------------------------------

/**  cut on extra good electrons */
class ZeeDCutTwoGoodElec : public ZeeDCut {
    public:
        ZeeDCutTwoGoodElec(TString CutName) : ZeeDCut (CutName),
        fSvcHelper("ZeeDCutPtMinCentralBornElecZ"),
        fAnaOptions(fSvcHelper.GetAnaSvc())   {}
        Bool_t evaluate(const ZeeDEvent* event);

    private:

        ZeeDSvcHelper fSvcHelper;
        ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;
};


//-----------------------------------------------
class ZeeDCutCentralForwardLooseGoodElec : public ZeeDCut {
    public:
        ZeeDCutCentralForwardLooseGoodElec(TString CutName) : ZeeDCut (CutName),
        fSvcHelper("ZeeDCutCentralForwardLooseGoodElec"),
        fAnaOptions(fSvcHelper.GetAnaSvc())   {}
        Bool_t evaluate(const ZeeDEvent* event);

    private:

        ZeeDSvcHelper fSvcHelper;
        ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;
};

//-----------------------------------------------
class ZeeDCutCentralForwardTightGoodElec : public ZeeDCut {
    public:
        ZeeDCutCentralForwardTightGoodElec(TString CutName) : ZeeDCut (CutName),
        fSvcHelper("ZeeDCutCentralForwardTightGoodElec"),
        fAnaOptions(fSvcHelper.GetAnaSvc())   {}
        Bool_t evaluate(const ZeeDEvent* event);

    private:

        ZeeDSvcHelper fSvcHelper;
        ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;
};

//-----------------------------------------------
/** Event passed LAr event veto */
class ZeeDCutLArEventVeto : public ZeeDCut {
    public:
        ZeeDCutLArEventVeto(TString CutName) : ZeeDCut(CutName) {}

        Bool_t evaluate(const ZeeDEvent* event);

};

//-----------------------------------------------
/** Event passed GRL cut */
class ZeeDCutGRLEvent : public ZeeDCut {
    public:
        ZeeDCutGRLEvent(TString CutName) : ZeeDCut(CutName) {}

        Bool_t evaluate(const ZeeDEvent* event);

};

class ZeeDCutMinClusterEtNElec : public ZeeDCut {
    public:
        ZeeDCutMinClusterEtNElec(TString CutName,
                UInt_t nElec,
                Double_t minClusterEt,
                Bool_t useRawEvent,
                Bool_t requireIsEM = kTRUE,
                Bool_t ZeeDLeptonBags::IsEM::*IsEMType =
                &ZeeDLeptonBags::IsEM::isMedium )
            : ZeeDCut(CutName),
            fNElec(nElec),
            fMinClusterEt(minClusterEt),
            bRequireIsEM(requireIsEM),
            bUseRawEvent(useRawEvent),
            pIsEMType(IsEMType)
    {}

        Bool_t evaluate(const ZeeDEvent* event);

    private:

        UInt_t fNElec;
        Double_t fMinClusterEt;
        Bool_t bRequireIsEM;
        Bool_t bUseRawEvent;
        Bool_t ZeeDLeptonBags::IsEM::*pIsEMType;

};

//-----------------------------------------------
/**  Cut on missing Et */
class ZeeDCutEventMaxMissEt : public ZeeDCut {
    public:
        ZeeDCutEventMaxMissEt(TString cutName, Double_t cutval) : ZeeDCut (cutName) {
            m_cutval = cutval;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t m_cutval;
};

class ZeeDCutCorRecoilEtMissW: public ZeeDCut {
    public:
        ZeeDCutCorRecoilEtMissW (TString cutName, Double_t cutval) : ZeeDCut (cutName) {
            m_cutval = cutval;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t m_cutval;
};


class ZeeDCutEventMinMissEt : public ZeeDCut {
    public:
        ZeeDCutEventMinMissEt(TString cutName, Double_t cutval) : ZeeDCut (cutName) {
            m_cutval = cutval;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t m_cutval;
};


//-----------------------------------------------
/**  Cut on missing Mt */
class ZeeDCutEventMaxMissMt : public ZeeDCut {
    public:
        ZeeDCutEventMaxMissMt(TString cutName, Double_t cutval) : ZeeDCut (cutName) {
            m_cutval = cutval;
        }
        Bool_t evaluate(const ZeeDEvent* event);
    private:
        Double_t m_cutval;
};

//-----------------------------------------------
class ZeeDCutPeriodEvent : public ZeeDCut {

    public:

        ZeeDCutPeriodEvent(TString CutName, TString period) :  ZeeDCut(CutName), fPeriod(period) {}

        Bool_t evaluate(const ZeeDEvent* event);

    private:

        TString fPeriod;

};

//-----------------------------------------------  
class ZeeDCutPtCone20overEt : public ZeeDCut {
    public:
        ZeeDCutPtCone20overEt(TString CutName, Double_t cutval)
            : ZeeDCut (CutName) {
                m_cutval = cutval;
            }
        Bool_t evaluate(const ZeeDEvent* event);

    private:
        Double_t m_cutval;
};

//-----------------------------------------------  
class ZeeDCutNZeeBosons : public ZeeDCut {
    public:
        ZeeDCutNZeeBosons(TString name, int nbosons) : ZeeDCut(name) {
            m_nbosons = nbosons;
        }

        Bool_t evaluate(const ZeeDEvent* event);

    private:
        int m_nbosons;
};

//-----------------------------------------------  
class ZeeDCutNZemuBosons : public ZeeDCut {
    public:
        ZeeDCutNZemuBosons(TString name, int nbosons) : ZeeDCut(name) {
            m_nbosons = nbosons;
        }

        Bool_t evaluate(const ZeeDEvent* event);

    private:
        unsigned int m_nbosons;
};
