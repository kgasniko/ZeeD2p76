#include "ZeeDCalculator/ZeeDReadGenAOD.h"
// Root libraries
#include <TSystem.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TObjArray.h>

// Athena includes
#include "McParticleEvent/TruthParticleContainer.h"
#include "StoreGate/StoreGateSvc.h"

// ZeeD analysis includes
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDGenParticle.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

//------------------------------------------------------
ZeeDReadGenAOD::ZeeDReadGenAOD() :
    fMcEvent(0),
    fEventWeight(1.0),
    fSvcHelper("ZeeDReadGenAOD"),
    fAnaOptions(fSvcHelper.GetAnaSvc())
{
    // Constructor

    // Initalize services
    StatusCode sc = this->Initialize();

    if ( sc.isFailure() ) {
        Error("ZeeDReadGenAOD::ZeeDReadGenAOD", "Failed to initialize object");
        gSystem->Exit(EXIT_FAILURE);
    }
}

//------------------------------------------------------
ZeeDReadGenAOD::~ZeeDReadGenAOD()
{
    // Destructor
}

//------------------------------------------------------
StatusCode ZeeDReadGenAOD::Initialize()
{
    // Initializer
    DEBUG_MES_START;

    ServiceHandle<StoreGateSvc>* storeGate = fSvcHelper.GetStoreGate();

    fAnaOptions = fSvcHelper.GetAnaSvc();

    StatusCode sc = (*storeGate)->retrieve(fMcEvent, "GEN_AOD");

    if( sc.isFailure()  || !fMcEvent ) {
        Error("ZeeDReadGenAOD::Initalize",
              "No GEN_AOD Truth MC Collection found");
        return sc;
    }

    fEventWeight = 1.0;

    DEBUG_MES_END;

    return sc;
}

//------------------------------------------------------
void ZeeDReadGenAOD::ReadInfo(ZeeDEvent* event)
{
  // Reads generated information
  DEBUG_MES_START;

  StatusCode sc = this->Initialize();

  if ( sc.isFailure() ) {
    Error("ZeeDReadGenAOD::ReadInfo", "Failed to initialize object");
    gSystem->Exit(EXIT_FAILURE);
  }

  // Get all the info about the "hard" process from the first
  // sub-event inside the mc event.
  // Do not search for the Z boson in the "pile-up" (not first) events.
  const HepMC::GenEvent* genEvt = fMcEvent->front();
  if (!genEvt) return;

  // Read PDF information -------------
  const HepMC::PdfInfo* pdfInfo = genEvt->pdf_info();
  if (pdfInfo) {
    ZeeDEventGenBags::PDF PDFBag;
    PDFBag.id1      = pdfInfo->id1();
    PDFBag.id2      = pdfInfo->id2();
    PDFBag.x1       = pdfInfo->x1();
    PDFBag.x2       = pdfInfo->x2();
    PDFBag.scalePDF = pdfInfo->scalePDF();
    PDFBag.pdf1     = pdfInfo->pdf1();
    PDFBag.pdf2     = pdfInfo->pdf2();
    event->PDF().Set(PDFBag);
    event->PDF().Lock();
  }

  // Read gen vertex information ---------
  const HepMC::GenVertex* vtx = (*genEvt->vertices_begin());
  if( vtx && event->Vertex().IsSet()==false ){
    ZeeDEventGenBags::Vertex vtxBag;
    HepMC::ThreeVector vtxpos = vtx->point3d();

    vtxBag.position.SetXYZ(vtxpos.x(), vtxpos.y(), vtxpos.z());
    event->Vertex().Set(vtxBag);
    event->Vertex().Lock();
  } 

  Int_t mcChannelNumber = event->MCChannelNumber().Get();

  //loop over the particles in the event
  for (HepMC::GenEvent::particle_const_iterator p = genEvt->particles_begin(); p != genEvt->particles_end(); p++) {
    const HepMC::GenParticle* particle = *p;

    CHECK_NULL_PTR(particle);

    if (ZeeDMisc::IsSherpa(mcChannelNumber)){

      // start from intermediate electron in decay chain e(born) -> e(intermediate) -> e(final)
      if (( particle->pdg_id() == 11 ) &&  (particle->status() == 11 ) ){

	TObjArray* GenZ  = event->GetGenBosons();
	TObjArray* GenElec  = event->GetGenLeptons();
	TObjArray* GenGamma = event->GetGenGammas();

	this->AddDaughtersSherpa(particle, GenZ, GenElec, GenGamma);
      }
    }
    else if (ZeeDMisc::IsAlpgenHerwig(mcChannelNumber)){

      // add all particles with pdg = 23 (Z boson)
      if ( particle->pdg_id() == 23 ) {

	ZeeDGenParticle* genZboson = new ZeeDGenParticle();

	const HepMC::FourVector& fourVector = particle->momentum();
	const Double_t E  = fourVector.e()  / GeV;
	const Double_t Px = fourVector.px() / GeV;
	const Double_t Py = fourVector.py() / GeV;
	const Double_t Pz = fourVector.pz() / GeV;

	genZboson->SetMCFourVector( TLorentzVector(Px, Py, Pz, E) );
	genZboson->SetParticleID( particle->pdg_id() );
//	genZboson->SetParticleStatus( particle->status());

	TObjArray* GenZ  = event->GetGenBosons();
	TObjArray* GenElec  = event->GetGenLeptons();
	TObjArray* GenGamma = event->GetGenGammas();

	// add all generated Z bosons to the event
	GenZ->AddLast(genZboson);
	
	//if bare Z boson
	if (particle->status() == 155)
	  this->AddDaughtersAH(particle, genZboson, GenZ,  GenElec, GenGamma);

      } //endif Zboson
      
    } //endif AlpgenHerwig
    else {

      // add all particles with pdg = 23 (Z boson)
      if ( particle->pdg_id() == 23 ) {
	
	ZeeDGenParticle* genZboson = new ZeeDGenParticle();

	const HepMC::FourVector& fourVector = particle->momentum();
	const Double_t E  = fourVector.e()  / GeV;
	const Double_t Px = fourVector.px() / GeV;
	const Double_t Py = fourVector.py() / GeV;
	const Double_t Pz = fourVector.pz() / GeV;

	genZboson->SetMCFourVector( TLorentzVector(Px, Py, Pz, E) );
	genZboson->SetParticleID( particle->pdg_id() );
//	genZboson->SetParticleStatus( particle->status());

	// add genZ to the event
	(event->GetGenBosons())->AddLast(genZboson);

	// Add Daughters to this Z:
	TObjArray* GenElec  = event->GetGenLeptons();
	TObjArray* GenGamma = event->GetGenGammas();

	this->AddDaughters(particle, genZboson, GenElec, GenGamma);
      }

    }

  }

  DEBUG_MES_END;
}


