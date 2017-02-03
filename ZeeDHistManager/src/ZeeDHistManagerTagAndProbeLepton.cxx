#include "ZeeDHistManager/ZeeDHistManagerTagAndProbeLepton.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>
#include <TH1.h>

// Analysis includes
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDTools/ZeeDBags.h"
#include "ZeeDTools/ZeeDCutBit.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDLepton.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisZBosonCuts.h"

using std::cout;
using std::endl;
using std::string;
//---------------------------------------------------



void ZeeDHistManagerTagAndProbeLepton::BookHistos()
{   
    const TString LepEataPtTrigmatchBins = ZeeDMisc::FindFile("Binning/ElecEtaPt.txt");
    AddTH2D("LepEtaVSPt_CommonCuts",                LepEataPtTrigmatchBins,   "P_{t} [MeV]",   "#eta");
    AddTH2D("LepEtaVSPt_IDcut",                     LepEataPtTrigmatchBins,   "P_{t} [MeV]",   "#eta");
    AddTH2D("LepEtaVSPt_ID_TrigMatch_cuts",     LepEataPtTrigmatchBins,   "P_{t} [MeV]",   "#eta");

    AddTH1D("LepPt_CommonCuts",              150,   0.0,   150000.0,   "P_{T}^{lep} [MeV]", "");
    AddTH1D("LepPt_IDcut",                   150,   0.0,   150000.0,   "P_{T}^{lep} [MeV]", "");
    AddTH1D("LepPt_ID_TrigMatch_cuts",   150,   0.0,   150000.0,   "P_{T}^{lep} [MeV]", "");

    AddTH1D("LepEta_CommonCuts",             100,   -5.0,  5.0,   "#eta^{lep}",  "");
    AddTH1D("LepEta_IDcut",                  100,   -5.0,  5.0,   "#eta^{lep}",  "");
    AddTH1D("LepEta_ID_TrigMatch_cuts",  100,   -5.0,  5.0,   "#eta^{lep}",  "");    

}


void ZeeDHistManagerTagAndProbeLepton::FillLeptonHists(const TObjArray* leptons, Double_t Weight)
{
	this->SetProperties(kFALSE, kFALSE);
	if (leptons == NULL) {
        return;
    }

    string sSVCname("ZeeDAnalysisSvc");
    ServiceHandle<IZeeDAnalysisSvc> ZeeDAnaOptions(sSVCname, "ZeeDCalcZ");
    StatusCode sc = ZeeDAnaOptions.retrieve();
    if ( sc.isFailure() ) {
        throw std::runtime_error("ZeeDHistmanagerTagAndProbeLepton::FillLeptonHists: can not retrieve " + sSVCname + " service");
    }

    for ( Int_t i = 0; i < leptons->GetEntriesFast(); ++i ) {
        const ZeeDLepton * lepton = static_cast< ZeeDLepton* > (leptons->At(i));       
        
        if( i == 0 && IsTag(lepton) ) { // check first lepton as tag
            lepton = static_cast< ZeeDLepton* > (leptons->At(i + 1)); // try second lepton as probe
            FillProbe(lepton, Weight);  // fill histograms for probe leptons
        }
        else if (i == 1 && IsTag(lepton) ) { // check second lepton as tag
            lepton = static_cast< ZeeDLepton* > (leptons->At(i - 1));  // try first lepton as probe
            FillProbe(lepton, Weight);  // fill histograms for probe leptons
        }	
    }  

}

Bool_t ZeeDHistManagerTagAndProbeLepton::IsTag(const ZeeDLepton * lepton)
{
    // Check if input lepton is a "tag lepton"
    const Bool_t bTagLepton = IdLepCut(lepton) && TrigMatchLepCut(lepton);

    return bTagLepton;
}

void ZeeDHistManagerTagAndProbeLepton::FillProbe(const ZeeDLepton * lepton, Double_t Weight)
{
    const Double_t dThouthand = 1000.0; // for filling histograms in MeV (instead of GeV)
    const TLorentzVector & fourVector = lepton->GetFourVector();

    // Filling gistograms for probe leprons after common sleption
    FillTH2(fourVector.Pt() * dThouthand,  fourVector.Eta(), Weight, "LepEtaVSPt_CommonCuts");
    FillTH1(fourVector.Pt() * dThouthand,  Weight, "LepPt_CommonCuts");
    FillTH1(fourVector.Eta(),  Weight, "LepEta_CommonCuts");    
    

    // Filling gistograms for probe leprons after common sleption + Identification
    if( IdLepCut(lepton) ) {
        FillTH2(fourVector.Pt() * dThouthand,  fourVector.Eta(), Weight, "LepEtaVSPt_IDcut");
        FillTH1(fourVector.Pt() * dThouthand,  Weight, "LepPt_IDcut");
        FillTH1(fourVector.Eta(),  Weight, "LepEta_IDcut");        
        
    }
    // Filling gistograms for probe leprons after common sleption + Identification + Isolation + Trigger matching
    if( IdLepCut(lepton) && TrigMatchLepCut(lepton) ) {
        FillTH2(fourVector.Pt() * dThouthand,  fourVector.Eta(), Weight, "LepEtaVSPt_ID_TrigMatch_cuts");
        FillTH1(fourVector.Pt() * dThouthand,  Weight, "LepPt_ID_TrigMatch_cuts");
        FillTH1(fourVector.Eta(),  Weight, "LepEta_ID_TrigMatch_cuts");        
        
    } 
 
}


Bool_t ZeeDHistManagerTagAndProbeLepton::IdLepCut(const ZeeDLepton * lepton)
{
    // Identifacation
    const Bool_t bPassId = lepton->IsEM().IsSet() && lepton->IsEM().Get().isMediumPP;

    return bPassId;
}
    
Bool_t ZeeDHistManagerTagAndProbeLepton::TrigMatchLepCut(const ZeeDLepton * lepton)
{
    // Trigger matching
	const Bool_t bTrigIsSet = lepton->Trigger().IsSet();
 	const Bool_t bPassTrigMatch = bTrigIsSet && (lepton->IsMatchedToTriggerChain(ZeeDEnum::Trigger::EF_mu10));	
   
    return bPassTrigMatch;
}




