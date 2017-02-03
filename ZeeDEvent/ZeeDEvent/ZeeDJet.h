#ifndef __ZEEDJET_H
#define __ZEEDJET_H

////////////////////////////////////////////////////////
// Name    : ZeeDJet.h
////////////////////////////////////////////////////////
//
// Contact persons: Andrei Nikforov
//

// Root includes
#include <TLorentzVector.h>
#include <TObject.h>
#include <string>
#include <vector>
#include "ZeeDTools/ZeeDQuantity.h"

/** Base jet class */
class ZeeDJet : public TObject {

public:
    ZeeDJet();
    //    ZeeDJet(const ZeeDJet& jet);

    ~ZeeDJet();

    virtual Bool_t IsSortable()const {return true;};
    virtual Int_t Compare(const TObject* j) const
    {
      Double_t ptj = (static_cast<const ZeeDJet*>(j))->GetFourVector().Pt();
      Double_t pt = this->GetFourVector().Pt();
      
      if (pt < ptj)
        return 1;
      else if (pt > ptj)
        return -1;
      return 0;
    }


    // Getters
    /** Get jet four vector */
    const TLorentzVector& GetFourVector() const {
        return fFourVector;
    }
    const TLorentzVector& GetFourVectorRaw() const {
        return fFourVectorRaw;
    }
    const TLorentzVector& GetFourVectorCS() const {
        return fFourVectorCS;
    }

    const Int_t& GetCollectionIndex() const {
      return fCollectionIndex;
    }

    const Double_t& GetEtaOrigin() const {
      return fEtaOrigin;
    }
    const Double_t& GetPhiOrigin() const {
      return fPhiOrigin;
    }
    const Double_t& GetMOrigin() const {
        return fMOrigin;
    }

    

    /** Get jet em fraction */
    const Double_t& GetEmFraction() const {
        return fEmFraction;
    }

   
    const Float_t& GetEnergy_PresamplerE () const {
      return fenergy_PreSamplerE ;
    }
    const Float_t& GetEnergy_PresamplerB () const{
      return  fenergy_PreSamplerB;
    }
    const Float_t& GetEnergy_EME1 ()const{
      return fenergy_EME1;
    }
    const Float_t& GetEnergy_EME2 ()const {
      return  fenergy_EME2;
    }
    const Float_t& GetEnergy_EME3 () const {
      return fenergy_EME3;
    }
    const Float_t& GetEnergy_EMB1 () const {
      return fenergy_EMB1;
    }
    const Float_t& GetEnergy_EMB2 () const {
      return fenergy_EMB2;
    }
    const Float_t& GetEnergy_EMB3 () const {
      return fenergy_EMB3;
    }
    const Float_t& GetEnergy_HEC0 () const {
      return fenergy_HEC0;
    }
    const Float_t& GetEnergy_HEC1 () const {
      return fenergy_HEC1;
    }
    const Float_t& GetEnergy_HEC2 () const {
      return fenergy_HEC2;
    }
    const Float_t& GetEnergy_HEC3 () const {
      return fenergy_HEC3;
    }
    const Float_t& GetEnergy_FCAL0 () const {
      return fenergy_FCAL0;
    }
    const Float_t& GetEnergy_FCAL1 () const {
      return fenergy_FCAL1;
    }
    const Float_t& GetEnergy_FCAL2 () const {
      return fenergy_FCAL2;
    }
    const Float_t& GetEnergy_TileBar0 () const {
      return fenergy_TileBar0;
    }
    const Float_t& GetEnergy_TileBar1 () const {
      return fenergy_TileBar1;
    }
    const Float_t& GetEnergy_TileBar2 () const {
      return fenergy_TileBar2;
    }
    const Float_t& GetEnergy_TileExt0 () const {
      return fenergy_TileExt0;
    }
    const Float_t& GetEnergy_TileExt1 () const {
      return fenergy_TileExt1;
    }
    const Float_t& GetEnergy_TileExt2 () const {
      return fenergy_TileExt2;
    }
    const Float_t& GetEnergy_TileGap1 () const {
      return fenergy_TileGap1;
    }
    const Float_t& GetEnergy_TileGap2 () const {
      return fenergy_TileGap2;
    }
    const Float_t& GetEnergy_TileGap3 () const {
      return fenergy_TileGap3;
    }


