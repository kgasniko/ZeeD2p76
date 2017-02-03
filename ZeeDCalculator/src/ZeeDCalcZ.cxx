#include "ZeeDCalculator/ZeeDCalcZ.h"

// Root libraries
#include <TSystem.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TObjArray.h>

// Athena includes
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"

// ZeeD analysis includes
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDMuon.h"
#include "ZeeDCalculator/ZeeDFilters.h"

// Functions:
#include "ZeeDCalculator/ZeeDFunctions.h"

//------------------------------------------------------
ZeeDCalcZ::ZeeDCalcZ() : fSvcHelper("ZeeDCalcZ"),
    fAnaOptions(fSvcHelper.GetAnaSvc())

{
    // Constructor
}

//------------------------------------------------------
ZeeDCalcZ::~ZeeDCalcZ()
{
    // Destructor
}

//------------------------------------------------------
void ZeeDCalcZ::Calculate(ZeeDEvent* event)
{
    // Calculate Z boson candidates

    // Get electrons
    const TObjArray* electrons = event->GetElectrons();
    if (electrons == NULL) {
        Error("ZeeDCalcZ::Calculate", "Electron array is not initialised");
        gSystem->Exit(EXIT_FAILURE);
    }

    // experimental option:
    // in order to accelerate the analysis
    // apply the pre-filtration to the electrons
    // from which the Z-boson is constructed
    if ((*fAnaOptions)->UseZCCElectronFiltration()) {
        electrons = ZeeDFilters::filter(electrons, ZeeDFilters::electronFilter); 
    }

    // Loop over pairs of electrons
    event->ClearZBosons();
    for ( Int_t iOne = 0; iOne < electrons->GetEntriesFast(); ++iOne ) {
        for ( Int_t iTwo = iOne + 1; iTwo < electrons->GetEntriesFast(); ++iTwo ) {
            event->AddZBoson(this->CreateZ(static_cast<ZeeDElectron*> (electrons->At(iOne)),
                        static_cast<ZeeDElectron*> (electrons->At(iTwo))));

        }
    }
    /*
    // Loop over electron-muon pairs to make fake bosons
    if ((*fAnaOptions)->CalcEMu() == true) {
    event->ClearEMuBosons();
    const TObjArray* muons = event->GetMuons();
    for ( Int_t iOne = 0; iOne < electrons->GetEntriesFast(); ++iOne ) {
    for ( Int_t iTwo = 0; iTwo < muons->GetEntriesFast(); ++iTwo ) {

    ZeeDBosonEMu* Z = this->CreateEMu( static_cast<ZeeDElectron*>(electrons->At(iOne)), static_cast<ZeeDMuon*>(muons->At(iTwo)));
    event->AddEMuBoson(Z, 0.);

    }
    }
    }
    */
    // Loop over muon-muon pairs to make Zmumu bosons
    event->ClearZmumuBosons();
    const TObjArray* muons = event->GetMuons();
    for ( Int_t iOne = 0; iOne < muons->GetEntriesFast(); ++iOne ) {
        for ( Int_t iTwo = iOne + 1; iTwo < muons->GetEntriesFast(); ++iTwo ) {

            ZeeDBosonZ* Z = this->CreateZmumu( static_cast<ZeeDMuon*>(muons->At(iOne)), static_cast<ZeeDMuon*>(muons->At(iTwo)));
            event->AddZmumuBoson(Z);

        }
    }

    // experimental feature, see above
    if ((*fAnaOptions)->UseZCCElectronFiltration()) delete electrons;

}

