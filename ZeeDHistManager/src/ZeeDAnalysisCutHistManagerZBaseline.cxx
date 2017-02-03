#include "ZeeDHistManager/ZeeDAnalysisCutHistManagerZBaseline.h"

// Root includes
#include <TString.h>

// ZeeD analysis includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

// Cut selector:
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorZCC.h"
#include "ZeeDAnalysisCuts/ZeeDCalcWeightCutDepZ.h"

// Hist managers:
#include "ZeeDHistManager/ZeeDHistManagerElectron.h"
#include "ZeeDHistManager/ZeeDHistManagerBoson.h"
#include "ZeeDHistManager/ZeeDHistManagerZjet.h"
#include "ZeeDHistManager/ZeeDHistManagerEvent.h"
#include "ZeeDHistManager/ZeeDCalibratorHM.h"
#include "ZeeDHistManager/ZeeDHistManagerElectronDetails.h"
#include "ZeeDHistManager/ZeeDHistManagerTruthMatch.h"
#include "ZeeDHistManager/ZeeDHistManagerWriter.h"
#include "ZeeDHistManager/ZeeDHistManagerPrint.h"
#include "ZeeDHistManager/ZeeDHistManagerYield.h"
#include "ZeeDHistManager/ZeeDHistManagerYieldAlt.h"

#include "ZeeDHistManager/ZeeDHistManagerGenInfo.h"

#include "ZeeDHistManager/ZeeDHistManagerZjetAna.h"
#include "ZeeDHistManager/ZeeDHistManagerZplusJet.h"
#include "ZeeDHistManager/ZeeDHistManagerJetCharge.h"

#include "ZeeDHistManager/ZeeDHistManagerWeights.h"
#include "ZeeDHistManager/ZeeDHistManagerQCDBkg.h"

#include "ZeeDHistManager/ZeeDHistManagerZBaseline.h"
#include "ZeeDHist/ZeeDObservable.h"

#include "ZeeDHistManager/ZeeDHistManagerBosonWriter.h"
#include "ZeeDHistManager/ZeeDHistManagerEWUnfoldWriter.h"

#include "ZeeDHistManager/ZeeDGetVariables.h"
#include "ZeeDHist/ZeeDGridHistManager.h"

//#define DEBUG_ON 1
#define USE_DEBUG_MESSAGES
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDArrays/ZeeDArrayD.h"
#include "ZeeDArrays/ZeeDBinGrid.h"

#include <iostream>

#include <boost/algorithm/string/join.hpp>


void ZeeDAnalysisCutHistManagerZBaseline::BookCutHistManager()
{

    DEBUG_MES_START;

    this->SetupCuts();

    IZeeDCalcWeight* cw = new ZeeDCalcWeightCutDepZ();
    this->AddCalcWeighter(cw);

    this->SetDefaultLevel((*fAnaOptions)->MinimalHistoMode() ? ZeeDLevel::Systematics : ZeeDLevel::Analysis);

    ZeeDSystematics* sys = fSys;

    switch(fMode) {

    case Mode::SIGNAL:
        {

            this->BookRec();

            if((*fAnaOptions)->IsMC() && (*fAnaOptions)->FillGenInfo()) {

                if(!(sys != NULL && !sys->isCurrentShiftGenShift() )) {
                    this->BookGen();
                }

                this->BookGenRec();

            }

        }

        break;

    case Mode::QCD:

        if(sys == NULL || sys->isShiftInUse(ZeeDSystematics::eNoShift)) {
            this->BookQCD();
        }

        break;

    default:

        Error("ZeeDAnalysisCutHistManagerZBaseline::BookCutHistManager", "Unknown mode %i", fMode);
        gSystem->Exit(EXIT_FAILURE);

    }


    DEBUG_MES_END;

}

void ZeeDAnalysisCutHistManagerZBaseline::SetupCuts() {

    DEBUG_MES_START;

    this->SetupRecCuts();

    if((*fAnaOptions)->IsMC() && (*fAnaOptions)->FillGenInfo()) {

        this->SetupGenCuts();
        this->SetupGenRecCuts();

    }

    DEBUG_MES_END;

}

