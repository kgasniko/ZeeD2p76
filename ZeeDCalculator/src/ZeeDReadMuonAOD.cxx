/*
 * ZeeDReadMuonAOD.cxx
 *
 *  Created on: Aug 7, 2012
 *      Author: morse
 */

#include "ZeeDCalculator/ZeeDReadMuonAOD.h"
#include "muonEvent/MuonParamDefs.h"
#include "MuonRecHelperTools/MuonEDMHelperTool.h"
#include "MuonRecHelperTools/MuonIdHelperTool.h"
#include "MuonCalibITools/IIdToFixedIdTool.h"
#include "MuonCombinedToolInterfaces/IMuonPropertiesTool.h"
#include "Particle/TrackParticleContainer.h"
#include "Particle/TrackParticle.h"
#include "ParticleTruth/TrackParticleTruthCollection.h"

#include "MCTruthClassifier/IMCTruthClassifier.h"
#include "MCTruthClassifier/MCTruthClassifierDefs.h"
#include "McParticleEvent/TruthParticleContainer.h"


#include "AnalysisTriggerEvent/Muon_ROI.h"
#include "TrigMuonEvent/MuonFeature.h"
#include "TrigMuonEvent/MuonFeatureContainer.h"
#include "muonEvent/MuonParamDefs.h"
#include "MuonRecHelperTools/MuonEDMHelperTool.h"
#include "MuonRecHelperTools/MuonIdHelperTool.h"
#include "MuonCalibITools/IIdToFixedIdTool.h"
#include "MuonCombinedToolInterfaces/IMuonPropertiesTool.h"
#include "TrigMuonEvent/TrigMuonEFInfoContainer.h"
#include "TrigMuonEvent/TrigMuonEFInfoTrackContainer.h"
#include "muonEvent/MuonContainer.h"
#include "MCTruthClassifier/IMCTruthClassifier.h"
#include "MCTruthClassifier/MCTruthClassifierDefs.h"

#include "TrigObjectMatching/TrigMatchTool.h"


#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDMuon.h"


ZeeDReadMuonAOD::ZeeDReadMuonAOD() :
  fSvcHelper("ZeeDReadMuonAOD"),
  fStoreGate(fSvcHelper.GetStoreGate()),
  fAnaOptions(fSvcHelper.GetAnaSvc()),
  m_muonPropertiesTool("Rec::MuonPropertiesTool/MuonPropertiesTool"),
  m_trackToVertexTool("Reco::TrackToVertex"),
  m_qualityTool ("MuonQualityTool"),
  fMCTruthClassifier(0),
  fMuonContainer(NULL), vxContainer(NULL),
  m_matchTool("TrigMatchTool/TrigMatchTool" )
{
    if( (*fAnaOptions)->IsMC() && (*fAnaOptions)->DoTruthMatching() ) {

        fMCTruthClassifier = fSvcHelper.GetMCTruthClassifier();

    }


}

ZeeDReadMuonAOD::~ZeeDReadMuonAOD() {
  // TODO Auto-generated destructor stub
}

void ZeeDReadMuonAOD::Initialize() {

}

