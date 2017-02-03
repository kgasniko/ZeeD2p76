#include "ZeeDAnalysisCuts/ZeeDCalcWeightCutDepZ.h"

// Root libraries
#include <TSystem.h>
#include <TFile.h>
#include <TH1D.h>
#include <TMath.h>
#include <TROOT.h>

#include "ZeeDEvent/ZeeDGenParticle.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDLepton.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDTools/ZeeDSystematics.h"

#include "ZeeDArrays/ZeeDBinGrid.h"
#include "ZeeDArrays/ZeeDArrayD.h"

#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDCalcWeightCutDepZ::ZeeDCalcWeightCutDepZ(ZeeDDoWeight doWeight) :
    IZeeDCalcWeight(doWeight),
    fEvent(NULL),
    fEventWeight(1.0),
    fSvcHelper("ZeeDCalcWeightCutDepZ"),
    fAnaOptions(fSvcHelper.GetAnaSvc())
{
    // Constructor
    fSys = ZeeDSystematics::getInstance();
}

//------------------------------------------------------
Double_t ZeeDCalcWeightCutDepZ::CalculateEventWeight(const ZeeDEvent* event)
{
    // Calculate event weights for given event

    fEvent       = event;
    fEventWeight = event->GetWeight();


    if ( fDoWeight.IDSFBothMediumPP &&  (*fAnaOptions)->IsMC() &&  (*fAnaOptions)->ApplyCentralIDCorrection() ){

        this->ApplyIDSFMediumPPBothLepZ();
    }

    if ( fDoWeight.IDSFBothTightPP &&  (*fAnaOptions)->IsMC() &&  (*fAnaOptions)->ApplyCentralIDCorrection()){

        this->ApplyIDSFTightPPBothLepZ();
    }

    if ( fDoWeight.RecSF &&  (*fAnaOptions)->IsMC() &&  (*fAnaOptions)->ApplyRecoCorrection()){
        this->ApplyRecSFTrkQualBothLepZ();
    }
    
    if ( fDoWeight.RecSFCentral &&  (*fAnaOptions)->IsMC() &&  (*fAnaOptions)->ApplyRecoCorrection()) {
        this->ApplyRecSFTrkQualCentralLepZ();
    }    
    
    if ( fDoWeight.IDSFCentralTightPP &&  (*fAnaOptions)->IsMC() &&  (*fAnaOptions)->ApplyCentralIDCorrection() ){
        this->ApplyIDSFTightPPCentralLepZ();
    }          
    
    if ( fDoWeight.IsoSFCentral &&  (*fAnaOptions)->IsMC() &&  (*fAnaOptions)->ApplyElecIsoCorr() &&
         !(fSys->isShiftInUse(ZeeDSystematics::eOffIsoEffOff) ) )  {

        this->ApplyIsoOneLep();
    } 
    
    if ( fDoWeight.IsoSFBoth &&  (*fAnaOptions)->IsMC() &&  (*fAnaOptions)->ApplyElecIsoCorr() &&
         !(fSys->isShiftInUse(ZeeDSystematics::eOffIsoEffOff) ) )  {

        this->ApplyIsoBothLep();
    } 
    
    if ( fDoWeight.TrigSFSingle &&  (*fAnaOptions)->IsMC() &&  (*fAnaOptions)->ApplyTrigCorr() &&
         !(fSys->isShiftInUse(ZeeDSystematics::eOffTrigOff) ) )  {
        this->ApplyTrigSFDefCF();
    } 
    
    if ( fDoWeight.TrigSFDiMediumPP &&  (*fAnaOptions)->IsMC() &&  (*fAnaOptions)->ApplyTrigCorr() &&
         !(fSys->isShiftInUse(ZeeDSystematics::eOffTrigOff) ) )  {
        this->ApplyTrigSFDiLep(ZeeDEnum::IsEM_t::MediumPP);
    }

    if ( fDoWeight.TrigSFDiTightPP &&  (*fAnaOptions)->IsMC() &&  (*fAnaOptions)->ApplyTrigCorr() &&
         !(fSys->isShiftInUse(ZeeDSystematics::eOffTrigOff) ) )  {
        this->ApplyTrigSFDiLep(ZeeDEnum::IsEM_t::TightPP);
    }


    return fEventWeight;
}

