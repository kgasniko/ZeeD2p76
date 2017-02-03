#ifndef ZEEDROOTELECTRON_H
#define ZEEDROOTELECTRON_H

#include <TObject.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TString.h>
#include <map>

class ZeeDElectron;

#include "SGTools/CLASS_DEF.h"

/** Partner class of ZeeDElectron
    @author George Sedov
    @date Wed Feb 9 16:24:57 CEST 2011
*/
class ZeeDROOTElectron {

public:

    ZeeDROOTElectron();
    virtual ~ZeeDROOTElectron();

    Int_t       fillIn (const ZeeDElectron* const electron);
    Int_t       fillOut(ZeeDElectron* const electron) const;

private:
    // Fields that must be saved in ROOT. NO POINTERS ALLOWED
    //Fields of fShower
    Double_t         fShower_f1;
    Double_t         fShower_f1core;
    Double_t         fShower_e2tsts1;
    Double_t         fShower_emins1;
    Double_t         fShower_weta1;
    Double_t         fShower_weta2;
    Double_t         fShower_wtots1;
    Double_t         fShower_fracs1;
    Double_t         fShower_e233;
    Double_t         fShower_e237;
    Double_t         fShower_e277;
    Double_t         fShower_widths2;
    Double_t         fShower_f3core;
    Double_t         fShower_ptcone20;
    Double_t         fShower_ptcone30;
    Double_t         fShower_ptcone40;
    Double_t         fShower_etap;
    Double_t         fShower_etcone15;
    Double_t         fShower_etcone20;
    Double_t         fShower_etcone25;
    Double_t         fShower_etcone30;
    Double_t         fShower_etcone35;
    Double_t         fShower_etcone40;
    Bool_t           fShower_isSet;
    //Fields of fCluster
    Double_t         fCluster_etaBE2;
    Double_t         fCluster_energy;
    Double_t         fCluster_eT;
    Double_t         fCentralLambda;
    Double_t         fSecondLambda;
    Double_t         fSecondR;
    Double_t         flongitudinal;
    Double_t         flateral;
    Double_t         fFracMax;
    TLorentzVector   fCluster_fourVector;
    Bool_t           fCluster_isSet;
    // for the new em-calibration
    float            frawcl_Es0;
    float            frawcl_Es1;
    float            frawcl_Es2;
    float            frawcl_Es3;
    float            fcl_etaCalo;
    float            fcl_phiCalo;
    //Fields of fTrackMatch
    Double_t         fTrackMatch_deltaEta1;
    Double_t         fTrackMatch_deltaEta2;
    Double_t         fTrackMatch_deltaPhi2;
    Double_t         fTrackMatch_EoverP;
    Bool_t           fTrackMatch_expectHitInBLayer;
    Bool_t           fTrackMatch_isSet;
    //Fields of fTrackParticle
    Double_t         fTrackParticle_pt;
    Double_t         fTrackParticle_eta;
    TLorentzVector   fTrackParticle_fourVector;
    Bool_t           fTrackParticle_isSet;
    //Fields of fTrackSummary
    Int_t           fTrackSummary_numberOfBLayerHits;
    Int_t           fTrackSummary_numberOfPixelHits;
    Int_t           fTrackSummary_numberOfSCTHits;
    Int_t           fTrackSummary_numberOfBLayerOutliers;
    Int_t           fTrackSummary_numberOfPixelOutliers;
    Int_t           fTrackSummary_numberOfSCTOutliers;
    Int_t           fTrackSummary_numberOfCscEtaHits;
    Bool_t          fTrackSummary_isSet;
    //Fields of fMCTruth
    Int_t           fMCTruth_truthLeptonType;
    TLorentzVector  fMCTruth_fourVector;
    Bool_t          fMCTruth_isSet;
    //Fields of fVertex
    TVector3        fVertex_position;
    TVector3        fVertex_Err2;
    Bool_t          fVertex_isSet;
    //Fields of fAuthor
    Bool_t          fAuthor_isUnknown;
    Bool_t          fAuthor_isElectron;
    Bool_t          fAuthor_isSofte;
    Bool_t          fAuthor_isPhoton;
    Bool_t          fAuthor_isFrwd;
    Bool_t          fAuthor_isRConv;
    Int_t           fAuthor_bits;
    Bool_t          fAuthor_isSet;
    //Fields of fIsEM
    Bool_t          fIsEM_isLoose;
    Bool_t          fIsEM_isLoosePP;
    Bool_t          fIsEM_isMedium;
    Bool_t          fIsEM_isMediumPP;
    Bool_t          fIsEM_isTight;
    Bool_t          fIsEM_isTightPP;

    Bool_t          fIsEM_isFrwdLoose;
    Bool_t          fIsEM_isFrwdTight;

    UInt_t          fIsEM_bits;
    UInt_t          fIsEM_bits_Loose;
    UInt_t          fIsEM_bits_Medium;
    UInt_t          fIsEM_bits_Tight;
    Bool_t          fIsEM_isSet;
    //Fields of fTrigger
    typedef std::map<std::string, int> TriggerROOTMap;
    TriggerROOTMap  fTrigger_isMatchedToChain;
    Bool_t          fTrigger_isSet;


    Bool_t          fIsGoodOQAOD;
    Bool_t          fIsGoodOQAOD_isSet;
    Bool_t          fcaloIso98;
    Bool_t          fcaloIso95;
    Bool_t          fcaloIso_isSet;
    TLorentzVector  fFourVector;

    TLorentzVector  fCombinedFourVector;

    Double_t        fCharge;
    Bool_t          fCharge_isSet;

    Int_t           fCollectionIndex;

};

CLASS_DEF( ZeeDROOTElectron , 155096810 , 3 )

#endif
