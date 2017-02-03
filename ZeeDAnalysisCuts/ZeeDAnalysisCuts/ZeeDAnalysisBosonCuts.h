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
#include "ZeeDEvent/ZeeDBoson.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDEvent.h"

// FIXME: should be boson
#include "ZeeDEvent/ZeeDBosonZ.h"


//-----------------------------------------------
/**  cut on maximum pt of the Z boson */
class ZeeDCutPtZMaxZ : public ZeeDCut {
public:
    ZeeDCutPtZMaxZ(TString CutName, Double_t cutval) : ZeeDCut (CutName) {
        m_cutval = cutval;
    }
    Bool_t evaluate(const ZeeDEvent* event);

private:
    Double_t m_cutval;
};

//-----------------------------------------------
/**  cut on minimal pt of the electrons */
class ZeeDCutPtMinBothLepZ : public ZeeDCut {
public:
    ZeeDCutPtMinBothLepZ(TString CutName, Double_t cutval)
        : ZeeDCut (CutName) {
        m_cutval = cutval;
    }
    Bool_t evaluate(const ZeeDEvent* event);

private:
    Double_t m_cutval;
};

//-----------------------------------------------
/**  cut on minimal pt of the electrons */
class ZeeDCutPtMinOneLepZ : public ZeeDCut {
public:
    ZeeDCutPtMinOneLepZ(TString CutName, Double_t cutval) : ZeeDCut (CutName) {
        m_cutval = cutval;
    }
    Bool_t evaluate(const ZeeDEvent* event);
private:
    Double_t m_cutval;
};

//-----------------------------------------------
/**  cut on minimal pt of the electrons */
class ZeeDCutAsymmetricalLepPt: public ZeeDCut {
    public:
        ZeeDCutAsymmetricalLepPt(TString CutName, double cutmin1, double cutmin2)
            : ZeeDCut (CutName) {
                // keep the order: m_min1 <= m_min2
                if (cutmin1 < cutmin2) {
                    m_min1 = cutmin1;
                    m_min2 = cutmin2;
                } else {
                    m_min1 = cutmin2;
                    m_min2 = cutmin1;
                }
            }

        Bool_t evaluate(const ZeeDEvent* event);

    private:
        double m_min1;
        double m_min2;
};

//-----------------------------------------------
/** rapidity cut */
class ZeeDCutZYZ : public ZeeDCut {
public:
    ZeeDCutZYZ(TString CutName, Double_t yLow, Double_t yHigh) : ZeeDCut (CutName) {
        m_ywlow  = yLow;
        m_ywhigh = yHigh;
    }
    Bool_t evaluate(const ZeeDEvent* event);
private:
    Double_t m_ywlow;
    Double_t m_ywhigh;
};

/** mass range cut for background charge symmetry analysis */
class ZeeDCutZMassZCharge : public ZeeDCut {
public:
    ZeeDCutZMassZCharge(TString CutName) : ZeeDCut(CutName) {}

    Bool_t evaluate(const ZeeDEvent* event);

};

//-----------------------------------------------
/** return false if at least one elec is in LAr hole region for periods I-K */
class ZeeDCutTriggerLArHoleBugBothElecZ : public ZeeDCut {

public:

    ZeeDCutTriggerLArHoleBugBothElecZ(TString CutName) :  ZeeDCut(CutName) {}

    Bool_t evaluate(const ZeeDEvent* event);

private:

};

