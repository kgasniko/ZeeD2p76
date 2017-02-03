#ifndef __ZEEDMERGEDCELL_H
#define __ZEEDMERGEDCELL_H

////////////////////////////////////////////////////////
// Name    : ZeeDMergedCell.h
////////////////////////////////////////////////////////


// Root includes
#include <TObject.h>
#include <TObjArray.h>

// ZeeDAnalysis includes
#include "ZeeDArrays/ZeeDArrayI.h"

class ZeeDBinGrid;

/** Class to define a cell merged from several ZeeDGridCells
 @author  Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
 @date 2008/11/03
*/
class ZeeDMergedCell : public TObject {

public:

    explicit ZeeDMergedCell(ZeeDBinGrid* grid);
    ZeeDMergedCell(const ZeeDMergedCell& cell);

    ZeeDMergedCell& operator= (const ZeeDMergedCell& rhs);

    ~ZeeDMergedCell();

    ZeeDMergedCell& operator+=(const ZeeDMergedCell& rhs);

    void AddCell(Int_t index);

    /** Sets merged cell index to all cells belonging to the merged one */
    void SetIndex(Int_t index);

    /** Returns index of the smallets neighbour */
    Int_t GetSmallestNeighbour() const;

    /** Returns index of the smallets neighbour */
    Int_t GetSmallestNeighbour(TObjArray* array) const;

    /** Returns value stored in merged cells */
    Double_t GetValue() const;

    const ZeeDArrayI* GetNeighbours() const {
        return &fNeighbours;
    }
    const ZeeDArrayI* GetCells() const {
        return &fArray;
    }

    void ReCalcualte();

    /** Removes neighbour from the list */
    void RemoveNeighbour(Int_t index);

    /** Cleans neighbours list */
    void CleanNeighbours();

    void Print(Option_t* option = "") const;

    /** Draws merged cell */
    void Draw(Option_t* option);

    /** Compare two objects. Returns 0 when equal, -1 when this is
        smaller and +1 when bigger (like strcmp()).
    */
    Int_t Compare(const TObject* cell) const;

private:

    ZeeDBinGrid* fGrid;       //! Bin grid
    ZeeDArrayI fArray;        //! Array of cell indices
    ZeeDArrayI fNeighbours;   //! Array of neigbours

    Int_t GetMergedCell( Int_t index, TObjArray* array) const;
};

#endif