    const Int_t& GetIsBadLooseMinus () const {
      return fIsBadLooseMinus;
    }
    const Int_t& GetIsBadLoose () const {
      return fIsBadLoose;
    }
    const Int_t& GetIsBadMedium () const {
      return fIsBadMedium;
    }
    const Int_t& GetIsBadTight () const {
      return fIsBadTight;
    }


    /** Get jet vertex fraction */
    const Double_t& GetJVF() const {
      return fJVF;
    }
    const Double_t& GetSumPtTrk() const {
      return fSumPtTrk;
    }
    const Double_t& GetOriginIndex() const {
      return fOriginIndex;
    }
    const Double_t& GetNTrk() const {
      return fNTrk;
    }


    /** Get jet width */
    const Double_t& GetWIDTH() const {
      return fWIDTH;
    }

    /** Get jet numtowers */
    const Double_t& GetNumTowers() const {
      return fNumTowers;
    }

    /** new moments */

    const Double_t& GetEtaOriginEM() const {
      return fEtaOriginEM;
    }
    const Double_t& GetPhiOriginEM() const {
      return fPhiOriginEM;
    }

    const Double_t& GetMOriginEM() const {
        return fMOriginEM;
    }

    const Double_t& GetWidthFraction() const {
        return fWidthFraction;
    }

    const Double_t& GetGSCFactorF() const {
        return fGSCFactorF;
    }

   const Double_t& GetEMJESnooffset() const {
        return fEMJESnooffset;
    }

    const Double_t& GetCONST() const {
        return fCONST;
    }

   const Double_t& GetActiveArea() const {
        return fActiveArea;
    }

   const Double_t& GetActiveAreaPx() const {
        return fActiveAreaPx;
    }
   const Double_t& GetActiveAreaPy() const {
        return fActiveAreaPy;
    }
   const Double_t& GetActiveAreaPz() const {
        return fActiveAreaPz;
    }
   const Double_t& GetActiveAreaE() const {
        return fActiveAreaE;
    }
            
   const Double_t& GetLowEtConstituentsFrac() const {
        return fLowEtConstituentsFrac;
    }

   const Double_t& GetnTrk_pv0_500MeV() const {
        return fnTrk_pv0_500MeV;
    }

   const Double_t& GetsumPtTrk_pv0_500MeV() const {
        return fsumPtTrk_pv0_500MeV;
    }

   const Double_t& GetnTrk_pv0_1GeV() const {
        return fnTrk_pv0_1GeV;
    }

   const Double_t& GetsumPtTrk_pv0_1GeV() const {
        return fsumPtTrk_pv0_1GeV;
    }

    const Double_t& GettrackWIDTH_pv0_1GeV() const {
        return ftrackWIDTH_pv0_1GeV;
    }

   const Double_t& GetnTrk_allpv_1GeV() const {
        return fnTrk_allpv_1GeV;
    }

   const Double_t& GetsumPtTrk_allpv_1GeV() const {
        return fsumPtTrk_allpv_1GeV;
    }

    const Double_t& GettrackWIDTH_allpv_1GeV() const {
        return ftrackWIDTH_allpv_1GeV;
    }

    const Double_t& Getpt_truth() const {
        return fpt_truth;
    }

    const Double_t& GetLikeLihood_0() const {
        return fLikeLihood_0;
    }

    const Double_t& GetCentroid_r() const {
      return fCentroid_r;
    }

    const Double_t& GetKtDr() const { 
      return fKtDr;
    }

    const Double_t& GetIsoKR20Perp() const { 
      return fIsoKR20Perp; 
    }

    const Double_t& GetIsoKR20Par() const { 
      return fIsoKR20Par; 
    }

    const Double_t& GetIsoKR20SumPt() const { 
      return fIsoKR20SumPt; 
    }

    const Double_t& GetIsoDelta2Perp() const { 
      return fIsoDelta2Perp; 
    }

    const Double_t& GetIsoDelta2Par() const {  
      return fIsoDelta2Par;
    }

