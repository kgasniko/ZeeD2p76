#include "ZeeDArrays/ZeeDBinGrid.h"

// STD includes
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <float.h>

#include <sstream>

// Root includes
#include <TMath.h>
#include <TBox.h>
#include <TH1.h>
#include <TH2F.h>
#include <TLatex.h>
#include <TString.h>
#include <TSystem.h>

// ZeeD analysis includes
#include "ZeeDArrays/ZeeDTable.h"
#include "ZeeDArrays/ZeeDArrayI.h"

using namespace std;

//------------------------------------------------------
ZeeDBinGrid::ZeeDBinGrid()
{
    // Constructor
    fArray       = new TObjArray();
    fMergedCells = new TObjArray();
    fBinEdges    = new TObjArray();
    fMinLow      = new ZeeDArrayD();
    fMaxHigh     = new ZeeDArrayD();
    fRandom      = new TRandom();

    fName        = "";
    fTitleX      = "X";
    fTitleY      = "Y";
    fDefValue    = 0;

    bIsRegular   = kFALSE;
}

//------------------------------------------------------
ZeeDBinGrid::ZeeDBinGrid(TString name)
{
    // Named constructor
    fArray       = new TObjArray();
    fMergedCells = new TObjArray();
    fBinEdges    = new TObjArray();
    fMinLow      = new ZeeDArrayD();
    fMaxHigh     = new ZeeDArrayD();
    fRandom      = new TRandom();

    fName        = name;
    fTitleX      = "X";
    fTitleY      = "Y";
    fDefValue    = 0;

    bIsRegular   = kFALSE;
}

/*
//------------------------------------------------------
ZeeDBinGrid::ZeeDBinGrid(const ZeeDBinGrid& array) : TObject(array)
{
    // Copy constructor
}
*/

//------------------------------------------------------
ZeeDBinGrid& ZeeDBinGrid::operator= (const ZeeDBinGrid& rhs)
{
    // Assignment operator
    if (this != &rhs ) {
        TObject::operator=(rhs);
    }

    return *this;
}

//------------------------------------------------------
ZeeDBinGrid& ZeeDBinGrid::operator/ (ZeeDBinGrid& rhs)
{

    ZeeDBinGrid* Result = NULL; 
    Result = &rhs;

    for(Int_t idx = 0; idx < rhs.GetNumOfCells(); ++idx ) {

        ZeeDGridCell* cellTop = this->GetCell(idx);
        ZeeDGridCell* cellBot = rhs.GetCell(idx);

        if(cellBot->GetValue() != 0.0) { // protect nans
            Result->Set(idx, (cellTop->GetValue() / cellBot->GetValue()) * 100);
        } else {
            Result->Set(idx, 100.0);
        }
    }

    return *Result;
}

//------------------------------------------------------
ZeeDBinGrid& ZeeDBinGrid::operator+ (ZeeDBinGrid& rhs)
{

    ZeeDBinGrid* Result = NULL;
    Result = &rhs;

    for(Int_t idx = 0; idx < rhs.GetNumOfCells(); ++idx ) {

        ZeeDGridCell* cellTop = this->GetCell(idx);
        ZeeDGridCell* cellBot = rhs.GetCell(idx);

        Result->Set(idx, (cellTop->GetValue() + cellBot->GetValue()));
    }

    return *Result;
}


//------------------------------------------------------
double ZeeDBinGrid::GetSize(Int_t index) const
{
    ZeeDGridCell* cell = this->GetCell(index);
    double Size;

    if (cell == NULL) {
        return 0;
    }

    const ZeeDArrayD* low  = cell->GetCoordLow();
    const ZeeDArrayD* high = cell->GetCoordHigh();

    for (Int_t j = 0; j < low->GetEntriesFast(); ++j) {
        // SHOULD BE DEBUG
        //        std::cout << low->At(j) << "\n";
    }

    for (Int_t k = 0; k < high->GetEntriesFast(); ++k) {
        // SHOULD BE DEBUG
        //    std::cout << high->At(k) << "\n";
    }

    Size =  ( (high->At(1)) - (low->At(1)) ) * ( (high->At(0)) - (low->At(0)) );

    return Size;
}

//------------------------------------------------------
double ZeeDBinGrid::GetEntriesFast() const
{
    double Entry = 0;

    for (Int_t index = 0; index < fArray->GetEntriesFast(); ++index) {

        ZeeDGridCell* cell = this->GetCell(index);

        if (cell == NULL) {
            break;
        }

        Entry = Entry + (cell->GetValue());
    }

    return Entry;
}

//------------------------------------------------------
double ZeeDBinGrid::GetValue(Int_t index) const
{

    double Value = fDefValue;

    ZeeDGridCell* cell = this->GetCell(index);

    if (cell == NULL) {
        return Value;
    }

    Value = cell->GetValue();

    return Value;
}

