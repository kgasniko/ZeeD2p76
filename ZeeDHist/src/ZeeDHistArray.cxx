#include "ZeeDHist/ZeeDHistArray.h"

// std includes
#include <iostream>
#include <cstdlib>

// Root includes
#include <TROOT.h>
#include <TDirectory.h>
#include <TCollection.h>
#include <TPostScript.h>
#include <TKey.h>
#include <TStyle.h>
#include <TClass.h>
#include <TH2.h>
#include <TProfile.h>
#include <TSystem.h>

using std::cout;
using std::endl;

//------------------------------------------------------
ZeeDHistArray::ZeeDHistArray(TFile* file, TString dirName, TString pattern)
{

    // Constructor

    fHistArray = new TObjArray();
    fHistArray->SetName(dirName);

    this->SetPlottingStyle();

    if ( file->IsOpen() ) {
        file = (TFile*)gDirectory->Get(dirName);

        if ( file == NULL ) {
            TString message = TString("Can not find directory ");
            message.Append(dirName);

            Error("ZeeDHistArray::ZeeDHistArray", message);
            gSystem->Exit(EXIT_FAILURE);
        }

        TIter* iterator = new TIter( file->GetListOfKeys() );

        while ( TKey* key = (TKey*)iterator->Next()  ) {

            TObject* obj = key->ReadObj();

            if ( obj->IsA()->InheritsFrom( "TH1" ) ) {
                TH1* hist = static_cast<TH1*>( obj );
                TString name = hist->GetName();

                if ( name.Contains(pattern) ) {
                    fHistArray->AddLast(hist);
                }
            }
        }

        delete iterator;
    }
}

//------------------------------------------------------
ZeeDHistArray::ZeeDHistArray(const ZeeDHistArray& rhs) : TObject(rhs)
{
    // A copy constructor
    fHistArray = new TObjArray(*(rhs.fHistArray));

    this->SetPlottingStyle();
}

//------------------------------------------------------
ZeeDHistArray& ZeeDHistArray::operator=(const ZeeDHistArray& rhs)
{
    if (this == &rhs) {
        return *this;
    }

    TObject::operator=(rhs);
    this->SetPlottingStyle();
    fHistArray = new TObjArray(*(rhs.fHistArray));
    return *this;
}

//------------------------------------------------------
ZeeDHistArray::~ZeeDHistArray()
{
    // Destructor
    if (fHistArray != NULL ) {
        delete fHistArray;
    }
}

//------------------------------------------------------
void ZeeDHistArray::SetPlottingStyle()
{
    fFillStyle   = 0;
    fFillColor   = 40;
    fLineColor   = 1;
    fMarkerSize  = 0.7;
    fMarkerStyle = 8;
}

//------------------------------------------------------
void ZeeDHistArray::DrawHist(Int_t index, Option_t* option)
{
    if (index < 0 || index > this->GetEntriesFast() ) {
        return;
    }

    TH1* hist = static_cast<TH1*>(fHistArray->At(index));

    hist->SetNdivisions(505, "x");
    hist->SetNdivisions(505, "y");

    hist->GetXaxis()->SetLabelSize(0.09);
    hist->GetXaxis()->SetTitleSize(0.09);
    hist->GetXaxis()->SetTitleOffset(1.2);
    hist->GetXaxis()->SetLabelFont(12);     // times italic
    hist->GetXaxis()->SetTitleFont(32);     // times bold italic

    hist->GetYaxis()->SetLabelSize(0.09);
    hist->GetYaxis()->SetTitleSize(0.09);
    hist->GetYaxis()->SetTitleOffset(0.98);
    hist->GetYaxis()->SetLabelFont(12);     // times italic
    hist->GetYaxis()->SetTitleFont(32);     // times bold italic

    hist->SetMarkerStyle(fMarkerStyle);
    hist->SetMarkerSize(fMarkerSize);
    hist->SetMarkerColor(fLineColor);

    hist->SetLineColor(fLineColor);
    hist->SetFillStyle(fFillStyle);
    hist->SetFillColor(fFillColor);

    if (hist->InheritsFrom(TH2::Class())) {
        hist->SetMarkerStyle(1);
        hist->SetMarkerSize(0.2);

        // Fix root bug for plotting 2D histos
        TString str = TString(option);

        if (str.Contains("same") ) {
            option = " sames scat";
        } else {
            option = "SCAT";
        }
    }

    // Increase top margin of the histo
    if ( hist->InheritsFrom( TH1::Class() )) {
        Double_t max = hist->GetMaximum();
        hist->SetMaximum( max * 1.3 );

        hist->SetMarkerStyle(fMarkerStyle);
        hist->SetMarkerSize(fMarkerSize);
    }

    hist->Draw(option);
}

