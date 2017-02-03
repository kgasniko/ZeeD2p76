#include "ZeeDHistManager/ZeeDHistManagerBkgDscrZCF.h"

//#define USE_DEBUG_MESSAGES
#include "ZeeDMisc/ZeeDMisc.h"

#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDVertex.h"
#include "ZeeDEvent/ZeeDEtMiss.h"
#include <algorithm>

//-----------------------------------------------------
ZeeDHistManagerBkgDscrZCF::ZeeDHistManagerBkgDscrZCF(TString name, bool doBS)
: ZeeDHistManager(name),
fdoBS(doBS),
fAnaOptions(fSvcHelper.GetAnaSvc())
{}

//-----------------------------------------------------
void ZeeDHistManagerBkgDscrZCF::BookHistos(){
  DEBUG_MES_START;
  
  const TString BosY	  = ZeeDMisc::FindFile("Binning/BosY.txt");
  const TString BosPt	  = ZeeDMisc::FindFile("Binning/BosPt.txt");
  const TString BosMass     = ZeeDMisc::FindFile("Binning/BosMass.txt");
  double ZMassCutVeryLooseMin = (*fAnaOptions)->ZMassCutVeryLooseMin();
  double ZMassCutVeryLooseMax = (*fAnaOptions)->ZMassCutVeryLooseMax();
  
  //control boson
  AddTH1D("BosMass", 250, 0, 250, "M_{ee} / GeV", "");
  AddTH2D("BosMassY", 250, 0, 250, 15, 1, 4, "M_{ee} / GeV", "y_{Z}");

  AddTH1D("BosMassF", BosMass, "M_{ee} / GeV", "");
  AddTH1D("BosMassB", BosMass, "M_{ee} / GeV", "");
  
  AddTH1D("BosMassVeryLoose",  (ZMassCutVeryLooseMax-ZMassCutVeryLooseMin)*2, ZMassCutVeryLooseMin, ZMassCutVeryLooseMax, "M_{Z->ee} / GeV",	 "");
  AddTH1D("BosY",    BosY,   "y (Z->ee)",	 "");
  AddTH1D("BosCosThStarBD", 20, -1, 1, "Cos(Th*)_{BD}", "");
  AddTH1D("BosCosThStarCS", 20, -1, 1, "Cos(Th*)_{CS}", "");
  AddTH1D("BosPt",   BosPt,  "P_{T} (Z->ee) / GeV", "");
  
  AddTH1D("Elec12_deltaPhi_abs", 100, 0, TMath::Pi(), "|#phi|", "");
  AddTH2D("Elec12_deltaPhi_absY", 100, 0, TMath::Pi(), 15, 1, 4, "|#phi|", "y_{Z}");

  //control electron
  AddTH1D("ElecPtC", 200, 0, 100, "p_{T,e^{central}}", "");
  AddTH1D("ElecPtF", 200, 0, 100, "p_{T,e^{forward}}", "");
  
  AddTH1D("ElecEtaFine",       200,  -5.0,  5.0, "#eta^{elec}", 	  "" );
  AddTH1D("ElecPhi",	    50, -200., 200., "#varphi_{elec} / deg",  "" );
  
  //control event
  AddTH1D("Mu"        ,        101,    -0.5,  100.5, "Mu"	 , "Average Interactions Per Crossing");
  AddTH1D("Nvtx"      , 	70,	0.5,   70.5, "N_{vtx}",      "");
  AddTH1D("VtxXPos",	   50,    -1.,    1., "X_{vtx} / mm",	  "");
  AddTH1D("VtxYPos",	   50,     0.,    2., "Y_{vtx} / mm",	  "");
  AddTH1D("VtxZPos",	   30,  -300.,  300., "Z_{vtx} / mm",	  "");
  AddTH1D("VertexTracks",  50,     0.,  150., "# vertex tracks", "");
  
  AddTH2D("ElecPtCY", 200, 0, 100, 20, 0, 4, "p_{T,e^{central}}", "y_{Z}");
  AddTH2D("ElecPtFY", 200, 0, 100, 20, 0, 4, "p_{T,e^{forward}}", "y_{Z}");
  
  AddTH1D("PtCone20overEt", 200,-0.5,1.," PtCone20 / Et cluster",""); //check new cut
  
  AddTH1D("ElecEtCone20", 300,-1.,2.," Et cone / Et cluster",""); //default
  AddTH1D("ElecEtCone30", 300,-1.,2.," Et cone / Et cluster","");

  AddTH1D("ElecEtCone20_EMEC", 300,-1.,2.," Et cone / Et cluster",""); //Emec
  AddTH1D("ElecEtCone30_EMEC", 300,-1.,2.," Et cone / Et cluster","");
  
  AddTH1D("ElecEtCone20_FCal", 300,-1.,2.," Et cone / Et cluster",""); //Fcal
  AddTH1D("ElecEtCone30_FCal", 300,-1.,2.," Et cone / Et cluster","");
  
  AddTH1D("ElecEtCone20_EMEC_2", 300,-1.,2.," Et cone / Et cluster",""); //Emec +200
  AddTH1D("ElecEtCone30_EMEC_2", 300,-1.,2.," Et cone / Et cluster","");
  
  AddTH1D("ElecEtCone20_FCal_2", 300,-1.,2.," Et cone / Et cluster",""); //Fcal +200
  AddTH1D("ElecEtCone30_FCal_2", 300,-1.,2.," Et cone / Et cluster","");

  //AddTH1D("ElecEtCone20_EMEC_3", 300,-1.,2.," Et cone / Et cluster",""); //Emec +300
  //AddTH1D("ElecEtCone30_EMEC_3", 300,-1.,2.," Et cone / Et cluster","");
  
  //AddTH1D("ElecEtCone20_FCal_3", 300,-1.,2.," Et cone / Et cluster",""); //Fcal +300
  //AddTH1D("ElecEtCone30_FCal_3", 300,-1.,2.," Et cone / Et cluster","");
  
  
  AddTH1D("ElecEtCone20_m1", 300,-1.,2.," Et cone / Et cluster",""); //-0.1
  AddTH1D("ElecEtCone30_m1", 300,-1.,2.," Et cone / Et cluster","");
  
  AddTH1D("ElecEtCone20_0", 300,-1.,2.," Et cone / Et cluster",""); //0
  AddTH1D("ElecEtCone30_0", 300,-1.,2.," Et cone / Et cluster","");
  
  AddTH1D("ElecEtCone20_1", 300,-1.,2.," Et cone / Et cluster",""); //0.1
  AddTH1D("ElecEtCone30_1", 300,-1.,2.," Et cone / Et cluster","");

  AddTH1D("ElecEtCone20_2", 300,-1.,2.," Et cone / Et cluster",""); //0.2
  AddTH1D("ElecEtCone30_2", 300,-1.,2.," Et cone / Et cluster","");
  
  AddTH1D("ElecEtCone20_3", 300,-1.,2.," Et cone / Et cluster",""); //0.3
  AddTH1D("ElecEtCone30_3", 300,-1.,2.," Et cone / Et cluster","");
  
  AddTH1D("ElecEtCone20_4", 300,-1.,2.," Et cone / Et cluster",""); //0.4
  AddTH1D("ElecEtCone30_4", 300,-1.,2.," Et cone / Et cluster","");
  
  AddTH1D("ElecEtCone20_5", 300,-1.,2.," Et cone / Et cluster",""); //0.5
  AddTH1D("ElecEtCone30_5", 300,-1.,2.," Et cone / Et cluster","");
 
  //AddTH1D("ElecEtCone30overEt", 4000,-1.,3.,"Et cone 30/ Et cluster","");
  
  AddTH2D("ElecEtCone20Y", 300,-1.,2., 15, 1, 4," Et cone / Et cluster", "y_{Z}");
  AddTH2D("ElecEtCone30Y", 300,-1.,2., 15, 1, 4," Et cone / Et cluster", "y_{Z}");
  

  AddTH1D("ElecEtCone20HM", 300,-1.,2.," Et cone / Et cluster","");
  AddTH1D("ElecEtCone30HM", 300,-1.,2.," Et cone / Et cluster","");
  
  AddTH2D("ElecEtCone20YHM", 300,-1.,2., 15, 1, 4," Et cone / Et cluster", "y_{Z}");
  AddTH2D("ElecEtCone30YHM", 300,-1.,2., 15, 1, 4," Et cone / Et cluster", "y_{Z}");
  
  
  double etaBin[10+1] = {2.50, 2.60, 2.70, 2.80, 2.90, 3.00, 3.16, 3.35, 3.60, 4.00, 4.90};
  AddTH2D("ElecEtCone30YE", 300,-1.,2., 10, etaBin," Et cone / Et cluster", "el_{fwd}");
  
  
  //AddTH1D("ElecPtCone20", 300,-1,2," Pt cone / Et cluster","");
  //AddTH1D("ElecPtCone30", 300,-1,2," Pt cone / Et cluster","");
  
  DEBUG_MES_END;
  
  if ( (*fAnaOptions)->genBootstrapRnd() && fdoBS ) {
    DoBootstrap("ElecEtCone20Y");
    DoBootstrap("ElecEtCone30Y");
  //DoBootstrap("BosMassY");
  }
  
}
  
  //-----------------------------------------------------
