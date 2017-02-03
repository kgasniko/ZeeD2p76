#include "ZeeDCalculator/ZeeDReadElecAOD.h"

// The basic units are :
//              millimeter              (millimeter)
//              Giga electron Volt      (GeV)
//              radian                  (radian)
//              positron charge         (eplus)



// FIXME: ugly hack for the trigger matching
#define private public


// Root libraries
#include <TSystem.h>
#include <TVector3.h>
#include <TMath.h>
#include <TObjArray.h>

// Athena libraries

#include "CaloEvent/CaloClusterContainer.h"
#include "Particle/TrackParticleContainer.h"
#include "Particle/TrackParticle.h"
#include "ParticleTruth/TrackParticleTruthCollection.h"

#include "TrigObjectMatching/TrigMatchTool.h"
#include "TrigDecisionTool/TrigDecisionTool.h"

#include "GaudiKernel/ServiceHandle.h"
#include "StoreGate/StoreGateSvc.h"


#include "MCTruthClassifier/IMCTruthClassifier.h"
#include "MCTruthClassifier/MCTruthClassifierDefs.h"
#include "McParticleEvent/TruthParticleContainer.h"

#include "egammaEvent/egamma.h"
#include "egammaEvent/EMShower.h"
#include "egammaEvent/EMTrackMatch.h"
#include "egammaEvent/EMConvert.h"
#include "egammaEvent/ElectronContainer.h"
#include "egammaEvent/egammaContainer.h"

#include "EventInfo/EventInfo.h"
#include "EventInfo/EventID.h"

// ZeeD analysis includes
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDEvent.h"

// ZeeD
#include "ZeeDMisc/ZeeDMisc.h"

// GEO21 em calibration: raw cluster construction
#include "CaloUtils/CaloClusterStoreHelper.h"

// AOD fix for the "++" em quality
#include "egammaAnalysisUtils/IsEMPlusPlusHelper.h"

//Trigger matching 
#include "AnalysisTriggerEvent/EmTau_ROI.h"
#include "TrigParticle/TrigElectronContainer.h"
#include "egammaEvent/ElectronContainer.h"
#include "egammaEvent/egammaContainer.h"


// FIXME:  ugly hack for the trigger matching
#include "TrigDecisionTool/TrigDecisionToolCore.h"
#undef private


using std::cout;
using std::endl;

//------------------------------------------------------
ZeeDReadElecAOD::ZeeDReadElecAOD() :
    fElectronInProcess(NULL),
    fdRCut(0.2),
    bUseTrigger(kFALSE),
    fElecContainer(NULL),
    fCaloClusContainer(NULL),
    fSvcHelper("ZeeDReadElecAOD"),
    fAnaOptions(fSvcHelper.GetAnaSvc()),
    fMCTruthClassifier(0),
    fStoreGate(fSvcHelper.GetStoreGate()),
    fTrigMatchTool(fSvcHelper.GetTrigMatchTool()),
    fTrigDecisionTool(fSvcHelper.GetTrigDecisionTool())

{
    // Constructor

    if( (*fAnaOptions)->IsMC() && (*fAnaOptions)->DoTruthMatching() ) {

        fMCTruthClassifier = fSvcHelper.GetMCTruthClassifier();

    }

    // phiCalo/etaCalo calculation
    m_caloCellDetPos = new CaloCellDetPos();

    // Initalize services
    StatusCode sc = this->Initialize();

    if ( sc.isFailure() ) {
        Error("ZeeDReadElecAOD", "Failed to initialize object");
        gSystem->Exit(EXIT_FAILURE);
    }
}

//------------------------------------------------------
ZeeDReadElecAOD::~ZeeDReadElecAOD()
{
    // Destructor
}

//------------------------------------------------------
StatusCode ZeeDReadElecAOD::Initialize()
{
    // Initializer
    DEBUG_MES_START;

    StatusCode sc = StatusCode::SUCCESS;

    // Get trigger chain names
    if ( (*fAnaOptions)->UseTrigger() ) {

        fL1TrigChainNames = (*fAnaOptions)->L1TriggerElecMatch();
        fL2TrigChainNames = (*fAnaOptions)->L2TriggerElecMatch();
        fEFTrigChainNames = (*fAnaOptions)->EFTriggerElecMatch();

        if ( fTrigDecisionTool == 0 || fTrigMatchTool == 0) {
            Error("ZeeDReadElecAOD::Initialize",
                  " TrigDecTool or TrigMatchTool is not initialized");
            return StatusCode::FAILURE;
        }

        bUseTrigger = kTRUE;
    }

    if( (*fAnaOptions)->FillIsolationDecision() ) {

      f_egammaIsolationMVATopTool = (*fAnaOptions)->GetIsolationTool();
      sc = f_egammaIsolationMVATopTool.retrieve();
      if ( sc.isFailure() ) {
	Error("ZeeDAnalysisSvc::initialize", "Can't get a handle on the IsolationMVATool");
	return sc;
      }

    }

    fClusterCorrectionTool = (*fAnaOptions)->ClusterCorrectionTool();
    sc = fClusterCorrectionTool.retrieve();


    // Get if the electron is the MC electron
    std::string nameSrv("ZeeDAnalysisSvc");
    ServiceHandle<IZeeDAnalysisSvc> ZeeDAnaOptions(nameSrv, "");
    sc = ZeeDAnaOptions.retrieve();
    fIsMC = (ZeeDAnaOptions)->IsMC();

    DEBUG_MES_END;

    return sc;
}

