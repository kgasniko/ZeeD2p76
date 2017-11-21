#include "ZeeDEvent/ZeeDEvent.h"
// ZeeD analysis includes
#include "ZeeDEvent/ZeeDBoson.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDEtMiss.h"
#include "ZeeDEvent/ZeeDJet.h"
#include "ZeeDEvent/ZeeDTrack.h"
#include "ZeeDEvent/ZeeDVertex.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDBosonEMu.h"
#include "ZeeDEvent/ZeeDJet.h"
#include "ZeeDEvent/ZeeDMuon.h"
#include "ZeeDEvent/ZeeDMuonSpShower.h"
#include "ZeeDEvent/ZeeDBosonW.h"
#include "ZeeDEvent/ZeeDGenParticle.h"
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDEvent/ZeeDPhoton.h"
#include "ZeeDEvent/ZeeDTau.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

//------------------------------------------------------
ZeeDEvent::ZeeDEvent() : fElecArray(NULL),
    fEtMissArray(NULL),
    fVertexArray(NULL),
    fZBosArray(NULL),
    fZmumuBosArray(NULL),
    fJetCollectionArray(NULL),
    fMuonArray(NULL),
    fMuonSpShowerArray(NULL),
    fWBosArray(NULL),
    fWmuBosArray(NULL),
    fGenZBosArray(NULL),
    fGenElecArray(NULL),
    fGenGammaArray(NULL),
    fPhotonArray(NULL),
    fTauArray(NULL),
    fNvtx(0),
    fNpvJets(0),
    fRunNumber(0),
    fEventNumber(0),
    fLB(0),
    fEventWeight(1.0),
    fGenWeight(1.0),
    fEtDensity_3EM(0),
    fEtDensity_4EM(0),
    fEtDensity_3LC(0),
    fEtDensity_4LC(0),
    fCurrentBoson(NULL),
    fCurrentW(NULL),
    fCurrentJet(NULL),
    fGenBornBoson(NULL),
    fGenBareBoson(NULL),
    fGenDressedBoson(NULL),
    fGenBornLeptons(ZeeDGenElectrons(NULL,NULL)),
    fGenBareLeptons(ZeeDGenElectrons(NULL,NULL)),
    fGenDressedLeptons(ZeeDGenElectrons(NULL,NULL)),
    fIsData(kFALSE),
    fRawEvent(NULL),
    fSvcHelper("ZeeDEvent"),
    numWGood(-1),
    numWmuGood(-1),
    fAnaOptions(fSvcHelper.GetAnaSvc())
{
    // Constructor
    DEBUG_MES_START;
    //fCurrentWmuBoson = NULL;
    //fCurrentW = NULL;
    //std::cout << " New Event STARTED! " << std::endl;
    fElecArray = new TObjArray();
    toBeDeleted.Add(fElecArray);

    fTrackArray = new TObjArray();
    toBeDeleted.Add(fTrackArray);

    fEtMissArray = new TObjArray();
    toBeDeleted.Add(fEtMissArray);

    fVertexArray = new TObjArray();
    toBeDeleted.Add(fVertexArray);

    fZBosArray = new TObjArray();
    toBeDeleted.Add(fZBosArray); // comment for TClonesArray

    //fEMuBosArray = new LinkWeightContainer<ZeeDBosonEMu*>();

    fZmumuBosArray = new TObjArray();
    toBeDeleted.Add(fZmumuBosArray);

    fJetCollectionArray = new THashTable();
    toBeDeleted.Add(fJetCollectionArray);

    fMuonArray = new TObjArray();
    toBeDeleted.Add(fMuonArray);

    fMuonSpShowerArray = new TObjArray();
    toBeDeleted.Add(fMuonSpShowerArray);

    fWBosArray = new TObjArray();
    toBeDeleted.Add(fWBosArray);
   
    fWmuBosArray = new TObjArray();
    toBeDeleted.Add(fWmuBosArray);

    fGenZBosArray = new TObjArray();
    toBeDeleted.Add(fGenZBosArray);

    fGenElecArray = new TObjArray();
    toBeDeleted.Add(fGenElecArray);

    fGenGammaArray = new TObjArray();
    toBeDeleted.Add(fGenGammaArray);

    fPhotonArray = new TObjArray();
    toBeDeleted.Add(fPhotonArray);

    fTauArray = new TObjArray();
    toBeDeleted.Add(fTauArray);
     
    //toBeDeleted.Add(fCurrentW);

    for (Int_t i = 0; i < toBeDeleted.GetEntriesFast(); ++i) {
        TObjArray* objArray = static_cast<TObjArray*>( toBeDeleted.At(i) );
        objArray->SetOwner(kTRUE);
    }


    DEBUG_MES_END;
}

