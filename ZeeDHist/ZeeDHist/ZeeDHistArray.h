#ifndef __ZEEDHISTARRAY_H
#define __ZEEDHISTARRAY_H

////////////////////////////////////////////////////////
// Name    : ZeeDHistArray.h
////////////////////////////////////////////////////////

// Root includes
#include <TObject.h>
#include <TObjArray.h>

#include <TFile.h>
#include <TH1.h>
#include <TString.h>

/** Basic class for storing and plotting array of histograms
    @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
    @date 2008/08/15
*/
class ZeeDHistArray : public TObject {

public:

    /** Reads directory and fills all histos in the array */
    ZeeDHistArray(TFile* file, TString dirName, TString pattern = "");
    ZeeDHistArray(const ZeeDHistArray& rhs);
    ZeeDHistArray& operator=(const ZeeDHistArray& rhs);

    virtual ~ZeeDHistArray();

    void SetLineColor(Int_t color) {
        fLineColor = color;
    }
    void SetFillColor(Int_t color) {
        fFillColor = color;
    }
    void SetFillStyle(Int_t style) {
        fFillStyle = style;
    }
    void SetMarkerSize(Double_t size) {
        fMarkerSize = size;
    }
    void SetMarkerStyle(Double_t style) {
        fMarkerStyle = style;
    }

    Int_t GetEntriesFast() const {
        return fHistArray->GetEntriesFast();
    }
    TObjArray* GetHistArray() const {
        return fHistArray;
    }

    /** Returns name of  histogram stored at index position in the fHistArray */
    const char* GetHistName(Int_t index) const;

    /** Draws histogram stored at index position in the fHistArray */
    void DrawHist(Int_t index, Option_t* option);

    /** Add another histogram array to a copy of this one, leaving the
        original unchanged. Uses the TH1::Add() function
    */
    ZeeDHistArray  operator+(const ZeeDHistArray& rhs) const;

    /** Add another histogram array to this one. Uses the TH1::Add()
        function
    */
    ZeeDHistArray& operator+=(const ZeeDHistArray& rhs);

    /** Add a number to each histogram in a copy of this hist array,
        leaving the original unchanged.
    */
    ZeeDHistArray  operator+(const Double_t rhs) const;

    /** Add a number to each histogram in this histogram array. */
    ZeeDHistArray& operator+=(const Double_t rhs);

    /** Subtract another histogram array from a copy of this one,
        leaving the original unchanged. This uses the TH1::Add(hist, -1)
        function for each histogram
    */
    ZeeDHistArray  operator-(const ZeeDHistArray& rhs) const;

    /** Subtract another histogram array to this one.
        Uses the TH1::Add(hist, -1) function
    */
    ZeeDHistArray& operator-=(const ZeeDHistArray& rhs);

    /** Subtract a number to each histogram in a copy of this hist array,
        leaving the original unchanged.
    */
    ZeeDHistArray  operator-(const Double_t rhs) const;

    /** Subtract a number to each histogram in this histogram array. */
    ZeeDHistArray& operator-=(const Double_t rhs);

    /** Multiply another histogram array by a copy of this one,
        leaving the original unchanged. This uses the  TH1::Multiply()
        function for each histogram.
    */
    ZeeDHistArray  operator*(const ZeeDHistArray& rhs) const;

    /** Multiply another histogram array to this one.
        Uses the TH1::Multiply() function
    */
    ZeeDHistArray& operator*=(const ZeeDHistArray& rhs);

    /** Multiply each histogram in a copy of this hist array by a number,
        leaving the original unchanged.
    */
    ZeeDHistArray  operator*(const Double_t rhs) const;

    /** Multiply each histogram in this hist array by a number. */
    ZeeDHistArray& operator*=(const Double_t rhs);

    /** Divide another histogram array by a copy of this one,
        leaving the original unchanged. This uses the  TH1::Multiply()
        function for each histogram.
    */
    ZeeDHistArray  operator/(const ZeeDHistArray& rhs) const;

    /** Divide another histogram array to this one.
        Uses the TH1::Multiply() function
    */
    ZeeDHistArray& operator/=(const ZeeDHistArray& rhs);

    /** Divide each histogram in a copy of this hist array by a number,
        leaving the original unchanged.
    */
    ZeeDHistArray  operator/(const Double_t rhs) const;

    /** Divide each histogram in this hist array by a number. */
    ZeeDHistArray& operator/=(const Double_t rhs);

    /** Returns color of the line */
    Int_t GetLineColor() const {
        return fLineColor;
    }

private:

    /**  array of histograms */
    TObjArray* fHistArray; //!

    /**  fill style */
    Int_t fFillStyle; //!

    /**  fill color */
    Int_t fFillColor; //!

    /**  line color */
    Int_t fLineColor; //!

    /** Marker size */
    Double_t fMarkerSize;   //!

    /** Marker style */
    Double_t fMarkerStyle;   //!

    /** Sets initial plotting style */
    void SetPlottingStyle();

    void CheckSize(ZeeDHistArray array) const;
};

#endif // ZeeDHistArray
