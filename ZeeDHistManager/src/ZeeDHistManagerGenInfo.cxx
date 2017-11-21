#include "ZeeDHistManager/ZeeDHistManagerGenInfo.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDGenParticle.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

//#define USE_DEBUG_MESSAGES
#include "ZeeDMisc/ZeeDMisc.h"

using std::cout;
using std::endl;

//-----------------------------------------------------
void ZeeDHistManagerGenInfo::BookHistos()
{
    // Books histograms

    // Get analysis svc
    fZeeDAnaOptions = fSvcHelper.GetAnaSvc();

    const TString BosY      = ZeeDMisc::FindFile("Binning/BosY.txt");
    const TString BosAbsY   = ZeeDMisc::FindFile("Binning/BosAbsY.txt");
    const TString ElecEta   = ZeeDMisc::FindFile("Binning/ElecEta.txt");
    const TString BosPt     = ZeeDMisc::FindFile("Binning/BosPt.txt");

    AddTH1D("BosPt",        400, 0.0, 200.0,   "P_{T} (Z->ee) ", "", ZeeDHistLevel::Systematics);
    AddTH1D("BosID",         60,  -30, 30, "BosonID", "");
    AddTH2D("RecVsGenBosPt", 60, 0.0, 60.0, 60, 0.0, 60.0, "Rec P_{T}^{W} [GeV]", "Truth P_{T}^{W} [GeV]"); 
    AddTH2D("BosYPt",   100, -3.5, 3.5, 400, 0, 200., "Y", "Pt");
    AddTH1D("BosPtRew", ZeeDMisc::FindFile("Binning/BosPtRew.txt"), "P_{T}", "");
    AddTH1D("BosPtFine",  500, 0, 500,  "P_{T} (Z->ee) / GeV", "", ZeeDHistLevel::Technical);
    AddTH1D("BosMt",        100, 0, 100, "M_genBoson", "", ZeeDHistLevel::Systematics);
//    AddTH1D("Weight",  400 ,    -2.,   4.0,  "Event weight",    "");
//    AddTH1D("weight", 50, 0.0, 2.0, "event weight", "", ZeeDHistLevel::Systematics);
    AddTH2D("WeightPtBos", 400 ,    -2.,   4.0,  60,    0.0, 60.0, "Weight", "Pt_{boson}^{truth} [GeV]", ZeeDHistLevel::Systematics);
    AddTH1D("BosY",          BosY,     "Y (Z->ee) ", "", ZeeDHistLevel::ToyMC);
    AddTH1D("BosYFine",   500, -5, 5,   "y (Z->ee)",        "", ZeeDHistLevel::Technical);
    AddTH1D("Wminenu", 1, 0., 1., "", "");
    AddTH1D("Wminmunu", 1, 0., 1., "","");
    AddTH1D("Wplusenu", 1, 0., 1., "", "");
    AddTH1D("Wplusmunu", 1, 0., 1., "", "");
    AddTH1D("BosMass",       50,    66., 116.0, "M (Z->ee) / GeV", "");

    AddTH1D("BosMassFine",      500,   0, 250, "M_{Z->ee} / GeV",     "", ZeeDHistLevel::Technical);

    // Add bootstrap to get proper errors:
    if ( (*fZeeDAnaOptions)->genBootstrapRnd() ) {
    }

    AddTH1D("ZPosVtx",       30,  -300.,  300., "Z_{vtx} / mm",    "");
    //AddTH1D("BosMt",        100,   0.0, 100.0,         "M_{T}",    "");

  
    AddTH1D("PosPtBare",         50,    0.0, 100.0, "P_{T} e^+ / GeV", "", ZeeDHistLevel::Systematics);
    AddTH1D("PosPt",         50,    0.0, 100.0, "P_{T} e^+ / GeV", "", ZeeDHistLevel::Systematics);
    AddTH1D("PosEta",        50,    -5.0, 5.0,  "#eta e^+  ", "");
    AddTH1D("PosEt",         50,    0.0, 100.0, "E_{T} e^+" , "");
    AddTH1D("PosID",         40,    -20, 20, "Particle ID", "");

    AddTH1D("ElecPtBare",        50,    0.0, 100.0, "P_{T} e^- / GeV", "", ZeeDHistLevel::Systematics);
    AddTH1D("ElecPt",        50,    0.0, 100.0, "P_{T} e^- / GeV", "", ZeeDHistLevel::Systematics);
    AddTH1D("ElecEta",       50,    -5.0, 5.0,  "#eta e^- ", "");
    AddTH1D("ElecEt",       100,    0, 100, "E_{T} e", " ");
    AddTH1D("ElecID",       40,    -20, 20, "Particle ID", "");
    AddTH1D("EtMiss",         100,    0, 100, "E_{T} neutrino", " ");
    AddTH1D("id",            13,    -6.5,  6.5,  "parton ID", "");
    AddTH2D("id1_vs_id2",    13,    -6.5,  6.5, 13,    -6.5,  6.5, "parton ID 1", "parton ID 2");
    AddTH1D("x",            120,     0.0,  0.3, "parton X", "");
    AddTH1D("Q2",           120,    20.0,  200.0, "parton Q^2", "");
    AddTH1D("PDF",          120,     0.0,  500000.0,  "PDF", "");
    AddTH2D("xQ2",           50,     0.0, 0.3,  120,  20.0, 200.0, "X", "Q^2");
    AddTH2D("id1_id2",       40,  -20., 20., 40, -20., 20., "particle id1 particle id2", "dsasd");
    AddTH1D("GenEventWeight",  100,    -10,   10.0,  "Gen Event weight",    "", ZeeDHistLevel::Systematics);

}

