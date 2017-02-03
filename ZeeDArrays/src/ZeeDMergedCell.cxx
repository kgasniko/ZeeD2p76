#include "ZeeDArrays/ZeeDMergedCell.h"

// Std includes
#include <iostream>
#include <iomanip>

// Root includes
#include <TLatex.h>
#include <TBox.h>
#include <TMath.h>

// ZeeDAnalysis includes
#include "ZeeDArrays/ZeeDBinGrid.h"
#include "ZeeDArrays/ZeeDGridCell.h"

using std::cout;
using std::endl;
using std::setw;

//------------------------------------------------------
ZeeDMergedCell::ZeeDMergedCell(ZeeDBinGrid* grid)
{
    // Constructor
    fGrid = grid;
}

//------------------------------------------------------
ZeeDMergedCell::ZeeDMergedCell(const ZeeDMergedCell& cell) : TObject(cell)
{
    // Copy constructor
}

//------------------------------------------------------
ZeeDMergedCell& ZeeDMergedCell::operator= (const ZeeDMergedCell& rhs)
{
    // Assignment operator
    if (this == &rhs ) {
        return *this;
    }

    TObject::operator=(rhs);

    fGrid       = rhs.fGrid;
    fArray      = rhs.fArray;
    fNeighbours = rhs.fNeighbours;

    return *this;
}

//------------------------------------------------------
ZeeDMergedCell::~ZeeDMergedCell()
{
    // Destructor
}

//------------------------------------------------------
void ZeeDMergedCell::AddCell(Int_t index)
{
    // Adds cell
    fArray.AddLast(index);

    // Add cell neighbours to this one
    ZeeDGridCell* cell = fGrid->GetCell(index);

    if (cell != NULL ) {
        const ZeeDArrayI* newNeighbours = cell->GetNeighbours();

        for (Int_t j = 0; j < newNeighbours->GetEntriesFast(); ++j) {
            fNeighbours.AddLast( newNeighbours->At(j) );
        }
    }

    this->CleanNeighbours();
}

//------------------------------------------------------
ZeeDMergedCell& ZeeDMergedCell::operator+=(const ZeeDMergedCell& rhs)
{
    // Add another cell to  this one

    fArray      += rhs.fArray;
    fNeighbours += rhs.fNeighbours;

    this->CleanNeighbours();

    return *this;
}

//------------------------------------------------------
void ZeeDMergedCell::CleanNeighbours()
{
    // Remove merged cells indices from list of neighbours
    for (Int_t i = 0; i < fArray.GetEntriesFast(); ++i) {
        fNeighbours.Remove( fArray.At(i) );
    }

    // Remove dublicated entries
    fNeighbours.Sort();
    fNeighbours.Unique();
    fNeighbours.Compress();
}

//------------------------------------------------------
Double_t ZeeDMergedCell::GetValue() const
{
    Double_t value = 0;

    for (Int_t i = 0; i < fArray.GetEntriesFast(); ++i) {

        Int_t index = fArray.At(i);
        ZeeDGridCell* cell = fGrid->GetCell(index);

        if (cell != NULL ) {
            value += cell->GetValue();
        }
    }

    return value;
}

//------------------------------------------------------
void ZeeDMergedCell::ReCalcualte()
{
    // Puts sum of all
    Double_t sum = this->GetValue();

    for (Int_t i = 0; i < fArray.GetEntriesFast(); ++i) {

        Int_t index = fArray.At(i);
        ZeeDGridCell* cell = fGrid->GetCell(index);

        if (cell != NULL ) {
            cell->SetValue(sum);
        }
    }
}

//------------------------------------------------------
Int_t ZeeDMergedCell::GetSmallestNeighbour() const
{
    Int_t index = 0;

    for (Int_t i = 0; i < fNeighbours.GetEntriesFast(); ++i) {
        ZeeDGridCell* cell     = fGrid->GetCell( fNeighbours.At(i) );
        ZeeDGridCell* cell_min = fGrid->GetCell( fNeighbours.At(index) );

        if (cell == NULL || cell_min == NULL) {
            Error("ZeeDMergedCell::GetSmallestNeighbour",
                  "Not valid cell index: %i, %i", i, index);
            continue;
        }

        if (cell->GetValue() < cell_min->GetValue() ) {
            index = i;
        }
    }

    return fNeighbours.At(index);
}

//------------------------------------------------------
Int_t ZeeDMergedCell::GetSmallestNeighbour(TObjArray* array) const
{
    Int_t index = 0;

    for (Int_t i = 0; i < fNeighbours.GetEntriesFast(); ++i) {

        Int_t merged_cell_idx     = this->GetMergedCell(i, array);
        Int_t merged_min_cell_idx = this->GetMergedCell(index, array);

        if (merged_cell_idx < 0 || merged_min_cell_idx < 0 ) {
            Error("ZeeDMergedCell::GetSmallestNeighbour", "Not valid indices");
            continue;
        }

        ZeeDMergedCell* merged_cell = static_cast<ZeeDMergedCell*>
                                      ( array->At(merged_cell_idx) );

        ZeeDMergedCell* merged_min_cell = static_cast<ZeeDMergedCell*>
                                          ( array->At(merged_min_cell_idx) );

        if ( merged_cell->GetValue() < merged_min_cell->GetValue() ) {
            index = i;
        }
    }

    return this->GetMergedCell(index, array);
}