    const Double_t& GetIsoDelta2SumPt() const { 
      return fIsoDelta2SumPt; 
    }

    const Double_t& GetIsoFixedCone8Perp() const { 
      return fIsoFixedCone8Perp; 
    }

    const Double_t& GetIsoFixedCone8Par() const { 
      return fIsoFixedCone8Par; 
    }

    const Double_t& GetIsoFixedCone8SumPt() const { 
      return fIsoFixedCone8SumPt; 
    }

    const Double_t& GetIsoFixedArea13Perp() const { 
      return fIsoFixedArea13Perp; 
    }

    const Double_t& GetIsoFixedArea13Par() const { 
      return fIsoFixedArea13Par; 
    }

    const Double_t& GetIsoFixedArea13SumPt() const {  
      return fIsoFixedArea13SumPt;
    }

    const Double_t& GetIso6To88Perp() const {  
      return fIso6To88Perp;
    }

    const Double_t& GetIso6To88Par() const { 
      return fIso6To88Par; 
    }

    const Double_t& GetIso6To88SumPt() const { 
      return fIso6To88SumPt; 
    }

    const Double_t& GetBCH_CORR_CELL() const { 
      return fBCH_CORR_CELL; 
    }

    const Double_t& GetBCH_CORR_DOTX() const {  
      return fBCH_CORR_DOTX;
    }

    const Double_t& GetBCH_CORR_JET() const {  
      return fBCH_CORR_JET;
    }
    

    const Double_t& GetTruthMFindex() const {  
      return fTruthMFindex;
    }

    const Double_t& GetTruthMF() const {  
      return fTruthMF;
    }

    const Double_t& Getn90() const {  
      return fn90;
    }

    const Double_t& GetootFracClusters5() const {  
      return footFracClusters5;
    }

    const Double_t& GetootFracClusters10() const {  
      return footFracClusters10;
    }

    const Double_t& GetLArQuality() const {  
      return fLArQuality;
    }

    const Double_t& GetHECQuality() const {  
      return fHECQuality;
    }

    const Double_t& GetN_BAD_CELLS() const {  
      return fN_BAD_CELLS;
    }

    const Double_t& GetENG_BAD_CELLS() const {  
      return fENG_BAD_CELLS;
    }

    /** Blue print */
    void Print(Option_t* option = "") const;

    int GetQGLabel() const {
      return fQGLabel;
    }

    int GetJetNumCDecay() const {
      return fJetNumCDecay;
    }
 
    TString GetJetNameCDecay() const {
      return fJetNameCDecay;
    }

    std::vector<TLorentzVector> GetJetConstituents() const {
      return fJetConstituents;
    }

    const Bool_t& GetIsBadMediumBCH() const {
      return fIsBadMediumBCH;
    }

    const Bool_t& GetIsBadTightBCH() const {
      return fIsBadTightBCH;
    }

    const Double_t& GetNSegments() const {
      return fnSegments;
    }


private:

    // Setters
    /** Set jet four vector */
    void SetFourVector(TLorentzVector fourVector) {
      fFourVector = fourVector;
    }
    void SetFourVectorCS(TLorentzVector fourVector) {
      fFourVectorCS = fourVector;
    }
    void SetFourVectorRaw(TLorentzVector fourVector) {
      fFourVectorRaw = fourVector;
    }
    Int_t& GetCollectionIndex() {
      return fCollectionIndex ;
    }

    void SetEtaOrigin( Double_t frac ) {
      fEtaOrigin = frac;
    }
    void SetPhiOrigin( Double_t frac ) {
      fPhiOrigin = frac;
    }
    void SetMOrigin( Double_t frac ) {
      fMOrigin = frac;
    }


    /** Set jet em fraction */
    void SetEmFraction( Double_t frac ) {
        fEmFraction = frac;
    }

