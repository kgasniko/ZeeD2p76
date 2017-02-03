#ifndef __ZeeDReadGenAOD_H
#define __ZeeDReadGenAOD_H

////////////////////////////////////////////////////////
// Name    : ZeeDReadGenAOD.h
////////////////////////////////////////////////////////
//
// Contact persons: Sebastian Schmitt, Andrei Nikiforov
//

// Root includes
#include <TObject.h>

// Athena includes
#include "GaudiKernel/ServiceHandle.h"
#include "GaudiKernel/StatusCode.h"
#include "StoreGate/DataHandle.h"

#include "GeneratorObjects/McEventCollection.h"

// Zeed includes
#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDEvent/ZeeDEvent.h"

class TObjArray;

class ZeeDGenParticle;

namespace HepMC {

class GenParticle;
}

/** Class to read generated level information (Z bosons) */
class ZeeDReadGenAOD : public TObject {

public:

    ZeeDReadGenAOD();
    ~ZeeDReadGenAOD();

    void ReadInfo(ZeeDEvent* event);

    void AddDaughters(const HepMC::GenParticle* particle,
                      ZeeDGenParticle* ZBos, TObjArray* GenElec, TObjArray* GenGamma);

    void AddDaughtersAH(const HepMC::GenParticle* particle,
			ZeeDGenParticle* ZBos, TObjArray* GenZ, TObjArray* GenElec, TObjArray* GenGamma);

    void AddDaughtersSherpa(const HepMC::GenParticle* particle,
			    TObjArray* GenZ, TObjArray* GenElec, TObjArray* GenGamma);

    /** Return MC weight */
    Double_t GetMCWeight() {
        return fEventWeight;
    }

private:

    StatusCode Initialize();

    const DataHandle<McEventCollection> fMcEvent;

    Double_t  fEventWeight;
    ZeeDSvcHelper fSvcHelper;

    /** Service handle to job options file */
    ServiceHandle<IZeeDAnalysisSvc> *fAnaOptions;


};

#endif // ZeeDReadGenAOD
