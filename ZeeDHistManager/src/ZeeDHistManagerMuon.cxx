#include "ZeeDHistManager/ZeeDHistManagerMuon.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>
#include <TH1.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDVertex.h"
#include "ZeeDEvent/ZeeDMuon.h"
#include "ZeeDTools/ZeeDBags.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include "ZeeDMisc/ZeeDMisc.h"

#include "ZeeDTools/ZeeDCutBit.h"
#include "ZeeDTools/LinkWeightContainer.h"

#include "ZeeDTools/ZeeDSystematics.h"
using namespace std;
//-----------------------------------------------------
void ZeeDHistManagerMuon::BookHistos()
{

  // Books histograms

 	AddTH1D("EventWeight",          	      1010,  -2000.,   2000.,	"Event weight", "");
	AddTH1D("NMuons",	  	                    50,     -0.5,    49.5,	"N_{Muons}",    "");
	AddTH1D("Pt",		  	 ZeeDMisc::FindFile("Binning/LepPt.txt"),	"p_{T}^{Muon} / GeV", "", ZeeDHistLevel::Systematics );
	AddTH1D("PtPlus",		  	 ZeeDMisc::FindFile("Binning/LepPt.txt"),	"p_{T}^{Muon} / GeV", "");
	AddTH1D("PtMinus",		  	 ZeeDMisc::FindFile("Binning/LepPt.txt"),	"p_{T}^{Muon} / GeV", "");
	AddTH1D("Eta",	    	 ZeeDMisc::FindFile("Binning/LepEta.txt"),	"#eta^{Muon}",	"", ZeeDHistLevel::Systematics);
	AddTH1D("Charge", 						     7,  	 -2,       2,   "charge" , "");
	//MuonProperties	
	AddTH1D("MuAuthor",	             	        20,     -0.5,    19.5,	"Author_{Muon}","");
//	AddTH1D("ZmumuMass",            	100, 	50.,   150.,	"M_{Zmumu}",	"");
	AddTH1D("CaloMuonID",	                	50,     0.,   5., 	    "CaloID_{Muon}","");
	AddTH1D("IsCombinedMuon",       	         2,     0.,   10.,	    "IsCombined_{Muon}","");
	AddTH1D("Loose",	                         2,     0.,   2.,	    "Loose","");
	AddTH1D("Medium",	                         2,     0.,   2.,	    "Medium","");
	AddTH1D("Tight",	            	         2,     0.,   2.,	    "Tight","");
	AddTH1D("IsStandAloneMuon",	                 2,     0.,   2.,	    "IsStandAlone_{Muon}","");
	AddTH1D("IsSegmentTaggedMuon",               2,     0.,   2.,	    "IsSegmentTagged_{Muon}","");
	AddTH1D("IsSiliconAssociatedForwardMuon",    2,     0.,   2.,	    "IsSiliconAssociatedForward_{Muon}","");
   	AddTH1D("IsTrack",       	     	         2,     0.,   2.,	    "IsTrack","");
//CombineTrack
	AddTH1D("NprecisionLayers",	                 2,     0.,   2.,	    "NprecisionLayers","");
	AddTH1D("NprecisionHoleLayers",              2,     0.,   2.,	    "NprecisionHoleLayers","");
	AddTH1D("D0_exPV",	  	                    50,    -0.2,   0.2,	    "D0_exPV","");
	AddTH1D("Z0_exPV",	            	        50,    -1.,   1.,	    "Z0_exPV","");
//IsolationBag
	AddTH1D("Ptcone20",	            	        50,     0.,   3.,	    "p_{T}^{cone20}","");
	AddTH1D("Ptcone30",	    	                50,     0.,   3.,	    "p_{T}^{cone30}","");
	AddTH1D("Ptcone40",	            	        50,     0.,   3.,	    "p_{T}^{cone40}","");
// histograms for the MuonSpectrometerTrackParticle
	AddTH1D("MStrackPt",	   ZeeDMisc::FindFile("Binning/LepPt.txt"),	    "MStrack_Pt","");
	AddTH1D("MStrackPtexPV",	   ZeeDMisc::FindFile("Binning/LepPt.txt"),	    "MStrack_Pt","");
	AddTH1D("MStrackTheta",	        	        20,     0.,    3.141592,"MStrackTheta","");
	AddTH1D("MStrackPhi",	        	        20,    -3.141592,3.141592,"MStrackPhi","");
    AddTH1D("MStrackEta",	   ZeeDMisc::FindFile("Binning/LepEta.txt"),	    "MStrackEta","");
	AddTH1D("MSd0_exPV",	               	    20,    -0.1,   0.1,	    "MSd0_exPV","");
	AddTH1D("MSsigd0_exPV",	        	        20,    -0.1,   0.1,	    "MSsigd0_exPV","");
	AddTH1D("MSz0_exPV",	        	        20,    -0.2,   0.2,	    "MSz0_exPV","");
	AddTH1D("MSsigz0_exPV",	        	       20,    -0.2,   0.2,	    "MSsigz0_exPV","");
	AddTH1D("MSphi_exPV",	        	       20,     0.,    1.,	    "MSphi_exPV","");
	AddTH1D("MStheta_exPV",	        	       20,     0.,    1.,	    "MStheta_exPV","");
	AddTH1D("MSqoverp_exPV",	               20,     0.,    1., 	    "MSqoverp_exPV","");
//InnerDetectorParticle
	AddTH1D("IDtrackPt",	    ZeeDMisc::FindFile("Binning/LepPt.txt"),	    "IDtrack_Pt","");
	AddTH1D("IDtrackPtexPV",	   ZeeDMisc::FindFile("Binning/LepPt.txt"),	    "MStrack_Pt","");
AddTH1D("IDtrackEta",	     ZeeDMisc::FindFile("Binning/LepEta.txt"), 	"IDtrack_Eta","");
	AddTH1D("IDtrackTheta",	        	       20,     0.,     3.141592,"IDtrack_Theta","");
	AddTH1D("IDtrackPhi",	        	       20,    -3.141592,3.141592,"IDtrack_Phi","");
	AddTH1D("HasInnerDetectorTrack",	       20,     0.,     1., 	"HasInnerDetectorTrack","");
	AddTH1D("fIDd0_exPV",	        	       20,    -0.5,    0.5,	    "fIDd0_exPV","");
	AddTH1D("fIDsigd0_exPV",	               20,    -5.,    5.,	    "fIDsigd0_exPV","");
	AddTH1D("fIDz0_exPV",	        	       20,  -300.,  300.,	    "fIDz0_exPV","");
	AddTH1D("fIDsigz0_exPV",	               20,   -10.,   10.,	    "fIDsigz0_exPV","");
    AddTH1D("fIDd0_exPV_withcharge",	       20,   -0.5,  0.5,	    "fIDd0_exPV_withQ","");
	
	AddTH1D("MuonPtMax",      ZeeDMisc::FindFile("Binning/LepPt.txt"),  "p_{T}^{Muon} / GeV", "");

    // Some extra histograms to see results of selection tool
    AddTH1D("MuonQuality",     5, 0., 5.,"Muon Quality","");
    AddTH1D("PassIDCuts",      2, 0., 2.,"Pass ID cuts","");
    AddTH1D("PassToolCuts",    2, 0., 2.,"Pass Tool","");

	if( (*fAnaOptions)->DoTriggerMatching() && (*fAnaOptions)->UseTrigger() ){
		AddTH1D("IDtrackPhiTrig1Match",	        	   20,    -3.141592,3.141592,"IDtrack_Phi_{TrigMatch}","");
		AddTH1D("EtaTrig1Match",	    	           20,	    -5.,      5.,	"#eta^{Muon}_{TrigMatch}",	"");
	}
    AddTH1D("fID1d0_exPV_withcharge",	       20,   -0.5,  0.5,	    "fID1d0_exPV_withQ","");
	AddTH1D("fID2d0_exPV_withcharge",	       20,   -0.5,  0.5,	    "fID2d0_exPV_withQ","");
	AddTH1D("fID1d0_exPV",	       20,   -0.5,  0.5,	    "fID1d0_exPV","");
    AddTH1D("fID2d0_exPV",	       20,   -0.5,  0.5,	    "fID2d0_exPV","");
}

