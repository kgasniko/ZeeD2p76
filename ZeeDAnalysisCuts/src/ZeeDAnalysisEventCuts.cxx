#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"

// Root includes
#include <TMath.h>
#include <TString.h>
#include <TObjArray.h>

// ZeeD Analysis includes
#include "ZeeDEvent/ZeeDBosonW.h"
#include "ZeeDEvent/ZeeDVertex.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDJet.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDGenParticle.h"

#include "ZeeDMisc/ZeeDMisc.h"


// -----------------------------------------------------------------------------
//              Event specific analysis cuts
// -----------------------------------------------------------------------------

//------------------------------------------------------------------------------

Bool_t ZeeDCutNumberGoodW::evaluate(const ZeeDEvent* event)
{
    return (event->GetNGoodW() == m_numbW);
}

Bool_t ZeeDCutNumberGoodWmu::evaluate(const ZeeDEvent* event)
{
    return (event->GetNGoodWmu() == m_numbW);
}



Bool_t ZeeDCutExistZ::evaluate(const ZeeDEvent* event)
{

    return (event->GetCurrentBoson() != NULL);

}

Bool_t ZeeDCutExistElectrons::evaluate(const ZeeDEvent* event)
{
    UInt_t elec = event->GetElectrons()->GetEntriesFast();
    return elec >= fNElectrons;
}

Bool_t ZeeDCutNumberVert::evaluate(const ZeeDEvent* event)
{
    UInt_t vert = event->GetVertices()->GetEntriesFast();
    return vert >= fNVert;
}