//------------------------------------------------------
double ZeeDBinGrid::GetValue(double par) const
{

    if ( this->GetBinEdges()->GetEntriesFast() != 1 ) {
        Info("ZeeDBinGrid::GetValue", "Wrong array dimension. Dimension != 1");
	return fDefValue;
    }

    ZeeDArrayD coordinates(1);
    coordinates[0] = par;

    int index    = this->GetIndex(&coordinates);
    return GetValue(index);
}

//------------------------------------------------------
double ZeeDBinGrid::GetValue(double par1, double par2) const
{

    if ( this->GetBinEdges()->GetEntriesFast() != 2 ) {
        Info("ZeeDBinGrid::GetValue", "Wrong array dimension. Dimension != 2");
	return fDefValue;
    }

    ZeeDArrayD coordinates(2);
    coordinates[0] = par1;
    coordinates[1] = par2;

    int index    = this->GetIndex(&coordinates);
    return GetValue(index);
}

//------------------------------------------------------
ZeeDBinGrid::~ZeeDBinGrid()
{
    // Destructor
    if (fArray       != NULL) {
        delete fArray;
    }

    if (fMergedCells != NULL) {
        delete fMergedCells;
    }

    if (fBinEdges    != NULL) {
        delete fBinEdges;
    }

    if (fMinLow      != NULL) {
        delete fMinLow;
    }

    if (fMaxHigh     != NULL) {
        delete fMaxHigh;
    }

    if (fRandom      != NULL) {
        delete fRandom;
    }
}

//------------------------------------------------------
void ZeeDBinGrid::Reset(Double_t value)
{
    for (Int_t index = 0; index < fArray->GetEntriesFast(); ++index) {
        ZeeDGridCell* cell = this->GetCell( index );
        cell -> SetValue(value);

        ZeeDArrayI* origin = cell->GetOrigin();
        origin->Clear();
        origin->AddLast(index);
        origin->Compress();
    }
}

//------------------------------------------------------
void ZeeDBinGrid::Set(Int_t index, Double_t value)
{
    ZeeDGridCell* cell = this->GetCell( index );
    cell->SetValue(value);
}

//------------------------------------------------------
void ZeeDBinGrid::Add(Int_t index, Double_t value)
{
    ZeeDGridCell* cell = this->GetCell( index );
    cell->AddValue(value);
}

//------------------------------------------------------
void ZeeDBinGrid::Fill(Double_t value, Double_t xCoord)
{
    ZeeDArrayD* coord =  new ZeeDArrayD();
    coord->Add(xCoord);

    this->Fill(value, coord);
    delete coord;
}

//------------------------------------------------------
void ZeeDBinGrid::Fill(Double_t value, Double_t x, Double_t y)
{
    if (bIsRegular) {

        Double_t xMin = fMinLow->At(0);
        Double_t yMin = fMinLow->At(1);
        Double_t xMax = fMaxHigh->At(0);
        Double_t yMax = fMaxHigh->At(1);

        Bool_t outside = ( x < xMin || x >= xMax || y < yMin || y >= yMax);

        if (outside ) {
            return;
        }

        ZeeDArrayD* array_x = static_cast<ZeeDArrayD*>(fBinEdges->At(0));
        ZeeDArrayD* array_y = static_cast<ZeeDArrayD*>(fBinEdges->At(1));

        Int_t binx = this->FindBin(x, array_x);
        Int_t biny = this->FindBin(y, array_y);
        Int_t bin  = biny * ( array_x->GetEntriesFast() - 1) + binx;

        ZeeDGridCell* cell = this->GetCell( bin );

        if (cell != NULL) {
            cell -> AddValue(value);
        }

    } else {

        ZeeDArrayD* coord =  new ZeeDArrayD();
        coord->Add(x);
        coord->Add(y);

        this->Fill(value, coord);
        delete coord;
    }
}

//------------------------------------------------------
void ZeeDBinGrid::Fill(Double_t value,
                       Double_t xCoord, Double_t yCoord, Double_t zCoord)
{
    ZeeDArrayD* coord =  new ZeeDArrayD();
    coord->Add(xCoord);
    coord->Add(yCoord);
    coord->Add(zCoord);

    this->Fill(value, coord);
    delete coord;
}

//------------------------------------------------------
void ZeeDBinGrid::Fill(Double_t value, ZeeDArrayD* coord)
{
    ZeeDGridCell* cell = this->GetCell(coord);

    if ( cell != NULL ) {
        cell->AddValue(value);
    }
}

//------------------------------------------------------
Int_t ZeeDBinGrid::FindBin(Double_t x, ZeeDArrayD* array)
{
    return TMath::BinarySearch(array->GetEntriesFast(), array->GetArray(), x);
}