void ZeeDAnalysisCutHistManagerZBaseline::SetupRecCuts() {

    DEBUG_MES_START;

    if( (*fAnaOptions)->IsData() ) {
        fRecStages.push_back("ZeeDCutLArEventVeto");
    }

    fRecStages.push_back("ExistZ");

    if((*fAnaOptions)->ApplyGenBosMassCut() && (*fAnaOptions)->FillGenInfo()) {
        fRecStages.push_back("MassGenBornBoson");
    }

    fRecStages.push_back("NTracksAtPrimVtx");
    fRecStages.push_back("OQMaps");

    fRecStages.push_back("AuthorBothElecZ");
    fRecStages.push_back("TrackBothElecZ");

    fRecStages.push_back("EtaMaxBothElecZ");
    fRecStages.push_back("EtaCrackBothElecZ");

    if((*fAnaOptions)->ExtraCrackCut()){
        fRecStages.push_back("EtaCrack2BothElecZ");
    }

    fRecStages.push_back("PtMinBothElecZ");

    fQCDCuts = boost::algorithm::join(fRecStages, "+");

    fRecStages.push_back("ChargeBothElecOppositeZ");
    fRecStages.push_back("MaxTwoGoodElec");
    fRecStages.push_back("IsEMMediumPPOneElecZ");
    fRecStages.push_back("IsEMMediumPPBothElecZ");

    if( (*fAnaOptions)->UseTrigger() ) {

        //    fRecStages.push_back("TrigDefaultDiElecEvent");
        fRecStages.push_back("TrigDefaultDiElecBothElecZ");
        fQCDCuts += "+Trigger_EF2g20_loose";

    }

    fQCDCuts += "+IsEMLooseBothElecZ";

    fRecCuts = boost::algorithm::join(fRecStages, "+");


    if((*fAnaOptions)->WriteEWUnfoldNtuple()) {

        ZeeDCutSelector* cutSelector = this->GetCutSelector();

        if(cutSelector->hasCut("IsReconstructed") == kFALSE) {

            ZeeDCompoundCut* IsReconstructed = new ZeeDCompoundCut("IsReconstructed");

            for(UInt_t n=0; n != fRecStages.size(); ++n) {
                IsReconstructed->addCut(cutSelector->getCut(fRecStages[n]));
            }
            cutSelector->AddCut(IsReconstructed);

        }

        fRecoIsReconstructed = "IsReconstructed";
    }
    // Set default mask
    this->SetDefaultMask(fRecCuts);

    DEBUG_MES_END;

}

void ZeeDAnalysisCutHistManagerZBaseline::SetupGenCuts() {

    DEBUG_MES_START;

    fGenStages.push_back("ExistBornBoson+ExistTwoBornElectrons"); //0

    if((*fAnaOptions)->ApplyGenBosMassCut() && (*fAnaOptions)->FillGenInfo()) {
        fGenStages.push_back("MassGenBornBoson");
    }

    // extrapolate to full eta
    fMinimalGenCuts = "ExistBornBoson+ExistTwoBornElectrons+PtMinBothBornElecZ";

    if((*fAnaOptions)->ApplyGenBosMassCut() && (*fAnaOptions)->FillGenInfo()) {
        fMinimalGenCuts += "+MassGenBornBoson";
    }

    fGenStages.push_back("EtaMaxBothBornElecZ");   //3
    fGenStages.push_back("PtMinBothBornElecZ");    //4
    fGenStages.push_back("EtaCrackBothBornElecZ");   //3
    if((*fAnaOptions)->ExtraCrackCut()){
        fGenStages.push_back("EtaCrack2BothBornElecZ");   //3
    }

    fGenCuts = boost::algorithm::join(fGenStages, "+");

    if((*fAnaOptions)->WriteEWUnfoldNtuple()) {

        ZeeDCutSelector* cutSelector = this->GetCutSelector();

        if(cutSelector->hasCut("TruthFiducial") == kFALSE) {

            ZeeDCompoundCut* TruthFiducial = new ZeeDCompoundCut("TruthFiducial");

            TruthFiducial->addCut(cutSelector->getCut("EtaMaxBothBornElecZ"));
            TruthFiducial->addCut(cutSelector->getCut("PtMinBothBornElecZ"));
            TruthFiducial->addCut(cutSelector->getCut("EtaCrackBothBornElecZ"));
            if((*fAnaOptions)->ExtraCrackCut()){
                TruthFiducial->addCut(cutSelector->getCut("EtaCrack2BothBornElecZ"));
            }
            cutSelector->AddCut(TruthFiducial);


        }

        fTruthIsFiducial = "TruthFiducial";

    }

    DEBUG_MES_END;

}

