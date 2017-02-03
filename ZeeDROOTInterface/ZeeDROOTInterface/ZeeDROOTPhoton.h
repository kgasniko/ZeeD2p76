#ifndef ZEEDROOTPHOTON_H
#define ZEEDROOTPHOTON_H

#include <TObject.h>
#include <TLorentzVector.h>

#include "SGTools/CLASS_DEF.h"

class ZeeDPhoton;

class ZeeDROOTPhoton {
public:
	ZeeDROOTPhoton();
	~ZeeDROOTPhoton();

    Int_t       fillIn (const ZeeDPhoton* const photon);
    Int_t       fillOut(ZeeDPhoton* const photon) const;

private:

    TLorentzVector  fFourVector;
    Int_t           fCollectionIndex;

};

CLASS_DEF( ZeeDROOTPhoton , 180190308 , 1 )

#endif /* ZEEDROOTPHOTON_H */