void ZeeDHistManagerBkgDscrZCF::Fill(){
  DEBUG_MES_START;
  DEBUG_MES(this->GetHMName());
  
  const ZeeDEvent* event = GetEvent();
  const Double_t Weight = event->GetWeight();
  
  const ZeeDBosonZ* boson = event->GetCurrentBoson();
  const ZeeDElectron* elec1 =(ZeeDElectron*) event->GetCurrentBoson()->GetCentralLep();
  const ZeeDElectron* elec2 =(ZeeDElectron*) event->GetCurrentBoson()->GetForwardLep();
  
  const TLorentzVector& boson_fourVec = boson->GetFourVector();
  const TLorentzVector& elec1_fourVec = elec1->GetFourVector();
  const TLorentzVector& elec2_fourVec = elec2->GetFourVector();
  
  const Double_t boson_M = boson_fourVec.M();
  const Double_t boson_Y = fabs(boson_fourVec.Rapidity());
  
  FillTH1(boson_M, Weight, "BosMass");
  FillTH2(boson_M, boson_Y, Weight, "BosMassY");
  
  const Double_t elec1_pt = elec1_fourVec.Pt();
  const Double_t elec2_pt = elec2_fourVec.Pt();
  const Double_t elec1_et = elec1_fourVec.Et();
 
  const Double_t elec2_eta = elec2_fourVec.Eta();
  
  FillTH1(boson_fourVec.M(),   Weight, "BosMassVeryLoose");
  FillTH1(boson_fourVec.Pt(),  Weight, "BosPt");
  FillTH1(boson_fourVec.Rapidity(), Weight, "BosY");
  FillTH1(elec1_pt, Weight, "ElecPtC");
  FillTH1(elec2_pt, Weight, "ElecPtF");

  FillTH1(elec1_fourVec.Phi()  * ZeeDMisc::Rad2Deg, Weight, "ElecPhi");
  FillTH1(elec2_fourVec.Phi()  * ZeeDMisc::Rad2Deg, Weight, "ElecPhi");
  FillTH1(elec1_fourVec.Eta(),  	 Weight, "ElecEtaFine");
  FillTH1(elec2_fourVec.Eta(),  	 Weight, "ElecEtaFine");
  
  if ( event->AverageInteractionsPerCrossing().IsSet() ) {
    const Double_t Mu = event->AverageInteractionsPerCrossing().Get();
    FillTH1(Mu, Weight, "Mu");
  }
  // Get array of vertices
  const TObjArray* vertices = event->GetVertices();
  
  if ( event->Nvtx().IsSet() ) {
      Int_t NumVtx = event->Nvtx().Get();
      FillTH1(NumVtx, Weight, "Nvtx");
  }
  
  if ( vertices->GetEntriesFast() > 0 ) {
      ZeeDVertex* vertex = static_cast< ZeeDVertex* >( vertices->At(0) );
      TVector3 vtx3Vector = vertex->GetVertexCandidate();
  
      // Fill vertex position
    FillTH1(vtx3Vector.X(), Weight, "VtxXPos");
    FillTH1(vtx3Vector.Y(), Weight, "VtxYPos");
    FillTH1(vtx3Vector.Z(), Weight, "VtxZPos");

    // Fill number of vertex tracks
    Int_t numTracks = vertex->GetNTrack();
    FillTH1(numTracks, Weight, "VertexTracks");

  }
  
  
  //if(boson->CosThStar(ZeeDEnum::CosThStar::BeamDirection).IsSet()) {
  
      //const Double_t cosThStarBD = boson->CosThStar(ZeeDEnum::CosThStar::BeamDirection).Get();
  const Double_t cosThStarBD = boson->GetCosThStar(ZeeDEnum::CosThStar::BeamDirection);
  const Double_t cosThStarCS  = boson->GetCosThStar(ZeeDEnum::CosThStar::CollinsSoper);
  
  FillTH1(cosThStarBD, Weight, "BosCosThStarBD");
  FillTH1(cosThStarCS, Weight, "BosCosThStarCS");
  
  //}
  FillTH2(elec1_pt, boson_Y, Weight, "ElecPtCY");
  FillTH2(elec2_pt, boson_Y, Weight, "ElecPtFY");
  
  if(cosThStarCS>0){
    FillTH1(boson_M, Weight, "BosMassF");
  }
  
  if(cosThStarCS<0){
    FillTH1(boson_M, Weight, "BosMassB");
  }
  
  
  //-------------------------------------------------------------------------
  const Double_t elec12_deltaPhi = elec1_fourVec.DeltaPhi(elec2_fourVec);
  FillTH1(TMath::Pi()-TMath::Abs(elec12_deltaPhi), Weight, "Elec12_deltaPhi_abs");
  FillTH2(TMath::Pi()-TMath::Abs(elec12_deltaPhi), boson_Y, Weight, "Elec12_deltaPhi_absY");
  
  if( elec1->Shower().IsSet() ) {
    const ZeeDElectronBags::Shower elec1_shower = elec1->Shower().Get();
  
    Double_t elec1_ptcone20 = elec1_shower.ptcone20/1000;  
    Double_t elec1_ptCone20overEt = elec1_ptcone20/elec1_et; 
  
    Double_t elec1_etcone30overpt = elec1_shower.etcone30/(elec1_et*1000);
    Double_t elec1_etcone20overpt = elec1_shower.etcone20/(elec1_et*1000);
  
    Double_t elec1_etcone30 = elec1_shower.etcone30/1000;
    Double_t elec1_etcone20 = elec1_shower.etcone20/1000;
    Double_t elec1_etcone30_m1 =(elec1_etcone30 - 0.1)/elec1_et;
    Double_t elec1_etcone20_m1 =(elec1_etcone20 - 0.1)/elec1_et;
    Double_t elec1_etcone30_0 =elec1_etcone30/elec1_pt;
    Double_t elec1_etcone20_0 =elec1_etcone20/elec1_pt;
    Double_t elec1_etcone30_1 =(elec1_etcone30 + 0.1)/elec1_et;
    Double_t elec1_etcone20_1 =(elec1_etcone20 + 0.1)/elec1_et;
    Double_t elec1_etcone30_2 =(elec1_etcone30 + 0.2)/elec1_et;
    Double_t elec1_etcone20_2 =(elec1_etcone20 + 0.2)/elec1_et;
    Double_t elec1_etcone30_3 =(elec1_etcone30 + 0.3)/elec1_et;
    Double_t elec1_etcone20_3 =(elec1_etcone20 + 0.3)/elec1_et;
    Double_t elec1_etcone30_4 =(elec1_etcone30 + 0.4)/elec1_et;
    Double_t elec1_etcone20_4 =(elec1_etcone20 + 0.4)/elec1_et;
    Double_t elec1_etcone30_5 =(elec1_etcone30 + 0.5)/elec1_et;
    Double_t elec1_etcone20_5 =(elec1_etcone20 + 0.5)/elec1_et;
    if( (*fAnaOptions)->IsMC() ){
      elec1_etcone30overpt = elec1_etcone30overpt + 0.0095;
      elec1_etcone20overpt = elec1_etcone20overpt + 0.0065;
      elec1_etcone30 += 0.2;
      elec1_etcone20 += 0.3;
    
    //const Double_t elec1_etcone30overEt   = (elec1_shower.etcone30+0.4)/(elec1_et*1000);
    //const Double_t elec2_etcone30overEt   = (elec2_shower.etcone30+0.4)/(elec2_et*1000);
    }
    Double_t elec1_etcone30overpt_2 = elec1_etcone30/elec1_et;
    Double_t elec1_etcone20overpt_2 = elec1_etcone20/elec1_et;
    
    if ((fabs(elec2_eta)) < 3.2){
      // std::cout << "EMEC elec: " << elec2_eta << std::endl;
      FillTH1(elec1_etcone30overpt, Weight, "ElecEtCone30_EMEC");
      FillTH1(elec1_etcone20overpt, Weight, "ElecEtCone20_EMEC");
  
      FillTH1(elec1_etcone30overpt_2, Weight, "ElecEtCone30_EMEC_2"); //+200
      FillTH1(elec1_etcone20overpt_2, Weight, "ElecEtCone20_EMEC_2");
    
    //  FillTH1(elec1_etcone30overpt_3, Weight, "ElecEtCone30_EMEC_3"); //+300
    //  FillTH1(elec1_etcone20overpt_3, Weight, "ElecEtCone20_EMEC_3");    
    }
  
    if ((fabs(elec2_eta)) > 3.2){
    //  std::cout << "FCal elec: " << elec2_eta << std::endl;
      FillTH1(elec1_etcone30overpt, Weight, "ElecEtCone30_FCal");
      FillTH1(elec1_etcone20overpt, Weight, "ElecEtCone20_FCal");
  
      FillTH1(elec1_etcone30overpt_2, Weight, "ElecEtCone30_FCal_2"); //+200
      FillTH1(elec1_etcone20overpt_2, Weight, "ElecEtCone20_FCal_2");
      
    //  FillTH1(elec1_etcone30overpt_3, Weight, "ElecEtCone30_FCal_3"); //+300
    //  FillTH1(elec1_etcone20overpt_3, Weight, "ElecEtCone20_FCal_3"); 
      
    }
  
    FillTH1(elec1_ptCone20overEt, Weight, "PtCone20overEt");  
    
    FillTH1(elec1_etcone30overpt, Weight, "ElecEtCone30");  //default
    FillTH2(elec1_etcone30overpt, boson_Y, Weight, "ElecEtCone30Y");

    FillTH1(elec1_etcone20overpt, Weight, "ElecEtCone20");
    FillTH2(elec1_etcone20overpt, boson_Y, Weight, "ElecEtCone20Y");
    //}
    
    FillTH1(elec1_etcone30_m1, Weight, "ElecEtCone30_m1");
    FillTH1(elec1_etcone20_m1, Weight, "ElecEtCone20_m1");
  
    FillTH1(elec1_etcone30_0, Weight, "ElecEtCone30_0");
    FillTH1(elec1_etcone20_0, Weight, "ElecEtCone20_0");
    
    FillTH1(elec1_etcone30_1, Weight, "ElecEtCone30_1");
    FillTH1(elec1_etcone20_1, Weight, "ElecEtCone20_1");
    
    FillTH1(elec1_etcone30_2, Weight, "ElecEtCone30_2");
    FillTH1(elec1_etcone20_2, Weight, "ElecEtCone20_2");
    
    FillTH1(elec1_etcone30_3, Weight, "ElecEtCone30_3");
    FillTH1(elec1_etcone20_3, Weight, "ElecEtCone20_3");
  
    FillTH1(elec1_etcone30_4, Weight, "ElecEtCone30_4");
    FillTH1(elec1_etcone20_4, Weight, "ElecEtCone20_4");
    
    FillTH1(elec1_etcone30_5, Weight, "ElecEtCone30_5");
    FillTH1(elec1_etcone20_5, Weight, "ElecEtCone20_5");
    
    if( boson_M>116 && boson_M<150 ){
      FillTH1(elec1_etcone30, Weight, "ElecEtCone30HM");
      FillTH2(elec1_etcone30, boson_Y, Weight, "ElecEtCone30YHM");
    
      FillTH1(elec1_etcone20, Weight, "ElecEtCone20HM");
      FillTH2(elec1_etcone20, boson_Y, Weight, "ElecEtCone20YHM");
    }
  
    if( boson_M>80 && boson_M<100 ){
      FillTH2(elec1_etcone30, elec2_fourVec.Eta(), Weight, "ElecEtCone30YE");
    }
  
  }
  
  // Get Miss Et
  const TObjArray* etmisarr = event->GetEtMissArray();

  if ( etmisarr == NULL) {
    return;
  }
  ZeeDEtMiss* etmiss = static_cast< ZeeDEtMiss* >( etmisarr->At(0) );
  
  if ( etmiss == NULL) {
  return;
}


DEBUG_MES_END;
}
