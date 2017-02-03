#ifndef ZEEDCALIBRATORHM
#define ZEEDCALIBRATORHM

////////////////////////////////////////////////////////
// Name    : ZeeDCalibratorHM.h
////////////////////////////////////////////////////////

// Analysis includes
#include "ZeeDHist/ZeeDHistManager.h"
#include "ZeeDTools/ZeeDCalcCalib.h"

#include "TString.h"

class ZeeDCalcCalib;

/** Dummy manager to fill calibrator
    @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
    @date 2008/09/03
*/
class ZeeDCalibratorHM : public ZeeDHistManager {

public:

    ZeeDCalibratorHM(TString name, double MassMin = 78, double MassMax = 103);
    virtual ~ZeeDCalibratorHM();

    virtual void BookHistos();  //
    using ZeeDHistManager::Fill;
    virtual void Fill();        // Update calibrator
    virtual void Finalize();    // Get calibration

private:

    ZeeDCalcCalib* fZeeCal;
    int fAllEvt;
    int fSelEvt;
    int fCalib_Method;
    string fCalib_In;
    string fdirname;

    double ZMassMin;
    double ZMassMax;
};

#endif //
