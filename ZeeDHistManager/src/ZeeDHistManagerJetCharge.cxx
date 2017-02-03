#include "ZeeDHistManager/ZeeDHistManagerJetCharge.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDJet.h"
#include "ZeeDEvent/ZeeDTrack.h"
#include "ZeeDEvent/ZeeDGenParticle.h"
#include "ZeeDEvent/ZeeDVertex.h"

#include "ZeeDTools/ZeeDBags.h"
#include "ZeeDTools/ZeeDSystematics.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

//#define USE_DEBUG_MESSAGES
//#define DEBUG_ON 1
#include "ZeeDMisc/ZeeDMisc.h"

using std::cout;
using std::endl;

//-----------------------------------------------------
void  ZeeDHistManagerJetCharge::bookAnalysisCuts()
{
  //std::cout <<__PRETTY_FUNCTION__<< std::endl;

  
  bosonPtCut           =  20.0;
  deltaRcut = 3.0;

  deltaRCutMap.insert( std::pair<std::string,Double_t>("4" , 0.350) ) ;
  deltaRCutMap.insert( std::pair<std::string,Double_t>("5" , 0.425) ) ;
  deltaRCutMap.insert( std::pair<std::string,Double_t>("6" , 0.500) ) ;
  
  jetCone = 1.0;

  jetConeMap.insert( std::pair<std::string,Double_t>("4" , 0.4) ) ;
  jetConeMap.insert( std::pair<std::string,Double_t>("5" , 0.5) ) ;
  jetConeMap.insert( std::pair<std::string,Double_t>("6" , 0.6) ) ;

  return;
}

