#pragma once

////////////////////////////////////////////////////////
// Name    : ZeeDSystematics.h
////////////////////////////////////////////////////////
//
// Contact persons: A. Glazov, SSh 
//

// std includes
#include <string>
#include <sstream>

// Root includes
#include <TObject.h>
#include <TString.h>
#include <TSystem.h>
#include "TRandom3.h"

// ZeeD includes
#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDTools/ZeeDCutBit.h"

using std::string;
using std::map;
using std::vector;
using std::cout;
using std::endl;


/** Class to define and apply systematics shifts */
class ZeeDSystematics : public TObject {

  public:

    static ZeeDSystematics* getInstance() {
      if (fInstance != NULL) {
	return fInstance; 
      } else {
	fInstance = new ZeeDSystematics();
	return fInstance;    
      }
    }

    enum ShiftType {
      phaseSpaceShift,
      weightShift
    };

    enum Systematics {

        // ---------------------------------------------------------------------
        // No shift fake systematics
        eNoShift,

        // ---------------------------------------------------------------------
	// Systematics shifts on the electron energy mean and resolution

	// no energy correction
        eElecEnOff,

	// scale shifts mainly for the NN calibration
   eElecEnZeeStatUp,
   eElecEnZeeStatDown,
	eElecEnZeeSystUp,
	eElecEnZeeSystDown,
   eElecEnPSUp,
   eElecEnPSDown,
   eElecEnS12Up,
   eElecEnS12Down,
	eElecEnMatIDUp,
	eElecEnMatIDDown,
	eElecEnMatCryoUp, 
	eElecEnMatCryoDown, 
   eElecEnMatCaloUp,
   eElecEnMatCaloDown,
   eElecEnLArCalibUp,
   eElecEnLArCalibDown,
   eElecEnLArUnconvCalibUp,
	eElecEnLArUnconvCalibDown,
	eElecEnLArElecUnconvUp,
	eElecEnLArElecUnconvDown,
	eElecEnLArElecCalibUp,
	eElecEnLArElecCalibDown,
	eElecEnL1GainUp,
	eElecEnL1GainDown,
	eElecEnL2GainUp,
	eElecEnL2GainDown,
	eElecEnG4Up,
	eElecEnG4Down,
	eElecPedestalUp,
	eElecPedestalDown,

	// NN smearing
	eElecEnZSmearingUp,
	eElecEnZSmearingDown,
	eElecEnSamplingTermUp,
	eElecEnSamplingTermDown,
	eElecEnMaterialIDUp,
	eElecEnMaterialIDDown,
	eElecEnMaterialCaloUp,
	eElecEnMaterialCaloDown,
	eElecEnMaterialGapUp,
	eElecEnMaterialGapDown,
	eElecEnMaterialCryoUp,
	eElecEnMaterialCryoDown,
	eElecEnPileUpUp,
	eElecEnPileUpDown,

	// scale shifts mainly for the old std calibration
        eElecEnZeeMethodUp,
	eElecEnZeeGenUp,
	eElecEnZeeAllUp,
        eElecEnR12StatUp,
        eElecEnPSStatUp,
        eElecEnPSStatDown,
	eElecEnLowPtUp,
        eElecEnZeeMethodDown,
        eElecEnZeeGenDown,
        eElecEnZeeAllDown,
        eElecEnR12StatDown,
        eElecEnLowPtDown,
        eElecEnMC,
        eElecEnAllUp,
        eElecEnAllDown,

	// old smearing
        eSmearOff,
        eSmearUp,
        eSmearDown,

        // ---------------------------------------------------------------------
        eExtraCrackCutOff,

        // ---------------------------------------------------------------------
        eZPtRewOff,
        eZPtRewUp,
        eZPtRewDown,
        eZPtRew,

        // ---------------------------------------------------------------------
        eOffTrigOff,
        eOffTrigUp,
        eOffTrigDown,
        eOffTrigToyMC,
        muOffTrigOff,
        muOffTrigUp,
        muOffTrigDown,
        muOffTrigToyMC,

        // ---------------------------------------------------------------------
        eOffIDEffOff,
        eOffIDEffUp,
        eOffIDEffDown,
        eOffIDEffToyMC,
        muOffIDEffUp,
        muOffIDEffDown,
        muOffIDEffToyMC,
		// ---------------------------------------------------------------------
        eSFMC,