//------------------------------------------------------
void ZeeDBinGrid::AddCell(TObject* cell)
{
    fArray->AddLast( cell );
}

//------------------------------------------------------
ZeeDGridCell* ZeeDBinGrid::GetCell(Int_t index) const
{
    ZeeDGridCell* cell = NULL;

    if ( !this->OutOfBounds(index, fArray) ) {
        cell = static_cast<ZeeDGridCell*>( fArray->At(index) );
    } else {
        //        Error("ZeeDBinGrid::GetCell", "Invalid index %d", index);
    }

    return cell;
}

//------------------------------------------------------
ZeeDMergedCell* ZeeDBinGrid::GetMergedCell(Int_t index) const
{
    ZeeDMergedCell* cell = NULL;

    if ( !this->OutOfBounds(index, fMergedCells) ) {
        cell = static_cast<ZeeDMergedCell*>( fMergedCells->At(index) );
    } else {
        Error("ZeeDBinGrid::GetMergedCell", "Invalid index %d", index);
    }

    return cell;
}

//------------------------------------------------------
Int_t ZeeDBinGrid::GetIndex(const ZeeDArrayD* coord) const
{
    for (Int_t index = 0; index < fArray->GetEntriesFast(); ++index) {
        ZeeDGridCell* cell = this->GetCell( index );

        if (cell != NULL && cell->isInside( coord ) ) {
            return index;
        }
    }

    return -1;
}

//------------------------------------------------------
ZeeDGridCell* ZeeDBinGrid::GetCell(ZeeDArrayD* coord) const
{
    Int_t index = this->GetIndex(coord);
    return this->GetCell(index);
}

//------------------------------------------------------
void ZeeDBinGrid::Merge(Double_t minN)
{
    // Sort merged cells by their value
    this->SortMergedCells( fMergedCells );

    Bool_t needsMerging = kTRUE;

    while ( needsMerging ) {

        ZeeDMergedCell* first_cell = this->GetMergedCell(0);

        if ( first_cell->GetValue() >= minN || fMergedCells->GetEntriesFast() < 2 ) {
            needsMerging = kFALSE;
        }

        Int_t index = 0;

        while ( index < fMergedCells->GetEntriesFast() ) {

            ZeeDMergedCell* current_cell = this->GetMergedCell(index);

            if ( current_cell->GetValue() < minN ) {

                Int_t smallest_cell_idx = current_cell->GetSmallestNeighbour(fMergedCells);

                // Merge current cell to the smallest neighbour
                this->MergeCells(index, smallest_cell_idx);
            }

            ++index;
        }

        // Sort merged cells
        this->SortMergedCells( fMergedCells );
    }
}

//------------------------------------------------------
void ZeeDBinGrid::MergeCells(Int_t one, Int_t two)
{
    ZeeDMergedCell* cell_one = this->GetMergedCell(one);
    ZeeDMergedCell* cell_two = this->GetMergedCell(two);

    if (cell_one == NULL || cell_two == NULL ) {
        Error("ZeeDBinGrid::MergeCells", "Invalid cell index");
        return;
    }

    // Merge cell_two into cell_one
    *cell_one += *cell_two;

    // Remove cell_two from array
    delete cell_two;
    fMergedCells->RemoveAt(two);
    fMergedCells->Compress();

    for (Int_t j = 0; j < fMergedCells->GetEntriesFast(); ++j ) {
        ZeeDMergedCell* cell =
            static_cast<ZeeDMergedCell*>( fMergedCells->At(j) );
        cell->SetIndex(j);
    }
}

//------------------------------------------------------
void ZeeDBinGrid::SortMergedCells( TObjArray* array )
{
    this->QSort(array, 0, array->GetEntriesFast());

    // Set new indicies
    for (Int_t i = 0; i < fMergedCells->GetEntriesFast(); ++i) {
        ZeeDMergedCell* cell =
            static_cast<ZeeDMergedCell*>( fMergedCells->At(i) );
        cell->SetIndex( i );
    }
}

//------------------------------------------------------
void ZeeDBinGrid::QSort(TObjArray* a, Int_t first, Int_t last)
{
    static TObject* tmp;
    static Int_t i;           // "static" to save stack space
    Int_t j;

    while (last - first > 1) {
        i = first;
        j = last;

        for (;;) {
            while (++i < last && ObjCompare(a->At(i), a->At(first)) < 0) {
                ;
            }

            while (--j > first && ObjCompare(a->At(j), a->At(first)) > 0) {
                ;
            }

            if (i >= j) {
                break;
            }

            tmp = a->At(i);
            a->AddAt(a->At(j), i);
            a->AddAt(tmp, j);
        }

        if (j == first) {
            ++first;
            continue;
        }

        tmp = a->At(first);
        a->AddAt(a->At(j), first);
        a->AddAt(tmp, j);

        if (j - first < last - (j + 1)) {
            QSort(a, first, j);
            first = j + 1;   // QSort(j + 1, last);
        } else {
            QSort(a, j + 1, last);
            last = j;        // QSort(first, j);
        }
    }
}