//------------------------------------------------------

void ZeeDReadGenAOD::AddDaughters(const HepMC::GenParticle* particle,
                                  ZeeDGenParticle* genZboson, TObjArray* GenElec, TObjArray* GenGamma)
{
    // Get a decay vertex:

    CHECK_NULL_PTR(particle);

    const HepMC::GenVertex* vtx = particle->end_vertex();
    // CHECK_NULL_PTR(vtx);
    if (vtx == NULL) {
        Info("ZeeDReadGenAOD::AddDaughters", "No vertex");
        return;
    }

    const HepMC::GenParticle* child = 0;

    for (HepMC::GenVertex::particles_out_const_iterator pchild = vtx->particles_out_const_begin();
            pchild != vtx->particles_out_const_end(); ++pchild) {

        child = *pchild;

        Int_t pid = child->pdg_id();
        Int_t stat = child->status();

        if ( 0 != child ) {
            if ((TMath::Abs(pid) == 11) || (pid == 22)) {
                if ( ! (stat == 2) )  {
                    ZeeDGenParticle* gParticle = new ZeeDGenParticle();

                    const HepMC::FourVector& fourVector = child->momentum();
                    const Double_t E  = fourVector.e()  / GeV;
                    const Double_t Px = fourVector.px() / GeV;
                    const Double_t Py = fourVector.py() / GeV;
                    const Double_t Pz = fourVector.pz() / GeV;

                    gParticle->SetMCFourVector( TLorentzVector(Px, Py, Pz, E) );

                    gParticle->SetParent(genZboson);
                    genZboson->AddDaughter(gParticle);
                    gParticle->SetParticleID( child->pdg_id() );
//                    gParticle->SetParticleStatus( child->status() );

                    if (TMath::Abs(pid) == 11) {
                        GenElec->AddLast(new ZeeDGenParticle(*gParticle));			
                    } else {
                        GenGamma->AddLast(new ZeeDGenParticle(*gParticle));
                    }

                } else { // status 2
		  AddDaughters(child, genZboson, GenElec, GenGamma);
                }

            } // endif  electrons or photons

        } // endif no child

    } // end of the loop at the decays vertex

    DEBUG_MES_END;
}

