#include "ZeeDAnalysisCuts/ZeeDFinderZmumu.h"

#include "ZeeDEvent/ZeeDEvent.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"


// Root libraries
#include "TSystem.h"

// ZeeD includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorZmumu.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisJetCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"


//------------------------------------------------------
ZeeDFinderZmumu::ZeeDFinderZmumu(TString Name) :  ZeeDFinder(Name)
{
}

//------------------------------------------------------
TObjArray ZeeDFinderZmumu::SelectAllGoodBosons( ZeeDEvent* event, const TObjArray* bosons)
{
     // Find all the bosons satysfying the selection
  TObjArray bosonArray;
  ZeeDCutBit Mask;
   
  // safe current boson - loop will change it for weight calculation
  const ZeeDBosonZ* currentBosonSafe = event->GetCurrentBoson();
    
  // bit mask for the case all the cuts passed, like: (ncuts == 3) ==> all_bits_1 = 0...0111
  //unsigned int all_bits_1 = all_bits_1_lookup(CutWeights.size());

  for (Int_t i = 0; i < bosons->GetEntriesFast(); i++) {
    ZeeDBosonZ* boson = static_cast<ZeeDBosonZ*>(bosons->At(i));
    if (boson != NULL) {
            bosonArray.Add((TObject*)boson);
         }

    }
 
  // restore current boson
  event->SetCurrentBoson(currentBosonSafe);
  
  return bosonArray;
     
}

//------------------------------------------------------
void ZeeDFinderZmumu::BookCuts()
{ 
     
     std::string name = std::string("ZeeDAnalysisSvc");
     
     ServiceHandle<IZeeDAnalysisSvc> ZeeDAnaOptions(name, "ZeeDFinderZmumu");
     StatusCode sc = ZeeDAnaOptions.retrieve();
     
     if ( sc.isFailure() ) {
         throw std::runtime_error("ZeeDFinderZmumu: cannot retrieve " + name + " service");
         }

    //const Double_t elecEtCut    = 20.0;  // GeV
    //const Double_t etMissCut    = 25.0;  // GeV
    //const Double_t etaCrackMin  = 1.37;
    //const Double_t etaCrackMax  = 1.52;
    //const Double_t elecEtaMax   = 2.47;
    //const Double_t vertexZCut   = 300.0; // mm
    //const Int_t    minNumTrkCut = 3;    //
    //const Double_t massTransvCut   = 40;

    // Declare cuts for preselecting best Zmumu bosons


    // Cut on transverse energy of electron
    //this->AddCut(new ZeeDCutElecClustEtMinZmumu("ElecClustEtMinZmumu", elecEtCut));
    
    // Cut on electron trigger
    //this->AddCut(new ZeeDCutTriggersZmumu("TriggersZmumu"));
 
    // Cut on missing et in event
    //this->AddCut(new ZeeDCutEtMissMinZmumu("EtMissMinZmumu", etMissCut));

    // Cut on transcverse mass of Zmumu
    //this->AddCut(new ZeeDCutMassTransvZmumu("MassTransvZmumu", massTransvCut));
    
    // You need ALL of this cuts to CutZmumueights
    // In another case you will get no bosons =)
    //CutZmumueights.push_back(new CutZmumueight("ElecClustEtMinZmumu", 1, getBitMask("ElecClustEtMinZmumu")));
    //CutZmumueights.push_back(new CutZmumueight("TriggersZmumu", 1, getBitMask("TriggersZmumu")));
    //CutZmumueights.push_back(new CutZmumueight("EtMissMinZmumu", 1, getBitMask("EtMissMinZmumu")));
    //CutZmumueights.push_back(new CutZmumueight("ElecClustEtaMaxZmumu", 1, getBitMask("ElecClustEtaMaxZmumu")));
    //CutZmumueights.push_back(new CutZmumueight("MassTransvZmumu", 1, getBitMask("MassTransvZmumu")));
         
}
