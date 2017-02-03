#define DEBUG_ON 0
#include "ZeeDCalculator/ZeeDReadEtMissAOD.h"

// The basic units are :
//              millimeter              (millimeter)
//              Giga electron Volt      (GeV)
//              radian                  (radian)
//              positron charge         (eplus)


// std libraries
#include <string>

// Root libraries
#include <TSystem.h>
#include <TObjArray.h>

// Athena libraries
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"
#include "StoreGate/StoreGateSvc.h"
#include "MissingETEvent/MissingET.h"
#include "MissingETEvent/MissingETComposition.h"

#include "egammaEvent/ElectronContainer.h"
#include "egammaEvent/egammaContainer.h"


#include "egammaEvent/Electron.h"
#include "egammaEvent/Photon.h"
#include "tauEvent/TauJet.h"
#include "muonEvent/Muon.h"
#include "JetEvent/Jet.h"

// ZeeD analysis includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDEtMiss.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDPhoton.h"
#include "ZeeDEvent/ZeeDJet.h"
#include "ZeeDEvent/ZeeDTau.h"
#include "ZeeDEvent/ZeeDMuon.h"
#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDReadEtMissAOD::ZeeDReadEtMissAOD() :
    fMissingEtObj(NULL),
    fMissingEtSoftTermObj(NULL),
    fMissingEtCompositionObj(NULL),
    fJetCollection(NULL),
    fElecContainer(NULL),
    fPhotonContainer(NULL),
    fMuonContainer(NULL),
    fTauContainer(NULL),
    fSvcHelper("ZeeDReadEtMissAOD"),
    fAnaOptions(fSvcHelper.GetAnaSvc()),
    fStoreGate(fSvcHelper.GetStoreGate())
{
    // Constructor
}

//------------------------------------------------------
ZeeDReadEtMissAOD::~ZeeDReadEtMissAOD()
{
    // Destructor
}

