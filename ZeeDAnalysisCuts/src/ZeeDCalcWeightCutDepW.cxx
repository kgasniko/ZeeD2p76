#include "ZeeDAnalysisCuts/ZeeDCalcWeightCutDepW.h"

// Root libraries
#include <TSystem.h>
#include <TFile.h>
#include <TH1D.h>
#include <TMath.h>
#include <TROOT.h>

#include "ZeeDEvent/ZeeDGenParticle.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDLepton.h"
#include "ZeeDEvent/ZeeDBosonW.h"
#include "ZeeDTools/ZeeDSystematics.h"

#include "ZeeDArrays/ZeeDBinGrid.h"
#include "ZeeDArrays/ZeeDArrayD.h"

#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDCalcWeightCutDepW::ZeeDCalcWeightCutDepW(ZeeDDoWeight doWeight) :
    IZeeDCalcWeight(doWeight),
    fEvent(NULL),
    fEventWeight(1.0),
    fSvcHelper("ZeeDCalcWeightCutDepW"),
    fAnaOptions(fSvcHelper.GetAnaSvc())
{
    // Constructor
    fSys = ZeeDSystematics::getInstance();
}

//------------------------------------------------------
Double_t ZeeDCalcWeightCutDepW::CalculateEventWeight(const ZeeDEvent* event)
{
    // Calculate event weights for given event

    fEvent       = event;
    fEventWeight = event->GetWeight();


    if ( fDoWeight.IDSFBothMediumPP &&  (*fAnaOptions)->IsMC() &&  (*fAnaOptions)->ApplyCentralIDCorrection() &&
            !(fSys->isShiftInUse(ZeeDSystematics::eOffIDEffOff) ) )  {
        this->ApplyIDSFLepW(ZeeDEnum::IsEM_t::MediumPP);
    }
    if ( fDoWeight.IDSFBothLoosePP &&  (*fAnaOptions)->IsMC() &&  (*fAnaOptions)->ApplyCentralIDCorrection() &&
            !(fSys->isShiftInUse(ZeeDSystematics::eOffIDEffOff) ) )  {
        this->ApplyIDSFLepW(ZeeDEnum::IsEM_t::LoosePP);
    }
    if ( fDoWeight.RecSF &&  (*fAnaOptions)->IsMC() &&  (*fAnaOptions)->ApplyRecoCorrection() &&
            !(fSys->isShiftInUse(ZeeDSystematics::eOffRecEffOff) ) )  {
        this->ApplyRecSFW();
    }
    if ( fDoWeight.TrigSFSingle &&  (*fAnaOptions)->IsMC() &&  (*fAnaOptions)->ApplyTrigCorr() &&
            !(fSys->isShiftInUse(ZeeDSystematics::eOffTrigOff) ) )  {
        this->ApplyTrigSFDefCF();
    } 
    if (fDoWeight.HadrRecoil && (*fAnaOptions)->IsMC() && !(*fAnaOptions)->RecalcEtMiss() &&
            !(fSys->isShiftInUse(ZeeDSystematics::HadrRecoilSumEtOff) ) )  {
        this->ApplyHadrRecoilSF();
    }
    return fEventWeight;
}

void ZeeDCalcWeightCutDepW::ApplyHadrRecoilSF(){

    DEBUG_MES_START;

    const ZeeDBosonW* bestW = fEvent->GetCurrentBosonW();
    Double_t eff_weight = bestW->GetWeight();

    fEventWeight *= eff_weight;
    /*
    if (fSys->isShiftInUse(ZeeDSystematics::eOffIDEffToyMC)){
        std::cout << "ToyMC weight after HadrRecoil "<< fEventWeight << "   " << eff_weight<< std::endl;
    }
    if (fSys->isShiftInUse(ZeeDSystematics::eNoShift)){
        std::cout << "NoShift weight after HadrRecoil "<< fEventWeight<< "   " << eff_weight << std::endl;
    }
*/

    DEBUG_MES_END;
    return;
}

//------------------------------------------------------
void ZeeDCalcWeightCutDepW::ApplyTrigSFDefCF() {
    DEBUG_MES_START;

    const ZeeDBosonW* bestW = fEvent->GetCurrentBosonW();
    Double_t eff_weight = 1.0;
    //const int    run_number     = fEvent->RunNumber().Get();
    if (bestW != NULL) {

        const ZeeDLepton* elec = bestW->GetLep();

        // Check existence of electron
        if (elec == NULL) {
            return;
        }

        if (elec->TriggerSF().IsSet() == kFALSE) {

            Error("ZeeDCalcWeightCutDepW::ApplyTrigSFDefCF (single elec trigger)", "Central electron does not have trigger scale factors!");

            ZeeDMisc::Dump4Vec("electron FourVec:", elec->GetFourVector());

            gSystem->Exit(EXIT_FAILURE);

        }


        double weight = 1;

        const double sf = elec->TriggerSF().Get().SFTrig;

        if (sf != -1) {
            weight = sf;
        }

        eff_weight = TMath::Abs(weight);

    }
/* if (fSys->isShiftInUse(ZeeDSystematics::muIDEffToyMC))   std::cout << "Before = " << fEventWeight << std::endl;
    fEventWeight *= eff_weight;
    
    if (fSys->isShiftInUse(ZeeDSystematics::muIDEffToyMC)){
        std::cout << "ToyMC weight after Trig "<< fEventWeight << "   " << eff_weight << std::endl;
    }
    if (fSys->isShiftInUse(ZeeDSystematics::eNoShift)){
        std::cout << "NoShift weight after Trig "<< fEventWeight<< "   " << eff_weight << std::endl;
    }
*/
    fEventWeight *= eff_weight;
    DEBUG_MES_END;
    return;
}


