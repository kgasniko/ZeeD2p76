#include "ZeeDCalculator/ZeeDCalcRecWeight.h"
#include "TFile.h"

//#define USE_DEBUG_MESSAGES
//#define DEBUG_ON 1
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDMisc/ZeeDMisc.h"

#include "ZeeDTools/ZeeDSystematics.h"

#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDGenParticle.h"
//------------------------------------------------------
ZeeDCalcRecWeight::ZeeDCalcRecWeight() :
    fSvcHelper("ZeeDCalcRecWeight"),
    fAnaOptions(fSvcHelper.GetAnaSvc()){
        
        // initialize access to the systematics
        fSys = ZeeDSystematics::getInstance();
   
        // initialize re-weighing tools (via ElectronEfficiencyCorrection package)
        tool_reconstruction_SF.addFileName((ZeeDMisc::FindFile((*fAnaOptions)->ReconstructionEfficiencyElectronFileName())).Data());
        
        if ((*fAnaOptions)->DoCombToyMC()) {
            this->BookIDSF();
            this->BookTrigSF();
        }

        if ((*fAnaOptions)->DoCombToyMC()) tool_reconstruction_SF.bookCombToyMCScaleFactors((*fAnaOptions)->MCSystNumbers());    
        tool_reconstruction_SF.initialize();

        tool_identification_medium_SF.addFileName((ZeeDMisc::FindFile((*fAnaOptions)->IdentificationEfficiencyElectronMediumFileName())).Data());
  //      if ((*fAnaOptions)->DoCombToyMC()) tool_identification_medium_SF.bookCombToyMCScaleFactors((*fAnaOptions)->MCSystNumbers());    
        tool_identification_medium_SF.initialize();

        tool_identification_tight_SF.addFileName((ZeeDMisc::FindFile((*fAnaOptions)->IdentificationEfficiencyElectronTightFileName())).Data());
  //      if ((*fAnaOptions)->DoCombToyMC()) tool_identification_tight_SF.bookCombToyMCScaleFactors((*fAnaOptions)->MCSystNumbers());   
        tool_identification_tight_SF.initialize();

        tool_identification_loose_SF.addFileName((ZeeDMisc::FindFile((*fAnaOptions)->IdentificationEfficiencyElectronLooseFileName())).Data());
  //      if ((*fAnaOptions)->DoCombToyMC()) tool_identification_loose_SF.bookCombToyMCScaleFactors((*fAnaOptions)->MCSystNumbers());   
        tool_identification_loose_SF.initialize();

        tool_identification_Fwd_tight_SF.addFileName((ZeeDMisc::FindFile((*fAnaOptions)->IdentificationEfficiencyElectronFwdTightFileName())).Data());
  //      if ((*fAnaOptions)->DoCombToyMC()) tool_identification_Fwd_tight_SF.bookCombToyMCScaleFactors((*fAnaOptions)->MCSystNumbers());
        tool_identification_Fwd_tight_SF.initialize();    

        tool_trigger_SF.addFileName((ZeeDMisc::FindFile((*fAnaOptions)->TriggerEfficiencyElectronFileName())).Data());
 //       if ((*fAnaOptions)->DoCombToyMC()) tool_trigger_SF.bookCombToyMCScaleFactors((*fAnaOptions)->MCSystNumbers());    
        tool_trigger_SF.initialize();

        // XXX: still the old tool, change to the new one
        //sFTool = new ElectronEfficiencySFTool((ZeeDMisc::FindFile("SFIso.root")).Data(), "Iso_wrt_Tight");

        }

