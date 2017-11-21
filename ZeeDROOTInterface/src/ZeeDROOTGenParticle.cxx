#include "ZeeDROOTInterface/ZeeDROOTGenParticle.h"
#include "ZeeDEvent/ZeeDGenParticle.h"


ZeeDROOTGenParticle::ZeeDROOTGenParticle() : fParticleID(0),
    fParticleStatus(ZeeDEnum::MCFSRLevel::Born)
{

    //fDaughters.SetOwner(kTRUE);

}

ZeeDROOTGenParticle::~ZeeDROOTGenParticle()
{
    //fDaughters.Delete();
}

Int_t ZeeDROOTGenParticle::fillOut (ZeeDGenParticle* const genParticle) const
{
    // Fills ZeeDGenParticle with the information from ZeeDROOTGenParticle

    
    genParticle->fMCFourVector   = fMCFourVector;
    genParticle->fParticleID     = fParticleID;
    genParticle->fParticleStatus = fParticleStatus;
    genParticle->fDaughters->Clear();
    
    for(UInt_t d=0; d != fDaughters.size(); ++d) {

        ZeeDGenParticle* daughter = new ZeeDGenParticle();
        
        ZeeDROOTGenParticle* rootDaughter = fDaughters[d];
        rootDaughter->fillOut(daughter);

        genParticle->fDaughters->Add(daughter);

    }

    return 0;
}

Int_t ZeeDROOTGenParticle::fillIn (const ZeeDGenParticle* const genParticle)
{
    // Fills ZeeDROOTGenParticle with the information from ZeeDGenParticle

    fMCFourVector   = genParticle->fMCFourVector;
    fParticleID     = genParticle->fParticleID;
    fParticleStatus = ZeeDEnum::MCFSRLevel::Born;

    fDaughters.clear();

    TIter nextDaughter(genParticle->fDaughters);

    ZeeDGenParticle* daughter;

    while(( daughter = (ZeeDGenParticle*) nextDaughter())) {
        ZeeDROOTGenParticle* rootDaughter = new ZeeDROOTGenParticle();
        rootDaughter->fillIn(daughter);
        fDaughters.push_back(rootDaughter);
    }

    return 0;
}
