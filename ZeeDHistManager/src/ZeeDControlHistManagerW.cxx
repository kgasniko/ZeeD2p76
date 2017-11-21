#include "ZeeDHistManager/ZeeDControlHistManagerW.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"

#include "ZeeDEvent/ZeeDBosonW.h"
#include "ZeeDEvent/ZeeDTrack.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDJet.h"
#include "ZeeDEvent/ZeeDVertex.h"
#include "ZeeDEvent/ZeeDGenParticle.h"
#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDControlHistManagerW::ZeeDControlHistManagerW(TString name) : ZeeDHistManager(name)
{
    // Named constructor
}

//------------------------------------------------------
ZeeDControlHistManagerW::~ZeeDControlHistManagerW()
{
    // Destructor

}

//------------------------------------------------------
void ZeeDControlHistManagerW::BookHistos()
{
    // Books histogram
    AddTH1D("lepEt",    ZeeDMisc::FindFile("Binning/LepPt.txt"), "E_{T}^{lep}  [GeV]",  "");
    AddTH1D("weight" ,        50,    0.0,   2.0, "Weight of event",        "", ZeeDHistLevel::Systematics);
    AddTH1D("lepEta",   ZeeDMisc::FindFile("Binning/LepEta.txt"), "#eta^{lep}",          "", ZeeDHistLevel::Systematics);
    AddTH1D("lepEtaFine", ZeeDMisc::FindFile("Binning/ElecEta.txt"), "#eta^{lep}", "", ZeeDHistLevel::Systematics);
    AddTH1D("lepPhi",   20,   -4.0,   4.0, "#varphi^{} [rad]", "", ZeeDHistLevel::Systematics);
    AddTH1D("dPhi" ,    20,   -4.0,   4.0, "d#varphi^{} [rad]", "");
    AddTH1D("lepPt",      ZeeDMisc::FindFile("Binning/LepPt.txt"), "P_{T}^{lep} [GeV]","", ZeeDHistLevel::Systematics);
    AddTH1D("eNEvent",        20,    0.0,  20.0, "Number of electrons in event", "");
    AddTH1D("mu",             60,    0.0,   1.0, "Average number of interactions per bunch crossing", "Mu");
    AddTH1D("wCharge",         5,   -2.5,   2.5, "W boson charge",         "" );

    //    AddTH1D("ElecEt",         20,    0.0, 100.0, "E_{T}^{elec}  [GeV]", "" );
    //    AddTH1D("ElecEta",        50,   -2.5,   2.5, "#eta^{elec}",         "" );
    //    AddTH1D("ElecPhi",        20, -180.0, 180.0, "#varphi^{elec} [deg]","" );

    AddTH1D("etMiss",  ZeeDMisc::FindFile("Binning/etMiss.txt"), "E_{T}^{miss} [GeV]",     "", ZeeDHistLevel::Systematics);
    AddTH1D("mtW",     ZeeDMisc::FindFile("Binning/BosMassTransv.txt"), "M_{T} [GeV]",         "", ZeeDHistLevel::Systematics);
    AddTH1D("ptW",            60,    0.0, 60.0, "P_{T}^{W} [GeV]",         "" , ZeeDHistLevel::Systematics);
    AddTH1D("BosY",		250,	-5.,   5.,	"y^{W}",		"", ZeeDHistLevel::ToyMC);
    AddTH2D("etMiss_ElecClustEt",  20, 0.0, 100.0, 19, 20.0, 100.0, "E_{T}^{miss} [GeV]", "E_{T}^{Cluster}  [GeV]");
    AddTH2D("etMiss_MtW",          20, 0.0, 100.0, 24,  0.0, 120.0, "E_{T}^{miss} [GeV]", "M_{T, W} [GeV]");
    AddTH2D("clustEt_MtW",         20, 0.0, 100.0, 24,  0.0, 120.0, "E_{T}^{cluster} [GeV]", "M_{T, W} [GeV]");
    AddTH2D("clustEt_PtW",         20, 0.0, 100.0,200,  0.0, 100.0, "E_{T}^{cluster} [GeV]", "P_{T, W} [GeV]"); 
    AddTH2D("mu_weight",           20, 0.0,   1.0,  20,  0.0,    3.0, "#mu",   "weight");
    AddTH2D("MtW_Phi",             20, 0.0, 100.0, 100, -4.0,    4.0, "M_{T}^{W} [GeV]", "#varphi^{} [rad]");    
    AddTH2D("Pt_Phi",              20, 0.0, 100.0, 100, -4.0,    4.0, "P_{T}{lep} [GeV]", "#varphi^{} [rad]");    
    AddTH2D("Pt_EtMiss", 		   20, 0.0, 100.0, 100,  0.0,  100.0, "P_{T}{lep} [GeV]", "E_{T}^{miss} [GeV]");
    AddTH2D("SumEt_ptWTruth",           60, 0.0, 230.0,  60,  0.0,  120.0, "SumEt [GeV]", "P_{T,W} [GeV]");
    AddTH2D("ptW_ptWTruth",        60, 0.0, 120.0,  60,  0.0,  120.0, "P_{T,W} [GeV]", "P_{T,W} [GeV]");
    AddTH1D("ptWTruth",            60, 0.0, 120.0, "P_{T,W} [GeV]", "");
    AddTH2D("SumEt_ptW",           60, 0.0, 230.0,  60,  0.0,  120.0, "SumEt [GeV]", "P_{T,W} [GeV]");
    AddTH2D("SumEtCor_ptW",        60, -50.0, 230.0,  60,  0.0,  120.0, "SumEt-Et [GeV]", "P_{T,W} [GeV]");

    AddTH1D("jetPt",          50,    0.0, 100.0, "Jet P_{T} / GeV",        "" );
    AddTH1D("jetEta",         50,   -5.0,   5.0, "Jet #eta",               "" );
    AddTH1D("jetPhi",         60, -180.0, 180.0, "Jet #varphi / deg",      "" );
    AddTH1D("jetElecR",      300,    0.0,   3.0, "Radius between jet and electron", "");
    AddTH1D("elecIsoJet",     40,    0.0,  40.0, "Electron track isolation", "");   
    AddTH1D("trackElecR",    300,    0.0,   3.0, "Radius between jet and electron", "");
    AddTH1D("elecIsoTr",      40,    0.0,   2.0, "Electron track isolation", "");
    AddTH1D("numOfJets",      20,    0.0,  20.0, "Number of jets",         "" );
    AddTH1D("emFraction",     20,    0.0,   2.0, "e/m fraction",           "" );

    AddTH1D("vtxNTracks",     50,    0.0,  50.0, "Number of vertex tracks", "" );
    AddTH1D("vtxX",           50,   -0.2,   0.2, "X_{vtx} [cm]",           "" );
    AddTH1D("vtxY",           50,   -0.2,   0.2, "Y_{vtx} [cm]",           "" );
    AddTH1D("vtxZ",           30,  -30.0,  30.0, "Z_{vtx} [cm]",           "" );

}

