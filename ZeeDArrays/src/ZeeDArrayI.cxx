#include "ZeeDArrays/ZeeDArrayI.h"

#include <iostream>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <TMath.h>

using namespace std;

Int_t ZeeDArrayI::fDummy = 0;

//------------------------------------------------------
ZeeDArrayI::ZeeDArrayI()
{
    // Constructor

    fArray    = NULL;
    fLast     = -1;
    fCapacity = 0;
}

//------------------------------------------------------
ZeeDArrayI::ZeeDArrayI(Int_t size)
{
    // Create ZeeDArrayI object and set array size to size intergers

    fArray    = NULL;
    fLast     = -1;
    fCapacity = 0;

    if (size > 0) {
        this->SetSize(size);
    } else {
        Error("ZeeDArrayI::ZeeDArrayI", "invalide size %i", size);
    }
}

//------------------------------------------------------
ZeeDArrayI::ZeeDArrayI(Int_t size, const Int_t* array)
{
    // Create ZeeDArrayI object and initialize it with values of array

    fArray = NULL;
    fLast  = size - 1;
    fCapacity     = size;
    this->Set(size, array);
}

//------------------------------------------------------
ZeeDArrayI::ZeeDArrayI(const ZeeDArrayI& array) : ZeeDCollection(array)
{
    // Copy constructor

    fArray = NULL;
    this->Set(array.fCapacity, array.fArray);

    // Set method overrides fLast as (fCapacity -1)
    fLast = array.fLast;
}

//------------------------------------------------------
ZeeDArrayI& ZeeDArrayI::operator= (const ZeeDArrayI& rhs)
{
    // ZeeDArrayI assignment operator

    if (this != &rhs) {
        this->Set(rhs.fCapacity, rhs.fArray);
        fLast = rhs.fLast;
    }

    return *this;
}

//------------------------------------------------------
ZeeDArrayI::~ZeeDArrayI()
{
    // Destructor
    if ( fArray != NULL ) {
        delete [] fArray;
        fArray = NULL;
    }
}

//------------------------------------------------------
void ZeeDArrayI::Add(Int_t number)
{
    // Add number to the array
    this->AddLast(number);
}

//------------------------------------------------------
void ZeeDArrayI::AddLast(Int_t number)
{
    // Add number to the array
    this->AddAtAndExpand(number, fLast + 1);
}

//------------------------------------------------------
void ZeeDArrayI::AddAtAndExpand(Int_t number, Int_t idx)
{
    // Add object at position idx. If idx is larger than the current size
    // of the array, expand the array (double its size).
    if (idx < 0) {
        Error("ZeeDArrayI::AddAtAndExpand", "out of bounds at %d in %p", idx, static_cast<void*>(this));
        return;
    }

    if ( idx >= fCapacity ) {
        if ( fCapacity == 0 ) {
            this->SetSize(1);
        } else {
            this->SetSize( 2 * fCapacity );
        }
    }

    if ( !BoundsOk("ZeeDArrayI::AddAtAndExpand", fLast + 1, fCapacity) ) {
        Error("ZeeDArrayI::AddAtAndExpand", "out of bounds at %d in %p", fLast + 1, static_cast<void*>(this));
    }

    fArray[fLast + 1] = number;
    fLast = TMath::Max(idx, fLast);
}

//------------------------------------------------------
void ZeeDArrayI::AddAt(Int_t number, Int_t index)
{
    // Add Int_t number at position index. Check for out of bounds
    if ( BoundsOk("ZeeDArrayI::AddAt", index, fCapacity) ) {
        fArray[index] = number;
    }

    if ( index > fLast ) {
        fLast = index;
    }
}

//------------------------------------------------------

Int_t& ZeeDArrayI::operator[](Int_t index)
{
    if ( !BoundsOk("ZeeDArrayI::operator[]", index, fCapacity) ) {
        return fDummy;
    }

    if ( index > fLast ) {
        fLast = index;
    }

    return fArray[index];
}

//------------------------------------------------------
void ZeeDArrayI::Reset(Int_t val)
{
    for ( Int_t i = 0; i < fCapacity; i++ )  {
        fArray[i] = val;
    }

    fLast   = -1;
}

//------------------------------------------------------
void ZeeDArrayI::Clear(Option_t* option)
{
    option = "";
    fLast  = -1;
    this->SetSize(fLast + 1);
}

