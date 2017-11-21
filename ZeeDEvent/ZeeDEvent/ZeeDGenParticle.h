#ifndef __ZEEDGENPARTICLE_H
#define __ZEEDGENPARTICLE_H

////////////////////////////////////////////////////////
// Name    : ZeeDGenParticle.h
////////////////////////////////////////////////////////
//
// Contact persons: Andrei Nikiforov, Sebastian Schmitt
//

// Root includes
#include <TObject.h>
#include <TLorentzVector.h>
#include <TObjArray.h>
#include "ZeeDMisc/ZeeDMisc.h"


class ZeeDGenParticle : public TObject {
public:

    ZeeDGenParticle();
    ZeeDGenParticle(const ZeeDGenParticle& particle);
    ~ZeeDGenParticle();

    // Getters
    const TLorentzVector& GetMCFourVector() const {
        return fMCFourVector;
    }
    ZeeDGenParticle* GetParent()     const {
        return fParent;
    }
    TObjArray* GetDaughters()        const {
        return fDaughters;
    }

    Int_t GetParticleID()     const {
        return fParticleID;
    }
    
    Double_t GetMt()       const {
        return fBosMt;
    }

    ZeeDEnum::MCFSRLevel::Value GetParticleStatus() const {
        return fParticleStatus;
    }


    // Setters
    void SetMCFourVector(const TLorentzVector& fourVector) {
        fMCFourVector = fourVector;
    }
    void SetParent( ZeeDGenParticle* parent) {
        fParent = parent;
    }
    void AddDaughter( ZeeDGenParticle* daughter) {
        fDaughters->AddLast(daughter);
    }

    void SetParticleID(Int_t id) {
        fParticleID = id;
    }

    void SetParticleStatus(ZeeDEnum::MCFSRLevel::Value FSRstatus) {
        fParticleStatus = FSRstatus;
    }
    
    void SetMt (Double_t bosMt){
        fBosMt=bosMt;
    }

    /** Blue print */
    void Print(Option_t* option = "") const;

private:

    /**  Truth four-vector */
    TLorentzVector fMCFourVector;            //!
    ZeeDGenParticle* fParent;                //!
    TObjArray* fDaughters;                   //!
    Int_t fParticleID;                       //!
	ZeeDEnum::MCFSRLevel::Value  fParticleStatus;                  //!
    Double_t fBosMt;

    friend class ZeeDROOTGenParticle;

};

#endif // ZeeDGenParticle
