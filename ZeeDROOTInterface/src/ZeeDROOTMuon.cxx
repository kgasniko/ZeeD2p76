/*
 * ZeeDROOTMuon.cxx
 *
 *  Created on: Aug 10, 2012
 *      Author: morse
 */

#include "ZeeDROOTInterface/ZeeDROOTMuon.h"
#include "ZeeDEvent/ZeeDMuon.h"

ZeeDROOTMuon::ZeeDROOTMuon() :
  ffourVector(0,0,0,0),
  fcombinedMuonTrackParticle(0,0,0,0),
  fauthor(0),
  fnprecisionLayers(0),
  fnprecisionHoleLayers(0),
  fmomentumBalanceSignificance(0),
  fisCaloMuonId(0),
  fisCombinedMuon(0),
  floose(0),
  fmedium(0),
  ftight(0),
  fisStandAloneMuon(0),
  fisSegmentTaggedMuon(0),
  fisSiliconAssociatedForwardMuon(0),
  fistrack(0),
  ftracktheta(0),
  ftrackphi(0),
  ftrackpt(0),
  ftracketa(0),
  fd0_exPV(0),
  fsigd0_exPV(0),
  fz0_exPV(0),
  fsigz0_exPV(0),
  fphi_exPV(0),
  ftheta_exPV(0),
  fqoverp_exPV(0),
  fptcone20(0),
  fptcone30(0),
  fptcone40(0),
  fmstracktheta(0.0),
  fmstrackphi(0.0),
  fmstrackpt(0.0),
  fmstracketa(0.0),
  fmsd0_exPV(0.0),
  fmssigd0_exPV(0.0),
  fmsz0_exPV(0.0),
  fmssigz0_exPV(0.0),
  fmsphi_exPV(0.0),
  fmstheta_exPV(0.0),
  fmsqoverp_exPV(0.0),
  fhasInnerDetectorTrack(false),
  fidtracktheta(0.0),
  fidtrackphi(0.0),
  fidtrackpt(0.0),
  fidtracketa(0.0),
  fidd0_exPV(0.0),
  fidsigd0_exPV(0.0),
  fidz0_exPV(0.0),
  fidsigz0_exPV(0.0),
  fidphi_exPV(0.0),
  fidtheta_exPV(0.0),
  fidqoverp_exPV(0.0),
  fTrackSummary_numberOfBLayerOutliers(0),
  fTrackSummary_numberOfPixelOutliers(0),
  fTrackSummary_numberOfSCTOutliers(0),
  fTrackSummary_numberOfCscEtaHits(0),
  fCharge(0.0),
  fCharge_isSet(false),
  fCaloEnergyLoss(0.0),
  fCollectionIndex(-1)
 {
	// TODO Auto-generated constructor stub

}

ZeeDROOTMuon::~ZeeDROOTMuon() {
	// TODO Auto-generated destructor stub
}

