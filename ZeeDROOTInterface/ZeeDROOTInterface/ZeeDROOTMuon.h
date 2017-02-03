/*
 * ZeeDROOTMuon.h
 *
 *  Created on: Aug 10, 2012
 *      Author: morse
 */

#ifndef ZEEDROOTMUON_H_
#define ZEEDROOTMUON_H_

#include <TObject.h>
#include <TLorentzVector.h>

#include "SGTools/CLASS_DEF.h"

class ZeeDMuon;

class ZeeDROOTMuon {
public:
	ZeeDROOTMuon();
	virtual ~ZeeDROOTMuon();

    Int_t       fillIn (const ZeeDMuon* const muon);
    Int_t       fillOut(ZeeDMuon* const muon) const;

private:
    Int_t fnCSCEtaHits;
    Int_t fnSCTHits;
    Int_t fnPixHits;
    TLorentzVector	ffourVector;
    TLorentzVector      fcombinedMuonTrackParticle;
    Double_t            fmucharge;
    Int_t                 fauthor;
    Int_t                 fnprecisionLayers;
    Int_t                 fnprecisionHoleLayers;
    Double_t               fmomentumBalanceSignificance;
    Int_t                 fisCaloMuonId;
    Int_t                 fisCombinedMuon;
    Int_t                 floose;
    Int_t                 fmedium;
    Int_t                 ftight;
    Int_t                 fisStandAloneMuon;
    Int_t                 fisSegmentTaggedMuon;
    Int_t                 fisSiliconAssociatedForwardMuon;
    Bool_t                fistrack;
    Double_t               ftracktheta;
    Double_t               ftrackphi;
    Double_t               ftrackpt;
    Double_t               ftracketa;
    Double_t               fd0_exPV;
    Double_t               fsigd0_exPV;
    Double_t               fz0_exPV;
    Double_t               fsigz0_exPV;
    Double_t               fphi_exPV;
    Double_t               ftheta_exPV;
    Double_t               fqoverp_exPV;
    Double_t               fptcone20;
    Double_t               fptcone30;
    Double_t               fptcone40;
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
 
    Bool_t          fhasMuonSpectrometerTrack;
    Double_t		fmstracktheta;
    Double_t		fmstrackphi;
    Double_t		fmstrackpt;
    Double_t		fmstracketa;
    Double_t		fmsd0_exPV;
    Double_t		fmssigd0_exPV;
    Double_t		fmsz0_exPV;
    Double_t		fmssigz0_exPV;
    Double_t		fmsphi_exPV;
    Double_t		fmstheta_exPV;
    Double_t		fmsqoverp_exPV;

    Bool_t                fhasInnerDetectorTrack;
    Double_t		fidtracktheta;
    Double_t		fidtrackphi;
    Double_t		fidtrackpt;
    Double_t		fidtracketa;
    Double_t		fidd0_exPV;
    Double_t		fidsigd0_exPV;
    Double_t		fidz0_exPV;
    Double_t		fidsigz0_exPV;
    Double_t		fidphi_exPV;
    Double_t		fidtheta_exPV;
    Double_t		fidqoverp_exPV;

    Double_t               fCaloEnergyLoss;
    Bool_t                fProperties_isSet;
    Bool_t                fCombTrack_isSet;
    Bool_t                fIsolation_isSet;
    Bool_t                fMStrack_isSet;
    Bool_t                fIDtrack_isSet;

    TLorentzVector  fFourVector;

    TLorentzVector  fCombinedFourVector;

    Double_t        fCharge;
    Bool_t          fCharge_isSet;


    Int_t           fCollectionIndex;

    typedef std::map<std::string, Int_t> TriggerROOTMap;
    TriggerROOTMap  fTrigger_isMatchedToChain;
    Bool_t          fTrigger_isSet;


};

CLASS_DEF( ZeeDROOTMuon , 227935401 , 1 )

#endif /* ZEEDROOTMUON_H_ */