        // ---------------------------------------------------------------------
        eOffIDEffFwdOff,
        eOffIDEffFwdUp,
        eOffIDEffFwdDown,
        eIDEffFwdUp,
        eIDEffFwdDown,
        eOffIDEffFwdToyMC,

        // ---------------------------------------------------------------------
        eOffIDEffEMECIWUp,
        eOffIDEffEMECIWDown,
        eOffIDEffFCalUp,
        eOffIDEffFCalDown,	

        // ---------------------------------------------------------------------
        eOffRecEffOff,
        eOffRecEffUp,
        eOffRecEffDown,
        eOffRecEffToyMC,

        // ---------------------------------------------------------------------
        eOffIsoEffOff,
        eOffIsoEffUp,
        eOffIsoEffDown,
        eOffIsoEffToyMC,

        // ---------------------------------------------------------------------
        ePileUpOff,
        eLineShapeOff,

        // ---------------------------------------------------------------------
        eChMIDUp,
        eChMIDDown,
        eChMIDOff,

        // ---------------------------------------------------------------------
        eZvRewOff,
        eZvRewUp,	
        eZvRewDown,

	// ---------------------------------------------------------------------
	eNTrkRewOff,
	eNTrkRewUp,
	eNTrkRewDown,

	// ---------------------------------------------------------------------
	eNvtxOff,

	// ---------------------------------------------------------------------
	eLArHoleOff,

        // ---------------------------------------------------------------------
        eZjetJetPtCutUp,
        eZjetJetPtCutDown,

        eZjetSubJetPtCutUp,
        eZjetSubJetPtCutDown,

        eZjetJVFCutUp,
        eZjetJVFCutDown,

        // ---------------------------------------------------------------------
	eZplusJetJES_P1_Up,
	eZplusJetJES_P1_Down,
	eZplusJetJES_P2_Up,
	eZplusJetJES_P2_Down,
	eZplusJetJES_P3_Up,
	eZplusJetJES_P3_Down,
	eZplusJetJES_P4_Up,
	eZplusJetJES_P4_Down,
	eZplusJetJES_P5_Up,
	eZplusJetJES_P5_Down,
	eZplusJetJES_P6_Up,
	eZplusJetJES_P6_Down,
	eZplusJetJES_P7_Up,
	eZplusJetJES_P7_Down,
	eZplusJetJES_P8_Up,
	eZplusJetJES_P8_Down,
	eZplusJetJES_P9_Up,
	eZplusJetJES_P9_Down,
	eZplusJetJES_P10_Up,
	eZplusJetJES_P10_Down,

	eZplusJetPU_OffsetMu_Up,
	eZplusJetPU_OffsetMu_Down,
	eZplusJetPU_OffsetNPV_Up,
	eZplusJetPU_OffsetNPV_Down,
	eZplusJetPU_PtTerm_Up,
	eZplusJetPU_PtTerm_Down,
	eZplusJetPU_Rho_Up,
	eZplusJetPU_Rho_Down,

	eZplusJetFlavor_Comp_Up,
	eZplusJetFlavor_Comp_Down,
	eZplusJetFlavor_Response_Up,
	eZplusJetFlavor_Response_Down,
eZplusJetPunchThrough_Up,
	eZplusJetPunchThrough_Down,
	eZplusJetJER,
    
    //EtMiss systematics
    eEtMissScaleUp,
    eEtMissScaleDown,
    muEtMissScaleUp,
    muEtMissScaleDown,
    eEtMissResoUp,
    eEtMissResoDown,
    muEtMissResoIDUp,
    muEtMissResoIDDown,
    muEtMissResoMSUp,
    muEtMissResoMSDown,
	muSmearScaleUp,
	muSmearScaleDown,
	muSmearMSUp,
	muSmearMSDown,
	muSmearIDUp,
	muSmearIDDown,
	muOffIDEffFwdOff,
	muOffIDEffFwdUp,
	muOffIDEffFwdDown,
	jetEtMissResoUp,
	jetEtMissResoDown,
	jetEtMissScaleUp,
	jetEtMissScaleDown,
	EtMissClustResUp,
	EtMissClustResDown,
	EtMissClustScaleUp,
	EtMissClustScaleDown,
	EtMissScaleSoftTermUp,
	EtMissScaleSoftTermDown,
	EtMissResoSoftTermUp,
	EtMissResoSoftTermDown,
    HadrRecoilSumEtDown,
    HadrRecoilSumEtUp,
    HadrRecoilSumEtOff,
    HadrRecoilSumEt,
    HadrRecoilSumEtToyMC,
	// ---------------------------------------------------------------------
        eNumShifts,
	eNULL
    };