Int_t ZeeDROOTMuon::fillIn(const ZeeDMuon* const muon) {

  //std::cout <<__PRETTY_FUNCTION__<< "writing muon" << std::endl;
    fTrackParticle_isSet             = muon->fTrackParticle.IsSet();

    if(fTrackParticle_isSet) {
        fTrackParticle_pt                = muon->fTrackParticle.Get().pt;
        fTrackParticle_eta               = muon->fTrackParticle.Get().eta;
        fTrackParticle_fourVector        = muon->fTrackParticle.Get().fourVector;
    }

    fTrackSummary_isSet              = muon->fTrackSummary.IsSet();

    if(fTrackSummary_isSet) {
        fTrackSummary_numberOfBLayerHits = muon->fTrackSummary.Get().numberOfBLayerHits;
        fTrackSummary_numberOfSCTHits    = muon->fTrackSummary.Get().numberOfSCTHits;
        fTrackSummary_numberOfPixelHits  = muon->fTrackSummary.Get().numberOfPixelHits;

        fTrackSummary_numberOfBLayerOutliers = muon->fTrackSummary.Get().numberOfBLayerOutliers;
        fTrackSummary_numberOfSCTOutliers    = muon->fTrackSummary.Get().numberOfSCTOutliers;
        fTrackSummary_numberOfPixelOutliers  = muon->fTrackSummary.Get().numberOfPixelOutliers;
        fTrackSummary_numberOfCscEtaHits     = muon->fTrackSummary.Get().numberOfCscEtaHits;
    }

    fMCTruth_isSet                   = muon->fMCTruth.IsSet();

    if(fMCTruth_isSet) {
        fMCTruth_truthLeptonType       = muon->fMCTruth.Get().truthLeptonType;
        fMCTruth_fourVector              = muon->fMCTruth.Get().fourVector;
    }

    fVertex_isSet                    = muon->fVertex.IsSet();

    if(fVertex_isSet) {
        fVertex_position                 = muon->fVertex.Get().position;
        fVertex_Err2                     = muon->fVertex.Get().Err2;
    }


    fIsEM_isSet                      = muon->fIsEM.IsSet();

    if(fIsEM_isSet) {

        fIsEM_isLoose                    = muon->fIsEM.Get().isLoose;
        fIsEM_isLoosePP                  = muon->fIsEM.Get().isLoosePP;
        fIsEM_isMedium                   = muon->fIsEM.Get().isMedium;
        fIsEM_isMediumPP                 = muon->fIsEM.Get().isMediumPP;
        fIsEM_isTight                    = muon->fIsEM.Get().isTight;
        fIsEM_isTightPP                  = muon->fIsEM.Get().isTightPP;

        fIsEM_isFrwdLoose                = muon->fIsEM.Get().isFrwdLoose;
        fIsEM_isFrwdTight                = muon->fIsEM.Get().isFrwdTight;

        fIsEM_bits                       = muon->fIsEM.Get().bits;
        fIsEM_bits_Loose                 = muon->fIsEM.Get().bits_Loose;
        fIsEM_bits_Medium                = muon->fIsEM.Get().bits_Medium;
        fIsEM_bits_Tight                 = muon->fIsEM.Get().bits_Tight;
    }
 
  fProperties_isSet = muon->fProperties.IsSet();
  
  if(fProperties_isSet) {
	fisCaloMuonId = muon->fProperties.Get().fisCaloMuonId;
  	fisCombinedMuon = muon->fProperties.Get().fisCombinedMuon;
	fisStandAloneMuon = muon->fProperties.Get().fisStandAloneMuon;
  	fisSegmentTaggedMuon = muon->fProperties.Get().fisSegmentTaggedMuon;
  	fisSiliconAssociatedForwardMuon = muon->fProperties.Get().fisSiliconAssociatedForwardMuon;
 	fauthor = muon->fProperties.Get().fauthor;
  	fistrack = muon->fProperties.Get().fistrack;

      
  }

  fCombTrack_isSet = muon->fCombTrack.IsSet();
  
  if(fCombTrack_isSet) {
   	fnprecisionLayers = muon->fCombTrack.Get().fnprecisionLayers;
  	fnprecisionHoleLayers = muon->fCombTrack.Get().fnprecisionHoleLayers;
  	fmomentumBalanceSignificance = muon->fCombTrack.Get().fmomentumBalanceSignificance;
   	fcombinedMuonTrackParticle = muon->fCombTrack.Get().fcombinedMuonTrackParticle;
  	ftracktheta = muon->fCombTrack.Get().ftracktheta;
  	ftrackphi = muon->fCombTrack.Get().ftrackphi;
  	ftrackpt = muon->fCombTrack.Get().ftrackpt;
  	ftracketa = muon->fCombTrack.Get().ftracketa;
  	fd0_exPV = muon->fCombTrack.Get().fd0_exPV;
  	fsigd0_exPV = muon->fCombTrack.Get().fsigd0_exPV;
  	fz0_exPV = muon->fCombTrack.Get().fz0_exPV;
  	fsigz0_exPV = muon->fCombTrack.Get().fsigz0_exPV;
  	fphi_exPV = muon->fCombTrack.Get().fphi_exPV;
  	ftheta_exPV = muon->fCombTrack.Get().ftheta_exPV;
  	fqoverp_exPV = muon->fCombTrack.Get().fqoverp_exPV;

      
  }

   fIsolation_isSet = muon->fIsolation.IsSet();
  
  if(fIsolation_isSet) {
  	fptcone20 = muon->fIsolation.Get().fptcone20;
  	fptcone30 = muon->fIsolation.Get().fptcone30;
  	fptcone40 = muon->fIsolation.Get().fptcone40;

      
  }

  fMStrack_isSet = muon->fMStrack.IsSet();
  
  if(fMStrack_isSet) {
   	fhasMuonSpectrometerTrack = muon->fMStrack.Get().fhasMuonSpectrometerTrack;
  	fmstracktheta = muon->fMStrack.Get().fmstracktheta;
  	fmstrackphi = muon->fMStrack.Get().fmstrackphi;
  	fmstrackpt = muon->fMStrack.Get().fmstrackpt;
  	fmstracketa = muon->fMStrack.Get().fmstracketa;
  	fmsd0_exPV = muon->fMStrack.Get().fmsd0_exPV;
  	fmssigd0_exPV = muon->fMStrack.Get().fmssigd0_exPV;
  	fmsz0_exPV = muon->fMStrack.Get().fmsz0_exPV;
  	fmssigz0_exPV = muon->fMStrack.Get().fmssigz0_exPV;
  	fmsphi_exPV = muon->fMStrack.Get().fmsphi_exPV;
  	fmstheta_exPV = muon->fMStrack.Get().fmstheta_exPV;
  	fmsqoverp_exPV = muon->fMStrack.Get().fmsqoverp_exPV;

      
  }

  fIDtrack_isSet = muon->fIDtrack.IsSet();
  
  if(fIDtrack_isSet) {
   	fhasInnerDetectorTrack = muon->fIDtrack.Get().fhasInnerDetectorTrack;
  	fidtracktheta = muon->fIDtrack.Get().fidtracktheta;
  	fidtrackphi = muon->fIDtrack.Get().fidtrackphi;
  	fidtrackpt = muon->fIDtrack.Get().fidtrackpt;
 	fidtracketa = muon->fIDtrack.Get().fidtracketa;
  	fidd0_exPV = muon->fIDtrack.Get().fidd0_exPV;
  	fidsigd0_exPV = muon->fIDtrack.Get().fidsigd0_exPV;
  	fidz0_exPV = muon->fIDtrack.Get().fidz0_exPV;
  	fidsigz0_exPV = muon->fIDtrack.Get().fidsigz0_exPV;
  	fidphi_exPV = muon->fIDtrack.Get().fidphi_exPV;
  	fidtheta_exPV = muon->fIDtrack.Get().fidtheta_exPV;
  	fidqoverp_exPV = muon->fIDtrack.Get().fidqoverp_exPV;

  	fCaloEnergyLoss= muon->fIDtrack.Get().fCaloEnergyLoss;
  }


   fTrigger_isSet                   = muon->fTrigger.IsSet();
   if(fTrigger_isSet) {

        for(ZeeDLeptonBags::Trigger::TriggerMap::const_iterator it = muon->fTrigger.Get().isMatchedToChain.begin(),
                stop = muon->fTrigger.Get().isMatchedToChain.end();
            it != stop;
            ++it) {
            fTrigger_isMatchedToChain[ZeeDEnum::Trigger::ToString(it->first)] = it->second;
        }

    }

    fCombinedFourVector              = muon->fCombinedFourVector;

    fCharge_isSet                    = muon->getHasCharge();
    fCharge                          = muon->getCharge();


    fCollectionIndex            = muon->GetCollectionIndex();

    return 0;
}

