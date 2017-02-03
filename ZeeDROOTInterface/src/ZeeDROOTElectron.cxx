#include "ZeeDROOTInterface/ZeeDROOTElectron.h"
#include "ZeeDEvent/ZeeDElectron.h"

//#define USE_DEBUG_MESSAGES 1
//#define DEBUG 1
#include "ZeeDMisc/ZeeDMisc.h"

ZeeDROOTElectron::ZeeDROOTElectron() : fShower_f1(0),
    fShower_f1core(0),
    fShower_e2tsts1(0),
    fShower_emins1(0),
    fShower_weta1(0),
    fShower_weta2(0),
    fShower_wtots1(0),
    fShower_fracs1(0),
    fShower_e233(0),
    fShower_e237(0),
    fShower_e277(0),
    fShower_widths2(0),
    fShower_f3core(0),
    fShower_ptcone20(0),
    fShower_ptcone30(0),
    fShower_ptcone40(0),
    fShower_etap(0),
    fShower_etcone15(0),
    fShower_etcone20(0),
    fShower_etcone25(0),
    fShower_etcone30(0),
    fShower_etcone35(0),
    fShower_etcone40(0),
    fCluster_etaBE2(0),
    fCluster_energy(0),
    fCluster_eT(0),
    fCentralLambda(0),
    fSecondLambda(0),
    fSecondR(0),
    flongitudinal(0),
    flateral(0),
    fFracMax(0),
    frawcl_Es0(0),
    frawcl_Es1(0),
    frawcl_Es2(0),
    frawcl_Es3(0),
    fcl_etaCalo(0),
    fcl_phiCalo(0),
    fTrackMatch_deltaEta1(0),
    fTrackMatch_deltaEta2(0),
    fTrackMatch_deltaPhi2(0),
    fTrackMatch_EoverP(0),
    fTrackMatch_expectHitInBLayer(kFALSE),
    fTrackParticle_pt(0),
    fTrackParticle_eta(0),
    fTrackSummary_numberOfBLayerHits(0),
    fTrackSummary_numberOfPixelHits(0),
    fTrackSummary_numberOfSCTHits(0),
    fTrackSummary_numberOfBLayerOutliers(0),
    fTrackSummary_numberOfPixelOutliers(0),
    fTrackSummary_numberOfSCTOutliers(0),
    fTrackSummary_numberOfCscEtaHits(0),
    fMCTruth_truthLeptonType(0),
    fAuthor_isUnknown(kFALSE),
    fAuthor_isElectron(kFALSE),
    fAuthor_isSofte(kFALSE),
    fAuthor_isPhoton(kFALSE),
    fAuthor_isFrwd(kFALSE),
    fAuthor_isRConv(kFALSE),
    fAuthor_bits(0),

    fIsEM_isLoose(kFALSE),
    fIsEM_isLoosePP(kFALSE),
    fIsEM_isMedium(kFALSE),
    fIsEM_isMediumPP(kFALSE),
    fIsEM_isTight(kFALSE),
    fIsEM_isTightPP(kFALSE),

    fIsEM_isFrwdLoose(kFALSE),
    fIsEM_isFrwdTight(kFALSE),

    fIsEM_bits(0),
    fIsEM_bits_Loose(0),
    fIsEM_bits_Medium(0),
    fIsEM_bits_Tight(0),
    fIsGoodOQAOD(kFALSE),
    fcaloIso98(kFALSE),
    fcaloIso95(kFALSE),
    fCharge(0),
    fCollectionIndex(-1)
{

    DEBUG_MES_START;
    DEBUG_MES_END;

}

ZeeDROOTElectron::~ZeeDROOTElectron()
{

    DEBUG_MES_START;

    fTrigger_isMatchedToChain.clear();

    DEBUG_MES_END;

}