//-----------------------------------------------------
void ZeeDHistManagerGenInfo::Fill()
{
    DEBUG_MES_START;
    DEBUG_MES(this->GetHMName());

    // Get event
    const ZeeDEvent* event = GetEvent();
    DEBUG_VAR(event);
    CHECK_NULL_PTR(event);

    double genWeight = event->GetGenWeight();
    DEBUG_VAR(genWeight);
    FillTH1(genWeight, 1., "GenEventWeight");

    if (event->PDF().IsSet() ){
        double id1 = event->PDF().Get().id1;
	double id2 = event->PDF().Get().id2;
	if ( id1 == 21 ) { id1 = 0;}
	if ( id2 == 21 ) { id2 = 0;}
        DEBUG_VAR(id1);
        FillTH1(id1,       genWeight, "id");
        FillTH1(id2,       genWeight, "id");
        FillTH2(id1, id2,  genWeight, "id1_vs_id2");
        FillTH1(event->PDF().Get().x1,	    genWeight, "x");
        FillTH1(event->PDF().Get().x2,	    genWeight, "x");
        FillTH1(event->PDF().Get().scalePDF,  genWeight, "Q2");
        FillTH1(event->PDF().Get().pdf1,	    genWeight, "PDF");
        FillTH1(event->PDF().Get().pdf2,	    genWeight, "PDF");
        FillTH2(event->PDF().Get().x1,  event->PDF().Get().scalePDF,  genWeight, "xQ2");
        FillTH2(event->PDF().Get().x2,  event->PDF().Get().scalePDF,  genWeight, "xQ2");
    }

    if (event->Vertex().IsSet() ){
        TVector3 vtx = event->Vertex().Get().position;
        FillTH1(vtx.Z(),       genWeight, "ZPosVtx");
    }
    const ZeeDGenParticle* Boson = event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);    
    if (Boson == NULL) {
        return;
    }

    int id = Boson->GetParticleID();

    ZeeDGenElectrons genLeptons = event->GetGenLeptons(ZeeDEnum::MCFSRLevel::Born);
    const ZeeDGenParticle* El    = genLeptons.first;
    const ZeeDGenParticle* Pos   = genLeptons.second;

    // Fill information without cuts
    const TLorentzVector& fourVector = Boson->GetMCFourVector();
    FillTH1(fourVector.Pt(),       genWeight, "BosPtRew");
    FillTH2(genWeight, fourVector.Pt(), 1, "WeightPtBos");
    FillTH1(fourVector.Pt(),       genWeight, "BosPt");
    FillTH1(fourVector.Pt(),       genWeight, "BosPtFine");

    FillTH1(fourVector.Rapidity(), genWeight, "BosY");
    FillTH2(fourVector.Rapidity(), fourVector.Pt(), genWeight, "BosYPt");
    FillTH1(fourVector.Rapidity(), genWeight, "BosYFine");

    FillTH1(fourVector.M(),        genWeight, "BosMass");
    FillTH1(fourVector.M(),        genWeight, "BosMassFine");
    FillTH1(Boson->GetMt(),       genWeight, "BosMt");

    const ZeeDBosonW* boson = event->GetCurrentBosonW();
    if (boson != NULL) {
        const TLorentzVector bos4Vec= boson->GetFourVector();
        FillTH2(bos4Vec.Pt(), fourVector.Pt(), genWeight, "RecVsGenBosPt");
    }



	TLorentzVector sumLept;
	sumLept=fourVector;

    if (El != NULL) {
        const TLorentzVector& ElFV  = El->GetMCFourVector();
        FillTH1(ElFV.Pt(),       genWeight, "ElecPt");
        FillTH1(ElFV.Eta(),      genWeight, "ElecEta");
        FillTH1(ElFV.Et(),       genWeight, "ElecEt"); 
        FillTH1(El->GetParticleID(), genWeight, "ElecID");
        if (id == 24 || id == -24) {
          double EtMiss = fourVector.Et()-ElFV.Et();
          FillTH1(EtMiss, genWeight, "EtMiss");
          }
    }

    if (Pos != NULL) {
        const TLorentzVector& PosFV = Pos->GetMCFourVector();
        FillTH1(PosFV.Pt(),      genWeight, "PosPt");
        FillTH1(PosFV.Eta(),     genWeight, "PosEta");
        FillTH1(PosFV.Et(),      genWeight, "PosEt");
        FillTH1(Pos->GetParticleID(), genWeight, "PosID");

    }
    //std::cout << "Filling gen information " << Pos->GetParticleID() << " " << El->GetParticleID() << std::endl;
    FillTH1(id, genWeight, "BosID");
    
    if (id == - 24){
           if (El->GetParticleID() == 12){
              FillTH1(0.5, genWeight, "Wplusenu");
           }else{
               FillTH1(0.5, genWeight, "Wplusmunu");
           }
    } else if (id == 24) {
        if (El->GetParticleID()==11){
            FillTH1(0.5, genWeight, "Wminenu");
        } else{
            FillTH1(0.5, genWeight, "Wminmunu");
        }
    }


	ZeeDGenElectrons genLeptonsBare = event->GetGenLeptons(ZeeDEnum::MCFSRLevel::Bare);
    const ZeeDGenParticle* ElBare    = genLeptonsBare.first;
    const ZeeDGenParticle* PosBare   = genLeptonsBare.second;

    if (ElBare != NULL) {
        const TLorentzVector& ElFV  = ElBare->GetMCFourVector();
        FillTH1(ElFV.Pt(),       genWeight, "ElecPtBare");
    }

    if (PosBare != NULL) {
        const TLorentzVector& PosFV = PosBare->GetMCFourVector();
        FillTH1(PosFV.Pt(),      genWeight, "PosPtBare");
    }

    //FillTH2(El->GetParticleID(), Pos->GetParticleID(), genWeight, "id1_id2");

}
