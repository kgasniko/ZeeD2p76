#include "ZeeDArrays/ZeeDArrayD.h"

#include <iostream>
#include <TMath.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>


using namespace std;

Double_t ZeeDArrayD::fDummy = 0.0;

//------------------------------------------------------
ZeeDArrayD::ZeeDArrayD()
{
    // Constructor

    fArray    = NULL;
    fLast     = -1;
    fCapacity = 0;
}

//------------------------------------------------------
ZeeDArrayD::ZeeDArrayD(Int_t size)
{

    fArray    = NULL;
    fLast     = -1;
    fCapacity = 0;

    if (size > 0) {
        this->SetSize(size);
    } else {
        Error("ZeeDArrayD::ZeeDArrayD", "invalide size %i", size);
    }
}

//------------------------------------------------------
ZeeDArrayD::ZeeDArrayD(Int_t size, const Double_t* array)
{

    fArray = NULL;
    fLast  = size - 1;
    fCapacity     = size;
    this->Set(size, array);
}

//------------------------------------------------------
ZeeDArrayD::ZeeDArrayD(const ZeeDArrayD& array) : ZeeDCollection(array)
{
    // Copy constructor
    fArray = NULL;
    this->Set(array.fCapacity, array.fArray);

    // Set method overrides fLast as (fCapacity -1)
    fLast = array.fLast;
}

//------------------------------------------------------
ZeeDArrayD& ZeeDArrayD::operator= (const ZeeDArrayD& rhs)
{
    // ZeeDArrayD assignment operator

    if (this != &rhs) {
        this->Set(rhs.fCapacity, rhs.fArray);
        fLast = rhs.fLast;
    }

    return *this;
}

//------------------------------------------------------
ZeeDArrayD::~ZeeDArrayD()
{
    // Destructor
    if ( fArray != NULL ) {
        delete [] fArray;
        fArray = NULL;
    }
}

//------------------------------------------------------
void ZeeDArrayD::Add(Double_t number)
{
    this->AddLast(number);
}

//------------------------------------------------------
void ZeeDArrayD::AddLast(Double_t number)
{
    this->AddAtAndExpand(number, fLast + 1);
}

//------------------------------------------------------
void ZeeDArrayD::AddAtAndExpand(Double_t number, Int_t idx)
{

    if (idx < 0) {
        Error("ZeeDArrayD::AddAtAndExpand",
              "out of bounds at %d in %p", idx, static_cast<void*>(this));
        return;
    }

    if ( idx >= fCapacity ) {
        if ( fCapacity == 0 ) {
            this->SetSize(1);
        } else {
            this->SetSize( 2 * fCapacity );
        }
    }

    if ( !BoundsOk("ZeeDArrayD::AddAtAndExpand", fLast + 1, fCapacity) ) {
        Error("ZeeDArrayD::AddAtAndExpand",
              "out of bounds at %d in %p", fLast + 1, static_cast<void*>(this));
    }

    fArray[fLast + 1] = number;
    fLast = TMath::Max(idx, fLast);
}

//------------------------------------------------------
void ZeeDArrayD::AddAt(Double_t number, Int_t index)
{

    if ( BoundsOk("ZeeDArrayD::AddAt", index, fCapacity) ) {
        fArray[index] = number;
    }

    if ( index > fLast ) {
        fLast = index;
    }
}

//------------------------------------------------------

Double_t& ZeeDArrayD::operator[](Int_t index)
{
    if ( !BoundsOk("ZeeDArrayD::operator[]", index, fCapacity) ) {
        return fDummy;
    }

    if ( index > fLast ) {
        fLast = index;
    }

    return fArray[index];
}

//------------------------------------------------------
void ZeeDArrayD::Reset(Double_t val)
{
    for ( Int_t i = 0; i < fCapacity; i++ )  {
        fArray[i] = val;
    }

    fLast   = -1;
}

//------------------------------------------------------
void ZeeDArrayD::Compress()
{
    this->SetSize(fLast + 1);
}

//------------------------------------------------------
void ZeeDArrayD::Adopt(Int_t size, Double_t* arr)
{

    if ( fArray != NULL ) {
        delete [] fArray;
    }

    fCapacity = size;
    fArray    = arr;
    fLast     = size - 1;
}

//------------------------------------------------------
void ZeeDArrayD::SetSize(Int_t size)
{

    if (size < 0) {
        return;
    }

    if (size != fCapacity) {
        Double_t* temp = fArray;

        if ( size != 0 ) {
            fArray = new Double_t[size];

            if ( size < fCapacity ) {
                memcpy(fArray, temp, size * sizeof(Double_t));
            } else {
                memcpy(fArray, temp, fCapacity * sizeof(Double_t));
                memset(&fArray[fCapacity], 0, (size - fCapacity)*sizeof(Double_t));
            }
        } else {
            fArray = NULL;
        }

        if ( fCapacity ) {
            delete [] temp;
        }

        fCapacity = size;
        fLast = TMath::Min(fLast, size - 1);
    }
}