void ZeeDReadMuonAOD::ReadInfo(ZeeDEvent* event)
{
  // Info(__PRETTY_FUNCTION__,"start");

  this->Initialize();
  // see
  // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/WZElectroweakCommonTopics2012
  // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/MCPAnalysisGuidelinesData2012
  // https://twiki.cern.ch/twiki/bin/viewauth/Atlas/TrigMuonEfficiency



  event->ClearMuons();


  // retrieve the container
  StatusCode sc = (*fStoreGate)->retrieve(fMuonContainer, "Muons" );
  if ( sc.isFailure() || !fMuonContainer) {
    Warning("ZeeDReadMuonAOD::ReadInfo","No muon collection found");
    return;
  }
  
  sc = (*fStoreGate)->retrieve(vxContainer, "VxPrimaryCandidate");
  if (sc.isFailure() || ! vxContainer) {
    REPORT_MESSAGE (MSG::WARNING) << "Could not retrieve primary vertex info: " << "VxPrimaryCandidate" << " will not fill from MuonTrackCovarianceAtPV";
    return;
  }

  Analysis::MuonContainer::const_iterator iMuonB = fMuonContainer->begin();
  Analysis::MuonContainer::const_iterator iMuonE = fMuonContainer->end();


  for( Analysis::MuonContainer::const_iterator iMuon(iMuonB); iMuon != iMuonE; ++iMuon) {

    Int_t ind(std::distance( iMuonB, iMuon ));

    ZeeDMuon * curmuon = new ZeeDMuon();
    curmuon->fCollectionIndex = ind;

    const Rec::TrackParticle* trackParticle        = (*iMuon)->track();
    const Trk::TrackSummary*  trackParticleSummary = trackParticle ? trackParticle->trackSummary() : 0;
    const Trk::VxCandidate*   vertex               = trackParticle ? trackParticle->reconstructedVertex() : 0; 
    const HepMC::GenParticle* genParticle = NULL;
    const Rec::TrackParticle* trackMS   = (*iMuon)-> muonSpectrometerTrackParticle();
    const Rec::TrackParticle* trackComb = (*iMuon)->track();
    const Rec::TrackParticle* trackID   = (*iMuon)-> inDetTrackParticle();
 
    if ((*fAnaOptions)->IsMC() && (*fAnaOptions)->DoTruthMatching()) {
	  (void) (*fMCTruthClassifier)->particleTruthClassifier(*iMuon);
	  genParticle = (*fMCTruthClassifier)->getGenPart();
	}
       
 	 

    TLorentzVector fourVector((*iMuon)->px()/GeV, (*iMuon)->py()/GeV, (*iMuon)->pz()/GeV, (*iMuon)->e()/GeV );
    curmuon->setFourVector(fourVector);
    curmuon->setCharge((*iMuon)->charge());

    this->ReadProperties(*iMuon);

    if (trackComb != NULL) {
       this->ReadCombTrack(trackComb, *iMuon);
    }

    if ( trackMS != NULL ){
        this->ReadMStrack(trackMS);
	   }

    if (trackID != NULL) {
        this->ReadIDtrack(trackID, *iMuon);
    }

    this->ReadIsolation(*iMuon);
  

    if(trackParticle != NULL) {
            this->ReadTrackParticle(trackParticle);
        }

    if(vertex        != NULL) {
            this->ReadVertex(vertex);
        }

    if(trackParticleSummary != NULL) {
            this->ReadTrackSummary(trackParticleSummary);
        }

    this->ReadIsEM( *iMuon );  
    
    if((*fAnaOptions)->DoTriggerMatching()){
       this->ReadTrigger(*iMuon);
       }    

    event->AddMuon(curmuon);
  }

  //Info(__PRETTY_FUNCTION__,"stop");

}

void ZeeDReadMuonAOD::ReadIsEM(const Analysis::Muon* iMuon){
   
   int mu_auth = ((iMuon)->author());
   int mu_prec=-1;
   if (curmuon->CombTrack().IsSet()) {
     curmuon->CombTrack().Get().fnprecisionLayers;
     }
   int mu_BalSig = (iMuon)->parameter(MuonParameters::momentumBalanceSignificance);
   bool mu_comb = (iMuon)->isCombinedMuon(); 

   ZeeDLeptonBags::IsEM IsEMBag;
   m_qualityTool->calcBits(*iMuon);

   IsEMBag.isLoose  = m_qualityTool->isLoose();
   IsEMBag.isMedium = m_qualityTool->isMedium();
   IsEMBag.isTight  = m_qualityTool->isTight();

   IsEMBag.isLoosePP = true;
   IsEMBag.isMediumPP = (IsEMBag.isMedium) && ((mu_auth==11 && mu_prec>2) || (mu_auth==6) || (mu_auth==12 && mu_BalSig<4) || (mu_auth==13 && mu_BalSig<4 && mu_comb));

   curmuon->IsEM().Set(IsEMBag);
   curmuon->IsEM().Lock();

}

void ZeeDReadMuonAOD::ReadTrackParticle(const Rec::TrackParticle* trackParticle )
{
    DEBUG_MES_START;

    ZeeDLeptonBags::TrackParticle trackParticleBag;

    trackParticleBag.eta = trackParticle->eta();
    trackParticleBag.pt  = trackParticle->pt() / GeV;
    trackParticleBag.fourVector = ZeeDMisc::FormTLorentzVec( trackParticle->hlv() / GeV );

    CHECK_NULL_PTR(curmuon);
    curmuon->TrackParticle().Set(trackParticleBag);
    curmuon->TrackParticle().Lock();

    DEBUG_MES_END;
}


