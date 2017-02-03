#pragma once


////////////////////////////////////////////////////////
// Name    : ZeeDCalcElec.h
////////////////////////////////////////////////////////

// Root includes
#include <TObject.h>
#include <TH1D.h>

#include "ElectronEfficiencyCorrection/TElectronEfficiencyCorrectionTool.h"

// Athena includes
#include "GaudiKernel/ToolHandle.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "ZeeDTools/ZeeDBags.h"

class ZeeDSystematics;
class ZeeDEvent;
class ZeeDElectron;
class ElectronEfficiencySFTool;

class ZeeDCalcRecWeight {

public:

  ZeeDCalcRecWeight();
  ~ZeeDCalcRecWeight();

  void Calculate(ZeeDEvent* event);

private:

    /**  Pointer to the systematics manager */
    ZeeDSystematics* fSys;
    TObjArray* IDToy, *TrigToy;
    void makeToyHist(TH2D* hist, TH2D*& histReturn);
    void BookIDSF();
    void BookTrigSF();
    double getIDToySF(int toyNum, double eta, double pt);
    double getTrigToySF(int toyNum, double eta, double pt);


    /** Calculate identification scale factors */
    void CalcIdentificationSF(	ZeeDElectron* electron,
				int run_number,
				PATCore::ParticleDataType::DataType electron_type,
				double pt_MeV,
				double eta_cluster);

    /** Calculate reconstruction scale factors */
    void CalcReconstructionSF(	ZeeDElectron* electron,
				int run_number,
				PATCore::ParticleDataType::DataType electron_type,
				double pt_MeV,
				double eta_cluster);

    /** Calculate reconstruction scale factors */
    void CalcTriggerSF(		ZeeDElectron* electron,
				int run_number,
				PATCore::ParticleDataType::DataType electron_type,
				double pt_MeV,
				double eta_cluster);

    /** Calculate isolation scale factors */
    void CalcIsolationSF(ZeeDElectron* electron);

    /** scale factor helper class */
    //egammaSFclass fSFclass;

    /** Service helper */
    ZeeDSvcHelper fSvcHelper; //!

    /** Analysis options service */
    ServiceHandle<IZeeDAnalysisSvc> *fAnaOptions; //!

    // re-weighing tool for the reconstruction eff (via ElectronEfficiencyCorrection package)
    Root::TElectronEfficiencyCorrectionTool tool_reconstruction_SF;

    // re-weighing tool for the reconstruction eff (via ElectronEfficiencyCorrection package)
    Root::TElectronEfficiencyCorrectionTool tool_identification_medium_SF;
    Root::TElectronEfficiencyCorrectionTool tool_identification_tight_SF;
    Root::TElectronEfficiencyCorrectionTool tool_identification_Fwd_tight_SF;     
     Root::TElectronEfficiencyCorrectionTool tool_identification_loose_SF;
   
    // re-weighing tool for the reconstruction eff (via ElectronEfficiencyCorrection package)
    Root::TElectronEfficiencyCorrectionTool tool_trigger_SF;


    // XXX: still the old tool, move to the new one
    ElectronEfficiencySFTool* sFTool;


    //------------------------------------------------------
    // The functions below deteremine where the ElectronEfficiencyCorrection package
    // is applicable. See the recommendations on
    // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/TElectronEfficiencyCorrectionTool
    // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/EfficiencyMeasurements2012
    // also private communication from JK

    inline bool isElecEffCorrRecRegion(const double& eta_cluster, const double& pt_MeV)
    {
      return (fabs(eta_cluster) < 2.47 && pt_MeV > 7000.);
    }

    inline bool isElecEffCorrTriggerRegion(const double& eta_cluster, const double& pt_MeV)
    {
      return (fabs(eta_cluster) < 2.47 && pt_MeV > 20000.);
    }

    inline bool isElecEffCorrIdCntRegion(const double& eta_cluster, const double& pt_MeV)
    {
      return (fabs(eta_cluster) < 2.47 && pt_MeV > 20000.);
    }

    inline bool isElecEffCorrIdFwdRegion(const double& eta_cluster, const double& pt_MeV)
    {
      return (fabs(eta_cluster) > 2.5 && fabs(eta_cluster) < 4.9 && pt_MeV > 20000.);
    }



};

