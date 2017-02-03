#ifndef ZEEDROOTGENPARTICLE_H
#define ZEEDROOTGENPARTICLE_H

#include <TObject.h>
#include <TObjArray.h>
#include <TLorentzVector.h>
#include "ZeeDMisc/ZeeDMisc.h"

#include "SGTools/CLASS_DEF.h"

class ZeeDGenParticle;

/** Partner class of ZeeDGenParticle
    @author George Sedov
    @date Wed Mar 16 13:15:57 CEST 2011
*/
class ZeeDROOTGenParticle {

public:

    ZeeDROOTGenParticle();
    virtual ~ZeeDROOTGenParticle();

    Int_t       fillIn (const ZeeDGenParticle* const genParticle);
    Int_t       fillOut(ZeeDGenParticle* const genParticle) const;

private:
    // Fields that must be saved in ROOT. NO POINTERS ALLOWED
    TLorentzVector fMCFourVector;
    std::vector<ZeeDROOTGenParticle*> fDaughters;
    Int_t fParticleID;
	ZeeDEnum::MCFSRLevel::Value fParticleStatus;

};

CLASS_DEF( ZeeDROOTGenParticle , 165679871 , 1 )

#endif