    // helper structure which keeps all the info about
    // each single systematics
    struct ZeeDSingleSystematics {

      Systematics systematics_type;
      string name;
      ShiftType shift_type;
      ZeeDLevel::TYPE level;
      string suffix;
      int toyMCIndex;
      float randomNumber;  

      ZeeDSingleSystematics(Systematics s, string n, ShiftType st,
	  ZeeDLevel::TYPE lt, string suf = "", int ri = 0, float rn = 0.) :
	systematics_type(s),
	name(n),
	shift_type(st),
	level(lt),
	suffix(suf),
	toyMCIndex(ri),
	randomNumber(rn)
      { }

    };

    typedef map<ZeeDSystematics::Systematics, ZeeDSystematics::ZeeDSingleSystematics*> MapIdSystematics;

    // list of all the systematics available
    static vector<ZeeDSystematics::ZeeDSingleSystematics*> allSystematics;

    // add systematics to the list of all the systematics
    void addSingleSystematics(Systematics i, string name, ShiftType st, ZeeDLevel::TYPE lt = ZeeDLevel::Systematics, int ns = 1) {
      if (ns < 2) {
	allSystematics.push_back(new ZeeDSingleSystematics(i, name, st, lt));
	return;
      }
      for (int isys = 0; isys < ns; ++isys) {
	std::ostringstream oss;
	oss << isys;
	string suffix = oss.str();
	// XXX
	//float rn = analysis.getPRN();
	float prn = 1.1;

	allSystematics.push_back(new ZeeDSingleSystematics(i, name + "_" + suffix, st, lt, suffix, isys, prn));
      }
    }

    void addSingleSystematics(Systematics i, string name, ShiftType st, ZeeDLevel::TYPE lt, vector<string> ns) {
      for (vector<string>::const_iterator isys = ns.begin(); isys != ns.end(); ++isys) {
	name += "_";
	string suffix = *isys;
	name += suffix;
	// XXX
	//float rn = analysis.getPRN();
	float prn = 1.1;

	allSystematics.push_back(new ZeeDSingleSystematics(i, name, st, lt, suffix, isys - ns.begin(), prn));
      }
    }

    /** Is shift Sys currently used? */
    ZeeDSingleSystematics* isShiftInUse(ZeeDSystematics::Systematics isys);

    ZeeDSystematics::Systematics getCurrentShift() {
      // CAUTION: the function returns the first shift
      // CAUTION: no check for the empty list of systematics, add fake eNoShift always
      return actualSystematics.begin()->first;
    }

    bool isCurrentShiftGenShift() {
      for (MapIdSystematics::const_iterator it = actualSystematics.begin();
	  it != actualSystematics.end(); ++it) if (it->second->shift_type == ZeeDSystematics::weightShift) return true;

      return false;
    }

    bool isCurrentShiftToyMCShift() {
      for (MapIdSystematics::const_iterator it = actualSystematics.begin();
	  it != actualSystematics.end(); ++it) if (it->second->level == ZeeDLevel::ToyMC) return true;

      return false;
    }

    bool isCurrentShiftKinShift() {
      for (MapIdSystematics::const_iterator it = actualSystematics.begin();
	  it != actualSystematics.end(); ++it) if (it->second->shift_type == ZeeDSystematics::phaseSpaceShift) return true;

      return false;
    }

    double getToySystematics(double mean, double sigma){
        double randomResult = m_Rnd->Gaus(mean, sigma);
        //std::cout << randomResult << "  " << mean  << "   " << sigma << std::endl;
     return randomResult;   
    }

    void enableShift(ZeeDSingleSystematics* sys) { actualSystematics[sys->systematics_type] = sys; }

    /** Reset the list of actually used systematics (typically just one) */
    void resetAllShifts() { actualSystematics.clear(); }

    ~ZeeDSystematics();


private:

    ZeeDSystematics();
    ZeeDSystematics(const ZeeDSystematics&);
    void operator=(const ZeeDSystematics&);

    static ZeeDSystematics* fInstance;

    // list of all the systematics are currently in use
    // (normally just one shift of one source)
    MapIdSystematics actualSystematics;
    TRandom3* m_Rnd;

    // methods to initialize systematics for various analysis
    void addZeeSystematics();
    void addZjetSystematics();
    void addZplusJetSystematics();
    void addWSystematics();
    // helpers
    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;

};