//------------------------------------------------------
void ZeeDReadElecAOD::ReadInfo(ZeeDEvent* event)
{
    DEBUG_MES_START;

    const EventInfo* evt;
    StatusCode sc = (*fStoreGate)->retrieve(evt);
    if ( sc.isFailure()) {
        Error("ZeeDReadElecAOD::Initialize",
              "Event is not found");
    }

    // Get electron  container
    static const std::string elecContainerName = (*fAnaOptions)->ElectronContainer();
    sc = (*fStoreGate)->retrieve( fElecContainer, elecContainerName );
    if ( sc.isFailure() || fElecContainer == NULL ) {
        Error("ZeeDReadElecAOD::Initialize",
              "Electron container is not found");
    }
/*
   // XXX: not needed?
    // Read cluster container from persistecy storage
    std::string caloclusContainerName = (*fAnaOptions)->CaloClusterContainer();
    sc = (*fStoreGate)->retrieve( fCaloClusContainer, caloclusContainerName );

    if ( sc.isFailure()  || fCaloClusContainer == NULL ) {
        Error("ZeeDReadElecAOD::Initialize",
              "Calo Cluster container is not found");
    }
*/
    TObjArray* electrons = event->GetElectrons();

    // Clear electron arrays
    electrons->Clear();

    // Iterators over the electron container
    ElectronContainer::const_iterator elecItrB = fElecContainer->begin();
    ElectronContainer::const_iterator elecItrE = fElecContainer->end();

    DEBUG_VAR(fElecContainer->size()*event->GetGenWeight());

    // Loop over the AOD electron container
    for ( ElectronContainer::const_iterator elecItr  = fElecContainer->begin(); elecItr != elecItrE; ++elecItr) {

        // Tracks and clusters
        const Rec::TrackParticle* trackParticle        = (*elecItr)->trackParticle();
        const CaloCluster*        caloCluster          = (*elecItr)->cluster();
        const EMShower*           emshower             = (*elecItr)->detail<EMShower>("egDetailAOD");
        const EMTrackMatch*       trackMatch           = (*elecItr)->detail<EMTrackMatch>("egDetailAOD");
        const Trk::TrackSummary*  trackParticleSummary = trackParticle ? trackParticle->trackSummary() : 0;
        const Trk::VxCandidate*   vertex               = trackParticle ? trackParticle->reconstructedVertex() : 0;
	const HepMC::GenParticle* genParticle = NULL;
	if ((*fAnaOptions)->IsMC() && (*fAnaOptions)->DoTruthMatching()) {
	  (void) (*fMCTruthClassifier)->particleTruthClassifier(*elecItr);
	  genParticle = (*fMCTruthClassifier)->getGenPart();
	}
       
        // Electrons without track and cluster do happen.
        // What should we do in such a case?
        // I decided to "continue" and ignore that electron.
        if( caloCluster == NULL && trackParticle == NULL) {
            DEBUG_MES("Electron has neither cluster nor a track associated! Ignore it.")
            continue;
        }

	// Create new electron
        fElectronInProcess = new ZeeDElectron();
		fElectronInProcess->fCollectionIndex= std::distance(elecItrB, elecItr);
	
        fElectronInProcess->setFourVector(TLorentzVector((*elecItr)->get4Mom().px(),
                                                                (*elecItr)->get4Mom().py(),
                                                                (*elecItr)->get4Mom().pz(),
								(*elecItr)->get4Mom().e()
                                                                ));

        // Access Object Qoulity information
        // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/LArCleaningAndObjectQuality#Usage_on_AOD
        Bool_t isGoodOQAOD = (*elecItr)->isgoodoq(egammaPID::BADCLUSELECTRON) == 0;
        fElectronInProcess->IsGoodOQAOD().Set(  isGoodOQAOD  );
        fElectronInProcess->IsGoodOQAOD().Lock();

	fElectronInProcess->setHasCharge((*elecItr)->hasCharge());
	fElectronInProcess->setCharge((*elecItr)->charge() / eplus);

        if(trackParticle != NULL) {
            this->ReadTrackParticle(trackParticle);
        }

        if(vertex        != NULL) {
            this->ReadVertex(vertex);
        }

        if(caloCluster   != NULL) {
            this->ReadCluster(caloCluster);
        }

        if(emshower      != NULL) {
            this->ReadShower(emshower);
        }

        if(trackMatch    != NULL) {
            this->ReadTrackMatch(trackMatch);
        }

        if(genParticle   != NULL) {
            this->ReadMCTruth(genParticle);
        }

        if(bUseTrigger && (*fAnaOptions)->DoTriggerMatching()) {
            this->ReadTrigger( *elecItr );
        }

        if(trackParticleSummary != NULL) {
            this->ReadTrackSummary(trackParticleSummary);
        }

        this->ReadAuthor( *elecItr );
        this->ReadIsEM( *elecItr );

        if( (*fAnaOptions)->FillIsolationDecision() ){
            this->ReadCaloIso( *elecItr );
        }

        electrons->AddLast( fElectronInProcess );
        fElectronInProcess = NULL;
    }

    DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDReadElecAOD::ReadAuthor(const Analysis::Electron* aodElectron)
{
    // Reads electron's author

    DEBUG_MES_START;

    ZeeDElectronBags::Author authorBag;

    authorBag.bits = aodElectron->author();

    authorBag.isUnknown  = aodElectron->author(egammaParameters::AuthorUnknown);
    authorBag.isElectron = aodElectron->author(egammaParameters::AuthorElectron);
    authorBag.isSofte    = aodElectron->author(egammaParameters::AuthorSofte);
    authorBag.isPhoton   = aodElectron->author(egammaParameters::AuthorPhoton);
    authorBag.isFrwd     = aodElectron->author(egammaParameters::AuthorFrwd);
    authorBag.isRConv    = aodElectron->author(egammaParameters::AuthorRConv);

    CHECK_NULL_PTR(fElectronInProcess);
    fElectronInProcess->Author().Set(authorBag);
    fElectronInProcess->Author().Lock();

    DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDReadElecAOD::ReadIsEM(const Analysis::Electron* aodElectron)
{
    // Reads electron's isEm flag
    ZeeDLeptonBags::IsEM IsEMBag;

    /* see PhysicsAnalysis/D3PDMaker/egammaD3PDMaker/src/egammaIsEMoneFillerTool.cxx */
    IsEMBag.bits        = static_cast<unsigned int>(aodElectron->egammaID(egammaPID::IsEM));
    IsEMBag.bits_Loose  = static_cast<unsigned int>(aodElectron->egammaID(egammaPID::IsEMLoose));
    IsEMBag.bits_Medium = static_cast<unsigned int>(aodElectron->egammaID(egammaPID::IsEMMedium));
    IsEMBag.bits_Tight  = static_cast<unsigned int>(aodElectron->egammaID(egammaPID::IsEMTight));

    
    // FIXME: 	for now the code calculates the em decision for the 2012 year; this should
    //		be fixed (the decision should come just from the AOD) as soon as AOD's are fixed
    // For the details see:
    // 		https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/TechnicalitiesForMedium1#Macro

    IsEMBag.isLoose        = aodElectron->passID(egammaPID::ElectronIDLoose);
    // bug fix (?)
    //IsEMBag.isLoosePP      = aodElectron->passID(egammaPID::ElectronIDLoosePP); //isEMPlusPlusHelper::IsLoosePlusPlus(aodElectron, egammaMenu::eg2012);
    // Aliaksei
    IsEMBag.isLoosePP      = isEMPlusPlusHelper::IsLoosePlusPlus(aodElectron, egammaMenu::eg2012);
    IsEMBag.isMedium       = aodElectron->passID(egammaPID::ElectronIDMedium);
    IsEMBag.isMediumPP     = isEMPlusPlusHelper::IsMediumPlusPlus(aodElectron, egammaMenu::eg2012);
    IsEMBag.isTight        = aodElectron->passID(egammaPID::ElectronIDTight);
    IsEMBag.isTightPP      = isEMPlusPlusHelper::IsTightPlusPlus(aodElectron, egammaMenu::eg2012);

    IsEMBag.isFrwdLoose    = aodElectron->isElectron(egammaPID::frwdElectronLoose);
    IsEMBag.isFrwdTight    = aodElectron->isElectron(egammaPID::frwdElectronTight);



    // FIXME: add also LLH estimators
    // see https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/ElectronLikelihoodPID
    // release v > 17.7.0 is needed



    CHECK_NULL_PTR(fElectronInProcess);
    fElectronInProcess->IsEM().Set(IsEMBag);
    fElectronInProcess->IsEM().Lock();
}

//------------------------------------------------------
void ZeeDReadElecAOD::ReadCaloIso(const Analysis::Electron* aodElectron)
{
    // Reads electron's CaloIso flag
    DEBUG_MES_START;

  ZeeDElectronBags::CaloIso CaloIsoBag;

    CaloIsoBag.CaloIso98  = f_egammaIsolationMVATopTool->getCutDecision( (aodElectron), egammaIsolationType::IsoElectron_Jet, egammaIsolationType::caloIso98);
    CaloIsoBag.CaloIso95  = f_egammaIsolationMVATopTool->getCutDecision( (aodElectron), egammaIsolationType::IsoElectron_Jet, egammaIsolationType::caloIso95);

    CHECK_NULL_PTR(fElectronInProcess);
    fElectronInProcess->CaloIso().Set(CaloIsoBag);
    fElectronInProcess->CaloIso().Lock();

    DEBUG_MES_END;
}

//------------------------------------------------------

void ZeeDReadElecAOD::ReadTrackMatch(const EMTrackMatch* trackMatch)
{
    // Reads track match parameters

    DEBUG_MES_START;

    ZeeDElectronBags::TrackMatch trackMatchBag;

    trackMatchBag.deltaEta1 = trackMatch->deltaEta(1);
    trackMatchBag.deltaEta2 = trackMatch->deltaEta(2);
    trackMatchBag.deltaPhi2 = trackMatch->deltaPhi(2);

    trackMatchBag.trackd0_physics = trackMatch->trackd0_physics();
    trackMatchBag.trackd0_pv      = trackMatch->trackd0_pv();

    trackMatchBag.expectHitInBLayer = trackMatch->parameter(egammaParameters::expectHitInBLayer);

    CHECK_NULL_PTR(fElectronInProcess);
    fElectronInProcess->TrackMatch().Set(trackMatchBag);
    fElectronInProcess->TrackMatch().Lock();

    DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDReadElecAOD::ReadShower(const EMShower* emshower)
{
    // Reads e/m shower parameters
    ZeeDElectronBags::Shower showerBag;

    showerBag.f1      = emshower->f1();
    showerBag.f1core  = emshower->f1core();
    showerBag.e2tsts1 = emshower->e2tsts1();
    showerBag.emins1  = emshower->emins1();
    showerBag.weta1   = emshower->weta1();
    showerBag.wtots1  = emshower->wtots1();
    showerBag.fracs1  = emshower->fracs1();
    showerBag.widths2 = emshower->widths2();
    showerBag.f3core  = emshower->f3core();
    showerBag.e237    = emshower->e237();
    showerBag.e233    = emshower->e233();
    showerBag.e277    = emshower->e277();
    showerBag.weta2   = emshower->weta2();
    showerBag.etcone15   = emshower->etcone15();
    showerBag.etcone20   = emshower->etcone20();
    showerBag.etcone25   = emshower->etcone25();
    showerBag.etcone30   = emshower->etcone30();
    showerBag.etcone35   = emshower->etcone35();
    showerBag.etcone40   = emshower->etcone40();
    showerBag.etap       = emshower->etap();
    showerBag.ptcone20   = emshower->ptcone20();
    showerBag.ptcone30   = emshower->ptcone30();
    showerBag.ptcone40   = emshower->ptcone40();
    fElectronInProcess->Shower().Set(showerBag);
    fElectronInProcess->Shower().Lock();
}

//------------------------------------------------------
void ZeeDReadElecAOD::ReadCluster(const CaloCluster* caloCluster)
{

  // Get raw cluster for the GEO21 calculation.
  // We can only do this for the SW clusters.
  CaloCluster* rawCluster = NULL;
  switch (caloCluster->getClusterSize()) // it is a fast form of if-else of what?
  {
    case CaloCluster::SW_55ele:
    case CaloCluster::SW_35ele:
    case CaloCluster::SW_37ele:
    case CaloCluster::SW_55gam:
    case CaloCluster::SW_35gam:
    case CaloCluster::SW_37gam:
    case CaloCluster::SW_55Econv:
    case CaloCluster::SW_35Econv:
    case CaloCluster::SW_37Econv:
    case CaloCluster::SW_softe:

      rawCluster = CaloClusterStoreHelper::makeCluster(
	  caloCluster->eta0(),
	  caloCluster->phi0(),
	  0x80000007,
	  caloCluster->getClusterSize());

      rawCluster->setBarrel (caloCluster->inBarrel());
      rawCluster->setEndcap (caloCluster->inEndcap());

      fClusterCorrectionTool->execute(rawCluster->getClusterSize(), rawCluster);

    default: ;
  }


  // Reads cluster parameters
  ZeeDElectronBags::Cluster clusterBag;

  clusterBag.etaBE2           = caloCluster->etaBE(2);
  clusterBag.energy           = caloCluster->energy() / GeV;
  
  if ( clusterBag.etaBE2 == 0.0  ||  fabs(clusterBag.etaBE2) > 10){
    clusterBag.eT = 0;
  } else {
    clusterBag.eT           = clusterBag.energy / TMath::CosH( TMath::Abs(clusterBag.etaBE2) );
  }

  clusterBag.fourVector       = ZeeDMisc::FormTLorentzVec( caloCluster->hlv() / GeV );
  clusterBag.CentralLambda    = caloCluster->getMomentValue(CaloClusterMoment::CENTER_LAMBDA);
  clusterBag.SecondLambda     = caloCluster->getMomentValue(CaloClusterMoment::SECOND_LAMBDA);
  clusterBag.SecondR          = caloCluster->getMomentValue(CaloClusterMoment::SECOND_R);
  clusterBag.longitudinal     = caloCluster->getMomentValue(CaloClusterMoment::LONGITUDINAL);
  clusterBag.lateral          = caloCluster->getMomentValue(CaloClusterMoment::LATERAL);
  clusterBag.FracMax          = caloCluster->getMomentValue(CaloClusterMoment::ENG_FRAC_MAX);

  // variables for the GEO21 em calibration
  if (rawCluster) {
    clusterBag.rawcl_Es0       = rawCluster->energyBE(0)/GeV;
    clusterBag.rawcl_Es1       = rawCluster->energyBE(1)/GeV;
    clusterBag.rawcl_Es2       = rawCluster->energyBE(2)/GeV;
    clusterBag.rawcl_Es3       = rawCluster->energyBE(3)/GeV;
  } else {
    clusterBag.rawcl_Es0       = 0.;
    clusterBag.rawcl_Es1       = 0.;
    clusterBag.rawcl_Es2       = 0.;
    clusterBag.rawcl_Es3       = 0.;
  }
  CaloCell_ID::CaloSample sam = CaloCell_ID::EMB2;
  if (caloCluster->inBarrel() && !caloCluster->inEndcap()) {
    sam = CaloCell_ID::EMB2;
  } else if (!caloCluster->inBarrel() && caloCluster->inEndcap()) {
    sam = CaloCell_ID::EME2;
  } else { 
    if (caloCluster->eSample(CaloSampling::EMB2) > caloCluster->eSample(CaloSampling::EME2)) {
      sam=CaloCell_ID::EMB2;
    } else {
      sam = CaloCell_ID::EME2;
    }
  }
  double etaCalo, phiCalo;
  m_caloCellDetPos->getDetPosition(sam, caloCluster->eta(), caloCluster->phi(), etaCalo, phiCalo);
  clusterBag.cl_etaCalo      = etaCalo;
  clusterBag.cl_phiCalo      = phiCalo;

  // lock the cluster container
  fElectronInProcess->Cluster().Set(clusterBag);
  fElectronInProcess->Cluster().Lock();
}

//------------------------------------------------------
void ZeeDReadElecAOD::ReadTrackParticle(const Rec::TrackParticle* trackParticle )
{
    // Reads electron track
    DEBUG_MES_START;

    ZeeDLeptonBags::TrackParticle trackParticleBag;

    trackParticleBag.eta = trackParticle->eta();
    trackParticleBag.pt  = trackParticle->pt() / GeV;
    trackParticleBag.fourVector = ZeeDMisc::FormTLorentzVec( trackParticle->hlv() / GeV );

    CHECK_NULL_PTR(fElectronInProcess);
    fElectronInProcess->TrackParticle().Set(trackParticleBag);
    fElectronInProcess->TrackParticle().Lock();

    DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDReadElecAOD::ReadTrackSummary(const Trk::TrackSummary* trackParticleSummary)
{
    // Reads track  aditional parameters
    DEBUG_MES_START;

    ZeeDLeptonBags::TrackSummary trackSummaryBag;

    trackSummaryBag.numberOfBLayerHits = trackParticleSummary->get(Trk::numberOfBLayerHits);
    trackSummaryBag.numberOfPixelHits  = trackParticleSummary->get(Trk::numberOfPixelHits);
    trackSummaryBag.numberOfSCTHits    = trackParticleSummary->get(Trk::numberOfSCTHits);

    trackSummaryBag.numberOfBLayerOutliers = trackParticleSummary->get(Trk::numberOfBLayerOutliers);
    trackSummaryBag.numberOfPixelOutliers  = trackParticleSummary->get(Trk::numberOfPixelOutliers);
    trackSummaryBag.numberOfSCTOutliers    = trackParticleSummary->get(Trk::numberOfSCTOutliers);

    trackSummaryBag.numberOfTRTHits                  = trackParticleSummary->get(Trk::numberOfTRTHits);
    trackSummaryBag.numberOfTRTOutliers              = trackParticleSummary->get(Trk::numberOfTRTOutliers); 
    trackSummaryBag.numberOfTRTHighThresholdHits     = trackParticleSummary->get(Trk::numberOfTRTHighThresholdHits);
    trackSummaryBag.numberOfTRTHighThresholdOutliers = trackParticleSummary->get(Trk::numberOfTRTHighThresholdOutliers);
    trackSummaryBag.numberOfCscEtaHits               = trackParticleSummary->get(Trk::numberOfCscEtaHits);
 


    CHECK_NULL_PTR(fElectronInProcess);
    fElectronInProcess->TrackSummary().Set(trackSummaryBag);
    fElectronInProcess->TrackSummary().Lock();

    DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDReadElecAOD::ReadVertex(const Trk::VxCandidate* vertex)
{
    // Read electron vertex
    DEBUG_MES_START;

    ZeeDLeptonBags::Vertex vertexBag;

    // Reconstructed vertex
    const Trk::RecVertex    vtx      = vertex->recVertex();
    const CLHEP::Hep3Vector position = vtx.position() / millimeter;

    vertexBag.position.SetXYZ(position[0],
                              position[1],
                              position[2]);

    // Vertex errors
    const Trk::ErrorMatrix errvtx = vtx.errorPosition() ;

    vertexBag.Err2.SetXYZ(errvtx.covariance()[0][0] / millimeter,
                          errvtx.covariance()[1][1] / millimeter,
                          errvtx.covariance()[2][2] / millimeter);

    CHECK_NULL_PTR(fElectronInProcess);
    fElectronInProcess->Vertex().Set(vertexBag);
    fElectronInProcess->Vertex().Lock();

    DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDReadElecAOD::ReadMCTruth(const HepMC::GenParticle* genParticle)
{
    // Read electron mc truth info
    DEBUG_MES_START;

    ZeeDLeptonBags::MCTruth MCTruthBag;

    // electron type and origin
    std::pair < MCTruthPartClassifier::ParticleType,
        MCTruthPartClassifier::ParticleOrigin >
        res = (*fMCTruthClassifier)->particleTruthClassifier(genParticle);

    const MCTruthPartClassifier::ParticleType TypeOfEle = res.first;

    MCTruthBag.trueLepton      = genParticle;
    MCTruthBag.truthLeptonType = TypeOfEle;

    const HepMC::FourVector& fourVector = genParticle->momentum();

    const Double_t E  = fourVector.e()  / GeV;
    const Double_t Px = fourVector.px() / GeV;
    const Double_t Py = fourVector.py() / GeV;
    const Double_t Pz = fourVector.pz() / GeV;

    MCTruthBag.fourVector = TLorentzVector(Px, Py, Pz, E);

    CHECK_NULL_PTR(fElectronInProcess);
    fElectronInProcess->MCTruth().Set(MCTruthBag);
    fElectronInProcess->MCTruth().Lock();

    DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDReadElecAOD::ReadTrigger(const Analysis::Electron* AODelectron)
{

  // FIXME: ugly hack for the trigger matching 
  Trig::TrigDecisionToolCore* tdt = (*fTrigMatchTool)->m_trigDecisionToolCore;



  const EventInfo* evt;
  StatusCode sc = (*fStoreGate)->retrieve(evt);

  if ( sc.isSuccess() == kFALSE) {
    Error("ZeeDReadEventAOD::ReadTrigger",
	"Can not get EventInfo");
    gSystem->Exit(EXIT_FAILURE);
  }

  const Int_t  runNumber = evt->event_ID()->run_number();
  const Bool_t isData    = (*fAnaOptions)->IsData();

  ZeeDLeptonBags::Trigger TriggerBag;

  std::vector<ZeeDEnum::Trigger::Value> defaultTriggers;

  // always check default trigger chain
  const ZeeDEnum::Trigger::Value defaultSingleElecTrigger =
    isData ? ZeeDMisc::GetDefaultSingleElectronTrigger(runNumber) :	// DATA
    ZeeDMisc::GetDefaultSingleElectronTrigger(runNumber)		// MC
    ;

  if(defaultSingleElecTrigger == ZeeDEnum::Trigger::UNKNOWN) {
    Error("ZeeDReadElecAOD::ReadTrigger","no default trigger found");
    gSystem->Exit(EXIT_FAILURE);
  }

  defaultTriggers.push_back(defaultSingleElecTrigger);

  // always check default trigger chain
  const ZeeDEnum::Trigger::Value defaultDiElecTrigger =
    isData ? ZeeDMisc::GetDefaultDiElectronTrigger(runNumber) : // DATA
    ZeeDMisc::GetDefaultDiElectronTrigger(runNumber)        // MC
    ;

  if(defaultDiElecTrigger == ZeeDEnum::Trigger::UNKNOWN) {
    Error("ZeeDReadElecAOD::ReadTrigger","no default trigger found");
    gSystem->Exit(EXIT_FAILURE);
  }

  defaultTriggers.push_back(defaultDiElecTrigger);

  // FIXME: ugly hack for the trigger matching
  for(UInt_t t = 0; t != defaultTriggers.size(); ++t) {

    const ZeeDEnum::Trigger::Value defaultTrigger = defaultTriggers[t];
    const std::string defaultTrigger_str = ZeeDEnum::Trigger::ToString(defaultTriggers[t]);

    Bool_t passed = kFALSE;

    if(defaultTrigger_str.substr(0,2) == "L1") {

      const Trig::FeatureContainer trfeatures = tdt->features(defaultTrigger_str);
      const std::vector< Trig::Feature<CaloCluster> > trfeatures_clusters = trfeatures.get<CaloCluster>();
      passed = this->IsPassedTrigMatch(AODelectron, trfeatures_clusters, fdRCut);

    } else if(defaultTrigger_str.substr(0,2) == "L2") {

      const Trig::FeatureContainer trfeatures = tdt->features(defaultTrigger_str);
      const std::vector< Trig::Feature<CaloCluster> > trfeatures_clusters = trfeatures.get<CaloCluster>();
      passed = this->IsPassedTrigMatch(AODelectron, trfeatures_clusters, fdRCut);


    } else if(defaultTrigger_str.substr(0,2) == "EF") {


      const Trig::FeatureContainer trfeatures = tdt->features(defaultTrigger_str);
      const std::vector< Trig::Feature<CaloCluster> > trfeatures_clusters = trfeatures.get<CaloCluster>();
      passed = this->IsPassedTrigMatch(AODelectron, trfeatures_clusters, fdRCut);


    } else {

      Error("ZeeDReadElecAOD::ReadTrigger",TString("Chain prefix not L1, L2 or EF, but ")+defaultTrigger_str.substr(0,2));
      gSystem->Exit(EXIT_FAILURE);

    }

    TriggerBag.isMatchedToChain.insert(std::make_pair(defaultTrigger, passed));

  }

     

  //const ChainGroup* chain = tdt->getChainGroup("L1_.*");
 /* std::vector<std::string> bL1TrigChainNames = tdt->getChainGroup("L1_.*")->getListOfTriggers();
  for(UInt_t i = 0; i != bL1TrigChainNames.size(); ++i) {
    const std::string chainName = bL1TrigChainNames[i];
    const Trig::FeatureContainer trfeatures = tdt->features(chainName);
    const std::vector< Trig::Feature<CaloCluster> > trfeatures_clusters = trfeatures.get<CaloCluster>();
    const Bool_t passed = this->IsPassedTrigMatch(AODelectron, trfeatures_clusters, fdRCut);
    const TrigMatch::TrigElectronL1 *match = (*fTrigMatchTool)->matchToTriggerObject< TrigMatch::TrigElectronL1 >( AODelectron, chainName, 0.1, true);
    bool passed2 = match;
    if (passed2 != passed) 
          std::cout << "TRIGGER  " << chainName << " "  <<  passed  << std::endl;
    }
  */
/*
  std::cout.precision(15);
  std::vector<std::string> bL2TrigChainNames = tdt->getChainGroup("L2_.*")->getListOfTriggers();
  for(UInt_t i = 0; i != bL2TrigChainNames.size(); ++i) {   
    const std::string chainName = bL2TrigChainNames[i];
    const Trig::FeatureContainer trfeatures = tdt->features(chainName);
    const std::vector< Trig::Feature<CaloCluster> > trfeatures_clusters = trfeatures.get<CaloCluster>();
    const Bool_t passed = this->IsPassedTrigMatch(AODelectron, trfeatures_clusters, fdRCut);
    const TrigMatch::TrigElectronL2 *match = (*fTrigMatchTool)->matchToTriggerObject< TrigMatch::TrigElectronL2 >( AODelectron, chainName, 0.2, true);
    bool passed2 = match!= NULL;
    if (passed2 != passed)
          std::cout << "TRIGGER  " << chainName << " old = "  <<  passed  << " new = "<< passed2 << std::endl;
    if (passed2) {
       double  distance = (*fTrigMatchTool)->distance(AODelectron, match);
       if (distance != 0)
               std::cout << "For new dR = " << distance << std::endl;
       bool match2 = false;
       std::vector< std::string > matchingChains = (*fTrigMatchTool)->chainsPassedByObject< TrigMatch::TrigElectronL2 >(AODelectron);
       for( std::vector< std::string >::const_iterator chainIter = matchingChains.begin();chainIter != matchingChains.end(); ++chainIter ) {
              if(chainName.compare(*chainIter) == 0)
                  match2=true;
              }
       if (!match2)
            std::cout << "Trigger " << chainName << "is passed, but not in list of matching chains" << std::endl;
       }
    
 */ 
  std::vector<std::string> bEFTrigChainNames = tdt->getChainGroup("EF_e15.*")->getListOfTriggers();
  std::cout << "Chain size " << bEFTrigChainNames.size() << std::endl;
  for(UInt_t i = 0; i != bEFTrigChainNames.size(); ++i) {
    const std::string chainName = bEFTrigChainNames[i];
    const Trig::FeatureContainer trfeatures = tdt->features(chainName);
    const std::vector< Trig::Feature<CaloCluster> > trfeatures_clusters = trfeatures.get<CaloCluster>();
    const Bool_t passed = this->IsPassedTrigMatch(AODelectron, trfeatures_clusters, fdRCut);
    const TrigMatch::TrigElectronEF *match =  (*fTrigMatchTool)->matchToTriggerObject< TrigMatch::TrigElectronEF >( AODelectron, chainName, 0.2, true);
    bool passed2 = match!=NULL;
    if (passed2 != passed)
          std::cout << "TRIGGER  " << chainName << " old="  <<  passed  <<" new = "<< passed2 << std::endl;
    if (passed2) {
       std::cout << "TRIGGER! " << chainName << std::endl;
       double  distance = (*fTrigMatchTool)->distance(AODelectron, match);
       if (distance != 0) 
               std::cout << "For new dR = " << distance << std::endl;
        bool match2 = false;
       std::vector< std::string > matchingChains = (*fTrigMatchTool)->chainsPassedByObject< TrigMatch::TrigElectronEF >(AODelectron);
       for( std::vector< std::string >::const_iterator chainIter = matchingChains.begin();chainIter != matchingChains.end(); ++chainIter ) {
              if(chainName.compare(*chainIter) == 0)
                  match2=true;
              }
       if (!match2)
            std::cout << "Trigger " << chainName << "is passed, but not in list of matching chains" << std::endl;
       }

    }

//    TriggerBag.isMatchedToChain.insert(std::make_pair(ZeeDEnum::Trigger::ToEnum(chainName), passed));


  for(UInt_t i = 0; i != fL1TrigChainNames.size(); ++i) {
    const std::string chainName = fL1TrigChainNames[i];
    const Trig::FeatureContainer trfeatures = tdt->features(chainName);
    const std::vector< Trig::Feature<CaloCluster> > trfeatures_clusters = trfeatures.get<CaloCluster>();
    const Bool_t passed = this->IsPassedTrigMatch(AODelectron, trfeatures_clusters, fdRCut);
    TriggerBag.isMatchedToChain.insert(std::make_pair(ZeeDEnum::Trigger::ToEnum(chainName), passed));
  }

  for(UInt_t i = 0; i != fL2TrigChainNames.size(); ++i) {
    const std::string chainName = fL2TrigChainNames[i];
    const Trig::FeatureContainer trfeatures = tdt->features(chainName);
    const std::vector< Trig::Feature<CaloCluster> > trfeatures_clusters = trfeatures.get<CaloCluster>();
    const Bool_t passed = this->IsPassedTrigMatch(AODelectron, trfeatures_clusters, fdRCut);

    TriggerBag.isMatchedToChain.insert(std::make_pair(ZeeDEnum::Trigger::ToEnum(chainName), passed));
  }

  for(UInt_t i = 0; i != fEFTrigChainNames.size(); ++i) {
    const std::string chainName = fEFTrigChainNames[i];
    const Trig::FeatureContainer trfeatures = tdt->features(chainName);
    const std::vector< Trig::Feature<CaloCluster> > trfeatures_clusters = trfeatures.get<CaloCluster>();
    const Bool_t passed = this->IsPassedTrigMatch(AODelectron, trfeatures_clusters, fdRCut);

    TriggerBag.isMatchedToChain.insert(std::make_pair(ZeeDEnum::Trigger::ToEnum(chainName), passed));
  }

  fElectronInProcess->Trigger().Set(TriggerBag);
  fElectronInProcess->Trigger().Lock();

}


// FIXME: for the handmade trigger match only
bool ZeeDReadElecAOD::IsPassedTrigMatch(const Analysis::Electron* AODelectron,
    					const std::vector< Trig::Feature<CaloCluster> > tfeatures_clusters,
					double fdRcut)
{
  const CaloCluster* elec_cluster = AODelectron->cluster();
  if (elec_cluster != NULL) {
    double elec_eta = elec_cluster->eta();
    double elec_phi = elec_cluster->phi();
    double dR2 = fdRcut*fdRcut;

    for (std::vector< Trig::Feature<CaloCluster> >::const_iterator it = tfeatures_clusters.begin(); it != tfeatures_clusters.end(); ++it) {

      double deta = elec_eta - ((*it).cptr())->eta();
      double dphi = this->NormalizeAngle(elec_phi - ((*it).cptr())->phi());

      if (deta*deta + dphi*dphi < dR2) {
                return true;
                }
    }

    return false;

  } else {
    double elec_eta = AODelectron->eta();
    double elec_phi = AODelectron->phi();
    double dR2 = fdRcut*fdRcut;

    for (std::vector< Trig::Feature<CaloCluster> >::const_iterator it = tfeatures_clusters.begin(); it != tfeatures_clusters.end(); ++it) {

      double deta = elec_eta - ((*it).cptr())->eta();
      double dphi = this->NormalizeAngle(elec_phi - ((*it).cptr())->phi());

      if (deta*deta + dphi*dphi < dR2) {
                  return true;
                  }
    }

    return false;

  }
}

double ZeeDReadElecAOD::NormalizeAngle(double dphi)
{
  static const double pi    = 3.1415926;
  static const double twopi = 2*pi;
  dphi = fabs(dphi - ((int)(dphi / twopi))*twopi);
  if (dphi > pi) dphi = twopi - dphi;
  return dphi;
}