//------------------------------------------------------
const char* ZeeDHistArray::GetHistName(Int_t index) const
{
    // Returns the name of histogram
    if (index < 0 || index > this->GetEntriesFast() ) {
        return "";
    }

    TH1* hist = static_cast<TH1*>(fHistArray->At(index));
    return ( hist->GetName() );
}

//------------------------------------------------------
void ZeeDHistArray::CheckSize(ZeeDHistArray array) const
{
    // Compares size

    if (this->GetEntriesFast() != array.GetEntriesFast() ) {

        cout << "Tried to sum two arrays of histograms that contain " << endl;
        cout << "different numbers of histograms!!" << endl;
        cout << "HistArray " << this->GetName() << " has " << this->GetEntriesFast() << endl;
        cout << "HistArray " << array.GetName() << " has " << array.GetEntriesFast() << endl;

        gSystem->Exit(EXIT_FAILURE);
    }
}

//------------------------------------------------------
ZeeDHistArray ZeeDHistArray::operator+(const ZeeDHistArray& rhs) const
{
    this->CheckSize(rhs);

    ZeeDHistArray temp(*this);

    for (Int_t j = 0; j < temp.GetEntriesFast(); ++j) {
        const TH1* lhshist = static_cast<TH1*>( this->GetHistArray()->UncheckedAt(j) );
        const TH1* rhshist = static_cast<TH1*>( rhs.GetHistArray()  ->UncheckedAt(j) );
        TH1* resulthist    = static_cast<TH1*>( temp.GetHistArray() ->UncheckedAt(j) );
        resulthist->Add(lhshist, rhshist, 1.0, 1.0);
    }

    return temp;
}

//------------------------------------------------------
ZeeDHistArray& ZeeDHistArray::operator+=(const ZeeDHistArray& rhs)
{
    this->CheckSize(rhs);

    for (Int_t j = 0; j < this->GetEntriesFast(); ++j) {
        TH1* hist1       = static_cast<TH1*>( this->GetHistArray()->UncheckedAt(j) );
        const TH1* hist2 = static_cast<TH1*>( rhs.GetHistArray()  ->UncheckedAt(j) );

        hist1->Add(hist2, 1.0);
    }

    return *this;
}

//------------------------------------------------------
ZeeDHistArray ZeeDHistArray::operator+(const Double_t rhs) const
{
    ZeeDHistArray temp(*this);

    for (Int_t j = 0; j < temp.GetEntriesFast(); ++j) {

        TH1* hist = static_cast<TH1*>( temp.GetHistArray()->UncheckedAt(j) );

        for (Int_t k = 0; k <= hist->GetNbinsX() + 1; ++k) {
            hist->SetBinContent(k, hist->GetBinContent(k) + rhs);
        }

        //        hist->SetMaximum(hist->GetMaximum() + rhs);
    }

    return temp;
}

//------------------------------------------------------
ZeeDHistArray& ZeeDHistArray::operator+=(const Double_t rhs)
{
    for (Int_t j = 0; j < this->GetEntriesFast(); ++j) {

        TH1* hist = static_cast<TH1*>( this->GetHistArray()->UncheckedAt(j) );

        for (Int_t k = 0; k <= hist->GetNbinsX() + 1; ++k) {
            hist->SetBinContent(k, hist->GetBinContent(k) + rhs);
        }

        //        hist->SetMaximum(hist->GetMaximum() + rhs);
    }

    return *this;
}

