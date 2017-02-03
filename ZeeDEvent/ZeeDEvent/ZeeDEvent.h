#pragma once

////////////////////////////////////////////////////////
// Name    : ZeeDEvent.h
////////////////////////////////////////////////////////
//
// Contact persons: Andrei Nikiforov, Sebastian Schmitt
//

// std includes
#include <utility>

// Root includes
#include <TObject.h>
#include <Rtypes.h>
#include <TObjArray.h>
#include <THashTable.h>

// ZeeD includes
#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDEvent/ZeeDBoson.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDBosonEMu.h"
#include "ZeeDEvent/ZeeDBosonW.h"
#include "ZeeDTools/LinkWeightContainer.h"
#include "ZeeDTools/ZeeDBags.h"
#include "ZeeDTools/ZeeDQuantity.h"
#include "ZeeDMisc/ZeeDMisc.h"

class ZeeDCalcElec;
class ZeeDCalcZ;
class ZeeDCalcW;
class ZeeDCalcGen;
class ZeeDSystematics;
class ZeeDInlineZ;
class ZeeDCalibrator;
class ZeeDJet;
class ZeeDTrack;
class ZeeDElectron;
class ZeeDMuon;
class ZeeDMuonSpShower;
class ZeeDCalcJets;
class ZeeDVertex;
class ZeeDCalcVertex;
class ZeeDGenParticle;

class ZeeDEvent;
typedef ZeeDEvent RawEvent;

/** Basic event class */
class ZeeDEvent : public TObject {

public:

    ~ZeeDEvent();

    // Getters
    const TObjArray* GetElectrons()   const {
        return fElecArray;
    }
    const TObjArray* GetMuons()   const {
        return fMuonArray;
    }
    const TObjArray* GetMuonSpShowers()   const {
        return fMuonSpShowerArray;
    }
    const TObjArray* GetEtMissArray() const {
        return fEtMissArray;
    }
    const TObjArray* GetZBosons()     const {
        return fZBosArray;
    }
/*
    LinkWeightContainer<ZeeDBosonEMu*>* GetZemuBosons()     const {
        return fEMuBosArray;
    }
*/
    const TObjArray* GetZmumuBosons()     const {
        return fZmumuBosArray;
    }

    const TObjArray* GetPhotons()   const {
        return fPhotonArray;
    }

    const TObjArray* GetTaus()   const {
        return fTauArray;
    }

    const TObjArray* GetJets(const TString& collection="", Bool_t bWarnIfMissing=kTRUE) const;

    const TObjArray* GetTracks()   const {
        return fTrackArray;
    }

    Bool_t HasJets(const TString& collections) const;

    const TObjArray* GetWBosons()     const {
        return fWBosArray;
    }
   
    const TObjArray* GetWmuBosons()   const {
        return fWmuBosArray;
    }

    /** @todo implement as ZeeDQuantity */
    std::string      GetJetFlavour() const {
        return "Vanilla";
    }

    const TObjArray* GetVertices()   const {
        return fVertexArray;
    }

    const ZeeDBosonW* GetW(Int_t index = 0) const;
    
    const ZeeDBosonW* GetWmu(Int_t index=0) const;    

    const ZeeDElectron* GetElectronW(Int_t index = 0) const;

    const ZeeDMuon* GetMuonWmu(Int_t index=0) const;

    const TObjArray* GetGenBosons()      const {
        return fGenZBosArray;
    }
    const TObjArray* GetGenLeptons()   const {
        return fGenElecArray;
    }
    const TObjArray* GetGenGammas()  const {
        return fGenGammaArray;
    }

    const ZeeDBosonW* GetCurrentBosonW() const;

    void SetGenBoson(const ZeeDGenParticle* genBornBoson, ZeeDEnum::MCFSRLevel::Value bosonType);    
    void SetGenLeptons(ZeeDGenElectrons genLeptons, ZeeDEnum::MCFSRLevel::Value leptonType);    

