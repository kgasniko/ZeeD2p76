#include "ZeeDHist/ZeeDHistPlotter.h"

// std includes
#include <iostream>

// Root includes
#include <TROOT.h>
#include <TCanvas.h>
#include <TPostScript.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TPaveStats.h>

// ZeeD analysis includes
#include "ZeeDHist/ZeeDHistArray.h"

using std::cout;
using std::endl;

//------------------------------------------------------
ZeeDHistPlotter::ZeeDHistPlotter(TString filename) :
    fLegend(NULL),
    fDrawLegend(kTRUE),
    fDrawLegendOnce(kTRUE),

    fUseStats(kFALSE),

    fNPadsX(3),
    fNPadsY(3),

    fLogScale(kFALSE),

    fNormalize(kFALSE),
    fDrawOptionFirst("e"),
    fDrawOptionOther("hist sames")
{
    // Constructor
    fHistArrays     = new TObjArray();
    fRootFile       = new TFile(filename);

    this->InitRoot();
}

//------------------------------------------------------
ZeeDHistPlotter::ZeeDHistPlotter() :
    fRootFile(NULL),

    fLegend(NULL),

    fDrawLegend(kTRUE),
    fDrawLegendOnce(kTRUE),

    fUseStats(kFALSE),

    fNPadsX(3),
    fNPadsY(3),

    fLogScale(kFALSE),
    fNormalize(kFALSE),

    fDrawOptionFirst("e"),
    fDrawOptionOther("hist sames")
{
    // Constructor
    fHistArrays     = new TObjArray();

    this->InitRoot();
}

//------------------------------------------------------
ZeeDHistPlotter::~ZeeDHistPlotter()
{
    // Destructor
    for (Int_t index = 0; index < fHistArrays->GetEntriesFast(); ++index) {
        TObjArray* array = static_cast<TObjArray*>( fHistArrays->At(index) );
        delete array;
    }

    delete fHistArrays;

    if (fRootFile != NULL) {
        delete fRootFile;
    }
}

//------------------------------------------------------
void ZeeDHistPlotter::ReadArray(TString dirname)
{
    // Reads array of histograms from the directory dirname
    ZeeDHistArray* histArray = new ZeeDHistArray(fRootFile, dirname);
    this->AddArray(histArray);
}

//------------------------------------------------------
void ZeeDHistPlotter::ReadArray(TString dirname, TString filename)
{
    // Reads array of histograms from the directory dirname of the
    // root file filename

    TFile* rootFile = TFile::Open(filename);
    ZeeDHistArray* histArray = new ZeeDHistArray(rootFile, dirname);
    this->AddArray(histArray);
}

//------------------------------------------------------
void ZeeDHistPlotter::AddArray(ZeeDHistArray* histArray)
{
    // Adds hist array
    fHistArrays->AddLast(histArray);

    if (fHistArrays->GetEntriesFast() > 1) {
        histArray->SetLineColor( 2 * (fHistArrays->GetEntriesFast()) );
    }
}

//------------------------------------------------------
void ZeeDHistPlotter::ReadEfficiencyArray(TString dirname)
{
    // Reads array of histograms from the directory dirname
    ZeeDHistArray* histArrayTop = new ZeeDHistArray(fRootFile, dirname, "Top");
    ZeeDHistArray* histArrayBot = new ZeeDHistArray(fRootFile, dirname, "Bot");
    *histArrayTop /= *histArrayBot;
    fHistArrays->AddLast(histArrayTop);
    //    histArrayTop->SetLineColor(2*fHistArrays->GetEntriesFast());

    // Set maximum and minimum
    TObjArray* array = histArrayTop->GetHistArray();

    for (Int_t index = 0; index < array->GetEntriesFast(); ++index) {
        TH1* hist = static_cast<TH1*>(array->At(index));
        hist->SetMaximum(1.2);
        hist->SetMinimum(0.0);
        hist->SetLineColor(1);
        hist->SetMarkerColor(1);
    }
}

