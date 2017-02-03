#define DEBUG_ON 0

#include "ZeeDCalculator/ZeeDCalcWmu.h"

// std libraries
#include <string>
#include <iostream>

// Root libraries
#include <TSystem.h>
#include <TMath.h>

// Athena libraries
//#include "GaudiKernel/ServiceHandle.h"

// ZeeD analysis includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDMuon.h"
#include "ZeeDEvent/ZeeDEtMiss.h"
#include "ZeeDEvent/ZeeDBoson.h"

#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDCalcWmu::ZeeDCalcWmu() :
    fMissingEtObj(NULL),
    fMuons(NULL),
    fEventInProcess(NULL),
    fEtMiss(NULL),
    fSvcHelper("ZeeDCalcWmu")
{
    // Constructor

}

//------------------------------------------------------
ZeeDCalcWmu::~ZeeDCalcWmu()
{
    // Destructor
}

//------------------------------------------------------
void ZeeDCalcWmu::Calculate(ZeeDEvent* event)
{
    // Creates W boson candidates
    DEBUG_MES_START;

    fEventInProcess = event;

    // Get array of electrons from event
    const TObjArray* muons = event->GetMuons();
    //std::cout << "Muons = " << muons->GetEntriesFast() <<std::endl;   
    if (muons == NULL) {
        Error("ZeeDCalcWmu::Calculate", "Muon array in not initialised");
        gSystem->Exit(EXIT_FAILURE);
        }

    event->ClearWmuBosons();
    //Double_t eMax=0;
    //int indMax=0;
/*    
    for (int elecIndex=0; elecIndex< electrons->GetEntriesFast(); elecIndex++) {
         ZeeDElectron* electron = static_cast< ZeeDElectron* >(electrons->At(elecIndex));
         TLorentzVector fourVector = electron->GetFourVector();
         
         if (fourVector.E() > eMax){
             eMax=fourVector.E();
             indMax=elecIndex;
             }
         }
    if (eMax != 0)
         event->AddWBoson(this->CreateW((ZeeDElectron*)electrons->At(indMax)));*/
    
    // Select event with exaclty one electron candidate
    // FIXME: one shall consider also more general
    // case with several W candiates


    for (int muIndex=0; muIndex< muons->GetEntriesFast(); muIndex++) {
         ZeeDMuon* mu = static_cast< ZeeDMuon* >(muons->At(muIndex));
         if (mu == NULL) return;
         TLorentzVector fourVector = mu->GetFourVector();
         //std::cout << fourVector.Px() << " "<<fourVector.E() << std::endl;
         event->AddWmuBoson(this->CreateWmu(mu));         
    }
 
    DEBUG_MES_END;
    
}

ZeeDBosonW* ZeeDCalcWmu::CreateWmu (const ZeeDMuon* mu){
    //std::cout << "Here!!" <<std::endl; 
    ZeeDBosonW* W = new ZeeDBosonW;
    
    W->SetLepton(mu);
    
    const TLorentzVector mu4Vec = mu->GetFourVector();

    Double_t muPt  = mu4Vec.Pt();
    Double_t muPhi = mu4Vec.Phi();
    
    TObjArray* etMissArray = fEventInProcess->GetEtMissArray();

    if (etMissArray == NULL || etMissArray->GetEntriesFast() <= 0 ) {
        Error("ZeeDCalcW::ReadEtMiss", "Et miss objects are not filled");
        gSystem->Exit(EXIT_FAILURE);
        }
    //you NEED to make it with all of ETmiss
    /*        
    for (int iEM=0; iEM< etMissArray->GetEntriesFast(); iEM++){
        ZeeDEtMiss* fEtMiss= (ZeeDEtMiss*) etMissArray->At(iEM);
        W->SetEtMissPtr(etM);
        
        // Neutrino
        Double_t neutrinoPt  = fEtMiss->GetEt();
        Double_t neutrinoPhi = fEtMiss->GetPhi();
    */
    //std::cout << " Number of EtMiss in event = " << etMissArray->GetEntriesFast() <<std::endl;
    ZeeDEtMiss* fEtMiss= (ZeeDEtMiss*) etMissArray->At(0);
    W->SetEtMissPtr(fEtMiss);    
    
    // Neutrino
    Double_t neutrinoPt  = fEtMiss->GetEt();
    Double_t neutrinoPhi = fEtMiss->GetPhi();

    // Calculate W's transverse mass
    // MT = sqrt ( 2 p_T^e p_T^nu ( 1 - cos delta phi))
    Double_t Mt = 2 * muPt * neutrinoPt;
    Mt *= 1 - TMath::Cos(muPhi - neutrinoPhi);
    Mt = TMath::Sqrt(Mt);

    // Make transverse neutrino 4-vector
    TLorentzVector p_nu(fEtMiss->GetEtX(), fEtMiss->GetEtY(), 0., fEtMiss->GetEt());

    // Make transverse electron  4-vector
    TLorentzVector p_mu = mu4Vec;
    p_mu.SetE( mu4Vec.Et() );
    p_mu.SetPz(0.);

    // Reconstruct (transverse) W 4-momentum
    TLorentzVector p_W = p_nu + p_mu;    
    
    W->SetMt(Mt);
    W->SetFourVector(p_W);
    
    return W;
}


//------------------------------------------------------
Bool_t ZeeDCalcWmu::ReadMuons()
{
    // Reads electrons from event
    DEBUG_MES_START;

    // Get electrons
    fMuons = fEventInProcess->GetMuons();
    CHECK_NULL_PTR(fMuons);

    DEBUG_MES_END;

    return ( fMuons->GetEntriesFast() > 0 );
}

//------------------------------------------------------
void ZeeDCalcWmu::ReadEtMiss()
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
