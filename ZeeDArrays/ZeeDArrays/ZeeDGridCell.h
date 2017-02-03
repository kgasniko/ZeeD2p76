#ifndef __ZEEDGRIDCELL_H
#define __ZEEDGRIDCELL_H

////////////////////////////////////////////////////////
// Name    : ZeeDGridCell.h
////////////////////////////////////////////////////////


#include <TObject.h>
#include "ZeeDArrays/ZeeDArrayD.h"
#include "ZeeDArrays/ZeeDArrayI.h"


/** Class to define a cell of a bin grid
    @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
    @date 2008/09/30
 */
class ZeeDGridCell : public TObject {

public:

    ZeeDGridCell(ZeeDArrayD low, ZeeDArrayD high);

    ZeeDGridCell(const Int_t size,
                 const Double_t* low, const Double_t* high);
    ZeeDGridCell(const Int_t numDimentions, const Double_t* coord);

    ZeeDGridCell(const ZeeDGridCell& cell);

    ~ZeeDGridCell();

    // Math
    ZeeDGridCell& operator=(const ZeeDGridCell& rhs);

    /** Add another cell to  this one */
    ZeeDGridCell& operator+=(const ZeeDGridCell& rhs);

    /** Add another cell to this one */
    void AddCell(ZeeDGridCell* cell);

    /** Check wether point is inside this cell */
    Bool_t isInside(const ZeeDArrayD* point) const;

    /** Checks wether cell is neighbour for this one */
    Bool_t isNeighbour(const ZeeDGridCell* cell) const;

    // Setters
    void SetValue(Double_t value) {
        fValue = value;
    }
    void AddValue(Double_t value) {
        fValue += value;
    }
    void SetNeighbours(ZeeDArrayI array) {
        fNeighbours = array;
    }
    void SetMergedCellIdx(Int_t index) {
        fMergedCellIdx = index;
    }

    // Getters
    Int_t GetNumDimentions() const {
        return fNumDimentions;
    }
    Double_t GetValue() const {
        return fValue;
    }

    const ZeeDArrayD* GetCoordLow()   const {
        return &fLow;
    }
    const ZeeDArrayD* GetCoordHigh()  const {
        return &fHigh;
    }
    const ZeeDArrayI* GetNeighbours() const {
        return &fNeighbours;
    }
    Int_t GetMergedCellIdx() const {
        return fMergedCellIdx;
    }

    ZeeDArrayI* GetOrigin() const {
        return fOrigin;
    }

    void Print(Option_t* option = "") const;

private:

    /** Array of neighbours of this cell */
    ZeeDArrayI fNeighbours; //!

    /** Array of original indices */
    ZeeDArrayI* fOrigin; //!

    /** Low bound of cell */
    ZeeDArrayD fLow; //!

    /** High bound of cell */
    ZeeDArrayD fHigh; //!

    /** Number of dimentions */
    Int_t fNumDimentions; //!

    /** Value */
    Double_t fValue; //!

    /** Index of the merged cell */
    Int_t fMergedCellIdx; //!

    void CheckDimentions(const ZeeDGridCell* cell) const;

    /** Merges two 1D cells */
    void MergeWith1D(const ZeeDGridCell& rhs);

    /** Merges two 2D cells */
    void MergeWith2D(const ZeeDGridCell& rhs);
};

#endif