//------------------------------------------------------
Int_t ZeeDBinGrid::ObjCompare(TObject* a, TObject* b)
{
    if (a == 0 && b == 0) {
        return 0;
    }

    if (a == 0) {
        return 1;
    }

    if (b == 0) {
        return -1;
    }

    return a->Compare(b);
}

//------------------------------------------------------
void ZeeDBinGrid::AddBinEdges(ZeeDArrayD* array)
{
    array->Sort();
    fBinEdges->Add(array);
}

//------------------------------------------------------
void ZeeDBinGrid::CreateGrid()
{
    Int_t numBinEdges = fBinEdges->GetEntriesFast();

    if (numBinEdges == 1) {
        ZeeDArrayD* array_x = static_cast <ZeeDArrayD*>(fBinEdges->At(0));

        for (Int_t x = 0; x < array_x->GetEntriesFast() - 1; ++x) {
            ZeeDArrayD low;
            ZeeDArrayD high;

            low.Add( array_x->At(x) );
            high.Add( array_x->At(x + 1) );

            fArray->Add( new ZeeDGridCell(low, high) );
        }

    } else {

      // if you faced nD, n > 1

      // prepare the array of the edge-arrays
      vector<ZeeDArrayD*> array_d;
      ZeeDArrayI dims(numBinEdges+1);
      dims.AddAt(1, 0);
      for (Int_t dim = 0; dim < numBinEdges; ++dim) {
	array_d.push_back(static_cast <ZeeDArrayD*> (fBinEdges->At(dim)));
	Int_t divisor = (dims.At(dim))*((static_cast <ZeeDArrayD*>(fBinEdges->At(dim)))->GetEntriesFast() - 1);
	dims.AddAt(divisor, dim+1);
      }

      // fill the array of cells
      for (Int_t bin = 0; bin < dims.At(numBinEdges); ++bin) {	// loop over the bins has to be added
	// decode the bin coordinates from the bin number
	// and create the bin-edges arrays
	ZeeDArrayD low(numBinEdges);
	ZeeDArrayD high(numBinEdges);
	Int_t rest = bin;
	for (Int_t edge = numBinEdges-1; edge > -1; --edge) {	// reverse loop over the edges
	  Int_t divisor = dims.At(edge);
	  low.AddAt((array_d.at(edge)) ->At(rest/divisor    ), edge);
	  high.AddAt((array_d.at(edge))->At(rest/divisor + 1), edge);
	  rest = rest%divisor;
	}

	// create the bin
	fArray->Add(new ZeeDGridCell(low, high));
      }
    }

    bIsRegular = kTRUE;

    this->Reset();
    this->AnalyseGrid();

    // Create array of merged cells
    for (Int_t i = 0; i < fArray->GetEntriesFast(); ++i) {
        ZeeDMergedCell* cell = new ZeeDMergedCell( this );
        cell->AddCell( i );
        cell->SetIndex( i );
        fMergedCells->AddLast(cell);
    }
}

//------------------------------------------------------
void ZeeDBinGrid::CreateGrid2DFromTable(TString tableName)
{
    ZeeDTable* table = new ZeeDTable(tableName);
    table->AddCell("lowX");
    table->AddCell("lowY");
    table->AddCell("highX");
    table->AddCell("highY");
    table->ReadTable();

    Double_t lowX, lowY, highX, highY;

    for (Int_t bin = 0; bin < table->GetTableLength(); ++bin ) {
        lowX  = table->GetTableValue("lowX",  bin);
        lowY  = table->GetTableValue("lowY",  bin);
        highX = table->GetTableValue("highX", bin);
        highY = table->GetTableValue("highY", bin);

        ZeeDArrayD low;
        ZeeDArrayD high;

        low.Add(lowX);
        low.Add(lowY);

        high.Add(highX);
        high.Add(highY);

        fArray->Add( new ZeeDGridCell(low, high) );
    }

    delete table;

    this->Reset();
    this->AnalyseGrid();
}