//------------------------------------------------------
void ZeeDReadMuonAOD::ReadTrackSummary(const Trk::TrackSummary* trackParticleSummary)
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
 
    CHECK_NULL_PTR(curmuon);
    curmuon->TrackSummary().Set(trackSummaryBag);
    curmuon->TrackSummary().Lock();

    DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDReadMuonAOD::ReadVertex(const Trk::VxCandidate* vertex)
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

    CHECK_NULL_PTR(curmuon);
    curmuon->Vertex().Set(vertexBag);
    curmuon->Vertex().Lock();

    DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDReadMuonAOD::ReadMCTruth(const HepMC::GenParticle* genParticle)
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

    CHECK_NULL_PTR(curmuon);
    curmuon->MCTruth().Set(MCTruthBag);
    curmuon->MCTruth().Lock();

    DEBUG_MES_END;
}

void ZeeDReadMuonAOD::ReadProperties(const Analysis::Muon* iMuon)
{
    // Reading basic properties of muon
    
    ZeeDMuonBags::Properties propBag;
   
    propBag.fisStandAloneMuon			= (iMuon)->isStandAloneMuon();
    propBag.fisSegmentTaggedMuon		= (iMuon)->isSegmentTaggedMuon();
    propBag.fisSiliconAssociatedForwardMuon	= (iMuon)->isSiliconAssociatedForwardMuon();
    propBag.fisCaloMuonId	= (iMuon)->isCaloMuonId();
    propBag.fisCombinedMuon	= (iMuon)->isCombinedMuon();
    propBag.fauthor		= (iMuon)->author();
    propBag.fistrack = true;
    

    curmuon->Properties().Set(propBag);
    curmuon->Properties().Lock();
}

void ZeeDReadMuonAOD::ReadCombTrack(const  Rec::TrackParticle* track, const Analysis::Muon* iMuon)
{ 
    ZeeDMuonBags::CombTrack combBag;
    const Trk::MeasuredPerigee* perigee(NULL);
    VxContainer::const_iterator vxI = vxContainer->begin();

    combBag.fmomentumBalanceSignificance = (iMuon)->momentumBalanceSignificance();
      combBag.ftracktheta	= track->measuredPerigee()->parameters()[Trk::theta];
      combBag.ftrackphi	= track->measuredPerigee()->parameters()[Trk::phi0];
      combBag.ftrackpt		= track->measuredPerigee()->pT()/GeV;
      combBag.ftracketa	= track->measuredPerigee()->eta();

    if((*vxI)) {
      perigee = m_trackToVertexTool->perigeeAtVertex((*track), (*vxI)->recVertex().position());
    }
    if(perigee) {
      const Trk::ErrorMatrix errormat = perigee->localErrorMatrix();
      combBag.fd0_exPV		= perigee->parameters()[Trk::d0];
      combBag.fsigd0_exPV	= errormat.error(Trk::d0) == 0 ? 0 : perigee->parameters()[Trk::d0]/errormat.error(Trk::d0);
      combBag.fz0_exPV		= perigee->parameters()[Trk::z0];
      combBag.fsigz0_exPV	= errormat.error(Trk::z0) == 0 ? 0 : perigee->parameters()[Trk::z0]/errormat.error(Trk::z0);
      combBag.fphi_exPV	= perigee->parameters()[Trk::phi0];
      combBag.ftheta_exPV	= perigee->parameters()[Trk::theta];
      combBag.fqoverp_exPV	= perigee->parameters()[Trk::qOverP]*GeV;
    }
    if (perigee) delete perigee; perigee = NULL;
    
    if ((iMuon)->hasCombinedMuonTrackParticle()) {
        combBag.fcombinedMuonTrackParticle = TLorentzVector((iMuon)->combinedMuonTrackParticle()->e() /GeV,
							   (iMuon)->combinedMuonTrackParticle()->px()/GeV,
							   (iMuon)->combinedMuonTrackParticle()->py()/GeV,
							   (iMuon)->combinedMuonTrackParticle()->pz()/GeV);
    }

    // precision layers
    combBag.fnprecisionLayers= -1;
    combBag.fnprecisionHoleLayers= -1;
    if ((iMuon)->combinedMuonTrkTrack() != NULL) {
      int nprecisionLayers(0), nprecisionHoleLayers(0);

      Muon::IMuonHitSummaryTool::CompactSummary summary = m_muonPropertiesTool->muonHitSummary(*iMuon);
      std::map<Muon::IMuonHitSummaryTool::StIndex,Muon::IMuonHitSummaryTool::HitSummary>::const_iterator hsit = summary.stationLayers.begin();
      std::map<Muon::IMuonHitSummaryTool::StIndex,Muon::IMuonHitSummaryTool::HitSummary>::const_iterator hsit_end = summary.stationLayers.end();
      for( ;hsit != hsit_end; ++hsit ){
	const Muon::IMuonHitSummaryTool::HitSummary& hitSummary = hsit->second;
	if( hitSummary.nprecisionHits > 2 )                                                     ++nprecisionLayers;
	else if( hitSummary.nprecisionHoles > 2 && hsit->first!=Muon::IMuonHitSummaryTool::EE ) ++nprecisionHoleLayers;
      }
      combBag.fnprecisionLayers=nprecisionLayers;
      combBag.fnprecisionHoleLayers=nprecisionHoleLayers;
    }



    curmuon->CombTrack().Set(combBag);
    curmuon->CombTrack().Lock();
}