//------------------------------------------------------
void ZeeDCalcRecWeight::Calculate(ZeeDEvent* event){

    // Calculate electron weights for the given event

    // Get electrons
    const TObjArray* electrons = event->GetElectrons();

    // Loop over electrons
    for ( Int_t i = 0; i < electrons->GetEntriesFast(); ++i ) {

        ZeeDElectron* electron =  static_cast< ZeeDElectron* > (electrons->At(i));

        // be careful:
        // PATCore::ParticleDataType::Data corresponds to the Data,
        // PATCore::ParticleDataType::Full corresponds to the Full simulation (is used for central electrons now),
        // PATCore::ParticleDataType::FastShower corresponds to the FrozenShower simulation (is used for forward electrons now)
        PATCore::ParticleDataType::DataType electron_type = PATCore::ParticleDataType::Data;
        if ((*fAnaOptions)->IsData()) {	// data
            electron_type = PATCore::ParticleDataType::Data;
        } else {			// mc
            if(electron->IsCentral()) {		// central mc
                electron_type = PATCore::ParticleDataType::Full;
                //	  } else {				// forward mc
                // FIXME: should be FS, but no corresponding table yet
                //    electron_type = PATCore::ParticleDataType::FastShower;
                //	    electron_type = PATCore::ParticleDataType::Full;	
        }
        }
        const TLorentzVector& cluster	= electron->Cluster().Get().fourVector;
        const double eta_cluster	= cluster.Eta();
        const double pt_MeV		= electron->GetFourVector().Pt()* 1000.; // GeV -> MeV
        const int    run_number		= event->RunNumber().Get();
        this->CalcReconstructionSF(electron, run_number, electron_type, pt_MeV, eta_cluster);
        this->CalcTriggerSF(electron, 200805, electron_type, pt_MeV, eta_cluster);
        this->CalcIdentificationSF(electron, run_number, electron_type, pt_MeV, eta_cluster);
        this->CalcIsolationSF(electron);

    }

}


//------------------------------------------------------
void ZeeDCalcRecWeight::CalcReconstructionSF(	ZeeDElectron* electron,
        int run_number,
        PATCore::ParticleDataType::DataType electron_type,
        double pt_MeV,
        double eta_cluster) {

    ZeeDLeptonBags::ReconstructionSF reconstructionSF;

    if ((*fAnaOptions)->ApplyRecoCorrection() && this->isElecEffCorrRecRegion(eta_cluster, pt_MeV)) {

        const Root::TResult&   reco_eff 	= tool_reconstruction_SF.calculate(electron_type, run_number, eta_cluster, pt_MeV);
        double 		   weight	= reco_eff.getScaleFactor();
        double		   weight_unc	= reco_eff.getTotalUncertainty();
        reconstructionSF.RecoTrkQual = weight;
        //std::cout << "Reco unc " << weight_unc << std::endl;	
        // The uncertainty shifts
        if (fSys->isShiftInUse(ZeeDSystematics::eOffRecEffUp)) {
            reconstructionSF.RecoTrkQual += weight_unc;
        } else if (fSys->isShiftInUse(ZeeDSystematics::eOffRecEffDown)) {
            reconstructionSF.RecoTrkQual -= weight_unc; 
        } else if (fSys->isShiftInUse(ZeeDSystematics::eOffRecEffOff)) {
            reconstructionSF.RecoTrkQual  = 1.; 
        } else if (fSys->isShiftInUse(ZeeDSystematics::eOffRecEffToyMC)) {

            ZeeDSystematics::ZeeDSingleSystematics* toyMCsyst = fSys->isShiftInUse(ZeeDSystematics::eOffRecEffToyMC);
            int idxToy = toyMCsyst->toyMCIndex;

            int first_toy_position_reco  = tool_reconstruction_SF.getFirstToyMCPosition(); 

            if (first_toy_position_reco+idxToy<=tool_reconstruction_SF.getLastToyMCPosition()){      
                reconstructionSF.RecoTrkQual = reco_eff.getResult(first_toy_position_reco+idxToy);
            }

        }

    } else {

        reconstructionSF.RecoTrkQual  = 1.;   
    }

    electron->ReconstructionSF().Set(reconstructionSF);
}