//------------------------------------------------------
void ZeeDCalcWeightCutDepZ::ApplyIDSFBothLepZ(ZeeDEnum::IsEM_t::Value elecID)
{

    const ZeeDBosonZ* bestZ = fEvent->GetCurrentBoson();
    Double_t eff_weight = 1.0;

    if(  bestZ != NULL) {

        const ZeeDLepton* e1 = bestZ->GetFirstLep();
        const ZeeDLepton* e2 = bestZ->GetSecondLep();

        // Check existence of electrons
        if (e1 == NULL || e2 == NULL ) {
            Error("ZeeDCalcWeightCutDepZ::ApplyIDSFBothLepZ",
                  "At least one electron is absent!");

            gSystem->Exit(EXIT_FAILURE);
        }

        if (e1->IdentificationSF().IsSet() == kFALSE ||
            e2->IdentificationSF().IsSet() == kFALSE) {

            Error("ZeeDCalcWeightCutDepZ::ApplyIDSFBothLepZ",
                  "At least one electron does not have identification scale factors!");

            ZeeDMisc::Dump4Vec("e1 FourVec:", e1->GetFourVector());
            ZeeDMisc::Dump4Vec("e2 FourVec:", e2->GetFourVector());

            //ZeeDMisc::Dump4Vec("e1 Cluster FourVec:", e1->Cluster().Get().fourVector);
            //ZeeDMisc::Dump4Vec("e2 Cluster FourVec:", e2->Cluster().Get().fourVector);

            gSystem->Exit(EXIT_FAILURE);

        }

        Double_t ZeeDLeptonBags::IdentificationSF::*idSF = NULL;

        switch(elecID) {

        case ZeeDEnum::IsEM_t::MediumPP:
            idSF = &ZeeDLeptonBags::IdentificationSF::MediumPP;
            break;

        case ZeeDEnum::IsEM_t::TightPP:
            idSF = &ZeeDLeptonBags::IdentificationSF::TightPP;
            break;

        default:
            Error("ZeeDCalcWeightCutDepZ::ApplyIDSFBothLepZ",
                  "Unhandled ZeeDEnum::IsEM_t::Value %i", (int)elecID);
            gSystem->Exit(EXIT_FAILURE);

        }

        const Double_t W1 = e1->IdentificationSF().Get().*(idSF);
        const Double_t W2 = e2->IdentificationSF().Get().*(idSF);

        eff_weight = TMath::Abs(W1 * W2);

    }

    fEventWeight *= eff_weight;

    return;
}

//------------------------------------------------------
void ZeeDCalcWeightCutDepZ::ApplyIDSFForwardLooseLepZ()
{
    DEBUG_MES_START;

    const ZeeDBosonZ* bestZ = fEvent->GetCurrentBoson();
    Double_t eff_weight = 1.0;

    if(  bestZ != NULL) {

        const ZeeDLepton* e = bestZ->GetForwardLep();

        // Check existence of electrons
        if (e == NULL ) {
            return;
        }

        Double_t W = e->IdentificationSF().Get().*(&ZeeDLeptonBags::IdentificationSF::NewFwdLoose);

        eff_weight = TMath::Abs(W);
    }

    fEventWeight *= eff_weight;

    DEBUG_MES_END;
    return;
}

//------------------------------------------------------
void ZeeDCalcWeightCutDepZ::ApplyIDSFForwardMediumLepZ()
{
    DEBUG_MES_START;

    const ZeeDBosonZ* bestZ = fEvent->GetCurrentBoson();
    Double_t eff_weight = 1.0;

    if(  bestZ != NULL) {

        const ZeeDLepton* e = bestZ->GetForwardLep();

        // Check existence of electrons
        if (e == NULL ) {
            return;
        }

        Double_t W = e->IdentificationSF().Get().*(&ZeeDLeptonBags::IdentificationSF::NewFwdMedium);

        eff_weight = TMath::Abs(W);
    }

    fEventWeight *= eff_weight;

    DEBUG_MES_END;
    return;
}

//------------------------------------------------------
void ZeeDCalcWeightCutDepZ::ApplyIDSFForwardTightLepZ()
{
    DEBUG_MES_START;

    const ZeeDBosonZ* bestZ = fEvent->GetCurrentBoson();
    Double_t eff_weight = 1.0;

    if(  bestZ != NULL) {

        const ZeeDLepton* e = bestZ->GetForwardLep();

        // Check existence of electrons
        if (e == NULL ) {
            return;
        }

        Double_t W = e->IdentificationSF().Get().*(&ZeeDLeptonBags::IdentificationSF::NewFwdTight);

        eff_weight = TMath::Abs(W);
    }

    fEventWeight *= eff_weight;

    DEBUG_MES_END;
    return;
}