    void SetEnergy_PresamplerE( Float_t frac ){
      fenergy_PreSamplerE  = frac;
      return;
    }
    void SetEnergy_PresamplerB( Float_t frac ){
      fenergy_PreSamplerB  = frac;
      return;
    }
    void SetEnergy_EME1( Float_t frac ){
      fenergy_EME1  = frac;
      return;
    }
    void SetEnergy_EME2( Float_t frac ){
      fenergy_EME2  = frac;
      return;
    }
    void SetEnergy_EME3( Float_t frac ){
      fenergy_EME3  = frac;
      return;
    }
    void SetEnergy_EMB1( Float_t frac ){
      fenergy_EMB1  = frac;
      return;
    }
    void SetEnergy_EMB2( Float_t frac ){
      fenergy_EMB2  = frac;
      return;
    }
    void SetEnergy_EMB3( Float_t frac ){
      fenergy_EMB3  = frac;
      return;
    }
    void SetEnergy_HEC0( Float_t frac ){
      fenergy_HEC0  = frac;
      return;
    }
    void SetEnergy_HEC1( Float_t frac ){
      fenergy_HEC1  = frac;
      return;
    }
    void SetEnergy_HEC2( Float_t frac ){
      fenergy_HEC2  = frac;
      return;
    }
    void SetEnergy_HEC3( Float_t frac ){
      fenergy_HEC3  = frac;
      return;
    }
    void SetEnergy_FCAL0( Float_t frac ){
      fenergy_FCAL0  = frac;
      return;
    }
    void SetEnergy_FCAL1( Float_t frac ){
      fenergy_FCAL1  = frac;
      return;
    }
    void SetEnergy_FCAL2( Float_t frac ){
      fenergy_FCAL2  = frac;
      return;
    }
    void SetEnergy_TileBar0( Float_t frac ){
      fenergy_TileBar0  = frac;
      return;
    }
    void SetEnergy_TileBar1( Float_t frac ){
      fenergy_TileBar1  = frac;
      return;
    }
    void SetEnergy_TileBar2( Float_t frac ){
      fenergy_TileBar2  = frac;
      return;
    }
    void SetEnergy_TileExt0( Float_t frac ){
      fenergy_TileExt0  = frac;
      return;
    }
    void SetEnergy_TileExt1( Float_t frac ){
      fenergy_TileExt1  = frac;
      return;
    }
    void SetEnergy_TileExt2( Float_t frac ){
      fenergy_TileExt2  = frac;
      return;
    }
    void SetEnergy_TileGap1( Float_t frac ){
      fenergy_TileGap1  = frac;
      return;
    }
    void SetEnergy_TileGap2( Float_t frac ){
      fenergy_TileGap2  = frac;
      return;
    }
    void SetEnergy_TileGap3( Float_t frac ){
      fenergy_TileGap3  = frac;
      return;
    }

    void SetIsBadLooseMinus( Int_t frac ){
      fIsBadLooseMinus  = frac;
      return;
    }
    void SetIsBadLoose( Int_t frac ){
      fIsBadLoose  = frac;
      return;
    }
    void SetIsBadMedium( Int_t frac ){
      fIsBadMedium  = frac;
      return;
    }
    void SetIsBadTight( Int_t frac ){
      fIsBadTight  = frac;
      return;
    }



   /** Set jet vertex fraction */
    void SetJVF( Double_t frac ) {
      fJVF = frac;
      return;
    }
    void SetSumPtTrk( Double_t frac ) {
      fSumPtTrk = frac;
      return;
    }
    void SetOriginIndex( Double_t frac ) {
      fOriginIndex = frac;
      return;
    }
    void SetNTrk( Double_t frac ) {
      fNTrk = frac;
      return;
    }



   /** Set jet width */
    void SetWIDTH( Double_t frac ) {
      fWIDTH = frac;
      return;
    }

    void SetNumTowers( Double_t frac ) {
      fNumTowers = frac;
      return;
    }


    /** new moments */

    void SetEtaOriginEM( Double_t frac ) {
     fEtaOriginEM = frac;
     return;
    }
     void SetPhiOriginEM( Double_t frac ) {
      fPhiOriginEM = frac;
     return;
    }

     void SetMOriginEM( Double_t frac ) {
       fMOriginEM = frac;
     return;
    }

     void SetWidthFraction( Double_t frac ){
       fWidthFraction = frac;
     return;
    }

     void SetGSCFactorF( Double_t frac ) {
       fGSCFactorF = frac;
     return;
    }

