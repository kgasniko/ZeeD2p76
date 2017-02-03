#ifndef __ZEEDHISTMANAGERELECTRON_H
#define __ZEEDHISTMANAGERELECTRON_H

///////////////////////////////////////////////
//  Name ZeeDHistManagerElectron
//  Basic histograms for electrons
//  Created by SG, 13 Feb 2011.
//
//////////////////////////////////////////////

#include "ZeeDHistManager/ZeeDHistManagerLeptonBase.h"
#include "ZeeDHist/ZeeDHistManager.h"
#include "TString.h"

class ZeeDElectron;
class TObjArray;


class ZeeDHistManagerElectron : public ZeeDHistManagerLeptonBase {
public:

    explicit ZeeDHistManagerElectron(TString name, Bool_t isAll=kFALSE)
        : ZeeDHistManagerLeptonBase(name, isAll)
    {
	}

    // Book all histograms:
    virtual void BookHistos();

    // Fill histos for list of electrons
    virtual void FillLeptonHists(const TObjArray* electrons, Double_t Weight);

private:

    void FillBitsHist(UInt_t bits, Double_t Weight, const TString& hist);

};

#endif //ZeeDHistManagerElectron

