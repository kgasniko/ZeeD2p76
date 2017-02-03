#include "ZeeDAnalysisCuts/ZeeDAnalysisWBosonCuts.h"

// -----------------------------------------------------------------------------
//              W boson analysis selection requirements
// -----------------------------------------------------------------------------

//------------------------------------------------------------------------------
Bool_t ZeeDCutMuonD0W::evaluate(const ZeeDEvent* event){

    const ZeeDBosonW* w = event->GetCurrentBosonW();
    if (w == NULL) {
        return kFALSE;
    }
    const ZeeDMuon* mu =(ZeeDMuon*) w->GetLep();
    if (!mu->CombTrack().IsSet())
        return kFALSE;
    const ZeeDMuonBags::CombTrack track = mu->CombTrack().Get();

    return track.fd0_exPV < m_cutval;
}

Bool_t ZeeDCutMuonTrackIso::evaluate(const ZeeDEvent* event){

    const ZeeDBosonW* w = event->GetCurrentBosonW();
    if (w == NULL) {
        return kFALSE;
    }
    const ZeeDMuon* mu = (ZeeDMuon*)w->GetLep();
    if (!mu->Isolation().IsSet())
        return kFALSE;
    const ZeeDMuonBags::Isolation iso = mu->Isolation().Get();

    return ((iso.fptcone40 < m_cutval) == bDecision);

}

Bool_t ZeeDCutCombinedMuon::evaluate(const ZeeDEvent* event){
    const ZeeDBosonW* w = event->GetCurrentBosonW();
    if (w == NULL) {
        return kFALSE;
    }
    const ZeeDMuon* mu = (ZeeDMuon*) w->GetLep();
    if (!mu->Properties().IsSet())
        return kFALSE;
    const ZeeDMuonBags::Properties prop = mu->Properties().Get();
    return prop.fisCombinedMuon == bDecision;
}

Bool_t ZeeDCutMuonHasIDTrack::evaluate(const ZeeDEvent* event){
    const ZeeDBosonW* w = event->GetCurrentBosonW();
    if (w == NULL) {
        return kFALSE;
    }
    const ZeeDMuon* mu = (ZeeDMuon*)w->GetLep();
    if (!mu->Properties().IsSet())
       return kFALSE;
    const ZeeDMuonBags::Properties prop = mu->Properties().Get();
    return prop.fisCombinedMuon || prop.fisStandAloneMuon || prop.fisSegmentTaggedMuon;
}

Bool_t ZeeDCutEtCone30overEtW::evaluate (const ZeeDEvent* event) 
{
    const ZeeDBosonW* w = event->GetCurrentBosonW();
    if (w == NULL) {
        return kFALSE;
    }
    /*if (w.GetBosonType() != BosonType::Value::Wenu){
      return kFALSE;
      }
      */const ZeeDElectron* elec = (ZeeDElectron*)w->GetLep();
    const TLorentzVector& elec_fourVec = elec->GetFourVector();
    const Double_t elec_et = elec_fourVec.Et();
    if(!elec->Shower().IsSet() || !elec->CaloIsoCor().IsSet())
        return kFALSE;
    const ZeeDElectronBags::Shower elec1_shower = elec->Shower().Get();
    double elec1_etcone30 = elec1_shower.etcone30/1000.;
    const Double_t elec1_etcone30overEt   = elec1_etcone30/elec_et;

    return elec1_etcone30overEt > m_EtCut;

}

Bool_t ZeeDCutLepTrackEtaW::evaluate(const ZeeDEvent* event){

    const ZeeDBosonW* w = event->GetCurrentBosonW();
    if (w == NULL) {
        return kFALSE;
    }
    const ZeeDLepton* mu = w->GetLep();
    if (mu->GetFourVector().Pt() == 0)
        return kFALSE;
    return fabs(mu->GetFourVector().Eta()) < m_cutval;
}

