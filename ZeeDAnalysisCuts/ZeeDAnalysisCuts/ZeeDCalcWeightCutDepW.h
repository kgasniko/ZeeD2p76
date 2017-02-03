#ifndef __ZEEDCalcWeightIDEffW_H
#define __ZEEDCalcWeightIDEffW_H

////////////////////////////////////////////////////////
// Name    : ZeeDCalcWeightCutDepW.h
////////////////////////////////////////////////////////

// Root includes
#include <TObject.h>
#include <TLorentzVector.h>

// ZeeD analysis includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDCuts/IZeeDCalcWeight.h"

#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDMuon.h"

class ZeeDEvent;
class ZeeDSystematics;
class ZeeDArrayD;
class ZeeDBinGrid;
class TRandom3;

/** brief Class for calculation of event weights
*/


// You need to implement this in case you want to apply scaling factors
//
class ZeeDCalcWeightCutDepW : public TObject, public IZeeDCalcWeight {

public:
    ZeeDCalcWeightCutDepW(ZeeDDoWeight doWeight = ZeeDDoWeight());
    virtual ~ZeeDCalcWeightCutDepW() {}

    /** Calculate event weights */
    virtual Double_t CalculateEventWeight(const ZeeDEvent* event);


private:

    //egammaSFclass fSFclass;

    // Pointer to the ZeeDSystematics object 
    ZeeDSystematics* fSys;

    /** ZeeD event */
    const ZeeDEvent* fEvent;

    /** Event weight */
    Double_t fEventWeight;

    void ApplyIDSFLepW(ZeeDEnum::IsEM_t::Value elecID);

    void ApplyRecSFW();
	void ApplyTrigSFDefCF();
    void ApplyHadrRecoilSF(); 

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;      //!

};

#endif // ZeeDCalcWeightCutDepZ
