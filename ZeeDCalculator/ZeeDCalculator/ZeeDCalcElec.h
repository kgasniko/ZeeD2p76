#pragma once

////////////////////////////////////////////////////////
// Name    : ZeeDCalcElec.h
////////////////////////////////////////////////////////
//
// Contact persons: Andrei Nikiforov, Sebastian Schmitt
//

// Root includes
#include <TObject.h>
#include <TH1D.h>

// Athena includes
#include "GaudiKernel/ToolHandle.h"
#include "egammaAnalysisUtils/EnergyRescalerUpgrade.h"
#include "egammaAnalysisUtils/EisoTool.h"
#include "ElectronPhotonFourMomentumCorrection/egammaEnergyCorrectionTool.h"


// ZeeD analysis includes
#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDTools/ZeeDBags.h"
#include "ZeeDTools/ZeeDSystematics.h"


class ZeeDCalibrator;
class ZeeDEvent;
class ZeeDElectron;

/**
  Class for calculation of electron quantities.
  Electrons are corrected to the right energy scale.
 */
class ZeeDCalcElec : public TObject {

public:

    ZeeDCalcElec();
    ~ZeeDCalcElec();

    /** Calibrator to be used. Not owned by ZeeDCalcElec */
    //        void UseCalibrator(ZeeDCalibrator *cal) {fCalibrator = cal;}

    /** Calculate electrons and add to array
      @param event - raw event electrons are recalculated
    */
    void Calculate(ZeeDEvent* event);

private:

    /** Initialize energy rescaler */
    void InitializeEnergyRescaler(int runNumber);

    /** Calculate electron 4-vector using track and cluster
      information. To this 4-vector energy corrections are applied.
    */
    void CalcFourVector(ZeeDElectron* electron, int runNumber);
    void calibrateCluster(ZeeDElectron* electron, int runNumber);
    void smeareCluster(TLorentzVector& electronCluster, bool isCentralElectron);

    egEnergyCorr::Scale::Variation getGEO21EnergyScaleVariation();
    egEnergyCorr::Resolution::Variation getGEO21EnergyResolutionVariation();

    egRescaler::EnergyRescalerUpgrade::CorrType getGEO20EnergyResolutionVariation();
    egRescaler::EnergyRescalerUpgrade::Variation getGEO20EnergyScaleVariation();

    // Calculate IsEM for the forward electron
    void CalcFwdISEM(ZeeDElectron* electron, ZeeDEvent* event);

    /** Calculate electron isolation using EisoTool */
    void CalcEIso(ZeeDElectron* electron, ZeeDEvent* event);

    void CalcCaloIsoCorrections(ZeeDElectron* electron, ZeeDEvent* event);

    void CalcIDCut(ZeeDElectron* electron);

    void CorrectCharge(ZeeDEvent* event, ZeeDElectron* electron);

    /** Pointer to the current event */
    ZeeDEvent* fEventInProcess; //!

    /**  Pointer to the systematics manager */
    ZeeDSystematics* fSys; //!

    /**  Pointer to the calibrator */
    ZeeDCalibrator* fCalibrator; //!

    /** Energy rescaler */
    egRescaler::EnergyRescalerUpgrade fEnerRescaler;
    AtlasRoot::egammaEnergyCorrectionTool energy_correction_tool;

    /** Service helper */
    ZeeDSvcHelper fSvcHelper; //!

    /** Analysis options service */
    ServiceHandle<IZeeDAnalysisSvc> *fAnaOptions; //!

    /** EIso tool */
    EisoTool fEiso;

};

