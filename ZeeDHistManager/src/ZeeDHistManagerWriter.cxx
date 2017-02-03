#include "ZeeDHistManager/ZeeDHistManagerWriter.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"

#include "ZeeDCuts/ZeeDCutHistManager.h"
#include "ZeeDTools/ZeeDCutBit.h"

#include "ZeeDTools/ZeeDBags.h"

#include "AthenaKernel/IDictLoaderSvc.h"

#include <iostream>

#include <TDirectory.h>

//------------------------------------------------------
ZeeDHistManagerWriter::ZeeDHistManagerWriter(TString name, int maxSize) : ZeeDHistManager(name), fInputType("UNKNOWN"), bFirstEvent(kTRUE)
{
    TFile* f = new TFile(name, "RECREATE");
    treeOut = new TTree("T", "ZeeD TTree");

    f->mkdir("TMeta");
    TDirectory* d = ((TDirectory*)f->Get("TMeta"));
    metaTreeOut = new TTree("MetaTree", "ZeeD Meta TTree");
    metaTreeOut->SetDirectory(d);
    metaTreeOut->Branch("inputType",&fInputType);
    metaTreeOut->Branch("triggerMap",&fTriggerMap);

    if ((*fAnaOptions)->IsMC()) {
        metaTreeOut->Branch("datasetID", &fDatasetID);
    }

    if (maxSize > 0) {
        treeOut->SetMaxTreeSize(maxSize);
    }

    ServiceHandle<IDictLoaderSvc> dictsvc("AthDictLoaderSvc", "ZeeDHistManagerWriter");
    dictsvc->load_type("ZeeDROOTEvent");

    treeOut->Branch("events", &rootEvent, 32000, 0);
}
//------------------------------------------------------
void ZeeDHistManagerWriter::BookHistos()
{
    DEBUG_MES_START;
    // Books histograms


    DEBUG_MES_END;
}

//------------------------------------------------------
ZeeDHistManagerWriter::~ZeeDHistManagerWriter()
{

}

//------------------------------------------------------
void ZeeDHistManagerWriter::Fill()
{

    if (bFirstEvent) {

        fTriggerMap = *((*fAnaOptions)->getTriggerMap());

        fInputType = ZeeDMisc::inputTypeToStr((*fAnaOptions)->InputType());

        if ((*fAnaOptions)->IsMC()) {
            fDatasetID = GetEvent()->MCChannelNumber().IsSet() ? GetEvent()->MCChannelNumber().Get() : 0;
        }

        metaTreeOut->Fill();

        bFirstEvent = kFALSE;

    }

    // Get event
    const RawEvent* event = GetEvent()->GetRawEvent();

    rootEvent.fillIn(event);
    treeOut->Fill();
    rootEvent.clear();

}

//------------------------------------------------------
void ZeeDHistManagerWriter::Finalize()
{

    Info("ZeeDHistManagerWriter::Finalize", " Close TTree ");

    treeOut->Print();
    metaTreeOut->Print();

    TFile* tfile = treeOut->GetCurrentFile();
    tfile->Write();
    tfile->Close();

}
