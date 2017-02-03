#ifndef __ZEEDHISTTOOLS_H
#define __ZEEDHISTTOOLS_H

//////////////////////////////////////////////////////
//
// Name: ZeeDHistToools.h
//
// Created 6/03/11 by SG. Collection of tools for histograms.
//
//
/////////////////////////////////////////////////////

#include <TString.h>


/** Class to store collection of random numbers generated for an event.
 */
class IZeeDEventRandomNumberVault {
public:
    IZeeDEventRandomNumberVault(Int_t NRandom)
        : fNRandom(NRandom),
          fRandom(NULL) {
        fRandom = new Double_t [fNRandom];
    };
    virtual ~IZeeDEventRandomNumberVault() {
        delete [] fRandom;
    };
    virtual void UpdateNumbers() = 0;
    Double_t GetRandom(Int_t iRandom) {
        if ((iRandom >= 0) && (iRandom <= fNRandom)) {
            return fRandom[iRandom];
        } else {
            return -1.;
        };
    }

    Int_t GetNRandom()        {
        return fNRandom;
    }

protected:
    Int_t      fNRandom;
    Double_t*  fRandom;
};


class TRandom3;


class ZeeDPoissonRandomVault : public IZeeDEventRandomNumberVault {
public:
    ZeeDPoissonRandomVault(Int_t NRandom, Int_t iSeed = 65539);
    ~ZeeDPoissonRandomVault();
    virtual void UpdateNumbers();
private:
    TRandom3* fRndGen;
private:

};



#endif