void ZeeDAnalysisCutHistManagerZBaseline::SetupGenRecCuts() {

    DEBUG_MES_START;

    fGenRecCuts = fRecCuts + "+" + fGenCuts;

    DEBUG_MES_END;

}

std::vector<ZeeDHistManager*> ZeeDAnalysisCutHistManagerZBaseline::GetRecHMs(TString path, TString grid) {

    DEBUG_MES_START;

    path.Prepend(this->getName()+"/");
    path.Append("/");

    ZeeDGridHistManager* tripleGrid = new ZeeDGridHistManager("Triple",
                                                              grid,
                                                              &getAbsY_M_cosTh_rec);

    //tripleGrid->RegisterGridifiedHM<ZeeDHistManagerBoson>(path + "Boson_Triple");

    tripleGrid->RegisterGridifiedHM<ZeeDHistManagerBoson>(path + "Boson_Triple");
    tripleGrid->RegisterGridifiedHM<ZeeDHistManagerElectron>(path + "Electron_Triple");
    tripleGrid->RegisterGridifiedHM<ZeeDHistManagerEvent>(path + "Event_Triple");

    tripleGrid->RegisterGridifiedHM<ZeeDHistManagerQCDBkg>(path + "QCDBkg_Triple");

    std::vector<ZeeDHistManager*> HMs;
    HMs.push_back(tripleGrid);

    //Jet plots
    if ((*fAnaOptions)->CalcJets()) {
      if ((*fAnaOptions)->DoZjet()){
	HMs.push_back(new ZeeDHistManagerZjet(path + "Zjet"));
      }
      if ((*fAnaOptions)->DoZjetAna()){
        HMs.push_back(new ZeeDHistManagerZjetAna(path + "ZjetAna"));
      }
      if ((*fAnaOptions)->DoZplusJet()){
	HMs.push_back(new ZeeDHistManagerZplusJet(path + "ZplusJet"));
      }
      if ((*fAnaOptions)->CalcJetCharge()){
	HMs.push_back(new ZeeDHistManagerJetCharge(path + "JetCharge"));
      }
    }

    HMs.push_back(new ZeeDHistManagerWeights(path + "Weights"));
    HMs.push_back(new ZeeDHistManagerQCDBkg(path + "QCDBkg"));
    HMs.push_back(new ZeeDHistManagerElectron(path + "Electron"));
    HMs.push_back(new ZeeDHistManagerEvent(path + "Event"));
    HMs.push_back(new ZeeDHistManagerBoson(path + "Boson"));

    DEBUG_MES_END;

    return HMs;

}

