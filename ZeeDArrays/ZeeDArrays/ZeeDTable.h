#ifndef __ZEEDTABLE_H
#define __ZEEDTABLE_H

////////////////////////////////////////////////////////
// Name    : ZeeDTable.h
////////////////////////////////////////////////////////

#include <TROOT.h>
#include <TString.h>
#include <TObject.h>
#include <TObjArray.h>
#include <TArrayD.h>

/** Basic class to read text tables
    @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
    @date 2008/10/14
*/
class ZeeDTable : public TObject {
public:

    ZeeDTable(TString filename);
    virtual ~ZeeDTable();

    Double_t GetTableValue(TString name, Int_t bin) const;
    Int_t GetTableLength() const;

    Int_t GetBin(TString nameOne, TString nameTwo,
                 Double_t valueOne, Double_t valueTwo) const;

    Bool_t ReadTable();
    void AddCell(TString name);
    void SetTableColumns(TString* columnNames, Int_t numOfColumns);
    void Sort(TString columnName);

private:

    TObjArray* fTable; //!
    Int_t fLength;     //!
    TString fFileName; //!

    Int_t GetTableIndex(TString name) const;
    void Swap(Int_t binOne, Int_t binTwo);
};

#endif
