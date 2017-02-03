#include "ZeeDCalculator/ZeeDCalcRecWeightMuon.h"

//#define USE_DEBUG_MESSAGES
//#define DEBUG_ON 1
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDMisc/ZeeDMisc.h"
//#include "MuonEfficiencyCorrection/AnalysisMuonConfigurableScaleFactors.h"
#include "TRandom3.h"

#include "ZeeDTools/ZeeDSystematics.h"
#include "TFile.h"

#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDMuon.h"
#include "ZeeDEvent/ZeeDGenParticle.h"
//------------------------------------------------------
ZeeDCalcRecWeightMuon::ZeeDCalcRecWeightMuon() :
    fSvcHelper("ZeeDCalcRecWeightMuon"),
    fAnaOptions(fSvcHelper.GetAnaSvc()){

        // initialize access to the systematics
        fSys = ZeeDSystematics::getInstance();

        std::string name = (*fAnaOptions)->ReconstructionEfficiencyMuonFileName();
        // initialize re-weighing tools (via ElectronEfficiencyCorrection package)
        m_MCPsf=  new Analysis::AnalysisMuonConfigurableScaleFactors ("", name,"GeV",Analysis::AnalysisMuonConfigurableScaleFactors::AverageOverPeriods);

        if ((*fAnaOptions)->DoCombToyMC()){
            this->bookToyMC();
            this->bookToyMCTrig();
        }
        m_MCPsf->Initialise();

        TFile* fileSF = new TFile("/afs/desy.de/user/k/kgasniko/ZeeD/share/SF2.root");
        hPlus  = (TH1D*)fileSF->Get("SF2Plus");
        hMinus = (TH1D*)fileSF->Get("SF2Minus");


    }

void ZeeDCalcRecWeightMuon::bookToyMC(){
    
    for (int i=0; i<(*fAnaOptions)->MCSystNumbers(); i++){
        std::cout << " Booking scale factor " << i << std::endl;
        ostringstream convert;
        convert << i;
        string ii =convert.str();
        std::string name = "MuonsChain_MediumPlus_2012_"+ii+"_SF.txt.gz";
        Analysis::AnalysisMuonConfigurableScaleFactors *sfToy;
        sfToy= new  Analysis::AnalysisMuonConfigurableScaleFactors ("", name,"GeV",Analysis::AnalysisMuonConfigurableScaleFactors::AverageOverPeriods);
        sfToy->Initialise();
        m_MCPsfVec.push_back(sfToy);
    }
}

void ZeeDCalcRecWeightMuon::bookToyMCTrig() {
    
    for (int i=0; i<(*fAnaOptions)->MCSystNumbers(); i++){
        TrigPlus.push_back(fSys->getToySystematics(0.97, 0.02));
        TrigMinus.push_back(fSys->getToySystematics(0.93, 0.03));
    }
}

double ZeeDCalcRecWeightMuon::getToySFTrig(int toyN, int charge){
    if (charge>0)
        return TrigPlus[toyN];
    else
        return TrigMinus[toyN];
}

double ZeeDCalcRecWeightMuon::getToySF(int toyN, int charge, TLorentzVector lv){
 
    Analysis::AnalysisMuonConfigurableScaleFactors *sfToy = m_MCPsfVec[toyN];
    return sfToy->scaleFactor(charge, lv);

}
//------------------------------------------------------
void ZeeDCalcRecWeightMuon::Calculate(ZeeDEvent* event){

    // Calculate electron weights for the given event

    // Get electrons
    const TObjArray* muons = event->GetMuons();

    // Loop over electrons
    for ( Int_t i = 0; i < muons->GetEntriesFast(); ++i ) {
        int charge;
        ZeeDMuon* muon =  static_cast< ZeeDMuon* > (muons->At(i));
        charge = muon->getCharge();
        if (muon->CombTrack().IsSet()){
            const TLorentzVector& lv	= muon->CombTrack().Get().fcombinedMuonTrackParticle;
            this->CalcIDRecSF(muon,charge, lv);
        }
        else 
            muon->IdentificationSF().Lock();
        this->CalcTriggerSF(muon);
        const TLorentzVector& lv = muon->GetFourVector();
        muon->IsolationSF().Lock();
        muon->ReconstructionSF().Lock();
    }

}