//------------------------------------------------------
void ZeeDCalcRecWeight::CalcTriggerSF(	ZeeDElectron* electron,
        int run_number,
        PATCore::ParticleDataType::DataType electron_type,
        double pt_MeV,
        double eta_cluster) {

    ZeeDLeptonBags::TriggerSF triggerSFbag;
    ZeeDEnum::Trigger::Value default_trigger = ZeeDMisc::GetDefaultSingleElectronTrigger(run_number);
    //ZeeDEnum::Trigger::Value default_trigger = ZeeDEnum::Trigger::EF_e15_loose1;
    if ((*fAnaOptions)->ApplyTriggerCorrection() && this->isElecEffCorrTriggerRegion(eta_cluster, pt_MeV)) {


        const Root::TResult& trig_eff  = tool_trigger_SF.calculate(electron_type, run_number, eta_cluster, pt_MeV);
        double weight     		   = trig_eff.getScaleFactor();
        double eff 				   = trig_eff.getEfficiency();
        double weight_unc		   = trig_eff.getTotalUncertainty();
        //	std::cout << "Trigger uncertanty = " << weight_unc<< std::endl;
        triggerSFbag.TrigSF[default_trigger] = weight; 
        triggerSFbag.SFTrig = weight;
        // The uncertainty shifts
        if (fSys->isShiftInUse(ZeeDSystematics::eOffTrigUp)) {
            //std::cout << "HERE!"  << weight << weight+weight_unc<<std::endl;
            triggerSFbag.SFTrig += weight_unc;
            triggerSFbag.TrigSF[default_trigger]    += weight_unc;
        } else if (fSys->isShiftInUse(ZeeDSystematics::eOffTrigDown)) {
            triggerSFbag.SFTrig -= weight_unc;
            triggerSFbag.TrigSF[default_trigger]    -= weight_unc; 
        } else if (fSys->isShiftInUse(ZeeDSystematics::eOffTrigOff)) {
            triggerSFbag.TrigSF[default_trigger]     = 1.; 
        } else if (fSys->isShiftInUse(ZeeDSystematics::eOffTrigToyMC)) {
            ZeeDSystematics::ZeeDSingleSystematics* toyMCsyst = fSys->isShiftInUse(ZeeDSystematics::eOffTrigToyMC);
            //int idxToy = toyMCsyst->toyMCIndex;    
            //double sf = fSys->getToySystematics(weight, weight_unc);
            int idxToy = toyMCsyst->toyMCIndex;
            double sf = getTrigToySF(idxToy, eta_cluster, pt_MeV);
            //std::cout << idxToy << "  " << weight << "  " << weight_unc << "  " << sf << std::endl;
            //int first_toy_position_Fwd_tight  = tool_trigger_SF.getFirstToyMCPosition(); 
            //std::cout << "HEEERE! " << std::endl;
            //if (first_toy_position_Fwd_tight+idxToy<=tool_identification_Fwd_tight_SF.getLastToyMCPosition()){     
                //double sf = trig_eff.getResult(first_toy_position_Fwd_tight+idxToy);
                //std::cout << weight << "  " << weight_unc << "   " << sf << std::endl;
                triggerSFbag.SFTrig = sf;
                triggerSFbag.TrigSF[default_trigger]= sf;
            //}      
            //const UInt_t css = fSys->CurrentSystShift(ZeeDSystematics::eOffTrigToyMC);
            //	  triggerSFbag.TrigSF[default_trigger]    = sFTrig_EF_e12_medium_MediumPP_IJ->GetToyMCScaleFactor(css, eta_cluster, pt_GeV);
        }

        triggerSFbag.TrigEff= -1;
    } else {

        triggerSFbag.TrigSF[default_trigger]     = 1.; 
    }  

    electron->TriggerSF().Set(triggerSFbag);
}

void ZeeDCalcRecWeight::makeToyHist(TH2D* hist, TH2D*& histReturn){
    for (int i=1; i< hist->GetNbinsX(); i++){
        for (int j=1; j<hist->GetNbinsY(); j++){
            double sf=hist->GetBinContent(i,j);
            double sf_err=hist->GetBinError(i,j);
            //std::cout << sf << " " << sf_err << std::endl;
            double sfRet = fSys->getToySystematics(sf, sf_err);
            histReturn->SetBinContent(i,j, sfRet);
        }
    }
}