void ZeeDAnalysisCutHistManagerZBaseline::BookRec()
{

    DEBUG_MES_START;

    ZeeDDoWeight doWeight;

    if( (*fAnaOptions)->IsMC() ) {

        doWeight.IDSFBothMediumPP = kTRUE;
        doWeight.RecSF = kTRUE;
        doWeight.TrigSFDiMediumPP = kTRUE;

    }


    this->AddHMs(this->GetRecHMs("RecOnly/dyMlow/", ZeeDMisc::FindFile("Binning/BosYMass_low.txt")),
                 "DEFAULT", "DEFAULT", doWeight);

    this->AddHMs(this->GetRecHMs("RecOnly/dyM2010/", ZeeDMisc::FindFile("Binning/BosYMass_peak.txt")),
                 "DEFAULT", "DEFAULT", doWeight);

    this->AddHMs(this->GetRecHMs("RecOnly/dyMhigh/", ZeeDMisc::FindFile("Binning/BosYMass_high.txt")),
                 "DEFAULT", "DEFAULT", doWeight);



    //

    /*

    ZeeDDoWeight doWeight_noPtMin;
    doWeight_noPtMin.IDSFBothMediumPP = kFALSE;
    doWeight_noPtMin.RecSF = kFALSE;
    doWeight_noPtMin.TrigSFDiMediumPP = kFALSE;

    this->AddHMs(this->GetRecHMs("RecOnly_noPtMin/dyMlow/", ZeeDMisc::FindFile("Binning/BosYMass_low.txt"), kTRUE),
                 "DEFAULT-PtMinBothElecZ", "DEFAULT-PtMinBothElecZ", doWeight_noPtMin);

    this->AddHMs(this->GetRecHMs("RecOnly_noPtMin/dyM2010/", ZeeDMisc::FindFile("Binning/BosYMass_peak.txt"), kTRUE),
                 "DEFAULT-PtMinBothElecZ", "DEFAULT-PtMinBothElecZ", doWeight_noPtMin);

    this->AddHMs(this->GetRecHMs("RecOnly_noPtMin/dyMhigh/", ZeeDMisc::FindFile("Binning/BosYMass_high.txt"), kTRUE),
                 "DEFAULT-PtMinBothElecZ", "DEFAULT-PtMinBothElecZ", doWeight_noPtMin);

    */

    if((*fAnaOptions)->WriteEWUnfoldNtuple()) {

        ZeeDHistManagerEWUnfoldWriter* EWUnfoldWriter;

        EWUnfoldWriter = new ZeeDHistManagerEWUnfoldWriter(this->getName()+"/RecOnly/EWUnfold",
                                                           ZeeDMisc::FindFile("Binning/BosYMass_regularY.txt"),
                                                           getAbsY_M_cosTh_rec,
                                                           getAbsY_M_cosTh_gen
                                                           );
        EWUnfoldWriter->SetRecoIsReconstructed(ZeeDHistManagerEWUnfoldWriter::yes);
        this->AddMask(fRecCuts, EWUnfoldWriter, fRecCuts, doWeight);
        EWUnfoldWriter->SetLevelMask(0);

        EWUnfoldWriter = new ZeeDHistManagerEWUnfoldWriter(this->getName()+"/RecOnly/EWUnfold_peak",
                                                           ZeeDMisc::FindFile("Binning/BosYFine.txt"),
                                                           getBosonAbsY,
                                                           getBornBosonAbsY
                                                           );
        EWUnfoldWriter->SetRecoIsReconstructed(ZeeDHistManagerEWUnfoldWriter::yes);
        this->AddMask(fRecCuts+"+ZMassZ", EWUnfoldWriter, fRecCuts+"+ZMassZ", doWeight);
        EWUnfoldWriter->SetLevelMask(0);

    }


    /*
    this->AddMaskLoose(fRecCuts, new ZeeDObservable(this->getName()+"RecOnly/Obs/dyMlow",
                                                       ZeeDMisc::FindFile("Binning/BosYMass_low.txt"),
                                                       "",
                                                       getAbsY_M_cosTh_rec,
                                                    NULL,
                                                       NULL,
                                                       kTRUE,
                                                       kFALSE,
                                                    kFALSE), doWeight)->SetLevelMask(0);

    this->AddMaskLoose(fRecCuts, new ZeeDObservable(this->getName()+"RecOnly/Obs/dyM2010",
                                                       ZeeDMisc::FindFile("Binning/BosYMass_peak.txt"),
                                                       "",
                                                       getAbsY_M_cosTh_rec,
                                                    NULL,
                                                       NULL,
                                                       kTRUE,
                                                       kFALSE,
                                                    kFALSE), doWeight)->SetLevelMask(0);

    this->AddMaskLoose(fRecCuts, new ZeeDObservable(this->getName()+"RecOnly/Obs/dyMhigh",
                                                       ZeeDMisc::FindFile("Binning/BosYMass_high.txt"),
                                                       "",
                                                       getAbsY_M_cosTh_rec,
                                                    NULL,
                                                       NULL,
                                                       kTRUE,
                                                       kFALSE,
                                                    kFALSE), doWeight)->SetLevelMask(0);
    */

    /*
    this->AddMask("DEFAULT+Run189280+ZMassZ",
                  new ZeeDHistManagerBosonWriter(this->getName() + "/RecOnly_Run189280/" + "BosonWriter"),
                  "DEFAULT+Run189280+ZMassZ",
                  doWeight);

    this->AddMask("DEFAULT+Run189280+ZMassZ",
                  new ZeeDHistManagerBoson(this->getName() + "/RecOnly_Run189280/" + "Boson"),
                  "DEFAULT+Run189280+ZMassZ",
                  doWeight);
    */

    DEBUG_MES_END;

}

