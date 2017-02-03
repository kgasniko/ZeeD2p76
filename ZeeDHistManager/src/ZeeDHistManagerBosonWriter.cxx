#include "ZeeDHistManager/ZeeDHistManagerBosonWriter.h"

//#define USE_DEBUG_MESSAGES
#include "ZeeDMisc/ZeeDMisc.h"

#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDEvent.h"

#include <TLorentzVector.h>


//-----------------------------------------------------
ZeeDHistManagerBosonWriter::ZeeDHistManagerBosonWriter(TString name)
  : ZeeDHistManager(name),
    fTreeOut(NULL),
    fBoson4Vec(-1,-1,-1,-1),
    fSvcHelper(name),
    fAnaOptions(fSvcHelper.GetAnaSvc())
{

    new TFile("Boson4Vec.root", "RECREATE");
    fTreeOut = new TTree("TB4C", "ZeeD Boson4Vec TTree");

    fTreeOut->Branch("boson4Vec"     , &fBoson4Vec);
    fTreeOut->Branch("runNumber"     , &fRunNumber);
    fTreeOut->Branch("origRunNumber" , &fOrigRunNumber);
    fTreeOut->Branch("eventNumber"   , &fEventNumber);
    fTreeOut->Branch("LB"            , &fLB);
    fTreeOut->Branch("eventWeight"   , &fEventWeight);

}

//-----------------------------------------------------
void ZeeDHistManagerBosonWriter::BookHistos()
{
    DEBUG_MES_START;

    DEBUG_MES_END;
}

//-----------------------------------------------------
void ZeeDHistManagerBosonWriter::Fill()
{
    DEBUG_MES_START;
    DEBUG_MES(this->GetHMName());

    // Get event
    const ZeeDEvent* event = GetEvent();
    CHECK_NULL_PTR(event);

    const ZeeDBosonZ* boson = event->GetCurrentBoson();

    if( boson ) {
        fBoson4Vec = boson->GetFourVector();
    } else {
        fBoson4Vec = TLorentzVector(-1,-1,-1,-1);
    }


    DEBUG_VAR(event->RunNumber().IsSet());
    fRunNumber     = event->RunNumber().Get();
    DEBUG_VAR(event->OrigRunNumber().IsSet());
    fOrigRunNumber = event->OrigRunNumber().Get();
    DEBUG_VAR(event->EventNumber().IsSet());
    fEventNumber   = event->EventNumber().Get();
    DEBUG_VAR(event->LB().IsSet());
    fLB            = event->LB().Get();

    fEventWeight = event->GetWeight();

    fTreeOut->Fill();

    DEBUG_MES_END;
}


void ZeeDHistManagerBosonWriter::Finalize()
{
    Info("ZeeDHistManagerBosonWriter::Finalize", " Close TTree ");

    fTreeOut->Print();
    TFile* tfile = fTreeOut->GetCurrentFile();
    tfile->Write();
    tfile->Close();

}
