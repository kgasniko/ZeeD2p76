#define DEBUG_ON 0

#include "ZeeDCalculator/ZeeDReadJetsAOD.h"

// std libraries
#include <iostream>
#include <string>

// Root libraries
#include <TLorentzVector.h>
#include <TSystem.h>
#include <TObjArray.h>

// Athena libraries
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"

#include "McParticleEvent/TruthParticleContainer.h"
#include "StoreGate/StoreGateSvc.h"

#include "JetEvent/Jet.h"
#include "JetUtils/JetCaloHelper.h"
#include "JetTagInfo/TruthInfo.h"
#include "JetEvent/JetCollection.h"
#include "JetTagInfo/BaseTagInfo.h"

#include "JetEvent/JetConstituentIterator.h"

// ZeeD analysis includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDEvent/ZeeDJet.h"
#include "ZeeDEvent/ZeeDVertex.h"
#include "JetSelectorTools/AthJetCleaningTool.h"

#include "ZeeDEvent/ZeeDGenParticle.h"


using std::cout;
using std::endl;
//------------------------------------------------------
ZeeDReadJetsAOD::ZeeDReadJetsAOD() :
    fIsMC(kFALSE),
    fJetCollection(NULL),
    fJetCollName("AntiKt4TopoJets"),
    fSvcHelper("ZeeDReadJetsAOD"),
    fStoreGate(fSvcHelper.GetStoreGate()),
    fAnaOptions(fSvcHelper.GetAnaSvc()),
    fMcEvent(0)

{

    this->Initialize();

}

//------------------------------------------------------
ZeeDReadJetsAOD::~ZeeDReadJetsAOD()
{
    // Destructor
      toBeDeleted.Delete();
}

//------------------------------------------------------
void ZeeDReadJetsAOD::Initialize()
{
  StatusCode sc = StatusCode::SUCCESS;

  if ( sc.isFailure() ) {
    Error("ZeeDReadJetsAOD::Initialize",
	  "Unable to open ZeeDAnalysisSvc");
    gSystem->Exit(EXIT_FAILURE);
  }

  fIsMC = (*fAnaOptions)->IsMC();

  toBeDeleted.SetOwner();

  genParticle  = new TObjArray;
  genParticle->SetOwner();
  toBeDeleted.Add(genParticle);

   
}
//---------------------------------------------
void  ZeeDReadJetsAOD::bookCuts()
{

  jetCone = 1.0;

  jetConeMap.insert( std::pair<std::string,Double_t>("4" , 0.4) ) ;
  jetConeMap.insert( std::pair<std::string,Double_t>("5" , 0.5) ) ;
  jetConeMap.insert( std::pair<std::string,Double_t>("6" , 0.6) ) ;

  return;
}