Bool_t ZeeDCutLepTrackEtaWSim::evaluate(const ZeeDEvent* event){

    const ZeeDBosonW* w = event->GetCurrentBosonW();
    if (w == NULL) {
        return kFALSE;
    }
    const ZeeDLepton* mu = w->GetLep();
    if (mu->GetFourVector().Pt() == 0)
        return kFALSE;
    bool ret = (fabs(mu->GetFourVector().Eta()) < 1.0) && (fabs(mu->GetFourVector().Eta()) > 0.1);
    return fabs(mu->GetFourVector().Eta()) < ret;
}

Bool_t ZeeDCutCrackLepTrackEtaW::evaluate(const ZeeDEvent* event){

    const ZeeDBosonW* w = event->GetCurrentBosonW();
    if (w == NULL) {
        return kFALSE;
    }
    const ZeeDLepton* mu = w->GetLep();
    if (mu->GetFourVector().Pt() == 0)
        return kFALSE;
    double eta = mu->GetFourVector().Eta();
    return fabs(eta) > m_max || fabs(eta) < m_min;

}


Bool_t ZeeDCutLepPtMinW::evaluate(const ZeeDEvent* event)
{
    // Get muon of W
    const ZeeDBosonW* w = event->GetCurrentBosonW();
    //std::cout << 0;    
    // Return "fail" if there is no W or electron
    if ( w == NULL) {
        return kFALSE;
    }
    const ZeeDLepton* muon = w->GetLep();
    //std::cout << 1;    
    // Return "fail" if there is no W or muon
    if (muon == NULL) {
        return kFALSE;
    }
    //std::cout <<2;
    // Get muon pt
    Double_t pt = muon->GetFourVector().Pt();
    return pt > m_cutval;
}



Bool_t ZeeDCutEtCone20overEtW::evaluate (const ZeeDEvent* event)
{
    const ZeeDBosonW* w = event->GetCurrentBosonW();
    if (w == NULL) {
        return kFALSE;
    }
    // if (w.GetBosonType() != BosonType::Value::Wenu){
    //       return kFALSE;
    //       }
    const ZeeDElectron* elec = (ZeeDElectron*)w->GetLep(); 
    const TLorentzVector& elec_fourVec = elec->GetFourVector();
    const Double_t elec_et = elec_fourVec.Et();
    if(!elec->Shower().IsSet() || !elec->CaloIsoCor().IsSet())
        return kFALSE;
    const ZeeDElectronBags::Shower elec1_shower = elec->Shower().Get();
    double elec1_etcone20 = elec1_shower.etcone20/1000.;
    const Double_t elec1_etcone20overEt   = elec1_etcone20/elec_et;

    return elec1_etcone20overEt < m_EtCut;

}

Bool_t ZeeDCutEtCone30overEtCorW::evaluate (const ZeeDEvent* event)
{
    const ZeeDBosonW* w = event->GetCurrentBosonW();
    if (w == NULL) {
        return kFALSE;
    }
    /* if (w.GetBosonType() != BosonType::Value::Wenu){
       return kFALSE;
       }
       */const ZeeDElectron* elec = (ZeeDElectron*)w->GetLep();
    const TLorentzVector& elec_fourVec = elec->GetFourVector();
    const Double_t elec_et = elec_fourVec.Et();
    if(!elec->Shower().IsSet() || !elec->CaloIsoCor().IsSet())
        return kFALSE;
    const ZeeDElectronBags::Shower elec1_shower = elec->Shower().Get();
    const ZeeDElectronBags::CaloIsoCor elec1_CaloIsoCor = elec->CaloIsoCor().Get();
    const Double_t elec1_etcone30_PtNPVCorrected = elec1_CaloIsoCor.etcone30_PtNPVCorrected/1000.;
    const Double_t elec1_etcone30_PtNPVCorrectedoverEt   =  elec1_etcone30_PtNPVCorrected/elec_et;

    return elec1_etcone30_PtNPVCorrectedoverEt > m_EtCut; 
}

