#pragma once

///////////////////////////////////////////////
//  Name ZeeDHistManagerMuon
//  Basic histograms for muons
//  Created by SSh, 01 Nov 2013.
//
//////////////////////////////////////////////

#include "ZeeDHistManager/ZeeDHistManagerLeptonBase.h"
#include "TString.h"

class ZeeDHistManagerMuon : public ZeeDHistManagerLeptonBase {
public:

    ZeeDHistManagerMuon(TString name, Bool_t isAll=kFALSE)
        : ZeeDHistManagerLeptonBase(name)
    {
        this->SetProperties(isAll,kFALSE); // Boson, muon        
    }

    // Book all histograms:
    virtual void BookHistos();

    // Fill histos
    virtual void FillLeptonHists(const TObjArray* muonArray, Double_t Weight);

};

