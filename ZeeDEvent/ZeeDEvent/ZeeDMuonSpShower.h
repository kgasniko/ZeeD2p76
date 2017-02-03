#pragma once

/*
 * ZeeDMuonSpShower.h
 *
 *  Created on: 2013.12.19
 *      Author: SSh
 */


#include <TObject.h>
#include "TLorentzVector.h"

class ZeeDMuonSpShower: public TObject {


  public:

    ZeeDMuonSpShower();
    virtual ~ZeeDMuonSpShower();

    float feta;
    float fphi;
    unsigned short ftrigHits;
    unsigned short finnerHits;
    unsigned short fmiddleHits;
    unsigned short fouterHits;
    unsigned short finnerSegments;
    unsigned short fmiddleSegments;
    unsigned short fouterSegments;


  private:

    friend class ZeeDReadMuonSpShowerAOD;
    friend class ZeeDROOTMuonSpShower;

};