Bool_t ZeeDCutEtCone20overEtCorW::evaluate (const ZeeDEvent* event)
{
    const ZeeDBosonW* w = event->GetCurrentBosonW();
    if (w == NULL) {
        return kFALSE;
    }
    /* if (w.GetBosonType() != BosonType::Value::Wenu){
       return kFALSE;
       }
       */ const ZeeDElectron* elec = (ZeeDElectron*)w->GetLep();
    const TLorentzVector& elec_fourVec = elec->GetFourVector();
    const Double_t elec_et = elec_fourVec.Et();
    if(!elec->Shower().IsSet() || !elec->CaloIsoCor().IsSet())
        return kFALSE;
    const ZeeDElectronBags::Shower elec1_shower = elec->Shower().Get();
    const ZeeDElectronBags::CaloIsoCor elec1_CaloIsoCor = elec->CaloIsoCor().Get();
    const Double_t elec1_etcone20_PtNPVCorrected = elec1_CaloIsoCor.etcone20_PtNPVCorrected/1000.;
    const Double_t elec1_etcone20_PtNPVCorrectedoverEt   =  elec1_etcone20_PtNPVCorrected/elec_et;

    return elec1_etcone20_PtNPVCorrectedoverEt < m_EtCut;
}



Bool_t ZeeDCutTrigLepW::evaluate(const ZeeDEvent* event)
{
    const ZeeDBosonW* w = event->GetCurrentBosonW();
    // Return "fail" if there is no W or electron
    if ( w == NULL) {
        return kFALSE;
    }
    const ZeeDLepton* electron = w->GetLep();

    if (electron == NULL) {
        return kFALSE;
    }


    return electron->IsMatchedToTriggerChain (fChain);

}

Bool_t ZeeDCutMassTransvW::evaluate(const ZeeDEvent* event)
{
    const ZeeDBosonW* w = event->GetCurrentBosonW();
    if (w == NULL)
        return kFALSE;

    const Bool_t pass = w->GetMt() > m_MassTrCut;
    //if (pass) 
    //      std::cout << "MT!"<<std::endl;
    return pass;

}


