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

// ZeeD Analysis includes
#include "ZeeDEvent/ZeeDBoson.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDEvent.h"

#include "ZeeDMisc/ZeeDMisc.h"

// ZeeD Analysis includes
#include "ZeeDEvent/ZeeDJet.h"


/** cut on minimum pt of the jet*/
class ZeeDCutPtMinJet: public ZeeDCut {
public:

    ZeeDCutPtMinJet(TString CutName,Double_t cutVal): ZeeDCut (CutName){
        fCutval = cutVal;
    }

    Bool_t evaluate(const ZeeDEvent* event);
private:
    Double_t fCutval;
};

/** cut on maximum eta of the jet*/
class ZeeDCutEtaMaxJet:public ZeeDCut {
public:

    ZeeDCutEtaMaxJet(TString CutName,Double_t cutval): ZeeDCut (CutName) {
        fCutval = cutval;
    }

    Bool_t evaluate(const ZeeDEvent* event);

private:
    Double_t fCutval;
};