void ZeeDCalcRecWeightMuon::CalcTriggerSF(ZeeDMuon* muon) {

    ZeeDLeptonBags::TriggerSF triggerSFbag;
    //  ZeeDEnum::Trigger::Value default_trigger = ZeeDMisc::GetDefaultSingleElectronTrigger(run_number);
    ZeeDEnum::Trigger::Value default_trigger = ZeeDEnum::Trigger::EF_e15_loose1;
    if ((*fAnaOptions)->ApplyTriggerCorrection()) {

        double eta = muon->GetFourVector().Eta();
        double pt  = muon->GetFourVector().Pt();
        int charge = muon->getCharge();
        TH1D* hist= NULL;
        if (charge > 0)
            hist=hPlus;
        else hist=hMinus; 
        int binN = hist->GetXaxis()->FindBin(eta);
        double weight = hist->GetBinContent(binN);
        double weight_unc = hist->GetBinError(binN);
        /*if (charge>0){
            weight = 0.97;
            weight_unc=0.02;
        } else {
            weight = 0.97;
            weight_unc=0.03;
        }*/
        /*	 if (charge>0 && pt < 27){
             weight = 0.84;
             weight_unc = 0.3;
             }
        //std::cout << weight << std::endl;

        double weight = 0;
        if (charge>0) {
        if (eta < - 1.52) weight = 1.05;
        else if (eta < -0.6) weight = 1.02;
        else if (eta < 0.6)  weight = 0.87;
        else if (eta < 1.52) weight = 1.13;
        else if (eta < 2.47) weight = 0.91;
        }
        else{
        if (eta < - 1.52) weight = 0.98;
        else if (eta < -0.6) weight = 1.00;
        else if (eta < 0.6)  weight = 0.87;
        else if (eta < 1.52) weight = 0.83;
        else if (eta < 2.47) weight = 1.07;
        }
        */
        //double weight     		   = 0.95;
        //double weight = 1.;
        double eff 				   = 1;
        //double weight_unc		   = 0.015;

        triggerSFbag.TrigSF[default_trigger] = weight; 
        triggerSFbag.SFTrig = weight;
        // The uncertainty shifts
        if (fSys->isShiftInUse(ZeeDSystematics::muOffTrigUp)) {
            triggerSFbag.SFTrig += weight_unc;
            triggerSFbag.TrigSF[default_trigger]    += weight_unc;
        } else if (fSys->isShiftInUse(ZeeDSystematics::muOffTrigDown)) {
            triggerSFbag.SFTrig -= weight_unc;
            triggerSFbag.TrigSF[default_trigger]    -= weight_unc; 
        } else if (fSys->isShiftInUse(ZeeDSystematics::muOffTrigOff)) {
            triggerSFbag.TrigSF[default_trigger]     = 1.; 
            // FIXME: something about ToyMC
        } else if (fSys->isShiftInUse(ZeeDSystematics::muOffTrigToyMC)) {
           ZeeDSystematics::ZeeDSingleSystematics* toyMCsyst = fSys->isShiftInUse(ZeeDSystematics::muOffTrigToyMC);
           int idxToy = toyMCsyst->toyMCIndex;
           double sf = getToySFTrig(idxToy, charge); 
           //const UInt_t css = fSys->CurrentSystShift(ZeeDSystematics::eOffTrigToyMC);
 //           double sf = fSys->getToySystematics(weight, weight_unc);
            //std::cout << weight << "  " << weight_unc << "   " << sf << std::endl;
            triggerSFbag.TrigSF[default_trigger]    = sf;
            triggerSFbag.SFTrig = sf;
        }
        //	  triggerSFbag.TrigSF[default_trigger]    = sFTrig_EF_e12_medium_MediumPP_IJ->GetToyMCScaleFactor(css, eta_cluster, pt_GeV);
        triggerSFbag.TrigEff= eff;
    } else {

        triggerSFbag.TrigSF[default_trigger]     = 1.; 
    }  

    muon->TriggerSF().Set(triggerSFbag);
}

//------------------------------------------------------
void ZeeDCalcRecWeightMuon::CalcIDRecSF(ZeeDMuon* muon,	int charge,
        const TLorentzVector& lv) {

    ZeeDLeptonBags::IdentificationSF reconstructionSF;
    if (charge ==0) 
        reconstructionSF.MediumPP  = 1.;
    else {
        if ((*fAnaOptions)->ApplyCentralIDCorrection()) {

            double 		   weight	= m_MCPsf->scaleFactor(charge, lv); 
            double		   weight_unc_stat	= m_MCPsf->scaleFactorUncertainty(charge, lv);
            double                 weight_unc_syst      = m_MCPsf->scaleFactorSystematicUncertainty(charge, lv);
            //    double 	           weight_unc           = weight_unc_stat+weight_unc_syst;
            double weight_unc = sqrt(pow(weight_unc_syst,2)+pow(weight_unc_stat,2));
            reconstructionSF.MediumPP = weight;
            // The uncertainty shifts
            if (fSys->isShiftInUse(ZeeDSystematics::muOffIDEffUp)) {
                reconstructionSF.MediumPP += weight_unc;
            } else if (fSys->isShiftInUse(ZeeDSystematics::muOffIDEffDown)) {
                reconstructionSF.MediumPP -= weight_unc; 
            } else if (fSys->isShiftInUse(ZeeDSystematics::muOffIDEffToyMC)) {
                     ZeeDSystematics::ZeeDSingleSystematics* toyMCsyst = fSys->isShiftInUse(ZeeDSystematics::muOffIDEffToyMC);
           int idxToy = toyMCsyst->toyMCIndex;
            double sf = getToySF(idxToy, charge, lv); 
           //double sf = fSys->getToySystematics(weight, weight_unc);
                reconstructionSF.MediumPP = sf;
            }

        } else {

            reconstructionSF.MediumPP  = 1.;   
        }
    }
    muon->IdentificationSF().Set(reconstructionSF);
}