//------------------------------------------------------
void ZeeDArrayI::Compress()
{
    this->SetSize(fLast + 1);
}

//------------------------------------------------------
void ZeeDArrayI::Adopt(Int_t size, Int_t* arr)
{
    // Adopt array arr into ZeeDArrayI, i.e. don't copy arr but use it directly
    // in ZeeDArrayI. User may not delete arr, ZeeDArrayI dtor will do it.

    if ( fArray != NULL ) {
        delete [] fArray;
    }

    fCapacity = size;
    fArray    = arr;
    fLast     = size - 1;
}

//------------------------------------------------------
void ZeeDArrayI::SetSize(Int_t size)
{
    // Set size of this array to size ints.
    // A new array is created, the old contents copied to the new array,
    // then the old array is deleted.
    //
    // If the new array is larger, all the new entries are set to zero

    if (size < 0) {
        return;
    }

    if (size != fCapacity) {
        Int_t* temp = fArray;

        if ( size != 0 ) {
            fArray = new Int_t[size];

            if ( size < fCapacity ) {
                memcpy(fArray, temp, size * sizeof(Int_t));
            } else {
                memcpy(fArray, temp, fCapacity * sizeof(Int_t));
                memset(&fArray[fCapacity], 0, (size - fCapacity)*sizeof(Int_t));
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
void ZeeDArrayI::Set(Int_t size, const Int_t* array)
{
    // Set size of this array to size ints and set the contents.

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
        fArray = new Int_t[fCapacity];
    }

    memcpy(fArray, array, size * sizeof(Int_t));
}

//------------------------------------------------------
ZeeDArrayI ZeeDArrayI::operator+(const ZeeDArrayI& rhs) const
{
    // Returns a new array,
    // which is the addition of this plus rhs (appended to this)
    return ZeeDArrayI(*this) += rhs;
}

//------------------------------------------------------
ZeeDArrayI ZeeDArrayI::operator+(Int_t number) const
{
    // Returns a new array,
    // which is the addition of this plus number (as last element)

    return ZeeDArrayI(*this) += number;
}

//------------------------------------------------------
ZeeDArrayI& ZeeDArrayI::operator+=(const ZeeDArrayI& rhs)
{
    // Adds another ZeeDArrayI onto the end of this one

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
ZeeDArrayI& ZeeDArrayI::operator+=(Int_t number)
{
    // Adds number onto the end of this

    this->Add(number);

    return *this;
}

//------------------------------------------------------
void ZeeDArrayI::Sort()
{
    // Sorts elements in the array
    Int_t first = 0;
    Int_t last  = fLast;

    this->Quicksort(first, last);
}

//------------------------------------------------------
void ZeeDArrayI::Quicksort(Int_t first, Int_t last)
{
    // Sorts elements in the array
    if ( first < last ) {
        Int_t pivotIndex = this->Partition(first, last);
        this->Quicksort(first, pivotIndex - 1);
        this->Quicksort(pivotIndex + 1, last);
    }
}

//------------------------------------------------------
Int_t ZeeDArrayI::Partition(Int_t first, Int_t last)
{
    Int_t pivot = fArray[first];
    Int_t lo    = first;
    Int_t hi    = last;

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
void ZeeDArrayI::Swap(Int_t i, Int_t j)
{
    // Swaps two elemnts in the array
    Int_t temp = fArray[i];
    fArray[i]  = fArray[j];
    fArray[j]  = temp;
}

//------------------------------------------------------
void ZeeDArrayI::Unique()
{
    // Removes dublicated elements from the array

    const Int_t size = fLast + 1;

    if(size <= 0) return;

    ZeeDArrayI temp(size);

    for ( Int_t i = 0; i <= fLast; i++ ) {
        Int_t  value  = fArray[i];
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

//------------------------------------------------------
void ZeeDArrayI::Remove(Int_t element)
{
    // Removes element from the array

    const Int_t size = fLast + 1;

    if(size <= 0) return;

    ZeeDArrayI temp(size);

    for ( Int_t i = 0; i <= fLast; i++ ) {
        Int_t  value  = fArray[i];

        if ( value != element ) {
            temp += value;
        }
    }

    temp.Compress();

    (*this) = temp;
}

//------------------------------------------------------
void ZeeDArrayI::DumpArray() const
{
    // Print all objects in this array

    for ( Int_t i = 0; i <= fLast; i++ ) {
        cout << fArray[i] << " ";
    }

    cout << endl;
}
