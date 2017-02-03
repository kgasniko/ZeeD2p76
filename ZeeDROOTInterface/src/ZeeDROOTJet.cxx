#include "ZeeDROOTInterface/ZeeDROOTJet.h"
#include "ZeeDEvent/ZeeDJet.h"

ZeeDROOTJet::ZeeDROOTJet() :
    fFourVector(0,0,0,0),
    fFourVectorRaw(0,0,0,0),
    fFourVectorCS(0,0,0,0),
    fCollectionIndex(-1),
    fEtaOrigin(0),
    fPhiOrigin(0),
    fMOrigin(0),
    fEmFraction(0),
    fenergy_PreSamplerE(0.0), fenergy_PreSamplerB(0.0),
    fenergy_EME1(0.0), fenergy_EME2(0.0), fenergy_EME3(0.0), fenergy_EMB1(0.0), fenergy_EMB2(0.0), fenergy_EMB3(0.0),
    fenergy_HEC0(0.0), fenergy_HEC1(0.0), fenergy_HEC2(0.0), fenergy_HEC3(0.0),
    fenergy_FCAL0(0.0), fenergy_FCAL1(0.0), fenergy_FCAL2(0.0),
    fenergy_TileBar0(0.0), fenergy_TileBar1(0.0), fenergy_TileBar2(0.0), fenergy_TileExt0(0.0), fenergy_TileExt1(0.0), fenergy_TileExt2(0.0), fenergy_TileGap1(0.0), fenergy_TileGap2(0.0), fenergy_TileGap3(0.0),
    fIsBadLooseMinus(0),fIsBadLoose(0),fIsBadMedium(0),fIsBadTight(0),
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
    fQGLabel(0)


{
}

ZeeDROOTJet::~ZeeDROOTJet()
{
}