//------------------------------------------------------
void ZeeDBinGrid::AnalyseRegGrid()
{
    ZeeDGridCell* cellFirst = this->GetCell(0);
    ZeeDGridCell* cellLast  = this->GetCell(fArray->GetEntriesFast() - 1 );

    if (cellFirst == NULL || cellLast == NULL) {
        return;
    }

    const ZeeDArrayD* low  = cellFirst->GetCoordLow();
    const ZeeDArrayD* high = cellLast->GetCoordHigh();

    for (Int_t dim = 0; dim < cellFirst->GetNumDimentions(); ++dim ) {
        fMinLow->AddLast( low->At(dim) );
        fMaxHigh->AddLast( high->At(dim) );
    }

    if ( cellFirst->GetNumDimentions() == 1 ) {  // 1D neighbours
        ZeeDArrayD* array_x = static_cast<ZeeDArrayD*>(fBinEdges->At(0));
        Int_t max_x = array_x ->GetEntriesFast() - 1;

        for (Int_t index = 0; index < fArray->GetEntriesFast(); ++index) {
            Int_t x = index;
            ZeeDArrayI neighbours;

            if ( x > 1         ) {
                neighbours.AddLast(x - 1);
            }

            if ( x < max_x - 1 ) {
                neighbours.AddLast(x + 1);
            }

            ZeeDGridCell* cell = this->GetCell(index);
            cell->SetNeighbours(neighbours);
        }

    } else if ( cellFirst->GetNumDimentions() == 2 ) { // 2D neighbours

        ZeeDArrayD* array_x = static_cast<ZeeDArrayD*>(fBinEdges->At(0));
        ZeeDArrayD* array_y = static_cast<ZeeDArrayD*>(fBinEdges->At(1));
        Int_t max_x = array_x ->GetEntriesFast() - 1;
        Int_t max_y = array_y ->GetEntriesFast() - 1;

        for (Int_t index = 0; index < fArray->GetEntriesFast(); ++index) {
            Int_t x = index % max_x;
            Int_t y = index / max_x;

            ZeeDArrayI neighbours;

            if ( x > 0 ) {
                neighbours.AddLast( this->CalcIndex(x - 1, y, max_x) );
            }

            if ( x < max_x - 1 ) {
                neighbours.AddLast( this->CalcIndex(x + 1, y, max_x) );
            }

            if ( y > 0 ) {
                neighbours.AddLast( this->CalcIndex(x, y - 1, max_x) );
            }

            if ( y < max_y - 1 ) {
                neighbours.AddLast( this->CalcIndex(x, y + 1, max_x) );
            }

            ZeeDGridCell* cell = this->GetCell(index);
            cell->SetNeighbours(neighbours);
        }

    } else {
        Warning("ZeeDBinGrid::AnalyseRegGrid",
                "Unsupported number of dimentions");
    }
}

//------------------------------------------------------
Int_t ZeeDBinGrid::CalcIndex(Int_t x, Int_t y, Int_t max_x) const
{
    return (y * max_x + x);
}

//------------------------------------------------------
void ZeeDBinGrid::AnalyseGrid()
{
    // Analyses grid
    if ( bIsRegular && (fBinEdges->GetEntriesFast() < 3)) {
        this->AnalyseRegGrid();
        return;
    }

    // Looks for neighbours of each cell in the array
    for (Int_t i = 0; i < fArray->GetEntriesFast(); ++i) {

        ZeeDGridCell* cell = this->GetCell(i);

        if (cell == NULL) {
            continue;
        }

        ZeeDArrayI neighbours;

        // Look for neighbours
        for (Int_t j = 0; j < fArray->GetEntriesFast(); ++j) {
            ZeeDGridCell* cell_two = this->GetCell(j);

            if (cell_two == NULL) {
                continue;
            }

            if (cell->isNeighbour(cell_two) ) {
                neighbours.AddLast(j);
            }
        }

        if (neighbours.GetEntriesFast() > 0 ) {
            cell->SetNeighbours(neighbours);
        }

        // Find edges of the bin grid
        const ZeeDArrayD* low  = cell->GetCoordLow();
        const ZeeDArrayD* high = cell->GetCoordHigh();

        for (Int_t dim = 0; dim < this->GetCell(0)->GetNumDimentions(); ++dim ) {

            if (i == 0 ) {
                fMinLow->AddLast( low->At(dim) );
                fMaxHigh->AddLast( high->At(dim) );
            } else {

                if (fMinLow->At(dim) > low->At(dim) ) {
                    fMinLow->AddAt(low->At(dim), dim );
                }

                if (fMaxHigh->At(dim) < high->At(dim) ) {
                    fMaxHigh->AddAt(high->At(dim), dim );
                }
            }
        }
    }
}

