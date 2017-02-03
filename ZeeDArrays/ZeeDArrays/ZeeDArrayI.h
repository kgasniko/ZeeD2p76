#ifndef __ZEEDARRAYI_H
#define __ZEEDARRAYI_H

////////////////////////////////////////////////////////
// Name    : ZeeDArrayI.h
////////////////////////////////////////////////////////


#include "ZeeDCollection.h"
//#include <Htypes.h>

class ZeeDArrayI : public ZeeDCollection {

public:

    ZeeDArrayI();
    ZeeDArrayI(Int_t size);
    ZeeDArrayI(Int_t size, const Int_t* array);
    ZeeDArrayI(const ZeeDArrayI& array);
    ZeeDArrayI& operator=(const ZeeDArrayI& rhs);
    ~ZeeDArrayI();

    const Int_t* GetArray() const {
        return fArray;
    }

    void Adopt(Int_t size, Int_t* array);
    void Compress();

    void Clear(Option_t* option = "");

    void Add(Int_t number);
    void AddLast(Int_t number);
    void AddAt(Int_t number, Int_t index);

    Int_t At(Int_t index) const {
        if ( !BoundsOk("ZeeDArrayI::At", index, fLast + 1) ) {
            return -1;
        }

        return fArray[index];
    }

    Int_t operator[](Int_t index) const {
        return this->At(index);
    }

    Int_t& operator[](Int_t index);

    ZeeDArrayI  operator+(const ZeeDArrayI& rhs) const;
    ZeeDArrayI  operator+(Int_t number) const;
    ZeeDArrayI& operator+=(const ZeeDArrayI& rhs);
    ZeeDArrayI& operator+=(Int_t number);

    void Reset(Int_t val = 0); // Reset the number of stored elements to zero
    Int_t GetEntriesFast() const {
        return fLast + 1;    // number of actually stored elements
    }
    void DumpArray() const;

    void Unique();
    void Remove(Int_t element);
    void Quicksort(Int_t first, Int_t last);
    void Sort();

private:

    static Int_t fDummy; //! Dummy
    Int_t* fArray;       //! Array of integers

    void AddAtAndExpand(Int_t number, Int_t idx);
    void SetSize(Int_t size);  // Set (maximal) size of array
    void Set(Int_t size, const Int_t* array);
    void Swap(Int_t i, Int_t j);
    Int_t Partition(Int_t first, Int_t last);
};

#endif