Int_t ZeeDROOTMuon::fillOut(ZeeDMuon* const muon) const {
 


    muon->ffourVector = fTrackParticle_fourVector;

    if(fTrackParticle_isSet) {

        ZeeDLeptonBags::TrackParticle trackParticleBag;

        trackParticleBag.pt                = fTrackParticle_pt;
        trackParticleBag.eta               = fTrackParticle_eta;
        trackParticleBag.fourVector        = fTrackParticle_fourVector;
        muon->fTrackParticle.Set(trackParticleBag);

    }

    muon->fTrackParticle.Lock();

    if(fTrackSummary_isSet) {

        ZeeDLeptonBags::TrackSummary trackSummaryBag;

        trackSummaryBag.numberOfBLayerHits = fTrackSummary_numberOfBLayerHits;
        trackSummaryBag.numberOfPixelHits  = fTrackSummary_numberOfPixelHits;
        trackSummaryBag.numberOfSCTHits    = fTrackSummary_numberOfSCTHits;

        trackSummaryBag.numberOfBLayerOutliers = fTrackSummary_numberOfBLayerOutliers;
        trackSummaryBag.numberOfPixelOutliers  = fTrackSummary_numberOfPixelOutliers;
        trackSummaryBag.numberOfSCTOutliers    = fTrackSummary_numberOfSCTOutliers;
        trackSummaryBag.numberOfCscEtaHits     = fTrackSummary_numberOfCscEtaHits
;
        muon->fTrackSummary.Set(trackSummaryBag);

    }

    muon->fTrackSummary.Lock();

    if(fMCTruth_isSet) {

        ZeeDLeptonBags::MCTruth mcTruthBag;

        mcTruthBag.truthLeptonType       = fMCTruth_truthLeptonType;
        mcTruthBag.fourVector              = fMCTruth_fourVector;

        muon->fMCTruth.Set(mcTruthBag);

    }

    muon->fMCTruth.Lock();

    if(fVertex_isSet) {

        ZeeDLeptonBags::Vertex vertexBag;

        vertexBag.position                 = fVertex_position;
        vertexBag.Err2                     = fVertex_Err2;

        muon->fVertex.Set(vertexBag);

    }

    muon->fVertex.Lock();

    if(fIsEM_isSet) {

        ZeeDLeptonBags::IsEM isEMBag;

	isEMBag.isLoose                    = fIsEM_isLoose;
	isEMBag.isLoosePP                  = fIsEM_isLoosePP;
	isEMBag.isMedium                   = fIsEM_isMedium;
	isEMBag.isMediumPP                 = fIsEM_isMediumPP;
	isEMBag.isTight                    = fIsEM_isTight;
	isEMBag.isTightPP                  = fIsEM_isTightPP;

        isEMBag.isFrwdLoose                = fIsEM_isFrwdLoose;
        isEMBag.isFrwdTight                = fIsEM_isFrwdTight;

        isEMBag.bits                       = fIsEM_bits;
        isEMBag.bits_Loose                 = fIsEM_bits_Loose;
        isEMBag.bits_Medium                = fIsEM_bits_Medium;
        isEMBag.bits_Tight                 = fIsEM_bits_Tight;

        muon->fIsEM.Set(isEMBag);

    }

    muon->fIsEM.Lock();


    // Fills ZeeDMuon with the information from ZeeDROOTMuon
    
    if(fProperties_isSet) {
         ZeeDMuonBags::Properties   PropertiesBag;
         PropertiesBag.fisCaloMuonId = fisCaloMuonId;
         PropertiesBag.fisCombinedMuon = fisCombinedMuon;
         PropertiesBag.fauthor = fauthor;
  	 PropertiesBag.fisStandAloneMuon = fisStandAloneMuon;
  	 PropertiesBag.fisSegmentTaggedMuon = fisSegmentTaggedMuon;
 	 PropertiesBag.fistrack = fistrack;
   	 PropertiesBag.fisSiliconAssociatedForwardMuon = fisSiliconAssociatedForwardMuon;
  
         muon->fProperties.Set(PropertiesBag);
  }
 
    muon->fProperties.Lock();

    if(fCombTrack_isSet) {
       	ZeeDMuonBags::CombTrack   CombTrackBag;
     	CombTrackBag.fnprecisionLayers = fnprecisionLayers;
    	CombTrackBag.fcombinedMuonTrackParticle = fcombinedMuonTrackParticle;
    	CombTrackBag.fnprecisionHoleLayers = fnprecisionHoleLayers;
    	CombTrackBag.fmomentumBalanceSignificance = fmomentumBalanceSignificance;
        CombTrackBag.ftracktheta = ftracktheta;
  	CombTrackBag.ftrackphi = ftrackphi;
    	CombTrackBag.ftrackpt = ftrackpt;
   	CombTrackBag.ftracketa = ftracketa;
    	CombTrackBag.fd0_exPV = fd0_exPV;
    	CombTrackBag.fsigd0_exPV = fsigd0_exPV;
    	CombTrackBag.fz0_exPV = fz0_exPV;
    	CombTrackBag.fsigz0_exPV = fsigz0_exPV;
    	CombTrackBag.fphi_exPV = fphi_exPV;
    	CombTrackBag.ftheta_exPV = ftheta_exPV;
    	CombTrackBag.fqoverp_exPV = fqoverp_exPV;

       	muon->fCombTrack.Set(CombTrackBag);
     }
 
    muon->fCombTrack.Lock();

    if(fIsolation_isSet) {
       	ZeeDMuonBags::Isolation   IsolationBag;
     	IsolationBag.fptcone20 = fptcone20;
    	IsolationBag.fptcone30 = fptcone30;
    	IsolationBag.fptcone40 = fptcone40;

      	muon->fIsolation.Set(IsolationBag);
       	}
 
    muon->fIsolation.Lock();

    if(fMStrack_isSet) {
       	ZeeDMuonBags::MStrack   MStrackBag;
     	MStrackBag.fhasMuonSpectrometerTrack = fhasMuonSpectrometerTrack;
    	MStrackBag.fmstracktheta = fmstracktheta;
    	MStrackBag.fmstrackphi = fmstrackphi;
    	MStrackBag.fmstrackpt = fmstrackpt;
    	MStrackBag.fmstracketa = fmstracketa;
    	MStrackBag.fmsd0_exPV = fmsd0_exPV;
    	MStrackBag.fmssigd0_exPV = fmssigd0_exPV;
    	MStrackBag.fmsz0_exPV = fmsz0_exPV;
    	MStrackBag.fmssigz0_exPV = fmssigz0_exPV;
    	MStrackBag.fmsphi_exPV = fmsphi_exPV;
    	MStrackBag.fmstheta_exPV = fmstheta_exPV;
    	MStrackBag.fmsqoverp_exPV = fmsqoverp_exPV;

      	muon->fMStrack.Set(MStrackBag);
       	}
 
    muon->fMStrack.Lock();

    if(fIDtrack_isSet) {
       	ZeeDMuonBags::IDtrack   IDtrackBag;

     	IDtrackBag.fhasInnerDetectorTrack = fhasInnerDetectorTrack;
    	IDtrackBag.fidtracktheta = fidtracktheta;
    	IDtrackBag.fidtrackphi = fidtrackphi;
    	IDtrackBag.fidtrackpt = fidtrackpt;
    	IDtrackBag.fidtracketa = fidtracketa;
    	IDtrackBag.fidd0_exPV = fidd0_exPV;
    	IDtrackBag.fidsigd0_exPV = fidsigd0_exPV;
    	IDtrackBag.fidz0_exPV = fidz0_exPV;
    	IDtrackBag.fidsigz0_exPV = fidsigz0_exPV;
    	IDtrackBag.fidphi_exPV = fidphi_exPV;
    	IDtrackBag.fidtheta_exPV = fidtheta_exPV;
    	IDtrackBag.fidqoverp_exPV = fidqoverp_exPV;
    	IDtrackBag.fCaloEnergyLoss = fCaloEnergyLoss;

      	muon->fIDtrack.Set(IDtrackBag);
       	}
 
    muon->fIDtrack.Lock();
    
    if(fTrigger_isSet) {

        ZeeDLeptonBags::Trigger triggerBag;

        for(TriggerROOTMap::const_iterator it = fTrigger_isMatchedToChain.begin();
                it != fTrigger_isMatchedToChain.end(); ++it) {
            triggerBag.isMatchedToChain[ZeeDEnum::Trigger::ToEnum((it->first).c_str())] = it->second;
        }

        muon->fTrigger.Set(triggerBag);

    }

    muon->fTrigger.Lock();

    muon->setHasCharge(fCharge_isSet);
    muon->setCharge(fCharge);

    muon->fCombinedFourVector                    = fCombinedFourVector;
 

    muon->fCollectionIndex  = fCollectionIndex ;

    return 0;
}
