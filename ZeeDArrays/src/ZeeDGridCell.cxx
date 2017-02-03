#include "ZeeDArrays/ZeeDGridCell.h"

#include <iostream>
#include <iomanip>

#include <TMath.h>
#include <TSystem.h>

using namespace std;

//------------------------------------------------------
ZeeDGridCell::ZeeDGridCell(ZeeDArrayD low, ZeeDArrayD high)
{
    // Constructor
    fNumDimentions = 0;
    fMergedCellIdx = -1;

    // Check consistency
    if (low.GetEntriesFast() != high.GetEntriesFast() ) {
        Error("ZeeDGridCell::ZeeDGridCell",
              "Coordinates are not consistent");
    } else {
        fLow           = low;
        fHigh          = high;
        fNumDimentions = low.GetEntriesFast();
        fValue         = 0.0;
    }

    fOrigin = new ZeeDArrayI();
}

//------------------------------------------------------
ZeeDGridCell::ZeeDGridCell(const ZeeDGridCell& cell) : TObject(cell)
{
    // Copy constructor
}

//------------------------------------------------------
ZeeDGridCell& ZeeDGridCell::operator= (const ZeeDGridCell& rhs)
{
    // Assignment operator
    if (this == &rhs ) {
        return *this;
    }

    TObject::operator=(rhs);

    fLow           = rhs.fLow;
    fHigh          = rhs.fHigh;
    fNumDimentions = rhs.fNumDimentions;
    fValue         = rhs.fValue;
    fOrigin        = rhs.fOrigin;
    fMergedCellIdx = rhs.fMergedCellIdx;

    return *this;
}

//------------------------------------------------------
ZeeDGridCell::ZeeDGridCell(const Int_t size,
                           const Double_t* low, const Double_t* high)
{
    // Constructor
    fNumDimentions = size;

    fLow           = ZeeDArrayD(size, low);
    fHigh          = ZeeDArrayD(size, high);
    fOrigin        = new ZeeDArrayI();
    fMergedCellIdx = -1;
}

//------------------------------------------------------
ZeeDGridCell::ZeeDGridCell(const Int_t numDimentions, const Double_t* coord)
{
    // Constructor
    fNumDimentions = numDimentions;

    ZeeDArrayD low, high;

    Int_t index = 0;

    for (; index < fNumDimentions; ++index) {
        low.AddLast(coord[index]);
    }

    for (; index < 2 * fNumDimentions; ++index) {
        high.AddLast(coord[index]);
    }

    fLow           = low;
    fHigh          = high;
    fOrigin        = new ZeeDArrayI();
    fMergedCellIdx = -1;
}

//------------------------------------------------------
ZeeDGridCell::~ZeeDGridCell()
{
    // Destructor
    if (fOrigin != NULL) {
        delete fOrigin;
    }
}

//------------------------------------------------------
ZeeDGridCell& ZeeDGridCell::operator+=(const ZeeDGridCell& rhs)
{
    this->CheckDimentions(&rhs);

    ZeeDArrayI* rhsOrig = rhs.GetOrigin();

    for (Int_t index = 0; index < rhsOrig->GetEntriesFast(); ++index ) {
        fOrigin->AddLast( rhsOrig->At(index) );
    }

    if(fNumDimentions == 1) {
        MergeWith1D(rhs);
    } else if ( fNumDimentions == 2 ) {
        MergeWith2D(rhs);
    } else {
        Error("ZeeDGridCell:operator+=",
              "Operator += is not supported yet for D >=3");
    }

    return *this;
}

//------------------------------------------------------
void ZeeDGridCell::AddCell(ZeeDGridCell* cell)
{
    *this += *cell;
}

//------------------------------------------------------
void ZeeDGridCell::MergeWith1D(const ZeeDGridCell& rhs)
{
    if (this->GetCoordLow()->At(0) == rhs.GetCoordHigh()->At(0) ) {
        fLow = rhs.fLow;
    } else if (this->GetCoordHigh()->At(0) == rhs.GetCoordLow()->At(0) ) {
        fHigh = rhs.fHigh;
    } else {
        Error("ZeeDGridCell::MergeWith1D", "Cells are not adjacent");
    }

    fValue = rhs.fValue;

    return;
}