//------------------------------------------------------
ZeeDHistArray ZeeDHistArray::operator-(const ZeeDHistArray& rhs) const
{
    this->CheckSize(rhs);

    ZeeDHistArray temp(*this);

    for (Int_t j = 0; j < temp.GetEntriesFast(); ++j) {
        const TH1* lhshist = static_cast<TH1*>( this->GetHistArray()->UncheckedAt(j) );
        const TH1* rhshist = static_cast<TH1*>( rhs.GetHistArray()  ->UncheckedAt(j) );
        TH1* resulthist    = static_cast<TH1*>( temp.GetHistArray() ->UncheckedAt(j) );
        resulthist->Add(lhshist, rhshist, 1.0, -1.0);
    }

    return temp;
}

//------------------------------------------------------
ZeeDHistArray& ZeeDHistArray::operator-=(const ZeeDHistArray& rhs)
{
    this->CheckSize(rhs);

    for (Int_t j = 0; j < this->GetEntriesFast(); ++j) {
        TH1* hist1       = static_cast<TH1*>( this->GetHistArray()->UncheckedAt(j) );
        const TH1* hist2 = static_cast<TH1*>( rhs.GetHistArray()  ->UncheckedAt(j) );

        hist1->Add(hist2, -1.0);
    }

    return *this;
}

//------------------------------------------------------
ZeeDHistArray ZeeDHistArray::operator-(const Double_t rhs) const
{
    ZeeDHistArray temp(*this);

    for (Int_t j = 0; j < temp.GetEntriesFast(); ++j) {

        TH1* hist = static_cast<TH1*>( temp.GetHistArray()->UncheckedAt(j) );

        for (Int_t k = 0; k <= hist->GetNbinsX() + 1; ++k) {
            hist->SetBinContent(k, hist->GetBinContent(k) - rhs);
        }

        //        hist->SetMaximum(hist->GetMaximum() - rhs);
    }

    return temp;
}

//------------------------------------------------------
ZeeDHistArray& ZeeDHistArray::operator-=(const Double_t rhs)
{
    for (Int_t j = 0; j < this->GetEntriesFast(); ++j) {

        TH1* hist = static_cast<TH1*>( this->GetHistArray()->UncheckedAt(j) );

        for (Int_t k = 0; k <= hist->GetNbinsX() + 1; ++k) {
            hist->SetBinContent(k, hist->GetBinContent(k) - rhs);
        }

        //        hist->SetMaximum(hist->GetMaximum() - rhs);
    }

    return *this;
}

//------------------------------------------------------
ZeeDHistArray ZeeDHistArray::operator*(const ZeeDHistArray& rhs) const
{
    this->CheckSize(rhs);

    ZeeDHistArray temp(*this);

    for (Int_t j = 0; j < temp.GetEntriesFast(); ++j) {
        const TH1* lhshist = static_cast<TH1*>( this->GetHistArray()->UncheckedAt(j) );
        const TH1* rhshist = static_cast<TH1*>( rhs.GetHistArray()  ->UncheckedAt(j) );
        TH1* resulthist    = static_cast<TH1*>( temp.GetHistArray() ->UncheckedAt(j) );
        resulthist->Multiply(lhshist, rhshist, 1.0, 1.0);
    }

    return temp;
}

//------------------------------------------------------
ZeeDHistArray& ZeeDHistArray::operator*=(const ZeeDHistArray& rhs)
{
    this->CheckSize(rhs);

    for (Int_t j = 0; j < this->GetEntriesFast(); ++j) {
        TH1* hist1       = static_cast<TH1*>( this->GetHistArray()->UncheckedAt(j) );
        const TH1* hist2 = static_cast<TH1*>( rhs.GetHistArray()  ->UncheckedAt(j) );

        hist1->Multiply(hist2);
    }

    return *this;
}