void ZeeDAnalysisCutHistManagerZBaseline::BookQCD() {

    DEBUG_MES_START;

    std::string mask;
    std::string refMask;

    ZeeDDoWeight doWeight;

    if( (*fAnaOptions)->IsMC() ) {

        doWeight.RecSF = kTRUE;

    }


    this->AddHMs(this->GetRecHMs("LooseNotMedium/dyMlow", ZeeDMisc::FindFile("Binning/BosYMass_low.txt")),
                 fQCDCuts, fQCDCuts+"+IsEMMediumOneElecZ", doWeight);

    this->AddHMs(this->GetRecHMs("LooseNotMedium/dyM2010", ZeeDMisc::FindFile("Binning/BosYMass_peak.txt")),
                 fQCDCuts, fQCDCuts+"+IsEMMediumOneElecZ", doWeight);

    this->AddHMs(this->GetRecHMs("LooseNotMedium/dyMhigh", ZeeDMisc::FindFile("Binning/BosYMass_high.txt")),
                 fQCDCuts, fQCDCuts+"+IsEMMediumOneElecZ", doWeight);


    DEBUG_MES_END;


}

std::vector<ZeeDHistManager*> ZeeDAnalysisCutHistManagerZBaseline::GetGenHMs(TString path, TString grid) {
    DEBUG_MES_START;

    path.Prepend(this->getName()+"/");
    path.Append("/");

    ZeeDGridHistManager* tripleGrid = new ZeeDGridHistManager(path+"Boson_Y",
                                                              grid,
                                                              &getAbsY_M_cosTh_gen);
    tripleGrid->RegisterGridifiedHM<ZeeDHistManagerGenInfo>(path+"GenInfo_Triple");

    std::vector<ZeeDHistManager*> HMs;
    HMs.push_back(tripleGrid);

    HMs.push_back( new ZeeDHistManagerGenInfo(    path + "GenInfo") );

    return HMs;

    DEBUG_MES_END;

}

