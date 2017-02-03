
#include "ZeeDHistManager/ZeeDHistManagerControlCF.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDEtMiss.h"

#include "ZeeDTools/ZeeDBags.h"
#include "ZeeDTools/ZeeDSystematics.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
//#include "egammaAnalysisUtils/ElectronEfficiencySFTool.h"

//#define USE_DEBUG_MESSAGES
//#define DEBUG_ON 1
#include "ZeeDMisc/ZeeDMisc.h"

using std::cout;
using std::endl;


//-----------------------------------------------------
void ZeeDHistManagerControlCF::BookHistos(){
  // Books histograms
  const TString BosY = ZeeDMisc::FindFile("Binning/BosY.txt");
  const TString BosAbsY = ZeeDMisc::FindFile("Binning/BosAbsY.txt");
  const TString BosPt = ZeeDMisc::FindFile("Binning/BosPt.txt");
  const TString BosYPt = ZeeDMisc::FindFile("Binning/BosYPt.txt");
  
  const TString BosYcosTh = ZeeDMisc::FindFile("Binning/BosYcosTh.txt");
  
  double ZMassCutVeryLooseMin = (*fAnaOptions)->ZMassCutVeryLooseMin();
  double ZMassCutVeryLooseMax = (*fAnaOptions)->ZMassCutVeryLooseMax();
  
  const double BosMassBins[3] = {66.0, 116.0, 150.0};
  const double BosMassFine[5] = {66.0, 80., 100., 116.0, 150.0};

  const double BosYBins[10] = {1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.8, 3.2, 3.6};
  const double BosPtBins[20] = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 36, 42, 48, 54, 60, 80, 100, 180, 350};
  
  AddTH1D("BosMass", 84, 66.0, 150.0, "M_{Z->ee} / GeV", "");
  AddTH1D("BosMassVeryLoose",  (ZMassCutVeryLooseMax-ZMassCutVeryLooseMin)*2, ZMassCutVeryLooseMin, ZMassCutVeryLooseMax, "M_{Z->ee} / GeV",	 "");
  AddTH1D("BosPt",   BosPt,  "P_{T} (Z->ee) / GeV", "");
  AddTH1D("BosY",    BosY,   "y (Z->ee)",	 "");
  AddTH1D("BosCosThStarBD", 20, -1, 1, "Cos(Th*)_{BD}", "");	    
  
  AddTH2D("BosPtM", 19, BosPtBins, 2, BosMassBins, "P_{T} (Z->ee) / GeV", "M_{Z}");
  AddTH2D("BosAbsYM", 9, BosYBins, 2,BosMassBins, "|y| (Z->ee)", "M_{Z}");

  AddTH2D("ClustEtaM", 50, -5.0, 5.0, 4, BosMassFine, "#eta^{clust} / GeV", "M_{Z}");
  AddTH2D("TrackPhiM", 50, -200., 200., 2, BosMassBins, "#varphi_{track} / deg", "M_{Z}" );
  AddTH2D("ElecPtCM", 80, 20.0, 100.0, 4, BosMassFine, "P_{T}^{elec} / GeV", "M_{Z}");
  AddTH2D("ElecPtFM", 80, 20.0, 100.0, 4, BosMassFine, "P_{T}^{elec} / GeV", "M_{Z}");
  
  AddTH2D("ClustEtaM1Y", 50, -5.0, 5.0, 9, BosYBins, "P_{T}^{clust} / GeV", "M_{Z}");
  AddTH2D("TrackPhiM1Y", 50, -200., 200., 9, BosYBins, "#varphi_{track} / deg", "M_{Z}" );
  AddTH2D("ElecPtCM1Y", 80, 20.0, 100.0, 9, BosYBins, "P_{T}^{elec} / GeV", "M_{Z}");
  AddTH2D("ElecPtFM1Y", 80, 20.0, 100.0, 9, BosYBins, "P_{T}^{elec} / GeV", "M_{Z}");
  
  AddTH2D("ClustEtaM2Y", 50, -5.0, 5.0, 9, BosYBins, "P_{T}^{clust} / GeV", "M_{Z}");
  AddTH2D("TrackPhiM2Y", 50, -200., 200., 9, BosYBins, "#varphi_{track} / deg", "M_{Z}" );
  AddTH2D("ElecPtCM2Y", 80, 20.0, 100.0, 9, BosYBins, "P_{T}^{elec} / GeV", "M_{Z}");
  AddTH2D("ElecPtFM2Y", 80, 20.0, 100.0, 9, BosYBins, "P_{T}^{elec} / GeV", "M_{Z}");
  
  std::string str = this->GetHMName().Data();
  bool isNoShift = (str.find("NoShift") > 1 && str.find("NoShift")<150);
  
  if(isNoShift){
    AddTH2D("EtMissM", 100, 0.0, 100.0, 4, BosMassFine, "E_{T} Miss", "M_{Z}");
    AddTH2D("MtMissM", 100, 0.0, 100.0, 4, BosMassFine, "M_{T} Miss", "M_{Z}");
  
    AddTH2D("EtMissMY", 100, 0.0, 100.0, 9, BosYBins, "E_{T} Miss", "M_{Z}");
    AddTH2D("MtMissMY", 100, 0.0, 100.0, 9, BosYBins, "M_{T} Miss", "M_{Z}" );
  }

}