//------------------------------------------------------
ZeeDEvent::ZeeDEvent(const RawEvent& ev): TObject(ev)
{
    // Copy constructor

    DEBUG_MES_START;

    toBeDeleted.Clear();
    toBeDeleted.Compress();

    // Electrons
    fElecArray = new TObjArray();

    for (Int_t i = 0; i < ev.fElecArray->GetEntriesFast(); ++i) {
        ZeeDElectron* elec = static_cast<ZeeDElectron*>( ev.fElecArray->At(i));
        fElecArray->AddLast( new ZeeDElectron( *elec ) );
    }
    toBeDeleted.Add(fElecArray);

    // Tracks
    fTrackArray = new TObjArray();

    for (Int_t i = 0; i < ev.fTrackArray->GetEntriesFast(); ++i) {
        ZeeDTrack* track = static_cast<ZeeDTrack*>( ev.fTrackArray->At(i));
        fTrackArray->AddLast( new ZeeDTrack( *track ) );
    }

    toBeDeleted.Add(fTrackArray);

    // Missing Et
    fEtMissArray = new TObjArray();

    for (Int_t i = 0; i < ev.fEtMissArray->GetEntriesFast(); ++i) {
        ZeeDEtMiss* etMiss = static_cast<ZeeDEtMiss*>( ev.fEtMissArray->At(i));
        fEtMissArray->AddLast( new ZeeDEtMiss( *etMiss ) );
    }

    toBeDeleted.Add(fEtMissArray);

    fAnaOptions = fSvcHelper.GetAnaSvc();

    // Jets
    fJetCollectionArray = new THashTable();

    TIter next(ev.fJetCollectionArray);
    while (TObjArray* ev_tja  = (TObjArray*)next()) {

        TObjArray* tja = new TObjArray();
        tja->SetName(ev_tja->GetName());
        tja->SetOwner(kTRUE);

	for (Int_t i = 0; i < ev_tja->GetEntriesFast(); ++i) {
            ZeeDJet* jet =static_cast<ZeeDJet*>( ev_tja->At(i) ) ;
            tja->AddLast( new ZeeDJet(*jet) );
        }

        fJetCollectionArray->Add(tja);

    }

    toBeDeleted.Add(fJetCollectionArray);

    // Muons
    fMuonArray = new TObjArray();

    for (Int_t i = 0; i < ev.fMuonArray->GetEntriesFast(); ++i) {
        ZeeDMuon* muon = static_cast<ZeeDMuon*>( ev.fMuonArray->At(i));
        fMuonArray->AddLast( new ZeeDMuon( *muon ) );
    }

    toBeDeleted.Add(fMuonArray);

    // MuonSpShowers
    fMuonSpShowerArray = new TObjArray();

    for (Int_t i = 0; i < ev.fMuonSpShowerArray->GetEntriesFast(); ++i) {
        ZeeDMuonSpShower* muonspshower = static_cast<ZeeDMuonSpShower*>( ev.fMuonSpShowerArray->At(i));
        fMuonSpShowerArray->AddLast( new ZeeDMuonSpShower( *muonspshower ) );
    }

    toBeDeleted.Add(fMuonSpShowerArray);

    // Verticies
    fVertexArray = new TObjArray();

    for (Int_t i = 0; i < ev.fVertexArray->GetEntriesFast(); ++i) {
        ZeeDVertex* vertex = static_cast<ZeeDVertex*>( ev.fVertexArray->At(i));
        fVertexArray->AddLast( new ZeeDVertex( *vertex ) );
    }

    toBeDeleted.Add(fVertexArray);

    // Z bosons
    fZBosArray = new TObjArray();

    for (Int_t i = 0; i < ev.fZBosArray->GetEntriesFast(); ++i) {
        ZeeDBosonZ* bos = static_cast<ZeeDBosonZ*>( ev.fZBosArray->At(i));
        fZBosArray->AddLast( new ZeeDBosonZ( *bos ) );
    }

    toBeDeleted.Add(fZBosArray); // comment for TClonesArray
/*
    // EMu fake bosons
    if ((*fAnaOptions)->CalcEMu()) {
      fEMuBosArray = new LinkWeightContainer<ZeeDBosonEMu*>();
      fEMuBosArray->DeepCopy(ev.fEMuBosArray);
    }
*/
    // Zmumu bosons
    fZmumuBosArray = new TObjArray();

    for (Int_t i = 0; i < ev.fZmumuBosArray->GetEntriesFast(); ++i) {
        ZeeDBosonZ* bos = static_cast<ZeeDBosonZ*>( ev.fZmumuBosArray->At(i));
        fZmumuBosArray->AddLast( new ZeeDBosonZ( *bos ) );
    }

    toBeDeleted.Add(fZmumuBosArray);


     // W bosons
    fWBosArray = new TObjArray();

    for (Int_t i = 0; i < ev.fWBosArray->GetEntriesFast(); ++i) {
        ZeeDBosonW* bos = static_cast<ZeeDBosonW*>( ev.fWBosArray->At(i));
        fWBosArray->AddLast( new ZeeDBosonW( *bos ) );
    }

    toBeDeleted.Add(fWBosArray);

    // W mu bosons    
    
    fWmuBosArray = new TObjArray();

    for (Int_t i = 0; i < ev.fWmuBosArray->GetEntriesFast(); ++i) {
        ZeeDBosonW* bos = static_cast<ZeeDBosonW*>( ev.fWmuBosArray->At(i));
        fWmuBosArray->AddLast( new ZeeDBosonW( *bos ) );
    }

    toBeDeleted.Add(fWmuBosArray);

    // Gen Z bosons
    fGenZBosArray = new TObjArray();

    for (Int_t i = 0; i < ev.fGenZBosArray->GetEntriesFast(); ++i) {
        ZeeDGenParticle* genParticle =
            static_cast<ZeeDGenParticle*>( ev.fGenZBosArray->At(i));
        fGenZBosArray->AddLast( new ZeeDGenParticle( *genParticle ) );
    }

    toBeDeleted.Add(fGenZBosArray);

    // Gen Electrons
    fGenElecArray = new TObjArray();

    for (Int_t i = 0; i < ev.fGenElecArray->GetEntriesFast(); ++i) {
        ZeeDGenParticle* genParticle =
            static_cast<ZeeDGenParticle*>( ev.fGenElecArray->At(i));
        fGenElecArray->AddLast( new ZeeDGenParticle( *genParticle ) );
    }

    toBeDeleted.Add(fGenElecArray);

    // Gen photons
    fGenGammaArray = new TObjArray();

    for (Int_t i = 0; i < ev.fGenGammaArray->GetEntriesFast(); ++i) {
        ZeeDGenParticle* genParticle =
            static_cast<ZeeDGenParticle*>( ev.fGenGammaArray->At(i));
        fGenGammaArray->AddLast( new ZeeDGenParticle( *genParticle ) );
    }

    toBeDeleted.Add(fGenGammaArray);

    // Photons
    fPhotonArray = new TObjArray();

    for (Int_t i = 0; i < ev.fPhotonArray->GetEntriesFast(); ++i) {
        ZeeDPhoton* photon =
            static_cast<ZeeDPhoton*>( ev.fPhotonArray->At(i));
        fPhotonArray->AddLast( new ZeeDPhoton( *photon ) );
    }

    toBeDeleted.Add(fPhotonArray);

    // Taus
    fTauArray = new TObjArray();

    for (Int_t i = 0; i < ev.fTauArray->GetEntriesFast(); ++i) {
        ZeeDTau* tau =
            static_cast<ZeeDTau*>( ev.fTauArray->At(i));
        fTauArray->AddLast( new ZeeDTau( *tau ) );
    }

    toBeDeleted.Add(fTauArray);


    // SetOwner kTRUE
    for (Int_t i = 0; i < toBeDeleted.GetEntriesFast(); ++i) {
        TObjArray* objArray = static_cast<TObjArray*>( toBeDeleted.At(i) );
        objArray->SetOwner(kTRUE);
    }

    fNvtx         = ev.fNvtx;
    fNpvJets      = ev.fNpvJets;
    fRunNumber    = ev.fRunNumber;
    fOrigRunNumber = ev.fOrigRunNumber;
    fEventNumber  = ev.fEventNumber;
    fLB           = ev.fLB;
    fActualInteractionsPerCrossing  = ev.fActualInteractionsPerCrossing;
    fAverageInteractionsPerCrossing = ev.fAverageInteractionsPerCrossing;
    fMCChannelNumber = ev.fMCChannelNumber;
    fMCtype       = ev.fMCtype;
    fEventWeight  = ev.fEventWeight;
    fGenWeight    = ev.fGenWeight;

    fEtDensity_3EM = ev.fEtDensity_3EM;
    fEtDensity_4EM = ev.fEtDensity_4EM;
    fEtDensity_3LC = ev.fEtDensity_3LC;
    fEtDensity_4LC = ev.fEtDensity_4LC;

    fCurrentBoson = ev.fCurrentBoson;
    fCurrentW     = ev.fCurrentW;

    fGenBornBoson = ev.fGenBornBoson;
    fGenBareBoson = ev.fGenBareBoson;
    fGenDressedBoson = ev.fGenDressedBoson;
    fGenBornLeptons = ev.fGenBornLeptons;
    fGenBareLeptons = ev.fGenBareLeptons;
    fGenDressedLeptons = ev.fGenDressedLeptons;
    fIsData       = ev.fIsData;
    fTrigger      = ev.fTrigger;
    fIsInGRL      = ev.fIsInGRL;
    fLArErrorState = ev.fLArErrorState;
    fPDF          = ev.PDF();
    fVertex       = ev.Vertex();
    numWGood=0;

    // Pointer to the Raw event:
    fRawEvent          = &ev;

    //    fAnaOptions = fSvcHelper.GetAnaSvc();
 
    DEBUG_MES_END;
}

