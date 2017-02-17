#include "ZeeDHistManager/ZeeDHistManagerEtmiss.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>
#include <TH1.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDVertex.h"
#include "ZeeDEvent/ZeeDEtMiss.h"
#include "ZeeDTools/ZeeDBags.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

//#define USE_DEBUG_MESSAGES
#include "ZeeDMisc/ZeeDMisc.h"

#include "ZeeDTools/ZeeDCutBit.h"

//-----------------------------------------------------
void ZeeDHistManagerEtmiss::BookHistos()
{
    // Books histograms

    // Get analysis svc
    ServiceHandle<IZeeDAnalysisSvc>* ZeeDAnaOptions = fSvcHelper.GetAnaSvc();

    // Do we run on data or MC?
    if ( (*ZeeDAnaOptions)->IsMC() ) {
        bIsMC = kTRUE;
    }


    // book histograms
    AddTH1D("EventWeight",  1010,    -2.,   200.0,  "Event weight",    "");
    AddTH1D("EtMiss"    ,         50,     0.0,  100.0, "E_{T}^{miss} [GeV]",     "" );    
/*    AddTH1D("EtMiss0Jet"    ,         100,     0.0,  100.0, "E_{T}^{miss} [GeV]",     "" );    
    AddTH1D("EtMiss1Jet"    ,         100,     0.0,  100.0, "E_{T}^{miss} [GeV]",     "" );    
    AddTH1D("EtMiss2Jet"    ,         100,     0.0,  100.0, "E_{T}^{miss} [GeV]",     "" );   */ 
    AddTH1D("EtPhi",  50, -200., 200., "#varphi^{elec} / deg", "");
/*    AddTH1D("MET_Electron",    100,     0.0,  100.0, "len(elec) in MET composition",     "" );  
    AddTH1D("MET_Muon",   100,     0.0,  100.0, "len(muon) in MET composition",     "" ); 
    AddTH1D("MET_Tau",   10,     1.0,  500.0, "E_{T}^{miss} [GeV]",     "" ); 
    AddTH1D("MET_Jet",   50,     2.0,  60.0, "E_{T}^{miss} [GeV]",     "" ); 
    AddTH1D("MET_Soft",   100,     0.0,  100.0, "E_{T}^{miss} [GeV]",     "" ); 
    AddTH1D("MET_Photon",   10,     1.0,  50.0, "E_{T}^{miss} [GeV]",     "" ); 
    AddTH1D("MET_Truth",    100,     0.0,  100.0, "E_{T}^{miss} [GeV]",     "" );
    AddTH1D("SoftElecGreat35", 100, 0., 100., "E_{T}^{miss} [GeV]", "");
    AddTH1D("SoftElecLess35", 100, 0., 100., "E_{T}^{miss} [GeV]", "");
    AddTH1D("SoftMuonGreat35", 100, 0., 100., "E_{T}^{miss} [GeV]", "");
    AddTH1D("SoftMuonLess35", 100, 0., 100., "E_{T}^{miss} [GeV]", "");
    AddTH1D("SoftLowPileup", 100, 0., 100., "E_{T}^{miss} [GeV]", "");
    AddTH1D("SoftHighPileup", 100, 0., 100., "E_{T}^{miss} [GeV]", "");
    AddTH1D("EtMissLowPileup", 100, 0., 100., "E_{T}^{miss} [GeV]", "");
    AddTH1D("EtMissHighPileup", 100, 0., 100., "E_{T}^{miss} [GeV]", "");
    AddTH1D("EtMissElecGreat35", 100, 0., 100., "E_{T}^{miss} [GeV]", "");
    AddTH1D("EtMissElecLess35", 100, 0., 100., "E_{T}^{miss} [GeV]", "");
    AddTH1D("EtMissMuonGreat35", 100, 0., 100., "E_{T}^{miss} [GeV]", "");
    AddTH1D("EtMissMuonLess35", 100, 0., 100., "E_{T}^{miss} [GeV]", "");
    AddTH1D("Elecwpx", 100, 0., 2., "E_{T}^{miss} wpx" , "");
    AddTH1D("Elecwpy", 100, 0., 2., "E_{T}^{miss} wpy", "");
    AddTH1D("Muonwpx", 100, 0., 2., "wpx muon", "");*/
    AddTH1D("CorRecoilEt", 50, 0., 100., "E_{T}^{Hadron Recoil} [GeV]", "");
    AddTH1D("CorRecoilSumet", 50, 0., 150., "Sum E_{T}^{Hadron Recoil} [GeV]", "");
    AddTH1D("CorRecoilPhi", 50, -4., 4., "#phi^{Hadron Recoil} [GeV]", "");
//    AddTH1D("Muonwpy", 100, 0., 2., "wpy muon", "");
    AddTH1D("CorRecoilEtMiss", 50, 0., 100., "EtMiss^{Hadron Recoil} [GeV]");
    AddTH1D("nMETElec", 5, 0, 5, "N Electrons in METComposition");
    AddTH1D("nMETMuon", 5, 0, 5, "N Muons in METComposition");
    //    AddTH2D("EtmissEtaPhi", 50, -5., 5., 50, -200., 200, "Eta", "Phi"); // fill me!
    AddTH2D("EtmissXY", 40, -100., 100., 40, -100., 100, "EtX", "EtY");
/*    AddTH2D("SoftVsMuon" , 100, 0.0, 100., 100, 0.0, 100., "Soft Term", "Muon EtMiss");
    AddTH2D("SoftVsElectron", 100, 0.0, 100., 100, 0.0, 100., "Soft Term", "Electron EtMiss");*/
    AddTH2D("HadrRecoilvsEtMiss", 50, 0.0, 100., 50, 0.0, 100., "HadronRecoil EtMiss", "Standart EtMiss");


}

