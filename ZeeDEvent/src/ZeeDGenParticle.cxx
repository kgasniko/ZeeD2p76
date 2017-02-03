#include "ZeeDEvent/ZeeDGenParticle.h"

// std libraries
#include <iostream>

// ZeeD libraries
#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDGenParticle::ZeeDGenParticle() :
    fParent(NULL),
    fDaughters(NULL),
    fParticleID(-999),
    fParticleStatus(ZeeDEnum::MCFSRLevel::Born)
{
    // Constructor
    fMCFourVector.SetXYZT(0., 0., 0., 0.);
    fDaughters = new TObjArray();
    //fDaughters->SetOwner(kFALSE);
    fDaughters->SetOwner(kTRUE);
}

//------------------------------------------------------
ZeeDGenParticle::ZeeDGenParticle(const ZeeDGenParticle& particle) : TObject(particle)
{
    // Copy constructor

    fMCFourVector   = particle.fMCFourVector;
    fParent         = particle.fParent;
    fParticleID     = particle.fParticleID;
    fParticleStatus = particle.fParticleStatus;

    fDaughters = new TObjArray();
    //fDaughters->SetOwner(kFALSE);
    fDaughters->SetOwner(kTRUE);

    for (Int_t i = 0; i < particle.fDaughters->GetEntriesFast(); ++i) {
        ZeeDGenParticle* daughter =
            static_cast<ZeeDGenParticle*>( particle.fDaughters->At(i) );
        fDaughters->AddLast( new ZeeDGenParticle( *daughter ) );
    }
}

//------------------------------------------------------
ZeeDGenParticle::~ZeeDGenParticle()
{
    // Destructor
    delete fDaughters;
}

//------------------------------------------------------
void ZeeDGenParticle::Print(Option_t* option) const
{
    // Blue print
    if ( !option || !option[0] ) {
        std::cout << option << std::endl;
    }

    ZeeDMisc::Dump4Vec("fourVector: ", fMCFourVector);
    std::cout << "  particle ID = " << fParticleID << std::endl;
}