//------------------------------------------------------
ZeeDBosonZ* ZeeDCalcZ::CreateZ(const ZeeDElectron* elecOne, const ZeeDElectron* elecTwo)
{
    // Make Z boson candidate
    ZeeDBosonZ* Z = new ZeeDBosonZ();


    // electrons 4-vectors
    // take the pt of the electrons
    const TLorentzVector& fourVectorOne    = elecOne->GetFourVector();
    const TLorentzVector& fourVectorTwo    = elecTwo->GetFourVector();
    const TLorentzVector& zBosonFourVector = fourVectorOne + fourVectorTwo;
    /*
    // if cluster exists take the cluster variables
    // else take "just electron" variables
    TLorentzVector const* fourVectorOne    = elecOne->Cluster().IsSet() ?
    new TLorentzVector(elecOne->Cluster().Get().fourVector) :
    new TLorentzVector(elecOne->GetFourVector());
    TLorentzVector const* fourVectorTwo    = elecTwo->Cluster().IsSet() ?
    new TLorentzVector(elecTwo->Cluster().Get().fourVector) :
    new TLorentzVector(elecTwo->GetFourVector());
    const TLorentzVector zBosonFourVector = *fourVectorOne + *fourVectorTwo;
    */

    // set 4-vector for the Z boson
    Z->SetFourVector(zBosonFourVector);


    // set Z mass
    double mass2 = zBosonFourVector.M2();
    if (mass2 > 0.) {
        Z->SetMass(TMath::Sqrt(mass2));
    } else {
        Z->SetMass(0.);
    }


    // set net charge of the Z boson
    float charge = 0;
    if(elecOne->getHasCharge()) {
        charge += elecOne->getCharge();
    }
    if(elecTwo->getHasCharge()) {
        charge += elecTwo->getCharge();
    }
    Z->SetCharge(charge);


    // set the Pt-ordered ordering
    if (fourVectorOne.Pt() > fourVectorTwo.Pt()) {
        Z->SetPtLepPair(ZeeDBoson::LepParticlesPair(elecOne, elecTwo));
    } else {
        Z->SetPtLepPair(ZeeDBoson::LepParticlesPair(elecTwo, elecOne));
    }


    // set pos-neg electrons
    Double_t ch1 = (elecOne->getHasCharge()) ? elecOne->getCharge() : 0.0;
    Double_t ch2 = (elecTwo->getHasCharge()) ? elecTwo->getCharge() : 0.0;
    PositiveParticle::Value part = this->GetPositiveParticle(ch1, ch2);
    if (part == ZeeDCalcZ::PositiveParticle::First) {
        Z->SetPosNegLepPair(ZeeDBoson::LepParticlesPair(elecOne, elecTwo));
    } else if (part == ZeeDCalcZ::PositiveParticle::Second) {
        Z->SetPosNegLepPair(ZeeDBoson::LepParticlesPair(elecTwo, elecOne));
    }


    // set eta-ordered electrons
    if( TMath::Abs(fourVectorOne.Eta()) < TMath::Abs(fourVectorTwo.Eta()) ) {
        Z->SetCentralForwardLepPair(ZeeDBoson::LepParticlesPair(elecOne,elecTwo));
    } else {
        Z->SetCentralForwardLepPair(ZeeDBoson::LepParticlesPair(elecTwo,elecOne));
    }


    // set CosTheta* angles
    this->CalcCosThStar(Z);


    // set vertex of the Z boson calculated from the electrons
    // SG: calculate vertex
    TVector3 v1(0.,0.,0.);
    TVector3 v1e(1000.,1000.,1000.);
    TVector3 v2(0.,0.,0.);
    TVector3 v2e(1000.,1000.,1000.);

    if ( elecOne->Vertex().IsSet() ) {
        v1  = elecOne->Vertex().Get().position;
        v1e = elecOne->Vertex().Get().Err2;
    }

    if ( elecTwo->Vertex().IsSet() ) {
        v2  = elecTwo->Vertex().Get().position;
        v2e = elecTwo->Vertex().Get().Err2;
    }

    // Protection agains 0s:
    if ( (v1e.Px() == 0) || (v1e.Py() == 0) || (v1e.Pz() == 0 ) ) {
        // Reset vertex errors:
        v1e.SetXYZ(1000.,1000.,1000.);
        Error("ZeeDCalcZ::CreateZ", " Reset vertex error to 1000 ");
    }

    if ( (v2e.Px() == 0) || (v2e.Py() == 0) || (v2e.Pz() == 0 ) ) {
        // Reset vertex errors to unity:
        v2e.SetXYZ(1000.,1000.,1000.);
        Error("ZeeDCalcZ::CreateZ", " Reset vertex error to 1000 ");
    }

    // Have to do this component by component ...
    TVector3 vZ  = TVector3(
            (v1.Px() / v1e.Px() + v2.Px() / v2e.Px()) / (1 / v1e.Px() + 1 / v2e.Px()),
            (v1.Py() / v1e.Py() + v2.Py() / v2e.Py()) / (1 / v1e.Py() + 1 / v2e.Py()),
            (v1.Pz() / v1e.Pz() + v2.Pz() / v2e.Pz()) / (1 / v1e.Pz() + 1 / v2e.Pz())
            );

    TVector3 vZe = TVector3(
            1 / (1 / v1e.Px() + 1 / v2e.Px()),
            1 / (1 / v1e.Py() + 1 / v2e.Py()),
            1 / (1 / v1e.Pz() + 1 / v2e.Pz())
            );
    Z->SetVertex(vZ);
    Z->SetVerErr2(vZe);

    return Z;
}