//------------------------------------------------------
void ZeeDArrayD::Set(Int_t size, const Double_t* array)
{

    if ( size < 0 || array == NULL ) {
        return;
    }

    if ( fArray && fCapacity != size) {
        delete [] fArray;
        fArray = 0;
    }

    fCapacity = size;
    fLast     = size - 1;

    if ( fCapacity == 0 ) {
        return;
    }

    if ( !fArray )  {
        fArray = new Double_t[fCapacity];
    }

    memcpy(fArray, array, size * sizeof(Double_t));
}

//------------------------------------------------------
ZeeDArrayD ZeeDArrayD::operator+(const ZeeDArrayD& rhs) const
{
    return ZeeDArrayD(*this) += rhs;
}

//------------------------------------------------------
ZeeDArrayD ZeeDArrayD::operator+(Double_t number) const
{
    return ZeeDArrayD(*this) += number;
}

//------------------------------------------------------
ZeeDArrayD& ZeeDArrayD::operator+=(const ZeeDArrayD& rhs)
{

    Int_t originalSize = fLast + 1;
    Int_t newSize      = (fLast + 1) + (rhs.fLast + 1);

    this->SetSize(newSize);

    // copy the extra members
    for ( Int_t i = originalSize; i < newSize; i++ ) {
        fArray[i] = rhs.fArray[i - originalSize];
    }

    fLast   = newSize - 1;

    return *this;
}

//------------------------------------------------------
ZeeDArrayD& ZeeDArrayD::operator+=(Double_t number)
{

    this->Add(number);

    return *this;
}

//------------------------------------------------------
void ZeeDArrayD::DumpArray() const
{
    for ( Int_t i = 0; i <= fLast; i++ ) {
        cout << fArray[i] << " ";
    }

    cout << endl;
}

//------------------------------------------------------
Int_t ZeeDArrayD::GetMinElement() const
{
    Int_t minIdx = 0;

    for ( Int_t index = 1; index <= fLast; index++ ) {
        if (fArray[index] < fArray[minIdx] ) {
            minIdx = index;
        }
    }

    return minIdx;
}

//------------------------------------------------------
Int_t ZeeDArrayD::GetMaxElement() const
{
    Int_t maxIdx = 0;

    for ( Int_t index = 1; index <= fLast; index++ ) {
        if (fArray[index] > fArray[maxIdx] ) {
            maxIdx = index;
        }
    }

    return maxIdx;
}

//------------------------------------------------------
void ZeeDArrayD::Sort()
{
    Int_t first = 0;
    Int_t last  = fLast;

    this->Quicksort(first, last);
}

//------------------------------------------------------
void ZeeDArrayD::Quicksort(Int_t first, Int_t last)
{
    if ( first < last ) {
        Int_t pivotIndex = this->Partition(first, last);
        this->Quicksort(first, pivotIndex - 1);
        this->Quicksort(pivotIndex + 1, last);
    }
}

//------------------------------------------------------
Int_t ZeeDArrayD::Partition(Int_t first, Int_t last)
{
    Double_t pivot = fArray[first];
    Int_t lo = first;
    Int_t hi = last;

    while ( lo <= hi ) {
        while ((lo <= last) && (fArray[lo] <= pivot)) {
            lo++;
        }

        while (fArray[hi] > pivot) {
            hi--;
        }

        if (lo < hi ) {
            this->Swap(lo, hi);
            lo++;
            hi--;
        }
    }

    this->Swap(first, hi);
    return hi;
}

//------------------------------------------------------
void ZeeDArrayD::Swap(Int_t i, Int_t j)
{
    Double_t temp = fArray[i];
    fArray[i]  = fArray[j];
    fArray[j]  = temp;
}

//------------------------------------------------------
void ZeeDArrayD::Unique()
{
    ZeeDArrayD temp(fLast + 1);

    for ( Int_t i = 0; i <= fLast; i++ ) {
        Double_t  value  = fArray[i];
        Bool_t unique = kTRUE;

        for ( Int_t j = 0; j < temp.GetEntriesFast(); j++ ) {
            if ( value == temp[j] ) {
                unique = kFALSE;
            }
        }

        if ( unique ) {
            temp += value;
        }
    }

    temp.Compress();

    (*this) = temp;
}