//-----------------------------------------------------
void ZeeDHistManagerMuon::FillLeptonHists(
    const TObjArray* muonArray, Double_t Weight)
{

  // Get event
  const ZeeDEvent* event = GetEvent();
/*  const ZeeDMuon* mu1 = (ZeeDMuon*)event->GetCurrentBoson()->GetFirstPtLepton();
  const ZeeDMuon* mu2 = (ZeeDMuon*)event->GetCurrentBoson()->GetSecondPtLepton();

  
  if (mu1->IDtrack().IsSet() &&mu2->IDtrack().IsSet()){
	
	  if (TMath::Abs(mu1->IDtrack().Get().fidd0_exPV) > 0.1){
		  FillTH1(mu2->IDtrack().Get().fidd0_exPV, Weight, "fID2d0_exPV");std::cout<<"2d0 "<<mu2->IDtrack().Get().fidd0_exPV<<std::endl;
		FillTH1(mu2->IDtrack().Get().fidd0_exPV * mu2->getCharge() , Weight, "fID2d0_exPV_withcharge");}

	  if (TMath::Abs(mu2->IDtrack().Get().fidd0_exPV) > 0.1){
		FillTH1(mu1->IDtrack().Get().fidd0_exPV, Weight, "fID1d0_exPV");
		FillTH1(mu1->IDtrack().Get().fidd0_exPV * mu1->getCharge() , Weight, "fID1d0_exPV_withcharge");}	 
	 }
*/
  FillTH1(Weight, 1., "EventWeight");
  Double_t maxPt = 0.;

  FillTH1(muonArray->GetEntriesFast(), Weight, "NMuons");

  if ( muonArray->GetEntriesFast() == 2) {

	  const ZeeDMuon* muon1 = static_cast<const ZeeDMuon*>(muonArray->At(0));
	  const ZeeDMuon* muon2 = static_cast<const ZeeDMuon*>(muonArray->At(1));

	  if (muon1->IDtrack().IsSet() && muon2->IDtrack().IsSet()){
	  if ( TMath::Abs(muon1->IDtrack().Get().fidd0_exPV)  > 0.1) {
		  FillTH1(muon2->IDtrack().Get().fidd0_exPV, Weight, "fID2d0_exPV");//std::cout<<"2D0 = "<<muon->IDtrack().Get().fidd0_exPV<<std::endl;
		  FillTH1(muon2->IDtrack().Get().fidd0_exPV * muon2->getCharge(), Weight, "fID2d0_exPV_withcharge");}
	  if ( TMath::Abs(muon2->IDtrack().Get().fidd0_exPV) > 0.1) {
		  FillTH1(muon1->IDtrack().Get().fidd0_exPV, Weight, "fID1d0_exPV");//std::cout<<"2D0 = "<<muon->IDtrack().Get().fidd0_exPV<<std::endl;
		  FillTH1(muon1->IDtrack().Get().fidd0_exPV * muon1->getCharge(), Weight, "fID1d0_exPV_withcharge");}
	  }
  }
  for(Int_t i=0; i != muonArray->GetEntriesFast(); ++i) {
    const ZeeDMuon* muon = static_cast<const ZeeDMuon*>(muonArray->At(i));
	ZeeDLepton::LeptonType::Value type = muon->GetLeptonType();
	const double charge = muon->getCharge();
 	//if (type != ZeeDLepton::LeptonType::muon) 
	//	return;
	//const TLorentzVector& fourVector = muon->GetFourVector();	
   	if ((muon->GetFourVector().Pt()) > maxPt) {
		maxPt  =(muon->GetFourVector().Pt()) ;
		 FillTH1(maxPt,Weight, "MuonPtMax");
	}


	if (muon->Properties().IsSet()) {        
		FillTH1(muon->Properties().Get().fauthor, Weight, "MuAuthor");
		FillTH1(muon->Properties().Get().fisCaloMuonId, Weight, "IsCaloMuonID");
		FillTH1(muon->Properties().Get().fisCombinedMuon, Weight, "IsCombinedMuon");
		FillTH1(muon->Properties().Get().fisStandAloneMuon, Weight, "IsStandAloneMuon");
		FillTH1(muon->Properties().Get().fisSegmentTaggedMuon, Weight, "IsSegmentTaggedMuon");
		FillTH1(muon->Properties().Get().fisSiliconAssociatedForwardMuon, Weight, "IsSiliconAssociatedForwardMuon");
    
    }
    if (muon->CombTrack().IsSet()){
		FillTH1(muon->CombTrack().Get().fnprecisionLayers, Weight, "NprecisionLayers");
		FillTH1(muon->CombTrack().Get().fnprecisionHoleLayers, Weight, "NprecisionHoleLayers");
		FillTH1(muon->CombTrack().Get().fd0_exPV, Weight, "D0_exPV");
		FillTH1(muon->CombTrack().Get().fz0_exPV, Weight, "Z0_exPV");
    }

    if (muon->Isolation().IsSet()){
		FillTH1(muon->Isolation().Get().fptcone20, Weight, "Ptcone20");
		FillTH1(muon->Isolation().Get().fptcone30, Weight, "Ptcone30");
		FillTH1(muon->Isolation().Get().fptcone40, Weight, "Ptcone40");
    }

    if (muon->MStrack().IsSet()){
		FillTH1(muon->MStrack().Get().fmstrackpt, Weight, "MStrackPt");
		double ptMS = fabs(sin(muon->MStrack().Get().fmstheta_exPV)/muon->MStrack().Get().fmsqoverp_exPV);
		FillTH1(ptMS, Weight, "MStrackPtexPV");
		FillTH1(muon->MStrack().Get().fhasMuonSpectrometerTrack, Weight, "HasMuonSpectrometerTrac");
		FillTH1(muon->MStrack().Get().fmstracktheta, Weight, "MStrackTheta");
		FillTH1(muon->MStrack().Get().fmstrackphi, Weight, "MStrackPhi");
		FillTH1(muon->MStrack().Get().fmstracketa, Weight, "MStrackEta");
		FillTH1(muon->MStrack().Get().fmsd0_exPV, Weight, "MSd0_exPV");
		FillTH1(muon->MStrack().Get().fmssigd0_exPV, Weight, "MSsigd0_exPV");
		FillTH1(muon->MStrack().Get().fmsz0_exPV, Weight, "MSz0_exPV");
		FillTH1(muon->MStrack().Get().fmssigz0_exPV, Weight, "MSsigz0_exPV");
		FillTH1(muon->MStrack().Get().fmsphi_exPV, Weight, "MSphi_exPV");
		FillTH1(muon->MStrack().Get().fmstheta_exPV, Weight, "MStheta_exPV");
		FillTH1(muon->MStrack().Get().fmsqoverp_exPV, Weight, "MSqoverp_exPV");

    }

    if (muon->IDtrack().IsSet()){
		//	double charge = muon->getCharge();
		//	double IdD0 = muon->IDtrack().Get().fidd0_exPV;std::cout<<"ID "<<IdD0<<std::endl;
		double ptID  = fabs(sin(muon->IDtrack().Get().fidtheta_exPV)/muon->IDtrack().Get().fidqoverp_exPV);
		FillTH1(ptID, Weight, "IDtrackPtexPV");
//	double idD0charge = charge*IdD0;
		FillTH1(muon->IDtrack().Get().fhasInnerDetectorTrack, Weight, "HasInnerDetectorTrac");
		FillTH1(muon->IDtrack().Get().fidtrackpt, Weight, "IDtrackPt");
		FillTH1(muon->IDtrack().Get().fidtracketa, Weight, "IDtrackEta");
		FillTH1(muon->IDtrack().Get().fidtrackphi, Weight, "IDtrackPhi");
		FillTH1(muon->IDtrack().Get().fidtracktheta, Weight, "IDtrackTheta");
		FillTH1(muon->IDtrack().Get().fidd0_exPV, Weight, "fIDd0_exPV");
		FillTH1(muon->IDtrack().Get().fidz0_exPV, Weight, "fIDz0_exPV");
		FillTH1(muon->IDtrack().Get().fidsigd0_exPV, Weight, "fIDsigd0_exPV");
		FillTH1(muon->IDtrack().Get().fidsigz0_exPV, Weight, "fIDsigz0_exPV");
		FillTH1( muon->IDtrack().Get().fidd0_exPV * muon->getCharge() , Weight, "fIDd0_exPV_withcharge");


	
    }
    	
		
    FillTH1(muon->GetFourVector().Pt(), Weight, "Pt");
	if (charge > 0 )
 	    FillTH1(muon->GetFourVector().Pt(), Weight, "PtPlus");
	else 
   		FillTH1(muon->GetFourVector().Pt(), Weight, "PtMinus");

    FillTH1(muon->GetFourVector().Eta(), Weight, "Eta");


  }


  // fill test part for the Zmumu mass
//  const LinkWeightContainer<ZeeDBosonZmumu*>* zmumuBosons = event->getZmumuBosons();
  // for (LinkWeightContainer<ZeeDBosonZmumu*>::const_iterator it = zmumuBosons->begin();
  //    it != zmumuBosons->end(); ++it) {
  //  FillTH1(it->first->GetMass(), Weight, "ZmumuMass");
  // }

  // FillTH1(maxPt,Weight, "MuonPtMax");
}