//------------------------------------------------------
/*ZeeDBosonEMu* ZeeDCalcZ::CreateEMu(const ZeeDElectron* elec, const ZeeDMuon* muon)
  {
// Make Z boson candidate
ZeeDBosonEMu* Z = new ZeeDBosonEMu();

// set 4-vector for the Z boson
const TLorentzVector fourVectorElectron	= elec->GetFourVector();
/c*
TLorentzVector const* fourVectorElectron	=  elec->Cluster().IsSet() ?
new TLorentzVector(elec->Cluster().Get().fourVector) :
new TLorentzVector(elec->GetFourVector());
 *d/
 const TLorentzVector fourVectorMuon		= muon->GetFourVector();
 const TLorentzVector zBosonFourVector		= fourVectorElectron + fourVectorMuon;


// set 4-vector for the Z boson
Z->SetFourVector(zBosonFourVector);


// set Z mass
double mass2 = zBosonFourVector.M2();
if (mass2 > 0.) {
Z->SetMass(TMath::Sqrt(mass2));
} else {
Z->SetMass(0.);
}


// add electron and muon 4-vectors to the Z boson
// keep the Pt ordering
if (fourVectorElectron.Pt() > fourVectorMuon.Pt()) {
Z->SetPtEMPair(ZeeDBoson::EMParticlesPair(elec, muon));
} else {
Z->SetPtEMPair(ZeeDBoson::EMParticlesPair(muon, elec));
}


// add electrons to the Z boson
// keep the Pt ordering
Z->SetElectron(elec);
Z->SetMuon(muon);


// set net charge of the Z boson
float charge = 0;
if(elec->getHasCharge()) {
charge += elec->getCharge();
}
charge += muon->getCharge();
Z->SetCharge(charge);


// set pos-neg e-mu pair
double ch1 = (elec->getHasCharge()) ? elec->getCharge() : 0.0;
double ch2 = muon->getCharge();
PositiveParticle::Value part = this->GetPositiveParticle(ch1, ch2);
if (part == ZeeDCalcZ::PositiveParticle::First) {
Z->SetPosNegEMPair(ZeeDBoson::EMParticlesPair(elec, muon));
} else if (part == ZeeDCalcZ::PositiveParticle::Second) {
Z->SetPosNegEMPair(ZeeDBoson::EMParticlesPair(muon, elec));
}


// set vertex of the Z boson calculated from the electron and muon
// FIXME: add vertex calculated from the e-mu
TVector3 vZ  = TVector3(0., 0., 0.);
TVector3 vZe = TVector3(1000., 1000., 1000.);
Z->SetVertex(vZ);
Z->SetVerErr2(vZe);


// set CosTheta* angles
this->CalcCosThStar(Z);


return Z;
}
*/

