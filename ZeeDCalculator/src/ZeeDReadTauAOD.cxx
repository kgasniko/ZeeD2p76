#include "ZeeDCalculator/ZeeDReadTauAOD.h"

#include "tauEvent/TauJet.h"
#include "tauEvent/TauPID.h"
#include "tauEvent/TauJetParameters.h"

#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDTau.h"

#include "GaudiKernel/ServiceHandle.h"
#include "StoreGate/StoreGateSvc.h"

#include <TLorentzVector.h>

ZeeDReadTauAOD::ZeeDReadTauAOD() : 
  fSvcHelper("ZeeDReadTauAOD"),
  fTauContainer(NULL),
  fStoreGate(fSvcHelper.GetStoreGate()),
  fAnaOptions(fSvcHelper.GetAnaSvc()) 
{
}

ZeeDReadTauAOD::~ZeeDReadTauAOD() {
}

void ZeeDReadTauAOD::Initialize()
{
  
  StatusCode sc (StatusCode::SUCCESS);

  //Retrieve TauCollection
  sc = (*fStoreGate)->retrieve(fTauContainer, "TauRecContainer");

  if ( sc.isFailure() || !fTauContainer) 
    {
      Error("ZeeDReadTauAOD::ReadInfo", "No TauRecContainer container found");
      gSystem->Exit(EXIT_FAILURE);
    }

}


void ZeeDReadTauAOD::ReadInfo(ZeeDEvent* event)
{

  this->Initialize();


  TObjArray* taus = event->GetTaus();

  taus->Clear();

  // Iterators over the tau container
  Analysis::TauJetContainer::const_iterator tauItrB  = fTauContainer -> begin();
  Analysis::TauJetContainer::const_iterator tauItrE = fTauContainer -> end();

  DEBUG_VAR(tauContainer -> size() * event -> GetGenWeight());

  // Loop over the AOD tau container
  for (  Analysis::TauJetContainer::const_iterator tauItr  = fTauContainer -> begin(); tauItr != tauItrE; ++tauItr) {

    const Analysis::TauJet* pTau = (*tauItr);
    const Analysis::TauPID* pTauID  = pTau->tauID();

    TLorentzVector tauFourVector( pTau->px()/GeV, pTau->py()/GeV, pTau->pz()/GeV, pTau->e()/GeV );
   
//     std::cout << " READ AOD TAU "
// 	      << " E = "<<(*tauItr)->e()
// 	      << " PT = "<<(*tauItr)->pt()
// 	      << " Eta = "<<(*tauItr)->eta()
// 	      << " phi = "<<(*tauItr)->phi()
// 	      << " P = "<<(*tauItr)->p()
// 	      << std::endl; 
      

    // Create new tau candidate
    ZeeDTau* tauCand = new ZeeDTau();

    tauCand -> GetCollectionIndex() = std::distance( tauItrB, tauItr );
    tauCand -> SetFourVector( tauFourVector );
    

    if (pTauID->isTau(TauJetParameters::TauCutTight))
      tauCand->SetIsTight(true);

    if (pTauID->isTau(TauJetParameters::TauCutMedium))
      tauCand->SetIsMedium(true);
     
    if (pTauID->isTau(TauJetParameters::TauCutLoose))
      tauCand->SetIsLoose(true);
     
    if (pTauID->isTau(TauJetParameters::JetBDTSigTight))
      tauCand->SetIsBDTTight(true);
    
    if (pTauID->isTau(TauJetParameters::JetBDTSigMedium))
      tauCand->SetIsBDTMedium(true);
   
    if (pTauID->isTau(TauJetParameters::JetBDTSigLoose))
      tauCand->SetIsBDTLoose(true);
     
    if (pTauID->isTau(TauJetParameters::EleBDTTight))
      tauCand->SetIsEleBDTTight(true);
    
    if (pTauID->isTau(TauJetParameters::EleBDTMedium))
      tauCand->SetIsEleBDTMedium(true);
    
    if (pTauID->isTau(TauJetParameters::EleBDTLoose))
      tauCand->SetIsEleBDTLoose(true);
    
    if (pTauID->isTau(TauJetParameters::TauLlhTight))
      tauCand->SetIsLLHTight(true);
    
    if (pTauID->isTau(TauJetParameters::TauLlhMedium))
      tauCand->SetIsLLHMedium(true);
    
    if (pTauID->isTau(TauJetParameters::TauLlhLoose))
      tauCand->SetIsLLHLoose(true);
    
    if (pTauID->isTau(TauJetParameters::MuonVeto))
      tauCand->SetIsMuonVeto(true);
    
    taus->AddLast(tauCand);

  }

}