//------------------------------------------------------

void ZeeDReadGenAOD::AddDaughtersAH(const HepMC::GenParticle* particle,
				    ZeeDGenParticle* genZboson,  TObjArray* GenZ, TObjArray* GenElec, TObjArray* GenGamma)
{
  // Get a decay vertex:
  CHECK_NULL_PTR(particle);

  const HepMC::GenVertex* vtx = particle->end_vertex();
  // CHECK_NULL_PTR(vtx);
  if (vtx == NULL) {
    Info("ZeeDReadGenAOD::AddDaughters", "No vertex");
    return;
  }

  const HepMC::GenParticle* child = 0;
  TLorentzVector fakeZbosonMomentum ;
  ZeeDGenParticle* fakeZboson = new ZeeDGenParticle();

  int fakeZbosonStatus = 17;
  int fakeZbosonID = 23;

  for (HepMC::GenVertex::particles_out_const_iterator pchild = vtx->particles_out_const_begin();
       pchild != vtx->particles_out_const_end(); ++pchild) {

    child = *pchild;

    Int_t pid = child->pdg_id();

    if ( 0 != child ) {
      if ((TMath::Abs(pid) == 11) || (pid == 22)) {
	ZeeDGenParticle* gParticle = new ZeeDGenParticle();

	const HepMC::FourVector& fourVector = child->momentum();
	const Double_t E  = fourVector.e()  / GeV;
	const Double_t Px = fourVector.px() / GeV;
	const Double_t Py = fourVector.py() / GeV;
	const Double_t Pz = fourVector.pz() / GeV;

	gParticle->SetMCFourVector( TLorentzVector(Px, Py, Pz, E) );

	gParticle->SetParent(genZboson);
	genZboson->AddDaughter(gParticle);
	gParticle->SetParticleID( child->pdg_id() );
//	gParticle->SetParticleStatus( child->status() );

	if (TMath::Abs(pid) == 11) {
	  GenElec->AddLast(new ZeeDGenParticle(*gParticle));

	  if ( child -> status() == 123 || child -> status() == 124 ){
	    const HepMC::GenVertex* elec_vtx = child ->end_vertex();
	    if (elec_vtx == NULL) {
	      return;
	    }
			  
	    for (HepMC::GenVertex::particles_out_const_iterator elec_child = elec_vtx -> particles_out_const_begin();
		 elec_child != elec_vtx -> particles_out_const_end(); ++elec_child) {

	      if ((*elec_child) -> status() == 1 && abs((*elec_child) -> pdg_id()) == 11){
		ZeeDGenParticle* bare_electron = new ZeeDGenParticle();

		const HepMC::FourVector& fourVector = (*elec_child)->momentum();
		const Double_t E  = fourVector.e()  / GeV;
		const Double_t Px = fourVector.px() / GeV;
		const Double_t Py = fourVector.py() / GeV;
		const Double_t Pz = fourVector.pz() / GeV;

		fakeZbosonMomentum +=  TLorentzVector(Px, Py, Pz, E);

		bare_electron->SetMCFourVector( TLorentzVector(Px, Py, Pz, E) );
		bare_electron->SetParent(fakeZboson);
		fakeZboson->AddDaughter(bare_electron);
		bare_electron->SetParticleID( (*elec_child)->pdg_id() );
//		bare_electron->SetParticleStatus( (*elec_child)->status() );
		GenElec->AddLast(new ZeeDGenParticle(*bare_electron));
	      }
	    }
	  }		
	} else {
	  GenGamma->AddLast(new ZeeDGenParticle(*gParticle));
	}

      } // endif  electrons or photons

    } // endif no child

  } // end of the loop at the decays vertex

  fakeZboson->SetParticleID( fakeZbosonID );
//  fakeZboson->SetParticleStatus( fakeZbosonStatus );
  fakeZboson->SetMCFourVector( fakeZbosonMomentum );
  GenZ->AddLast(fakeZboson);

  DEBUG_MES_END;
}
//------------------------------------------------------