//-----------------------------------------------------
void ZeeDHistManagerJetCharge::BookHistos()
{
  bookAnalysisCuts();

  
  // Do we run on data or MC?
  if ( (*fAnaOptions)->IsMC() ) {
    bIsMC = kTRUE;
  }

  std::vector<std::string> collections = (*fAnaOptions)->JetCollectionNames();
  for (unsigned int i = 0; i < collections.size(); i++)
    {
      AddTH1D(collections[i] + "_Jets_BosPt", 1001, -0.5, 500.5, "P_{T} (Z->ee) / GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_Balance" , 100, 0, 50, "P_{T, jet}/P_{T, Z}*cos(#phi)","", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_deltaPhiJetBos" , 314, 0, 3.14, "#Delta#phi(Z, jet)","", ZeeDHistLevel::Analysis);

      AddTH1D(collections[i] + "_BosMass",      100, 46., 150, "M (Z->ee) / GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_BosMass_u",    100, 46., 150, "M (Z->ee) / GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_BosMass_ubar", 100, 46., 150, "M (Z->ee) / GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_BosMass_d",    100, 46., 150, "M (Z->ee) / GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_BosMass_dbar", 100, 46., 150, "M (Z->ee) / GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_BosMass_s",    100, 46., 150, "M (Z->ee) / GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_BosMass_sbar", 100, 46., 150, "M (Z->ee) / GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_BosMass_c",    100, 46., 150, "M (Z->ee) / GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_BosMass_cbar", 100, 46., 150, "M (Z->ee) / GeV", "", ZeeDHistLevel::Analysis);

      AddTH1D(collections[i] + "_PtJet" ,  1001, -0.5, 500.5,  "P_{T} (jet) / GeV", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_PtJet1",  1001, -0.5, 500.5,  "P_{T} (jet_{1}) / GeV", "", ZeeDHistLevel::Analysis);

      AddTH1D(collections[i] + "_SumQ_Jet",         21, -10.5, 10.5, "#sum q_{track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQ_Jet1",         21, -10.5, 10.5, "#sum q_{track}", "", ZeeDHistLevel::Analysis  );

      AddTH1D(collections[i] + "_SumQAbs_Jet",         11, -0.5, 10.5, "|#sum q_{track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQAbs_Jet1",         11, -0.5, 10.5, "|#sum q_{track}|", "", ZeeDHistLevel::Analysis  );

      AddTH1D(collections[i] + "_SumQPt_k02_Jet1",         23, -1.15, 1.15, "#sum q_{track}*p^{0.2}_{T,track}/#sum p^{0.2}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k04_Jet1",         23,-1.15, 1.15, "#sum q_{track}*p^{0.4}_{T,track}/#sum p^{0.2}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k06_Jet1",         23,-1.15, 1.15, "#sum q_{track}*p^{0.6}_{T,track}/#sum p^{0.2}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k08_Jet1",         23,-1.15, 1.15, "#sum q_{track}*p^{0.8}_{T,track}/#sum p^{0.2}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k10_Jet1",         23,-1.15, 1.15, "#sum q_{track}*p^{1.0}_{T,track}/#sum p^{1.0}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k12_Jet1",         23,-1.15, 1.15, "#sum q_{track}*p^{1.2}_{T,track}/#sum p^{1.2}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k14_Jet1",         23,-1.15, 1.15, "#sum q_{track}*p^{1.4}_{T,track}/#sum p^{1.4}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k16_Jet1",         23,-1.15, 1.15, "#sum q_{track}*p^{1.6}_{T,track}/#sum p^{1.6}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k18_Jet1",         23,-1.15, 1.15, "#sum q_{track}*p^{1.8}_{T,track}/#sum p^{1.8}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k20_Jet1",         23,-1.15, 1.15, "#sum q_{track}*p^{2.0}_{T,track}/#sum p^{2.0}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k30_Jet1",         23,-1.15, 1.15, "#sum q_{track}*p^{3.0}_{T,track}/#sum p^{3.0}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k40_Jet1",         23,-1.15, 1.15, "#sum q_{track}*p^{4.0}_{T,track}/#sum p^{4.0}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k50_Jet1",         23,-1.15, 1.15, "#sum q_{track}*p^{5.0}_{T,track}/#sum p^{5.0}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k60_Jet1",         23,-1.15, 1.15, "#sum q_{track}*p^{6.0}_{T,track}/#sum p^{6.0}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k70_Jet1",         23,-1.15, 1.15, "#sum q_{track}*p^{7.0}_{T,track}/#sum p^{7.0}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k80_Jet1",         23,-1.15, 1.15, "#sum q_{track}*p^{8.0}_{T,track}/#sum p^{8.0}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k90_Jet1",         23,-1.15, 1.15, "#sum q_{track}*p^{9.0}_{T,track}/#sum p^{9.0}_{T,track}", "", ZeeDHistLevel::Analysis  );
      
      AddTH1D(collections[i] + "_SumQPtAbs_k02_Jet1",         12, -0.05, 1.15, "|#sum q_{track}*p^{0.2}_{T,track}/#sum p^{0.2}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k04_Jet1",         12, -0.05, 1.15, "|#sum q_{track}*p^{0.4}_{T,track}/#sum p^{0.2}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k06_Jet1",         12, -0.05, 1.15, "|#sum q_{track}*p^{0.6}_{T,track}/#sum p^{0.2}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k08_Jet1",         12, -0.05, 1.15, "|#sum q_{track}*p^{0.8}_{T,track}/#sum p^{0.2}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k10_Jet1",         12, -0.05, 1.15, "|#sum q_{track}*p^{1.0}_{T,track}/#sum p^{1.0}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k12_Jet1",         12, -0.05, 1.15, "|#sum q_{track}*p^{1.2}_{T,track}/#sum p^{1.2}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k14_Jet1",         12, -0.05, 1.15, "|#sum q_{track}*p^{1.4}_{T,track}/#sum p^{1.4}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k16_Jet1",         12, -0.05, 1.15, "|#sum q_{track}*p^{1.6}_{T,track}/#sum p^{1.6}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k18_Jet1",         12, -0.05, 1.15, "|#sum q_{track}*p^{1.8}_{T,track}/#sum p^{1.8}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k20_Jet1",         12, -0.05, 1.15, "|#sum q_{track}*p^{2.0}_{T,track}/#sum p^{2.0}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k30_Jet1",         12, -0.05, 1.15, "|#sum q_{track}*p^{3.0}_{T,track}/#sum p^{3.0}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k40_Jet1",         12, -0.05, 1.15, "|#sum q_{track}*p^{4.0}_{T,track}/#sum p^{4.0}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k50_Jet1",         12, -0.05, 1.15, "|#sum q_{track}*p^{5.0}_{T,track}/#sum p^{5.0}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k60_Jet1",         12, -0.05, 1.15, "|#sum q_{track}*p^{6.0}_{T,track}/#sum p^{6.0}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k70_Jet1",         12, -0.05, 1.15, "|#sum q_{track}*p^{7.0}_{T,track}/#sum p^{7.0}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k80_Jet1",         12, -0.05, 1.15, "|#sum q_{track}*p^{8.0}_{T,track}/#sum p^{8.0}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k90_Jet1",         12, -0.05, 1.15, "|#sum q_{track}*p^{9.0}_{T,track}/#sum p^{9.0}_{T,track}|", "", ZeeDHistLevel::Analysis  );


      AddTH1D(collections[i] + "_SumQPt_k02_Jet1_Ntrk10",         23, -1.15, 1.15, "#sum q_{track}*p^{0.2}_{T,track}/#sum p^{0.2}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k04_Jet1_Ntrk10",         23,-1.15, 1.15, "#sum q_{track}*p^{0.4}_{T,track}/#sum p^{0.2}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k06_Jet1_Ntrk10",         23,-1.15, 1.15, "#sum q_{track}*p^{0.6}_{T,track}/#sum p^{0.2}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k08_Jet1_Ntrk10",         23,-1.15, 1.15, "#sum q_{track}*p^{0.8}_{T,track}/#sum p^{0.2}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k10_Jet1_Ntrk10",         23,-1.15, 1.15, "#sum q_{track}*p^{1.0}_{T,track}/#sum p^{1.0}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k12_Jet1_Ntrk10",         23,-1.15, 1.15, "#sum q_{track}*p^{1.2}_{T,track}/#sum p^{1.2}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k14_Jet1_Ntrk10",         23,-1.15, 1.15, "#sum q_{track}*p^{1.4}_{T,track}/#sum p^{1.4}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k16_Jet1_Ntrk10",         23,-1.15, 1.15, "#sum q_{track}*p^{1.6}_{T,track}/#sum p^{1.6}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k18_Jet1_Ntrk10",         23,-1.15, 1.15, "#sum q_{track}*p^{1.8}_{T,track}/#sum p^{1.8}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k20_Jet1_Ntrk10",         23,-1.15, 1.15, "#sum q_{track}*p^{2.0}_{T,track}/#sum p^{2.0}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k30_Jet1_Ntrk10",         23,-1.15, 1.15, "#sum q_{track}*p^{3.0}_{T,track}/#sum p^{3.0}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k40_Jet1_Ntrk10",         23,-1.15, 1.15, "#sum q_{track}*p^{4.0}_{T,track}/#sum p^{4.0}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k50_Jet1_Ntrk10",         23,-1.15, 1.15, "#sum q_{track}*p^{5.0}_{T,track}/#sum p^{5.0}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k60_Jet1_Ntrk10",         23,-1.15, 1.15, "#sum q_{track}*p^{6.0}_{T,track}/#sum p^{6.0}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k70_Jet1_Ntrk10",         23,-1.15, 1.15, "#sum q_{track}*p^{7.0}_{T,track}/#sum p^{7.0}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k80_Jet1_Ntrk10",         23,-1.15, 1.15, "#sum q_{track}*p^{8.0}_{T,track}/#sum p^{8.0}_{T,track}", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPt_k90_Jet1_Ntrk10",         23,-1.15, 1.15, "#sum q_{track}*p^{9.0}_{T,track}/#sum p^{9.0}_{T,track}", "", ZeeDHistLevel::Analysis  );
      
      AddTH1D(collections[i] + "_SumQPtAbs_k02_Jet1_Ntrk10",         12, -0.05, 1.15, "|#sum q_{track}*p^{0.2}_{T,track}/#sum p^{0.2}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k04_Jet1_Ntrk10",         12, -0.05, 1.15, "|#sum q_{track}*p^{0.4}_{T,track}/#sum p^{0.2}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k06_Jet1_Ntrk10",         12, -0.05, 1.15, "|#sum q_{track}*p^{0.6}_{T,track}/#sum p^{0.2}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k08_Jet1_Ntrk10",         12, -0.05, 1.15, "|#sum q_{track}*p^{0.8}_{T,track}/#sum p^{0.2}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k10_Jet1_Ntrk10",         12, -0.05, 1.15, "|#sum q_{track}*p^{1.0}_{T,track}/#sum p^{1.0}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k12_Jet1_Ntrk10",         12, -0.05, 1.15, "|#sum q_{track}*p^{1.2}_{T,track}/#sum p^{1.2}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k14_Jet1_Ntrk10",         12, -0.05, 1.15, "|#sum q_{track}*p^{1.4}_{T,track}/#sum p^{1.4}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k16_Jet1_Ntrk10",         12, -0.05, 1.15, "|#sum q_{track}*p^{1.6}_{T,track}/#sum p^{1.6}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k18_Jet1_Ntrk10",         12, -0.05, 1.15, "|#sum q_{track}*p^{1.8}_{T,track}/#sum p^{1.8}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k20_Jet1_Ntrk10",         12, -0.05, 1.15, "|#sum q_{track}*p^{2.0}_{T,track}/#sum p^{2.0}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k30_Jet1_Ntrk10",         12, -0.05, 1.15, "|#sum q_{track}*p^{3.0}_{T,track}/#sum p^{3.0}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k40_Jet1_Ntrk10",         12, -0.05, 1.15, "|#sum q_{track}*p^{4.0}_{T,track}/#sum p^{4.0}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k50_Jet1_Ntrk10",         12, -0.05, 1.15, "|#sum q_{track}*p^{5.0}_{T,track}/#sum p^{5.0}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k60_Jet1_Ntrk10",         12, -0.05, 1.15, "|#sum q_{track}*p^{6.0}_{T,track}/#sum p^{6.0}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k70_Jet1_Ntrk10",         12, -0.05, 1.15, "|#sum q_{track}*p^{7.0}_{T,track}/#sum p^{7.0}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k80_Jet1_Ntrk10",         12, -0.05, 1.15, "|#sum q_{track}*p^{8.0}_{T,track}/#sum p^{8.0}_{T,track}|", "", ZeeDHistLevel::Analysis  );
      AddTH1D(collections[i] + "_SumQPtAbs_k90_Jet1_Ntrk10",         12, -0.05, 1.15, "|#sum q_{track}*p^{9.0}_{T,track}/#sum p^{9.0}_{T,track}|", "", ZeeDHistLevel::Analysis  );


      AddTH1D(collections[i] + "_TrackZ0_BC_Jet1",                         200, -200, 200,        "Track Z0 (jet_{1}) ", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_TrackD0_BC_Jet1",                         200, -100, 100,        "Track D0 (jet_{1})", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_TrackNumberOfPixelHits_BC_Jet1",          11, -0.5, 10.5,     "number of pixel hits (jet_{1})", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_TrackNumberOfSCTHits_BC_Jet1",            16, 4.5, 20.5,      "number of SCT hits (jet_{1})", "", ZeeDHistLevel::Analysis);	
      AddTH1D(collections[i] + "_TrackQ_BC_Jet1",                          50, -2, 2,          "Track Q (jet_{1})", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_TrackQoverP_BC_Jet1",                     50, -0.001, 0.001,  "Track QoverP (jet_{1})", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_TrackTheta_BC_Jet1",                      50, 0, 3.14,        "Track #theta (jet_{1})", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_TrackChi2_BC_Jet1",                       100, 0, 10,         "Track #chi^2 (jet_{1})", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_TrackPt_BC_Jet1",                         50, 0, 25,          "Track p_{T} (jet_{1})", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_TrackPz_BC_Jet1",                         50, 0, 25,          "Track p_{z} (jet_{1})", "", ZeeDHistLevel::Analysis);

      AddTH1D(collections[i] + "_TrackZ0_AC_Jet1",                         100, -5, 5,      "Track Z0 (jet_{1}) ", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_TrackD0_AC_Jet1",                         40, -20, 20,       "Track D0 (jet_{1})", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_TrackNumberOfPixelHits_AC_Jet1",          11, -0.5, 10.5,     "number of pixel hits (jet_{1})", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_TrackNumberOfSCTHits_AC_Jet1",            16, 4.5, 20.5,      "number of SCT hits (jet_{1})", "", ZeeDHistLevel::Analysis);	
      AddTH1D(collections[i] + "_TrackQ_AC_Jet1",                          50, -2, 2,          "Track Q (jet_{1})", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_TrackQoverP_AC_Jet1",                     50, -0.001, 0.001,  "Track QoverP (jet_{1})", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_TrackTheta_AC_Jet1",                      50, 0, 3.14,        "Track #theta (jet_{1})", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_TrackChi2_AC_Jet1",                       100, 0, 10,         "Track #chi^2 (jet_{1})", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_TrackPt_AC_Jet1",                         50, 0, 25,          "Track p_{T} (jet_{1})", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_TrackPz_AC_Jet1",                         50, 0, 25,          "Track p_{z} (jet_{1})", "", ZeeDHistLevel::Analysis);

      AddTH1D(collections[i] + "_nTrkJet1_BC",   41, -0.5, 40.5,  "nTrk (jet_{1})", "", ZeeDHistLevel::Analysis);
      AddTH1D(collections[i] + "_nTrkJet1_AC",   41, -0.5, 40.5,  "nTrk (jet_{1})", "", ZeeDHistLevel::Analysis);

      AddTH1D(collections[i] + "_dRJet1_Track_BC",   50, 0.0, 0.7,  "#DeltaR (jet_{1}, track)", "", ZeeDHistLevel::Analysis);     
      AddTH1D(collections[i] + "_dRJet1_Track_AC",   50, 0.0, 0.7,  "#DeltaR (jet_{1}, track)", "", ZeeDHistLevel::Analysis);   


      //correlation with nTrk

      AddTH2D(collections[i] + "_SumQ_nTrk_Jet",         21, -10.5, 10.5,  41, -0.5, 40.5, "#sum q_{track}", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQ_nTrk_Jet1",         21, -10.5, 10.5,  41, -0.5, 40.5, "#sum q_{track}", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );

      AddTH2D(collections[i] + "_SumQAbs_nTrk_Jet",         11, -0.5, 10.5,  41, -0.5, 40.5, "|#sum q_{track}|", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQAbs_nTrk_Jet1",         11, -0.5, 10.5,  41, -0.5, 40.5, "|#sum q_{track}|",  "nTrk (jet_{1})",   ZeeDHistLevel::Analysis  );

      AddTH2D(collections[i] + "_SumQPt_k02_nTrk_Jet1",         23, -1.15, 1.15,  41, -0.5, 40.5, "#sum q_{track}*p^{0.2}_{T,track}/#sum p^{0.2}_{T,track}", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPt_k04_nTrk_Jet1",         23,-1.15, 1.15,  41, -0.5, 40.5, "#sum q_{track}*p^{0.4}_{T,track}/#sum p^{0.2}_{T,track}", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPt_k06_nTrk_Jet1",         23,-1.15, 1.15,  41, -0.5, 40.5, "#sum q_{track}*p^{0.6}_{T,track}/#sum p^{0.2}_{T,track}", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPt_k08_nTrk_Jet1",         23,-1.15, 1.15,  41, -0.5, 40.5, "#sum q_{track}*p^{0.8}_{T,track}/#sum p^{0.2}_{T,track}", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPt_k10_nTrk_Jet1",         23,-1.15, 1.15,  41, -0.5, 40.5, "#sum q_{track}*p^{1.0}_{T,track}/#sum p^{1.0}_{T,track}", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPt_k12_nTrk_Jet1",         23,-1.15, 1.15,  41, -0.5, 40.5, "#sum q_{track}*p^{1.2}_{T,track}/#sum p^{1.2}_{T,track}", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPt_k14_nTrk_Jet1",         23,-1.15, 1.15,  41, -0.5, 40.5, "#sum q_{track}*p^{1.4}_{T,track}/#sum p^{1.4}_{T,track}", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPt_k16_nTrk_Jet1",         23,-1.15, 1.15,  41, -0.5, 40.5, "#sum q_{track}*p^{1.6}_{T,track}/#sum p^{1.6}_{T,track}", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPt_k18_nTrk_Jet1",         23,-1.15, 1.15,  41, -0.5, 40.5, "#sum q_{track}*p^{1.8}_{T,track}/#sum p^{1.8}_{T,track}", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPt_k20_nTrk_Jet1",         23,-1.15, 1.15,  41, -0.5, 40.5, "#sum q_{track}*p^{2.0}_{T,track}/#sum p^{2.0}_{T,track}", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPt_k30_nTrk_Jet1",         23,-1.15, 1.15,  41, -0.5, 40.5, "#sum q_{track}*p^{3.0}_{T,track}/#sum p^{3.0}_{T,track}", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPt_k40_nTrk_Jet1",         23,-1.15, 1.15,  41, -0.5, 40.5, "#sum q_{track}*p^{4.0}_{T,track}/#sum p^{4.0}_{T,track}", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPt_k50_nTrk_Jet1",         23,-1.15, 1.15,  41, -0.5, 40.5, "#sum q_{track}*p^{5.0}_{T,track}/#sum p^{5.0}_{T,track}", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPt_k60_nTrk_Jet1",         23,-1.15, 1.15,  41, -0.5, 40.5, "#sum q_{track}*p^{6.0}_{T,track}/#sum p^{6.0}_{T,track}", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPt_k70_nTrk_Jet1",         23,-1.15, 1.15,  41, -0.5, 40.5, "#sum q_{track}*p^{7.0}_{T,track}/#sum p^{7.0}_{T,track}", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPt_k80_nTrk_Jet1",         23,-1.15, 1.15,  41, -0.5, 40.5, "#sum q_{track}*p^{8.0}_{T,track}/#sum p^{8.0}_{T,track}", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPt_k90_nTrk_Jet1",         23,-1.15, 1.15,  41, -0.5, 40.5, "#sum q_{track}*p^{9.0}_{T,track}/#sum p^{9.0}_{T,track}", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      
      AddTH2D(collections[i] + "_SumQPtAbs_k02_nTrk_Jet1",         12, -0.05, 1.15,  41, -0.5, 40.5,  "|#sum q_{track}*p^{0.2}_{T,track}/#sum p^{0.2}_{T,track}|", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPtAbs_k04_nTrk_Jet1",         12, -0.05, 1.15,  41, -0.5, 40.5,  "|#sum q_{track}*p^{0.4}_{T,track}/#sum p^{0.2}_{T,track}|", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPtAbs_k06_nTrk_Jet1",         12, -0.05, 1.15,  41, -0.5, 40.5,  "|#sum q_{track}*p^{0.6}_{T,track}/#sum p^{0.2}_{T,track}|", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPtAbs_k08_nTrk_Jet1",         12, -0.05, 1.15,  41, -0.5, 40.5,  "|#sum q_{track}*p^{0.8}_{T,track}/#sum p^{0.2}_{T,track}|", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPtAbs_k10_nTrk_Jet1",         12, -0.05, 1.15,  41, -0.5, 40.5,  "|#sum q_{track}*p^{1.0}_{T,track}/#sum p^{1.0}_{T,track}|", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPtAbs_k12_nTrk_Jet1",         12, -0.05, 1.15,  41, -0.5, 40.5,  "|#sum q_{track}*p^{1.2}_{T,track}/#sum p^{1.2}_{T,track}|", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPtAbs_k14_nTrk_Jet1",         12, -0.05, 1.15,  41, -0.5, 40.5,  "|#sum q_{track}*p^{1.4}_{T,track}/#sum p^{1.4}_{T,track}|", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPtAbs_k16_nTrk_Jet1",         12, -0.05, 1.15,  41, -0.5, 40.5,  "|#sum q_{track}*p^{1.6}_{T,track}/#sum p^{1.6}_{T,track}|", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPtAbs_k18_nTrk_Jet1",         12, -0.05, 1.15,  41, -0.5, 40.5,  "|#sum q_{track}*p^{1.8}_{T,track}/#sum p^{1.8}_{T,track}|", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPtAbs_k20_nTrk_Jet1",         12, -0.05, 1.15,  41, -0.5, 40.5,  "|#sum q_{track}*p^{2.0}_{T,track}/#sum p^{2.0}_{T,track}|", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPtAbs_k30_nTrk_Jet1",         12, -0.05, 1.15,  41, -0.5, 40.5,  "|#sum q_{track}*p^{3.0}_{T,track}/#sum p^{3.0}_{T,track}|", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPtAbs_k40_nTrk_Jet1",         12, -0.05, 1.15,  41, -0.5, 40.5,  "|#sum q_{track}*p^{4.0}_{T,track}/#sum p^{4.0}_{T,track}|", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPtAbs_k50_nTrk_Jet1",         12, -0.05, 1.15,  41, -0.5, 40.5,  "|#sum q_{track}*p^{5.0}_{T,track}/#sum p^{5.0}_{T,track}|", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPtAbs_k60_nTrk_Jet1",         12, -0.05, 1.15,  41, -0.5, 40.5,  "|#sum q_{track}*p^{6.0}_{T,track}/#sum p^{6.0}_{T,track}|", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPtAbs_k70_nTrk_Jet1",         12, -0.05, 1.15,  41, -0.5, 40.5,  "|#sum q_{track}*p^{7.0}_{T,track}/#sum p^{7.0}_{T,track}|", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPtAbs_k80_nTrk_Jet1",         12, -0.05, 1.15,  41, -0.5, 40.5,  "|#sum q_{track}*p^{8.0}_{T,track}/#sum p^{8.0}_{T,track}|", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );
      AddTH2D(collections[i] + "_SumQPtAbs_k90_nTrk_Jet1",         12, -0.05, 1.15,  41, -0.5, 40.5,  "|#sum q_{track}*p^{9.0}_{T,track}/#sum p^{9.0}_{T,track}|", "nTrk (jet_{1})",    ZeeDHistLevel::Analysis  );

    }
}

//-----------------------------------------------------
void ZeeDHistManagerJetCharge::Fill()
{
  DEBUG_MES_START;
  DEBUG_MES(this->GetHMName());

  // Get event
  const ZeeDEvent* event = GetEvent();
  CHECK_NULL_PTR(event);

  // Event weight
  Double_t Weight = event->GetWeight();

  // Arrange loop over bosons
  if (bIsAll) {
    // All bosons
    const TObjArray* bosons = (bIsW) ?  event->GetWBosons() : event->GetZBosons();

    if ( bosons != NULL ) {

      for ( Int_t index = 0; index < bosons->GetEntriesFast(); ++index ) {
	    
	const ZeeDBosonZ* boson = static_cast<ZeeDBosonZ*>( bosons->At(index) );
	    
	bosonMomentum     = boson->GetFourVector();
	Electron1Momentum = boson->GetFirstLep()->GetFourVector();
	Electron2Momentum = boson->GetSecondLep()->GetFourVector();
	    
	    
	std::vector<std::string> collections = (*fAnaOptions)->JetCollectionNames();
	for (unsigned int i = 0; i < collections.size(); i++)
	  {
	    fCollectionName = collections[i];
	    TString _truthCollection( fCollectionName.c_str() );
	    _truthCollection.ReplaceAll("TopoEM","Truth");
	    _truthCollection.ReplaceAll("LCTopo","Truth");
	    _truthCollection.ReplaceAll("New","");
	    fTruthCollectionName = _truthCollection.Data();
	    const TObjArray *jets = event->GetJets( fCollectionName, kFALSE); // don't warn if missing, just continue
		
	    if ( jets ) FillJetHists(jets, Weight);
	  }	    
      }
    }	
  } else {
    // Selected boson only
    if (event->GetCurrentBoson() != NULL) {
	
      bosonMomentum     = event->GetCurrentBoson()->GetFourVector();
      Electron1Momentum = event->GetCurrentBoson()->GetFirstLep()->GetFourVector();
      Electron2Momentum = event->GetCurrentBoson()->GetSecondLep()->GetFourVector();
	
      std::vector<std::string> collections = (*fAnaOptions)->JetCollectionNames();
      for (unsigned int i = 0; i < collections.size(); i++)
	{
	  fCollectionName = collections[i];
	  TString _truthCollection( fCollectionName.c_str() );
	  _truthCollection.ReplaceAll("TopoEM","Truth");
	  _truthCollection.ReplaceAll("LCTopo","Truth");  
	  _truthCollection.ReplaceAll("New","");
	  fTruthCollectionName = _truthCollection.Data();
	    
	  const TObjArray *jets = event->GetJets( fCollectionName , kFALSE);
	  if ( jets )  FillJetHists( jets,  Weight);
	}	
    }
  }

  DEBUG_MES_END;
}

//-----------------------------------------------------

void ZeeDHistManagerJetCharge::FillJetHists(const TObjArray* jets, Double_t Weight )
{
     
  Bool_t _debug_ = false;
  Double_t bosonPt = bosonMomentum.Pt();
  Double_t bosonMass = bosonMomentum.M();
  if ( bosonPt < bosonPtCut ) return;
  FillTH1(bosonPt,    Weight, (fCollectionName + "_Jets_BosPt").c_str()  );
  FillTH1(bosonMass,    Weight, (fCollectionName + "_BosMass").c_str()  );


  const ZeeDEvent* event = GetEvent();
  CHECK_NULL_PTR(event);

  //Get array of tracks 
  const TObjArray *tracks = event->GetTracks();

  Double_t vertexZ = 0.0, vertexY = 0.0, vertexX = 0.0;
  // Get array of vertices
  const TObjArray* vertices = event->GetVertices();

  // Take the first vertex
  // FIXME: A.N should it not be a primary event vertex(icies)
  //
  if ( vertices->GetEntriesFast() > 0 ) {
    ZeeDVertex* vertex = static_cast< ZeeDVertex* >( vertices->At(0) );
    TVector3 vtx3Vector = vertex->GetVertexCandidate();

    // vertex position
    vertexZ = vtx3Vector.Z();
    vertexY = vtx3Vector.Y();
    vertexX = vtx3Vector.X();

  }

  //  -----------------------------------------
  leadingJet          =  NULL; 
  subLeadingJet       =  NULL;

  Bool_t isSubLeadingJetPtCutCosPhi = true;
  Bool_t SubJet = false;
  //-----------------------------------------  
  Double_t ptReferenceCosPhi     = 0. ;
  Double_t ptBalanceCosPhi       = 0. ;
  //-----------------------------------------  

  for (std::map<std::string,Double_t>::iterator itr = deltaRCutMap.begin() ; itr != deltaRCutMap.end(); ++itr)
    if (fCollectionName.find(itr->first) != std::string::npos) 
      {
	deltaRcut = itr->second;
	break;
      }

  for (std::map<std::string,Double_t>::iterator itr = jetConeMap.begin() ; itr != deltaRCutMap.end(); ++itr)
    if (fCollectionName.find(itr->first) != std::string::npos) 
      {
	jetCone = itr->second;
	break;
      }

  if ( _debug_)
    {
      std::cout <<"\t\t\t  "<<fCollectionName<<" deltaR = "<<deltaRcut<< std::endl;
    }

  for (Int_t ijet = 0;  ijet< jets->GetEntries(); ++ijet)
    {
      ZeeDJet* jet =dynamic_cast<ZeeDJet*>( jets->At(ijet) ) ;
      CHECK_NULL_PTR( jet );

      if ( _debug_ )
	{
	  Info(__PRETTY_FUNCTION__,TString::Format(" ***** Looping jets %d",ijet));
	}

      TLorentzVector fourVector = jet->GetFourVector();
      TLorentzVector fourVectorCS = jet->GetFourVectorCS();
      TLorentzVector fourVectorRaw = jet->GetFourVectorRaw();

      Double_t pt        = fourVector.Pt();
      Double_t y         = fourVector.Rapidity();
      //       Double_t phi       = fourVector.Phi();

      //       Double_t emf        = jet->GetEmFraction();
      //       Double_t jvf        = jet->GetJVF();
      Double_t deltaPhi = fourVector.DeltaPhi( bosonMomentum );
      Double_t deltaRElec1 = fourVector.DeltaR( Electron1Momentum );
      Double_t deltaRElec2 = fourVector.DeltaR( Electron2Momentum );
      
      int qg_label = jet -> GetQGLabel();

      Bool_t isIsolated = true;
      if ( ( deltaRElec1 <= deltaRcut ) || ( deltaRElec2 <= deltaRcut ) ) isIsolated = false;
      if ( !isIsolated ) continue;
      if ( TMath::Abs(y) > 2.5 ) continue;
      if ( pt < 25 ) continue;
      if ( deltaPhi < 2.5 ) continue;

      Double_t SumQ = 0.0;
      Double_t SumPt = 0.0;
      Double_t SumQPt_02 = 0.0, SumQPt_04 = 0.0, SumQPt_06 = 0.0, SumQPt_08 = 0.0, SumQPt_10 = 0.0, SumQPt_12 = 0.0, SumQPt_14 = 0.0, SumQPt_16 = 0.0, SumQPt_18 = 0.0, SumQPt_20 = 0.0;
      Double_t SumQPt_30 = 0.0, SumQPt_40 = 0.0, SumQPt_50 = 0.0, SumQPt_60 = 0.0, SumQPt_70 = 0.0, SumQPt_80 = 0.0, SumQPt_90 = 0.0;     
      Double_t SumPt_02 = 0.0, SumPt_04 = 0.0, SumPt_06 = 0.0, SumPt_08 = 0.0, SumPt_10 = 0.0, SumPt_12 = 0.0, SumPt_14 = 0.0, SumPt_16 = 0.0, SumPt_18 = 0.0, SumPt_20 = 0.0;
      Double_t SumPt_30 = 0.0, SumPt_40 = 0.0, SumPt_50 = 0.0, SumPt_60 = 0.0, SumPt_70 = 0.0, SumPt_80 = 0.0, SumPt_90 = 0.0;   

      Int_t NumTracksBC = 0, NumTracksAC = 0;

      if ( leadingJet == NULL ) 
        {
          leadingJet = jet;

	  ptBalanceCosPhi = pt;
	  ptReferenceCosPhi = bosonPt * TMath::Abs( TMath::Cos( deltaPhi ) );

        }
      if (( jet           != leadingJet)  &&
          ( leadingJet    != NULL)        &&
          ( subLeadingJet == NULL ))
        {
          subLeadingJet =  jet ; 
	  SubJet = true;	  
	  // 	  if ( pt > ( 0.2 * ptReferenceCosPhi) ){ 
	  // 	    isSubLeadingJetPtCutCosPhi = false;
	  //	  }
	}     
      //*******************balance**************************//

      //   if (( leadingJet )&&( isSubLeadingJetPtCutCosPhi ))
      if (( leadingJet )&&( !SubJet ))
 	{
	  if (ptReferenceCosPhi > 0)
	    ptBalanceCosPhi /= ptReferenceCosPhi;
	  else
	    ptBalanceCosPhi = 0;
	  
	  FillTH1(ptBalanceCosPhi,            Weight, (fCollectionName +           "_Balance").c_str()  );
          FillTH1(pt,   Weight, (fCollectionName + "_PtJet1").c_str()  );
	  FillTH1(deltaPhi,   Weight, (fCollectionName + "_deltaPhiJetBos").c_str()  );

	  for (Int_t itrack = 0; itrack < tracks->GetEntries(); ++itrack)
	    {
	      ZeeDTrack* track = dynamic_cast<ZeeDTrack*>(tracks->At(itrack) );
	      CHECK_NULL_PTR( track );  
  
	      TLorentzVector fourVectorTrack = track->GetFourVector();
	      Double_t deltaRTrack = fourVector.DeltaR( fourVectorTrack );
	      Double_t p = sqrt( fourVectorTrack.Px()*fourVectorTrack.Px() + fourVectorTrack.Py()*fourVectorTrack.Py() + fourVectorTrack.Pz()*fourVectorTrack.Pz() );
	      Double_t pt = fourVectorTrack.Pt();
	      Double_t pz = fourVectorTrack.Pz();
	      Double_t d0 = track->GetTrackD0();
	      Double_t z0 = track->GetTrackZ0();
	      Double_t numpixhits = track->GetTrackNumberOfPixelHits();
	      Double_t numscthits = track->GetTrackNumberOfSCTHits();
	      Double_t qoverp = track->GetTrackqOverP();
	      Double_t q = track->GetTrackqOverP()*p*GeV;
	      Double_t theta = track->GetTrackTheta();
	      Double_t chi = track->GetTrackChiSquared();
	      Double_t dof = track->GetTrackNumberDoF();
	      
	      if ( deltaRTrack > jetCone ) continue;
	      NumTracksBC++;

	      FillTH1(deltaRTrack,                       Weight, (fCollectionName + "_dRJet1_Track_BC").c_str()  );
	      FillTH1(d0,                       Weight, (fCollectionName + "_TrackD0_BC_Jet1").c_str()  );
	      FillTH1((vertexZ - z0) * TMath::Sin(theta),   Weight, (fCollectionName + "_TrackZ0_BC_Jet1").c_str()  );
	      FillTH1(numpixhits,               Weight, (fCollectionName + "_TrackNumberOfPixelHits_BC_Jet1").c_str()  );
	      FillTH1(numscthits,               Weight, (fCollectionName + "_TrackNumberOfSCTHits_BC_Jet1").c_str()  );
	      FillTH1(qoverp,                   Weight, (fCollectionName + "_TrackQoverP_BC_Jet1").c_str()  );
	      FillTH1(qoverp * p * 1000,        Weight, (fCollectionName + "_TrackQ_BC_Jet1").c_str()  );
	      FillTH1(theta,                    Weight, (fCollectionName +"_TrackTheta_BC_Jet1").c_str()  );
	      FillTH1(chi / dof,                Weight, (fCollectionName +"_TrackChi2_BC_Jet1").c_str()  );
	      FillTH1(pt,                       Weight, (fCollectionName +"_TrackPt_BC_Jet1").c_str()  );
	      FillTH1(pz,                       Weight, (fCollectionName +"_TrackPz_BC_Jet1").c_str()  );

	      //	      if ( pt < 1.0 ) continue;
	      if ( numpixhits <= 1 ) continue;
	      if ( numscthits <= 6 )continue;
	      //  if ( TMath::Abs( d0 ) > 15 ) continue;
	      if ( TMath::Abs( (vertexZ - z0)*TMath::Sin( theta ) ) > 1.5 ) continue;

	      FillTH1(deltaRTrack,                       Weight, (fCollectionName + "_dRJet1_Track_AC").c_str()  );
	      FillTH1(d0,                       Weight, (fCollectionName + "_TrackD0_AC_Jet1").c_str()  );
	      FillTH1((vertexZ - z0) * TMath::Sin(theta),   Weight, (fCollectionName + "_TrackZ0_AC_Jet1").c_str()  );
	      FillTH1(numpixhits,               Weight, (fCollectionName + "_TrackNumberOfPixelHits_AC_Jet1").c_str()  );
	      FillTH1(numscthits,               Weight, (fCollectionName + "_TrackNumberOfSCTHits_AC_Jet1").c_str()  );
	      FillTH1(qoverp,                   Weight, (fCollectionName + "_TrackQoverP_AC_Jet1").c_str()  );
	      FillTH1(qoverp * p * 1000,        Weight, (fCollectionName + "_TrackQ_AC_Jet1").c_str()  );
	      FillTH1(theta,                    Weight, (fCollectionName +"_TrackTheta_AC_Jet1").c_str()  );
	      FillTH1(chi / dof,                Weight, (fCollectionName +"_TrackChi2_AC_Jet1").c_str()  );
	      FillTH1(pt,                       Weight, (fCollectionName +"_TrackPt_AC_Jet1").c_str()  );
	      FillTH1(pz,                       Weight, (fCollectionName +"_TrackPz_AC_Jet1").c_str()  );

	      SumQ+= q;
	      SumPt+= pt;

	      SumQPt_02+= q*pow(pt, 0.2);
	      SumQPt_04+= q*pow(pt, 0.4);
	      SumQPt_06+= q*pow(pt, 0.6);
	      SumQPt_08+= q*pow(pt, 0.8);
	      SumQPt_10+= q*pow(pt, 1.0);
	      SumQPt_12+= q*pow(pt, 1.2);
	      SumQPt_14+= q*pow(pt, 1.4);
	      SumQPt_16+= q*pow(pt, 1.6);
	      SumQPt_18+= q*pow(pt, 1.8);
	      SumQPt_20+= q*pow(pt, 2.0);
	      SumQPt_30+= q*pow(pt, 3.0);
	      SumQPt_40+= q*pow(pt, 4.0);
	      SumQPt_50+= q*pow(pt, 5.0);
	      SumQPt_60+= q*pow(pt, 6.0);
	      SumQPt_70+= q*pow(pt, 7.0);
	      SumQPt_80+= q*pow(pt, 8.0);
	      SumQPt_90+= q*pow(pt, 9.0);

	      SumPt_02+= pow(pt, 0.2);
	      SumPt_04+= pow(pt, 0.4);
	      SumPt_06+= pow(pt, 0.6);
	      SumPt_08+= pow(pt, 0.8);
	      SumPt_10+= pow(pt, 1.0);
	      SumPt_12+= pow(pt, 1.2);
	      SumPt_14+= pow(pt, 1.4);
	      SumPt_16+= pow(pt, 1.6);
	      SumPt_18+= pow(pt, 1.8);
	      SumPt_20+= pow(pt, 2.0);
	      SumPt_30+= pow(pt, 3.0);
	      SumPt_40+= pow(pt, 4.0);
	      SumPt_50+= pow(pt, 5.0);
	      SumPt_60+= pow(pt, 6.0);
	      SumPt_70+= pow(pt, 7.0);
	      SumPt_80+= pow(pt, 8.0);
	      SumPt_90+= pow(pt, 9.0);

	      NumTracksAC++;
	    }

	  FillTH1(NumTracksBC,                       Weight, (fCollectionName + "_nTrkJet1_BC").c_str()  );
	  FillTH1(NumTracksAC,                       Weight, (fCollectionName + "_nTrkJet1_AC").c_str()  );


	  if (NumTracksAC >= 2){
	    FillTH1( SumQ,               Weight, (fCollectionName +                 "_SumQ_Jet1").c_str()  );
	    FillTH1( TMath::Abs(SumQ),   Weight, (fCollectionName +                 "_SumQAbs_Jet1").c_str()  );

	    FillTH1( SumQPt_02/SumPt_02,            Weight, (fCollectionName +           "_SumQPt_k02_Jet1").c_str()  );
	    FillTH1( SumQPt_04/SumPt_04,            Weight, (fCollectionName +           "_SumQPt_k04_Jet1").c_str()  );
	    FillTH1( SumQPt_06/SumPt_06,            Weight, (fCollectionName +           "_SumQPt_k06_Jet1").c_str()  );
	    FillTH1( SumQPt_08/SumPt_08,            Weight, (fCollectionName +           "_SumQPt_k08_Jet1").c_str()  );
	    FillTH1( SumQPt_10/SumPt_10,            Weight, (fCollectionName +           "_SumQPt_k10_Jet1").c_str()  );
	    FillTH1( SumQPt_12/SumPt_12,            Weight, (fCollectionName +           "_SumQPt_k12_Jet1").c_str()  );
	    FillTH1( SumQPt_14/SumPt_14,            Weight, (fCollectionName +           "_SumQPt_k14_Jet1").c_str()  );
	    FillTH1( SumQPt_16/SumPt_16,            Weight, (fCollectionName +           "_SumQPt_k16_Jet1").c_str()  );
	    FillTH1( SumQPt_18/SumPt_18,            Weight, (fCollectionName +           "_SumQPt_k18_Jet1").c_str()  );
	    FillTH1( SumQPt_20/SumPt_20,            Weight, (fCollectionName +           "_SumQPt_k20_Jet1").c_str()  );
	    FillTH1( SumQPt_30/SumPt_30,            Weight, (fCollectionName +           "_SumQPt_k30_Jet1").c_str()  );
	    FillTH1( SumQPt_40/SumPt_40,            Weight, (fCollectionName +           "_SumQPt_k40_Jet1").c_str()  );
	    FillTH1( SumQPt_50/SumPt_50,            Weight, (fCollectionName +           "_SumQPt_k50_Jet1").c_str()  );
	    FillTH1( SumQPt_60/SumPt_60,            Weight, (fCollectionName +           "_SumQPt_k60_Jet1").c_str()  );
	    FillTH1( SumQPt_70/SumPt_70,            Weight, (fCollectionName +           "_SumQPt_k70_Jet1").c_str()  );
	    FillTH1( SumQPt_80/SumPt_80,            Weight, (fCollectionName +           "_SumQPt_k80_Jet1").c_str()  );
	    FillTH1( SumQPt_90/SumPt_90,            Weight, (fCollectionName +           "_SumQPt_k90_Jet1").c_str()  );

	    FillTH1( TMath::Abs(SumQPt_02/SumPt_02),            Weight, (fCollectionName +           "_SumQPtAbs_k02_Jet1").c_str()  );
	    FillTH1( TMath::Abs(SumQPt_04/SumPt_04),            Weight, (fCollectionName +           "_SumQPtAbs_k04_Jet1").c_str()  );
	    FillTH1( TMath::Abs(SumQPt_06/SumPt_06),            Weight, (fCollectionName +           "_SumQPtAbs_k06_Jet1").c_str()  );
	    FillTH1( TMath::Abs(SumQPt_08/SumPt_08),            Weight, (fCollectionName +           "_SumQPtAbs_k08_Jet1").c_str()  );
	    FillTH1( TMath::Abs(SumQPt_10/SumPt_10),            Weight, (fCollectionName +           "_SumQPtAbs_k10_Jet1").c_str()  );
	    FillTH1( TMath::Abs(SumQPt_12/SumPt_12),            Weight, (fCollectionName +           "_SumQPtAbs_k12_Jet1").c_str()  );
	    FillTH1( TMath::Abs(SumQPt_14/SumPt_14),            Weight, (fCollectionName +           "_SumQPtAbs_k14_Jet1").c_str()  );
	    FillTH1( TMath::Abs(SumQPt_16/SumPt_16),            Weight, (fCollectionName +           "_SumQPtAbs_k16_Jet1").c_str()  );
	    FillTH1( TMath::Abs(SumQPt_18/SumPt_18),            Weight, (fCollectionName +           "_SumQPtAbs_k18_Jet1").c_str()  );
	    FillTH1( TMath::Abs(SumQPt_20/SumPt_20),            Weight, (fCollectionName +           "_SumQPtAbs_k20_Jet1").c_str()  );
	    FillTH1( TMath::Abs(SumQPt_30/SumPt_30),            Weight, (fCollectionName +           "_SumQPtAbs_k30_Jet1").c_str()  );
	    FillTH1( TMath::Abs(SumQPt_40/SumPt_40),            Weight, (fCollectionName +           "_SumQPtAbs_k40_Jet1").c_str()  );
	    FillTH1( TMath::Abs(SumQPt_50/SumPt_50),            Weight, (fCollectionName +           "_SumQPtAbs_k50_Jet1").c_str()  );
	    FillTH1( TMath::Abs(SumQPt_60/SumPt_60),            Weight, (fCollectionName +           "_SumQPtAbs_k60_Jet1").c_str()  );
	    FillTH1( TMath::Abs(SumQPt_70/SumPt_70),            Weight, (fCollectionName +           "_SumQPtAbs_k70_Jet1").c_str()  );
	    FillTH1( TMath::Abs(SumQPt_80/SumPt_80),            Weight, (fCollectionName +           "_SumQPtAbs_k80_Jet1").c_str()  );
	    FillTH1( TMath::Abs(SumQPt_90/SumPt_90),            Weight, (fCollectionName +           "_SumQPtAbs_k90_Jet1").c_str()  );

	    FillTH2( SumQ,     NumTracksAC,          Weight, (fCollectionName +                 "_SumQ_nTrk_Jet1").c_str()  );
	    FillTH2( TMath::Abs(SumQ),  NumTracksAC,  Weight, (fCollectionName +                 "_SumQAbs_nTrk_Jet1").c_str()  );

	    FillTH2( SumQPt_02/SumPt_02,     NumTracksAC,         Weight, (fCollectionName +           "_SumQPt_k02_nTrk_Jet1").c_str()  );
	    FillTH2( SumQPt_04/SumPt_04,     NumTracksAC,        Weight, (fCollectionName +           "_SumQPt_k04_nTrk_Jet1").c_str()  );
	    FillTH2( SumQPt_06/SumPt_06,     NumTracksAC,        Weight, (fCollectionName +           "_SumQPt_k06_nTrk_Jet1").c_str()  );
	    FillTH2( SumQPt_08/SumPt_08,     NumTracksAC,        Weight, (fCollectionName +           "_SumQPt_k08_nTrk_Jet1").c_str()  );
	    FillTH2( SumQPt_10/SumPt_10,     NumTracksAC,        Weight, (fCollectionName +           "_SumQPt_k10_nTrk_Jet1").c_str()  );
	    FillTH2( SumQPt_12/SumPt_12,     NumTracksAC,        Weight, (fCollectionName +           "_SumQPt_k12_nTrk_Jet1").c_str()  );
	    FillTH2( SumQPt_14/SumPt_14,     NumTracksAC,        Weight, (fCollectionName +           "_SumQPt_k14_nTrk_Jet1").c_str()  );
	    FillTH2( SumQPt_16/SumPt_16,     NumTracksAC,        Weight, (fCollectionName +           "_SumQPt_k16_nTrk_Jet1").c_str()  );
	    FillTH2( SumQPt_18/SumPt_18,     NumTracksAC,        Weight, (fCollectionName +           "_SumQPt_k18_nTrk_Jet1").c_str()  );
	    FillTH2( SumQPt_20/SumPt_20,     NumTracksAC,        Weight, (fCollectionName +           "_SumQPt_k20_nTrk_Jet1").c_str()  );
	    FillTH2( SumQPt_30/SumPt_30,     NumTracksAC,        Weight, (fCollectionName +           "_SumQPt_k30_nTrk_Jet1").c_str()  );
	    FillTH2( SumQPt_40/SumPt_40,     NumTracksAC,        Weight, (fCollectionName +           "_SumQPt_k40_nTrk_Jet1").c_str()  );
	    FillTH2( SumQPt_50/SumPt_50,     NumTracksAC,        Weight, (fCollectionName +           "_SumQPt_k50_nTrk_Jet1").c_str()  );
	    FillTH2( SumQPt_60/SumPt_60,     NumTracksAC,        Weight, (fCollectionName +           "_SumQPt_k60_nTrk_Jet1").c_str()  );
	    FillTH2( SumQPt_70/SumPt_70,     NumTracksAC,        Weight, (fCollectionName +           "_SumQPt_k70_nTrk_Jet1").c_str()  );
	    FillTH2( SumQPt_80/SumPt_80,     NumTracksAC,        Weight, (fCollectionName +           "_SumQPt_k80_nTrk_Jet1").c_str()  );
	    FillTH2( SumQPt_90/SumPt_90,     NumTracksAC,        Weight, (fCollectionName +           "_SumQPt_k90_nTrk_Jet1").c_str()  );
	    
	    FillTH2( TMath::Abs(SumQPt_02/SumPt_02),    NumTracksAC,        Weight, (fCollectionName +           "_SumQPtAbs_k02_nTrk_Jet1").c_str()  );
	    FillTH2( TMath::Abs(SumQPt_04/SumPt_04),    NumTracksAC,        Weight, (fCollectionName +           "_SumQPtAbs_k04_nTrk_Jet1").c_str()  );
	    FillTH2( TMath::Abs(SumQPt_06/SumPt_06),    NumTracksAC,        Weight, (fCollectionName +           "_SumQPtAbs_k06_nTrk_Jet1").c_str()  );
	    FillTH2( TMath::Abs(SumQPt_08/SumPt_08),    NumTracksAC,        Weight, (fCollectionName +           "_SumQPtAbs_k08_nTrk_Jet1").c_str()  );
	    FillTH2( TMath::Abs(SumQPt_10/SumPt_10),    NumTracksAC,        Weight, (fCollectionName +           "_SumQPtAbs_k10_nTrk_Jet1").c_str()  );
	    FillTH2( TMath::Abs(SumQPt_12/SumPt_12),    NumTracksAC,        Weight, (fCollectionName +           "_SumQPtAbs_k12_nTrk_Jet1").c_str()  );
	    FillTH2( TMath::Abs(SumQPt_14/SumPt_14),    NumTracksAC,        Weight, (fCollectionName +           "_SumQPtAbs_k14_nTrk_Jet1").c_str()  );
	    FillTH2( TMath::Abs(SumQPt_16/SumPt_16),    NumTracksAC,        Weight, (fCollectionName +           "_SumQPtAbs_k16_nTrk_Jet1").c_str()  );
	    FillTH2( TMath::Abs(SumQPt_18/SumPt_18),    NumTracksAC,        Weight, (fCollectionName +           "_SumQPtAbs_k18_nTrk_Jet1").c_str()  );
	    FillTH2( TMath::Abs(SumQPt_20/SumPt_20),    NumTracksAC,        Weight, (fCollectionName +           "_SumQPtAbs_k20_nTrk_Jet1").c_str()  );
	    FillTH2( TMath::Abs(SumQPt_30/SumPt_30),    NumTracksAC,        Weight, (fCollectionName +           "_SumQPtAbs_k30_nTrk_Jet1").c_str()  );
	    FillTH2( TMath::Abs(SumQPt_40/SumPt_40),    NumTracksAC,        Weight, (fCollectionName +           "_SumQPtAbs_k40_nTrk_Jet1").c_str()  );
	    FillTH2( TMath::Abs(SumQPt_50/SumPt_50),    NumTracksAC,        Weight, (fCollectionName +           "_SumQPtAbs_k50_nTrk_Jet1").c_str()  );
	    FillTH2( TMath::Abs(SumQPt_60/SumPt_60),    NumTracksAC,        Weight, (fCollectionName +           "_SumQPtAbs_k60_nTrk_Jet1").c_str()  );
	    FillTH2( TMath::Abs(SumQPt_70/SumPt_70),    NumTracksAC,        Weight, (fCollectionName +           "_SumQPtAbs_k70_nTrk_Jet1").c_str()  );
	    FillTH2( TMath::Abs(SumQPt_80/SumPt_80),    NumTracksAC,        Weight, (fCollectionName +           "_SumQPtAbs_k80_nTrk_Jet1").c_str()  );
	    FillTH2( TMath::Abs(SumQPt_90/SumPt_90),    NumTracksAC,        Weight, (fCollectionName +           "_SumQPtAbs_k90_nTrk_Jet1").c_str()  );
	  }
	  if (NumTracksAC >= 10)
	    {
	      FillTH1( SumQ,               Weight, (fCollectionName +                 "_SumQ_Jet1").c_str()  );
	      FillTH1( TMath::Abs(SumQ),   Weight, (fCollectionName +                 "_SumQAbs_Jet1").c_str()  );

	      FillTH1( SumQPt_02/SumPt_02,            Weight, (fCollectionName +           "_SumQPt_k02_Jet1_Ntrk10").c_str()  );
	      FillTH1( SumQPt_04/SumPt_04,            Weight, (fCollectionName +           "_SumQPt_k04_Jet1_Ntrk10").c_str()  );
	      FillTH1( SumQPt_06/SumPt_06,            Weight, (fCollectionName +           "_SumQPt_k06_Jet1_Ntrk10").c_str()  );
	      FillTH1( SumQPt_08/SumPt_08,            Weight, (fCollectionName +           "_SumQPt_k08_Jet1_Ntrk10").c_str()  );
	      FillTH1( SumQPt_10/SumPt_10,            Weight, (fCollectionName +           "_SumQPt_k10_Jet1_Ntrk10").c_str()  );
	      FillTH1( SumQPt_12/SumPt_12,            Weight, (fCollectionName +           "_SumQPt_k12_Jet1_Ntrk10").c_str()  );
	      FillTH1( SumQPt_14/SumPt_14,            Weight, (fCollectionName +           "_SumQPt_k14_Jet1_Ntrk10").c_str()  );
	      FillTH1( SumQPt_16/SumPt_16,            Weight, (fCollectionName +           "_SumQPt_k16_Jet1_Ntrk10").c_str()  );
	      FillTH1( SumQPt_18/SumPt_18,            Weight, (fCollectionName +           "_SumQPt_k18_Jet1_Ntrk10").c_str()  );
	      FillTH1( SumQPt_20/SumPt_20,            Weight, (fCollectionName +           "_SumQPt_k20_Jet1_Ntrk10").c_str()  );
	      FillTH1( SumQPt_30/SumPt_30,            Weight, (fCollectionName +           "_SumQPt_k30_Jet1_Ntrk10").c_str()  );
	      FillTH1( SumQPt_40/SumPt_40,            Weight, (fCollectionName +           "_SumQPt_k40_Jet1_Ntrk10").c_str()  );
	      FillTH1( SumQPt_50/SumPt_50,            Weight, (fCollectionName +           "_SumQPt_k50_Jet1_Ntrk10").c_str()  );
	      FillTH1( SumQPt_60/SumPt_60,            Weight, (fCollectionName +           "_SumQPt_k60_Jet1_Ntrk10").c_str()  );
	      FillTH1( SumQPt_70/SumPt_70,            Weight, (fCollectionName +           "_SumQPt_k70_Jet1_Ntrk10").c_str()  );
	      FillTH1( SumQPt_80/SumPt_80,            Weight, (fCollectionName +           "_SumQPt_k80_Jet1_Ntrk10").c_str()  );
	      FillTH1( SumQPt_90/SumPt_90,            Weight, (fCollectionName +           "_SumQPt_k90_Jet1_Ntrk10").c_str()  );

	      FillTH1( TMath::Abs(SumQPt_02/SumPt_02),            Weight, (fCollectionName +           "_SumQPtAbs_k02_Jet1_Ntrk10").c_str()  );
	      FillTH1( TMath::Abs(SumQPt_04/SumPt_04),            Weight, (fCollectionName +           "_SumQPtAbs_k04_Jet1_Ntrk10").c_str()  );
	      FillTH1( TMath::Abs(SumQPt_06/SumPt_06),            Weight, (fCollectionName +           "_SumQPtAbs_k06_Jet1_Ntrk10").c_str()  );
	      FillTH1( TMath::Abs(SumQPt_08/SumPt_08),            Weight, (fCollectionName +           "_SumQPtAbs_k08_Jet1_Ntrk10").c_str()  );
	      FillTH1( TMath::Abs(SumQPt_10/SumPt_10),            Weight, (fCollectionName +           "_SumQPtAbs_k10_Jet1_Ntrk10").c_str()  );
	      FillTH1( TMath::Abs(SumQPt_12/SumPt_12),            Weight, (fCollectionName +           "_SumQPtAbs_k12_Jet1_Ntrk10").c_str()  );
	      FillTH1( TMath::Abs(SumQPt_14/SumPt_14),            Weight, (fCollectionName +           "_SumQPtAbs_k14_Jet1_Ntrk10").c_str()  );
	      FillTH1( TMath::Abs(SumQPt_16/SumPt_16),            Weight, (fCollectionName +           "_SumQPtAbs_k16_Jet1_Ntrk10").c_str()  );
	      FillTH1( TMath::Abs(SumQPt_18/SumPt_18),            Weight, (fCollectionName +           "_SumQPtAbs_k18_Jet1_Ntrk10").c_str()  );
	      FillTH1( TMath::Abs(SumQPt_20/SumPt_20),            Weight, (fCollectionName +           "_SumQPtAbs_k20_Jet1_Ntrk10").c_str()  );
	      FillTH1( TMath::Abs(SumQPt_30/SumPt_30),            Weight, (fCollectionName +           "_SumQPtAbs_k30_Jet1_Ntrk10").c_str()  );
	      FillTH1( TMath::Abs(SumQPt_40/SumPt_40),            Weight, (fCollectionName +           "_SumQPtAbs_k40_Jet1_Ntrk10").c_str()  );
	      FillTH1( TMath::Abs(SumQPt_50/SumPt_50),            Weight, (fCollectionName +           "_SumQPtAbs_k50_Jet1_Ntrk10").c_str()  );
	      FillTH1( TMath::Abs(SumQPt_60/SumPt_60),            Weight, (fCollectionName +           "_SumQPtAbs_k60_Jet1_Ntrk10").c_str()  );
	      FillTH1( TMath::Abs(SumQPt_70/SumPt_70),            Weight, (fCollectionName +           "_SumQPtAbs_k70_Jet1_Ntrk10").c_str()  );
	      FillTH1( TMath::Abs(SumQPt_80/SumPt_80),            Weight, (fCollectionName +           "_SumQPtAbs_k80_Jet1_Ntrk10").c_str()  );
	      FillTH1( TMath::Abs(SumQPt_90/SumPt_90),            Weight, (fCollectionName +           "_SumQPtAbs_k90_Jet1_Ntrk10").c_str()  );
	   
	    }

	  if (qg_label == 1)  FillTH1(bosonMass,    Weight, (fCollectionName + "_BosMass_d").c_str()  );
	  if (qg_label == -1) FillTH1(bosonMass,    Weight, (fCollectionName + "_BosMass_dbar").c_str()  );
	  if (qg_label == 2)  FillTH1(bosonMass,    Weight, (fCollectionName + "_BosMass_u").c_str()  );
	  if (qg_label == -2) FillTH1(bosonMass,    Weight, (fCollectionName + "_BosMass_ubar").c_str()  );
	  if (qg_label == 3)  FillTH1(bosonMass,    Weight, (fCollectionName + "_BosMass_s").c_str()  );
	  if (qg_label == -3) FillTH1(bosonMass,    Weight, (fCollectionName + "_BosMass_sbar").c_str()  );
	  if (qg_label == 4)  FillTH1(bosonMass,    Weight, (fCollectionName + "_BosMass_c").c_str()  );
	  if (qg_label == -4) FillTH1(bosonMass,    Weight, (fCollectionName + "_BosMass_cbar").c_str()  );
	  
	}

      //*******************all jets************************//

      for (Int_t itrack = 0; itrack < tracks->GetEntries(); ++itrack)
	{
	  ZeeDTrack* track = dynamic_cast<ZeeDTrack*>(tracks->At(itrack) );
	  CHECK_NULL_PTR( track );  
  
	  TLorentzVector fourVectorTrack = track->GetFourVector();
	  Double_t deltaRTrack = fourVector.DeltaR( fourVectorTrack );
	  Double_t p = sqrt( fourVectorTrack.Px()*fourVectorTrack.Px() + fourVectorTrack.Py()*fourVectorTrack.Py() + fourVectorTrack.Pz()*fourVectorTrack.Pz() );
	  Double_t pt = fourVectorTrack.Pt();
	  Double_t d0 = track->GetTrackD0();
	  Double_t z0 = track->GetTrackZ0();
	  Double_t numpixhits = track->GetTrackNumberOfPixelHits();
	  Double_t numscthits = track->GetTrackNumberOfSCTHits();
	  Double_t qoverp = track->GetTrackqOverP();
	  Double_t q = track->GetTrackqOverP()*p*GeV;
	  Double_t theta = track->GetTrackTheta();
	  Double_t chi = track->GetTrackChiSquared();
	  Double_t dof = track->GetTrackNumberDoF();
	      
	  if ( deltaRTrack > 0.4 ) continue;
	  if ( pt < 1.0 ) continue;
	  if ( numpixhits <= 1 ) continue;
	  if ( numscthits <= 6 )continue;
	  //  if ( TMath::Abs( d0 ) > 15 ) continue;
	  if ( TMath::Abs( (vertexZ - z0) * TMath::Sin( theta ) ) > 1.5 ) continue;

	  SumPt+= pt;
	  SumQPt_10+= q*pt;
	  SumQ+= q;

	  NumTracksAC++;
	}

      if (NumTracksAC >= 2){
	FillTH1( SumQ,               Weight, (fCollectionName +                             "_SumQ_Jet").c_str()  );
	FillTH1( TMath::Abs(SumQ),   Weight, (fCollectionName +                             "_SumQAbs_Jet").c_str()  );

      }
      FillTH1(pt,   Weight, (fCollectionName + "_PtJet").c_str() );

    } // jet loop
    
  return;
}

