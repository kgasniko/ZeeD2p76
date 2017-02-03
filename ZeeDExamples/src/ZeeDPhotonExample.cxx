#include "ZeeDExamples/ZeeDPhotonExample.h"

// std includes
#include <iostream>

// Root includes
#include "TFile.h"
#include "TH1.h"

#include "ZeeDInlineZ/ZeeDInlineZ.h"

// Athena includes
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/IToolSvc.h"


using std::cout;
using std::endl;

//------------------------------------------------------
ZeeDPhotonExample::ZeeDPhotonExample( const std::string& name,
                                      ISvcLocator* pSvcLocator) : Algorithm(name, pSvcLocator)
{
    // Constructor
}

//------------------------------------------------------
ZeeDPhotonExample::~ZeeDPhotonExample()
{
    // Destructor
}

//------------------------------------------------------
StatusCode ZeeDPhotonExample::initialize()
{
    return StatusCode::SUCCESS;
}

//------------------------------------------------------
StatusCode ZeeDPhotonExample::finalize()
{
    return StatusCode::SUCCESS;
}

//------------------------------------------------------
StatusCode ZeeDPhotonExample::execute()
{
    // ----------------------------------------
    // This is a example of using ZeeDInlineZ package
    // -----------------------------------------

    // Create events
    ZeeDInlineZ* event = new ZeeDInlineZ();

    TFile* hfile = new TFile("InlineGen.root", "recreate", "histograms");

    // Create some histograms for our example
    TH1D* MassSumElecNoRad = new TH1D("MassSumElecNoRad", "test", 100, 30, 140);
    TH1D* MassSumElecRad = new TH1D("MassSumElecRad", "test", 100, 30, 140);
    TH1D* MassSumElecSmear = new TH1D("MassSumElecSmear", "test", 100, 30, 140);
    TH1D* MassZNoRad = new TH1D("MassZNoRad", "test", 100, 30, 140);
    TH1D* MassZRad = new TH1D("MassZRad", "test", 100, 30, 140);

    TH1D* PtSumElecNoRad = new TH1D("PtSumElecNoRad", "test", 100, 0, 140);
    TH1D* PtSumElecRad = new TH1D("PtSumElecRad", "test", 100, 0, 140);
    TH1D* PtSumElecSmear = new TH1D("PtSumElecSmear", "test", 100, 0, 140);


    for (int i = 0; i < 100000; ++i) {

        // Full calculation of events
        event -> CalcEvent();

        // Fill histograms of mass distribution of sum electron 4-vector without radiations
        MassSumElecNoRad -> Fill ((((event -> GetGenElecNoRad(1)) + (event -> GetGenElecNoRad(0))).M()));

        // Fill histograms of mass distribution of sum electron 4-vector with radiations
        MassSumElecRad   -> Fill ((((event -> GetGenElecRad(1)) + (event -> GetGenElecRad(0))).M()));

        // Fill histograms of mass distribution of sum electron 4-vector with radiations and smearing
        MassSumElecSmear -> Fill ((((event -> GetGenElecSmear(1)) + (event -> GetGenElecSmear(0))).M()));

        // Fill histograms of mass distribution of Z boson without and with radiation
        MassZNoRad -> Fill ( ((event -> GetGenZ())).M());
        MassZRad   -> Fill ( ((event -> GetGenZRad())).M());

        // Fill some Pt distribution
        PtSumElecNoRad -> Fill ((((event -> GetGenElecNoRad(1)) + (event -> GetGenElecNoRad(0))).Pt()));
        PtSumElecRad   -> Fill ((((event -> GetGenElecRad(1)) + (event -> GetGenElecRad(0))).Pt()));
        PtSumElecSmear -> Fill ((((event -> GetGenElecSmear(1)) + (event -> GetGenElecSmear(0))).Pt()));

    }

    // As you see, using some simple methods of ZeeDInlineZ class tou can receave all needed parametr
    // GetGenElecNoRad(int i) - return 4-vector of electron number i (i = 0,1) without radiation.
    // GetGenElecRad(int i) - return 4-vector of electron number i (i = 0,1) with radiation.
    // GetGenElecSmear(0) - return 4-vector of electron number i (i = 0,1) with radiation and smearing.
    // GetGenZ() - returm Z boson 4-vector without radiation.
    // GetGenZRad() - returm Z boson 4-vector with radiation.
    // GetGenElec(0) - return 4-vector of electron number i (i = 0,1) with/without radiation (cheked flag).
    // If GetRadiation = False, GetGenElecRad(int i) and GetGenZRad() return 0 4-v.

    hfile->Write();
    hfile->Close();

    if (event             != NULL ) {
        delete event;
    }

    if (hfile             != NULL ) {
        delete hfile;
    }

    if (MassSumElecNoRad  != NULL ) {
        delete MassSumElecNoRad;
    }

    if (MassSumElecRad    != NULL ) {
        delete MassSumElecRad;
    }

    if (MassSumElecSmear  != NULL ) {
        delete MassSumElecSmear;
    }

    if (MassZNoRad        != NULL ) {
        delete MassZNoRad;
    }

    if (MassZRad          != NULL ) {
        delete MassZRad;
    }

    if (PtSumElecNoRad    != NULL ) {
        delete PtSumElecNoRad;
    }

    if (PtSumElecRad      != NULL ) {
        delete PtSumElecRad;
    }

    if (PtSumElecSmear    != NULL ) {
        delete PtSumElecSmear;
    }



    return StatusCode::SUCCESS;
}
