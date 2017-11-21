#include "ZeeDTools/ZeeDSystematics.h"

// STL includes
#include <iostream>

// Root includes
#include <TMath.h>
#include <TLorentzVector.h>

// ZeeD includes
#include "GaudiKernel/ServiceHandle.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDArrays/ZeeDGridCell.h"
#include "egammaAnalysisUtils/ElectronMCChargeCorrector.h"
#include "ZeeDMisc/ZeeDMisc.h"


//------------------------------------------------------
ZeeDSystematics* ZeeDSystematics::fInstance = NULL;
vector<ZeeDSystematics::ZeeDSingleSystematics*> ZeeDSystematics::allSystematics;


//------------------------------------------------------
ZeeDSystematics::ZeeDSystematics() :
    fSvcHelper("ZeeDSystematics"),
    fAnaOptions(fSvcHelper.GetAnaSvc())
{
    m_Rnd = new TRandom3();
    m_Rnd->SetSeed(2);
    // To register a new systematics provide the following info
    // 1. index of the systematics as written in enum ZeeDSystematics::Systematics (e.g. ZeeDSystematics::eElecEnUp)
    // 2. name of the new systematics as a text string
    // 3. whether this systematics entail full event recalculation (phaseSpaceShift) or just the re-weighing (weightShift)
    // 4. min level of the histograms could appear in the HM's (Analysis/Systematics/ToyMC)
    // 5. number of the copies for the systematics (used for the toyMC approach)

    // N.B.: In case you add some systematics twice problems with historgams could appear


    // add fake systematics for the central value iteration
    this->addSingleSystematics(ZeeDSystematics::eNoShift, "NoShift", ZeeDSystematics::weightShift, ZeeDLevel::Analysis);

    // add needed systematics
    // don't register systematics if in pileup file generating mode
    if ((*fAnaOptions)->EvaluateSystematics() && (*fAnaOptions)->GenPileupMCFile() == kFALSE) {
        if ((*fAnaOptions)->CalcZmumu() || (*fAnaOptions)->DoZCC() || (*fAnaOptions)->DoZCF()) {
            this->addZeeSystematics();
        }

        //    if ((*fAnaOptions)->CalcJets() && (*fAnaOptions)->DoZjet()) {
        //      this->addZjetSystematics();
        //    }

        if ((*fAnaOptions)->CalcW() || (*fAnaOptions)->CalcWmu()){
            this->addWSystematics();
        }

        //    if ((*fAnaOptions)->CalcJets() && (*fAnaOptions)->DoZplusJet() ) {
        //      this->addZplusJetSystematics();
        //    }

    }

}

//------------------------------------------------------
ZeeDSystematics::~ZeeDSystematics() {

}

//------------------------------------------------------
ZeeDSystematics::ZeeDSingleSystematics* ZeeDSystematics::isShiftInUse(ZeeDSystematics::Systematics isys) {

    // TODO: if this will work too slow do the check via
    //	   a hash table
    ZeeDSystematics::MapIdSystematics::iterator sys = actualSystematics.find(isys);
    if (!(sys == actualSystematics.end())) {
        return (sys->second);
    } else {
        return NULL;
    }
}