//------------------------------------------------------
void ZeeDCalcWeightCutDepZ::ApplyIDSFTightPPCentralLepZ()
{
    DEBUG_MES_START;

    const ZeeDBosonZ* bestZ = fEvent->GetCurrentBoson();
    Double_t eff_weight = 1.0;

    if(  bestZ != NULL) {

        const ZeeDLepton* e = bestZ->GetCentralLep();

        // Check existence of electrons
        if (e == NULL ) {
            return;
        }

        Double_t W = e->IdentificationSF().Get().*(&ZeeDLeptonBags::IdentificationSF::TightPP);

        eff_weight = TMath::Abs(W);
    }

    fEventWeight *= eff_weight;

    DEBUG_MES_END;
    return;
}

//------------------------------------------------------
void ZeeDCalcWeightCutDepZ::ApplyIsoOneLep(){
    DEBUG_MES_START;

    const ZeeDBosonZ* bestZ = fEvent->GetCurrentBoson();
    Double_t eff_weight = 1.0;

    if(  bestZ != NULL) {

        const ZeeDLepton* e = bestZ->GetCentralLep();

        // Check existence of electrons
        if (e == NULL ) {
            return;
        }

        Double_t W = e->IsolationSF().Get().*(&ZeeDLeptonBags::IsolationSF::Both);

        eff_weight = TMath::Abs(W);
    }

    fEventWeight *= eff_weight;

    DEBUG_MES_END;
    return;

}

//------------------------------------------------------
void ZeeDCalcWeightCutDepZ::ApplyIsoBothLep(){
    DEBUG_MES_START;

    const ZeeDBosonZ* bestZ = fEvent->GetCurrentBoson();
    Double_t eff_weight = 1.0;

    if(  bestZ != NULL) {

        const ZeeDLepton* e1 = bestZ->GetFirstLep();
        const ZeeDLepton* e2 = bestZ->GetSecondLep();

        // Check existence of electrons
        if (e1 == NULL || e2 == NULL ) {
            return;
        }

        Double_t W1 = e1->IsolationSF().Get().*(&ZeeDLeptonBags::IsolationSF::Both);
        Double_t W2 = e2->IsolationSF().Get().*(&ZeeDLeptonBags::IsolationSF::Both);

        eff_weight = TMath::Abs(W1 * W2);
    }

    fEventWeight *= eff_weight;

    DEBUG_MES_END;
    return;

}

//------------------------------------------------------
void ZeeDCalcWeightCutDepZ::ApplyRecSFTrkQualBothLepZ()
{
    DEBUG_MES_START;

    const ZeeDBosonZ* bestZ = fEvent->GetCurrentBoson();
    Double_t eff_weight = 1.0;

    if(  bestZ != NULL) {

        const ZeeDLepton* e1 = bestZ->GetFirstLep();
        const ZeeDLepton* e2 = bestZ->GetSecondLep();

        // Check existence of electrons
        if (e1 == NULL || e2 == NULL ) {
            return;
        }

        if (e1->ReconstructionSF().IsSet() == kFALSE ||
            e2->ReconstructionSF().IsSet() == kFALSE) {

            Error("ZeeDCalcWeightCutDepZ::ApplyOfficialRecEfficiencyCorrection",
                  "At least one electron does not have reconstruction scale factors!");

            Error("ZeeDCalcWeightCutDepZ::ApplyOfficialRecEfficiencyCorrection",
                  "e1 %i, e2 %i", e1->ReconstructionSF().IsSet(), e2->ReconstructionSF().IsSet());

            ZeeDMisc::Dump4Vec("e1 FourVec:", e1->GetFourVector());
            ZeeDMisc::Dump4Vec("e2 FourVec:", e2->GetFourVector());

            gSystem->Exit(EXIT_FAILURE);

        }

        const Double_t W1 = e1->ReconstructionSF().Get().RecoTrkQual;
        const Double_t W2 = e2->ReconstructionSF().Get().RecoTrkQual;

        eff_weight = TMath::Abs(W1 * W2);

    }

    fEventWeight *= eff_weight;

    DEBUG_MES_END;
    return;
}

//------------------------------------------------------
void ZeeDCalcWeightCutDepZ::ApplyRecSFTrkQualCentralLepZ()
{
    DEBUG_MES_START;

    const ZeeDBosonZ* bestZ = fEvent->GetCurrentBoson();
    Double_t eff_weight = 1.0;

    if(  bestZ != NULL) {

        const ZeeDLepton* e = bestZ->GetCentralLep();

        // Check existence of electron
        if (e == NULL ) {
            return;
        }

        if (e->ReconstructionSF().IsSet() == kFALSE ) {

            Error("ZeeDCalcWeightCutDepZ::ApplyRecSFTrkQualCentralLepZ",
                  "Central electron does not have reconstruction scale factors!");

            ZeeDMisc::Dump4Vec("e FourVec:", e->GetFourVector());

            gSystem->Exit(EXIT_FAILURE);

        }

        const Double_t W = e->ReconstructionSF().Get().RecoTrkQual;

        eff_weight = TMath::Abs(W);
    }

    fEventWeight *= eff_weight;

    DEBUG_MES_END;
    return;
}