//------------------------------------------------------
ZeeDBosonZ* ZeeDCalcZ::CreateZmumu(const ZeeDMuon* muon1, const ZeeDMuon* muon2)
{
    // Make Z boson candidate
    ZeeDBosonZ* Z = new ZeeDBosonZ();

    // set 4-vector for the Z boson
    const TLorentzVector fourVectorMuon1	= muon1->GetFourVector();
    const TLorentzVector fourVectorMuon2	= muon2->GetFourVector();
    const TLorentzVector zBosonFourVector	= fourVectorMuon1 + fourVectorMuon2;


    // set 4-vector for the Z boson
    Z->SetFourVector(zBosonFourVector);


    // set Z mass
    double mass2 = zBosonFourVector.M2();
    if (mass2 > 0.) {
        Z->SetMass(TMath::Sqrt(mass2));
    } else {
        Z->SetMass(0.);
    }


    // add electron and muon 4-vectors to the Z boson
    // keep the Pt ordering
    if (fourVectorMuon1.Pt() > fourVectorMuon2.Pt()) {
        Z->SetPtLepPair(ZeeDBoson::LepParticlesPair(muon1, muon2));
    } else {
        Z->SetPtLepPair(ZeeDBoson::LepParticlesPair(muon2, muon1));
    }


    // set net charge of the Z boson
    float charge = muon1->getCharge();
    charge += muon2->getCharge();
    Z->SetCharge(charge);


    // set pos-neg e-mu pair
    double ch1 = muon1->getCharge();
    double ch2 = muon2->getCharge();
    PositiveParticle::Value part = this->GetPositiveParticle(ch1, ch2);
    if (part == ZeeDCalcZ::PositiveParticle::First) {
        Z->SetPosNegLepPair(ZeeDBoson::LepParticlesPair(muon1, muon2));
    } else if (part == ZeeDCalcZ::PositiveParticle::Second) {
        Z->SetPosNegLepPair(ZeeDBoson::LepParticlesPair(muon2, muon1));
    }


    // set CosTheta* angles
    this->CalcCosThStar(Z);


    return Z;
}


//------------------------------------------------------
ZeeDCalcZ::PositiveParticle::Value ZeeDCalcZ::GetPositiveParticle(double ch1, double ch2) {

    // Checks if the first particle has positive charge

    // Cases:

    // charge1 > 0 && charge2 < 0		||
    //	charge1 > 0 && charge2 == 0	||
    //	charge1 == 0 && charge2 < 0	:	First

    // charge2 > 0 && charge1 < 0		||
    //	charge2 > 0 && charge1 == 0	||
    //	charge2 == 0 && charge1 < 0	:	Second

    // else: Undefined


    if ( (ch1 > 0 && ch2 < ch1) || (ch1 == 0 && ch2 < 0) ) return PositiveParticle::First;
    if ( (ch2 > 0 && ch1 < ch2) || (ch2 == 0 && ch1 < 0) ) return PositiveParticle::Second;
    return PositiveParticle::Undefined;

}


//------------------------------------------------------
void ZeeDCalcZ::CalcCosThStar(ZeeDBoson* Z) const
{
    if (Z->GetMass() > 1.) { 

        if (Z->GetPositiveLep() != NULL) { // check one particle from the pos-neg pair
            const TLorentzVector* epos = &(Z->GetPositiveLep()->GetFourVector());
            const TLorentzVector* eneg = &(Z->GetNegativeLep()->GetFourVector());

            Z->SetCosThStar(ZeeDFunctions::cosThStarCS(epos, eneg),   ZeeDEnum::CosThStar::CollinsSoper);
            Z->SetCosThStar(ZeeDFunctions::cosThStarZ(epos, eneg),    ZeeDEnum::CosThStar::ZFlightDirection);
            Z->SetCosThStar(ZeeDFunctions::cosThStarBeam(epos, eneg), ZeeDEnum::CosThStar::BeamDirection);
            return;
        }

    } 

    Z->SetCosThStar(-2., ZeeDEnum::CosThStar::CollinsSoper);
    Z->SetCosThStar(-2., ZeeDEnum::CosThStar::ZFlightDirection);
    Z->SetCosThStar(-2., ZeeDEnum::CosThStar::BeamDirection);

}

