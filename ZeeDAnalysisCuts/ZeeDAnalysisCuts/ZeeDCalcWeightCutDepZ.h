#ifndef __ZEEDCalcWeightIDEffZCC_H
#define __ZEEDCalcWeightIDEffZCC_H

////////////////////////////////////////////////////////
// Name    : ZeeDCalcWeightCutDepZ.h
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
  @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte, Sebastian Schmitt
  @date   Thu May 19 08:44:41 CEST 2010
*/
class ZeeDCalcWeightCutDepZ : public TObject, public IZeeDCalcWeight {

public:
    ZeeDCalcWeightCutDepZ(ZeeDDoWeight doWeight = ZeeDDoWeight());
    virtual ~ZeeDCalcWeightCutDepZ() {}

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

    void ApplyIDSFBothLepZ(ZeeDEnum::IsEM_t::Value elecID);

    void ApplyIDSFMediumPPBothLepZ() {
        this->ApplyIDSFBothLepZ(ZeeDEnum::IsEM_t::MediumPP);
    }
    void ApplyIDSFTightPPBothLepZ() {
        this->ApplyIDSFBothLepZ(ZeeDEnum::IsEM_t::TightPP);
    }

    void ApplyIDSFTightPPCentralLepZ();
    void ApplyIDSFForwardLooseLepZ();
    void ApplyIDSFForwardMediumLepZ();
    void ApplyIDSFForwardTightLepZ();

    void ApplyIsoOneLep();
    void ApplyIsoBothLep();

    void ApplyRecSFTrkQualCentralLepZ();
    void ApplyRecSFTrkQualBothLepZ();

    void ApplyTrigSFDefCF();
    void ApplyTrigSFDiLep(ZeeDEnum::IsEM_t::Value elecID);

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;      //!

};

#endif // ZeeDCalcWeightCutDepZ