//------------------------------------------------------
void ZeeDReadJetsAOD::ReadInfo(ZeeDEvent* event)
{

  bookCuts();
  StatusCode sc = StatusCode::SUCCESS;

  //filling array of truth particle for qg tag calculation
  if  (fIsMC){
    sc = (*fStoreGate)->retrieve(fMcEvent, "GEN_AOD");

    if( sc.isFailure()  || !fMcEvent ) {
      Error("ZeeDCalcJets::Initalize",
	    "No GEN_AOD Truth MC Collection found");
      gSystem->Exit(EXIT_FAILURE);
    }

    const HepMC::GenEvent* genEvt = fMcEvent->front();
    if (!genEvt) return;

    //loop over the particles in the event
    for (HepMC::GenEvent::particle_const_iterator p = genEvt->particles_begin(); p != genEvt->particles_end(); p++) {
      const HepMC::GenParticle* particle = *p;

      CHECK_NULL_PTR(particle);

      ZeeDGenParticle* gen = new ZeeDGenParticle();

      const HepMC::FourVector& fourVector = particle->momentum();
      const Double_t E  = fourVector.e()  / GeV;
      const Double_t Px = fourVector.px() / GeV;
      const Double_t Py = fourVector.py() / GeV;
      const Double_t Pz = fourVector.pz() / GeV;

      gen->SetMCFourVector( TLorentzVector(Px, Py, Pz, E) );
      gen->SetParticleID( particle->pdg_id() );
      //gen->SetParticleStatus( particle->status());

      // add gen particle to the event
      genParticle->AddLast(gen);
    }
  }

  std::vector<std::string> collections = (*fAnaOptions)->JetCollectionNames();
  ToolHandle<IAthSelectorTool> cleaningTool = (*fAnaOptions)->JetCleaningTool();

  //   std::cout <<"test NPV = "<< std::endl;
  //   //const Int_t ttt = event->Nvtx();
  //   std::cout <<"\t\t nv "<<event->Nvtx().IsSet()
  // 	    <<"\t\t en "<<event->EventNumber().IsSet()
  // 	    <<"\t\t rn "<<event->RunNumber().IsSet()
  // 	    <<"\t\t lb "<<event->LB().IsSet()
  // 	    <<"\t\t mu1 "<<event->ActualInteractionsPerCrossing().IsSet()
  // 	    <<"\t\t mu2 "<<event->AverageInteractionsPerCrossing().IsSet()
  // 	    << std::endl;

  //   std::cout <<"\t\t nv "<<nVtx<<std::endl;
  //   std::cout <<"\t\t nv "<<event->AverageInteractionsPerCrossing().Get()<<std::endl;


  for (unsigned int i = 0; i < collections.size(); i++){
    
    fJetCollName = collections[ i ];

    for (std::map<std::string,Double_t>::iterator itr = jetConeMap.begin() ; itr != jetConeMap.end(); ++itr)
      if (fJetCollName.find(itr->first) != std::string::npos) 
	{
	  jetCone = itr->second;
	  break;
	}

    //Retrieve the jet Container
    this->RetrieveJetContainer();

    TObjArray* JetArray = event->GetJets(collections[i], kFALSE);
    if(!JetArray) {
      JetArray = new TObjArray();
      JetArray->SetOwner(kTRUE);
      JetArray->SetName((collections[i]).c_str());
      event->fJetCollectionArray->Add(JetArray);
    }
    JetArray->Clear();
    
    // std::cout <<"Read : coll "<<collections[i]<<" size = "<< fJetCollection->size() << std::endl;
    
    JetCollection::const_iterator jetItrB  = fJetCollection->begin();
    JetCollection::const_iterator jetItrE = fJetCollection->end();
   
    // Loop over all jets in the collection
    for ( JetCollection::const_iterator jetItr(jetItrB) ; jetItr != jetItrE; ++jetItr ) {

      const Jet* jet = *jetItr;
      if (jet == NULL)    continue;
      
      // P.S. > This is not needed anynore. We store cleaning flag as an extra jet moment
      //      if ( !(cleaningTool->accept( jet )) && !(fJetCollName.find("Truth")!=std::string::npos) )   continue;      
      //

      /** Should be removed ?
       * if((*jetItr)->particleBase().origin()) {
       cout << "hey I have found particle base" << endl;
       }
      */
//       std::cout <<"Read : coll "<<collections[i]<<" size = "<< fJetCollection->size() << std::endl;
//       std::vector<std::string>  keys = jet->getMomentKeys();
//       for (int i =0 ; i< keys.size(); i++)
//       	{
//       	  std::cout <<"N = "<<i<< "name = " <<keys[i]<< std::endl; 
//       	}
//       std::cout <<" \n done"<< std::endl;
      
//      // ignore jets with zero energy  P.S. I am not sure if this is needed at this stage
//       if(jetE == 0) {
// 	continue;
//       }

      
      Double_t jetE  = jet->getRawE()  / GeV;
      Double_t jetPx = jet->getRawPx() / GeV;
      Double_t jetPy = jet->getRawPy() / GeV;
      Double_t jetPz = jet->getRawPz() / GeV;
      TLorentzVector jetFourVectorRaw(jetPx, jetPy, jetPz, jetE);
      jetE  = jet->getCScaleE()  / GeV;
      jetPx = jet->getCScalePx() / GeV;
      jetPy = jet->getCScalePy() / GeV;
      jetPz = jet->getCScalePz() / GeV;
      TLorentzVector jetFourVectorCS(jetPx, jetPy, jetPz, jetE);
      jetE  = jet->getCalE()  / GeV;
      jetPx = jet->getCalPx() / GeV;
      jetPy = jet->getCalPy() / GeV;
      jetPz = jet->getCalPz() / GeV;
      TLorentzVector jetFourVectorCal(jetPx, jetPy, jetPz, jetE);

      Double_t EtaOrigin =  jet->getMoment("EtaOrigin", true) ;
      Double_t PhiOrigin =  jet->getMoment("PhiOrigin", true) ;
      Double_t MOrigin   =  jet->getMoment("MOrigin", true) / GeV ;

      // Create new jet candidate
      ZeeDJet* jetCand = new ZeeDJet();

      jetCand -> GetCollectionIndex() =  std::distance( jetItrB, jetItr );

      if (jetFourVectorRaw.Pt() < 1e-3 )jetFourVectorRaw.SetPtEtaPhiE(0.0001,0,0,.001);
      if ( jetFourVectorCS.Pt() < 1e-3 ) jetFourVectorCS.SetPtEtaPhiE(0.0001,0,0,.001);
      if (jetFourVectorCal.Pt() < 1e-3 )jetFourVectorCal.SetPtEtaPhiE(0.0001,0,0,.001);

      //SetFourVector
      jetCand -> SetFourVector( jetFourVectorCal );
      jetCand -> SetFourVectorRaw( jetFourVectorRaw );
      jetCand -> SetFourVectorCS( jetFourVectorCS );

      jetCand -> SetEtaOrigin ( EtaOrigin );
      jetCand -> SetPhiOrigin ( PhiOrigin );
      jetCand -> SetMOrigin   ( MOrigin );
	
      //SetEmFraction
      jetCand -> SetEmFraction( JetCaloHelper::jetEMFraction(jet) );
	
      //SetEnergyFractions
      jetCand ->  SetEnergy_PresamplerE( jet->getMoment("energy_PreSamplerE", true) );
      jetCand ->  SetEnergy_PresamplerB( jet->getMoment("energy_PreSamplerB", true) );
      jetCand ->  SetEnergy_EME1( jet->getMoment("energy_EME1", true) );
      jetCand ->  SetEnergy_EME2( jet->getMoment("energy_EME2", true) );
      jetCand ->  SetEnergy_EME3( jet->getMoment("energy_EME3", true) );
      jetCand ->  SetEnergy_EMB1( jet->getMoment("energy_EMB1", true) );
      jetCand ->  SetEnergy_EMB2( jet->getMoment("energy_EMB2", true) );
      jetCand ->  SetEnergy_EMB3( jet->getMoment("energy_EMB3", true) );
      jetCand ->  SetEnergy_HEC0( jet->getMoment("energy_HEC0", true) );
      jetCand ->  SetEnergy_HEC1( jet->getMoment("energy_HEC1", true) );
      jetCand ->  SetEnergy_HEC2( jet->getMoment("energy_HEC2", true) );
      jetCand ->  SetEnergy_HEC3( jet->getMoment("energy_HEC3", true) );
      jetCand ->  SetEnergy_FCAL0( jet->getMoment("energy_FCAL0", true) );
      jetCand ->  SetEnergy_FCAL1( jet->getMoment("energy_FCAL1", true) );
      jetCand ->  SetEnergy_FCAL2( jet->getMoment("energy_FCAL2", true) );
      jetCand ->  SetEnergy_TileBar0( jet->getMoment("energy_TileBar0", true) );
      jetCand ->  SetEnergy_TileBar1( jet->getMoment("energy_TileBar1", true) );
      jetCand ->  SetEnergy_TileBar2( jet->getMoment("energy_TileBar2", true) );
      jetCand ->  SetEnergy_TileExt0( jet->getMoment("energy_TileExt0", true) );
      jetCand ->  SetEnergy_TileExt1( jet->getMoment("energy_TileExt1", true) );
      jetCand ->  SetEnergy_TileExt2( jet->getMoment("energy_TileExt2", true) );
      jetCand ->  SetEnergy_TileGap1( jet->getMoment("energy_TileGap1", true) );
      jetCand ->  SetEnergy_TileGap2( jet->getMoment("energy_TileGap2", true) );
      jetCand ->  SetEnergy_TileGap3( jet->getMoment("energy_TileGap3", true) );

      jetCand ->  SetIsBadLooseMinus( jet->getMoment("isBadLooseMinus", true) );
      jetCand ->  SetIsBadLoose     ( jet->getMoment("isBadLoose", true) );
      jetCand ->  SetIsBadMedium    ( jet->getMoment("isBadMedium", true) );
      jetCand ->  SetIsBadTight     ( jet->getMoment("isBadTight", true) );

      jetCand -> SetJVF        ( jet->getMoment("JVF", true)         );
      jetCand -> SetSumPtTrk   ( jet->getMoment("sumPtTrk", true)    );
      jetCand -> SetOriginIndex( jet->getMoment("OriginIndex", true) );
      jetCand -> SetNTrk       ( jet->getMoment("nTrk", true)        );	
      jetCand -> SetWIDTH        ( jet->getMoment("WIDTH", true)       );
      jetCand -> SetNumTowers    ( jet->getMoment("NumTowers", true)   );

      jetCand -> SetEtaOriginEM        ( jet->getMoment("EtaOriginEM", true)        );
      jetCand -> SetPhiOriginEM        ( jet->getMoment("PhiOriginEM", true)        );
      jetCand -> SetMOriginEM          ( jet->getMoment("MOriginEM", true)        );

      jetCand -> SetWidthFraction      ( jet->getMoment("WidthFraction", true)        );
      jetCand -> SetGSCFactorF         ( jet->getMoment("GSCFactorF", true)        );
      jetCand -> SetEMJESnooffset      ( jet->getMoment("EMJESnooffset", true)        );
      jetCand -> SetCONST              ( jet->getMoment("CONST", true)        );

      jetCand -> SetActiveArea         ( jet->getMoment("ActiveArea", true)        );
      jetCand -> SetActiveAreaPx       ( jet->getMoment("ActiveAreaPx", true)        );
      jetCand -> SetActiveAreaPy       ( jet->getMoment("ActiveAreaPy", true)        );
      jetCand -> SetActiveAreaPz       ( jet->getMoment("ActiveAreaPz", true)        );
      jetCand -> SetActiveAreaE        ( jet->getMoment("ActiveAreaE", true)        );

      jetCand -> SetLowEtConstituentsFrac       ( jet->getMoment("LowEtConstituentsFrac", true)        );

      jetCand -> SetnTrk_pv0_500MeV             ( jet->getMoment("nTrk_pv0_500MeV", true)        );
      jetCand -> SetsumPtTrk_pv0_500MeV         ( jet->getMoment("sumPtTrk_pv0_500MeV", true)        );
      jetCand -> SetnTrk_pv0_1GeV               ( jet->getMoment("nTrk_pv0_1GeV", true)        );
      jetCand -> SetsumPtTrk_pv0_1GeV           ( jet->getMoment("sumPtTrk_pv0_1GeV", true)        );
      jetCand -> SettrackWIDTH_pv0_1GeV         ( jet->getMoment("trackWIDTH_pv0_1GeV", true)        );
      jetCand -> SetnTrk_allpv_1GeV             ( jet->getMoment("nTrk_allpv_1GeV", true)        );
      jetCand -> SetsumPtTrk_allpv_1GeV         ( jet->getMoment("sumPtTrk_allpv_1GeV", true)        );
      jetCand -> SettrackWIDTH_allpv_1GeV       ( jet->getMoment("trackWIDTH_allpv_1GeV", true)        );

      jetCand -> Setpt_truth                    ( jet->getMoment("pt_truth", true)        );
      jetCand -> SetLikeLihood_0                ( jet->getMoment("LikeLihood_0", true)        );
      
      jetCand -> SetCentroid_r                  ( jet->getMoment("Centroid_r",true)   ); 
      jetCand -> SetKtDr                        ( jet->getMoment("KtDr",true)   ); 

      jetCand -> SetIsoKR20Perp                 ( jet->getMoment("IsoKR20Perp",true)   ); 
      jetCand -> SetIsoKR20Par                  ( jet->getMoment("IsoKR20Par",true)   ); 
      jetCand -> SetIsoKR20SumPt                ( jet->getMoment("IsoKR20SumPt",true)   ); 
      jetCand -> SetIsoDelta2Perp               ( jet->getMoment("IsoDelta2Perp",true)   ); 
      jetCand -> SetIsoDelta2Par                ( jet->getMoment("IsoDelta2Par",true)   ); 
      jetCand -> SetIsoDelta2SumPt              ( jet->getMoment("IsoDelta2SumPt",true)   ); 
      jetCand -> SetIsoFixedCone8Perp           ( jet->getMoment("IsoFixedCone8Perp",true)   ); 
      jetCand -> SetIsoFixedCone8Par            ( jet->getMoment("IsoFixedCone8Par",true)   ); 
      jetCand -> SetIsoFixedCone8SumPt          ( jet->getMoment("IsoFixedCone8SumPt",true)   ); 
      jetCand -> SetIsoFixedArea13Perp          ( jet->getMoment("IsoFixedArea13Perp",true)   ); 
      jetCand -> SetIsoFixedArea13Par           ( jet->getMoment("IsoFixedArea13Par",true)   ); 
      jetCand -> SetIsoFixedArea13SumPt         ( jet->getMoment("IsoFixedArea13SumPt",true)   ); 
      jetCand -> SetIso6To88Perp                ( jet->getMoment("Iso6To88Perp",true)   ); 
      jetCand -> SetIso6To88Par                 ( jet->getMoment("Iso6To88Par",true)   ); 
      jetCand -> SetIso6To88SumPt               ( jet->getMoment("Iso6To88SumPt",true)   ); 

      jetCand -> SetBCH_CORR_CELL               ( jet->getMoment("BCH_CORR_CELL",true)   ); 
      jetCand -> SetBCH_CORR_DOTX               ( jet->getMoment("BCH_CORR_DOTX",true)   ); 
      jetCand -> SetBCH_CORR_JET                ( jet->getMoment("BCH_CORR_JET",true)   ); 

      jetCand -> SetTruthMFindex                ( jet->getMoment("TruthMFindex",true)   ); 
      jetCand -> SetTruthMF                     ( jet->getMoment("TruthMF",true)   ); 

      jetCand -> Setn90                         ( jet->getMoment("n90",true)  );
      
      jetCand -> SetootFracClusters5            ( jet->getMoment("ootFracClusters5",true)  );
      jetCand -> SetootFracClusters10           ( jet->getMoment("ootFracClusters10",true)  );	
      jetCand -> SetLArQuality                  ( jet->getMoment("LArQuality",true)  );
      jetCand -> SetHECQuality                  ( jet->getMoment("HECQuality",true)  );
      jetCand -> SetN_BAD_CELLS                 ( jet->getMoment("N_BAD_CELLS",true)  );
      jetCand -> SetENG_BAD_CELLS               ( jet->getMoment("ENG_BAD_CELLS",true)  );
	
      if (fIsMC){	  
	jetCand -> SetQGLabel  (  this -> CalculateQGLabel( jetFourVectorCal )  );
      } else {
	jetCand -> SetQGLabel  ( 0  );           
      }

      std::vector<TLorentzVector> constituents;
      // create an iterator on constituents at the signal state used during jet finding :
      JetConstituentIterator it   = JetConstituentIterator::first(jet);
      JetConstituentIterator itE = JetConstituentIterator::last(jet);
      for(; it!=itE;++it){
	TLorentzVector constFourVector((*it)->px()/ GeV, (*it)->py()/ GeV, (*it)->pz()/ GeV, (*it)->e()/ GeV);
        constituents.push_back(constFourVector);
      }
      jetCand -> SetJetConstituents( constituents );
      constituents.clear();

      JetArray->AddLast(jetCand);
    }
    //    JetArray->Sort();

    //    std::cout <<"Read : coll "<< JetArray->GetName() <<" size = "<<JetArray->GetEntries()<< std::endl;
  }

  genParticle->Clear();
  //Info(__PRETTY_FUNCTION__,"stop");
  return;

}
//-------------------------------------------------------------