void ZeeDAnalysisCutHistManagerZBaseline::BookGen()
{

    DEBUG_MES_START;

    if((*fAnaOptions)->WriteEWUnfoldNtuple()) {

        ZeeDHistManagerEWUnfoldWriter* EWUnfoldWriter;

        EWUnfoldWriter = new ZeeDHistManagerEWUnfoldWriter(this->getName()+"/GenOnly/EWUnfold",
                                                           ZeeDMisc::FindFile("Binning/BosYMass_regularY.txt"),
                                                           getAbsY_M_cosTh_rec,
                                                           getAbsY_M_cosTh_gen
                                                           );
        EWUnfoldWriter->SetTruthIsFiducial(ZeeDHistManagerEWUnfoldWriter::yes);
        this->AddMask(fGenCuts, EWUnfoldWriter, fGenCuts, this->GetDefaultDoWeight());
        EWUnfoldWriter->SetLevelMask(0);

        EWUnfoldWriter = new ZeeDHistManagerEWUnfoldWriter(this->getName()+"/MinGenOnly/EWUnfold",
                                                           ZeeDMisc::FindFile("Binning/BosYMass_regularY.txt"),
                                                           getAbsY_M_cosTh_rec,
                                                           getAbsY_M_cosTh_gen
                                                           );
        EWUnfoldWriter->SetForAcceptance(ZeeDHistManagerEWUnfoldWriter::yes);
        this->AddMask(fMinimalGenCuts, EWUnfoldWriter, fMinimalGenCuts, this->GetDefaultDoWeight());
        EWUnfoldWriter->SetLevelMask(0);

        // --------------------------------------------------------------------------------

        EWUnfoldWriter = new ZeeDHistManagerEWUnfoldWriter(this->getName()+"/GenOnly/EWUnfold_peak",
                                                           ZeeDMisc::FindFile("Binning/BosYFine.txt"),
                                                           getBosonAbsY,
                                                           getBornBosonAbsY
                                                           );
        EWUnfoldWriter->SetTruthIsFiducial(ZeeDHistManagerEWUnfoldWriter::yes);
        this->AddMask(fGenCuts+"+MassBornBoson", EWUnfoldWriter, fGenCuts+"+MassBornBoson", this->GetDefaultDoWeight());
        EWUnfoldWriter->SetLevelMask(0);

        EWUnfoldWriter = new ZeeDHistManagerEWUnfoldWriter(this->getName()+"/MinGenOnly/EWUnfold_peak",
                                                           ZeeDMisc::FindFile("Binning/BosYFine.txt"),
                                                           getBosonAbsY,
                                                           getBornBosonAbsY
                                                           );
        EWUnfoldWriter->SetForAcceptance(ZeeDHistManagerEWUnfoldWriter::yes);
        this->AddMask(fMinimalGenCuts+"+MassBornBoson", EWUnfoldWriter, fMinimalGenCuts+"+MassBornBoson", this->GetDefaultDoWeight());
        EWUnfoldWriter->SetLevelMask(0);


    }



    this->AddHMs(this->GetGenHMs("GenOnly/dyMlow", ZeeDMisc::FindFile("Binning/BosYMass_low.txt")),
                 fGenCuts,
                 fGenCuts,
                 this->GetDefaultDoWeight()
                 );

    this->AddHMs(this->GetGenHMs("GenOnly/dyM2010", ZeeDMisc::FindFile("Binning/BosYMass_peak.txt")),
                 fGenCuts,
                 fGenCuts,
                 this->GetDefaultDoWeight()
                 );

    this->AddHMs(this->GetGenHMs("GenOnly/dyMhigh", ZeeDMisc::FindFile("Binning/BosYMass_high.txt")),
                 fGenCuts,
                 fGenCuts,
                 this->GetDefaultDoWeight()
                 );

    //--------------------------------------------------------------------------

    this->AddHMs(this->GetGenHMs("MinGenOnly/dyMlow", ZeeDMisc::FindFile("Binning/BosYMass_low.txt")),
                 fMinimalGenCuts,
                 fMinimalGenCuts,
                 this->GetDefaultDoWeight()
                 );

    this->AddHMs(this->GetGenHMs("MinGenOnly/dyM2010", ZeeDMisc::FindFile("Binning/BosYMass_peak.txt")),
                 fMinimalGenCuts,
                 fMinimalGenCuts,
                 this->GetDefaultDoWeight()
                 );

    this->AddHMs(this->GetGenHMs("MinGenOnly/dyMhigh", ZeeDMisc::FindFile("Binning/BosYMass_high.txt")),
                 fMinimalGenCuts,
                 fMinimalGenCuts,
                 this->GetDefaultDoWeight()
                 );


    //


    /*
    this->AddMaskLoose(fGenCuts, new ZeeDObservable(this->getName()+"GenOnly/Obs/dyMlow",
                                                       ZeeDMisc::FindFile("Binning/BosYMass_low.txt"),
                                                       "",
                                                    NULL,
                                                       getAbsY_M_cosTh_gen,
                                                       NULL,
                                                       kFALSE,
                                                       kTRUE,
                                                    kFALSE),this->GetDefaultDoWeight())->SetLevelMask(0);

    this->AddMaskLoose(fGenCuts, new ZeeDObservable(this->getName()+"GenOnly/Obs/dyM2010",
                                                       ZeeDMisc::FindFile("Binning/BosYMass_peak.txt"),
                                                       "",
                                                    NULL,
                                                       getAbsY_M_cosTh_gen,
                                                       NULL,
                                                       kFALSE,
                                                       kTRUE,
                                                       kFALSE),this->GetDefaultDoWeight())->SetLevelMask(0);

    this->AddMaskLoose(fGenCuts, new ZeeDObservable(this->getName()+"GenOnly/Obs/dyMhigh",
                                                       ZeeDMisc::FindFile("Binning/BosYMass_high.txt"),
                                                       "",
                                                    NULL,
                                                       getAbsY_M_cosTh_gen,
                                                       NULL,
                                                       kFALSE,
                                                       kTRUE,
                                                       kFALSE),this->GetDefaultDoWeight())->SetLevelMask(0);
    */

    DEBUG_MES_END;

}