//------------------------------------------------------
void ZeeDCalcWeightCutDepZ::ApplyTrigSFDefCF() {
    DEBUG_MES_START;

    const ZeeDBosonZ* bestZ = fEvent->GetCurrentBoson();
    Double_t eff_weight = 1.0;

    if (bestZ != NULL) {

        const ZeeDLepton* e1 = bestZ->GetFirstLep();
		const ZeeDLepton* e2 = bestZ->GetSecondLep();

        // Check existence of electron
        if (e1 == NULL|| e2 == NULL) {
            return;
        }

        if (e1->TriggerSF().IsSet() == kFALSE||e2->TriggerSF().IsSet() == kFALSE) {

            Error("ZeeDCalcWeightCutDepZ::ApplyTrigSFDefCF (single elec trigger)", "Central electron does not have trigger scale factors!");


            gSystem->Exit(EXIT_FAILURE);

        }


        double weight = 1;
	    ZeeDEnum::Trigger::Value trigger = fEvent->GetDefaultSingleElectronTrigger(); 
    	const Bool_t MatchElectron1 = e1->IsMatchedToTriggerChain(trigger); 
    	const Bool_t MatchElectron2 = e2->IsMatchedToTriggerChain(trigger);
		
        const double sf1 = MatchElectron1 ? e1->TriggerSF().Get().TrigSF[fEvent->GetDefaultSingleElectronTrigger()]:1.0;		
		const double sf2 = MatchElectron2 ? e2->TriggerSF().Get().TrigSF[fEvent->GetDefaultSingleElectronTrigger()]:1.0;
		const double sf=sf1*sf2;
        if (sf != -1) {
            weight = sf;
        }

        eff_weight = TMath::Abs(weight);

    }

    fEventWeight *= eff_weight;

    DEBUG_MES_END;
    return;
}

//------------------------------------------------------
void ZeeDCalcWeightCutDepZ::ApplyTrigSFDiLep(__attribute__((unused)) ZeeDEnum::IsEM_t::Value elecID) {
	
   	const ZeeDBosonZ* bestZ = fEvent->GetCurrentBoson();
    Double_t eff_weight = 1.0;

    // check existence of Z boson
    if (bestZ == NULL) return;

    const ZeeDLepton* lep1 = bestZ->GetFirstLep();
    const ZeeDLepton* lep2 = bestZ->GetSecondLep();


	
    Double_t TrtigSc_mc_lep1    = lep1->TriggerSF().IsSet() ? lep1->TriggerSF().Get().SFTrig : 1.0;
    Double_t TrtigSc_mc_lep2    = lep2->TriggerSF().IsSet() ? lep2->TriggerSF().Get().SFTrig : 1.0;

    if ( lep1->TriggerSF().IsSet() && lep2->TriggerSF().IsSet() && lep1->TriggerSF().Get().TrigEff != -1 && lep2->TriggerSF().Get().TrigEff != -1){
	Double_t TrigEff_mc_lep1    = lep1->TriggerSF().IsSet() ? lep1->TriggerSF().Get().TrigEff : 1.0;

    Double_t TrigEff_mc_lep2    = lep2->TriggerSF().IsSet() ? lep2->TriggerSF().Get().TrigEff : 1.0;


    Double_t TrigEff_data_lep1  = TrtigSc_mc_lep1 * TrigEff_mc_lep1;
    Double_t TrigEff_data_lep2  = TrtigSc_mc_lep2 * TrigEff_mc_lep2;
	
    Double_t Numenator          =  TrigEff_data_lep1 + (1 - TrigEff_data_lep1) * TrigEff_data_lep2;
    Double_t Denumenator        =  TrigEff_mc_lep1 + (1 - TrigEff_mc_lep1) * TrigEff_mc_lep2;

    Double_t TrigWeght          = Numenator / Denumenator;

    if(TrigWeght > 1.0){        // just for check (always TrigWeght < 1.0)
        TrigWeght = 1.0;
    }
    //std::cout<<"TrigWeght = "<<TrigWeght<<std::endl;

    fEventWeight*=TrigWeght; 
   	return;

	}else {

    eff_weight = TrtigSc_mc_lep1+(1-TrtigSc_mc_lep1)*TrtigSc_mc_lep2;

    fEventWeight *= eff_weight;

    return;
	}
}

