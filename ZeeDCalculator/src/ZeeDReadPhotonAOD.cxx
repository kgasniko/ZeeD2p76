#include "ZeeDCalculator/ZeeDReadPhotonAOD.h"

#include "egammaEvent/egammaContainer.h"
#include "egammaEvent/PhotonContainer.h"
#include "egammaEvent/Photon.h"

#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDPhoton.h"

#include "GaudiKernel/ServiceHandle.h"
#include "StoreGate/StoreGateSvc.h"

#include <TLorentzVector.h>

ZeeDReadPhotonAOD::ZeeDReadPhotonAOD() : 
  fSvcHelper("ZeeDReadPhotonAOD"),
  fStoreGate(fSvcHelper.GetStoreGate()),
  fAnaOptions(fSvcHelper.GetAnaSvc()) ,
  fPhotonContainer(NULL)
{
}

ZeeDReadPhotonAOD::~ZeeDReadPhotonAOD() {

}


void ZeeDReadPhotonAOD::Initialize()
{

  StatusCode sc( StatusCode::SUCCESS);


  sc = (*fStoreGate)->retrieve(fPhotonContainer, "PhotonAODCollection");
  if ( sc.isFailure() || !fPhotonContainer) 
    {
      Error("ZeeDReadPhotonAOD::ReadInfo", "No PhotonAODCollection container found");
      gSystem->Exit(EXIT_FAILURE);
    }
  
}


void ZeeDReadPhotonAOD::ReadInfo(ZeeDEvent* event)
{
  //Retrieve PhotonCollection
  this->Initialize();


  TObjArray* photons = event->GetPhotons();

  photons->Clear();

  // Iterators over the photon container
  egammaContainer::const_iterator photonItrB  = fPhotonContainer -> begin();
  egammaContainer::const_iterator photonItrE = fPhotonContainer -> end();

  DEBUG_VAR(photonContainer -> size() * event -> GetGenWeight());

  // Loop over the AOD photon container
  for (  egammaContainer::const_iterator photonItr  = fPhotonContainer -> begin(); photonItr != photonItrE; ++photonItr) {

    TLorentzVector photonFourVector( (*photonItr)->get4Mom().px()/GeV, (*photonItr)->get4Mom().py()/GeV, (*photonItr)->get4Mom().pz()/GeV, (*photonItr)->get4Mom().e()/GeV );
    
    // Create new photon candidate
    ZeeDPhoton* photonCand = new ZeeDPhoton();
      
//     std::cout << " READ AOD PHOTON "
// 	      << " E = "<<(*photonItr)->e()
// 	      << " PT = "<<(*photonItr)->pt()
// 	      << " Eta = "<<(*photonItr)->eta()
// 	      << " phi = "<<(*photonItr)->phi()
// 	      << " P = "<<(*photonItr)->p()
// 	      << std::endl; 

    photonCand -> GetCollectionIndex() = std::distance( photonItrB, photonItr );
    photonCand -> SetFourVector( photonFourVector );
    
    photons->AddLast(photonCand);

  }

}