std::vector<ZeeDHistManager*> ZeeDAnalysisCutHistManagerZBaseline::GetGenRecHMs(TString path) {

    DEBUG_MES_START;

    path.Prepend(this->getName()+"/");
    path.Append("/");

    std::vector<ZeeDHistManager*> HMs;

    HMs.push_back( new ZeeDHistManagerGenInfo(       path + "GenInfo" ));
    HMs.push_back( new ZeeDHistManagerWeights(       path + "Weights"));
    HMs.push_back( ZeeDHistManagerZBaseline::GenRec( path + "ZBaselineGenRec", kTRUE));

    DEBUG_MES_END;

    return HMs;

}

void ZeeDAnalysisCutHistManagerZBaseline::BookGenRec() {

    DEBUG_MES_START;

    ZeeDDoWeight doWeight;

    if( (*fAnaOptions)->IsMC() ) {

        doWeight.IDSFBothMediumPP = kTRUE;
        doWeight.RecSF = kTRUE;
        doWeight.TrigSFDiMediumPP = kTRUE;

    }

    if((*fAnaOptions)->WriteEWUnfoldNtuple()) {

        ZeeDHistManagerEWUnfoldWriter* EWUnfoldWriter;

        EWUnfoldWriter = new ZeeDHistManagerEWUnfoldWriter(this->getName()+"/GenRec/EWUnfold",
                                                           ZeeDMisc::FindFile("Binning/BosYMass_regularY.txt"),
                                                           getAbsY_M_cosTh_rec,
                                                           getAbsY_M_cosTh_gen
                                                           );

        EWUnfoldWriter->SetRecoIsReconstructed(ZeeDHistManagerEWUnfoldWriter::yes);
        EWUnfoldWriter->SetTruthIsFiducial(ZeeDHistManagerEWUnfoldWriter::yes);

        this->AddMask(fRecoIsReconstructed+"+"+fTruthIsFiducial,
                      EWUnfoldWriter,
                      fRecoIsReconstructed+"+"+fTruthIsFiducial,
                      doWeight);

        EWUnfoldWriter->SetLevelMask(0);

        EWUnfoldWriter = new ZeeDHistManagerEWUnfoldWriter(this->getName()+"/GenRec/EWUnfold_peak",
                                                           ZeeDMisc::FindFile("Binning/BosYFine.txt"),
                                                           getBosonAbsY,
                                                           getBornBosonAbsY
                                                           );

        EWUnfoldWriter->SetRecoIsReconstructed(ZeeDHistManagerEWUnfoldWriter::yes);
        EWUnfoldWriter->SetTruthIsFiducial(ZeeDHistManagerEWUnfoldWriter::yes);

        this->AddMask(fRecoIsReconstructed+"+"+fTruthIsFiducial+"+MassBornBoson+ZMassZ",
                      EWUnfoldWriter,
                      fRecoIsReconstructed+"+"+fTruthIsFiducial+"+MassBornBoson+ZMassZ",
                      doWeight);

        EWUnfoldWriter->SetLevelMask(0);

    }

    /*
    this->AddMaskLoose(fGenRecCuts, new ZeeDObservable(this->getName()+"GenRec/Obs/dyMlow",
                                                       ZeeDMisc::FindFile("Binning/BosYMass_low.txt"),
                                                       "",
                                                       getAbsY_M_cosTh_rec,
                                                       getAbsY_M_cosTh_gen,
                                                       NULL,
                                                       kTRUE,
                                                       kTRUE,
                                                       kTRUE), doWeight)->SetLevelMask(0);

    this->AddMaskLoose(fGenRecCuts, new ZeeDObservable(this->getName()+"GenRec/Obs/dyM2010",
                                                       ZeeDMisc::FindFile("Binning/BosYMass_peak.txt"),
                                                       "",
                                                       getAbsY_M_cosTh_rec,
                                                       getAbsY_M_cosTh_gen,
                                                       NULL,
                                                       kTRUE,
                                                       kTRUE,
                                                       kTRUE), doWeight)->SetLevelMask(0);

    this->AddMaskLoose(fGenRecCuts, new ZeeDObservable(this->getName()+"GenRec/Obs/dyMhigh",
                                                       ZeeDMisc::FindFile("Binning/BosYMass_high.txt"),
                                                       "",
                                                       getAbsY_M_cosTh_rec,
                                                       getAbsY_M_cosTh_gen,
                                                       NULL,
                                                       kTRUE,
                                                       kTRUE,
                                                       kTRUE), doWeight)->SetLevelMask(0);
    */

    DEBUG_MES_END;

}