//------------------------------------------------------
void ZeeDControlHistManagerW::Fill()
{
    // Fills histos
    DEBUG_MES_START;

    // ----- Define pointers to histograms -------------





    // ----- Calculate event quantities ----------------
    // Get event
    const ZeeDEvent* event = GetEvent();
    CHECK_NULL_PTR(event);
    const Double_t Weight = event->GetWeight();
    FillTH1(Weight, 1, "weight");    
    /*    if (fSys->isShiftInUse(ZeeDSystematics::eOffTrigToyMC)){
          std::cout << Weight << std::endl;
          }
          */
    if ( event->AverageInteractionsPerCrossing().IsSet() ) {
        const Double_t Mu = event->AverageInteractionsPerCrossing().Get();
        FillTH1(Mu, Weight, "mu");
        FillTH2(Mu, Weight, 1, "mu_weight");
    }


    const ZeeDBosonW* boson = event->GetCurrentBosonW();
    if (boson == NULL) return;

    FillTH1(boson->GetFourVector().Rapidity(), Weight, "BosY");


    const ZeeDLepton* lep = boson->GetLep();
    const TLorentzVector lep4Vec = lep->GetFourVector();
    Double_t lepEt  = lep4Vec.Et();
    Double_t lepEta = lep4Vec.Eta();
    Double_t lepPhi = lep4Vec.Phi();
    Double_t lepPt       = lep4Vec.Pt();

    FillTH1(lepPt,       Weight, "lepPt");
    FillTH1(lepEt,  Weight, "lepEt");
    FillTH1(lepEta, Weight, "lepEta");
    FillTH1(lepEta, Weight, "lepEtaFine");
    FillTH1(lepPhi, Weight, "lepPhi");


    ZeeDElectron* electron = (ZeeDElectron*)lep;
    if (electron == NULL) return;

    const Int_t nElec=event->GetElectrons()->GetEntriesFast();
    FillTH1(nElec, Weight, "eNEvent");
    const TLorentzVector elec4Vec = electron->GetFourVector();
    /*   const TLorentzVector clust4Vec = electron->Cluster().Get().fourVector;

         Double_t elecClustEt  = clust4Vec.Et();
         Double_t elecClustEta = clust4Vec.Eta();
         Double_t elecClustPhi = clust4Vec.Phi();
         Double_t elecPt       = elec4Vec.Pt();

         FillTH1(elecPt,       Weight, "elecPt");
         FillTH1(elecClustEt,  Weight, "elecClustEt");
         FillTH1(elecClustEta, Weight, "elecClustEta");
         FillTH1(elecClustPhi, Weight, "elecClustPhi");
         */     
    Double_t elecCharge = electron->getCharge();
    FillTH1(elecCharge, Weight, "wCharge");

    const ZeeDEtMiss* etMiss = (ZeeDEtMiss*)event->GetEtMissArray()->At(0);
    Int_t nObjMET(etMiss->GetMETCompositionElectron_index().size());
    FillTH1(nObjMET, Weight, "nMETElec");
    nObjMET = (etMiss->GetMETCompositionMuon_index().size());
    FillTH1(nObjMET, Weight, "nMETMuon");

    TLorentzVector etVec;
    etVec.SetPxPyPzE(etMiss->GetCorRecoilEtX(), etMiss->GetCorRecoilEtY(), 0, etMiss->GetCorRecoilEt());
    etVec-=elec4Vec;
    Double_t dPhi    =  etVec.Phi();
    FillTH1(dPhi, Weight, "dPhi");
    Double_t mtW     = boson->GetMt();
    TLorentzVector bosVec = boson->GetFourVector();
    Double_t pTrBos  = bosVec.Pt();
    Double_t sumEt= etMiss->GetCorRecoilSumet();
    Double_t corRecoilEt=etMiss->GetCorRecoilEt();

    FillTH1(pTrBos, Weight, "ptW");

    FillTH1(boson->GetEtMiss(), Weight, "etMiss");
    FillTH1(mtW,    Weight, "mtW");

    FillTH2(etMiss->GetEt(), mtW, Weight, "etMiss_MtW");

    FillTH2(etMiss->GetEt(), dPhi, Weight, "etMiss_Phi");
    FillTH2(mtW,    dPhi, Weight, "MtW_Phi");
    FillTH2(sumEt, pTrBos, Weight, "SumEt_ptW");
    FillTH2(sumEt-1.75*corRecoilEt, pTrBos, Weight, "SumEtCor_ptW");

    if ((*fAnaOptions)->IsMC()){ 
        const ZeeDGenParticle* Boson = event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);
        if (Boson != NULL){
            Double_t pTTruth = Boson->GetMCFourVector().Pt();
            FillTH2(sumEt, pTTruth, Weight, "SumEt_ptWTruth");
            FillTH2(pTrBos, pTTruth, Weight, "ptW_ptWTruth"); 
            FillTH1(pTTruth, Weight, "ptWTruth");
        }
    }
    FillTH2(lepPt,  dPhi, Weight, "Pt_Phi");
    FillTH2(lepPt, etMiss->GetEt(), Weight, "Pt_EtMiss");
    if (lepPt>32) {
        FillTH1(etMiss->GetEt(), Weight, "EtMissGreat40");
    }	else 
        FillTH1(etMiss->GetEt(), Weight, "EtMissLess40");
    // Get array of jets
    //const TObjArray* jets = event->GetJets();

    //FillTH1(jets->GetEntriesFast(),  Weight, "numOfJets");
    Double_t sumPt = 0;
    Double_t elecIso=sumPt/electron->GetFourVector().Pt();
    FillTH1 (elecIso, Weight, "elecIsoJet");
    sumPt=0;

    const TObjArray* tracks = event->GetTracks();
    for (Int_t index = 0; index < tracks->GetEntriesFast(); index++){
        ZeeDTrack* track = static_cast<ZeeDTrack*> ( tracks->At(index));
        TLorentzVector fourVector  = track->GetFourVector();
    }

    std::string flavour = event->GetJetFlavour();
    FillTH1(flavour.c_str(), Weight, "flavour");

    // Get array of vertices
    const TObjArray* vertices = event->GetVertices();

    // Get event primary vertex
    ZeeDVertex* vertex = static_cast< ZeeDVertex* >( vertices->At(0) );
    TVector3 vtxPos = vertex->GetVertexCandidate();
    vtxPos *= 0.1;  // Convert mm -> cm

    FillTH1(vtxPos.X(),  Weight, "vtxX");
    FillTH1(vtxPos.Y(),  Weight, "vtxY");
    FillTH1(vtxPos.Z(),  Weight, "vtxZ");

    Int_t numTracks = vertex->GetNTrack();
    FillTH1(numTracks,  Weight, "vtxNTracks");

    DEBUG_MES_END;
}
