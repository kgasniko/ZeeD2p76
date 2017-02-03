#include "ZeeDArrays/ZeeDTableCell.h"
#include <cstdlib>
#include <TSystem.h>

//------------------------------------------------------
ZeeDTableCell::ZeeDTableCell(TString name) : TObject()
{
    // Constructor
    fName   = name;
    fValue  = new ZeeDArrayD();
}

//------------------------------------------------------
ZeeDTableCell::~ZeeDTableCell()
{
    // Destructor

    if(fValue != NULL) {
        delete fValue;
    }
}

//------------------------------------------------------
void ZeeDTableCell::SetValue(Double_t value, Int_t index)
{
    // Sets value

    // Check boundaries
    if ( this->outOfBounds(index) ) {
        fValue->AddLast(value);
    } else {
        fValue->AddAt(value, index);
    }
}

//------------------------------------------------------
void ZeeDTableCell::AddLast(Double_t value)
{
    // Sets value
    fValue->AddLast(value);
}

//------------------------------------------------------
void ZeeDTableCell::InsertValue(Double_t value, Int_t index)
{
    // Inserts value  into array

    // Check boundaries
    if ( this->outOfBounds(index) ) {
        Error("ZeeDTableCell::InsertValue",  "Index out of bounds");
        gSystem->Exit(EXIT_FAILURE);
    }

    for(Int_t bin = fValue->GetEntriesFast() - 1; bin > index; --bin) {
        fValue[bin] = fValue[bin - 1];
    }

    fValue->AddAt(value, index);
}

//------------------------------------------------------
Double_t ZeeDTableCell::GetValue(Int_t index) const
{
    // Gets value

    // Check boundaries
    if ( this->outOfBounds(index) ) {
        Warning("GetValue",  "Index %i out of bounds", index);
        return 0;
    }

    return fValue->At(index);
}

//------------------------------------------------------
TString ZeeDTableCell::GetCellName() const
{
    // Gets name
    return fName;
}

//------------------------------------------------------
Bool_t ZeeDTableCell::outOfBounds(Int_t index) const
{
    // Validates the index
    return (index < 0 || index > (fValue->GetEntriesFast() - 1) );
}