Int_t ZeeDROOTElectron::fillOut (ZeeDElectron* const electron) const
{
    // Fills ZeeDElectron with the information from ZeeDROOTElectron

    if(fShower_isSet) {

        ZeeDElectronBags::Shower showerBag;

        showerBag.f1       = fShower_f1;
        showerBag.f1core   = fShower_f1core;
        showerBag.e2tsts1  = fShower_e2tsts1;
        showerBag.emins1   = fShower_emins1;
        showerBag.weta1    = fShower_weta1;
        showerBag.weta2    = fShower_weta2;
        showerBag.wtots1   = fShower_wtots1;
        showerBag.fracs1   = fShower_fracs1;
        showerBag.e233     = fShower_e233;
        showerBag.e237     = fShower_e237;
        showerBag.e277     = fShower_e277;
        showerBag.widths2  = fShower_widths2;
        showerBag.f3core   = fShower_f3core;
	showerBag.ptcone20 = fShower_ptcone20;
        showerBag.ptcone30 = fShower_ptcone30;
        showerBag.ptcone40 = fShower_ptcone40;
        showerBag.etap     = fShower_etap;
        showerBag.etcone15 = fShower_etcone15;
        showerBag.etcone20 = fShower_etcone20;
        showerBag.etcone25 = fShower_etcone25;
        showerBag.etcone30 = fShower_etcone30;
        showerBag.etcone35 = fShower_etcone35;
        showerBag.etcone40 = fShower_etcone40;
        electron->fShower.Set(showerBag);

    }

    electron->fShower.Lock();

    if(fCluster_isSet) {

        ZeeDElectronBags::Cluster clusterBag;

        clusterBag.etaBE2        = fCluster_etaBE2;
        clusterBag.energy        = fCluster_energy;
        clusterBag.eT            = fCluster_eT;
        clusterBag.CentralLambda = fCentralLambda;
        clusterBag.SecondLambda  = fSecondLambda;
        clusterBag.SecondR       = fSecondR;
        clusterBag.longitudinal  = flongitudinal;
        clusterBag.lateral       = flateral;
        clusterBag.FracMax       = fFracMax;
        clusterBag.fourVector    = fCluster_fourVector;
        clusterBag.originalFourVector = fCluster_fourVector;
        clusterBag.rawcl_Es0     = frawcl_Es0;
        clusterBag.rawcl_Es1     = frawcl_Es1;
        clusterBag.rawcl_Es2     = frawcl_Es2;
        clusterBag.rawcl_Es3     = frawcl_Es3;
        clusterBag.cl_etaCalo    = fcl_etaCalo;
        clusterBag.cl_phiCalo    = fcl_phiCalo;

        electron->fCluster.Set(clusterBag);
    }

    electron->fCluster.Lock();

    if(fTrackMatch_isSet) {

        ZeeDElectronBags::TrackMatch trackMatchBag;

        trackMatchBag.deltaEta1            = fTrackMatch_deltaEta1;
        trackMatchBag.deltaEta2            = fTrackMatch_deltaEta2;
        trackMatchBag.deltaPhi2            = fTrackMatch_deltaPhi2;
        trackMatchBag.EoverP               = fTrackMatch_EoverP;
        trackMatchBag.expectHitInBLayer    = fTrackMatch_expectHitInBLayer;

        electron->fTrackMatch.Set(trackMatchBag);

    }

    electron->fTrackMatch.Lock();

    if(fTrackParticle_isSet) {

        ZeeDLeptonBags::TrackParticle trackParticleBag;

        trackParticleBag.pt                = fTrackParticle_pt;
        trackParticleBag.eta               = fTrackParticle_eta;
        trackParticleBag.fourVector        = fTrackParticle_fourVector;

        electron->fTrackParticle.Set(trackParticleBag);

    }

    electron->fTrackParticle.Lock();

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

        electron->fTrackSummary.Set(trackSummaryBag);

    }

    electron->fTrackSummary.Lock();

    if(fMCTruth_isSet) {

        ZeeDLeptonBags::MCTruth mcTruthBag;

        mcTruthBag.truthLeptonType       = fMCTruth_truthLeptonType;
        mcTruthBag.fourVector              = fMCTruth_fourVector;

        electron->fMCTruth.Set(mcTruthBag);

    }

    electron->fMCTruth.Lock();

    if(fVertex_isSet) {

        ZeeDLeptonBags::Vertex vertexBag;

        vertexBag.position                 = fVertex_position;
        vertexBag.Err2                     = fVertex_Err2;

        electron->fVertex.Set(vertexBag);

    }

    electron->fVertex.Lock();

    if(fAuthor_isSet) {

        ZeeDElectronBags::Author authorBag;

        authorBag.isUnknown                = fAuthor_isUnknown;
        authorBag.isElectron               = fAuthor_isElectron;
        authorBag.isSofte                  = fAuthor_isSofte;
        authorBag.isPhoton                 = fAuthor_isPhoton;
        authorBag.isFrwd                   = fAuthor_isFrwd;
        authorBag.isRConv                  = fAuthor_isRConv;
        authorBag.bits                     = fAuthor_bits;

        electron->fAuthor.Set(authorBag);

    }

    electron->fAuthor.Lock();

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

        electron->fIsEM.Set(isEMBag);

    }

    electron->fIsEM.Lock();

    if(fTrigger_isSet) {

        ZeeDLeptonBags::Trigger triggerBag;

        for(TriggerROOTMap::const_iterator it = fTrigger_isMatchedToChain.begin();
                it != fTrigger_isMatchedToChain.end(); ++it) {
            triggerBag.isMatchedToChain[ZeeDEnum::Trigger::ToEnum((it->first).c_str())] = it->second;
        }

        electron->fTrigger.Set(triggerBag);

    }

    electron->fTrigger.Lock();

    if(fIsGoodOQAOD_isSet) {

        electron->fIsGoodOQAOD.Set(fIsGoodOQAOD);

    }

    electron->fIsGoodOQAOD.Lock();

    if(fcaloIso_isSet) {

        ZeeDElectronBags::CaloIso caloIsoBag;

        caloIsoBag.CaloIso98               = fcaloIso98;
        caloIsoBag.CaloIso95               = fcaloIso95;

        electron->fcaloIso.Set(caloIsoBag);

    }

    electron->fcaloIso.Lock();

    electron->setHasCharge(fCharge_isSet);
    electron->setCharge(fCharge);

    electron->setFourVector(fFourVector);
    electron->fCombinedFourVector                    = fCombinedFourVector;
    electron->fCollectionIndex= fCollectionIndex;

    return 0;
}