void ZeeDCalcRecWeight::BookIDSF() {

    TFile* file = new TFile( ZeeDMisc::FindFile((*fAnaOptions)->IdentificationEfficiencyElectronMediumFileName()).Data());
    TH2D* hist = (TH2D*)file->Get("0_999999999/FullSim_sf");
/*    TFile* fileOut = new TFile( "ToyMCIDOut.root", "RECREATE");
    hist->Write();
    TH2D* hist4=new TH2D(*hist);
    hist4->SetName("totvariation");
    hist4->Add(hist, -1);*/
    IDToy= new TObjArray();
    for (int i=0; i< (*fAnaOptions)->MCSystNumbers(); i++){
        TH2D* hist2 = new TH2D(*hist);
        this->makeToyHist(hist, hist2);
/*        hist2->Write();
        TH2D* hist3= new TH2D(*hist2);
        hist3->SetName("diff");
        hist3->Add(hist, -1);
        hist3->Write();
        hist3->Multiply(hist3);
        hist4->Add(hist3);*/
        IDToy->Add((TObject*)hist2);
    
    }
/*    hist4->Scale(1./(*fAnaOptions)->MCSystNumbers());
    hist4->Write();
    fileOut->Close();*/
}

void ZeeDCalcRecWeight::BookTrigSF(){
    TFile* file = new TFile( ZeeDMisc::FindFile((*fAnaOptions)->TriggerEfficiencyElectronFileName()).Data());
    TH2D* hist = (TH2D*)file->Get("200804_216432/FullSim_sf");
/*    TFile* fileOut = new TFile("ToyMCTrigOut.root", "RECREATE");
    hist->Write();*/
    TrigToy= new TObjArray();
    for (int i=0; i< (*fAnaOptions)->MCSystNumbers(); i++){
        TH2D* hist2 = new TH2D(*hist);
        makeToyHist(hist, hist2);
 //       hist2->Write();
        TrigToy->Add((TObject*)hist2);
    }
   // fileOut->Close();
}

double ZeeDCalcRecWeight::getIDToySF(int toyNum, double eta, double pt){
    TH2D* hist = (TH2D*)IDToy->At(toyNum);
    int binYn = hist->GetYaxis()->FindBin(eta);
    int binXn = hist->GetXaxis()->FindBin(pt);
    double sf = hist->GetBinContent(binXn, binYn);
    return sf;
}

double ZeeDCalcRecWeight::getTrigToySF(int toyNum, double eta, double pt){
    TH2D* hist = (TH2D*)TrigToy->At(toyNum);
    int binYn = hist->GetYaxis()->FindBin(eta);
    int binXn = hist->GetXaxis()->FindBin(pt);
    double sf = hist->GetBinContent(binXn, binYn);
    return sf;
}
        

