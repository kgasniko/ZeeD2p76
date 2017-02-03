#pragma once

/*
 * ZeeDROOTMuonSpShower.h
 *
 *  Created on: 2013.12.19
 *      Author: SSh
 */


#include <TObject.h>
#include <TLorentzVector.h>

#include "SGTools/CLASS_DEF.h"

class ZeeDMuonSpShower;

class ZeeDROOTMuonSpShower {


  public:

    ZeeDROOTMuonSpShower();
    virtual ~ZeeDROOTMuonSpShower();

    Int_t fillIn (const ZeeDMuonSpShower* const muonSpShower);
    Int_t fillOut(ZeeDMuonSpShower* const muonSpShower) const;


  private:

    float feta;
    float fphi;
    unsigned short ftrigHits;
    unsigned short finnerHits;
    unsigned short fmiddleHits;
    unsigned short fouterHits;
    unsigned short finnerSegments;
    unsigned short fmiddleSegments;
    unsigned short fouterSegments;

};