    void SetEMJESnooffset( Double_t frac ) {
     fEMJESnooffset = frac;
     return;
    }

     void SetCONST( Double_t frac ) {
       fCONST = frac;
     return;
    }

    void SetActiveArea( Double_t frac ) {
       fActiveArea = frac;
     return;
    }

    void SetActiveAreaPx( Double_t frac ) {
       fActiveAreaPx = frac;
     return;
    }
    void SetActiveAreaPy( Double_t frac ) {
        fActiveAreaPy = frac;
     return;
    }
    void SetActiveAreaPz( Double_t frac ) {
      fActiveAreaPz = frac;
     return;
    }
    void SetActiveAreaE( Double_t frac ) {
       fActiveAreaE = frac;
     return;
    }
            
    void SetLowEtConstituentsFrac( Double_t frac ) {
        fLowEtConstituentsFrac = frac;
     return;
    }

    void SetnTrk_pv0_500MeV( Double_t frac ) {
        fnTrk_pv0_500MeV = frac;
     return;
    }

    void SetsumPtTrk_pv0_500MeV( Double_t frac ) {
        fsumPtTrk_pv0_500MeV = frac;
     return;
    }

   void SetnTrk_pv0_1GeV( Double_t frac ) {
      fnTrk_pv0_1GeV = frac;
     return;
    }

   void SetsumPtTrk_pv0_1GeV( Double_t frac ) {
        fsumPtTrk_pv0_1GeV = frac;
     return;
    }

    void SettrackWIDTH_pv0_1GeV( Double_t frac ) {
       ftrackWIDTH_pv0_1GeV = frac;
     return;
    }

   void SetnTrk_allpv_1GeV( Double_t frac ) {
       fnTrk_allpv_1GeV = frac;
     return;
    }

   void SetsumPtTrk_allpv_1GeV( Double_t frac ) {
        fsumPtTrk_allpv_1GeV = frac;
     return;
    }

   void SettrackWIDTH_allpv_1GeV( Double_t frac ) {
        ftrackWIDTH_allpv_1GeV = frac;
     return;
    }
           

    void Setpt_truth( Double_t frac ){
         fpt_truth = frac;
      return;
    }

    void SetLikeLihood_0( Double_t frac ){
         fLikeLihood_0 = frac;
      return;
    }

    void SetCentroid_r( Double_t frac ) {
      fCentroid_r = frac;
      return;
    }

    void SetKtDr( Double_t frac ) { 
      fKtDr = frac;
      return;
    }

    void SetIsoKR20Perp( Double_t frac ) { 
      fIsoKR20Perp = frac; 
      return;
    }

    void SetIsoKR20Par( Double_t frac ) { 
      fIsoKR20Par = frac; 
      return;
    }

    void SetIsoKR20SumPt( Double_t frac ) { 
      fIsoKR20SumPt = frac; 
      return;
    }

    void SetIsoDelta2Perp( Double_t frac ) { 
      fIsoDelta2Perp = frac;
      return; 
    }

    void SetIsoDelta2Par( Double_t frac ) {  
      fIsoDelta2Par = frac;
      return;
    }

    void SetIsoDelta2SumPt( Double_t frac ) { 
      fIsoDelta2SumPt = frac; 
      return;
    }

    void SetIsoFixedCone8Perp( Double_t frac ) { 
        fIsoFixedCone8Perp = frac; 
      return;
    }

    void SetIsoFixedCone8Par( Double_t frac ) { 
        fIsoFixedCone8Par = frac; 
      return;
    }

    void SetIsoFixedCone8SumPt( Double_t frac ) { 
        fIsoFixedCone8SumPt = frac; 
      return;
    }

    void SetIsoFixedArea13Perp( Double_t frac ) { 
        fIsoFixedArea13Perp = frac; 
      return;
    }

    void SetIsoFixedArea13Par( Double_t frac ) { 
        fIsoFixedArea13Par = frac; 
      return;
    }

    void SetIsoFixedArea13SumPt( Double_t frac ) {  
        fIsoFixedArea13SumPt = frac;
      return;
    }