Int_t ZeeDROOTJet::fillIn (const ZeeDJet* const jet, const std::string& colName)
{
  fCollectionName             = colName; 
    // Fills ZeeDROOTJet with the information from ZeeDJet
  fFourVector                 = jet->GetFourVector();
  fFourVectorRaw              = jet->GetFourVectorRaw();
  fFourVectorCS               = jet->GetFourVectorCS();

  fCollectionIndex            = jet->GetCollectionIndex();

  fEtaOrigin                  = jet->GetEtaOrigin();
  fPhiOrigin                  = jet->GetPhiOrigin();
  fMOrigin                    = jet->GetMOrigin();

  fEmFraction                 = jet->GetEmFraction();

  fenergy_PreSamplerE         = jet->GetEnergy_PresamplerE();
  fenergy_PreSamplerB         = jet->GetEnergy_PresamplerB();
  fenergy_EME1                = jet->GetEnergy_EME1();
  fenergy_EME2                = jet->GetEnergy_EME2();
  fenergy_EME3                = jet->GetEnergy_EME3();
  fenergy_EMB1                = jet->GetEnergy_EMB1();
  fenergy_EMB2                = jet->GetEnergy_EMB2();
  fenergy_EMB3                = jet->GetEnergy_EMB3();
  fenergy_HEC0                = jet->GetEnergy_HEC0();
  fenergy_HEC1                = jet->GetEnergy_HEC1();
  fenergy_HEC2                = jet->GetEnergy_HEC2();
  fenergy_HEC3                = jet->GetEnergy_HEC3();
  fenergy_FCAL0               = jet->GetEnergy_FCAL0();
  fenergy_FCAL1               = jet->GetEnergy_FCAL1();
  fenergy_FCAL2               = jet->GetEnergy_FCAL2();
  fenergy_TileBar0            = jet->GetEnergy_TileBar0();
  fenergy_TileBar1            = jet->GetEnergy_TileBar1();
  fenergy_TileBar2            = jet->GetEnergy_TileBar2();
  fenergy_TileExt0            = jet->GetEnergy_TileExt0();
  fenergy_TileExt1            = jet->GetEnergy_TileExt1();
  fenergy_TileExt2            = jet->GetEnergy_TileExt2();
  fenergy_TileGap1            = jet->GetEnergy_TileGap1();
  fenergy_TileGap2            = jet->GetEnergy_TileGap2();
  fenergy_TileGap3            = jet->GetEnergy_TileGap3();

  fIsBadLooseMinus            = jet->GetIsBadLooseMinus();
  fIsBadLoose                 = jet->GetIsBadLoose();
  fIsBadMedium                = jet->GetIsBadMedium();
  fIsBadTight                 = jet->GetIsBadTight();

  fOriginIndex                = jet->GetOriginIndex();
  fNTrk                       = jet->GetNTrk();
  fJVF                        = jet->GetJVF();
  fSumPtTrk                   = jet->GetSumPtTrk();

  fWIDTH                      = jet->GetWIDTH();
  fNumTowers                  = jet->GetNumTowers();

  fEtaOriginEM                = jet->GetEtaOriginEM(); 
  fPhiOriginEM                = jet->GetPhiOriginEM(); 
  fMOriginEM                  = jet->GetMOriginEM() ;
  fWidthFraction              = jet->GetWidthFraction(); 
  fGSCFactorF                 = jet->GetGSCFactorF();
  fEMJESnooffset              = jet->GetEMJESnooffset();
  fCONST                      = jet->GetCONST();
  fActiveArea                 = jet->GetActiveArea();
  fActiveAreaPx               = jet->GetActiveAreaPx();
  fActiveAreaPy               = jet->GetActiveAreaPy();
  fActiveAreaPz               = jet->GetActiveAreaPz();
  fActiveAreaE                = jet->GetActiveAreaE();            
  fLowEtConstituentsFrac      = jet->GetLowEtConstituentsFrac();

  fnTrk_pv0_500MeV            = jet->GetnTrk_pv0_500MeV(); 
  fsumPtTrk_pv0_500MeV        = jet->GetsumPtTrk_pv0_500MeV(); 
  fnTrk_pv0_1GeV              = jet->GetnTrk_pv0_1GeV(); 
  fsumPtTrk_pv0_1GeV          = jet->GetsumPtTrk_pv0_1GeV();
  ftrackWIDTH_pv0_1GeV        = jet->GettrackWIDTH_pv0_1GeV(); 
  fnTrk_allpv_1GeV            = jet->GetnTrk_allpv_1GeV();
  fsumPtTrk_allpv_1GeV        = jet->GetsumPtTrk_allpv_1GeV(); 
  ftrackWIDTH_allpv_1GeV      = jet->GettrackWIDTH_allpv_1GeV();

  fpt_truth                   = jet->Getpt_truth();
  fLikeLihood_0               = jet->GetLikeLihood_0();
  fCentroid_r                 = jet->GetCentroid_r();
  fKtDr                       = jet->GetKtDr();

  fIsoKR20Perp                = jet->GetIsoKR20Perp();
  fIsoKR20Par                 = jet->GetIsoKR20Par();
  fIsoKR20SumPt               = jet->GetIsoKR20SumPt();
  fIsoDelta2Perp              = jet->GetIsoDelta2Perp();
  fIsoDelta2Par               = jet->GetIsoDelta2Par();
  fIsoDelta2SumPt             = jet->GetIsoDelta2SumPt();
  fIsoFixedCone8Perp          = jet->GetIsoFixedCone8Perp();
  fIsoFixedCone8Par           = jet->GetIsoFixedCone8Par();
  fIsoFixedCone8SumPt         = jet->GetIsoFixedCone8SumPt();
  fIsoFixedArea13Perp         = jet->GetIsoFixedArea13Perp();
  fIsoFixedArea13Par          = jet->GetIsoFixedArea13Par();
  fIsoFixedArea13SumPt        = jet->GetIsoFixedArea13SumPt();
  fIso6To88Perp               = jet->GetIso6To88Perp();
  fIso6To88Par                = jet->GetIso6To88Par();
  fIso6To88SumPt              = jet->GetIso6To88SumPt();

  fBCH_CORR_CELL              = jet->GetBCH_CORR_CELL();
  fBCH_CORR_DOTX              = jet->GetBCH_CORR_DOTX();
  fBCH_CORR_JET               = jet->GetBCH_CORR_JET();

  fTruthMFindex               = jet->GetTruthMFindex();
  fTruthMF                    = jet->GetTruthMF();

  fn90                        = jet->Getn90();
  footFracClusters5           = jet->GetootFracClusters5();
  footFracClusters10          = jet->GetootFracClusters10();
  fLArQuality                 = jet->GetLArQuality();
  fHECQuality                 = jet->GetHECQuality();  
  fN_BAD_CELLS                = jet->GetN_BAD_CELLS();
  fENG_BAD_CELLS              = jet->GetENG_BAD_CELLS();

  fQGLabel                    = jet->GetQGLabel();

  fJetConstituents            = jet->GetJetConstituents();

  return 0;
}

