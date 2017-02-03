#ifndef ZEEDCALIBRATOR_H
#define ZEEDCALIBRATOR_H

////////////////////////////////////////////////////////////////////////////////
/// Name    : ZeeDCalibrator.h
////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <TRandom3.h>

class TLorentzVector;

class ZeeDBinGrid;

class ZeeDCalibrator {

public:
    ZeeDCalibrator(std::string inFileCalib, std::string inFileSmear);
    ~ZeeDCalibrator();

    /** Method return correction factors */
    double CorrectionFactor(const TLorentzVector* Electron);

    /** Method return smearing factor */
    double SmearFactor(const TLorentzVector* Electron);

private:
    bool fAbs;

    /** Input calibration */
    ZeeDBinGrid* m_Calib;
    ZeeDBinGrid* m_Smear;

    TRandom3 *m_Rndm;
};

#endif // ZEEDCALIBRATOR
