#ifndef I_ZEED_CALCW_H
#define I_ZEED_CALCW_H

#include "ZeeDEvent/ZeeDEvent.h"


/** Helper struct to turn weights on and off

    All possible corrections etc. are collected here.

    The default value should not be set at this place but via jobOptions.

 */

struct ZeeDDoWeight {

    ZeeDDoWeight() : RecSF(kFALSE), RecSFCentral(kFALSE), 
            IDSFBothTightPP(kFALSE), IDSFBothMediumPP(kFALSE), IDSFCentralTightPP(kFALSE), IDSFNewFwdLoose(kFALSE), IDSFNewFwdMedium(kFALSE), IDSFNewFwdTight(kFALSE), IsoSFBoth(kFALSE), IsoSFCentral(kFALSE), TrigSFSingle(kFALSE), TrigSFDiMediumPP(kFALSE), TrigSFDiTightPP(kFALSE), HadrRecoil(kFALSE) {}

    Bool_t RecSF;
    Bool_t RecSFCentral;    
    Bool_t IDSFBothTightPP;
	Bool_t IDSFBothLoosePP;
    Bool_t IDSFBothMediumPP;
    Bool_t IDSFCentralTightPP;
    Bool_t IDSFNewFwdLoose;
    Bool_t IDSFNewFwdMedium;
    Bool_t IDSFNewFwdTight;
    Bool_t IsoSFBoth;
    Bool_t IsoSFCentral;    
    Bool_t TrigSFSingle;
    Bool_t TrigSFDiMediumPP;
    Bool_t TrigSFDiTightPP;
    Bool_t HadrRecoil;
    

};

class ZeeDSystematics;

class IZeeDCalcWeight {
public:
    IZeeDCalcWeight(ZeeDDoWeight doWeight = ZeeDDoWeight()) : fDoWeight(doWeight) {}

    virtual ~IZeeDCalcWeight() {}

    virtual Double_t CalculateEventWeight(const ZeeDEvent* event) = 0;

    virtual void UseDoWeight(ZeeDDoWeight doWeight) {

        fDoWeight = doWeight;

    }

protected:

    ZeeDDoWeight fDoWeight;

};

#endif
