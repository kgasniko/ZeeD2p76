#pragma once

/*
 * ZeeDEMParticle.h
 *
 *  Created on: Jan 21, 2013
 *      Author: SSh 
 */


#include <TObject.h>
#include "TLorentzVector.h"

class ZeeDEMParticle: public TObject {

  public:

    ZeeDEMParticle() : fhasCharge(false), fcharge(0.) {}

    const TLorentzVector& GetFourVector() const { return ffourVector; }
    float getCharge() const { return fcharge; }
    bool  getHasCharge() const { return fhasCharge; }

    void setFourVector(TLorentzVector fourVector) { ffourVector = fourVector; }
    void setCharge(float charge) { fcharge = charge; }
    void setHasCharge(bool hasCharge) { fhasCharge = hasCharge; }

  protected:

    TLorentzVector	ffourVector;
    bool		fhasCharge;
    float		fcharge;

};

