#ifndef ZEEDROOTTAU_H
#define ZEEDROOTTAU_H

#include <TObject.h>
#include <TLorentzVector.h>

#include "SGTools/CLASS_DEF.h"

class ZeeDTau;

class ZeeDROOTTau {
public:
	ZeeDROOTTau();
	~ZeeDROOTTau();

    Int_t       fillIn (const ZeeDTau* const tau);
    Int_t       fillOut(ZeeDTau* const tau) const;

private:

    TLorentzVector  fFourVector;
    Int_t           fCollectionIndex;

    Bool_t fIsTight;
    Bool_t fIsMedium;
    Bool_t fIsLoose;

    Bool_t fIsBDTTight;
    Bool_t fIsBDTMedium;
    Bool_t fIsBDTLoose;

    Bool_t fIsEleBDTTight;
    Bool_t fIsEleBDTMedium;
    Bool_t fIsEleBDTLoose;

    Bool_t fIsLLHTight;
    Bool_t fIsLLHMedium;
    Bool_t fIsLLHLoose;

    Bool_t fIsMuonVeto;

};

CLASS_DEF( ZeeDROOTTau , 149813289 , 1 )

#endif /* ZEEDROOTTAU_H */
