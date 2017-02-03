#include "ZeeDEvent/ZeeDJet.h"

// std libraries
#include <iostream>

// ZeeD libraries
#include "ZeeDMisc/ZeeDMisc.h"
#define DEBUG_ON 0

//------------------------------------------------------
// Constructor
ZeeDJet::ZeeDJet() : fFourVector(0., 0., 0., 0.), 
		     fFourVectorRaw(0., 0., 0., 0.), 
		     fFourVectorCS(0., 0., 0., 0.), 
		     fCollectionIndex(-1),
		     fEtaOrigin(0),
		     fPhiOrigin(0),
		     fMOrigin(0),
		     fEmFraction(0.0), 
		     fenergy_PreSamplerE(0.0),  
		     fenergy_PreSamplerB(0.0),
		     fenergy_EME1(0.0),
		     fenergy_EME2(0.0), 
		     fenergy_EME3(0.0),
		     fenergy_EMB1(0.0),
		     fenergy_EMB2(0.0), 
		     fenergy_EMB3(0.0),
		     fenergy_HEC0(0.0), 
		     fenergy_HEC1(0.0),
		     fenergy_HEC2(0.0),
		     fenergy_HEC3(0.0),
		     fenergy_FCAL0(0.0),
		     fenergy_FCAL1(0.0),
		     fenergy_FCAL2(0.0),
		     fenergy_TileBar0(0.0),
		     fenergy_TileBar1(0.0),
		     fenergy_TileBar2(0.0), 
		     fenergy_TileExt0(0.0),
		     fenergy_TileExt1(0.0), 
		     fenergy_TileExt2(0.0), 
		     fenergy_TileGap1(0.0),
		     fenergy_TileGap2(0.0),
		     fenergy_TileGap3(0.0),
		     fIsBadLooseMinus(0),
		     fIsBadLoose(0),
		     fIsBadMedium(0),
		     fIsBadTight(0),
		     fOriginIndex(0.0),  
		     fNTrk(0.0),  
		     fJVF(0.0), 
		     fSumPtTrk(0.0), 
		     fWIDTH(0.0),
		     fNumTowers(0.0),
		     fEtaOriginEM(0.0),
                     fPhiOriginEM(0.0),
                     fMOriginEM(0.0),
                     fWidthFraction(0.0),
                     fGSCFactorF(0.0),
                     fEMJESnooffset(0.0),
                     fCONST(0.0),
                     fActiveArea(0.0),
                     fActiveAreaPx(0.0),
                     fActiveAreaPy(0.0),
                     fActiveAreaPz(0.0),
                     fActiveAreaE(0.0),
                     fLowEtConstituentsFrac(0.0),
                     fnTrk_pv0_500MeV(0.0),
                     fsumPtTrk_pv0_500MeV(0.0),
                     fnTrk_pv0_1GeV(0.0),
                     fsumPtTrk_pv0_1GeV(0.0),
                     ftrackWIDTH_pv0_1GeV(0.0),
                     fnTrk_allpv_1GeV(0.0),
                     fsumPtTrk_allpv_1GeV(0.0),
                     ftrackWIDTH_allpv_1GeV(0.0),
		     fpt_truth(0.0),
		     fLikeLihood_0(0.0),
		     fCentroid_r(0.0),
		     fKtDr(0.0),
		     fIsoKR20Perp(0.0),
		     fIsoKR20Par(0.0),
		     fIsoKR20SumPt(0.0),
		     fIsoDelta2Perp(0.0),
		     fIsoDelta2Par(0.0),
		     fIsoDelta2SumPt(0.0),
		     fIsoFixedCone8Perp(0.0),
		     fIsoFixedCone8Par(0.0),
		     fIsoFixedCone8SumPt(0.0),
		     fIsoFixedArea13Perp(0.0),
		     fIsoFixedArea13Par(0.0),
		     fIsoFixedArea13SumPt(0.0),
		     fIso6To88Perp(0.0),
		     fIso6To88Par(0.0),
		     fIso6To88SumPt(0.0),
		     fBCH_CORR_CELL(0.0),
		     fBCH_CORR_DOTX(0.0),
		     fBCH_CORR_JET(0.0),
		     fTruthMFindex(0.0),
		     fTruthMF(0.0),
		     fn90(0.0),
		     footFracClusters5(0.0),
		     footFracClusters10(0.0),
		     fLArQuality(0.0),
		     fHECQuality(0.0),
		     fN_BAD_CELLS(0.0), 
		     fENG_BAD_CELLS(0.0),
		     fQGLabel(0),
		     fJetNumCDecay(0),
		     fJetNameCDecay(0),
		     fIsBadMediumBCH(kFALSE),
		     fIsBadTightBCH(kFALSE),
		     fnSegments(0)

{}

//------------------------------------------------------
// ZeeDJet::ZeeDJet(const ZeeDJet& jet) : TObject(jet)
// {
//     // Copy constructor
//     fEmFraction = jet.fEmFraction;
//     fFourVector = jet.fFourVector;
// }

//------------------------------------------------------
ZeeDJet::~ZeeDJet()
{
    // Destructor
}

//------------------------------------------------------
void ZeeDJet::Print(Option_t* option) const
{
    std::cout <<"----------------------------------------------------"<< std::endl;
    // Prints jet content
    if ( !option || !option[0] ) {
        std::cout << option << std::endl;
    }

    ZeeDMisc::Dump4Vec("jet", fFourVector);
    ZeeDMisc::Dump4Vec("jetRAW", fFourVectorRaw);
    ZeeDMisc::Dump4Vec("jetCS", fFourVectorCS);
    std::cout <<"----------------------------------------------------"<< std::endl;
}

