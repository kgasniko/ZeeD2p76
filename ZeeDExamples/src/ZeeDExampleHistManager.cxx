#include "ZeeDExamples/ZeeDExampleHistManager.h"

// Root includes
#include <TLorentzVector.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDCalculator/ZeeDElectron.h"
#include "ZeeDCalculator/ZeeDZBoson.h"

#include "egammaEvent/ElectronContainer.h"

#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDExampleHistManager::ZeeDExampleHistManager() : ZeeDHistManager()
{
    // Constructor
    this->BookHistos();
}

//------------------------------------------------------
ZeeDExampleHistManager::ZeeDExampleHistManager(
    TString name) : ZeeDHistManager(name)
{
    // Named constructor
    this->BookHistos();
}

//------------------------------------------------------
ZeeDExampleHistManager::~ZeeDExampleHistManager()
{
    // Destructor
}

//------------------------------------------------------
void ZeeDExampleHistManager::BookHistos()
{
    // Books histogram
    AddTH1D("ElecPt",       50,    0.0, 100.0, "P_{T}^{elec} / GeV", "");
    AddTH1D("ZBosMass",     60,   70.0, 110.0, "M_{Z->ee} / GeV",    "");

    AddTH2D("PtCorr", 60, 70.0, 110.0, 60, 70.0, 110.0,
            "P_{T}^{elec} (ZeeD)", "P_{T}^{elec} (Athena)");
}

//------------------------------------------------------
void ZeeDExampleHistManager::Fill()
{
    // Fills histos

    // ----- Define pointers to histograms -------------

    // ----- Calculate event quantities ----------------
    // Get event
    ZeeDEvent* event = GetEvent();
    CHECK_NULL_PTR(event);

    // Event weight
    Double_t Weight = 1.0;

    // Get array of electrons with LOOSE selection
    TObjArray* electrons = event->GetElectrons(ZeeDEvent::LOOSE);

    // ----- Loop over electrons ------------------------
    for ( Int_t index = 0; index < electrons->GetEntriesFast(); ++index ) {

        ZeeDElectron* electron =  static_cast< ZeeDElectron* > (electrons->At(index));

        // Get electron 4-vector. Electron energy is stored in GeV
        TLorentzVector fourVector = electron->GetFourVector();

        // Fill histogram with Pt of electron from ZeeDEvent collection
        FillTH1(fourVector.Pt(),  Weight, ElecPtIdx);

        // Get electron iterator pointing to the original Athena
        // electron container

        ZeeDElectron::Origin* origin = electron->GetOrigin();

        if (origin != NULL) {
            ElectronContainer::const_iterator elecItr = origin->GetItr();

            // Get electron Pt in GeV from the electron container
            Double_t elecPt_container = (*elecItr)->pt() * 0.001;

            // Fill 2D electron Pt correlation plot
            FillTH2(fourVector.Pt(), elecPt_container, Weight, PtCorrIdx);
        }
    }

    // Get array of Z bosons with TIGHT selection
    TObjArray* bosons = event->GetZBosons(ZeeDEvent::LOOSE);

    // ----- Loop over bosons ---------------------------
    for ( Int_t index = 0; index < bosons->GetEntriesFast(); ++index ) {

        ZeeDZBoson* boson = static_cast< ZeeDZBoson* >( bosons->At(index) );

        // Get Z boson 4-vector. Energy is stored in GeV
        TLorentzVector fourVector = boson->GetFourVector();

        // Fill histogram with ee invariant mass
        FillTH1(fourVector.M(),   Weight, ZBosMassIdx);
    }
}