//------------------------------------------------------
void ZeeDGridCell::MergeWith2D(const ZeeDGridCell& rhs)
{
    ZeeDArrayD* xArray = new ZeeDArrayD();
    ZeeDArrayD* yArray = new ZeeDArrayD();

    xArray->AddLast(this->GetCoordLow()->At(0));
    xArray->AddLast(this->GetCoordHigh()->At(0));
    xArray->AddLast(rhs.GetCoordLow()->At(0));
    xArray->AddLast(rhs.GetCoordHigh()->At(0));

    yArray->AddLast(this->GetCoordLow()->At(1));
    yArray->AddLast(this->GetCoordHigh()->At(1));
    yArray->AddLast(rhs.GetCoordLow()->At(1));
    yArray->AddLast(rhs.GetCoordHigh()->At(1));

    Double_t x, y;
    ZeeDArrayD low, high;

    x = xArray->At( xArray->GetMinElement() );
    y = yArray->At( yArray->GetMinElement() );

    low.AddLast(x);
    low.AddLast(y);

    x = xArray->At( xArray->GetMaxElement() );
    y = yArray->At( yArray->GetMaxElement() );

    high.AddLast(x);
    high.AddLast(y);

    fLow  = low;
    fHigh = high;
    fValue += rhs.fValue;

    delete xArray;
    delete yArray;

    return;
}

//------------------------------------------------------
Bool_t ZeeDGridCell::isInside(const ZeeDArrayD* point) const
{
    Bool_t flag = kTRUE;

    if ( this->GetNumDimentions() != point->GetEntriesFast() ) {
        Error("ZeeDGridCell::isInside", "Point has wrong dimentions");
        flag = kFALSE;
    }

    for (Int_t coord = 0; coord < point->GetEntriesFast(); ++coord) {
        flag = flag
               && ( point->At(coord) >= fLow.At(coord)
                    &&   point->At(coord) < fHigh.At(coord) );
    }

    return flag;
}


//------------------------------------------------------
void ZeeDGridCell::CheckDimentions(const ZeeDGridCell* cell) const
{
    // Check that number of dimentions is the same for both cells
    if (this->GetNumDimentions() != cell->GetNumDimentions() ) {
        Error("ZeeDGridCell::CheckDimentions",
              "Attempt to sum cells with different dimentions");
        this->Print();
        cell->Print();
        gSystem->Exit(EXIT_FAILURE);
    }
}
//------------------------------------------------------
Bool_t ZeeDGridCell::isNeighbour(const ZeeDGridCell* cell) const
{
    Bool_t flag = kFALSE;

    this->CheckDimentions(cell);

    const ZeeDArrayD* low_1  = this->GetCoordLow();
    const ZeeDArrayD* high_1 = this->GetCoordHigh();

    const ZeeDArrayD* low_2  = cell->GetCoordLow();
    const ZeeDArrayD* high_2 = cell->GetCoordHigh();

    Int_t numMatches = 0;

    for (Int_t dim = 0; dim < this->GetNumDimentions(); ++dim ) {

        // Full overlay
        if ( low_1->At(dim) == low_2->At(dim) || low_1->At(dim) == high_2->At(dim) ) {
            ++numMatches;
        }

        if ( high_1->At(dim) == low_2->At(dim) || high_1->At(dim) == high_2->At(dim) ) {
            ++numMatches;
        }

        // Partial overlay
        if (  ( low_2->At(dim) < high_1->At(dim) &&
                high_2->At(dim) > high_1->At(dim) ) ||
                ( low_2->At(dim) < low_1->At(dim) &&
                  high_2->At(dim) > low_1->At(dim) ) ) {
            ++numMatches;
        }

        if (  ( low_1->At(dim) < high_2->At(dim) &&
                high_1->At(dim) > high_2->At(dim) ) ||
                ( low_1->At(dim) < low_2->At(dim) &&
                  high_1->At(dim) > low_2->At(dim) ) ) {
            ++numMatches;
        }
    }

    if ( numMatches == ( 2 * ( this->GetNumDimentions()) - 1 ) ) {
        flag = kTRUE;
    }

    return flag;
}

//------------------------------------------------------
void ZeeDGridCell::Print(Option_t* option) const
{
    // Blue print
    if ( !option || !option[0] ) {
        cout << option << endl;
    }

    Int_t numDimentions = this->GetNumDimentions();

    cout << numDimentions << "D cell: (";

    for (Int_t coord = 0; coord < numDimentions; ++coord) {
        cout << setw(7) << fLow.At(coord);

        if ( coord < numDimentions - 1 ) {
            cout << ", ";
        }
    }

    cout << "; ";

    for (Int_t coord = 0; coord < numDimentions; ++coord) {
        cout << setw(7) << fHigh.At(coord);

        if ( coord < numDimentions - 1 ) {
            cout << ", ";
        }
    }

    cout << ") = " << setw(7) << fValue;
    cout << "; neighbours: ";

    for (Int_t index = 0; index < fNeighbours.GetEntriesFast(); ++index) {
        cout << fNeighbours.At(index) << "  ";
    }

    cout << endl;
}