//-----------------------------------------------------
void ZeeDHistManagerEtmiss::Fill()
{

    // Get an event
    const ZeeDEvent* event = GetEvent();

    CHECK_NULL_PTR(event);

    // Event weight
    Double_t Weight = event->GetWeight();
    FillTH1(Weight, 1., "EventWeight");
    const ZeeDBosonW* w = event->GetCurrentBosonW();
    if (w == NULL) {
        return;
    }
    const ZeeDLepton* lep = w->GetLep();
    const TLorentzVector lepLV = lep->GetFourVector();


    // Fill Et miss variables
    const TObjArray* etmissarr = event->GetEtMissArray();
    for (int i = 0; i < etmissarr->GetEntriesFast(); ++i) {
        ZeeDEtMiss* etmiss = static_cast< ZeeDEtMiss* >( etmissarr->At(i) );

        TLorentzVector etmissLV;
        etmissLV.SetPxPyPzE(etmiss->GetCorRecoilEtX(), etmiss->GetCorRecoilEtY(), 0, etmiss->GetCorRecoilEt());
        etmissLV+=lepLV;
        FillTH1(etmissLV.Et(), Weight, "CorRecoilEtMiss");

        FillTH1(etmiss->GetEt(),  Weight, "EtMiss");
        FillTH1(etmiss->GetPhi() * ZeeDMisc::Rad2Deg, Weight, "EtPhi");
        FillTH1(etmiss->GetCorRecoilEt(), Weight, "CorRecoilEt");
        FillTH1(etmiss->GetCorRecoilPhi(), Weight, "CorRecoilPhi");
        FillTH1(etmiss->GetCorRecoilSumet(), Weight, "CorRecoilSumet");
        FillTH2(etmissLV.Et(), etmiss->GetEt(), Weight, "HadrRecoilvsEtMiss");
        FillTH1(etmiss->GetnMETElec(), Weight, "nMETElec");
        FillTH1(etmiss->GetnMETMuon(), Weight, "nMETMuon");

        /*      FillTH1(etmiss->GetElecEt(), Weight, "MET_Electron");
                FillTH1(etmiss->GetMuonEt(), Weight, "MET_Muon");
                FillTH1(etmiss->GetPhotonEt(), Weight, "MET_Photon");
                FillTH1(etmiss->GetTauEt(), Weight, "MET_Tau");
                FillTH1(etmiss->GetJetEt(), Weight, "MET_Jet");
                FillTH1(etmiss->GetTruth(), Weight, "MET_Truth");
                FillTH1(etmiss->GetMET_SoftTermETSUM(), Weight, "MET_Soft");
                FillTH2(etmiss->GetMET_SoftTermETSUM(), etmiss->GetMuonEt(), Weight, "SoftVsMuon");
                FillTH2(etmiss->GetMET_SoftTermETSUM(), etmiss->GetElecEt(), Weight, "SoftVsElectron");
                if (etmiss->GetElecEt() > 35){
                FillTH1(etmiss->GetEt(), Weight, "EtMissElecGreat35");
                FillTH1(etmiss->GetMET_SoftTermETSUM(), Weight, "SoftElecGreat35");
                }
                else{
                FillTH1(etmiss->GetEt(), Weight, "EtMissElecLess35");
                FillTH1(etmiss->GetMET_SoftTermETSUM(), Weight, "SoftElecLess35");
                }
                if (etmiss->GetMuonEt() > 35){
                FillTH1(etmiss->GetEt(), Weight, "EtMissMuonGreat35");
                FillTH1(etmiss->GetMET_SoftTermETSUM(), Weight, "SoftMuonGreat35");
                }
                else{
                FillTH1(etmiss->GetEt(), Weight, "EtMissMuonLess35");
                FillTH1(etmiss->GetMET_SoftTermETSUM(), Weight, "SoftMuonLess35");
                }
                if (event->AverageInteractionsPerCrossing().Get() < 0.25){
                FillTH1(etmiss->GetMET_SoftTermETSUM(), Weight, "SoftLowPileup");
                FillTH1(etmiss->GetEt(), Weight, "EtMissLowPileup");
                }
                else{ 
                FillTH1(etmiss->GetMET_SoftTermETSUM(), Weight, "SoftHighPileup");
                FillTH1(etmiss->GetEt(), Weight, "EtMissHighPileup");
                }
                Int_t nObjMET(etmiss->GetMETCompositionElectron_index().size());
                for ( Int_t iMET = 0; iMET < nObjMET; iMET++ ){
                FillTH1(etmiss->GetMETCompositionElectron_wpx()[ iMET ], Weight, "Elecwpx");
                FillTH1(etmiss->GetMETCompositionElectron_wpy()[iMET], Weight,"Elecwpy");
                }
                Int_t nObjMETMuon(etmiss->GetMETCompositionMuon_index().size());
                for ( Int_t iMET = 0; iMET < nObjMETMuon; iMET++ ){
                FillTH1(etmiss->GetMETCompositionMuon_wpx() [iMET], Weight, "Muonwpx");
                FillTH1(etmiss->GetMETCompositionMuon_wpy() [iMET], Weight, "Muonwpy");	
                }
        //std::cout<< etmiss->GetJetEt() << std::endl;
        if (etmiss->GetJetEt() == 0) {
        FillTH1(etmiss->GetEt(), Weight, "EtMiss0Jet");
        } else{
        FillTH1(etmiss->GetEt(), Weight, "EtMiss1Jet");
        }*/ 
        /*else if (jets->GetEntries() == 1) {
          FillTH1(etmiss->GetEt(), Weight, "EtMiss1Jet");
          } else if (jets->GetEntries() == 2) {
          FillTH1(etmiss->GetEt(), Weight, "EtMiss2Jet");
          }*/
        //      FillTH2(etmiss->GetPhi(), etmiss->GetEta(), Weight, "EtmissEtaPhi");	// fill me!
        FillTH2(etmiss->GetEtX(), etmiss->GetEtY(), Weight, "EtmissXY");

    }

}

