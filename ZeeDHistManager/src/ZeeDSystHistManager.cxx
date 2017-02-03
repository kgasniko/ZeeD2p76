#include "ZeeDHistManager/ZeeDSystHistManager.h"

// Root includes
#include <TLorentzVector.h>
#include <TMath.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDGenParticle.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include "TLorentzVector.h"

#include "ZeeDMisc/ZeeDMisc.h"

using std::cout;
using std::endl;

//------------------------------------------------------
ZeeDSystHistManager::ZeeDSystHistManager(
    TString name) : ZeeDHistManager(name)
{
    // Named constructor

}

//------------------------------------------------------

ZeeDSystHistManager::~ZeeDSystHistManager()
{
    // Destructor
}

//------------------------------------------------------
void ZeeDSystHistManager::BookHistos()
{
    // Books histogram

    const TString ZEtaPt    = ZeeDMisc::FindFile("Binning/BosYPt.txt");
    const TString ElecEtaPt = ZeeDMisc::FindFile("Binning/ElecEtaPt.txt");

    AddTH2D("ZEtaPtRec", ZEtaPt, "P_{T} Res", "#eta");
    AddTH2D("ZEtaPtGen", ZEtaPt, "P_{T} Res", "#eta");

    AddTH2D("ElecEtaPtRec", ElecEtaPt, "P_{T} Res", "#eta");
    AddTH2D("ElecEtaPtGen", ElecEtaPt, "P_{T} Res", "#eta");

    AddTH1D("ElecRecPt",        50, 0.0, 120.0, "P_{T} ", "");
    AddTH1D("ElecRecEta",       50, -2.5, 2.5,  "#eta", "");

}

//------------------------------------------------------
void ZeeDSystHistManager::Fill()
{
    // Fills histos
    this -> FillZBosGenHistos();
    this -> FillZBosRecHistos();
}

//------------------------------------------------------
void ZeeDSystHistManager::FillZBosGenHistos()
{

    // ----- Define pointers to histograms -------------


    // ----- Calculate event quantities ----------------
    // Get event
    const ZeeDEvent* event = GetEvent();
    CHECK_NULL_PTR(event);

    // Event weight
    Double_t Weight = event->GetWeight();

    // Get Z boson and array of all electrons
    const ZeeDGenParticle*  PgenZBosons = event->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);
    const ZeeDBosonZ* boson = event->GetCurrentBoson();

    // Fill Gen Z Events
    // -- Loop over generated Z bosons ------------
        TLorentzVector ZBoson4Vec = PgenZBosons->GetMCFourVector();

        if ((PgenZBosons->GetParticleStatus() == 3) &&  (ZBoson4Vec.M() < 110)  &&  (ZBoson4Vec.M() > 70)  ) {
            FillTH2(ZBoson4Vec.Rapidity(), ZBoson4Vec.Pt(), Weight, "ZEtaPtGen" );

            if (boson != NULL) {

                TLorentzVector ZBoson4Vec = boson->GetFourVector();

            }

        }


}

//------------------------------------------------------
void ZeeDSystHistManager::FillZBosRecHistos()
{
    // Fills Z boson histos


    // ----- Calculate event quantities ----------------
    // Get event
    const ZeeDEvent* event = GetEvent();
    CHECK_NULL_PTR(event);

    // Event weight
    Double_t Weight = event->GetWeight();

    // Get array of Z bosons
    const ZeeDBosonZ* boson = event->GetCurrentBoson();

    if (boson == NULL) {
        return;
    }

    TLorentzVector ZBoson4Vec = boson->GetFourVector();
    FillTH2(ZBoson4Vec.Rapidity(), ZBoson4Vec.Pt(), Weight, "ZEtaPtRec" );


    // ----- Loop over the two Z Rec electrons
    for ( Int_t i = 0; i < 2; ++i ) {
        const ZeeDElectron* electron =(ZeeDElectron*) ((i == 0) ? boson->GetFirstLep() : boson->GetSecondLep());

        TLorentzVector fourVector = electron->GetFourVector();

        if(electron->getCharge() < 0 ) {
            FillTH2(fourVector.Rapidity(), fourVector.Pt(), Weight, "ElecEtaPtRec" );

            FillTH1(fourVector.Pt(),  Weight , "ElecRecPt");
            FillTH1(fourVector.Eta(), Weight , "ElecRecEta");
        }

    }


}
