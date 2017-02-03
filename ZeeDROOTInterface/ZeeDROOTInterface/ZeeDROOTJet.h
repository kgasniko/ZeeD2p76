#ifndef ZEEDROOTJET_H
#define ZEEDROOTJET_H

#include <TObject.h>
#include <TLorentzVector.h>

#include "SGTools/CLASS_DEF.h"

class ZeeDJet;

/** Partner class of ZeeDJet
    @author George Sedov, Rohin Narayan
    @date Wed September 28 2011
*/
class ZeeDROOTJet {

public:

    ZeeDROOTJet();
    virtual ~ZeeDROOTJet();

    Int_t        fillIn (const ZeeDJet* const jet, const std::string& colName);
    std::string  fillOut(ZeeDJet* const jet) const;

private:
    // Fields that must be saved in ROOT. NO POINTERS ALLOWED
    TLorentzVector fFourVector;
    TLorentzVector fFourVectorRaw;
    TLorentzVector fFourVectorCS;
    Int_t           fCollectionIndex;
    Double_t fEtaOrigin;
    Double_t fPhiOrigin;
    Double_t fMOrigin;

    Double_t fEmFraction;

    Float_t fenergy_PreSamplerE, fenergy_PreSamplerB;
    Float_t fenergy_EME1, fenergy_EME2, fenergy_EME3, fenergy_EMB1, fenergy_EMB2, fenergy_EMB3;
    Float_t fenergy_HEC0, fenergy_HEC1, fenergy_HEC2, fenergy_HEC3;
    Float_t fenergy_FCAL0, fenergy_FCAL1, fenergy_FCAL2;
    Float_t fenergy_TileBar0, fenergy_TileBar1, fenergy_TileBar2, fenergy_TileExt0, fenergy_TileExt1, fenergy_TileExt2, fenergy_TileGap1, fenergy_TileGap2, fenergy_TileGap3;
    Int_t fIsBadLooseMinus,  fIsBadLoose,  fIsBadMedium,  fIsBadTight;

    Double_t fOriginIndex;
    Double_t fNTrk;
    Double_t fJVF;
    Double_t fSumPtTrk;     

    Double_t fWIDTH;
    Double_t fNumTowers; 

    Double_t fEtaOriginEM;
    Double_t fPhiOriginEM;
    Double_t fMOriginEM;
    Double_t fWidthFraction;
    Double_t fGSCFactorF;
    Double_t fEMJESnooffset;
    Double_t fCONST;
    Double_t fActiveArea;
    Double_t fActiveAreaPx;
    Double_t fActiveAreaPy;
    Double_t fActiveAreaPz;
    Double_t fActiveAreaE;
    Double_t fLowEtConstituentsFrac;
    Double_t fnTrk_pv0_500MeV;
    Double_t fsumPtTrk_pv0_500MeV;
    Double_t fnTrk_pv0_1GeV;
    Double_t fsumPtTrk_pv0_1GeV;
    Double_t ftrackWIDTH_pv0_1GeV;
    Double_t fnTrk_allpv_1GeV;
    Double_t fsumPtTrk_allpv_1GeV;
    Double_t ftrackWIDTH_allpv_1GeV;
    Double_t fpt_truth;
    Double_t fLikeLihood_0;
    Double_t fCentroid_r;
    Double_t fKtDr;
    Double_t fIsoKR20Perp;
    Double_t fIsoKR20Par;
    Double_t fIsoKR20SumPt;
    Double_t fIsoDelta2Perp;
    Double_t fIsoDelta2Par;
    Double_t fIsoDelta2SumPt;
    Double_t fIsoFixedCone8Perp;
    Double_t fIsoFixedCone8Par;
    Double_t fIsoFixedCone8SumPt;
    Double_t fIsoFixedArea13Perp;
    Double_t fIsoFixedArea13Par;
    Double_t fIsoFixedArea13SumPt;
    Double_t fIso6To88Perp;
    Double_t fIso6To88Par;
    Double_t fIso6To88SumPt;
    Double_t fBCH_CORR_CELL;
    Double_t fBCH_CORR_DOTX;
    Double_t fBCH_CORR_JET;
    Double_t fTruthMFindex;
    Double_t fTruthMF;

    Double_t fn90;
    Double_t footFracClusters5;
    Double_t footFracClusters10;
    Double_t fLArQuality;
    Double_t fHECQuality;
    Double_t fN_BAD_CELLS; 
    Double_t fENG_BAD_CELLS;

    std::string fCollectionName;

    int fQGLabel;

    std::vector<TLorentzVector> fJetConstituents;
};

CLASS_DEF( ZeeDROOTJet , 175855822 , 1 )

#endif