void ZeeDReadMuonAOD::ReadMStrack(const  Rec::TrackParticle* trackMS)
{
   ZeeDMuonBags::MStrack msBag;
   const Trk::MeasuredPerigee* perigee(NULL);
   VxContainer::const_iterator vxI = vxContainer->begin();
   bool trackmsok( (trackMS != NULL) &&  (trackMS->measuredPerigee() != NULL) );
   
   msBag.fhasMuonSpectrometerTrack = true;
   msBag.fmstracktheta	= trackMS->measuredPerigee()->parameters()[Trk::theta]; 
   msBag.fmstrackphi	= trackMS->measuredPerigee()->parameters()[Trk::phi0];
   msBag.fmstrackpt	= trackMS->measuredPerigee()->pT()/GeV;
   msBag.fmstracketa	= trackMS->measuredPerigee()->eta();
 
   if(trackmsok && (*vxI) ) {
     perigee = m_trackToVertexTool->perigeeAtVertex((*trackMS), (*vxI)->recVertex().position());
    }
   if(trackmsok && perigee) {
      const Trk::ErrorMatrix errormat = perigee->localErrorMatrix();
      msBag.fmsd0_exPV	= perigee->parameters()[Trk::d0];
      msBag.fmssigd0_exPV	= errormat.error(Trk::d0) == 0 ? 0 : perigee->parameters()[Trk::d0]/errormat.error(Trk::d0);
      msBag.fmsz0_exPV	= perigee->parameters()[Trk::z0];
      msBag.fmssigz0_exPV	= errormat.error(Trk::z0) == 0 ? 0 : perigee->parameters()[Trk::z0]/errormat.error(Trk::z0);
      msBag.fmsphi_exPV	= perigee->parameters()[Trk::phi0];
      msBag.fmstheta_exPV	= perigee->parameters()[Trk::theta];
      msBag.fmsqoverp_exPV	= perigee->parameters()[Trk::qOverP]*GeV;
    }
   if (perigee) delete perigee; perigee = NULL;

   curmuon->MStrack().Set(msBag);
   curmuon->MStrack().Lock();
 
}