//------------------------------------------------------
void ZeeDHistPlotter::MakePlots(TString filename)
{

    
    // Draws histograms into a poscript file filename

    if (fHistArrays->GetEntriesFast() == 0 ) {
        return;
    }

    // Make canvas
    TCanvas* can = new TCanvas("plots", "Plots", 800, 600);
    can -> SetFillColor(10);

    // Define a ps file to write histos
    TPostScript* ps = new TPostScript(filename, 112);

    can -> Divide(fNPadsX, fNPadsY);
    can -> SetFillColor(10);

    // Make canvas
    Int_t canvasNumber = 0;

    // Get number of histograms
    ZeeDHistArray* firstArray = static_cast<ZeeDHistArray*>( fHistArrays->At(0) );
    const Int_t numOfHistos = firstArray->GetEntriesFast();

    // Array to store hist max value
    Float_t* histMaxArray = new Float_t[numOfHistos];

    // Find hist maximums
    for (Int_t index = 0; index < numOfHistos; ++index) {
        histMaxArray[index] = 0.;

        // Loop over hist arrays
        for (Int_t arrayIdx = 0; arrayIdx < fHistArrays->GetEntriesFast(); ++arrayIdx) {

            ZeeDHistArray* histArray =
                static_cast<ZeeDHistArray*> ( fHistArrays->At(arrayIdx) );

            // Get histogram maximum
            TH1* hist = static_cast<TH1*>(histArray->GetHistArray()->At(index));
            Float_t max = hist->GetMaximum();

            if (max > histMaxArray[index] ) {
                histMaxArray[index] = max;
            }
        }
    }

    // Loop over histograms in the first array
    for (Int_t index = 0; index < numOfHistos; ++index) {

        if ( canvasNumber % (fNPadsX * fNPadsY) == 0 ) {
            ps->NewPage();
            canvasNumber = 0;
        }

        can -> cd(canvasNumber + 1);
        can -> SetFillColor(10);  // ringaile
        ++canvasNumber;

        // Loop over hist arrays
        for (Int_t arrayIdx = 0; arrayIdx < fHistArrays->GetEntriesFast(); ++arrayIdx) {

            ZeeDHistArray* histArray =
                static_cast<ZeeDHistArray*> ( fHistArrays->At(arrayIdx) );

            // Set histogram maximum
            TH1* hist = static_cast<TH1*>(histArray->GetHistArray()->At(index));
            hist->SetMaximum( histMaxArray[index] );

            if(fLogScale) {
                gPad -> SetLogy();
            }

            // First pad
            if (arrayIdx == 0) {

                if ( fUseStats ) {
                    gStyle->SetStatX(1.0);
                    gStyle->SetStatY(0.95);
                    gStyle->SetStatW(0.22);
                    gStyle->SetStatH(0.25);
                }

                histArray->DrawHist(index, fDrawOptionFirst );

            } else {
                histArray->DrawHist(index, fDrawOptionOther );
                can -> Update();

                if (fUseStats ) {
                    TPaveStats* st = (TPaveStats*)gPad->GetPrimitive("stats");

                    if (st != NULL ) {
                        Float_t oldy1 = st->GetY1NDC(); // 0.75
                        Float_t oldy2 = st->GetY2NDC(); // 1
                        Float_t newy1 = 2 * oldy1 - oldy2;
                        Float_t newy2 = oldy1;
                        st->SetTextColor( histArray->GetLineColor() );
                        st->SetY1NDC(newy1); // new y start position
                        st->SetY2NDC(newy2); // new y end position
                    }
                }
            }
        }

        // Redraw first array
        ZeeDHistArray* histArray =
            static_cast<ZeeDHistArray*> ( fHistArrays->At(0) );

        if ( histArray != NULL ) {
            histArray->DrawHist(index, fDrawOptionFirst + "sames" );
        }

        // Draw legend
        if (fDrawLegend && fLegend != NULL) {
            if (fDrawLegendOnce) {
                if ( canvasNumber == 1 ) {
                    fLegend->Draw("same");
                }
            } else {
                fLegend->Draw("same");
            }
        }

        gPad -> RedrawAxis();
        can -> Update();
    }

    can -> Update();
    ps  -> Close();

    delete can;
    delete ps;
    delete[] histMaxArray;

    
}

//------------------------------------------------------
void ZeeDHistPlotter::UseStats(Bool_t flag)
{
    // Change to the style with stats box
    fUseStats = flag;
    this->InitRoot();
}

//------------------------------------------------------
void ZeeDHistPlotter::InitRoot()
{
    //gROOT->Reset();  // ringaile. crashes in 15.6.1
    gROOT->SetStyle("Plain");

    TH1::AddDirectory(kFALSE);
    gROOT->SetBatch(kTRUE);

    //---=[ Stats ]=-------------------
    // Note: one can redefine this parameter in ZeeDPlotter
    // by calling gStyle->SetOptStat(1111);
    //
    if (fUseStats ) {
        gStyle->SetOptStat("emr"); // entries, mean, rms
    } else {
        gStyle->SetOptStat(0);      // no statistics _or_
        // gStyle->SetOptStat(11111111);
    }

    // Note: parameters below are redefined during the drawing
    gStyle->SetStatX(1.0);
    gStyle->SetStatY(0.95);
    gStyle->SetStatW(0.20);
    gStyle->SetStatH(0.30);

    gStyle->SetLineWidth(1);
    //    gStyle->SetErrorX(0);

    //---=[ Pad ]=---------------------
    gStyle -> SetPadTopMargin(0.05);
    gStyle -> SetPadBottomMargin(0.25);
    gStyle -> SetPadRightMargin(0.05);
    gStyle -> SetPadLeftMargin(0.22);

    if (fUseStats ) { // create space for stats
        gStyle -> SetPadRightMargin(0.23);
    }

    //---=[ Titles, Labels ]=----------
    gStyle -> SetOptTitle(0);             // title on/off
    gStyle -> SetTitleColor(0);           // title color
    gStyle -> SetTitleX(0.35);            // title x-position
    gStyle -> SetTitleH(0.1);             // title height
    gStyle -> SetTitleW(0.53);            // title width
    gStyle -> SetTitleFont(42);           // title font
    gStyle -> SetTitleFontSize(0.07);     // title font size

    //---=[ Histogram style ]=---------
    gStyle->SetHistFillColor(38);

    //---=[ Canvas,Frames ]=-----------
    gStyle -> SetFrameLineWidth(1);
    gStyle -> SetFrameBorderMode(0);
    gStyle -> SetCanvasBorderMode(0);
    gStyle -> SetCanvasColor(0);

    gStyle -> SetPadBorderMode(0);
    gStyle -> SetPadColor(0);

    //---=[ Ticks ]=-------------------
    gStyle -> SetTickLength(0.08, "X");
    gStyle -> SetTickLength(0.05, "Y");
}
