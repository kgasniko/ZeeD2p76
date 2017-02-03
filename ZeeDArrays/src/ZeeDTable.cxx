#include "ZeeDArrays/ZeeDTable.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <cstdlib>

#include <TError.h>

#include "ZeeDArrays/ZeeDTableCell.h"

using namespace std;

//------------------------------------------------------
ZeeDTable::ZeeDTable(TString filename) : TObject()
{
    // Constructor
    fLength   = 0;
    fFileName = filename;

    fTable = new TObjArray();
}

//------------------------------------------------------
ZeeDTable::~ZeeDTable()
{
    // Destructor

    if (fTable != NULL) {
        for (Int_t icell = 0; icell < fTable->GetEntriesFast(); ++icell) {
            (fTable[icell]).Delete();
        }

        delete fTable;
    }
}

//------------------------------------------------------
void ZeeDTable::SetTableColumns(TString* columnNames, Int_t numOfColumns)
{
    // Sets column names via TString array

    for (Int_t column = 0; column < numOfColumns; ++column) {
        this->AddCell(columnNames[column]);
    }
}

//------------------------------------------------------
Int_t ZeeDTable::GetTableLength() const
{
    // Returns table length
    return fLength;
}

//------------------------------------------------------
void ZeeDTable::AddCell(TString name)
{
    // Adds cell to the table

    ZeeDTableCell* cell = new ZeeDTableCell(name);
    fTable->AddLast(cell);
}

//------------------------------------------------------
Bool_t ZeeDTable::ReadTable()
{
    // Read table from the text file

    ifstream InFile;
    InFile.open(fFileName.Data());

    // if file couldn't be opend
    if (!InFile) {
        Error("ZeeDTable::ReadTable", "Couldn't open file %s", fFileName.Data());
        return EXIT_FAILURE;
    } else {
        Info("ZeeDTable::ReadTable", "file is opened %s", fFileName.Data());
    }

    Int_t nColumn    = fTable->GetEntriesFast();

    if ( nColumn <= 0 )  {
        Error("ZeeDTable::ReadTable", "Table is not properly initialised");
        return EXIT_FAILURE;
    }

    Int_t rowCounter = 0;
    Int_t counter    = 0;

    do {
        Int_t column = counter % nColumn;
        Double_t value;

        // Readin value
        InFile >> value;

        if( !InFile.eof() ) {

            static_cast<ZeeDTableCell*>(fTable->At(column))->AddLast(value);

            if( counter > 0 && counter % nColumn == (nColumn - 1)) {
                ++rowCounter;
            }

            ++counter;
        }
    } while (!InFile.eof()); //

    fLength = rowCounter;

    InFile.close();

    return(EXIT_SUCCESS);
}

//------------------------------------------------------
Int_t ZeeDTable::GetTableIndex(TString name) const
{
    // Returns index of the variable name

    for(Int_t index = 0; index < fTable->GetEntriesFast(); ++index) {
        TString columnName =
            static_cast<ZeeDTableCell*>(fTable->At(index))->GetCellName();

        if(columnName.CompareTo(name) == 0) {
            return index;
        }
    }

    // Return negative value if name is not the table
    return -1;
}

//------------------------------------------------------
Double_t ZeeDTable::GetTableValue(TString name, Int_t bin) const
{
    // Retruns value in the bin
    Double_t value = 0;

    Int_t index = this->GetTableIndex(name);

    if ( index >= 0 ) {
        value = static_cast<ZeeDTableCell*>(fTable->At(index))->GetValue(bin);
    } else {
        Warning("GetTableValue", "No such column %s", name.Data());
    }

    return value;
}

//------------------------------------------------------
void ZeeDTable::Sort(TString columnName)
{
    // Sorts elements by the column = columnName
    // Explores Buble algorithm. Not the fastest one, but stable :)

    Bool_t swapped = kFALSE;

    do {
        swapped = kFALSE;

        for (Int_t bin = 0; bin < fLength - 1; ++bin) {
            Double_t value      = this->GetTableValue(columnName, bin);
            Double_t valueNext  = this->GetTableValue(columnName, bin + 1);

            if (valueNext == 0) {
                continue;
            }

            if ( value > valueNext ) {
                this->Swap(bin, bin + 1);
                swapped = kTRUE;
            }
        }

    } while (swapped);
}

//------------------------------------------------------
void ZeeDTable::Swap(Int_t binOne, Int_t binTwo)
{
    // Swaps content of two bins

    for (Int_t column = 0; column < fTable->GetEntriesFast(); ++column) {

        Double_t valueOne = static_cast<ZeeDTableCell*>(
                                fTable->At(column))->GetValue(binOne);

        Double_t valueTwo = static_cast<ZeeDTableCell*>(
                                fTable->At(column))->GetValue(binTwo);

        static_cast<ZeeDTableCell*>(
            fTable->At(column))->SetValue(valueTwo, binOne);

        static_cast<ZeeDTableCell*>(
            fTable->At(column))->SetValue(valueOne, binTwo);
    }
}

//------------------------------------------------------
Int_t ZeeDTable::GetBin(TString nameOne, TString nameTwo,
                        Double_t valueOne, Double_t valueTwo) const
{
    // Retruns bin number corresponding to value in array
    Int_t BinNumber = -1;

    for(Int_t bin = 0; bin < fLength; ++bin) {

        Double_t variableOne = this->GetTableValue(nameOne, bin);
        Double_t variableTwo = this->GetTableValue(nameTwo, bin);

        if ( TMath::Abs(valueOne - variableOne) < 0.000001
                && TMath::Abs(valueTwo - variableTwo) < 0.000001 ) {
            return bin;
        }
    }

    return BinNumber;
}