///------------------------------------------------------
void ZeeDSystematics::addZeeSystematics() {

    if ((*fAnaOptions)->IsMC()) {

        if ((*fAnaOptions)->EvalSysElecEnergyScaleCorr()) {

            if ((*fAnaOptions)->UseNNEMCalibration()) {

                /*	this->addSingleSystematics(ZeeDSystematics::eElecEnZeeStatUp, "eElecEnZeeStatUp", ZeeDSystematics::phaseSpaceShift);
                    this->addSingleSystematics(ZeeDSystematics::eElecEnZeeStatDown, "eElecEnZeeStatDown", ZeeDSystematics::phaseSpaceShift);
                    this->addSingleSystematics(ZeeDSystematics::eElecEnZeeSystUp, "eElecEnZeeSystUp", ZeeDSystematics::phaseSpaceShift);
                    this->addSingleSystematics(ZeeDSystematics::eElecEnZeeSystDown, "eElecEnZeeSystDown", ZeeDSystematics::phaseSpaceShift);
                    this->addSingleSystematics(ZeeDSystematics::eElecEnPSUp, "eElecEnPSUp", ZeeDSystematics::phaseSpaceShift);
                    this->addSingleSystematics(ZeeDSystematics::eElecEnPSDown, "eElecEnPSDown", ZeeDSystematics::phaseSpaceShift);
                    this->addSingleSystematics(ZeeDSystematics::eElecEnS12Up, "eElecEnS12Up", ZeeDSystematics::phaseSpaceShift);
                    this->addSingleSystematics(ZeeDSystematics::eElecEnS12Down, "eElecEnS12Down", ZeeDSystematics::phaseSpaceShift);
                    this->addSingleSystematics(ZeeDSystematics::eElecEnMatIDUp, "eElecEnMatIDUp", ZeeDSystematics::phaseSpaceShift);
                    this->addSingleSystematics(ZeeDSystematics::eElecEnMatIDDown, "eElecEnMatIDDown", ZeeDSystematics::phaseSpaceShift);
                    this->addSingleSystematics(ZeeDSystematics::eElecEnMatCryoUp, "eElecEnMatCryoUp", ZeeDSystematics::phaseSpaceShift);
                    this->addSingleSystematics(ZeeDSystematics::eElecEnMatCryoDown, "eElecEnMatCryoDown", ZeeDSystematics::phaseSpaceShift);
                    this->addSingleSystematics(ZeeDSystematics::eElecEnMatCaloUp, "eElecEnMatCaloUp", ZeeDSystematics::phaseSpaceShift);
                    this->addSingleSystematics(ZeeDSystematics::eElecEnMatCaloDown, "eElecEnMatCaloDown", ZeeDSystematics::phaseSpaceShift);
                    */	this->addSingleSystematics(ZeeDSystematics::eElecEnLArCalibUp, "eElecEnLArCalibUp", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eElecEnLArCalibDown, "eElecEnLArCalibDown", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eElecEnLArUnconvCalibUp, "eElecEnLArUnconvCalibUp", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eElecEnLArUnconvCalibDown, "eElecEnLArUnconvCalibDown", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eElecEnLArElecUnconvUp, "eElecEnLArElecUnconvUp", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eElecEnLArElecUnconvDown, "eElecEnLArElecUnconvDown", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eElecEnLArElecCalibUp, "eElecEnLArElecCalibUp", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eElecEnLArElecCalibDown, "eElecEnLArElecCalibDown", ZeeDSystematics::phaseSpaceShift);
                /*	this->addSingleSystematics(ZeeDSystematics::eElecEnL1GainUp, "eElecEnL1GainUp", ZeeDSystematics::phaseSpaceShift);
                    this->addSingleSystematics(ZeeDSystematics::eElecEnL1GainDown, "eElecEnL1GainDown", ZeeDSystematics::phaseSpaceShift);
                    this->addSingleSystematics(ZeeDSystematics::eElecEnL2GainUp, "eElecEnL2GainUp", ZeeDSystematics::phaseSpaceShift);
                    this->addSingleSystematics(ZeeDSystematics::eElecEnL2GainDown, "eElecEnL2GainDown", ZeeDSystematics::phaseSpaceShift);
                    this->addSingleSystematics(ZeeDSystematics::eElecEnG4Up, "eElecEnG4Up", ZeeDSystematics::phaseSpaceShift);
                    this->addSingleSystematics(ZeeDSystematics::eElecEnG4Down, "eElecEnG4Down", ZeeDSystematics::phaseSpaceShift);
                    this->addSingleSystematics(ZeeDSystematics::eElecPedestalUp, "eElecPedestalUp", ZeeDSystematics::phaseSpaceShift);
                    this->addSingleSystematics(ZeeDSystematics::eElecPedestalDown, "eElecPedestalDown", ZeeDSystematics::phaseSpaceShift);
                    */    } else {
                        this->addSingleSystematics(ZeeDSystematics::eElecEnOff, "ElecEnOff", ZeeDSystematics::phaseSpaceShift);
                        this->addSingleSystematics(ZeeDSystematics::eElecEnZeeStatUp, "ElecEnZeeStatUp", ZeeDSystematics::phaseSpaceShift);
                        this->addSingleSystematics(ZeeDSystematics::eElecEnZeeMethodUp, "ElecEnZeeMethodUp", ZeeDSystematics::phaseSpaceShift);
                        this->addSingleSystematics(ZeeDSystematics::eElecEnZeeGenUp, "ElecEnZeeGenUp", ZeeDSystematics::phaseSpaceShift);
                        this->addSingleSystematics(ZeeDSystematics::eElecEnZeeAllUp, "ElecEnZeeAllUp", ZeeDSystematics::phaseSpaceShift);
                        this->addSingleSystematics(ZeeDSystematics::eElecEnR12StatUp, "ElecEnR12StatUp", ZeeDSystematics::phaseSpaceShift);
                        this->addSingleSystematics(ZeeDSystematics::eElecEnPSStatUp, "ElecEnPSStatUp", ZeeDSystematics::phaseSpaceShift);
                        this->addSingleSystematics(ZeeDSystematics::eElecEnLowPtUp, "ElecEnLowPtUp", ZeeDSystematics::phaseSpaceShift);
                        this->addSingleSystematics(ZeeDSystematics::eElecEnZeeStatDown, "ElecEnZeeStatDown", ZeeDSystematics::phaseSpaceShift);
                        this->addSingleSystematics(ZeeDSystematics::eElecEnZeeMethodDown, "ElecEnZeeMethodDown", ZeeDSystematics::phaseSpaceShift);
                        this->addSingleSystematics(ZeeDSystematics::eElecEnZeeGenDown, "ElecEnZeeGenDown", ZeeDSystematics::phaseSpaceShift);
                        this->addSingleSystematics(ZeeDSystematics::eElecEnZeeAllDown, "ElecEnZeeAllDown", ZeeDSystematics::phaseSpaceShift);
                        this->addSingleSystematics(ZeeDSystematics::eElecEnR12StatDown, "ElecEnR12StatDown", ZeeDSystematics::phaseSpaceShift);
                        this->addSingleSystematics(ZeeDSystematics::eElecEnPSStatDown, "ElecEnPSStatDown", ZeeDSystematics::phaseSpaceShift);
                        this->addSingleSystematics(ZeeDSystematics::eElecEnLowPtDown, "ElecEnLowPtDown", ZeeDSystematics::phaseSpaceShift);
                        this->addSingleSystematics(ZeeDSystematics::eElecEnMC, "ElecEnMC", ZeeDSystematics::phaseSpaceShift);
                        //this->addSingleSystematics(ZeeDSystematics::eElecEnAllUp, "ElecEnAllUp", ZeeDSystematics::phaseSpaceShift);
                        //this->addSingleSystematics(ZeeDSystematics::eElecEnAllDown, "ElecEnAllDown", ZeeDSystematics::phaseSpaceShift);
                    }
        }


        if ((*fAnaOptions)->EvalSysElecEnergySmearingCorr()) {

            if ((*fAnaOptions)->UseNNEMCalibration()) {

                this->addSingleSystematics(ZeeDSystematics::eElecEnZSmearingUp, "eElecEnZSmearingUp", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eElecEnZSmearingDown, "eElecEnZSmearingDown", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eElecEnSamplingTermUp, "eElecEnSamplingTermUp", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eElecEnSamplingTermDown, "eElecEnSamplingTermDown", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eElecEnMaterialIDUp, "eElecEnMaterialIDUp", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eElecEnMaterialIDDown, "eElecEnMaterialIDDown", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eElecEnMaterialCaloUp, "eElecEnMaterialCaloUp", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eElecEnMaterialCaloDown, "eElecEnMaterialCaloDown", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eElecEnMaterialGapUp, "eElecEnMaterialGapUp", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eElecEnMaterialGapDown, "eElecEnMaterialGapDown", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eElecEnMaterialCryoUp, "eElecEnMaterialCryoUp", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eElecEnMaterialCryoDown, "eElecEnMaterialCryoDown", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eElecEnPileUpUp, "eElecEnPileUpUp", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eElecEnPileUpDown, "eElecEnPileUpDown", ZeeDSystematics::phaseSpaceShift);
            } else {

                //	this->addSingleSystematics(ZeeDSystematics::eSmearOff, "SmearOff", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eSmearUp, "SmearUp", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eSmearDown, "SmearDown", ZeeDSystematics::phaseSpaceShift);
            }
        }

        //      this->addSingleSystematics(ZeeDSystematics::eOffTrigOff, "TrigOff", ZeeDSystematics::weightShift);
        if ((*fAnaOptions)->DoCombToyMC()){
            this->addSingleSystematics(ZeeDSystematics::eOffTrigToyMC, "TrigToyMC", ZeeDSystematics::weightShift, ZeeDLevel::ToyMC, (*fAnaOptions)->MCSystNumbers());
            //this->addSingleSystematics(ZeeDSystematics::muOffTrigToyMC, "muTrigToyMC", ZeeDSystematics::weightShift, ZeeDLevel::ToyMC, (*fAnaOptions)->MCSystNumbers());
        } else {
            this->addSingleSystematics(ZeeDSystematics::eOffTrigUp, "TrigUp", ZeeDSystematics::weightShift);
            this->addSingleSystematics(ZeeDSystematics::eOffTrigDown, "TrigDown", ZeeDSystematics::weightShift);
        }
        this->addSingleSystematics(ZeeDSystematics::muOffTrigUp, "muTrigUp", ZeeDSystematics::weightShift);
        this->addSingleSystematics(ZeeDSystematics::muOffTrigDown, "muTrigDown", ZeeDSystematics::weightShift);

        if ((*fAnaOptions)->DoCombToyMC()) {        
            this->addSingleSystematics(ZeeDSystematics::eOffRecEffToyMC, "RecEffToyMC", ZeeDSystematics::weightShift, ZeeDLevel::ToyMC, (*fAnaOptions)->MCSystNumbers());       
            //this->addSingleSystematics(ZeeDSystematics::muOffRecEffToyMC, "muRecEffToyMC", ZeeDSystematics::weightShift, ZeeDLevel::ToyMC, (*fAnaOptions)->MCSystNumbers());        
        } else { 
            // this->addSingleSystematics(ZeeDSystematics::eOffRecEffOff, "RecEffOff", ZeeDSystematics::weightShift);
            this->addSingleSystematics(ZeeDSystematics::eOffRecEffUp,    "RecEffUp",    ZeeDSystematics::weightShift);
            this->addSingleSystematics(ZeeDSystematics::eOffRecEffDown,  "RecEffDown",  ZeeDSystematics::weightShift);
            //this->addSingleSystematics(ZeeDSystematics::muOffRecEffUp,    "muRecEffUp",    ZeeDSystematics::weightShift);
            //this->addSingleSystematics(ZeeDSystematics::muOffRecEffDown,  "muRecEffDown",  ZeeDSystematics::weightShift);
        }

        if ((*fAnaOptions)->DoCombToyMC()) {      
            this->addSingleSystematics(ZeeDSystematics::eOffIDEffToyMC, "IDEffToyMC", ZeeDSystematics::weightShift, ZeeDLevel::ToyMC, (*fAnaOptions)->MCSystNumbers()); 
            this->addSingleSystematics(ZeeDSystematics::muOffIDEffToyMC, "muIDEffToyMC", ZeeDSystematics::weightShift, ZeeDLevel::ToyMC, (*fAnaOptions)->MCSystNumbers());
        } else {
            //      this->addSingleSystematics(ZeeDSystematics::eOffIDEffOff, "IDEffOff", ZeeDSystematics::weightShift);
            this->addSingleSystematics(ZeeDSystematics::eOffIDEffUp, "IDEffUp",       ZeeDSystematics::weightShift);
            this->addSingleSystematics(ZeeDSystematics::eOffIDEffDown, "IDEffDown",   ZeeDSystematics::weightShift);
            this->addSingleSystematics(ZeeDSystematics::muOffIDEffUp, "muIDEffUp",       ZeeDSystematics::weightShift);
            this->addSingleSystematics(ZeeDSystematics::muOffIDEffDown, "muIDEffDown",   ZeeDSystematics::weightShift);

        }

        //      this->addSingleSystematics(ZeeDSystematics::eOffIsoEffOff, "IsoEffOff", ZeeDSystematics::weightShift);
        //        this->addSingleSystematics(ZeeDSystematics::eOffIsoEffUp, "IsoEffUp", ZeeDSystematics::weightShift);
        //        this->addSingleSystematics(ZeeDSystematics::eOffIsoEffDown, "IsoEffDown", ZeeDSystematics::weightShift);

        //if ((*fAnaOptions)->EvalSysPtZReweighting()) {
        //    this->addSingleSystematics(ZeeDSystematics::eZPtRew, "ZPtRew", ZeeDSystematics::weightShift, ZeeDLevel::Systematics,
        //            (*fAnaOptions)->BosonPtReweightingAdditionalTargets().size());
        //  this->addSingleSystematics(ZeeDSystematics::eZPtRew, "ZPtRew", ZeeDSystematics::weightShift);
        //}

        //this->addSingleSystematics(ZeeDSystematics::eZvRewOff, "ZvRewOff", ZeeDSystematics::weightShift);
        //}

        if ((*fAnaOptions)->EvalSysLineShapeCorrection()) {
            this->addSingleSystematics(ZeeDSystematics::eLineShapeOff, "LineShapeOff", ZeeDSystematics::weightShift);
        }

        if ((*fAnaOptions)->EvalSysForwardIDCorrection()) {
            if ((*fAnaOptions)->DoCombToyMC()) {
                this->addSingleSystematics(ZeeDSystematics::eOffIDEffFwdToyMC, "IDEffFwdToyMC", ZeeDSystematics::weightShift, ZeeDLevel::ToyMC, (*fAnaOptions)->MCSystNumbers()); 
            } else {    
                this->addSingleSystematics(ZeeDSystematics::eOffIDEffFwdOff,   "IDEffFwdOff",   ZeeDSystematics::weightShift);
                this->addSingleSystematics(ZeeDSystematics::eOffIDEffFwdUp,    "IDEffFwdUp",    ZeeDSystematics::weightShift); 
                this->addSingleSystematics(ZeeDSystematics::eOffIDEffFwdDown,  "IDEffFwdDown",  ZeeDSystematics::weightShift);    
            }      
        }

        if ((*fAnaOptions)->EvalSysChargeCorrection()) {
            this->addSingleSystematics(ZeeDSystematics::eChMIDUp, "eChMIDUp", ZeeDSystematics::weightShift);
            this->addSingleSystematics(ZeeDSystematics::eChMIDDown, "ChMIDDown", ZeeDSystematics::weightShift);
            this->addSingleSystematics(ZeeDSystematics::eChMIDDown, "ChMIDOff", ZeeDSystematics::weightShift);
        }

} // isMC

}