//-----------------------------------------------------
void ZeeDHistManagerControlCF::Fill(){
  DEBUG_MES_START;
  DEBUG_MES(this->GetHMName());

  // Get event
  const ZeeDEvent* event = GetEvent();
  CHECK_NULL_PTR(event);

  // Event weight
  Double_t Weight = event->GetWeight();

  const ZeeDBosonZ* boson = event->GetCurrentBoson();

  if (boson == NULL) {
    return;
  }

  const TLorentzVector& fourVector = boson->GetFourVector();
  Double_t bosMass = fourVector.M();
  Double_t bosAbsY = fabs(fourVector.Rapidity());
  
  FillTH1(bosMass, Weight, "BosMass");
  FillTH1(bosMass,   Weight, "BosMassVeryLoose");
  FillTH1(fourVector.Pt(),  Weight, "BosPt");
  FillTH1(fourVector.Rapidity(), Weight, "BosY");
  
  //if(boson->CosThStar(ZeeDEnum::CosThStar::BeamDirection).IsSet()) {
  const Double_t cosThStarBD = boson->GetCosThStar(ZeeDEnum::CosThStar::BeamDirection);
  
  //	const Double_t cosThStarBD = boson->CosThStar(ZeeDEnum::CosThStar::BeamDirection).Get();
  FillTH1(cosThStarBD, Weight, "BosCosThStarBD");
  //}
  
  FillTH2(bosAbsY, bosMass, Weight, "BosAbsYM");
  FillTH2(fourVector.Pt(), bosMass, Weight, "BosPtM");
  
  const ZeeDElectron* eF =(ZeeDElectron*) boson->GetForwardLep();
  const ZeeDElectron* eC = (ZeeDElectron*)boson->GetCentralLep();
  
  FillTH2(eF->Cluster().Get().fourVector.Eta(), bosMass, Weight, "ClustEtaM");
  FillTH2(eC->Cluster().Get().fourVector.Eta(), bosMass, Weight, "ClustEtaM");

  FillTH2(eF->GetFourVector().Phi() * ZeeDMisc::Rad2Deg, bosMass, Weight, "TrackPhiM");
  FillTH2(eC->GetFourVector().Phi() * ZeeDMisc::Rad2Deg, bosMass, Weight, "TrackPhiM");
  
  FillTH2(eF->GetFourVector().Pt(), bosMass, Weight, "ElecPtFM");
  FillTH2(eC->GetFourVector().Pt(), bosMass, Weight, "ElecPtCM");
  
  
  if(bosMass>66.0 && bosMass<116.0 ){
    FillTH2(eF->Cluster().Get().fourVector.Eta(), bosAbsY, Weight, "ClustEtaM1Y");
    FillTH2(eC->Cluster().Get().fourVector.Eta(), bosAbsY, Weight, "ClustEtaM1Y");
   
    FillTH2(eF->GetFourVector().Phi() * ZeeDMisc::Rad2Deg, bosAbsY, Weight, "TrackPhiM1Y");
    FillTH2(eC->GetFourVector().Phi() * ZeeDMisc::Rad2Deg, bosAbsY, Weight, "TrackPhiM1Y");
  
    FillTH2(eF->GetFourVector().Pt(), bosAbsY, Weight, "ElecPtFM1Y");
    FillTH2(eC->GetFourVector().Pt(), bosAbsY, Weight, "ElecPtCM1Y");
  }
  
  
  if(bosMass>116.0 && bosMass<150.0 ){
    FillTH2(eF->Cluster().Get().fourVector.Eta(), bosAbsY, Weight, "ClustEtaM2Y");
    FillTH2(eC->Cluster().Get().fourVector.Eta(), bosAbsY, Weight, "ClustEtaM2Y");

    FillTH2(eF->GetFourVector().Phi() * ZeeDMisc::Rad2Deg, bosAbsY, Weight, "TrackPhiM2Y");
    FillTH2(eC->GetFourVector().Phi() * ZeeDMisc::Rad2Deg, bosAbsY, Weight, "TrackPhiM2Y");
  
    FillTH2(eF->GetFourVector().Pt(), bosAbsY, Weight, "ElecPtFM2Y");
    FillTH2(eC->GetFourVector().Pt(), bosAbsY, Weight, "ElecPtCM2Y");
  }
  
  // Fill MET and MMT
  
  const TObjArray* etmisarr = event->GetEtMissArray();
  if ( etmisarr == NULL) {
    return;
  }
  ZeeDEtMiss* etmiss = static_cast< ZeeDEtMiss* >( etmisarr->At(0) );
  
  if(event->GetCurrentBoson() == NULL){
    return;
  }
  
  const ZeeDElectron* electron = (ZeeDElectron*)event->GetCurrentBoson()->GetCentralLep();
  
  if ( electron == NULL) {
    return;
  }
  
  const TLorentzVector& elec = electron->GetFourVector();
  
  if ( etmiss == NULL) {
    return;
  }
  
  //Calculate MissingMt
  const double etMiss = etmiss->GetEt();
  const Double_t mtMiss = sqrt( 2 * elec.Pt() * etMiss * (1 - cos( elec.Phi() - etmiss->GetPhi()) ) );
  
  FillTH2(etMiss, bosMass, Weight, "EtMissM");
  FillTH2(mtMiss, bosMass, Weight, "MtMissM");
  
  if( bosMass>100.0 || bosMass<80.0 ){
    FillTH2(etMiss, bosAbsY, Weight, "EtMissMY");
    FillTH2(mtMiss, bosAbsY, Weight, "MtMissMY");
  }
  
  
}
  
  
  