//------------------------------------------------------
ZeeDHistArray ZeeDHistArray::operator*(const Double_t rhs) const
{
    ZeeDHistArray temp(*this);

    for (Int_t j = 0; j < temp.GetEntriesFast(); ++j) {

        TH1* hist = static_cast<TH1*>( temp.GetHistArray()->UncheckedAt(j) );

        for (Int_t k = 0; k <= hist->GetNbinsX() + 1; ++k) {
            hist->SetBinContent( k, hist->GetBinContent(k) * rhs );
            hist->SetBinError( k, hist->GetBinError(k) * rhs );
        }

        //        hist->SetMaximum(hist->GetMaximum()*rhs);
    }

    return temp;
}

//------------------------------------------------------
ZeeDHistArray& ZeeDHistArray::operator*=(const Double_t rhs)
{
    // Multiply each histogram in this hist array by a number.

    for (Int_t j = 0; j < this->GetEntriesFast(); ++j) {

        TH1* hist = static_cast<TH1*>( this->GetHistArray()->UncheckedAt(j) );

        for (Int_t k = 0; k <= hist->GetNbinsX() + 1; ++k) {
            hist->SetBinContent( k, hist->GetBinContent(k) * rhs );
            hist->SetBinError( k, hist->GetBinError(k) * rhs );
        }

        //        hist->SetMaximum(hist->GetMaximum()*rhs);
    }

    return *this;
}

//------------------------------------------------------
ZeeDHistArray ZeeDHistArray::operator/(const ZeeDHistArray& rhs) const
{
    this->CheckSize(rhs);

    ZeeDHistArray temp(*this);

    for (Int_t j = 0; j < temp.GetEntriesFast(); ++j) {
        const TH1* lhshist = static_cast<TH1*>( this->GetHistArray()->UncheckedAt(j) );
        const TH1* rhshist = static_cast<TH1*>( rhs.GetHistArray()  ->UncheckedAt(j) );
        TH1* resulthist    = static_cast<TH1*>( temp.GetHistArray() ->UncheckedAt(j) );
        resulthist->Divide(lhshist, rhshist, 1.0, 1.0, "b");
    }

    return temp;
}

//------------------------------------------------------
ZeeDHistArray& ZeeDHistArray::operator/=(const ZeeDHistArray& rhs)
{
    this->CheckSize(rhs);

    for (Int_t j = 0; j < this->GetEntriesFast(); ++j) {
        TH1* hist1       = static_cast<TH1*>( this->GetHistArray()->UncheckedAt(j) );
        const TH1* hist2 = static_cast<TH1*>( rhs.GetHistArray()  ->UncheckedAt(j) );

        hist1->Divide(hist2);
    }

    return *this;
}

//------------------------------------------------------
ZeeDHistArray ZeeDHistArray::operator/(const Double_t rhs) const
{
    ZeeDHistArray temp(*this);

    if (rhs == 0.0 ) {
        Error("ZeeDHistArray::operator/", "Division by zero");
        return temp;
    }

    for (Int_t j = 0; j < temp.GetEntriesFast(); ++j) {

        TH1* hist = static_cast<TH1*>( temp.GetHistArray()->UncheckedAt(j) );

        for (Int_t k = 0; k <= hist->GetNbinsX() + 1; ++k) {
            hist->SetBinContent( k, hist->GetBinContent(k) / rhs );
            hist->SetBinError( k, hist->GetBinError(k) / rhs );
        }

        //        hist->SetMaximum(hist->GetMaximum() / rhs);
    }

    return temp;
}

//------------------------------------------------------
ZeeDHistArray& ZeeDHistArray::operator/=(const Double_t rhs)
{
    if (rhs == 0.0 ) {
        Error("ZeeDHistArray::operator/=", "Division by zero");
        return *this;
    }

    for (Int_t j = 0; j < this->GetEntriesFast(); ++j) {

        TH1* hist = static_cast<TH1*>( this->GetHistArray()->UncheckedAt(j) );

        for (Int_t k = 0; k <= hist->GetNbinsX() + 1; ++k) {
            hist->SetBinContent( k, hist->GetBinContent(k) / rhs );
            hist->SetBinError( k, hist->GetBinError(k) / rhs );
        }

        //        hist->SetMaximum(hist->GetMaximum() / rhs);
    }

    return *this;
}