//------------------------------------------------------
void ZeeDBinGrid::Draw(Option_t* option)
{
    if (fArray->GetEntriesFast() <= 0) {
        return;
    }

    if ( this->GetCell(0)  == NULL || this->GetCell(0)->GetNumDimentions() != 2) {
        Warning("ZeeDBinGrid::Draw",
                "Only 2D drawing is supported");
        return;
    }

    if (fMinLow->GetEntriesFast() <= 0 || fMaxHigh->GetEntriesFast() <= 0 ) {
        Error("ZeeDBinGrid::Draw",
              "Run function ZeeDBinGrid::AnalyseGrid() first");
        return;
    }

    Double_t offset = 0.0;
    Double_t xMin = fMinLow->At(0) - offset;
    Double_t yMin = fMinLow->At(1) - offset;

    Double_t xMax = fMaxHigh->At(0) + offset;
    Double_t yMax = fMaxHigh->At(1) + offset;

    const Int_t nBinsX = 1000;
    const Int_t nBinsY = 1000;
    char value[50];
    TH1* empty;

    if ( !option || !option[0] ) {
        sprintf(value, "%f", fRandom->Rndm());
        empty = new TH2F(value, value, nBinsX, xMin, xMax, nBinsY, yMin, yMax);
    } else {
        empty = new TH2F(option, option, nBinsX, xMin, xMax, nBinsY, yMin, yMax);
    }

    empty->SetStats(0);

    empty->GetXaxis()->SetTitle(fTitleX);
    empty->GetXaxis()->SetNdivisions(505);
    empty->GetXaxis()->SetLabelSize(0.038);
    empty->GetXaxis()->SetTitleOffset(1.3);

    empty->GetYaxis()->SetTitle(fTitleY);
    empty->GetYaxis()->SetNdivisions(505);
    empty->GetYaxis()->SetLabelSize(0.038);
    empty->GetYaxis()->SetTitleOffset(1.3);

    empty->DrawClone("");

    TLatex* text = new TLatex();
    text->SetTextSize(0.025);
    text->SetTextFont(42);
    text->SetTextAlign(22); // centered

    TBox* box = new TBox();
    box->SetLineColor(1);
    box->SetLineWidth(1);
    box->SetFillStyle(0);

    if (fMergedCells->GetEntriesFast() != fArray->GetEntriesFast() ) {
        for (Int_t index = 0; index < fMergedCells->GetEntriesFast(); ++index) {
            ZeeDMergedCell* cell =
                static_cast<ZeeDMergedCell*> ( fMergedCells->At(index) );

            cell->Draw(option);
        }
    } else {
        for (Int_t index = 0; index < fArray->GetEntriesFast(); ++index) {
            ZeeDGridCell* cell = this->GetCell(index);

            if (cell == NULL) {
                continue;
            }

            Double_t x1 = cell->GetCoordLow()->At(0);
            Double_t y1 = cell->GetCoordLow()->At(1);

            Double_t x2 = cell->GetCoordHigh()->At(0);
            Double_t y2 = cell->GetCoordHigh()->At(1);

            box->DrawBox(x1, y1, x2, y2);

            sprintf(value, "%.f", cell->GetValue());
            text->DrawLatex( (x1 + x2) / 2, (y1 + y2) / 2, value);
        }
    }

    delete empty;
    delete text;
    delete box;
}

//------------------------------------------------------
void ZeeDBinGrid::DrawMergedCells(Option_t* option)
{
    for (Int_t index = 0; index < fMergedCells->GetEntriesFast(); ++index) {
        ZeeDMergedCell* cell =
            static_cast<ZeeDMergedCell*> ( fMergedCells->At(index) );
        cell->Draw(option);
    }
}
//------------------------------------------------------
vector<std::pair<Double_t, Double_t> > ZeeDBinGrid::GetBinBoundaries(Int_t index) const
{

  const ZeeDGridCell* cell = this->GetCell(index);
  const UInt_t ndimensions = cell->GetNumDimentions();

  vector<std::pair<Double_t, Double_t> > boundaries;

  std::pair<Double_t, Double_t> binBoundaries;

  for(UInt_t n = 0; n != ndimensions; ++n) {

      binBoundaries.first = cell->GetCoordLow()->At(n);
      binBoundaries.second = cell->GetCoordHigh()->At(n);

      boundaries.push_back(binBoundaries);

  }

  return boundaries;
}

std::vector<TString> ZeeDBinGrid::GetBinLabels(const TString& format) const {

    std::vector<TString> binLabels;

    for (Int_t i=0; i < this->GetNumOfCells(); i++) {

        TString binLabel = "Bin";
        binLabel += i;
        binLabel += "|";

        std::vector<std::pair<Double_t, Double_t> > binBoundaries = this->GetBinBoundaries(i);

        for( UInt_t n=0; n != binBoundaries.size(); ++n) {

            const Double_t low_edge = binBoundaries[n].first;
            const Double_t high_edge = binBoundaries[n].second;

            binLabel += TString::Format(format, low_edge);
            binLabel += "_";
            binLabel += TString::Format(format, high_edge);
            binLabel += "|";

        }

        binLabels.push_back(binLabel);

    }

    return binLabels;

}

