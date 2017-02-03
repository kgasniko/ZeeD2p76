#include "ZeeDHistManager/ZeeDHistManagerPrint.h"

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDBosonZ.h"

#include "ZeeDCuts/ZeeDCutHistManager.h"
#include "ZeeDTools/ZeeDCutBit.h"

#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDTools/ZeeDBags.h"
#define DEBUG_ON 0

#include <TArrayF.h>
#include <iostream>

//------------------------------------------------------
ZeeDHistManagerPrint::ZeeDHistManagerPrint(TString name) : ZeeDHistManager(name)
{
    // Constructor
    Bfile = new TFile(name + ".root", "RECREATE");
    treeOut = new TTree("T", "Test treeOut");

    e1    = new TLorentzVector;
    e2    = new TLorentzVector;
    e1Cl  = new TLorentzVector;
    e2Cl  = new TLorentzVector;
    mZ    = new TLorentzVector;

    ISEM[0] = 0;
    ISEM[1] = 0;

    Author[0] = 0;
    Author[1] = 0;

    OQMaps[0] = 0;
    OQMaps[1] = 0;

    Charge[0] = 0;
    Charge[1] = 0;

    Run = 0;
    LB  = 0;
    Evt = 0;

    treeOut->Branch("e1",   "TLorentzVector", &e1, 16000, 1);
    treeOut->Branch("e2",   "TLorentzVector", &e2, 16000, 1);
    treeOut->Branch("e1Cl", "TLorentzVector", &e1, 16000, 1);
    treeOut->Branch("e2Cl", "TLorentzVector", &e2, 16000, 1);
    treeOut->Branch("Z",    "TLorentzVector", &mZ, 16000, 1);

    treeOut->Branch("ISEM",   &ISEM, "ISEM[2]/S");
    treeOut->Branch("Author", &Author, "Author[2]/S");
    treeOut->Branch("OQMaps", &OQMaps, "OQMaps[2]/S");
    treeOut->Branch("Charge", &Charge, "Charge[2]/S");

    treeOut->Branch("Run",    &Run, "Run/I");
    treeOut->Branch("LB",     &LB,  "LB/I");
    treeOut->Branch("Evt",    &Evt, "Evt/I");

}

//------------------------------------------------------
void ZeeDHistManagerPrint::BookHistos()
{
    DEBUG_MES_START;
    // Books histograms


    DEBUG_MES_END;
}

//------------------------------------------------------
ZeeDHistManagerPrint::~ZeeDHistManagerPrint()
{

}

//------------------------------------------------------
void ZeeDHistManagerPrint::Fill()
{

    // Get event
    const ZeeDEvent* Event = GetEvent();

    const ZeeDBosonZ*   ZBos = Event->GetCurrentBoson();
    const ZeeDElectron* el1 = (ZeeDElectron*)ZBos->GetFirstLep();
    const ZeeDElectron* el2 = (ZeeDElectron*)ZBos->GetSecondLep();

    if ( (ZBos == NULL) || (el1 == NULL) || (el2 == NULL)) {
        return;
    }

    TLorentzVector V1 = el1->GetFourVector();
    TLorentzVector V2 = el2->GetFourVector();

    TLorentzVector V1Cl = el1->Cluster().Get().fourVector;
    TLorentzVector V2Cl = el1->Cluster().Get().fourVector;

    TLorentzVector Z = ZBos->GetFourVector();

    if ( Event->RunNumber().IsSet() ) {
        Run = Event->RunNumber().Get();
        LB  = Event->LB().Get();
        Evt = Event->EventNumber().Get();
    }

    if ( el1->IsEM().IsSet() ) {
        ISEM[0] = el1->IsEM().Get().ZeeDLeptonBags::IsEM::isMedium;
    }

    if ( el2->IsEM().IsSet() ) {
        ISEM[1] = el2->IsEM().Get().ZeeDLeptonBags::IsEM::isMedium;
    }

    if(el1->Author().IsSet()) {
        Author[0] = el1->Author().Get().isElectron;
    }

    if(el2->Author().IsSet()) {
        Author[1] = el2->Author().Get().isElectron;
    }

    Charge[0] = el1->getCharge();
    Charge[1] = el2->getCharge();

    e1->SetPxPyPzE(V1.Px(), V1.Py(), V1.Pz(), V1.E());
    e2->SetPxPyPzE(V2.Px(), V2.Py(), V2.Pz(), V2.E());

    if (el1->Cluster().IsSet()) {
        e1Cl->SetPxPyPzE(V1Cl.Px(), V1Cl.Py(), V1Cl.Pz(), V1Cl.E());
    } else {
        e1Cl->SetPxPyPzE(0, 0, 0, 0);
    }

    if (el2->Cluster().IsSet()) {
        e2Cl->SetPxPyPzE(V2Cl.Px(), V2Cl.Py(), V2Cl.Pz(), V2Cl.E());
    } else {
        e2Cl->SetPxPyPzE(0, 0, 0, 0);
    }


    mZ->SetPxPyPzE(Z.Px(), Z.Py(), Z.Pz(), Z.E());

    treeOut->Fill();

}

//------------------------------------------------------
void ZeeDHistManagerPrint::Finalize()
{

    Info("ZeeDHistManagerPrint::Finalize", " Close TTree ");

    Bfile->Write();
    Bfile->Close();

}