int ZeeDReadJetsAOD::CalculateQGLabel( TLorentzVector jet_4momentum  )
{
  int Particle = 0;
  double Emax=0;
  double dR=99999;
  double E;
  int pdgId;
  double cone = jetCone;

  for (Int_t part = 0; part < genParticle->GetEntriesFast(); ++part){

    ZeeDGenParticle* par = static_cast< ZeeDGenParticle* >( genParticle->At(part) );
    pdgId = par->GetParticleID();
    if ((par->GetMCFourVector()).Pt()<0.001) continue;
    dR = 0;
    E = (par->GetMCFourVector()).E();

    if (E<Emax) continue;
    if( abs(pdgId) > 21 ) continue;
    if( abs(pdgId) == 0 ) continue;
    if( abs(pdgId) == 6 ) continue;
    if(( abs(pdgId) > 10 ) && ( abs(pdgId) < 17 )) continue;

    dR = jet_4momentum.DeltaR(par->GetMCFourVector());
    if (dR>cone) continue;
    Emax=E;
    Particle = pdgId; 
  }	
  return Particle;
}


//-------------------------------------------------------------
void ZeeDReadJetsAOD::RetrieveJetContainer()
{
  //Retrieve JetCollection
  StatusCode sc;
  sc = (*fStoreGate)->retrieve(fJetCollection, fJetCollName);

  if ( sc.isFailure() || !fJetCollection) 
    {
      std::string message = "No " + fJetCollName + " jet container found";
      Error("ZeeDReadJetsAOD::ReadInfo", message.c_str());
      gSystem->Exit(EXIT_FAILURE);
    }
}

//-----------------------------------------------------



