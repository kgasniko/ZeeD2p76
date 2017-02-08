#include "ZeeDAnalysisCuts/ZeeDFinderWmu.h"

#include "ZeeDEvent/ZeeDEvent.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"


// Root libraries
#include "TSystem.h"

// ZeeD includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorWmu.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisJetCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisWBosonCuts.h"


//------------------------------------------------------
ZeeDFinderWmu::ZeeDFinderWmu(TString Name) :  ZeeDFinder(Name)
{
}

//------------------------------------------------------
TObjArray ZeeDFinderWmu::SelectAllGoodBosons( ZeeDEvent* event, const TObjArray* bosons)
{
    // Find all the bosons satysfying the selection
    TObjArray bosonArray;
    ZeeDCutBit Mask;

    // safe current boson - loop will change it for weight calculation
    const ZeeDBosonW* currentBosonSafe = event->GetCurrentBosonW();

    // bit mask for the case all the cuts passed, like: (ncuts == 3) ==> all_bits_1 = 0...0111
    unsigned int all_bits_1 = all_bits_1_lookup(CutWeights.size());
    for (Int_t i = 0; i < bosons->GetEntriesFast(); i++) {
        ZeeDBosonW* boson = static_cast<ZeeDBosonW*>(bosons->At(i));
        if (boson != NULL) {
            event->SetCurrentBosonW(boson);
            this->evaluate(event, &Mask);  // Get bit mask
            if (all_bits_1 == Mask.GetMask()) {
            bosonArray.Add((TObject*)boson);
            }
        }

    }
    TObjArray buf;
    // restore current boson
    event->SetCurrentBosonW(currentBosonSafe);
    //std::cout << bosonArray.GetEntriesFast() << std::endl; 
    if (bosonArray.GetEntriesFast()>1)
        return buf;
    else
        return bosonArray;

}

//------------------------------------------------------
void ZeeDFinderWmu::BookCuts()
{ 

    std::string name = std::string("ZeeDAnalysisSvc");

    ServiceHandle<IZeeDAnalysisSvc> ZeeDAnaOptions(name, "ZeeDFinderWmu");
    StatusCode sc = ZeeDAnaOptions.retrieve();

    if ( sc.isFailure() ) {
        throw std::runtime_error("ZeeDFinderWmu: cannot retrieve " + name + " service");
    }

    //const Double_t elecEtCut    = 20.0;  // GeV
    //const Double_t etMissCut    = 25.0;  // GeV
    //const Double_t etaCrackMin  = 1.37;
    // const Double_t etaCrackMax  = 1.52;
    //const Double_t elecEtaMax   = 2.47;
    //const Double_t vertexZCut   = 300.0; // mm
    //const Int_t    minNumTrkCut = 3;    //
    //const Double_t massTransvCut   = 40;


    this->AddCut(new ZeeDCutLepTrackEtaW  ("eta", 2.5));
    this->AddCut(new ZeeDCutLepPtMinW("ptMu", 20));
    //this->AddCut(new ZeeDCutCombinedMuon("comb"));
    this->AddCut(new ZeeDCutLepIdentW("Medium", &ZeeDLeptonBags::IsEM::isMedium));
    this->AddCut(new ZeeDCutTrigLepW("MuonTrig", ZeeDEnum::Trigger::EF_mu10));

    CutWeights.push_back(new CutWeight("eta", 100. , getBitMask("eta")));
    //CutWeights.push_back(new CutWeight("comb", 100., getBitMask("comb")));
    CutWeights.push_back(new CutWeight("ptMu", 100., getBitMask("ptMu")));
    CutWeights.push_back(new CutWeight("Medium", 100., getBitMask("Medium")));
    CutWeights.push_back(new CutWeight("MuonTrig", 100., getBitMask("MuonTrig")));
    // Declare cuts
    /*
    // Cut on existence and quality cut of vertex
    this->AddCut(new ZeeDCutPriVtxNtrack("NTracksAtPrimVtx", minNumTrkCut));

    // Cut on primary vertex Z position
    this->AddCut(new ZeeDCutPriVtxZ("PriVtxZ", vertexZCut));

    // Cut on missing et in event 
    this->AddCut(new ZeeDCutEventMinMissEt("EtMiss", etMissCut));

    // You need to add all of the cuts to CutWeights
    CutWeights.push_back(new CutWeight("NTracksAtPrimVtx", 1, getBitMask("NTracksAtPrimVtx")));
    CutWeights.push_back(new CutWeight("PriVtxZ", 1, getBitMask("PriVtxZ")));
    CutWeights.push_back(new CutWeight("EtMiss", 1, getBitMask("EtMiss")));
    */
}