void ZeeDSystematics::addWSystematics() {
    if ((*fAnaOptions)->IsMC()) {
        this->addSingleSystematics(ZeeDSystematics::muSmearScaleUp, "MuScaleUp", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::muSmearScaleDown, "MuScaleDown", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::muSmearMSUp , "MuSmearingMSUp", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::muSmearMSDown , "MuSmearingMSDown", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::muSmearIDUp , "MuSmearingIDUp", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::muSmearIDDown , "MuSmearingIDDown", ZeeDSystematics::phaseSpaceShift);

        if ((*fAnaOptions)->EvalSysEtMiss()){
            if ((*fAnaOptions)->RecalcEtMiss()){ 
                this->addSingleSystematics(ZeeDSystematics::eEtMissScaleUp, "eEtMissScaleUp", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eEtMissScaleDown, "eEtMissScaleDown", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::muEtMissScaleUp, "muEtMissScaleUp", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::muEtMissScaleDown, "muEtMissScaleDown", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eEtMissResoUp, "eEtMissResoUp", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::eEtMissResoDown, "eEtMissResoDown", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::muEtMissResoIDUp, "muEtMissResoIDUp", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::muEtMissResoIDDown, "muEtMissResoIDDown", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::muEtMissResoMSUp, "muEtMissResoMSUp", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::muEtMissResoMSDown, "muEtMissResoMSDown", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::jetEtMissResoUp, "jetEtMissResoUp", ZeeDSystematics::phaseSpaceShift);   
                this->addSingleSystematics(ZeeDSystematics::jetEtMissResoDown, "jetEtMissResoDown", ZeeDSystematics::phaseSpaceShift);   
                this->addSingleSystematics(ZeeDSystematics::jetEtMissScaleUp, "jetEtMissScaleUp", ZeeDSystematics::phaseSpaceShift);   
                this->addSingleSystematics(ZeeDSystematics::jetEtMissScaleDown, "jetEtMissScaleDown", ZeeDSystematics::phaseSpaceShift);   
                this->addSingleSystematics(ZeeDSystematics::EtMissClustResUp, "EtMissClustResUp", ZeeDSystematics::phaseSpaceShift);   
                this->addSingleSystematics(ZeeDSystematics::EtMissClustResDown, "EtMissClustResDown", ZeeDSystematics::phaseSpaceShift);   
                this->addSingleSystematics(ZeeDSystematics::EtMissClustScaleUp, "EtMissClustScaleUp", ZeeDSystematics::phaseSpaceShift);   
                this->addSingleSystematics(ZeeDSystematics::EtMissClustScaleDown, "EtMissClustScaleDown", ZeeDSystematics::phaseSpaceShift);   
                this->addSingleSystematics(ZeeDSystematics::EtMissScaleSoftTermUp, "EtMissScaleSoftTermUp", ZeeDSystematics::phaseSpaceShift);   
                this->addSingleSystematics(ZeeDSystematics::EtMissScaleSoftTermDown, "EtMissScaleSoftTermDown", ZeeDSystematics::phaseSpaceShift);   
                this->addSingleSystematics(ZeeDSystematics::EtMissResoSoftTermDown, "EtMissResoSoftTermDown", ZeeDSystematics::phaseSpaceShift);   
                this->addSingleSystematics(ZeeDSystematics::EtMissResoSoftTermUp, "EtMissResoSoftTermUp", ZeeDSystematics::phaseSpaceShift);   
            }
            else {
                this->addSingleSystematics(ZeeDSystematics::HadrRecoilScaleCorrectionUp, "HadronRecoilScaleCorrectionUp", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::HadrRecoilScaleCorrectionDown, "HadronRecoilScaleCorrectionDown", ZeeDSystematics::phaseSpaceShift);
                this->addSingleSystematics(ZeeDSystematics::HadrRecoilSmearingOff, "HadronRecoilSmearingOff", ZeeDSystematics::phaseSpaceShift);
                if ((*fAnaOptions)->DoCombToyMC()) {
                    //this->addSingleSystematics(ZeeDSystematics::HadrRecoilSumEtToyMC, "HadrRecoilSumetToyMC", ZeeDSystematics::weightShift, ZeeDLevel::ToyMC, 25);
                    this->addSingleSystematics(ZeeDSystematics::HadrRecoilSumEt, "HadrRecoilSumetOff", ZeeDSystematics::weightShift);
                } else {
                    this->addSingleSystematics(ZeeDSystematics::HadrRecoilSumEtOff, "HadrRecoilSumetOff", ZeeDSystematics::weightShift);
                }
            }
        }
    }
}
//------------------------------------------------------
void ZeeDSystematics::addZjetSystematics() {

    if ((*fAnaOptions)->EvalSysElecEnergyScaleCorr()) {

        this->addSingleSystematics(ZeeDSystematics::eElecEnOff, "ElecEnOff", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::eElecEnZeeStatUp, "ElecEnZeeStatUp", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::eElecEnZeeMethodUp, "ElecEnZeeMethodUp", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::eElecEnZeeGenUp, "ElecEnZeeGenUp", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::eElecEnZeeAllUp, "ElecEnZeeAllUp", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::eElecEnR12StatUp, "ElecEnR12StatUp", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::eElecEnPSStatUp, "ElecEnPSStatUp", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::eElecEnLowPtUp, "ElecEnLowPtUp", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::eElecEnZeeStatDown, "ElecEnZeeStatDown", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::eElecEnZeeMethodDown, "ElecEnZeeMethodDown", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::eElecEnZeeGenDown, "ElecEnZeeGenDown", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::eElecEnZeeAllDown, "ElecEnZeeAllDown", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::eElecEnR12StatDown, "ElecEnR12StatDown", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::eElecEnPSStatDown, "ElecEnPSStatDown", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::eElecEnLowPtDown, "ElecEnLowPtDown", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::eElecEnMC, "ElecEnMC", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::eElecEnAllUp, "ElecEnAllUp", ZeeDSystematics::phaseSpaceShift);
        this->addSingleSystematics(ZeeDSystematics::eElecEnAllDown, "ElecEnAllDown", ZeeDSystematics::phaseSpaceShift);

    }

    this->addSingleSystematics(ZeeDSystematics::eZjetSubJetPtCutUp, "ZjetSubJetPtCutUp", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZjetSubJetPtCutDown, "ZjetSubJetPtCutDown", ZeeDSystematics::phaseSpaceShift);

    this->addSingleSystematics(ZeeDSystematics::eZjetJVFCutUp, "ZjetJVFCutUp", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZjetJVFCutDown, "ZjetJVFCutDown", ZeeDSystematics::phaseSpaceShift);
    if ((*fAnaOptions)->EvalSysForwardIDCorrection()) {
        this->addSingleSystematics(ZeeDSystematics::muOffIDEffFwdOff,   "IDEffFwdOff",   ZeeDSystematics::weightShift);
        this->addSingleSystematics(ZeeDSystematics::muOffIDEffFwdUp,    "IDEffFwdUp",    ZeeDSystematics::weightShift); 
        this->addSingleSystematics(ZeeDSystematics::muOffIDEffFwdDown,  "IDEffFwdDown",  ZeeDSystematics::weightShift);    
    }


}

