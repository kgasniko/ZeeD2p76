#include "ZeeDAnalysisCuts/ZeeDFinderJet.h"

#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

ZeeDFinderJet::ZeeDFinderJet(TString Name, std::string coll): ZeeDFinder(Name+ TString(coll.c_str())), fJetCollection(coll)
{}

void ZeeDFinderJet::BookCuts()
{
    std::string name = std::string("ZeeDAnalysisSvc");

    ServiceHandle<IZeeDAnalysisSvc> ZeeDAnaOptions(name, "ZeeDFinderJet");
    StatusCode sc = ZeeDAnaOptions.retrieve();


    if( sc.isFailure())
    {
        throw std::runtime_error("ZeeDFinderJet: Cannot retireve "+name+" service");
    }

    Double_t jetPtCut   = ZeeDAnaOptions->Jet_PtCut();
    Double_t jetEtaCut  = ZeeDAnaOptions->Jet_EtaCut();

    this->AddCut(new ZeeDCutPtMinJet("PtMinJet", jetPtCut));
    this->AddCut(new ZeeDCutEtaMaxJet("EtaMaxJet", jetEtaCut));

    CutWeights.push_back(new CutWeight("PtMinJet", 1,getBitMask("PtMinJet")));
    CutWeights.push_back(new CutWeight("EtaMaxJet",1,getBitMask("EtaMaxJet")));
}