//------------------------------------------------------
void ZeeDCalcRecWeight::CalcIdentificationSF(	ZeeDElectron* electron,
        int run_number,
        PATCore::ParticleDataType::DataType electron_type,
        double pt_MeV,
        double eta_cluster) {

    ZeeDLeptonBags::IdentificationSF identificationSF;

    if ((*fAnaOptions)->ApplyCentralIDCorrection() && this->isElecEffCorrIdCntRegion(eta_cluster, pt_MeV)) {

        const Root::TResult& result_ident_medium	= tool_identification_medium_SF.calculate(electron_type, run_number, eta_cluster, pt_MeV);
        double		 weight_medium		= result_ident_medium.getScaleFactor();
        double		 weight_unc_medium	= result_ident_medium.getTotalUncertainty();
        //std::cout << "ID uns = " << weight_unc_medium << std::endl;
        //std::cout << weight_medium << std::endl;
        const Root::TResult& result_ident_tight	= tool_identification_tight_SF.calculate(electron_type, run_number, eta_cluster, pt_MeV);
        double		 weight_tight		= result_ident_tight.getScaleFactor();
        double		 weight_unc_tight	= result_ident_tight.getTotalUncertainty();
        const Root::TResult& result_ident_loose	= tool_identification_loose_SF.calculate(electron_type, run_number, eta_cluster, pt_MeV);
        double		 weight_loose		= result_ident_loose.getScaleFactor();
        double		 weight_unc_loose	= result_ident_loose.getTotalUncertainty();
        identificationSF.MediumPP	= weight_medium;
        identificationSF.TightPP	= weight_tight;
        identificationSF.LoosePP	= weight_loose;

        if (fSys->isShiftInUse(ZeeDSystematics::eOffIDEffUp)) {
            identificationSF.MediumPP += weight_unc_medium; 
            identificationSF.TightPP  += weight_unc_tight; 
            identificationSF.LoosePP  += weight_unc_loose; 
        } else if (fSys->isShiftInUse(ZeeDSystematics::eOffIDEffDown)) {
            identificationSF.MediumPP -= weight_unc_medium; 
            identificationSF.TightPP  -= weight_unc_tight; 
            identificationSF.LoosePP  -= weight_unc_loose; 
        } else if (fSys->isShiftInUse(ZeeDSystematics::eOffIDEffOff)) {
            identificationSF.MediumPP  = 1.;
            identificationSF.TightPP   = 1.;
            identificationSF.LoosePP   = 1.;
        } else if (fSys->isShiftInUse(ZeeDSystematics::eOffIDEffToyMC)) {
            ZeeDSystematics::ZeeDSingleSystematics* toyMCsyst = fSys->isShiftInUse(ZeeDSystematics::eOffIDEffToyMC);
 /*           int idxToy = toyMCsyst->toyMCIndex;

            int first_toy_position_medium = tool_identification_medium_SF.getFirstToyMCPosition(); 
            int first_toy_position_tight  = tool_identification_tight_SF.getFirstToyMCPosition();   
            int first_toy_position_loose  = tool_identification_loose_SF.getFirstToyMCPosition();   

            if (first_toy_position_medium+idxToy<=tool_identification_medium_SF.getLastToyMCPosition()){
                //std::cout << weight_medium << "  " << result_ident_medium.getResult(first_toy_position_medium+idxToy) << std::endl;
                identificationSF.MediumPP = result_ident_medium.getResult(first_toy_position_medium+idxToy);
            }

            if (first_toy_position_tight+idxToy<=tool_identification_tight_SF.getLastToyMCPosition()){      
                identificationSF.TightPP = result_ident_tight.getResult(first_toy_position_tight+idxToy);
            }
            if (first_toy_position_loose+idxToy<=tool_identification_loose_SF.getLastToyMCPosition()){      
                identificationSF.LoosePP = result_ident_loose.getResult(first_toy_position_loose+idxToy);
            }
            */
            int idxToy = toyMCsyst->toyMCIndex;
            double sf = getIDToySF(idxToy, eta_cluster, pt_MeV);
            //double sf = fSys->getToySystematics(weight_medium, weight_unc_medium);
            identificationSF.MediumPP = sf;

        }

    } else {

        identificationSF.MediumPP  = 1.;
        identificationSF.LoosePP   = 1.; 
        identificationSF.TightPP   = 1.; 
    }   

    if ((*fAnaOptions)->ApplyForwardIDCorrection() && this->isElecEffCorrIdFwdRegion(eta_cluster, pt_MeV)) {

        const Root::TResult& result_ident_Fwd_tight	= tool_identification_Fwd_tight_SF.calculate(electron_type, run_number, eta_cluster, pt_MeV);
        double		 weight_Fwd_tight		= result_ident_Fwd_tight.getScaleFactor();
        double		 weight_unc_Fwd_tight	        = result_ident_Fwd_tight.getTotalUncertainty();

        identificationSF.NewFwdTight	= weight_Fwd_tight;

        if (fSys->isShiftInUse(ZeeDSystematics::eOffIDEffFwdUp)) {
            identificationSF.NewFwdTight  += weight_unc_Fwd_tight; 
        } else if (fSys->isShiftInUse(ZeeDSystematics::eOffIDEffFwdDown)) {
            identificationSF.NewFwdTight  -= weight_unc_Fwd_tight; 
        } else if (fSys->isShiftInUse(ZeeDSystematics::eOffIDEffFwdOff)) {
            identificationSF.NewFwdTight   = 1.;
        } else if (fSys->isShiftInUse(ZeeDSystematics::eOffIDEffFwdToyMC)) {

            ZeeDSystematics::ZeeDSingleSystematics* toyMCsyst = fSys->isShiftInUse(ZeeDSystematics::eOffIDEffFwdToyMC);
            int idxToy = toyMCsyst->toyMCIndex;    

            int first_toy_position_Fwd_tight  = tool_identification_Fwd_tight_SF.getFirstToyMCPosition(); 

            if (first_toy_position_Fwd_tight+idxToy<=tool_identification_Fwd_tight_SF.getLastToyMCPosition()){      
                identificationSF.NewFwdTight = result_ident_Fwd_tight.getResult(first_toy_position_Fwd_tight+idxToy);
            }      

        }

    } else {
        identificationSF.NewFwdTight   = 1.;
    } 

    electron->IdentificationSF().Set(identificationSF);

}