void ZeeDReadMuonAOD::ReadIDtrack (const Rec::TrackParticle* trackID, const Analysis::Muon* iMuon)
{
 
   ZeeDMuonBags::IDtrack idBag;
   const Trk::MeasuredPerigee* perigee(NULL);
   VxContainer::const_iterator vxI = vxContainer->begin();

   bool trackidok((trackID != NULL) &&  (trackID->measuredPerigee() != NULL) );
    if ( trackidok )
      {
	idBag.fhasInnerDetectorTrack = true;
	idBag.fidtracktheta	= trackID->measuredPerigee()->parameters()[Trk::theta];
	idBag.fidtrackphi	= trackID->measuredPerigee()->parameters()[Trk::phi0];
	idBag.fidtrackpt	= trackID->measuredPerigee()->pT()/GeV;
	idBag.fidtracketa	= trackID->measuredPerigee()->eta();
      }


   if(trackidok && (*vxI) ) {
      perigee = m_trackToVertexTool->perigeeAtVertex((*trackID), (*vxI)->recVertex().position());
    }

    if(trackidok && perigee) {
      const Trk::ErrorMatrix errormat = perigee->localErrorMatrix();
      idBag.fidd0_exPV	= perigee->parameters()[Trk::d0];
      idBag.fidsigd0_exPV	= errormat.error(Trk::d0) == 0 ? 0 : perigee->parameters()[Trk::d0]/errormat.error(Trk::d0);
      idBag.fidz0_exPV	= perigee->parameters()[Trk::z0];
      idBag.fidsigz0_exPV	= errormat.error(Trk::z0) == 0 ? 0 : perigee->parameters()[Trk::z0]/errormat.error(Trk::z0);
      idBag.fidphi_exPV	= perigee->parameters()[Trk::phi0];
      idBag.fidtheta_exPV	= perigee->parameters()[Trk::theta];
      idBag.fidqoverp_exPV	= perigee->parameters()[Trk::qOverP]*GeV;
    }

   if (perigee) delete perigee; perigee = NULL;
   idBag.fCaloEnergyLoss    = (iMuon)->energyLoss().first/GeV;

 
   curmuon->IDtrack().Set(idBag);
   curmuon->IDtrack().Lock();

}

 

void ZeeDReadMuonAOD::ReadTrigger(const Analysis::Muon* iMuon)
{

    fL1TrigChainNames = (*fAnaOptions)->L1TriggerElecMatch();
    fL2TrigChainNames = (*fAnaOptions)->L2TriggerElecMatch();
    fEFTrigChainNames = (*fAnaOptions)->EFTriggerElecMatch();
    ZeeDLeptonBags::Trigger TriggerBag;

    for(UInt_t i = 0; i != fL1TrigChainNames.size(); ++i) {
         const std::string chainName = fL1TrigChainNames[i];
         const TrigMatch::TrigMuonL1 *match = m_matchTool->matchToTriggerObject< TrigMatch::TrigMuonL1 >( iMuon, chainName,0.15,true);
         TriggerBag.isMatchedToChain.insert(std::make_pair(ZeeDEnum::Trigger::ToEnum(chainName), match));
         }

     for(UInt_t i = 0; i != fL2TrigChainNames.size(); ++i) {
          const std::string chainName = fL2TrigChainNames[i];
          const TrigMatch::TrigMuonL2 *match = m_matchTool->matchToTriggerObject< TrigMatch::TrigMuonL2 >( iMuon, chainName, 0.1, true);
          TriggerBag.isMatchedToChain.insert(std::make_pair(ZeeDEnum::Trigger::ToEnum(chainName), match));
          }

     for(UInt_t i = 0; i != fEFTrigChainNames.size(); ++i) {
           const std::string chainName = fEFTrigChainNames[i];
           const TrigMuonEFInfo *match = m_matchTool->matchToTriggerObject< TrigMuonEFInfo >( iMuon, chainName, 0.1, true );
           TriggerBag.isMatchedToChain.insert(std::make_pair(ZeeDEnum::Trigger::ToEnum(chainName), match));
           }

    curmuon->Trigger().Set(TriggerBag);
    curmuon->Trigger().Lock();
          
}

void ZeeDReadMuonAOD::ReadIsolation(const Analysis::Muon* iMuon)
{  
    ZeeDMuonBags::Isolation isoBag;
 
    isoBag.fptcone20 = (iMuon)->parameter(MuonParameters::ptcone20)/GeV;
    isoBag.fptcone30 = (iMuon)->parameter(MuonParameters::ptcone30)/GeV;
    isoBag.fptcone40 = (iMuon)->parameter(MuonParameters::ptcone40)/GeV;

   
    curmuon->Isolation().Set(isoBag);
    curmuon->Isolation().Lock();
}
