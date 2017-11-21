#define DEBUG_ON 0

#include "ZeeDCalculator/ZeeDCalcW.h"

// std libraries
#include <string>
#include <iostream>
#include <sstream>

// Root libraries
#include <TSystem.h>
#include <TMath.h>
#include "TFile.h"

// Athena libraries
//#include "GaudiKernel/ServiceHandle.h"

// ZeeD analysis includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDEtMiss.h"
#include "ZeeDEvent/ZeeDBosonW.h"
#include "ZeeDTools/ZeeDSystematics.h"
#include "ZeeDEvent/ZeeDGenParticle.h"

#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDCalcW::ZeeDCalcW() :
    fMissingEtObj(NULL),
    fElectrons(NULL),
    fEventInProcess(NULL),
    fEtMiss(NULL),
    fSvcHelper("ZeeDCalcW"),
    fAnaOptions(fSvcHelper.GetAnaSvc())
{

    fSys = ZeeDSystematics::getInstance();

}

//------------------------------------------------------
ZeeDCalcW::~ZeeDCalcW()
{
    // Destructor
}

//------------------------------------------------------
void ZeeDCalcW::Calculate(ZeeDEvent* event)
{
    // Creates W boson candidates
    DEBUG_MES_START;

    fEventInProcess = event;

    // Get array of electrons from event
    const TObjArray* electrons = event->GetElectrons();
    const TObjArray* muons = event->GetMuons();

    if (electrons == NULL) {
        Error("ZeeDCalcW::Calculate", "Electron array in not initialised");
        gSystem->Exit(EXIT_FAILURE);
    }

    event->ClearWBosons();
    event->ClearWmuBosons();
    // Select event with exaclty one electron candidate
    // FIXME: one shall consider also more general
    // case with several W candiates

    for (int elIndex=0; elIndex< electrons->GetEntriesFast(); elIndex++) {
        ZeeDLepton* electron = static_cast< ZeeDLepton* >(electrons->At(elIndex));
        if (electron == NULL) return;
        event->AddWBoson(this->CreateW(electron));         
    }

    for (int muIndex=0; muIndex< muons->GetEntriesFast(); muIndex++) {
        ZeeDLepton* mu = static_cast< ZeeDLepton* >(muons->At(muIndex));
        if (mu == NULL) return;
        event->AddWmuBoson(this->CreateW(mu));         
    }

    DEBUG_MES_END;



}

ZeeDBosonW* ZeeDCalcW::CreateW (const ZeeDLepton* elec){

    ZeeDBosonW* W = new ZeeDBosonW;

    W->SetLepton(elec);

    const TLorentzVector elec4Vec = elec->GetFourVector();
    float charge = elec->getCharge();
    Double_t elecPt  = elec4Vec.Pt();
    Double_t elecPhi = elec4Vec.Phi();

    TObjArray* etMissArray = fEventInProcess->GetEtMissArray();

    if (etMissArray == NULL || etMissArray->GetEntriesFast() <= 0 ) {
        Error("ZeeDCalcW::ReadEtMiss", "Et miss objects are not filled");
        gSystem->Exit(EXIT_FAILURE);
    }
    // Make transverse electron  4-vector
    TLorentzVector p_el = elec4Vec;
    p_el.SetE( elec4Vec.Et() );
    p_el.SetPz(0.);


    ZeeDEtMiss* fEtMiss= (ZeeDEtMiss*) etMissArray->At(0);

    W->SetEtMissPtr(fEtMiss);  

    // Make transverse neutrino 4-vector
    TLorentzVector p_nu;
    p_nu.SetPxPyPzE(fEtMiss->GetEtX(), fEtMiss->GetEtY(), 0., fEtMiss->GetEt());

    // Neutrino
    Double_t neutrinoPt  = fEtMiss->GetEt();
    Double_t neutrinoPhi = p_nu.Phi();

    TLorentzVector p_W = p_nu + p_el;

    // Calculate W's transverse mass
    // MT = sqrt ( 2 p_T^e p_T^nu ( 1 - cos delta phi))
    Double_t Mt = 2 * elecPt * (neutrinoPt);
    Mt *= 1 - TMath::Cos(elecPhi - neutrinoPhi);
    Mt = TMath::Sqrt(Mt);

    W->SetMt(Mt);

    // Reconstruct (transverse) W 4-momentum

    W->SetEtMiss(neutrinoPt);
    W->SetFourVector(p_W); 


    //double bias=0;
    return W;
}


//------------------------------------------------------
Bool_t ZeeDCalcW::ReadElectrons()
{
    // Reads electrons from event
    DEBUG_MES_START;

    // Get electrons
    fElectrons = fEventInProcess->GetElectrons();
    CHECK_NULL_PTR(fElectrons);

    DEBUG_MES_END;

    return ( fElectrons->GetEntriesFast() > 0 );
}

//------------------------------------------------------
void ZeeDCalcW::ReadEtMiss()
{
    // Reads Et miss from event
    DEBUG_MES_START;

    // Get Et miss
    TObjArray* etMissArray = fEventInProcess->GetEtMissArray();

    if (etMissArray == NULL || etMissArray->GetEntriesFast() <= 0 ) {
        Error("ZeeDCalcW::ReadEtMiss", "Et miss objects are not filled");
        gSystem->Exit(EXIT_FAILURE);
    }

    // NOTE: currently only one etMiss object is filled
    fEtMiss = static_cast<ZeeDEtMiss*>( etMissArray->At(0) );

    DEBUG_MES_END;
}
