#ifndef __ZEEDCOLLECTION_H
#define __ZEEDCOLLECTION_H

////////////////////////////////////////////////////////
// Name    : ZeeDArrayI.h
////////////////////////////////////////////////////////

#include <TObject.h>
#include <TError.h>
#include "TString.h"

/** Base class for the simple array classes
    @author  Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
    @date 2008/08/07
 */
class ZeeDCollection : public TObject {

protected:

    Bool_t BoundsOk(const char* where, Int_t at, Int_t size) const {
        return (at < 0 || at >= size)
               ? OutOfBoundsError(where, at)
               : kTRUE;
    }

    Bool_t OutOfBoundsError(const char* where, Int_t i) const {
        Error(where, "index %d out of bounds (entries: %d, size: %d, this: %p)",
              i, fLast + 1, fCapacity, (void*)this);
        return kFALSE;
    }

    /**  Number of actual objects stored in array */
    Int_t  fLast; //!

    /**  Number of allocated array elements */
    Int_t  fCapacity; //!

    /**  boolean flag indicating whether elements in array are sorted by size */
    Bool_t fSorted; //!

public:

    virtual ~ZeeDCollection() {}

    Int_t GetSize() const {
        return fCapacity;
    }

    Int_t GetLast() const {
        return fLast;
    }

    void SetLast(Int_t last) {
        fLast = last;
    }

    virtual void SetSize(Int_t size) = 0;
    virtual void DumpArray() const = 0;

};

#endif
