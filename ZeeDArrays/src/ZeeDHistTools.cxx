#include "ZeeDArrays/ZeeDHistTools.h"
#include <TRandom3.h>

#include <iostream>

// ----------------------------------------------------------
ZeeDPoissonRandomVault::ZeeDPoissonRandomVault(Int_t NRandom, Int_t iSeed)
    : IZeeDEventRandomNumberVault(NRandom)
{
    fRndGen = new TRandom3(iSeed);
}

// ----------------------------------------------------------
ZeeDPoissonRandomVault::~ZeeDPoissonRandomVault()
{
    delete fRndGen;
}

// ----------------------------------------------------------
void ZeeDPoissonRandomVault::UpdateNumbers()
// Generate random numbers for new event
{
    for (Int_t i = 0; i < fNRandom; i++) {
        fRandom[i] = fRndGen->Poisson(1.0);
    }
}

