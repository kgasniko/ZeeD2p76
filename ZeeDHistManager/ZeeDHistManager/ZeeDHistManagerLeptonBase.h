#ifndef __ZEEDHISTMANAGERELECTRONBASE_H
#define __ZEEDHISTMANAGERELECTRONBASE_H

///////////////////////////////////////////////
//  Name ZeeDHistManagerLeptonBase
//  Interfact class to provide basic iterators for electron based histograms
//  Created by SG, 19 Feb 2011.
//
//////////////////////////////////////////////

#include "ZeeDHist/ZeeDHistManager.h"
#include "TString.h"

class ZeeDLepton;
class TObjArray;


class ZeeDHistManagerLeptonBase : public ZeeDHistManager {
public:

    ZeeDHistManagerLeptonBase(TString name, Bool_t isAll = kFALSE)
            : ZeeDHistManager(name), bIsMC(kFALSE), bIsAll(isAll), bIsElec(kTRUE), bIsZ(kTRUE)
    {}

    void SetProperties(Bool_t isAll=kFALSE, Bool_t isElec = kTRUE, Bool_t isZ = kTRUE) {
        bIsAll = isAll;
        bIsElec = isElec;       
	   	bIsZ = isZ;	
    }

	void SetW(){
		bIsZ = kFALSE;
	}

    // Arrange loop over electrons
    using ZeeDHistManager::Fill;
    virtual void Fill();

    // Fill histos for list of electrons
    virtual void FillLeptonHists(const TObjArray* electrons, Double_t Weight) = 0;


protected:
    Bool_t bIsMC;
    Bool_t bIsAll;
    Bool_t bIsElec;
	Bool_t bIsZ;
};

#endif //ZeeDHistManagerLeptonBase