Bool_t ZeeDCutCentralElectrons::evaluate(const ZeeDEvent* event)
{
    const TObjArray* electrons = event->GetElectrons();
    for (int i=0; i<electrons->GetEntriesFast(); i++) {   
        ZeeDElectron* electron = (ZeeDElectron*)electrons->At(i);
        if (electron->IsCentral()) 
            return kTRUE;
    }
    return kFALSE;         

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutExistBornBoson::evaluate(const ZeeDEvent* event)
{

    return (event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born) != NULL);

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutExistBornElectrons::evaluate(const ZeeDEvent* event)
{

    //UInt_t nElectrons = 0;

    /*    if (event->GetGenBornElectron() != NULL) {
          ++nElectrons;
          }

          if (event->GetGenBornPositron() != NULL) {
          ++nElectrons;
          }

          return nElectrons >= fNElectrons;*/
    return kFALSE;

}


//------------------------------------------------------------------------------
Bool_t ZeeDCutPtMinBothBornElecZ::evaluate(const ZeeDEvent* event)
{
    if ( event->GetGenBoson( ZeeDEnum::MCFSRLevel::Born )    == NULL)        {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDGenElectrons leptons = event->GetGenLeptons(   ZeeDEnum::MCFSRLevel::Born );
    const ZeeDGenParticle* e1 = leptons.first;
    const ZeeDGenParticle* e2 = leptons.second;

    if ( e1 == NULL || e2 == NULL ) {
        return kFALSE;        
    }

    // Get four Vectors
    const TLorentzVector& elec1 = e1->GetMCFourVector();
    const TLorentzVector& elec2 = e2->GetMCFourVector();

    const double pt1 = elec1.Pt();
    const double pt2 = elec2.Pt();

    const Double_t minPt = pt1 < pt2 ? pt1 : pt2;

    return minPt > m_cutval;

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutEtaMaxBothBornElecZ::evaluate(const ZeeDEvent* event)
{
    if ( event->GetGenBoson( ZeeDEnum::MCFSRLevel::Born )    == NULL)        {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDGenElectrons leptons = event->GetGenLeptons(   ZeeDEnum::MCFSRLevel::Born );
    const ZeeDGenParticle* e1 = leptons.first;
    const ZeeDGenParticle* e2 = leptons.second;

    if ( e1 == NULL || e2 == NULL ) {
        return kFALSE;        
    }

    // Get four Vectors
    const TLorentzVector& elec1 = e1->GetMCFourVector();
    const TLorentzVector& elec2 = e2->GetMCFourVector();

    const Double_t eta1 = TMath::Abs(elec1.Eta());
    const Double_t eta2 = TMath::Abs(elec2.Eta());

    const Double_t maxEta = (eta1) > (eta2) ? eta1 : eta2;

    return maxEta < m_cutval;

}

Bool_t ZeeDCutEtaMaxBornElecW::evaluate(const ZeeDEvent* event){

    if ( event->GetGenBoson( ZeeDEnum::MCFSRLevel::Born )    == NULL)        {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDGenElectrons leptons = event->GetGenLeptons(   ZeeDEnum::MCFSRLevel::Born );
    const ZeeDGenParticle* e1 = leptons.first;
    const ZeeDGenParticle* e2 = leptons.second;

    if ( e1 == NULL || e2 == NULL ) {
        return kFALSE;        
    }

    if (TMath::Abs(event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born)->GetParticleID()) != 24 ) 
        return kFALSE;

    const ZeeDGenParticle* e;
    if (TMath::Abs(e1->GetParticleID()) == 11 || TMath::Abs(e1->GetParticleID()) == 13) {
        e = e1;
    }else e=e2;

    const TLorentzVector& elec = e->GetMCFourVector();
    const Double_t eta = TMath::Abs(elec.Eta());
    return eta < m_cutval;

}

Bool_t ZeeDCutGenWPlus::evaluate(const ZeeDEvent* event){

    const ZeeDGenParticle* boson = event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);

    if (boson == NULL){
        return kFALSE;
    }

    return (! bIsPos ^ ( boson->GetParticleID() > 0) );
}


Bool_t ZeeDCutPtBornElectronW::evaluate(const ZeeDEvent* event){

    if ( event->GetGenBoson( ZeeDEnum::MCFSRLevel::Born )    == NULL)        {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDGenElectrons leptons = event->GetGenLeptons(   ZeeDEnum::MCFSRLevel::Born );
    const ZeeDGenParticle* e1 = leptons.first;
    const ZeeDGenParticle* e2 = leptons.second;

    if ( e1 == NULL || e2 == NULL ) {
        return kFALSE;        
    }

    if (TMath::Abs(event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born)->GetParticleID()) != 24 ) 
        return kFALSE;

    const ZeeDGenParticle* e;
    if (TMath::Abs(e1->GetParticleID()) == 11 || TMath::Abs(e1->GetParticleID()) == 13) {
        e = e1;
    }else e=e2;


    const TLorentzVector& elec = e->GetMCFourVector();
    const Double_t pt = elec.Pt();
    return pt > m_cutval;
}

Bool_t ZeeDCutMassTransvBornW::evaluate (const ZeeDEvent* event){
    if  (event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born) == NULL) {
        return kFALSE;
    }
    const Double_t mt = event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born)->GetMt();

    return mt > m_cutval;
}

Bool_t ZeeDCutCrackBornElecW::evaluate(const ZeeDEvent* event){
    if ( event->GetGenBoson( ZeeDEnum::MCFSRLevel::Born )    == NULL)        {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDGenElectrons leptons = event->GetGenLeptons(   ZeeDEnum::MCFSRLevel::Born );
    const ZeeDGenParticle* e1 = leptons.first;
    const ZeeDGenParticle* e2 = leptons.second;

    if ( e1 == NULL || e2 == NULL ) {
        return kFALSE;        
    }

    if (TMath::Abs(event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born)->GetParticleID()) != 24 ) 
        return kFALSE;

    const ZeeDGenParticle* e;
    if (TMath::Abs(e1->GetParticleID()) == 11 || TMath::Abs(e1->GetParticleID()) == 13) {
        e = e1;
    }else e=e2;




    const TLorentzVector& elec = e->GetMCFourVector();
    const Double_t eta = TMath::Abs(elec.Eta());
    return eta > 1.52 || eta < 1.32;
}

Bool_t ZeeDCutEtMissBornW::evaluate(const ZeeDEvent* event){
    if ( event->GetGenBoson( ZeeDEnum::MCFSRLevel::Born )    == NULL)        {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDGenElectrons leptons = event->GetGenLeptons(   ZeeDEnum::MCFSRLevel::Born );
    const ZeeDGenParticle* e1 = leptons.first;
    const ZeeDGenParticle* e2 = leptons.second;

    if ( e1 == NULL || e2 == NULL ) {
        return kFALSE;        
    }

    if (TMath::Abs(event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born)->GetParticleID()) != 24 ) 
        return kFALSE;

    const ZeeDGenParticle* e;
    if (TMath::Abs(e1->GetParticleID()) == 12 || TMath::Abs(e1->GetParticleID()) == 14) {
        e = e1;
    }else e=e2;


    const TLorentzVector etMiss = e->GetMCFourVector();  

    return etMiss.Et() > m_cutval;
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutEtaCrackBothBornElecZ::evaluate(const ZeeDEvent* event)
{
    if ( event->GetGenBoson( ZeeDEnum::MCFSRLevel::Born )    == NULL)        {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDGenElectrons leptons = event->GetGenLeptons(   ZeeDEnum::MCFSRLevel::Born );
    const ZeeDGenParticle* e1 = leptons.first;
    const ZeeDGenParticle* e2 = leptons.second;

    if ( e1 == NULL || e2 == NULL ) {
        return kFALSE;        
    }

    // Get four Vectors
    const TLorentzVector& elec1 = e1->GetMCFourVector();
    const TLorentzVector& elec2 = e2->GetMCFourVector();

    const Double_t eta1 = TMath::Abs(elec1.Eta());
    const Double_t eta2 = TMath::Abs(elec2.Eta());

    if ( ((eta1 > m_cutmax) || (eta1 < m_cutmin)) && ((eta2 > m_cutmax) || (eta2 < m_cutmin))) {
        return kTRUE;
    } else {
        return kFALSE;
    }

}

//------------------------------------------------------------------------------                                                                                                              
Bool_t ZeeDCutEtaCFBothBornElecZ::evaluate(const ZeeDEvent* event)
{
    if ( event->GetGenBoson( ZeeDEnum::MCFSRLevel::Born )    == NULL)        {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDGenElectrons leptons = event->GetGenLeptons(   ZeeDEnum::MCFSRLevel::Born );
    const ZeeDGenParticle* e1 = leptons.first;
    const ZeeDGenParticle* e2 = leptons.second;

    if ( e1 == NULL || e2 == NULL ) {
        return kFALSE;        
    }
    // Get four Vectors                                                                                                                                                                       
    const TLorentzVector& elec1 = e1->GetMCFourVector();
    const TLorentzVector& elec2 = e2->GetMCFourVector();

    const Double_t eta1 = TMath::Abs(elec1.Eta());
    const Double_t eta2 = TMath::Abs(elec2.Eta());

    if ( ((eta1 < m_cutvalmaxEMEC) && (eta2 > m_cutvalmin)  && (eta2 < m_cutvalmax)  ) ||
            ((eta2 < m_cutvalmaxEMEC) && (eta1 > m_cutvalmin)  && (eta1 < m_cutvalmax)  ) ) {
        return kTRUE;
    } else {
        return kFALSE;
    }
}


//------------------------------------------------------------------------------                                                                                                              
Bool_t ZeeDCutPtMinCentralBornElecZ::evaluate(const ZeeDEvent* event)
{
    if ( event->GetGenBoson( ZeeDEnum::MCFSRLevel::Born )    == NULL)        {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDGenElectrons leptons = event->GetGenLeptons(   ZeeDEnum::MCFSRLevel::Born );
    const ZeeDGenParticle* e1 = leptons.first;
    const ZeeDGenParticle* e2 = leptons.second;

    if ( e1 == NULL || e2 == NULL ) {
        return kFALSE;        
    }
    // Get four Vectors                                                                                                                                                                       
    const TLorentzVector& elec1 = e1->GetMCFourVector();
    const TLorentzVector& elec2 = e2->GetMCFourVector();

    const Double_t eta1 = TMath::Abs(elec1.Eta());
    const Double_t eta2 = TMath::Abs(elec2.Eta());

    if (  (eta1<eta2 && elec1.Pt()>m_cutval) || (eta2<eta1 && elec2.Pt()>m_cutval) ) {
        return kTRUE;
    } else {
        return kFALSE;
    }

}


//------------------------------------------------------------------------------                                                                                                              
Bool_t ZeeDCutPtMinForwardBornElecZ::evaluate(const ZeeDEvent* event)
{
    if ( event->GetGenBoson( ZeeDEnum::MCFSRLevel::Born )    == NULL)        {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDGenElectrons leptons = event->GetGenLeptons(   ZeeDEnum::MCFSRLevel::Born );
    const ZeeDGenParticle* e1 = leptons.first;
    const ZeeDGenParticle* e2 = leptons.second;

    if ( e1 == NULL || e2 == NULL ) {
        return kFALSE;        
    }

    // Get four Vectors                                                                                                                                                                       
    const TLorentzVector& elec1 = e1->GetMCFourVector();
    const TLorentzVector& elec2 = e2->GetMCFourVector();

    const Double_t eta1 = TMath::Abs(elec1.Eta());
    const Double_t eta2 = TMath::Abs(elec2.Eta());

    if (  (eta1>eta2 && elec1.Pt()>m_cutval) || (eta2>eta1 && elec2.Pt()>m_cutval) ) {
        return kTRUE;
    } else {
        return kFALSE;
    }
}


//------------------------------------------------------------------------------
Bool_t ZeeDCutMassBornBoson::evaluate(const ZeeDEvent* event)
{

    if ( event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born) == NULL ) {

        return kFALSE;

    }

    const ZeeDGenParticle* boson = event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);

    const Double_t M = boson->GetMCFourVector().M();

    // only cut on max mass if it's larger than min mass
    const Bool_t within_window = (m_mwlow < m_mwhigh) ? (m_mwlow < M && M < m_mwhigh) : (m_mwlow < M);
    const Bool_t accept = bMirror ? !within_window : within_window;

    return accept;

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutYBornBoson::evaluate(const ZeeDEvent* event)
{

    if ( event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born) == NULL ) {

        return kFALSE;

    }

    const ZeeDGenParticle* boson = event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);

    const Double_t y = boson->GetMCFourVector().Rapidity();

    if ( (y >= m_ywlow) && (y <= m_ywhigh) ) {
        return kTRUE;
    } else {
        return kFALSE;
    }
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutTrigger::evaluate(const ZeeDEvent* event)
{
    // The function checks a trigger decision for the trigger specified

    if (fTriggerChain == ZeeDEnum::Trigger::DefaultSingleElec) {

        return event->HasPassedTriggerChain(event->GetDefaultSingleElectronTrigger());

    } else if (fTriggerChain == ZeeDEnum::Trigger::DefaultDiElec) {

        return event->HasPassedTriggerChain(event->GetDefaultDiElectronTrigger());

    } else if (fTriggerChain == ZeeDEnum::Trigger::UNKNOWN) {

        Error("ZeeDCutTrigger", "ZeeDCutTrigger is unable to cut on UNKNOWN trigger");
        return kTRUE;

    } else {

        return event->HasPassedTriggerChain(fTriggerChain);
    }

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutTwoTriggersCF::evaluate(const ZeeDEvent* event)
{
    //CF 2012 Analysis. 
    //Checks is event was triggered by 
    //EF_e24vhi_medium1 OR EF_e60_medium1 trigger

    if (event->HasPassedTriggerChain(ZeeDEnum::Trigger::EF_e24vhi_medium1) ||  event->HasPassedTriggerChain(ZeeDEnum::Trigger::EF_e60_medium1)){
        return kTRUE;
    }

    return kFALSE;

}

Bool_t ZeeDCutTriggersW::evaluate(const ZeeDEvent* event)
{
    if (event->HasPassedTriggerChain(ZeeDEnum::Trigger::EF_e15_loose1)/* ||  event->HasPassedTriggerChain(ZeeDEnum::Trigger::EF_e15_loose1) || event->HasPassedTriggerChain(ZeeDEnum::Trigger::EF_e10_medium1)*/){
        //std::cout << " ";
        return kTRUE;
    }
    return kFALSE;

}


//------------------------------------------------------------------------------
Bool_t ZeeDCutRunNumber::evaluate(const ZeeDEvent* event)
{

    if(event->RunNumber().IsSet() == kFALSE) {
        return kFALSE;
    }

    const Int_t runNumber = event->RunNumber().Get();
    DEBUG_MES(runNumber);

    if (runNumber >= m_runnumber_min && runNumber <= m_runnumber_max) {
        DEBUG_MES("pass");
        return kTRUE;
    } else {
        DEBUG_MES("fail");
        return kFALSE;
    }
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutMinNVertex::evaluate(const ZeeDEvent* event)
{
    // Get number of vertices
    const Int_t Nvtx = event->Nvtx().Get();
    return Nvtx >= m_cutval;
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutPriVtxZ::evaluate(const ZeeDEvent* event)
{

    // Get list of vertices:
    const TObjArray* vertices = event->GetVertices();

    if( (vertices->GetEntriesFast() > 0) == kFALSE) {
        return kFALSE;
    }

    // ----- Read Primary Vertex ------------------------

    ZeeDVertex* vertex = static_cast< ZeeDVertex* >( vertices->At(0) );
    const TVector3 threeVector = vertex->GetVertexCandidate();
    const Double_t VtxZ = TMath::Abs(threeVector.z());

    return VtxZ < m_cutval;

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutPriVtxNtrack::evaluate(const ZeeDEvent* event)
{

    // Get list of vertices:
    const TObjArray* vertices = event->GetVertices();

    if( (vertices->GetEntriesFast() > 0) == kFALSE) {
        return kFALSE;
    }

    // ----- Read Primary Vertex ------------------------

    ZeeDVertex* vertex = static_cast< ZeeDVertex* >( vertices->At(0) );
    Int_t numTracks = vertex->GetNTrack();
    return numTracks >= m_cutval;
}


//------------------------------------------------------------------------------
Bool_t ZeeDCutTwoGoodElec::evaluate(const ZeeDEvent* event)
{
    const TObjArray* elecArray = event->GetElectrons();

    int nGoodCentralMedium = 0;

    for ( Int_t i = 0; i < elecArray->GetEntriesFast(); ++i ) {

        const ZeeDElectron* elec = static_cast< ZeeDElectron* > (elecArray->At(i));

        Double_t et     = elec->GetFourVector().Et();
        Double_t absEta = TMath::Abs( elec->Cluster().Get().fourVector.Eta() );

        if ( ( et > (*fAnaOptions)->Elec_PtCut() ) &&
                ( absEta < (*fAnaOptions)->Elec_EtaCut() ) &&
                ( (absEta < (*fAnaOptions)->EtaCrackEMBEMEC_Min()) ||
                  (absEta > (*fAnaOptions)->EtaCrackEMBEMEC_Max()) ) &&
                ( elec->Author().IsSet() ) &&
                ( elec->Author().Get().isElectron ) &&
                ( elec->IsEM().IsSet() ) &&
                ( elec->IsEM().Get().isMediumPP ) ) {

            nGoodCentralMedium++;
        }
    }

    return (nGoodCentralMedium < 3);
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutCentralForwardLooseGoodElec::evaluate(const ZeeDEvent* event)
{
    const TObjArray* elecArray = event->GetElectrons();

    int nGoodCentralTight = 0;
    int nGoodForwardLoose = 0;

    for ( Int_t i = 0; i < elecArray->GetEntriesFast(); ++i ) {

        const ZeeDElectron* elec = static_cast< ZeeDElectron* > (elecArray->At(i));

        Double_t et     = elec->GetFourVector().Et();
        Double_t absEta = TMath::Abs( elec->Cluster().Get().fourVector.Eta() );

        if ( ( et > (*fAnaOptions)->Elec_PtCut() ) &&
                ( absEta < (*fAnaOptions)->Elec_EtaCut() ) &&
                ( (absEta < (*fAnaOptions)->EtaCrackEMBEMEC_Min()) ||
                  (absEta > (*fAnaOptions)->EtaCrackEMBEMEC_Max()) ) &&
                ( elec->Author().IsSet() ) &&
                ( elec->Author().Get().isElectron ) &&
                ( elec->IsEM().IsSet() ) &&
                ( elec->IsEM().Get().isTightPP ) ) {

            nGoodCentralTight++;
        }

        if ( (et > (*fAnaOptions)->Elec_ForwardPtMinCut() )  &&
                ( absEta < (*fAnaOptions)->Elec_EtaFwdMax() ) &&
                ( absEta > (*fAnaOptions)->Elec_EtaFwdMin() ) &&
                ( elec->Author().IsSet() ) &&
                ( elec->Author().Get().isFrwd ) &&
                ( elec->IsEM().IsSet() ) &&
                ( elec->IsEM().Get().isFrwdLoose ) &&
                ( elec->IsGoodOQAOD().IsSet() ) &&
                ( elec->IsGoodOQAOD().Get() ) ) {

            nGoodForwardLoose++;
        }
    }

    return (nGoodCentralTight < 2 && nGoodForwardLoose < 2);
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutCentralForwardTightGoodElec::evaluate(const ZeeDEvent* event)
{
    const TObjArray* elecArray = event->GetElectrons();

    int nGoodCentralTight = 0;
    int nGoodForwardTight = 0;

    for ( Int_t i = 0; i < elecArray->GetEntriesFast(); ++i ) {

        const ZeeDElectron* elec = static_cast< ZeeDElectron* > (elecArray->At(i));

        Double_t et     = elec->GetFourVector().Et();
        Double_t absEta = TMath::Abs( elec->Cluster().Get().fourVector.Eta() );

        if ( ( et > (*fAnaOptions)->Elec_PtCut() ) &&
                ( absEta < (*fAnaOptions)->Elec_EtaCut() ) &&
                ( (absEta < (*fAnaOptions)->EtaCrackEMBEMEC_Min()) ||
                  (absEta > (*fAnaOptions)->EtaCrackEMBEMEC_Max()) ) &&
                ( elec->Author().IsSet() ) &&
                ( elec->Author().Get().isElectron ) &&
                ( elec->IsEM().IsSet() ) &&
                ( elec->IsEM().Get().isTightPP ) ) {

            nGoodCentralTight++;
        }

        if ( (et > (*fAnaOptions)->Elec_ForwardPtMinCut() )  &&
                ( absEta < (*fAnaOptions)->Elec_EtaFwdMax() ) &&
                ( absEta > (*fAnaOptions)->Elec_EtaFwdMin() ) &&
                ( elec->Author().IsSet() ) &&
                ( elec->Author().Get().isFrwd ) &&
                ( elec->IsEM().IsSet() ) &&
                ( elec->IsEM().Get().isFrwdLoose ) &&
                ( elec->IsGoodOQAOD().IsSet() ) &&
                ( elec->IsGoodOQAOD().Get() ) ) {

            nGoodForwardTight++;
        }
    }

    return (nGoodCentralTight < 2 && nGoodForwardTight < 2);
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutLArEventVeto::evaluate(const ZeeDEvent* event)
{

    // if LAr error state is unknown fail the cut
    if(event->LArErrorState().IsSet() == kFALSE) {
        return kFALSE;
    }

    // by construction: LArErrorState = larError for D3PD = my_EventInfo->errorState(EventInfo::LAr)
    // the std Zee cut: larError < 2
    // see also https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/LArCleaningAndObjectQuality#Usage_on_AOD
    if(event->LArErrorState().Get() < 2) {
        return kTRUE;
    }

    return kFALSE;
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutGRLEvent::evaluate(const ZeeDEvent* event) {

    const Bool_t isInGRL = event->IsInGRL().IsSet() && event->IsInGRL().Get();

    return isInGRL;

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutMinClusterEtNElec::evaluate(const ZeeDEvent* event) {

    const RawEvent* rawEvent = event->GetRawEvent();

    const TObjArray* electrons = bUseRawEvent ? rawEvent->GetElectrons() : event->GetElectrons();

    UInt_t n = 0;

    for ( Int_t i = 0; i < electrons->GetEntriesFast(); ++i ) {

        const ZeeDElectron* electron =  static_cast< ZeeDElectron* > (electrons->At(i));

        if(bRequireIsEM &&
                electron->IsEM().IsSet() &&
                electron->IsEM().Get().*pIsEMType == kFALSE) {

            continue;

        }

        if(electron->Cluster().IsSet() &&
                electron->Cluster().Get().fourVector.Et() >= fMinClusterEt) {

            ++n;

        }

    }

    return n >= fNElec;

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutEventMaxMissEt::evaluate(const ZeeDEvent* event)
{
    // Get Miss Et
    const TObjArray* etmisarr = event->GetEtMissArray();

    if ( etmisarr == NULL) {
        return kFALSE;
    }
    ZeeDEtMiss* etmiss = static_cast< ZeeDEtMiss* >( etmisarr->At(0) );

    if ( etmiss == NULL) {
        //std::cout << " NoMIT \n";
        return kFALSE;
    }

    // Get Et miss

    const Double_t etMiss = etmiss->GetEt();
    //std::cout << etMiss << " \n";
    return etMiss < m_cutval;
}

Bool_t ZeeDCutCorRecoilEtMissW::evaluate(const ZeeDEvent* event)
{
    const ZeeDBosonW* w = event->GetCurrentBosonW();
    if (w == NULL) {
        return kFALSE;
    }
    /*  	const ZeeDLepton* lep = w->GetLep();
            const TLorentzVector lepLV = lep->GetFourVector();

            const TObjArray* etmisarr = event->GetEtMissArray();

            if (etmisarr == NULL) {
            return kFALSE;
            }

            ZeeDEtMiss* etMiss = static_cast<ZeeDEtMiss*>(etmisarr->At(0));
            if (etMiss == NULL) {
            return kFALSE;
            }

            TLorentzVector etMissLV;
            etMissLV.SetPxPyPzE(etMiss->GetCorRecoilEtX(), etMiss->GetCorRecoilEtY(), 0, etMiss->GetCorRecoilEt());
            etMissLV+=lepLV;*/
    return w->GetEtMiss() > m_cutval;	
    //	return etMissLV.Et() > m_cutval;
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutEventMinMissEt::evaluate(const ZeeDEvent* event)
{
    // Get Miss Et
    const TObjArray* etmisarr = event->GetEtMissArray();

    if ( etmisarr == NULL) {
        return kFALSE;
    }
    ZeeDEtMiss* etmiss = static_cast< ZeeDEtMiss* >( etmisarr->At(0) );

    if ( etmiss == NULL) {
        return kFALSE;
    }

    // Get Et miss
    const Double_t etMiss = etmiss->GetEt();
    //std::cout << etMiss << " \n";
    return etMiss > m_cutval;
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutEventMaxMissMt::evaluate(const ZeeDEvent* event)
{
    // Get Miss Et
    const TObjArray* etmisarr = event->GetEtMissArray();

    if ( etmisarr == NULL) {
        return kFALSE;
    }
    ZeeDEtMiss* etmiss = static_cast< ZeeDEtMiss* >( etmisarr->At(0) );

    if(event->GetCurrentBoson() == NULL){
        return kFALSE;
    }

    const ZeeDLepton* electron = event->GetCurrentBoson()->GetCentralLep();

    if ( electron == NULL) {
        return kFALSE;
    }

    const TLorentzVector& elec = electron->GetFourVector();  

    if ( etmiss == NULL) {
        return kFALSE;
    }

    //Calculate MissingMt
    const Double_t mtMiss = sqrt( fabs(elec.Pt() * etmiss->GetEt() *  cos( elec.Phi() - etmiss->GetPhi()) ) );

    //std::cout << elec.Pt() << "  " << etmiss->GetEt() << "  " << elec.Phi() - etmiss->GetPhi() << "  " << cos( elec.Phi() - etmiss->GetPhi() ) << " \n";

    //std::cout << mtMiss << " \n";
    return mtMiss < m_cutval;
}


//------------------------------------------------------------------------------
Bool_t ZeeDCutPeriodEvent::evaluate(const ZeeDEvent* event) {

    return event->GetPeriod() == fPeriod;

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutPtCone20overEt::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }
    // Get the 2 electrons:
    const ZeeDElectron* e =(ZeeDElectron*) event->GetCurrentBoson()->GetCentralLep();

    if( e->Shower().IsSet()  == kFALSE ) {
        return kFALSE;
    }

    const TLorentzVector& elec = e->GetFourVector();

    const double Et = elec.Et();

    const ZeeDElectronBags::Shower elec1_shower = e->Shower().Get();    
    Double_t elec1_ptcone20 = elec1_shower.ptcone20/1000;    
    Double_t PtCone20overEt = elec1_ptcone20/Et; 


    return PtCone20overEt < m_cutval;
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutNZeeBosons::evaluate(const ZeeDEvent* event)
{

    return (event->GetZBosons()->GetEntriesFast() == m_nbosons);
}
/*
//------------------------------------------------------------------------------
Bool_t ZeeDCutNZemuBosons::evaluate(const ZeeDEvent* event)
{

return (event->GetZemuBosons()->size() == m_nbosons);
}
*/