void ZeeDReadGenAOD::AddDaughtersSherpa(const HepMC::GenParticle* particle,
					TObjArray* GenZ, TObjArray* GenElec, TObjArray* GenGamma)
{
  // Get a decay vertex:
  CHECK_NULL_PTR(particle);

  const HepMC::GenVertex* production_vtx = particle->production_vertex();
  if (production_vtx == NULL) {
    Info("ZeeDReadGenAOD::AddDaughtersSherpa", "No production vertex");
    return;
  }

  const HepMC::GenVertex* end_vtx = particle->end_vertex();
  if (end_vtx == NULL) {
    Info("ZeeDReadGenAOD::AddDaughtersSherpa", "No decay vertex");
    return;
  }

  // create new gen particles born/bare Z boson 
  TLorentzVector fakeBornZbosonMomentum ;
  TLorentzVector fakeBareZbosonMomentum ;

  ZeeDGenParticle* fakeBornZboson = new ZeeDGenParticle();
  ZeeDGenParticle* fakeBareZboson = new ZeeDGenParticle();

  int fakeBornZbosonStatus = 17;
  int fakeBareZbosonStatus = 19;
  int fakeZbosonID = 23;

  // born electrons
  for (HepMC::GenVertex::particles_in_const_iterator elec_parent = production_vtx -> particles_in_const_begin();
       elec_parent != production_vtx -> particles_in_const_end(); ++elec_parent) {
    Int_t pid = (*elec_parent)->pdg_id();
    Int_t status = (*elec_parent)->status();

    if ( 0 != (*elec_parent) ) {
      if ( (TMath::Abs(pid) == 11) && (status==3) ) {

	ZeeDGenParticle* gParticle = new ZeeDGenParticle();
	const HepMC::FourVector& fourVector = (*elec_parent)->momentum();
	const Double_t E  = fourVector.e()  / GeV;
	const Double_t Px = fourVector.px() / GeV;
	const Double_t Py = fourVector.py() / GeV;
	const Double_t Pz = fourVector.pz() / GeV;

	fakeBornZbosonMomentum += TLorentzVector(Px, Py, Pz, E);

	gParticle->SetParent(fakeBornZboson);
	fakeBornZboson->AddDaughter(gParticle);

	gParticle->SetMCFourVector( TLorentzVector(Px, Py, Pz, E) );
	gParticle->SetParticleID( pid );
//	gParticle->SetParticleStatus( status );

	GenElec->AddLast(new ZeeDGenParticle(*gParticle));
      }
    }
  }

  //bare electrons + photons
  for (HepMC::GenVertex::particles_out_const_iterator elec_child = end_vtx -> particles_out_const_begin();
       elec_child != end_vtx -> particles_out_const_end(); ++elec_child) {
    Int_t pid = (*elec_child)->pdg_id();
    Int_t status = (*elec_child)->status();

    if ( 0 != (*elec_child) ) {
      if (((TMath::Abs(pid) == 11) || (pid == 22)) && (status==1)) {

	ZeeDGenParticle* gParticle = new ZeeDGenParticle();
	const HepMC::FourVector& fourVector = (*elec_child)->momentum();
	const Double_t E  = fourVector.e()  / GeV;
	const Double_t Px = fourVector.px() / GeV;
	const Double_t Py = fourVector.py() / GeV;
	const Double_t Pz = fourVector.pz() / GeV;

	fakeBareZbosonMomentum += TLorentzVector(Px, Py, Pz, E);

	gParticle->SetParent(fakeBareZboson);
	fakeBareZboson->AddDaughter(gParticle);

	gParticle->SetMCFourVector( TLorentzVector(Px, Py, Pz, E) );
	gParticle->SetParticleID( pid );
//	gParticle->SetParticleStatus( status );

	if (TMath::Abs(pid) == 11) {
	  GenElec->AddLast(new ZeeDGenParticle(*gParticle));
	}
	if (TMath::Abs(pid) == 22) {
	  GenGamma->AddLast(new ZeeDGenParticle(*gParticle));
	}
      }
    }
  }

  fakeBornZboson->SetParticleID( fakeZbosonID );
//  fakeBornZboson->SetParticleStatus( fakeBornZbosonStatus );
  fakeBornZboson->SetMCFourVector( fakeBornZbosonMomentum );
  GenZ->AddLast( fakeBornZboson );

  fakeBareZboson->SetParticleID( fakeZbosonID );
//  fakeBareZboson->SetParticleStatus( fakeBareZbosonStatus );
  fakeBareZboson->SetMCFourVector( fakeBareZbosonMomentum );
  GenZ->AddLast( fakeBareZboson );

  DEBUG_MES_END;
}
