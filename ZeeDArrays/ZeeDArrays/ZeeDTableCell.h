#ifndef __ZEEDTABLECELL_H
#define __ZEEDTABLECELL_H

////////////////////////////////////////////////////////
// Name    : ZeeDTableCell.h
////////////////////////////////////////////////////////


#include <TString.h>
#include <TObject.h>
#include "ZeeDArrays/ZeeDArrayD.h"

/** Class to define a cell of a text table
   @author  Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
   @date 2008/10/14
*/
class ZeeDTableCell : public TObject {
public:

    ZeeDTableCell(TString name);
    virtual ~ZeeDTableCell();

    void SetValue(Double_t value, Int_t index);
    void AddLast(Double_t value);
    void InsertValue(Double_t value, Int_t index);

    Double_t GetValue(Int_t index) const;
    TString GetCellName() const;

private:

    TString fName;      //!
    ZeeDArrayD* fValue; //!

    Bool_t outOfBounds(Int_t index) const;
};

#endif