//------------------------------------------------------
Int_t ZeeDMergedCell::GetMergedCell( Int_t index, TObjArray* array) const
{
    // Getmerged
    Int_t merged_cell_idx = -1;

    ZeeDGridCell* cell = fGrid->GetCell( fNeighbours.At(index) );

    if (cell == NULL ) {
        Error("ZeeDMergedCell::GetMergedCell", "Not valid cell index: %i", index);
        return merged_cell_idx;
    }

    merged_cell_idx = cell->GetMergedCellIdx();

    if (merged_cell_idx < 0 || merged_cell_idx > array->GetEntriesFast() ) {
        merged_cell_idx = -1;
    }

    return merged_cell_idx;
}

//------------------------------------------------------
void ZeeDMergedCell::RemoveNeighbour(Int_t index)
{
    fNeighbours.Remove(index);
}

//------------------------------------------------------
void ZeeDMergedCell::SetIndex(Int_t index)
{
    for (Int_t i = 0; i < fArray.GetEntriesFast(); ++i) {

        ZeeDGridCell* cell = fGrid->GetCell( fArray.At(i) );
        cell->SetMergedCellIdx(index);
    }
}

//------------------------------------------------------
void ZeeDMergedCell::Print(Option_t* option) const
{
    // Blue print
    if ( !option || !option[0] ) {
        cout << option << endl;
    }

    cout << "value = " << this->GetValue() << endl;

    for (Int_t i = 0; i < fArray.GetEntriesFast(); ++i) {
        Int_t index = fArray.At(i);
        ZeeDGridCell* cell = fGrid->GetCell(index);
        cout << setw(5) << index  << " -> ";
        cell->Print();
    }

}

//------------------------------------------------------
void ZeeDMergedCell::Draw(Option_t* option)
{
    char value[50];

    TBox* box = new TBox();

    static Int_t colorIdx = 0;
    const Int_t numOfColors = 42;

    Int_t colorArray[numOfColors] = {
        kGreen - 1  , kYellow - 1  , kRed - 1  , kMagenta - 1  , kBlue - 1  , kCyan - 1  ,
        kGreen - 2  , kYellow - 2  , kRed - 2  , kMagenta - 2  , kBlue - 2  , kCyan - 2  ,
        kGreen - 3  , kYellow - 3  , kRed - 3  , kMagenta - 3  , kBlue - 3  , kCyan - 3  ,
        kGreen - 6  , kYellow - 6  , kRed - 6  , kMagenta - 6  , kBlue - 6  , kCyan - 6  ,
        kGreen - 8  , kYellow - 8  , kRed - 8  , kMagenta - 8  , kBlue - 8  , kCyan - 8  ,
        kGreen - 10 , kYellow - 10 , kRed - 10 , kMagenta - 10 , kBlue - 10 , kCyan - 10 ,
        kGreen + 1  , kYellow + 1  , kRed + 1  , kMagenta + 1  , kBlue + 1  , kCyan + 1
    };

    if (colorIdx >= numOfColors ) {
        colorIdx = 0;
    }

    box->SetFillColor( colorArray[++colorIdx] );
    box->SetLineColor(1);

    TLatex* text = new TLatex();
    text->SetTextSize(0.025);
    text->SetTextFont(42);
    text->SetTextAlign(22); // centered

    for (Int_t i = 0; i < fArray.GetEntriesFast(); ++i) {
        ZeeDGridCell* cell = fGrid->GetCell( fArray.At(i) );

        if (cell == NULL) {
            continue;
        }

        Double_t x1 = cell->GetCoordLow()->At(0);
        Double_t y1 = cell->GetCoordLow()->At(1);

        Double_t x2 = cell->GetCoordHigh()->At(0);
        Double_t y2 = cell->GetCoordHigh()->At(1);

        box->DrawBox(x1, y1, x2, y2);

        TString str = TString(option);

        if ( str.Contains("index") ) {
            Int_t index = fArray.At(i);
            sprintf(value, "%i", index);
        } else {
            sprintf(value, "%.f", cell->GetValue());
        }

        text->DrawLatex( (x1 + x2) / 2, (y1 + y2) / 2, value);
    }

    delete box;
    delete text;
}

//------------------------------------------------------
Int_t ZeeDMergedCell::Compare(const TObject* obj) const
{
    if ( obj->InheritsFrom( ZeeDMergedCell::Class() ) ) {

        const ZeeDMergedCell* cell = static_cast<const ZeeDMergedCell*>(obj);

        Double_t a = this->GetValue();
        Double_t b = cell->GetValue();

        if (a == b ) {
            return 0;
        }

        if (a > b ) {
            return 1;
        }

        if (a < b ) {
            return -1;
        }
    }

    return 0;
}