    void SetIso6To88Perp( Double_t frac ) {  
        fIso6To88Perp = frac;
      return;
    }

    void SetIso6To88Par( Double_t frac ) { 
        fIso6To88Par = frac; 
      return;
    }

    void SetIso6To88SumPt( Double_t frac ) { 
        fIso6To88SumPt = frac; 
      return;
    }

    void SetBCH_CORR_CELL( Double_t frac ) { 
        fBCH_CORR_CELL = frac; 
      return;
    }

    void SetBCH_CORR_DOTX( Double_t frac ) {  
        fBCH_CORR_DOTX = frac;
      return;
    }

    void SetBCH_CORR_JET( Double_t frac ) {  
        fBCH_CORR_JET = frac;
      return;
    }
    

    void SetTruthMFindex( Double_t frac ) {  
        fTruthMFindex = frac;
      return;
    }

    void SetTruthMF( Double_t frac ) {  
        fTruthMF = frac;
      return;
    }

    void Setn90( Double_t frac ) {  
        fn90 = frac;
      return;
    }

    void SetootFracClusters5( Double_t frac ) {  
        footFracClusters5 = frac;
      return;
    }

    void SetootFracClusters10( Double_t frac ) {  
        footFracClusters10 = frac;
      return;
    }

    void SetLArQuality( Double_t frac ) {  
        fLArQuality = frac;
      return;
    }

    void SetHECQuality( Double_t frac ) {  
        fHECQuality = frac;
      return;
    }

    void SetN_BAD_CELLS( Double_t frac ) { 
        fN_BAD_CELLS = frac; 
      return;
    }

    void SetENG_BAD_CELLS( Double_t frac ) {  
        fENG_BAD_CELLS = frac;
      return;
    }

    void SetQGLabel( int label )  {
         fQGLabel = label;
      return;
    }

    void SetJetNumCDecay( int num ) {
      fJetNumCDecay = num;
    }
 
    void SetJetNameCDecay( TString name ) {
      fJetNameCDecay = name;
    }


    void SetJetConstituents( std::vector<TLorentzVector> constituents ) {
      fJetConstituents = constituents ;
    }

   void SetIsBadMediumBCH( Bool_t flag ) {
      fIsBadMediumBCH = flag ;
    }

   void SetIsBadTightBCH( Bool_t flag ) {
      fIsBadTightBCH = flag ;
    }

   void SetNSegments(Double_t seg){
     fnSegments = seg;
   }

  /** Jet four-vector */
    
    TLorentzVector fFourVector, fFourVectorRaw, fFourVectorCS ;
    Int_t fCollectionIndex;
    Double_t fEtaOrigin, fPhiOrigin, fMOrigin; 

    /** Jet EM fraction */
    Double_t fEmFraction; 
    
    Float_t fenergy_PreSamplerE, fenergy_PreSamplerB;
    Float_t fenergy_EME1, fenergy_EME2, fenergy_EME3, fenergy_EMB1, fenergy_EMB2, fenergy_EMB3;
    Float_t fenergy_HEC0, fenergy_HEC1, fenergy_HEC2, fenergy_HEC3;
    Float_t fenergy_FCAL0, fenergy_FCAL1, fenergy_FCAL2;
    Float_t fenergy_TileBar0, fenergy_TileBar1, fenergy_TileBar2, fenergy_TileExt0, fenergy_TileExt1, fenergy_TileExt2, fenergy_TileGap1, fenergy_TileGap2, fenergy_TileGap3;

    /** quality flags**/

    Int_t fIsBadLooseMinus, fIsBadLoose, fIsBadMedium, fIsBadTight;

    /** Jet Vertex fraction */
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

    int  fQGLabel;
    int  fJetNumCDecay;
    TString fJetNameCDecay;

    Bool_t fIsBadMediumBCH;
    Bool_t fIsBadTightBCH;

    Double_t fnSegments;

    std::vector<TLorentzVector> fJetConstituents;

    friend class ZeeDReadEtMissAOD;
    friend class ZeeDReadJetsAOD;
    friend class ZeeDCalcJets;
    friend class ZeeDROOTJet;
};

#endif // ZeeDJet