//------------------------------------------------------
void ZeeDReadEtMissAOD::Initialize()
{
    // Initializer
    DEBUG_MES_START;

    StatusCode sc(StatusCode::SUCCESS);

    // Get Store Gate

    // Get missing Et object
    std::string containerName("");

    containerName = "MET_RefFinal";
    sc = (*fStoreGate)->retrieve(fMissingEtObj, containerName);
    if (sc.isFailure() || !fMissingEtObj) {
      std::string message("No " + containerName + " object found!");
      Error("ZeeDReadEtMissAOD::Initialize", message.c_str() );
      gSystem->Exit(EXIT_FAILURE);
    }

    containerName = "MET_CellOut_Eflow";
    sc = (*fStoreGate)->retrieve(fMissingEtSoftTermObj, containerName);
    if (sc.isFailure() || !fMissingEtObj) {
      std::string message("No " + containerName + " object found!");
      Error("ZeeDReadEtMissAOD::Initialize", message.c_str() );
      gSystem->Exit(EXIT_FAILURE);
    }
    
    containerName =  "MET_RefComposition";
    sc = (*fStoreGate)->retrieve(fMissingEtCompositionObj, containerName);
    if (sc.isFailure() || !fMissingEtCompositionObj) {
      std::string message("No " + containerName + " object found!");
      Error("ZeeDReadEtMissAOD::Initialize", message.c_str() );
      gSystem->Exit(EXIT_FAILURE);
    }

    containerName =  "AntiKt4LCTopoJets";
    sc = (*fStoreGate)->retrieve( fJetCollection, containerName) ;
    if (sc.isFailure() || !fJetCollection) {
      std::string message("No " + containerName + " object found!");
      Error("ZeeDReadEtMissAOD::Initialize", message.c_str() );
      gSystem->Exit(EXIT_FAILURE);
    }

    containerName =  (*fAnaOptions)->ElectronContainer();
    sc = (*fStoreGate)->retrieve( fElecContainer, containerName );
    if (sc.isFailure() || !fElecContainer) {
      std::string message("No " + containerName + " object found!");
      Error("ZeeDReadEtMissAOD::Initialize", message.c_str() );
      gSystem->Exit(EXIT_FAILURE);
    }

    containerName =  "PhotonAODCollection";
    sc = (*fStoreGate)->retrieve( fPhotonContainer, containerName );
    if (sc.isFailure() || !fPhotonContainer) {
      std::string message("No " + containerName + " object found!");
      Error("ZeeDReadEtMissAOD::Initialize", message.c_str() );
      gSystem->Exit(EXIT_FAILURE);
    }
    
    containerName = "Muons";
    sc = (*fStoreGate)->retrieve(fMuonContainer,containerName );
    if ( sc != StatusCode::SUCCESS || !fMuonContainer) {
      std::string message("No " + containerName + " object found!");
      Error("ZeeDReadEtMissAOD::Initialize", message.c_str() );
      gSystem->Exit(EXIT_FAILURE);
    }

    containerName = "TauRecContainer";
    sc = (*fStoreGate)->retrieve(fTauContainer,containerName );
    if ( sc != StatusCode::SUCCESS || !fTauContainer) {
      std::string message("No " + containerName + " object found!");
      Error("ZeeDReadEtMissAOD::Initialize", message.c_str() );
      gSystem->Exit(EXIT_FAILURE);
    }


    DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDReadEtMissAOD::ReadInfo(ZeeDEvent* event)
{
    // Read missing et for given event
    DEBUG_MES_START;

    this->Initialize();

    TObjArray* etMissArray = event->GetEtMissArray();
    etMissArray->Clear();

    // Create new missing et object
    ZeeDEtMiss* etMiss = new ZeeDEtMiss();

    this->ReadEtMiss(etMiss);//, event);

    // Add this et miss object to the array
    etMissArray->AddLast(etMiss);

    DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDReadEtMissAOD::ReadEtMiss(ZeeDEtMiss* etMiss)//, ZeeDEvent * event)
{
   // Reads Et miss
    DEBUG_MES_START;

    Double_t et  = fMissingEtObj->et()  / GeV;
    Double_t etx = fMissingEtObj->etx() / GeV;
    Double_t ety = fMissingEtObj->ety() / GeV;
    Double_t phi = fMissingEtObj->phi() / radian;
    //
    // Set Et miss quantities
    //
    etMiss -> SetEt(et);
    etMiss -> SetEtX(etx);
    etMiss -> SetEtY(ety);
    etMiss -> SetPhi(phi);

    // -----------------------------------

    JetCollection::const_iterator jetItrB  = fJetCollection->begin();    
    JetCollection::const_iterator jetItrE  = fJetCollection->end();

    MissingETComposition::const_iterator<Jet> fJet( fMissingEtCompositionObj->beginIterator<Jet>() );
    MissingETComposition::const_iterator<Jet> lJet( fMissingEtCompositionObj->endIterator<Jet>() );
    
    
    for( MissingETComposition::const_iterator<Jet>  iE = fJet; iE != lJet; iE++)
      {
	JetCollection::const_iterator iEJet = std::find(jetItrB, jetItrE, *iE );
	Int_t _index_( std::distance(jetItrB, iEJet) );

	etMiss->GetMETCompositionJet_wet().push_back(iE.getParameter().wet());
	etMiss->GetMETCompositionJet_wpx().push_back(iE.getParameter().wpx());
	etMiss->GetMETCompositionJet_wpy().push_back(iE.getParameter().wpy());
	etMiss->GetMETCompositionJet_statusWord().push_back(iE.getParameter().statusWord());
	etMiss->GetMETCompositionJet_index().push_back( _index_ );
      
// 	std::cout <<"\t\t MET JET\t E = "<< (*iE)->e() 
// 		  <<" pt = "<< (*iE)->pt() 
// 		  <<" eta = "<<(*iE)->eta()<<" phi = "<<(*iE)->phi()   
// 		  <<" wet "<< iE.getParameter().wet()
// 		  <<" wpx "<< iE.getParameter().wpx()
// 		  <<" wpy "<< iE.getParameter().wpy()
// 		  <<" status "<< iE.getParameter().statusWord()
// 		  <<" index = "<<  _index_
// 		  << std::endl;
//      }
//
//     TString jetCollName("AntiKt4LCTopoJets");
//     if ( event->HasJets( jetCollName ) )
//       {
// 	const TObjArray* jets = event->GetJets( jetCollName, kFALSE );
// 	for (int ii=0;ii<jets->GetEntries();ii++)
// 	  {
// 	    ZeeDJet* jet =dynamic_cast<ZeeDJet*>( jets->At(ii) ) ;
// 	    std::cout <<"\t\t ZEED JET \t E = "<<jet->GetFourVector().E()
// 		      <<" Pt = " <<jet->GetFourVector().Pt()
// 		      <<" eta = "<<jet->GetFourVector().Eta()<<" Phi = "<<jet->GetFourVector().Phi()
// 		      << "index = " << jet->GetCollectionIndex()
// 		      << std::endl;
 	  }
	
//       }
//     else
//       Info(__PRETTY_FUNCTION__, TString::Format(" Can't find %s collection", jetCollName.Data()));
    
    // -----------------------------------

    ElectronContainer::const_iterator elecItrB  = fElecContainer->begin();    
    ElectronContainer::const_iterator elecItrE  = fElecContainer->end();

    MissingETComposition::const_iterator<Analysis::Electron> fElectron( fMissingEtCompositionObj->beginIterator<Analysis::Electron>());
    MissingETComposition::const_iterator<Analysis::Electron> lElectron( fMissingEtCompositionObj->endIterator<Analysis::Electron>());
    
    for( MissingETComposition::const_iterator<Analysis::Electron>  iE = fElectron; iE != lElectron; iE++)
      {
	ElectronContainer::const_iterator iEElec = std::find(elecItrB, elecItrE, *iE );
	Int_t _index_( std::distance(elecItrB, iEElec) );

	etMiss->GetMETCompositionElectron_wet().push_back(iE.getParameter().wet());
	etMiss->GetMETCompositionElectron_wpx().push_back(iE.getParameter().wpx());
	etMiss->GetMETCompositionElectron_wpy().push_back(iE.getParameter().wpy());
	etMiss->GetMETCompositionElectron_statusWord().push_back(iE.getParameter().statusWord());
	etMiss->GetMETCompositionElectron_index().push_back( _index_ );
      
// 	std::cout <<"\t\t MET ELE\t E = "<< (*iE)->e() 
// 		  <<" pt = "<< (*iE)->pt() 
// 		  <<" eta = "<<(*iE)->eta()<<" phi = "<<(*iE)->phi()   
// 		  <<" wet "<< iE.getParameter().wet()
// 		  <<" wpx "<< iE.getParameter().wpx()
// 		  <<" wpy "<< iE.getParameter().wpy()
// 		  <<" status "<< iE.getParameter().statusWord()
// 		  <<" index = "<<  _index_
// 		  << std::endl;
//       }
//    
//     const TObjArray* electrons = event->GetElectrons(  );
//     for (int ii=0;ii<electrons->GetEntries();ii++)
//       {
// 	ZeeDElectron* electron =dynamic_cast<ZeeDElectron*>( electrons->At(ii) ) ;
// 	std::cout <<"\t\t ZEED ELE \t E = "<<electron->GetCombinedFourVector().E()
// 		  <<" Pt = " <<electron->GetCombinedFourVector().Pt()
// 		  <<" eta = "<<electron->GetCombinedFourVector().Eta()
// 		  <<" Phi = "<<electron->GetCombinedFourVector().Phi()
// 		  << "index = " << electron->GetCollectionIndex().Get()
// 		  << std::endl;
       }
    // -----------------------------------
    
    egammaContainer::const_iterator photonItrB  = fPhotonContainer->begin();    
    egammaContainer::const_iterator photonItrE  = fPhotonContainer->end();

    MissingETComposition::const_iterator<Analysis::Photon> fPhoton(fMissingEtCompositionObj->beginIterator<Analysis::Photon>());
    MissingETComposition::const_iterator<Analysis::Photon> lPhoton(fMissingEtCompositionObj->endIterator<Analysis::Photon>());
    
    for( MissingETComposition::const_iterator<Analysis::Photon>  iE = fPhoton; iE != lPhoton; iE++)
      {
	egammaContainer::const_iterator iEPhoton = std::find(photonItrB, photonItrE, *iE );
	Int_t _index_( std::distance(photonItrB, iEPhoton) );

	etMiss->GetMETCompositionPhoton_wet().push_back(iE.getParameter().wet());
	etMiss->GetMETCompositionPhoton_wpx().push_back(iE.getParameter().wpx());
	etMiss->GetMETCompositionPhoton_wpy().push_back(iE.getParameter().wpy());
	etMiss->GetMETCompositionPhoton_statusWord().push_back(iE.getParameter().statusWord());
	etMiss->GetMETCompositionPhoton_index().push_back( _index_ );
      
// 	std::cout <<"\t\t MET PHO\t E = "<< (*iE)->e() 
// 		  <<" pt = "<< (*iE)->pt() 
// 		  <<" eta = "<<(*iE)->eta()<<" phi = "<<(*iE)->phi()   
// 		  <<" wet "<< iE.getParameter().wet()
// 		  <<" wpx "<< iE.getParameter().wpx()
// 		  <<" wpy "<< iE.getParameter().wpy()
// 		  <<" status "<< iE.getParameter().statusWord()
// 		  <<" index = "<<  _index_
// 		  << std::endl;
//       }

//     const TObjArray* photons = event->GetPhotons(  );
//     for (int ii=0;ii<photons->GetEntries();ii++)
//       {
// 	ZeeDPhoton* photon =dynamic_cast<ZeeDPhoton*>( photons->At(ii) ) ;
// 	std::cout <<"\t\t ZEED PHO \t E = "<<photon->GetFourVector().E()
// 		  <<" Pt = " <<photon->GetFourVector().Pt()
// 		  <<" eta = "<<photon->GetFourVector().Eta()
// 		  <<" Phi = "<<photon->GetFourVector().Phi()
// 		  << "index = " << photon->GetCollectionIndex()
// 		  << std::endl;
      }
    // -----------------------------------
    Analysis::TauJetContainer::const_iterator tauItrB  = fTauContainer->begin();    
    Analysis::TauJetContainer::const_iterator tauItrE  = fTauContainer->end();

    MissingETComposition::const_iterator<Analysis::TauJet> fTau(fMissingEtCompositionObj->beginIterator<Analysis::TauJet>());
    MissingETComposition::const_iterator<Analysis::TauJet> lTau(fMissingEtCompositionObj->endIterator<Analysis::TauJet>());
    
    for( MissingETComposition::const_iterator<Analysis::TauJet>  iE = fTau; iE != lTau; iE++)
      {
	Analysis::TauJetContainer::const_iterator iETau = std::find(tauItrB, tauItrE, *iE );
	Int_t _index_( std::distance(tauItrB, iETau) );

	etMiss->GetMETCompositionTau_wet().push_back(iE.getParameter().wet());
	etMiss->GetMETCompositionTau_wpx().push_back(iE.getParameter().wpx());
	etMiss->GetMETCompositionTau_wpy().push_back(iE.getParameter().wpy());
	etMiss->GetMETCompositionTau_statusWord().push_back(iE.getParameter().statusWord());
	etMiss->GetMETCompositionTau_index().push_back( _index_ );
      
// 	std::cout <<"\t\t MET TAU\t E = "<< (*iE)->e() 
// 		  <<" pt = "<< (*iE)->pt() 
// 		  <<" eta = "<<(*iE)->eta()<<" phi = "<<(*iE)->phi()   
// 		  <<" wet "<< iE.getParameter().wet()
// 		  <<" wpx "<< iE.getParameter().wpx()
// 		  <<" wpy "<< iE.getParameter().wpy()
// 		  <<" status "<< iE.getParameter().statusWord()
// 		  <<" index = "<<  _index_
// 		  << std::endl;
//       }
//     const TObjArray* taus = event->GetTaus(  );
//     for (int ii=0;ii<taus->GetEntries();ii++)
//       {
// 	ZeeDTau* tau =dynamic_cast<ZeeDTau*>( taus->At(ii) ) ;
// 	std::cout <<"\t\t ZEED TAU \t E = "<<tau->GetFourVector().E()
// 		  <<" Pt = " <<tau->GetFourVector().Pt()
// 		  <<" eta = "<<tau->GetFourVector().Eta()
// 		  <<" Phi = "<<tau->GetFourVector().Phi()
// 		  << "index = " << tau->GetCollectionIndex()
// 		  << std::endl;
      }

    // -----------------------------------

    Analysis::MuonContainer::const_iterator muonItrB  = fMuonContainer->begin();    
    Analysis::MuonContainer::const_iterator muonItrE  = fMuonContainer->end();

    MissingETComposition::const_iterator<Analysis::Muon> fMuon(fMissingEtCompositionObj->beginIterator<Analysis::Muon>());
    MissingETComposition::const_iterator<Analysis::Muon> lMuon(fMissingEtCompositionObj->endIterator<Analysis::Muon>());
    
    for( MissingETComposition::const_iterator<Analysis::Muon>  iE = fMuon; iE != lMuon; iE++)
      {
	Analysis::MuonContainer::const_iterator iEMuon = std::find(muonItrB, muonItrE, *iE );
	Int_t _index_( std::distance( muonItrB, iEMuon) );

	etMiss->GetMETCompositionMuon_wet().push_back(iE.getParameter().wet());
	etMiss->GetMETCompositionMuon_wpx().push_back(iE.getParameter().wpx());
	etMiss->GetMETCompositionMuon_wpy().push_back(iE.getParameter().wpy());
	etMiss->GetMETCompositionMuon_statusWord().push_back(iE.getParameter().statusWord());
	etMiss->GetMETCompositionMuon_index().push_back( _index_ );
      
// 	std::cout <<"\t\t MET MUON\t E = "<< (*iE)->e() 
// 		  <<" pt = "<< (*iE)->pt() 
// 		  <<" eta = "<<(*iE)->eta()<<" phi = "<<(*iE)->phi()   
// 		  <<" wet "<< iE.getParameter().wet()
// 		  <<" wpx "<< iE.getParameter().wpx()
// 		  <<" wpy "<< iE.getParameter().wpy()
// 		  <<" status "<< iE.getParameter().statusWord()
// 		  <<" index = "<<  _index_
// 		  << std::endl;
//       }
//     const TObjArray* muons = event->GetMuons(  );
//     for (int ii=0;ii<muons->GetEntries();ii++)
//       {
// 	ZeeDMuon* muon =dynamic_cast<ZeeDMuon*>( muons->At(ii) ) ;
// 	std::cout <<"\t\t ZEED MUON \t E = "<<muon->GetFourVector().E()
// 		  <<" Pt = " <<muon->GetFourVector().Pt()
// 		  <<" eta = "<<muon->GetFourVector().Eta()
// 		  <<" Phi = "<<muon->GetFourVector().Phi()
// 		  << "index = " << muon->GetCollectionIndex()
// 		  << std::endl;
      }
    // -----------------------------------

    if (fMissingEtSoftTermObj != NULL )
      {
	etMiss->SetMET_SoftTermETX  ( fMissingEtSoftTermObj->etx()/GeV   );
	etMiss->SetMET_SoftTermETY  ( fMissingEtSoftTermObj->ety()/GeV   );
	etMiss->SetMET_SoftTermETSUM( fMissingEtSoftTermObj->sumet()/GeV );
	
// 	std::cout <<"\t\t MET SOFTTERM\t ETX = "<<etMiss->GetMET_SoftTermETX()
// 		  <<" ETY = "<<etMiss->GetMET_SoftTermETY()
// 		  <<" ETSUM = "<<etMiss->GetMET_SoftTermETSUM()
// 		  <<std::endl;
      }
    else
      {
	std::cout <<" SOFT TERM is ZERO"<<std::endl;
	
      }

    
//     std::cout <<" --------------------------------------------------------------------------------- "<<std::endl;
//     std::cout <<" --------------------------------------------------------------------------------- "<<std::endl;
//     std::cout <<" --------------------------------------------------------------------------------- "<<std::endl;

    // -----------------------------------

    DEBUG_MES_END;
}
