#include "ZeeDAnalysisCuts/ZeeDFinderW.h"

#include "ZeeDEvent/ZeeDEvent.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"


// Root libraries
#include "TSystem.h"

// ZeeD includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorW.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisJetCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisWBosonCuts.h"


//------------------------------------------------------
ZeeDFinderW::ZeeDFinderW(TString Name) :  ZeeDFinder(Name)
{
}

//------------------------------------------------------
TObjArray ZeeDFinderW::SelectAllGoodBosons( ZeeDEvent* event, const TObjArray* bosons)
{
     // Find all the bosons satysfying the selection
  TObjArray bosonArray;
  ZeeDCutBit Mask;
   
  // safe current boson - loop will change it for weight calculation
  const ZeeDBosonW* currentBosonSafe = event->GetCurrentBosonW();
    
  // bit mask for the case all the cuts passed, like: (ncuts == 3) ==> all_bits_1 = 0...0111
  //unsigned int all_bits_1 = all_bits_1_lookup(CutWeights.size());

  for (Int_t i = 0; i < bosons->GetEntriesFast(); i++) {
    ZeeDBosonW* boson = static_cast<ZeeDBosonW*>(bosons->At(i));
    if (boson != NULL) {
   		 event->SetCurrentBosonW(boson);
         this->evaluate(event, &Mask);  // Get bit mask
         //if (all_bits_1 == Mask.GetMask()) {
            bosonArray.Add((TObject*)boson);
         //   }
         }

    }
 
  TObjArray buf;
  // restore current boson
  event->SetCurrentBosonW(currentBosonSafe);
  
  if (bosonArray.GetEntriesFast()!=1)
	  return buf;
  else
 	  return bosonArray;
     

}

//------------------------------------------------------
void ZeeDFinderW::BookCuts()
{ 
     
     std::string name = std::string("ZeeDAnalysisSvc");
     
     ServiceHandle<IZeeDAnalysisSvc> ZeeDAnaOptions(name, "ZeeDFinderW");
     StatusCode sc = ZeeDAnaOptions.retrieve();
     
     if ( sc.isFailure() ) {
         throw std::runtime_error("ZeeDFinderW: cannot retrieve " + name + " service");
         }

    const Double_t elecEtCut    = 20.0;  // GeV
    //const Double_t etMissCut    = 25.0;  // GeV
    const Double_t etaCrackMin  = 1.37;
    const Double_t etaCrackMax  = 1.52;
    const Double_t elecEtaMax   = 2.47;
    //const Double_t vertexZCut   = 300.0; // mm
    //const Int_t    minNumTrkCut = 3;    //
    //const Double_t massTransvCut   = 40;
//   	this->AddCut(new ZeeDCutTrigLepW("LepTrig", ZeeDEnum::Trigger::EF_e15_loose1)); 
    // Cut away transition EMEC-EMB
    this->AddCut(new ZeeDCutEtaCrackElecW("EtaCrackElecW", etaCrackMin, etaCrackMax));

    // Cut on maximal eta of the electron
    this->AddCut(new ZeeDCutElecClustEtaMaxW("ElecClustEtaMaxW", elecEtaMax));
    this->AddCut(new ZeeDCutElecClustEtMinW("ElecClustEtMinW", elecEtCut));
//	CutWeights.push_back(new CutWeight("LepTrig", 100. , getBitMask("LepTrig")));
	CutWeights.push_back(new CutWeight("EtaCrackElecW", 100., getBitMask("EtaCrackElecW")));
	CutWeights.push_back(new CutWeight("ElecClustEtMinW", 100., getBitMask("ElecClustEtMinW")));
	CutWeights.push_back(new CutWeight("ElecClustEtaMaxW", 100., getBitMask("ElecClustEtaMaxW")));



    // Declare cuts for preselecting best W bosons


    // Cut on transverse energy of electron
    //this->AddCut(new ZeeDCutElecClustEtMinW("ElecClustEtMinW", elecEtCut));
    
    // Cut on electron trigger
    //this->AddCut(new ZeeDCutTriggersW("TriggersW"));
 
    // Cut on missing et in event
    //this->AddCut(new ZeeDCutEtMissMinW("EtMissMinW", etMissCut));

    // Cut on transcverse mass of W
    //this->AddCut(new ZeeDCutMassTransvW("MassTransvW", massTransvCut));
    
    // You need ALL of this cuts to CutWeights
    // In another case you will get no bosons =)
    //CutWeights.push_back(new CutWeight("ElecClustEtMinW", 1, getBitMask("ElecClustEtMinW")));
    //CutWeights.push_back(new CutWeight("TriggersW", 1, getBitMask("TriggersW")));
    //CutWeights.push_back(new CutWeight("EtMissMinW", 1, getBitMask("EtMissMinW")));
    //CutWeights.push_back(new CutWeight("ElecClustEtaMaxW", 1, getBitMask("ElecClustEtaMaxW")));
    //CutWeights.push_back(new CutWeight("MassTransvW", 1, getBitMask("MassTransvW")));
         
}