    const ZeeDGenParticle* GetGenBoson(ZeeDEnum::MCFSRLevel::Value bosonType) const;
    const ZeeDGenElectrons   GetGenLeptons(ZeeDEnum::MCFSRLevel::Value leptonType) const;

    // to leptons = electrons, muons, neutrinos 
    inline const ZeeDGenElectrons GetGenElectrons(ZeeDEnum::MCFSRLevel::Value elecType) const{
        return GetGenLeptons(elecType);        
    }
    inline const ZeeDGenElectrons GetGenMuons(ZeeDEnum::MCFSRLevel::Value muonType) const{
        return GetGenLeptons(muonType);        
    }

    const ZeeDQuantity<Int_t>& EventNumber() const {
        return fEventNumber;
    }
    const ZeeDQuantity<Int_t>& RunNumber() const {
        return fRunNumber;
    }
    const ZeeDQuantity<Int_t>& OrigRunNumber() const {
        return fOrigRunNumber;
    }
    const ZeeDQuantity<Int_t>& LB() const {
        return fLB;
    }
    const ZeeDQuantity<Double_t>& ActualInteractionsPerCrossing() const {
        return fActualInteractionsPerCrossing;
    }
    const ZeeDQuantity<Double_t>& AverageInteractionsPerCrossing() const {
        return fAverageInteractionsPerCrossing;
    }

    const ZeeDQuantity<Int_t>& MCChannelNumber() const {
        return fMCChannelNumber;
    }

    const ZeeDQuantity<ZeeDEnum::MCtype::Value>& MCtype() const {
        return fMCtype;
    }

    const ZeeDQuantity<Int_t>& Nvtx() const {
        return fNvtx;
    }

    const ZeeDQuantity<Int_t>& NpvJets() const {
        return fNpvJets;
    }

    const ZeeDQuantity<ZeeDEventBags::Trigger>& Trigger() const {
        return fTrigger;
    }

    const ZeeDQuantity<Bool_t>& IsInGRL() const {
        return fIsInGRL;
    }

    const ZeeDQuantity<Int_t>& LArErrorState() const {
        return fLArErrorState;
    }

    ZeeDQuantity<ZeeDEventBags::MCSyst>& MCSystW() const {
        return fMCSystW;
    }

    ZeeDEnum::Period::Value GetPeriod() const;

    ZeeDEnum::Trigger::Value GetDefaultSingleElectronTrigger() const;

    ZeeDEnum::Trigger::Value GetDefaultDiElectronTrigger() const;

    /** Returns event weight */
    Double_t GetWeight() const;

    /** Returns Gen weight */
    Double_t GetGenWeight() const;


    Double_t GetEtDensity_3EM() const ;
    Double_t GetEtDensity_4EM() const ;
    Double_t GetEtDensity_3LC() const ;
    Double_t GetEtDensity_4LC() const ;

    /** Returns current boson candidate */
    const ZeeDBosonZ* GetCurrentBoson() const {
        return (ZeeDBosonZ*)fCurrentBoson;
    }
   

    /** Returns current Jet */
    const ZeeDJet* GetCurrentJet() const;

    /** Returns current track */
    const ZeeDTrack* GetCurrentTrack() const;

    /** Returns electrons of the boson candidate */
    const TObjArray*    GetBosonElectrons() const;

    /** Boolean flag */
    Bool_t IsData() const {
        return fIsData;
    }

    /** Boolean flag */
    Bool_t IsMC() const {
        return !fIsData;
    }

    /** Returns array of generated Z bosons */
    TObjArray* GetGenBosons() {
        return fGenZBosArray;
    }

    /** Returns array of generated electrons */
    TObjArray* GetGenLeptons() {
        return fGenElecArray;
    }

    /** Returns array of generated photons */
    TObjArray* GetGenGammas() {
        return fGenGammaArray;
    }

    /** Returns array of electrons */
    TObjArray* GetElectrons() {
        return fElecArray;
    }

