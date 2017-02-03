
#include "ZeeDCalculator/ZeeDCalcMuon.h"

// Root libraries
#include <TSystem.h>
#include <TMath.h>
#include <TObjArray.h>
#include <TFile.h>
#include <TH1D.h>
#include <TROOT.h>

// Athena libraries
#include "GaudiKernel/ServiceHandle.h"
#include "StoreGate/StoreGateSvc.h"

// ZeeD analysis includes
#include "ZeeDTools/ZeeDSystematics.h"
#include "ZeeDTools/ZeeDCalibrator.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDMuon.h"
#include "ZeeDEvent/ZeeDVertex.h"
#include "ZeeDEvent/ZeeDGenParticle.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include "ZeeDTools/ZeeDBags.h"

#include "MuonMomentumCorrections/SmearingClass.h"

#include "ZeeDMisc/ZeeDMisc.h"

ZeeDCalcMuon::ZeeDCalcMuon():
    fEventInProcess(NULL),
    fSvcHelper("ZeeDCalcMuon"),
    fAnaOptions(fSvcHelper.GetAnaSvc())
{
    mcp_smear = new MuonSmear::SmearingClass("Data12", "staco", "q_pT", "Rel17.2Sum13");
    mcp_smear->UseGeV();
    fSys =ZeeDSystematics::getInstance();

}

ZeeDCalcMuon::~ZeeDCalcMuon()
{

}

void ZeeDCalcMuon::Calculate(ZeeDEvent* event)
{
    fEventInProcess = event;
    if (!(*fAnaOptions)->IsMC())
        return;
    const TObjArray* muonArray = event->GetMuons();

    for ( Int_t i=0; i<muonArray->GetEntriesFast(); ++i){

        ZeeDMuon* muon= static_cast<ZeeDMuon*> (muonArray->At(i));
        mcp_smear->SetSeed(event->EventNumber().Get(), i);
        this->CalcMomentum(muon);
    }
}

void ZeeDCalcMuon::CalcMomentum(ZeeDMuon* muon)	
{
    if (!muon->Properties().IsSet()) return;
    ZeeDMuonBags::CorMomentum momCor;

    if (muon->Properties().Get().fisCombinedMuon){
        Double_t pTCB_smeared;
        Double_t pTMS_smeared;
        Double_t pTID_smeared;

        if (!muon->CombTrack().IsSet() || !muon->IDtrack().IsSet() || !muon->MStrack().IsSet())
            return;
        const TLorentzVector fv = muon->GetFourVector();
        
        //Smearing i
        double ptID  = fabs(sin(muon->IDtrack().Get().fidtheta_exPV)/muon->IDtrack().Get().fidqoverp_exPV);
        double ptMS = fabs(sin(muon->MStrack().Get().fmstheta_exPV)/muon->MStrack().Get().fmsqoverp_exPV);
        double ptCB = muon->CombTrack().Get().ftrackpt;
        double ptCBExtr=fabs(sin(muon->CombTrack().Get().ftheta_exPV)/muon->CombTrack().Get().fqoverp_exPV);

        mcp_smear->Event(ptMS, ptID, 
                ptCB, muon->CombTrack().Get().ftracketa,
                muon->getCharge(), muon->CombTrack().Get().ftrackphi);
        
        pTCB_smeared = mcp_smear->pTCB(); 
        pTMS_smeared = mcp_smear->pTMS();
        pTID_smeared = mcp_smear->pTID();

        if (fSys->isShiftInUse(ZeeDSystematics::muSmearScaleUp)){
            mcp_smear->PTVar(pTMS_smeared,  pTID_smeared, pTCB_smeared, "SCALEUP");
        }else if (fSys->isShiftInUse(ZeeDSystematics::muSmearScaleDown)){
            mcp_smear->PTVar(pTMS_smeared,  pTID_smeared, pTCB_smeared, "SCALELOW");
        } else if (fSys->isShiftInUse(ZeeDSystematics::muSmearMSUp)){
            mcp_smear->PTVar(pTMS_smeared,  pTID_smeared, pTCB_smeared, "MSUP");
        } else if (fSys->isShiftInUse(ZeeDSystematics::muSmearMSDown)){
            mcp_smear->PTVar(pTMS_smeared,  pTID_smeared, pTCB_smeared, "MSLOW");
        } else if (fSys->isShiftInUse(ZeeDSystematics::muSmearIDUp)){
            mcp_smear->PTVar(pTMS_smeared,  pTID_smeared, pTCB_smeared, "IDUP");
        } else if (fSys->isShiftInUse(ZeeDSystematics::muSmearIDDown)){
            mcp_smear->PTVar(pTMS_smeared,  pTID_smeared, pTCB_smeared, "IDLOW");
        } 		


        TLorentzVector newMuonFv;
        newMuonFv.SetPtEtaPhiM(pTCB_smeared, fv.Eta(), fv.Phi(), fv.M());
        muon->setFourVector(newMuonFv);
        momCor.fMSPtCor = pTMS_smeared;
        momCor.fIDPtCor = pTID_smeared;
        muon->CorMomentum().Set(momCor);
        muon->CorMomentum().Lock();

        /*		if (mcp_smear->ChargeFlipCB()== 1){
                muon->setCharge(-1*muon->getCharge());
                }
                */
    }
}