//------------------------------------------------------
void ZeeDBinGrid::Print(Option_t* option) const
{
    // Blue print

    if ( strcmp(option, "merged" ) == 0 ) {
        for (Int_t i = 0; i < fMergedCells->GetEntriesFast(); ++i ) {
            /*
            ZeeDMergedCell *cell =
                static_cast<ZeeDMergedCell*>( fMergedCells->At(i) );
            */
            cout << "cell " << i << ", ";
            // SHOULD BE DEBUG
            //            cell->Print();
        }
    } else {

        for (Int_t index = 0; index < fArray->GetEntriesFast(); ++index) {
            cout << index << ": ";

            ZeeDGridCell* cell = this->GetCell(index);

            if (cell != NULL) {
                cell->Print();
            }
        }
    }
}

//------------------------------------------------------
void ZeeDBinGrid::WriteIntoFile(TString fileName) const
{

    // SHOULD BE DEBUG
    //    cout << "Writing grid into " <<  fileName << endl;

    // Open file for writing
    FILE* asciFile = fopen(fileName.Data(), "w");

    if ( !asciFile ) {
        Error(__FUNCTION__, "Couldn't open file " + fileName);
        return;
    }

    // Print edges
    for (Int_t j = 0; j < fBinEdges->GetEntriesFast(); ++j) {

        fprintf(asciFile, "Edges = { // Array of bin grid edges\n");

        ZeeDArrayD* edges = static_cast<ZeeDArrayD*>(fBinEdges->At(j));

        for (Int_t k = 0; k < edges->GetEntriesFast(); ++k) {

            fprintf(asciFile, "%12.6f", edges->At(k) );

            if ( k == edges->GetEntriesFast() - 1 ) {
                fprintf(asciFile, "\n");
            } else {
                fprintf(asciFile, ",");

                if ( (k + 1 ) % 5 == 0 ) {
                    fprintf(asciFile, "\n");
                }
            }
        }

        fprintf(asciFile, "}\n\n");
    }

    // Print merged cells
    fprintf(asciFile, "// Cells that are merged: \n");

    for (Int_t i = 0; i < fMergedCells->GetEntriesFast(); ++i) {
        ZeeDMergedCell* cell = this->GetMergedCell(i);
        const ZeeDArrayI* array = cell->GetCells();
        Int_t numOfCells = array->GetEntriesFast();

        // Print only cells with multiple entries
        if ( numOfCells < 2 ) {
            continue;
        }

        fprintf(asciFile, "Merged = { // merged cell No = %d \n", i);

        for (Int_t k  = 0; k < numOfCells; ++k ) {
            fprintf(asciFile, "%6i", array->At(k) );

            if ( k == numOfCells - 1 ) {
                fprintf(asciFile, "\n");
            } else {
                fprintf(asciFile, ",");

                if ( (k + 1) % 10 == 0 ) {
                    fprintf(asciFile, "\n");
                }
            }
        }

        fprintf(asciFile, "}\n\n");
    }

    // Print grid content into a file
    fprintf(asciFile, "// Human readable grid print-out. \n");
    fprintf(asciFile, "// It is optional for reading of the grid from text file.\n");
    fprintf(asciFile, "// Used to set up cell values \n\n");

    fprintf(asciFile, "Grid = {\n");
    fprintf(asciFile, "// Cell.idx MergedTo       Value  Coord: low(x,y..) high(x,y..)\n");

    for (Int_t index = 0; index < fArray->GetEntriesFast(); ++index) {

        ZeeDGridCell* cell = this->GetCell(index);

        if (cell == NULL) {
            break;
        }

        Int_t mergedIdx = cell->GetMergedCellIdx();
        Double_t value  = cell->GetValue();

        fprintf(asciFile, "%10d",    index);
        fprintf(asciFile, "%10d",    mergedIdx);
        fprintf(asciFile, "%14.5f",  value);

        const ZeeDArrayD* low  = cell->GetCoordLow();
        const ZeeDArrayD* high = cell->GetCoordHigh();

        for (Int_t j = 0; j < low->GetEntriesFast(); ++j) {
            fprintf(asciFile, "%14.5f",  low->At(j) );
        }

        for (Int_t k = 0; k < high->GetEntriesFast(); ++k) {
            fprintf(asciFile, "%14.5f",  high->At(k) );
        }

        fprintf(asciFile, "\n");
    }

    fprintf(asciFile, "}\n\n");

    // Close file
    fclose(asciFile);

}

