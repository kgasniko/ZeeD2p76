#ifndef __ZEEDCALCZ_H
#define __ZEEDCALCZ_H

////////////////////////////////////////////////////////
// Name    : ZeeDCalcZ.h
////////////////////////////////////////////////////////
//
// Contact persons: Alexander Glazov, Sebastian Schmitt
//

#include <TObject.h>

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDMuon.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDBosonEMu.h"
#include "ZeeDEvent/ZeeDBosonZ.h"

class TObjArray;
class ZeeDEvent;
//class ZeeDBosonZ;
//class ZeeDElectron;

/** Class for calculation of Z boson quantities */
class ZeeDCalcZ : public TObject {

public:

    struct PositiveParticle { enum Value { First, Second, Undefined }; };

    ZeeDCalcZ();
    ~ZeeDCalcZ();

    /** Creates Z boson candidates out of array of electrons */
    void Calculate(ZeeDEvent* event);

private:

    const TObjArray* filter(const TObjArray* elements, bool (*the_filter)(const TObject*));

    /** Creates Z boson object */
    ZeeDBosonZ*    CreateZ(const ZeeDElectron* elecOne, const ZeeDElectron* elecTwo);
    //ZeeDBosonEMu*  CreateEMu(const ZeeDElectron* electron, const ZeeDMuon* muon);
    ZeeDBosonZ* CreateZmumu(const ZeeDMuon* muon1, const ZeeDMuon* muon2);

    PositiveParticle::Value GetPositiveParticle(double ch1, double ch2);

    void CalcCosThStar(ZeeDBoson* Z) const;

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;


};

#endif // ZeeDCalcZ