Bool_t ZeeDCutLepIdentW::evaluate(const ZeeDEvent* event)
{
    const ZeeDBosonW* w = event->GetCurrentBosonW();
    // Return "fail" if there is no W or electron
    if ( w == NULL) {
        return kFALSE;
    }
    const ZeeDLepton* e = w->GetLep();

    if (e == NULL) {
        return kFALSE;
    }

    const Bool_t pass=
        (e->IsEM().IsSet() && e->IsEM().Get().*pIsEMType == bDecision_IsEMType);


    return pass;

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutElecClustEtMinW::evaluate(const ZeeDEvent* event)
{
    // Get electron of W
    const ZeeDBosonW* w = event->GetCurrentBosonW();

    // Return "fail" if there is no W or electron
    if ( w == NULL) {
        return kFALSE;
    }
    const ZeeDElectron* electron = (ZeeDElectron*)w->GetLep();

    // Return "fail" if there is no W or electron
    if (electron == NULL) {
        return kFALSE;
    }

    if( electron->Cluster().IsSet() == kFALSE ) {
        return kFALSE;
    }

    // Get electron cluster Et
    const Double_t pt = electron->Cluster().Get().eT;
    //    if (pt > m_cutval) 
    //        std::cout << "Pt" <<std::endl;
    return pt > m_cutval;

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutElecAuthorW::evaluate(const ZeeDEvent* event)
{
    // Get electron of W
    const ZeeDBosonW* w = event->GetCurrentBosonW();

    // Return "fail" if there is no W or electron
    if ( w == NULL) {
        return kFALSE;
    }
    const ZeeDElectron* electron = (ZeeDElectron*) w->GetLep();

    // Return "fail" if there is no W or electron
    if (electron == NULL) {
        return kFALSE;
    }

    if( electron->Author().IsSet() ) {

        return electron->Author().Get().isElectron == kTRUE;

    } else {

        return kFALSE;

    }

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutEtaCrackElecW::evaluate(const ZeeDEvent* event)
{
    // Get electron of W
    const ZeeDBosonW* w = event->GetCurrentBosonW();

    // Return "fail" if there is no W or electron
    if ( w == NULL) {
        return kFALSE;
    }
    const ZeeDElectron* electron = (ZeeDElectron*)w->GetLep();

    // Return "fail" if there is no W or electron
    if (electron == NULL) {
        return kFALSE;
    }
    if( electron->Cluster().IsSet() == kFALSE ) {
        return kFALSE;
    }

    const Double_t eta = TMath::Abs( electron->Cluster().Get().fourVector.Eta() );

    if( (eta > m_cutmax) || (eta < m_cutmin) ) {

        return kTRUE;

    } else {

        return kFALSE;

    }

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutElecClustEtaMaxW::evaluate(const ZeeDEvent* event)
{
    // Get electron of W
    const ZeeDBosonW* w = event->GetCurrentBosonW();

    // Return "fail" if there is no W or electron
    if ( w == NULL) {
        return kFALSE;
    }
    const ZeeDElectron* electron = (ZeeDElectron*)w->GetLep();
    // Return "fail" if there is no W or electron
    if (electron == NULL) {
        return kFALSE;
    }

    if( electron->Cluster().IsSet() ) {

        const Double_t eta = TMath::Abs( electron->Cluster().Get().fourVector.Eta() );

        return eta < m_cutval;

    }

    return kFALSE;

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutDefaultTrigW::evaluate(const ZeeDEvent* event)
{
    // Get electron of W
    const ZeeDBosonW* w = event->GetCurrentBosonW();

    // Return "fail" if there is no W or electron
    if ( w == NULL) {
        return kFALSE;
    }
    const ZeeDLepton* electron = w->GetLep();

    // Return "fail" if there is no W or electron
    if (electron == NULL) {
        return kFALSE;
    }

    if(event->RunNumber().IsSet() == kFALSE) {
        return kFALSE;
    }
    const ZeeDEnum::Trigger::Value defaultTrigger = event->GetDefaultSingleElectronTrigger();
    DEBUG_MES(defaultTrigger);

    Bool_t trigDec = electron->IsMatchedToTriggerChain(defaultTrigger);

    if ( trigDec == kTRUE ) {
        DEBUG_MES("pass");
        return kTRUE;
    } else {
        DEBUG_MES("fail");
        return kFALSE;
    }
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutOQMapsW::evaluate(const ZeeDEvent* event)
{
    // Get electron of W
    const ZeeDBosonW* w = event->GetCurrentBosonW();  
    // Return "fail" if there is no W or electron
    if ( w == NULL) {
        return kFALSE;
    }
    const ZeeDElectron* electron = (ZeeDElectron*)w->GetLep(); 
    // Return "fail" if there is no W or electron
    if (electron == NULL) {
        return kFALSE;
    }

    return (electron->IsGoodOQAOD().IsSet()
            && electron->IsGoodOQAOD().Get() == kTRUE) ? true : kFALSE;

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutLepChargePosW::evaluate(const ZeeDEvent* event)
{

    // Get electron of W
    const ZeeDBosonW* w = event->GetCurrentBosonW();

    // Return "fail" if there is no W or electron
    if ( w == NULL) {
        return kFALSE;
    }
    const ZeeDLepton* electron = w->GetLep();

    // Return "fail" if there is no W or electron
    if (electron == NULL) {
        return kFALSE;
    }

    // if no tracks, return fail:
    if ( ! ( electron->TrackParticle().IsSet() ) ) {
        return kFALSE;
    }

    const Double_t charge = electron->getCharge();

    if (charge == 0.0 ) {
        return kFALSE;
    }

    return (! bIsPos ^ (charge > 0.0) );
}