    void ClearMuons() {
      fMuonArray->Clear();
    }
    void AddMuon(ZeeDMuon* mu) {
      fMuonArray->AddLast((TObject*)mu);
    }

    void ClearMuonSpShowers() {
      fMuonSpShowerArray->Clear();
    }
    void AddMuonSpShower(ZeeDMuonSpShower* mu) {
      fMuonSpShowerArray->AddLast((TObject*)mu);
    }

    /** Returns array of et miss objects */
    TObjArray* GetEtMissArray() {
        return fEtMissArray;
    }

    /** Returns array of Z boson candidates */
    void ClearZBosons() {
      fZBosArray->Clear();
    }
    void AddZBoson(ZeeDBosonZ* z) {
      fZBosArray->AddLast((TObject*)z);
    }

    /** Handle the array of Z boson candidates */
    /*const ZeeDBosonEMu* GetCurrentEMuBoson() { return fCurrentEMuBoson; }

    const ZeeDBosonEMu* GetEMuBoson(unsigned int i = 0) const { return fEMuBosArray->GetLink(i); }

    void ClearEMuBosons() { fEMuBosArray->Clear(); }

    void AddEMuBoson(ZeeDBosonEMu* z, double weight) { fEMuBosArray->Add(z, weight); }
*/
    const ZeeDBosonZ* GetZmumuBoson(unsigned int i = 0) const { return (ZeeDBosonZ*)fZmumuBosArray->At(i);}

    void ClearZmumuBosons() { fZmumuBosArray->Clear(); }

    void AddZmumuBoson(ZeeDBosonZ* z) { fZmumuBosArray->Add(z); }

    const TObjArray* getZmumuBosons() const { return fZmumuBosArray; }

    /** Returns array of W boson candidates */
    TObjArray* GetWBosons() {
        return fWBosArray;
    }
    
    TObjArray* GetWmuBosons() {
        return fWmuBosArray;
    }
    
    void ClearWBosons() {
        fWBosArray->Clear();
    }
   
    void ClearWmuBosons() {
        fWmuBosArray->Clear();
    }
    
    void AddWBoson(ZeeDBosonW* w) {
         fWBosArray->AddLast((TObject*)w);
         //std::cout << 1;
         //this->SetCurrentBosonW(w);
    }    

    void AddWmuBoson(ZeeDBosonW* w) {
         fWmuBosArray->AddLast((TObject*)w);
    }

    TObjArray* GetPhotons() {
        return fPhotonArray;
    }

    TObjArray* GetTaus() {
        return fTauArray;
    }

    /** Returns array of Tracks candidates */
    TObjArray* GetTracks() {
        return fTrackArray;
    }

    /** Returns array of jets*/
    TObjArray* GetJets(const TString& collection, Bool_t bWarnIfMissing=kTRUE);

    /** Returns array of vertices */
    TObjArray* GetVertices()  {
        return fVertexArray;
    }

    /** Returns event number */
    ZeeDQuantity<Int_t>& EventNumber() {
        return fEventNumber;
    }

    /** Returns event run number - simulated for MC*/
    ZeeDQuantity<Int_t>& RunNumber() {
        return fRunNumber;
    }

    /** Returns unchanged event run number */
    ZeeDQuantity<Int_t>& OrigRunNumber() {
        return fOrigRunNumber;
    }

    /** Returns luminocity block number */
    ZeeDQuantity<Int_t>& LB() {
        return fLB;
    }

    ZeeDQuantity<Double_t>& ActualInteractionsPerCrossing() {
        return fActualInteractionsPerCrossing;
    }

    ZeeDQuantity<Double_t>& AverageInteractionsPerCrossing() {
        return fAverageInteractionsPerCrossing;
    }

    ZeeDQuantity<Int_t>& MCChannelNumber() {
        return fMCChannelNumber;
    }

    /** Returns MC type */
    ZeeDQuantity<ZeeDEnum::MCtype::Value>& MCtype() {
        return fMCtype;
    }