//------------------------------------------------------
std::string ZeeDROOTJet::fillOut(ZeeDJet* const jet) const
{
    // Fills ZeeDJet with the information from ZeeDROOTJet

  jet->SetFourVector   ( fFourVector );
  jet->SetFourVectorRaw( fFourVectorRaw );
  jet->SetFourVectorCS ( fFourVectorCS );

  jet->GetCollectionIndex()  = fCollectionIndex ;

  jet->SetEtaOrigin   ( fEtaOrigin );
  jet->SetPhiOrigin   ( fPhiOrigin );
  jet->SetMOrigin     ( fMOrigin );

  jet->SetEmFraction   ( fEmFraction );

  jet->SetEnergy_PresamplerE(fenergy_PreSamplerE );
  jet->SetEnergy_PresamplerB(fenergy_PreSamplerB );
  jet->SetEnergy_EME1       (fenergy_EME1        );
  jet->SetEnergy_EME2       (fenergy_EME2        );
  jet->SetEnergy_EME3       (fenergy_EME3        );
  jet->SetEnergy_EMB1       (fenergy_EMB1        );
  jet->SetEnergy_EMB2       (fenergy_EMB2        );
  jet->SetEnergy_EMB3       (fenergy_EMB3        );
  jet->SetEnergy_HEC0       (fenergy_HEC0        );
  jet->SetEnergy_HEC1       (fenergy_HEC1        );
  jet->SetEnergy_HEC2       (fenergy_HEC2        );
  jet->SetEnergy_HEC3       (fenergy_HEC3        );
  jet->SetEnergy_FCAL0      (fenergy_FCAL0       );
  jet->SetEnergy_FCAL1      (fenergy_FCAL1       );
  jet->SetEnergy_FCAL2      (fenergy_FCAL2       );
  jet->SetEnergy_TileBar0   (fenergy_TileBar0    );
  jet->SetEnergy_TileBar1   (fenergy_TileBar1    );
  jet->SetEnergy_TileBar2   (fenergy_TileBar2    );
  jet->SetEnergy_TileExt0   (fenergy_TileExt0    );
  jet->SetEnergy_TileExt1   (fenergy_TileExt1    );
  jet->SetEnergy_TileExt2   (fenergy_TileExt2    );
  jet->SetEnergy_TileGap1   (fenergy_TileGap1    );
  jet->SetEnergy_TileGap2   (fenergy_TileGap2    );
  jet->SetEnergy_TileGap3   (fenergy_TileGap3    );

  jet->SetIsBadLooseMinus   (fIsBadLooseMinus    );
  jet->SetIsBadLoose        (fIsBadLoose    );
  jet->SetIsBadMedium       (fIsBadMedium    );
  jet->SetIsBadTight        (fIsBadTight    );

  jet->SetOriginIndex  (fOriginIndex );
  jet->SetNTrk         (fNTrk        );
  jet->SetJVF          (fJVF         );
  jet->SetSumPtTrk     (fSumPtTrk    );
		  
  jet->SetWIDTH        (fWIDTH       );
  jet->SetNumTowers    (fNumTowers   );

  jet -> SetEtaOriginEM        ( fEtaOriginEM);
  jet -> SetPhiOriginEM        ( fPhiOriginEM );
  jet -> SetMOriginEM          ( fMOriginEM);
  jet -> SetWidthFraction      ( fWidthFraction );
  jet -> SetGSCFactorF         ( fGSCFactorF);
  jet -> SetEMJESnooffset      ( fEMJESnooffset );
  jet -> SetCONST              ( fCONST );
  jet -> SetActiveArea         ( fActiveArea );
  jet -> SetActiveAreaPx       ( fActiveAreaPx );
  jet -> SetActiveAreaPy       ( fActiveAreaPy );
  jet -> SetActiveAreaPz         ( fActiveAreaPz );
  jet -> SetActiveAreaE          ( fActiveAreaE );
  jet -> SetLowEtConstituentsFrac       ( fLowEtConstituentsFrac  );
  jet -> SetnTrk_pv0_500MeV             ( fnTrk_pv0_500MeV  );
  jet -> SetsumPtTrk_pv0_500MeV         ( fsumPtTrk_pv0_500MeV );
  jet -> SetnTrk_pv0_1GeV               ( fnTrk_pv0_1GeV );
  jet -> SetsumPtTrk_pv0_1GeV           ( fsumPtTrk_pv0_1GeV  );
  jet -> SettrackWIDTH_pv0_1GeV         ( ftrackWIDTH_pv0_1GeV );
  jet -> SetnTrk_allpv_1GeV             ( fnTrk_allpv_1GeV  );
  jet -> SetsumPtTrk_allpv_1GeV         ( fsumPtTrk_allpv_1GeV );
  jet -> SettrackWIDTH_allpv_1GeV       ( ftrackWIDTH_allpv_1GeV );

  jet -> Setpt_truth                    ( fpt_truth );
  jet -> SetLikeLihood_0                ( fLikeLihood_0 );
  jet -> SetCentroid_r                  ( fCentroid_r );
  jet -> SetKtDr                        ( fKtDr );

  jet -> SetIsoKR20Perp                 ( fIsoKR20Perp );
  jet -> SetIsoKR20Par                  ( fIsoKR20Par );
  jet -> SetIsoKR20SumPt                ( fIsoKR20SumPt );
  jet -> SetIsoDelta2Perp               ( fIsoDelta2Perp );
  jet -> SetIsoDelta2Par                ( fIsoDelta2Par );
  jet -> SetIsoDelta2SumPt              ( fIsoDelta2SumPt );
  jet -> SetIsoFixedCone8Perp           ( fIsoFixedCone8Perp );
  jet -> SetIsoFixedCone8Par            ( fIsoFixedCone8Par );
  jet -> SetIsoFixedCone8SumPt          ( fIsoFixedCone8SumPt );
  jet -> SetIsoFixedArea13Perp          ( fIsoFixedArea13Perp );
  jet -> SetIsoFixedArea13Par           ( fIsoFixedArea13Par );
  jet -> SetIsoFixedArea13SumPt         ( fIsoFixedArea13SumPt );
  jet -> SetIso6To88Perp                ( fIso6To88Perp );
  jet -> SetIso6To88Par                 ( fIso6To88Par );
  jet -> SetIso6To88SumPt               ( fIso6To88SumPt );

  jet -> SetBCH_CORR_CELL        ( fBCH_CORR_CELL );
  jet -> SetBCH_CORR_DOTX        ( fBCH_CORR_DOTX );
  jet -> SetBCH_CORR_JET         ( fBCH_CORR_JET );

  jet -> SetTruthMFindex         ( fTruthMFindex );
  jet -> SetTruthMF              ( fTruthMF );

  jet -> Setn90( fn90 );
  jet -> SetootFracClusters5( footFracClusters5 );
  jet -> SetootFracClusters10( footFracClusters10 );
  jet -> SetLArQuality( fLArQuality );
  jet -> SetHECQuality( fHECQuality );  
  jet -> SetN_BAD_CELLS( fN_BAD_CELLS );
  jet -> SetENG_BAD_CELLS( fENG_BAD_CELLS );

  jet -> SetQGLabel               ( fQGLabel );

  jet -> SetJetConstituents( fJetConstituents );

  return fCollectionName ;
}