//------------------------------------------------------
void ZeeDSystematics::addZplusJetSystematics() {

    this->addSingleSystematics(ZeeDSystematics::eZplusJetJES_P1_Up, "ZplusJetJES_P1_Up", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetJES_P1_Down, "ZplusJetJES_P1_Down", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetJES_P2_Up, "ZplusJetJES_P2_Up", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetJES_P2_Down, "ZplusJetJES_P2_Down", ZeeDSystematics::phaseSpaceShift);

    this->addSingleSystematics(ZeeDSystematics::eZplusJetJES_P3_Up, "ZplusJetJES_P3_Up", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetJES_P3_Down, "ZplusJetJES_P3_Down", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetJES_P4_Up, "ZplusJetJES_P4_Up", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetJES_P4_Down, "ZplusJetJES_P4_Down", ZeeDSystematics::phaseSpaceShift);

    this->addSingleSystematics(ZeeDSystematics::eZplusJetJES_P5_Up, "ZplusJetJES_P5_Up", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetJES_P5_Down, "ZplusJetJES_P5_Down", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetJES_P6_Up, "ZplusJetJES_P6_Up", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetJES_P6_Down, "ZplusJetJES_P6_Down", ZeeDSystematics::phaseSpaceShift);

    this->addSingleSystematics(ZeeDSystematics::eZplusJetJES_P7_Up, "ZplusJetJES_P7_Up", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetJES_P7_Down, "ZplusJetJES_P7_Down", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetJES_P8_Up, "ZplusJetJES_P8_Up", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetJES_P8_Down, "ZplusJetJES_P8_Down", ZeeDSystematics::phaseSpaceShift);

    this->addSingleSystematics(ZeeDSystematics::eZplusJetJES_P9_Up, "ZplusJetJES_P9_Up", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetJES_P9_Down, "ZplusJetJES_P9_Down", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetJES_P10_Up, "ZplusJetJES_P10_Up", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetJES_P10_Down, "ZplusJetJES_P10_Down", ZeeDSystematics::phaseSpaceShift);

    this->addSingleSystematics(ZeeDSystematics::eZplusJetPU_OffsetMu_Up, "ZplusJetPU_OffsetMu_Up", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetPU_OffsetMu_Up, "ZplusJetPU_OffsetMu_Down", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetPU_OffsetNPV_Up, "ZplusJetPU_OffsetNPV_Up", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetPU_OffsetNPV_Up, "ZplusJetPU_OffsetNPV_Down", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetPU_PtTerm_Up, "ZplusJetPU_PtTerm_Up", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetPU_PtTerm_Down, "ZplusJetPU_PtTerm_Down", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetPU_Rho_Up, "ZplusJetPU_Rho_Up", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetPU_Rho_Down, "ZplusJetPU_Rho_Down", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetFlavor_Comp_Up, "ZplusJetFlavor_Comp_Up", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetFlavor_Comp_Down, "ZplusJetFlavor_Comp_Down", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetFlavor_Response_Up, "ZplusJetFlavor_Response_Up", ZeeDSystematics::phaseSpaceShift);
    this->addSingleSystematics(ZeeDSystematics::eZplusJetFlavor_Response_Down, "ZplusJetFlavor_Response_Down", ZeeDSystematics::phaseSpaceShift);

}