    ///** Returns Weights for MC systematics */
    //ZeeDQuantity<ZeeDEventBags::MCSyst>& MCSystW() const {
    //    return fMCSystW;
    //}

    /** Returns number of good primary verticies */
    ZeeDQuantity<Int_t>& Nvtx() {
        return fNvtx;
    }
    ZeeDQuantity<Int_t>& NpvJets() {
        return fNpvJets;
    }
    
    ZeeDQuantity<ZeeDEventBags::Trigger>& Trigger() {
        return fTrigger;
    }

    ZeeDQuantity<Bool_t>& IsInGRL() {
        return fIsInGRL;
    }

    ZeeDQuantity<Int_t>& LArErrorState() {
        return fLArErrorState;
    }

    /** Get raw event: FOR EXPERTS ONLY ! */
    const ZeeDEvent* GetRawEvent() const {
        return fRawEvent;
    }


    // Setters
    void SetWeight(Double_t weight) const{
        fEventWeight = weight;
    }
    void SetGenWeight(Double_t weight) {
        fGenWeight = weight;
    }


    void SetEtDensity_3EM(Double_t  q){
      fEtDensity_3EM = q;
    }
    void SetEtDensity_4EM(Double_t  q){
      fEtDensity_4EM = q;
    }
    void SetEtDensity_3LC(Double_t  q){
      fEtDensity_3LC = q;
    }
    void SetEtDensity_4LC(Double_t  q){
      fEtDensity_4LC = q;
    }


    void SetCurrentBoson(const ZeeDBosonZ* selectedBoson);
 
    void SetCurrentBosonW   (const ZeeDBosonW* selectedBoson);
   
    void SetCurrentJet(const ZeeDJet* selectedJet);

    //void SetCurrentEMuBoson(const ZeeDBosonEMu* boson) { fCurrentEMuBoson = boson; }

    void SetCurrentTrack(const ZeeDTrack* selectedTrack);


    void SetIsData(Bool_t isData) {
        fIsData = isData;
    }

    const ZeeDQuantity<ZeeDEventGenBags::PDF>         PDF()        const {
        return fPDF;
    }
    
    const ZeeDQuantity<ZeeDEventGenBags::Vertex>         Vertex()        const {
        return fVertex;
    }
    
    Bool_t HasPassedTriggerChain(ZeeDEnum::Trigger::Value chain) const;
    
    void SetNGoodW (Int_t num) const{
         numWGood=num;
         }

    Int_t GetNGoodW () const{
         return numWGood;
         }
    
private:
   
    mutable Int_t numWGood;    
   
    ZeeDQuantity<ZeeDEventGenBags::PDF>&        PDF()        {
        return fPDF;
    }

    ZeeDQuantity<ZeeDEventGenBags::Vertex>&     Vertex()     {
        return fVertex;
    }
    
    ZeeDQuantity<ZeeDEventGenBags::PDF>         fPDF;        //!
    ZeeDQuantity<ZeeDEventGenBags::Vertex>      fVertex;     //!
    
    ZeeDEvent();
    ZeeDEvent(const RawEvent& ev);

    /** Event information array */
    TObjArray toBeDeleted; //!

    /**  Array of electrons */
    TObjArray* fElecArray; //!

    /**  Array of tracks */
    TObjArray* fTrackArray; //!

    /**  Array of et miss objects */
    TObjArray* fEtMissArray; //!

    /**  Array of Vertices */
    TObjArray* fVertexArray; //!

    /**  Array of Z bosons */
    TObjArray* fZBosArray; //!

    /**  Array of Z bosons */
    //LinkWeightContainer<ZeeDBosonEMu*>* fEMuBosArray;

    /**  Array of Z bosons */
    TObjArray* fZmumuBosArray;

    /** Array of Jets */
    THashTable* fJetCollectionArray;

    /** Array of Muons */
    TObjArray* fMuonArray;

    /** Array of MuonSpShowers */
    TObjArray* fMuonSpShowerArray;