//------------------------------------------------------
void ZeeDBinGrid::ReadFromFile(TString fileName, Bool_t createGrid, Bool_t extendGrid)
{
    string sline;
    string text_all;

    ifstream inFile;
    inFile.open(fileName);

    // If file couldn't be opend
    if (!inFile) {
        Error("ZeeDBinGrid::ReadFromFile",
              "Couldn't open file %s", fileName.Data());
        gSystem->Exit(EXIT_FAILURE);
    } else {
         Info("ZeeDBinGrid::ReadFromFile",
                 "file is opened %s", fileName.Data());
    }

    // Read file
    while ( !inFile.eof() ) {
        getline(inFile, sline);

        // Remove comments
        Int_t pos = sline.find("//");

        if ( pos >= 0 ) {
            sline.erase(pos, sline.length() - pos);
        }

        text_all += sline;
        text_all += '\n';
    }

    inFile.close();

    // Read bin edges from steer and create regular grid
    if (extendGrid) {
      this->ReadExtendedBinEdges( &text_all );
    } else {
      this->ReadBinEdges( &text_all );
    }

    if ( createGrid ) {

        this->CreateGrid();

        this->ReadMergedCells( &text_all );
        this->ReadGrid( &text_all );

    }
}

//------------------------------------------------------
Bool_t ZeeDBinGrid::ReadString(std::string* steer,
                               Int_t* pos, TString pattern,  std::string* str)
{

    Int_t pos1 = steer->find(pattern, *pos);
    Int_t pos2 = steer->find("{", pos1 + 1);
    Int_t pos3 = steer->find("}", pos2 + 1);
    *pos = pos3 + 1;

    if (  -1 < pos1 && -1 < pos2 && -1 < pos3 ) {
        str->assign(*steer, pos2 + 1, pos3 - pos2 - 1);

        // Replace ',' by spaces
        for (UInt_t i = 0; i < str->length(); i++) {
            if (str->at(i) == ',') {
                str->at(i) = ' ';
            }
        }

        return kTRUE;
    }

    return kFALSE;
}

//------------------------------------------------------
void ZeeDBinGrid::ReadBinEdges(std::string* steer)
{
    string str;
    Double_t value;

    Int_t pos0 = 0;

    while ( this->ReadString(steer,  &pos0, "Edges", &str) ) {

        ZeeDArrayD* array = new ZeeDArrayD();

        stringstream sstr(str);

        while ( sstr >> value ) {
            array->AddLast(value);
        }

        this->AddBinEdges(array);
    }
}

//------------------------------------------------------
void ZeeDBinGrid::ReadExtendedBinEdges(std::string* steer)
{
  // The function extends the specified binning with the +-inf borders
  // such that there is no over/underflow anymore

  string str;
  Int_t pos = 0;
  while ( this->ReadString(steer,  &pos, "Edges", &str) ) {

    stringstream sstr(str);

    ZeeDArrayD* array = new ZeeDArrayD();
    array->AddLast(-DBL_MAX);
    Double_t value;
    while ( sstr >> value ) array->AddLast(value);
    array->AddLast(DBL_MAX);

    this->AddBinEdges(array);
  }

}

//------------------------------------------------------
void ZeeDBinGrid::ReadMergedCells(std::string* steer)
{

    string str;
    Int_t index;

    Int_t pos0 = 0;

    while ( this->ReadString(steer,  &pos0, "Merged", &str) ) {

        stringstream sstr(str);
        sstr >> index;

        // First cell index
        ZeeDGridCell* first_cell = this->GetCell(index);

        // Merge
        while ( sstr >> index ) {
            Int_t first_cell_idx = first_cell->GetMergedCellIdx();

            ZeeDGridCell* cell = this->GetCell(index);
            Int_t cell_idx = cell->GetMergedCellIdx();

            MergeCells(first_cell_idx, cell_idx);
        }
    }
}

//------------------------------------------------------
void ZeeDBinGrid::ReadGrid(std::string* steer)
{
    string str;

    Int_t    cell_idx;
    Int_t    merged_cell_idx;
    Double_t value;

    Int_t  grid_pos = 0;

    while ( this->ReadString(steer,  &grid_pos, "Grid", &str) ) {

        Int_t pos = 0;

        while ( pos > -1 ) {
            stringstream sstr (stringstream::in | stringstream::out);

            sstr << str;

            sstr >> cell_idx;
            sstr >> merged_cell_idx;
            sstr >> value;

            ZeeDGridCell* cell = this->GetCell(cell_idx);
            cell->SetValue(value);

            pos = str.find('\n', 1);
            str.erase(0, pos + 1);
        }
    }
}

//------------------------------------------------------
Bool_t ZeeDBinGrid::OutOfBounds(Int_t index, TObjArray* array) const
{
    return (index < 0 || index > (array->GetEntriesFast() - 1) );
}