//------------------------------------------------------
void ZeeDCalcWeightCutDepW::ApplyIDSFLepW(ZeeDEnum::IsEM_t::Value elecID)
{

    const ZeeDBosonW* bestW = fEvent->GetCurrentBosonW();
    Double_t eff_weight = 1.0;

    if(  bestW != NULL) {

        const ZeeDLepton* e = bestW->GetLep();

        // Check existence of electrons
        if (e == NULL) {
            Error("ZeeDCalcWeightCutDepW::ApplyIDSFLepW",
                    "Lepton is absent!");

            gSystem->Exit(EXIT_FAILURE);
        }

        if (e->IdentificationSF().IsSet() == kFALSE) {

            Error("ZeeDCalcWeightCutDepW::ApplyIDSFLepW",
                    "Lepton does not have identification scale factors!");

            ZeeDMisc::Dump4Vec("e FourVec:", e->GetFourVector());

            //ZeeDMisc::Dump4Vec("e Cluster FourVec:", e->Cluster().Get().fourVector);

            gSystem->Exit(EXIT_FAILURE);

        }

        Double_t ZeeDLeptonBags::IdentificationSF::*idSF = NULL;

        switch(elecID) {

            case ZeeDEnum::IsEM_t::MediumPP:
                idSF = &ZeeDLeptonBags::IdentificationSF::MediumPP;
                break;

            case ZeeDEnum::IsEM_t::LoosePP:
                idSF = &ZeeDLeptonBags::IdentificationSF::LoosePP;
                break;

            case ZeeDEnum::IsEM_t::TightPP:
                idSF = &ZeeDLeptonBags::IdentificationSF::TightPP;
                break;

            default:
                Error("ZeeDCalcWeightCutDepW::ApplyIDSFLepW",
                        "Unhandled ZeeDEnum::IsEM_t::Value %i", (int)elecID);
                gSystem->Exit(EXIT_FAILURE);

        }

        const Double_t W1 = e->IdentificationSF().Get().*(idSF);

        eff_weight = TMath::Abs(W1);

    }
    fEventWeight *= eff_weight;
    /*
    if (fSys->isShiftInUse(ZeeDSystematics::eOffIDEffToyMC)){
        std::cout << "ToyMC weight after ID "<< fEventWeight  << "   " << eff_weight<< std::endl;
    }
    if (fSys->isShiftInUse(ZeeDSystematics::eNoShift)){
        std::cout << "NoShift weight after ID "<< fEventWeight << "   " << eff_weight<< std::endl;
    }*/


    return;
}


//------------------------------------------------------
void ZeeDCalcWeightCutDepW::ApplyRecSFW()
{
    DEBUG_MES_START;

    const ZeeDBosonW* bestW = fEvent->GetCurrentBosonW();
    Double_t eff_weight = 1.0;

    if(  bestW != NULL) {

        const ZeeDLepton* e = bestW->GetLep();

        // Check existence of electrons
        if (e == NULL ) {
            return;
        }

        if (e->ReconstructionSF().IsSet() == kFALSE ) {

            Error("ZeeDCalcWeightCutDepW::ApplyOfficialRecEfficiencyCorrection",
                    "Lepton does not have reconstruction scale factors!");

            Error("ZeeDCalcWeightCutDepW::ApplyOfficialRecEfficiencyCorrection",
                    "e %i", e->ReconstructionSF().IsSet());

            ZeeDMisc::Dump4Vec("e FourVec:", e->GetFourVector());

            gSystem->Exit(EXIT_FAILURE);

        }

        const Double_t W = e->ReconstructionSF().Get().RecoTrkQual;

        eff_weight = TMath::Abs(W);

    }
    fEventWeight *= eff_weight;
    /*
    if (fSys->isShiftInUse(ZeeDSystematics::eOffIDEffToyMC)){
        std::cout << "ToyMC weight after Rec "<< fEventWeight << "   " << eff_weight<< std::endl;
    }
    if (fSys->isShiftInUse(ZeeDSystematics::eNoShift)){
        std::cout << "NoShift weight after Rec "<< fEventWeight << "   " << eff_weight<< std::endl;
    }
*/
    DEBUG_MES_END;
    return;
}
