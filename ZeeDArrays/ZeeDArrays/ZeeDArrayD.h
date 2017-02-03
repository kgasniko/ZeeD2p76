#ifndef __ZEEDARRAYD_H
#define __ZEEDARRAYD_H

////////////////////////////////////////////////////////
// Name    : ZeeDArrayD.h
////////////////////////////////////////////////////////


#include "ZeeDCollection.h"


/** Array of double precision numbers (32/64 bits per element)

   @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
   @date 2008/09/30

 */
class ZeeDArrayD : public ZeeDCollection {

public:

    ZeeDArrayD();

    /** Create ZeeDArrayD object and set array size to size intergers */
    ZeeDArrayD(Int_t size);

    /** Create ZeeDArrayD object and initialize it with values of array */
    ZeeDArrayD(Int_t size, const Double_t* array);
    ZeeDArrayD(const ZeeDArrayD& array);
    ZeeDArrayD& operator=(const ZeeDArrayD& rhs);
    ~ZeeDArrayD();

    const Double_t* GetArray() const {
        return fArray;
    }


    /** Adopt array arr into ZeeDArrayD, i.e. don't copy arr but use it directly
        in ZeeDArrayD. User may not delete arr, ZeeDArrayD dtor will do it.
    */
    void Adopt(Int_t size, Double_t* array);
    void Compress();

    /** Add number to the array */
    void Add(Double_t number);

    /** Add number to the array */
    void AddLast(Double_t number);

    /** Add Double_t number at position index. Check for out of bounds */
    void AddAt(Double_t number, Int_t index);

    Double_t At(Int_t index) const {
        if ( !BoundsOk("ZeeDArrayD::At", index, fLast + 1) ) {
            return -1;
        }

        return fArray[index];
    }

    Double_t operator[](Int_t index) const {
        return this->At(index);
    }

    Double_t& operator[](Int_t index);

    /** Returns a new array, which is the addition of this plus rhs
        (appended to this)
    */
    ZeeDArrayD  operator+(const ZeeDArrayD& rhs) const;

    /** Returns a new array, which is the addition of this plus number
        (as last element)
    */
    ZeeDArrayD  operator+(Double_t number) const;

    /** Adds another ZeeDArrayD onto the end of this one */
    ZeeDArrayD& operator+=(const ZeeDArrayD& rhs);

    /** Adds number onto the end of this */
    ZeeDArrayD& operator+=(Double_t number);

    /** Reset the number of stored elements to zero */
    void Reset(Double_t val = 0.0);

    /** Number of actually stored elements */
    Int_t GetEntriesFast() const {
        return fLast + 1;
    }

    /** Print all objects in this array */
    void DumpArray() const;

    /** Returns index of the smallest element in the array */
    Int_t GetMinElement() const;

    /** Returns index of the largest element in the array */
    Int_t GetMaxElement() const;

    /** Removes dublicated elements from the array */
    void Unique();

    /** Sorts elements in the array */
    void Quicksort(Int_t first, Int_t last);

    /** Sorts elements in the array */
    void Sort();

private:

    /** Dummy */
    static Double_t fDummy; //!

    /** Array of doubles */
    Double_t* fArray;      //!

    /** Add object at position idx. If idx is larger than the current size
        of the array, expand the array (double its size).
    */
    void AddAtAndExpand(Double_t number, Int_t idx);

    /** Set size of this array to size ints.
        A new array is created, the old contents copied to the new array,
        then the old array is deleted.

        If the new array is larger, all the new entries are set to zero
    */
    void SetSize(Int_t size);  // Set (maximal) size of array

    /** Set size of this array to size ints and set the contents.
    */
    void Set(Int_t size, const Double_t* array);

    /** Swaps two elements in the array */
    void Swap(Int_t i, Int_t j);
    Int_t Partition(Int_t first, Int_t last);
};

#endif