    /**  Array of W bosons */
    TObjArray* fWBosArray; //!

    /**  Array of Wmu bosons */
    TObjArray* fWmuBosArray; //!

    /**  Generated Z bosons */
    TObjArray* fGenZBosArray; //!

    /**  Generated electrons */
    TObjArray* fGenElecArray; //!

    /**  Generated photons */
    TObjArray* fGenGammaArray; //!

    /**  Array of photons */
    TObjArray* fPhotonArray; //!

    /**  Array of taus */
    TObjArray* fTauArray; //!

    /** Number of primary vertices */
    ZeeDQuantity<Int_t> fNvtx;
    ZeeDQuantity<Int_t> fNpvJets;

    /**  Run number */
    ZeeDQuantity<Int_t> fRunNumber; //!

    ZeeDQuantity<Int_t> fOrigRunNumber; //!

    /**  Event number */
    ZeeDQuantity<Int_t> fEventNumber; //!

    /**  LB number */
    ZeeDQuantity<Int_t> fLB; //!

    ZeeDQuantity<Double_t> fActualInteractionsPerCrossing;

    ZeeDQuantity<Double_t> fAverageInteractionsPerCrossing;

    ZeeDQuantity<Int_t> fMCChannelNumber;

    /** MC type */
    ZeeDQuantity<ZeeDEnum::MCtype::Value> fMCtype;

    /**  MC Syst Weights */
    mutable ZeeDQuantity<ZeeDEventBags::MCSyst> fMCSystW; //!

    /** Trigger */
    ZeeDQuantity<ZeeDEventBags::Trigger> fTrigger; //!

    /** Manual GRL decision */
    ZeeDQuantity<Bool_t> fIsInGRL; //!

    ZeeDQuantity<Int_t> fLArErrorState;

    /**  Event weight */
    mutable Double_t fEventWeight; //!

    /**  Gen event weight */
    Double_t fGenWeight; //!

    /**  Et  Density    of the event . Needed for pileup correction fo jets        **/
    Double_t fEtDensity_3EM;
    Double_t fEtDensity_4EM;
    Double_t fEtDensity_3LC;
    Double_t fEtDensity_4LC;

    /**  Current boson, primary cand. */
    const ZeeDBosonZ* fCurrentBoson;

    /** Current Jet */
    const ZeeDJet* fCurrentJet;

    /**  Current e-mu fake boson */
    //const ZeeDBosonEMu* fCurrentEMuBoson;

    /**  Current mu-mu boson */
   // const ZeeDBosonZmumu* fCurrentZmumuBoson;

    /** Current Track */
    const ZeeDTrack* fCurrentTrack;

    const ZeeDGenParticle* fGenBornBoson;

    /** keep bare and dressed bosons */
    const ZeeDGenParticle* fGenBareBoson;
    const ZeeDGenParticle* fGenDressedBoson;

    ZeeDGenElectrons fGenBornLeptons;

    /** Also keep bare electrons */
    ZeeDGenElectrons fGenBareLeptons;

    /** And dressed electrons  TODO */
    ZeeDGenElectrons fGenDressedLeptons;

    const ZeeDBosonW* fCurrentW;

    /** Current W mu boson */
    //const ZeeDBosonWmu* fCurrentWmuBoson;

    
    /** true if Event is data, false if MC
        @todo do we need IsInline?
    */
    Bool_t fIsData; //!

    /** Pointer to the raw event */
    const ZeeDEvent* fRawEvent;

    /** Service handle to job options file */
    ZeeDSvcHelper fSvcHelper; 
    ServiceHandle<IZeeDAnalysisSvc> *fAnaOptions;

    friend class ZeeDEventFactory;
    friend class ZeeDReadGenAOD;
    friend class ZeeDReadJetsAOD;
    friend class ZeeDReadTracksAOD;
    friend class ZeeDROOTEvent;
    friend class ZeeDCalcWeight;
    friend class ZeeDCalcEventWeight;

};

