#include "ZeeDExamples/ZeeDArraysExample.h"

// std includes
#include <iostream>

// Root includes
#include <TROOT.h>
#include <TH1.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TPostScript.h>
#include <TRandom.h>
#include <TBenchmark.h>
#include <TString.h>
#include <TUnixSystem.h>

// Athena includes
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/IToolSvc.h"

// ZeeD analysis includes
#include "ZeeDArrays/ZeeDBinGrid.h"
#include "ZeeDArrays/ZeeDGridCell.h"

using std::cout;
using std::endl;

//------------------------------------------------------
ZeeDArraysExample::ZeeDArraysExample( const std::string& name,
                                      ISvcLocator* pSvcLocator) : Algorithm(name, pSvcLocator)
{
    // Constructor
}

//------------------------------------------------------
ZeeDArraysExample::~ZeeDArraysExample()
{
    // Destructor
}

//------------------------------------------------------
StatusCode ZeeDArraysExample::initialize()
{
    return StatusCode::SUCCESS;
}

//------------------------------------------------------
StatusCode ZeeDArraysExample::finalize()
{
    return StatusCode::SUCCESS;
}

//------------------------------------------------------
StatusCode ZeeDArraysExample::execute()
{

    // ------------------ Example 1 ----------------------

    TBenchmark*  benchMark = new TBenchmark();
    TRandom*     rand      = new TRandom();
    ZeeDBinGrid* binGrid   = new ZeeDBinGrid();

    const Int_t numXBins  = 20;
    const Int_t numYBins  = 20;
    const Int_t numEvents = 100000;

    Double_t xBins[numXBins];
    Double_t yBins[numYBins];

    for (Int_t ix = 0; ix < numXBins; ++ix) {
        xBins[ix] = 10 * ( ix + 0.9 * rand->Rndm() );
    }

    for (Int_t iy = 0; iy < numYBins; ++iy) {
        yBins[iy] = 10 * ( iy + 0.9 * rand->Rndm() );
    }

    // Make benchmarking
    benchMark->Start("Grid creation");

    // Define bin edges
    ZeeDArrayD* xGrid = new ZeeDArrayD(numXBins, xBins);
    ZeeDArrayD* yGrid = new ZeeDArrayD(numYBins, yBins);

    binGrid->AddBinEdges( xGrid );  // Add bin edges to the grid
    binGrid->AddBinEdges( yGrid );  //

    binGrid->CreateGrid(); // Create regular grid

    benchMark->Stop("Grid creation");
    benchMark->Start("Grid filling");

    // Fill grid with random numbers
    for (Int_t i = 0; i < numEvents; ++i) {
        binGrid->Fill( 1 * rand->Rndm(),
                       8.9 * numXBins * rand->Rndm(), 8.9 * numYBins * rand->Rndm());
    }

    benchMark->Stop("Grid filling");

    // Make canvas
    TCanvas* can = new TCanvas("plots", "Plots", 800, 600);
    can -> SetFillColor(10);

    // Define a ps file to write histos
    TPostScript* ps = new TPostScript("ArraysExample.ps", 112);

    // Draw grid into the ps file
    binGrid->Draw("Grid before merging");

    gPad->RedrawAxis();
    can -> Update();

    // Next page
    ps->NewPage();

    benchMark->Start("Grid merging");

    // Merge cells such that N > 600
    binGrid->Merge(600.0);

    benchMark->Stop("Grid merging");

    // Draw merged grid into the ps file
    binGrid->Draw("Grid after merging");

    gPad->RedrawAxis();
    can -> Update();

    // Uncomment this if you want to see the content of the grid
    // binGrid->Print();          // Prints grid
    // binGrid->Print("merged");  // Prints grid of merged cells

    // Create directory Text if it does not exist
    TString mkDirCommand = TString("if [ ! -d Text ]; then mkdir Text; fi");
    gSystem->Exec(mkDirCommand);

    // Write grid into a file
    binGrid->WriteIntoFile("Text/ExampleBinGrid.txt");

    // ------------------ Example 2 ----------------------

    // Read grid from a  text file
    ZeeDBinGrid* gridFromFile = new ZeeDBinGrid();
    gridFromFile->ReadFromFile("Text/ExampleBinGrid.txt");

    // Next page
    ps->NewPage();

    // Draw grid into the ps file
    gridFromFile->Draw("Grid read from ExampleBinGrid.txt");

    gPad->RedrawAxis();
    can -> Update();

    // Next page
    ps->NewPage();

    // Draw grid with cell numbers instead of values
    gridFromFile->Draw("index");

    gPad->RedrawAxis();
    can -> Update();

    // Close ps file
    ps  -> Close();

    // Print benchmark summary
    cout << endl;
    cout << "Benchmark summary:" << endl;

    Double_t a, b;
    benchMark->Summary(a, b);

    cout << endl;

    // Clean up
    delete can;
    delete ps;

    delete binGrid;
    delete gridFromFile;

    delete rand;
    delete benchMark;

    return StatusCode::SUCCESS;
}
