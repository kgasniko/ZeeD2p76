#ifndef __ZEEDBINGRID_H
#define __ZEEDBINGRID_H

////////////////////////////////////////////////////////
// Name    : ZeeDBinGrid.h
////////////////////////////////////////////////////////

// Root includes
#include <string>
#include <vector>
// Root includes
#include <TObject.h>
#include <TObjArray.h>
#include <TRandom.h>
#include <TString.h>

// ZeeD analysis includes
#include "ZeeDArrays/ZeeDArrayD.h"
#include "ZeeDArrays/ZeeDGridCell.h"
#include "ZeeDArrays/ZeeDMergedCell.h"

using std::vector;

/** Class to define bin grid interface
    @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
    @date 2008/09/30
*/
class ZeeDBinGrid : public TObject {

public:

    ZeeDBinGrid();

    ZeeDBinGrid(TString name);

    ZeeDBinGrid& operator=(const ZeeDBinGrid& rhs);

    ZeeDBinGrid& operator/(ZeeDBinGrid& rhs);

    ZeeDBinGrid& operator+(ZeeDBinGrid& rhs);

    ~ZeeDBinGrid();

    /** Add cell to the array */
    void AddCell(TObject* cell);

    /** Add a set of bin boundaries to the bin grid */
    void AddBinEdges(ZeeDArrayD* array);

    /** Creates grid from the bin edges */
    void CreateGrid();

    /** Creates grid from the table of cell edges */
    void CreateGrid2DFromTable(TString tableName);

    /** Merge cells until N = minN */
    void Merge(Double_t minN);

    /** Merges cells with indices one and two */
    void MergeCells(Int_t one, Int_t two);

    // Setters
    void SetTitleX(TString title) {
        fTitleX = title;
    }
    void SetTitleY(TString title) {
        fTitleY = title;
    }
    void SetName(TString name) {
        fName = name;
    }

    /** Sets value to all elements of the array */
    void Reset(Double_t value = 0.0);

    /** Sets value to the cell with index = index */

    /** Adds value to the cell with index = index content */
    void Set(Int_t index, Double_t value);

    void SetDefValue(double val) {
        fDefValue = val;
    }

    void Add(Int_t index, Double_t value);

    /** Fills cell with value */
    void Fill(Double_t value, ZeeDArrayD* coord);

    /** Fills 1D cell with value */
    void Fill(Double_t value, Double_t xCoord);

    /** Fills 2D cell with value */
    void Fill(Double_t value, Double_t xCoord, Double_t yCoord);

    /** Fills 3D cell with value */
    void Fill(Double_t value, Double_t xCoord, Double_t yCoord, Double_t zCoord);

    // Getters
    const char*        GetName() const {
        return fName;
    }

    /** Returns a size of bin with index = index */
    double             GetSize(Int_t index) const;

    /** Retruns a number of events of BinGrid */
    double             GetEntriesFast() const;

    /** Return value of bin by index*/
    double             GetValue(Int_t index) const;
    /** Return value of bin by 1D parameter (like Eta, Pt, Phi)*/
    double             GetValue(double par) const;
    /** Return value of bin by 2D parameter (like Eta, Pt, Phi)*/
    double             GetValue(double par1, double par2) const;

    /** Returns a cell with index = index */
    ZeeDGridCell*      GetCell(Int_t index) const;

    /** Retruns a cell with coordinates coord */
    ZeeDGridCell*      GetCell(ZeeDArrayD* coord) const;

    /** Retruns a cell with index = index */
    ZeeDMergedCell*    GetMergedCell(Int_t index) const;

    /** Returns index of a cell with coordinates coord */
    Int_t              GetIndex(const ZeeDArrayD* coord) const;
    Int_t              GetNumOfCells() const {
        return fArray->GetEntriesFast();
    }
    Int_t              GetNumOfMergedCells() const {
        return fMergedCells->GetEntriesFast();
    }

    /** Find boundaries of bin index, function returns vector of pairs  (bin1low, bin1high), (bin2low, bin2high), ... */
    vector<std::pair<Double_t, Double_t> > GetBinBoundaries(Int_t index) const;

    std::vector<TString> GetBinLabels(const TString& format="%.2f") const;

    void Print(Option_t* option = "") const;

    /** Draws a bin grid */
    void Draw(Option_t* option = "");

    /** Draws merged cells */
    void DrawMergedCells(Option_t* option = "");

    /** Writes bin grid into a text file */
    void WriteIntoFile(TString fileName = "BinGrid.txt") const;

    /** Reads table from a file */
    void ReadFromFile(TString fileName, Bool_t createGrid=kTRUE, Bool_t extendedGrid = kFALSE);

    TObjArray* GetBinEdges() const {
        return fBinEdges;
    }

private:

    ZeeDBinGrid(const ZeeDBinGrid& array);

    double fDefValue;

    /** Grids name */
    TString fName; //!

    /** Array of cells */
    TObjArray*  fArray; //!

    /** Array of merged cells */
    TObjArray*  fMergedCells; //!

    /** Array of bin edges */
    TObjArray*  fBinEdges; //!

    /** Lower edge of the grid */
    ZeeDArrayD* fMinLow; //!

    /** High edge of the grid */
    ZeeDArrayD* fMaxHigh; //!

    /** Is regular grid */
    Bool_t      bIsRegular; //!

    /** Random number generator */
    TRandom*    fRandom; //!

    /** X title */
    TString fTitleX; //!

    /** Y title */
    TString fTitleY; //!

    Bool_t OutOfBounds(Int_t index, TObjArray* array) const;

    /** Finds bin number corresponding to abscissa x */
    Int_t FindBin(Double_t x, ZeeDArrayD* array);
    Int_t CalcIndex(Int_t x, Int_t y, Int_t max_x) const;
    void AnalyseGrid();

    /** Analyses regular grid */
    void AnalyseRegGrid();

    /** Sorts elements  of merged array */
    void SortMergedCells( TObjArray* array );

    /** Compare two objects. Returns 0 when equal, -1 when this is
        smaller and +1 when bigger (like strcmp()).
    */
    Int_t ObjCompare(TObject* a, TObject* b);

    /** Sort array of TObject pointers using a quicksort algorithm.
        Uses ObjCompare() to compare objects.
    */
    void QSort(TObjArray* a, Int_t first, Int_t last);

    /** Finds definition of bin edges in the steer */
    void ReadBinEdges(std::string* steer);

    /** Finds definition of (extended with +-inf) bin edges in the steer */
    void ReadExtendedBinEdges(std::string* steer);

    void ReadMergedCells(std::string* steer);

    /** Reads grid values */
    void ReadGrid(std::string* steer);

    /** Reads string array from steer */
    Bool_t ReadString(std::string* steer, Int_t* pos,
                      TString pattern, std::string* str);
};

#endif