//------------------------------------------------------
void ZeeDCalcRecWeight::CalcIsolationSF(ZeeDElectron* electron){

    if (electron->IsEM().IsSet() == kFALSE ) {
        Error("ZeeDCalcElec::CalcIsolationSF",  "Electron does not have IsEM information! ");
        gSystem->Exit(EXIT_FAILURE);
    }

    if (electron->Cluster().IsSet() == kFALSE ) {
        Error("ZeeDCalcElec::CalcIsolationSF",  "Electron does not have cluster! ");
        gSystem->Exit(EXIT_FAILURE);
    }

    /*
       const TLorentzVector& cluster = electron->Cluster().Get().fourVector;
       const Double_t eta_cluster = cluster.Eta();
       const Double_t pt = electron->GetFourVector().Pt();
       */
    ZeeDLeptonBags::IsolationSF isolationSF;

    //     if ((*fAnaOptions)->ApplyElecIsoCorr() && sFTool != NULL) {
    // 
    //         isolationSF.Both = sFTool->GetScaleFactor(eta_cluster, pt);
    // 
    // 	// XXX: now the isolation systematics is wrong;
    // 	// (probably it's something like 0/1 for up/down)
    // 	// to fix this substitute the zero's with the right numbers
    // 	// from the corresponding packge and
    // 	// do not do that handmade "magic" again (see the previous version of ZeeDSystematics)
    //         if (fSys->isShiftInUse(ZeeDSystematics::eOffIsoEffUp)) {
    //             isolationSF.Both += sFTool->GetCorrSyst(0, eta_cluster, pt);
    // //            isolationSF.Both += sFTool->GetCorrSyst(fSys->CurrentSystShift(ZeeDSystematics::eOffIsoEffUp), eta_cluster, pt);
    //         }
    // 
    //         if (fSys->isShiftInUse(ZeeDSystematics::eOffIsoEffDown)) {
    //             isolationSF.Both -= sFTool->GetCorrSyst(0, eta_cluster, pt);
    // //            isolationSF.Both -= sFTool->GetCorrSyst(fSys->CurrentSystShift(ZeeDSystematics::eOffIsoEffDown), eta_cluster, pt);
    //         }
    // 
    //         if (fSys->isShiftInUse(ZeeDSystematics::eOffIsoEffToyMC)) {
    //             isolationSF.Both = sFTool->GetToyMCScaleFactor(0, eta_cluster, pt);
    // //            isolationSF.Both = sFTool->GetToyMCScaleFactor(fSys->CurrentSystShift(ZeeDSystematics::eOffIsoEffToyMC), eta_cluster, pt);
    //         }
    // 
    //     }
    electron->IsolationSF().Set(isolationSF);

}


