#include "ZeeDExamples/ZeeDEffExampleHistManager.h"

// Root includes
#include <TLorentzVector.h>
#include <TMath.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDCalculator/ZeeDElectron.h"
#include "ZeeDCalculator/ZeeDZBoson.h"

#include "egammaEvent/ElectronContainer.h"

#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDEffExampleHistManager::ZeeDEffExampleHistManager() : ZeeDEffHistManager()
{
    // Constructor
    this->BookHistos();
}

//------------------------------------------------------
ZeeDEffExampleHistManager::ZeeDEffExampleHistManager(
    TString name) : ZeeDEffHistManager(name)
{
    // Named constructor
    this->BookHistos();
}

//------------------------------------------------------
ZeeDEffExampleHistManager::~ZeeDEffExampleHistManager()
{
    // Destructor
}

//------------------------------------------------------
void ZeeDEffExampleHistManager::BookHistos()
{
    // Books histogram
    const Int_t NumOfHistos = 3;
    Histogram* Histos = new Histogram[NumOfHistos];
    Histos[0]  = DefineHisto("ElecPt",  50, GetLinBinGrid(50,    0.0, 100.0), "P_{T}^{elec} / GeV");
    Histos[1]  = DefineHisto("ElecEta", 40, GetLinBinGrid(40,   -4.0,   4.0), "#eta^{elec}");
    Histos[2]  = DefineHisto("ElecPhi", 80, GetLinBinGrid(80, -180.0, 180.0), "#varphi^{elec} / deg");

    const Int_t NumOfEffNames = 2;
    TString* EffNames   = new TString[NumOfEffNames];
    EffNames[0] = "EnergyEff";
    EffNames[1] = "PhiEff";

    for( Int_t i = 0; i < NumOfEffNames; ++i) {
        DeclareHistos(EffNames[i], NumOfHistos, Histos);
    }

    // Clean the memory
    if ( Histos != NULL ) {
        delete [] Histos;
        Histos   = NULL;
    }

    if (Histos != NULL ) {
        delete [] EffNames;
        EffNames = NULL;
    }
}

//------------------------------------------------------
void ZeeDEffExampleHistManager::Fill()
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

        const Int_t NumOfVaribles = 3;
        Double_t* VariablesToFill  = new Double_t[NumOfVaribles];
        VariablesToFill[0]  = fourVector.Pt();
        VariablesToFill[1]  = fourVector.Eta();
        VariablesToFill[2]  = fourVector.Phi() * ZeeDMisc::Rad2Deg;

        // Fill "Top" and "Bot" histos
        if ( fourVector.E() > 10 ) { // energy requirement
            FillHistos1D(NumOfVaribles, VariablesToFill, Weight, EnergyEffElecPtTopIdx);
        }

        FillHistos1D(NumOfVaribles, VariablesToFill, Weight, EnergyEffElecPtBotIdx);

        if ( TMath::Abs( fourVector.Phi() * ZeeDMisc::Rad2Deg ) > 10 ) {  // phi requirement
            FillHistos1D(NumOfVaribles, VariablesToFill, Weight, PhiEffElecPtTopIdx);
        }

        FillHistos1D(NumOfVaribles, VariablesToFill, Weight, PhiEffElecPtBotIdx);

        // Clean memory
        if ( VariablesToFill != NULL ) {
            delete [] VariablesToFill;
            VariablesToFill = NULL;
        }
    }
}