//------------------------------------------------------
ZeeDEvent::~ZeeDEvent()
{
    // Destructor
    DEBUG_MES_START;

    // Remove all objects from the array AND delete all heap based objects.
    toBeDeleted.Delete();

	if ( fGenDressedLeptons.first != NULL ) {
        delete  fGenDressedLeptons.first ;
    }

    if ( fGenDressedLeptons.second != NULL ) {
        delete  fGenDressedLeptons.second ;
    }

    // Remove dressed boson
    if ( fGenDressedBoson != NULL ) {
        delete fGenDressedBoson ;        
    }

    // Remove bare boson
    if ( fGenBareBoson != NULL ) {
        std::cout << fGenBareBoson->GetDaughters()->GetSize();
        delete fGenBareBoson ;        
    }

 
    //delete fZBosArray; // uncomment for TClonesArray

    // DO NOT Remove the raw event and its objects!
    // if (fRawEvent != NULL) { delete fRawEvent;    }

   /* 
    if ((*fAnaOptions)->CalcEMu()) {
      delete fEMuBosArray;
    }
*/
    
    DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDEvent::SetCurrentBoson(const ZeeDBosonZ* selectedBoson)
{
    // Set's current Z candidate.
    fCurrentBoson = selectedBoson;
}

//-------------------------------------------------------

void ZeeDEvent::SetCurrentBosonW(const ZeeDBosonW* selectedBoson)
{
    //Set's current W candidate.
    //fCurrentW = (ZeeDBosonW*) fWBosArray->At(0);
    fCurrentW = selectedBoson;
    /*if( fCurrentW != NULL ) {
        std::cout << fCurrentW->GetMt() << "   " ;
        std::cout << fCurrentW->GetElec() << std::endl;
        }
    else {
         std::cout << "NULL" << std::endl;
         }*/

}



//-------------------------------------------------------
void ZeeDEvent::SetCurrentJet(const ZeeDJet* selectedJet)
{
    // Set's current Jet
    fCurrentJet = selectedJet;
}

void ZeeDEvent::SetCurrentTrack(const ZeeDTrack* selectedTrack)
{
    // Set's current Jet
    fCurrentTrack = selectedTrack;
}

void ZeeDEvent::SetGenBoson(const ZeeDGenParticle* genBoson, ZeeDEnum::MCFSRLevel::Value bosonType)
{
    if ( bosonType == ZeeDEnum::MCFSRLevel::Born ) 
    {                
        fGenBornBoson = genBoson;
    }
    else if ( bosonType == ZeeDEnum::MCFSRLevel::Bare )
    {
        fGenBareBoson = genBoson;
    }
    else if ( bosonType == ZeeDEnum::MCFSRLevel::Dressed )
    {
        fGenDressedBoson = genBoson;
    }
}

void ZeeDEvent::SetGenLeptons(ZeeDGenElectrons genLeptons, ZeeDEnum::MCFSRLevel::Value leptonType) {
    if ( leptonType == ZeeDEnum::MCFSRLevel::Born ) 
    {                
        fGenBornLeptons = genLeptons;
    }
    else if ( leptonType == ZeeDEnum::MCFSRLevel::Bare )
    {
        fGenBareLeptons = genLeptons;
    }
    else if ( leptonType == ZeeDEnum::MCFSRLevel::Dressed )
    {
        fGenDressedLeptons = genLeptons;
    }
}


//------------------------------------------------------
const  ZeeDGenElectrons ZeeDEvent::GetGenLeptons(ZeeDEnum::MCFSRLevel::Value leptonType) const
{
    // Attention: This function by default will return fGenBornLeptons! It was set just for compiling 2.3.9 release.
    switch (leptonType) {
        case ZeeDEnum::MCFSRLevel::Born: return fGenBornLeptons;
        case ZeeDEnum::MCFSRLevel::Bare: return fGenBareLeptons;
        case ZeeDEnum::MCFSRLevel::Dressed: return fGenDressedLeptons;
	default: return fGenBornLeptons;
    }
}

//------------------------------------------------------
const ZeeDGenParticle* ZeeDEvent::GetGenBoson(ZeeDEnum::MCFSRLevel::Value bosonType) const
{
    // Attention: This function by default will return fGenBornBoson! It was set just for compiling 2.3.9 release.
    if (fGenZBosArray->GetEntriesFast() == 1){
        return static_cast< ZeeDGenParticle* >( fGenZBosArray->At(0));
    }
    switch (bosonType) {
        case ZeeDEnum::MCFSRLevel::Born: return fGenBornBoson;
        case ZeeDEnum::MCFSRLevel::Bare: return fGenBareBoson;            
        case ZeeDEnum::MCFSRLevel::Dressed: return fGenDressedBoson;
	default: return fGenBornBoson;
    }
}



//------------------------------------------------------

const ZeeDTrack* ZeeDEvent::GetCurrentTrack() const
{
    //Returns the current Track
    return fCurrentTrack;
}

const ZeeDJet* ZeeDEvent::GetCurrentJet() const
{
    //Returns the current Jet
    return fCurrentJet;
}

//------------------------------------------------------
Double_t ZeeDEvent::GetEtDensity_3EM() const
{
  return fEtDensity_3EM;
}
Double_t ZeeDEvent::GetEtDensity_4EM() const
{ return fEtDensity_4EM;
}
Double_t ZeeDEvent::GetEtDensity_3LC() const
{
  return fEtDensity_3LC;
}
Double_t ZeeDEvent::GetEtDensity_4LC() const
{
  return fEtDensity_4LC;
}


//------------------------------------------------------
Double_t ZeeDEvent::GetWeight() const
{
    // Returns event weight
  return fEventWeight;
}

//------------------------------------------------------
Double_t ZeeDEvent::GetGenWeight() const
{
    // Returns gen weight
    //std::cout << fGenWeight << std::endl;
    return fGenWeight;
}

//------------------------------------------------------
const ZeeDBosonW* ZeeDEvent::GetW(Int_t index) const
{
    // Returns W boson
    return ( index < fWBosArray->GetEntriesFast() && index >= 0 ) ?
           static_cast<ZeeDBosonW*>(fWBosArray->At(index)) : NULL;
}

const ZeeDBosonW* ZeeDEvent::GetWmu(Int_t index) const
{
    // Returns W boson
    return ( index < fWmuBosArray->GetEntriesFast() && index >= 0 ) ?
           static_cast<ZeeDBosonW*>(fWmuBosArray->At(index)) : NULL;
}

//------------------------------------------------------
const ZeeDElectron* ZeeDEvent::GetElectronW(Int_t index) const
{
    // Returns electron belonging to the W

    // Get W boson
    Int_t nWBosons= this->GetWBosons()->GetEntriesFast()-1;
    if (index > nWBosons) 
         return NULL;
    const ZeeDBosonW* W = this->GetW(index);
    /*
    return  W != NULL ?
            static_cast<ZeeDElectron*>( fElecArray->At(W->GetElecIndex()) ) : NULL;*/
            
    return (ZeeDElectron*)W->GetLep();
}

const ZeeDMuon* ZeeDEvent::GetMuonWmu(Int_t index) const
{
    Int_t nWBosons= this->GetWmuBosons()->GetEntriesFast()-1;
    if (index > nWBosons)
         return NULL;
    const ZeeDBosonW* W = this->GetWmu(index);
    return (ZeeDMuon*)W->GetLep();
}

  
const ZeeDBosonW* ZeeDEvent::GetCurrentBosonW() const
{   
/*
    if( fCurrentW != NULL ) {
        std::cout << fCurrentW->GetMt() << "   " ;
        std::cout << fCurrentW->GetElec() << std::endl;
        }
    else {
         std::cout << "NULL" << std::endl;
         }*/
    return fCurrentW;
}


//------------------------------------------------------
ZeeDEnum::Trigger::Value ZeeDEvent::GetDefaultSingleElectronTrigger() const {

    const Int_t runNumber = this->RunNumber().Get();

    return ZeeDMisc::GetDefaultSingleElectronTrigger(runNumber);

}

//------------------------------------------------------
ZeeDEnum::Trigger::Value ZeeDEvent::GetDefaultDiElectronTrigger() const {

    const Int_t runNumber = this->RunNumber().Get();

    return ZeeDMisc::GetDefaultDiElectronTrigger(runNumber);

}

//------------------------------------------------------
ZeeDEnum::Period::Value ZeeDEvent::GetPeriod() const {

    const Int_t runNumber = this->RunNumber().Get();

    return ZeeDMisc::GetPeriod(runNumber);

}

//------------------------------------------------------
Bool_t ZeeDEvent::HasPassedTriggerChain(ZeeDEnum::Trigger::Value chain) const {

    if(this->Trigger().IsSet() == kFALSE) {

        Error("ZeeDEvent::HasPassedTriggerChain","this event does not have any trigger information");
        gSystem->Exit(EXIT_FAILURE);

    }


    Bool_t hasPassedChain = kFALSE;
    ZeeDEventBags::Trigger::TriggerMap::const_iterator it1 = this->Trigger().Get().hasPassedChain.begin();
    //for (it1; it1 != this->Trigger().Get().hasPassedChain.end(); ++it1)
        //std::cout << ZeeDEnum::Trigger::ToString(it1->first) << " " << it1->second << std::endl;
 
    ZeeDEventBags::Trigger::TriggerMap::const_iterator it = this->Trigger().Get().hasPassedChain.find(chain);
    if( it == this->Trigger().Get().hasPassedChain.end() ) {

        Error("ZeeDEvent::HasPassedTriggerChain","no trigger information found for chain \"%s\"", ZeeDEnum::Trigger::ToString(chain));
        gSystem->Exit(EXIT_FAILURE);

    } else {

        hasPassedChain = it->second;

    }

    return hasPassedChain;

}

const TObjArray* ZeeDEvent::GetJets(const TString& collection, Bool_t bWarnIfMissing) const {

    DEBUG_MES_START;

    TObjArray* tja = NULL;

    if(collection.Length() == 0) {

        TIter next(this->fJetCollectionArray);
        tja = (TObjArray*)next();

    } else {

        tja = (TObjArray*)(fJetCollectionArray->FindObject(collection));

    }

    static long long __counter__ = 0;
    if(bWarnIfMissing && tja == NULL && __counter__++ < 25 ) {

      

        Warning("ZeeDEvent::GetJets","jet collection %s not found in event", collection.Data());

        TIter next(this->fJetCollectionArray);

        TObjArray* this_tja;

        while (( this_tja = (TObjArray*)next())) {
            Warning("ZeeDEvent::GetJets", "available collection: %s", this_tja->GetName());
        }

        //gSystem->Exit(EXIT_FAILURE);

    }

    DEBUG_MES_END;

    return tja;

}

TObjArray* ZeeDEvent::GetJets(const TString& collection, Bool_t bWarnIfMissing)  {

    // http://stackoverflow.com/a/123995/1350789
    return const_cast<TObjArray*>(static_cast<const ZeeDEvent&>(*this).GetJets(collection, bWarnIfMissing));

}

Bool_t ZeeDEvent::HasJets(const TString& collection) const {

    return this->GetJets(collection, kFALSE) != NULL;

}