Int_t ZeeDROOTElectron::fillIn(const ZeeDElectron* const electron)
{
    // Fills ZeeDROOTElectron with the information from ZeeDElectron

    fShower_isSet                    = electron->fShower.IsSet();

    if(fShower_isSet) {
        fShower_f1                       = electron->fShower.Get().f1;
        fShower_f1core                   = electron->fShower.Get().f1core;
        fShower_e2tsts1                  = electron->fShower.Get().e2tsts1;
        fShower_emins1                   = electron->fShower.Get().emins1;
        fShower_weta1                    = electron->fShower.Get().weta1;
        fShower_weta2                    = electron->fShower.Get().weta2;
        fShower_wtots1                   = electron->fShower.Get().wtots1;
        fShower_fracs1                   = electron->fShower.Get().fracs1;
        fShower_e233                     = electron->fShower.Get().e233;
        fShower_e237                     = electron->fShower.Get().e237;
        fShower_e277                     = electron->fShower.Get().e277;
        fShower_widths2                  = electron->fShower.Get().widths2;
        fShower_f3core                   = electron->fShower.Get().f3core;
        fShower_ptcone20                 = electron->fShower.Get().ptcone20;
        fShower_ptcone30                 = electron->fShower.Get().ptcone30;
        fShower_ptcone40                 = electron->fShower.Get().ptcone40;
        fShower_etap                     = electron->fShower.Get().etap;
        fShower_etcone15                 = electron->fShower.Get().etcone15;
        fShower_etcone20                 = electron->fShower.Get().etcone20;
        fShower_etcone25                 = electron->fShower.Get().etcone25;
        fShower_etcone30                 = electron->fShower.Get().etcone30;
        fShower_etcone35                 = electron->fShower.Get().etcone35;
        fShower_etcone40                 = electron->fShower.Get().etcone40;

    }

    fCluster_isSet                   = electron->fCluster.IsSet();

    if(fCluster_isSet) {
        fCluster_etaBE2                  = electron->fCluster.Get().etaBE2;
        fCluster_energy                  = electron->fCluster.Get().energy;
        fCluster_eT                      = electron->fCluster.Get().eT;
        fCentralLambda                   = electron->fCluster.Get().CentralLambda;
        fSecondLambda                    = electron->fCluster.Get().SecondLambda;
        fSecondR                         = electron->fCluster.Get().SecondR;
        flongitudinal                    = electron->fCluster.Get().longitudinal;
        flateral                         = electron->fCluster.Get().lateral;
        fFracMax                         = electron->fCluster.Get().FracMax;
        fCluster_fourVector              = electron->fCluster.Get().fourVector;
        frawcl_Es0                       = electron->fCluster.Get().rawcl_Es0;
        frawcl_Es1                       = electron->fCluster.Get().rawcl_Es1;
        frawcl_Es2                       = electron->fCluster.Get().rawcl_Es2;
        frawcl_Es3                       = electron->fCluster.Get().rawcl_Es3;
        fcl_etaCalo                      = electron->fCluster.Get().cl_etaCalo;
        fcl_phiCalo                      = electron->fCluster.Get().cl_phiCalo;
    }

    fTrackParticle_isSet             = electron->fTrackParticle.IsSet();

    if(fTrackParticle_isSet) {
        fTrackMatch_deltaEta1            = electron->fTrackMatch.Get().deltaEta1;
        fTrackMatch_deltaEta2            = electron->fTrackMatch.Get().deltaEta2;
        fTrackMatch_deltaPhi2            = electron->fTrackMatch.Get().deltaPhi2;
        fTrackMatch_EoverP               = electron->fTrackMatch.Get().EoverP;
        fTrackMatch_expectHitInBLayer    = electron->fTrackMatch.Get().expectHitInBLayer;
        fTrackMatch_isSet                = electron->fTrackMatch.IsSet();
        fTrackParticle_pt                = electron->fTrackParticle.Get().pt;
        fTrackParticle_eta               = electron->fTrackParticle.Get().eta;
        fTrackParticle_fourVector        = electron->fTrackParticle.Get().fourVector;
    }

    fTrackSummary_isSet              = electron->fTrackSummary.IsSet();

    if(fTrackSummary_isSet) {
        fTrackSummary_numberOfBLayerHits = electron->fTrackSummary.Get().numberOfBLayerHits;
        fTrackSummary_numberOfSCTHits    = electron->fTrackSummary.Get().numberOfSCTHits;
        fTrackSummary_numberOfPixelHits  = electron->fTrackSummary.Get().numberOfPixelHits;

        fTrackSummary_numberOfBLayerOutliers = electron->fTrackSummary.Get().numberOfBLayerOutliers;
        fTrackSummary_numberOfSCTOutliers    = electron->fTrackSummary.Get().numberOfSCTOutliers;
        fTrackSummary_numberOfPixelOutliers  = electron->fTrackSummary.Get().numberOfPixelOutliers;
        fTrackSummary_numberOfCscEtaHits     = electron->fTrackSummary.Get().numberOfCscEtaHits;
    }

    fMCTruth_isSet                   = electron->fMCTruth.IsSet();

    if(fMCTruth_isSet) {
        fMCTruth_truthLeptonType       = electron->fMCTruth.Get().truthLeptonType;
        fMCTruth_fourVector              = electron->fMCTruth.Get().fourVector;
    }

    fVertex_isSet                    = electron->fVertex.IsSet();

    if(fVertex_isSet) {
        fVertex_position                 = electron->fVertex.Get().position;
        fVertex_Err2                     = electron->fVertex.Get().Err2;
    }

    fAuthor_isSet                    = electron->fAuthor.IsSet();

    if(fAuthor_isSet) {
        fAuthor_isUnknown                = electron->fAuthor.Get().isUnknown;
        fAuthor_isElectron               = electron->fAuthor.Get().isElectron;
        fAuthor_isSofte                  = electron->fAuthor.Get().isSofte;
        fAuthor_isPhoton                 = electron->fAuthor.Get().isPhoton;
        fAuthor_isFrwd                   = electron->fAuthor.Get().isFrwd;
        fAuthor_isRConv                  = electron->fAuthor.Get().isRConv;
        fAuthor_bits                     = electron->fAuthor.Get().bits;
    }

    fIsEM_isSet                      = electron->fIsEM.IsSet();

    if(fIsEM_isSet) {

        fIsEM_isLoose                    = electron->fIsEM.Get().isLoose;
        fIsEM_isLoosePP                  = electron->fIsEM.Get().isLoosePP;
        fIsEM_isMedium                   = electron->fIsEM.Get().isMedium;
        fIsEM_isMediumPP                 = electron->fIsEM.Get().isMediumPP;
        fIsEM_isTight                    = electron->fIsEM.Get().isTight;
        fIsEM_isTightPP                  = electron->fIsEM.Get().isTightPP;

        fIsEM_isFrwdLoose                = electron->fIsEM.Get().isFrwdLoose;
        fIsEM_isFrwdTight                = electron->fIsEM.Get().isFrwdTight;

        fIsEM_bits                       = electron->fIsEM.Get().bits;
        fIsEM_bits_Loose                 = electron->fIsEM.Get().bits_Loose;
        fIsEM_bits_Medium                = electron->fIsEM.Get().bits_Medium;
        fIsEM_bits_Tight                 = electron->fIsEM.Get().bits_Tight;
    }

    fTrigger_isSet                   = electron->fTrigger.IsSet();

    if(fTrigger_isSet) {

        for(ZeeDLeptonBags::Trigger::TriggerMap::const_iterator it = electron->fTrigger.Get().isMatchedToChain.begin(),
                stop = electron->fTrigger.Get().isMatchedToChain.end();
            it != stop;
            ++it) {
            fTrigger_isMatchedToChain[ZeeDEnum::Trigger::ToString(it->first)] = it->second;
        }

    }

    fIsGoodOQAOD_isSet               = electron->fIsGoodOQAOD.IsSet();

    if(fIsGoodOQAOD_isSet) {
        fIsGoodOQAOD                     = electron->fIsGoodOQAOD.Get();
    }

    fcaloIso_isSet                   = electron->fcaloIso.IsSet();

    if(fcaloIso_isSet) {
        fcaloIso98                       = electron->fcaloIso.Get().CaloIso98;
        fcaloIso95                       = electron->fcaloIso.Get().CaloIso95;
    }

    fFourVector                      = electron->GetFourVector();
    fCombinedFourVector              = electron->fCombinedFourVector;
    
    fCollectionIndex                 = electron->fCollectionIndex;

    fCharge_isSet                    = electron->getHasCharge();
    fCharge                          = electron->getCharge();

    return 0;
}

