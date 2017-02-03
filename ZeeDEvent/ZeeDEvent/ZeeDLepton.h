#pragma once 

////////////////////////////////////////////////////////
// Name    : ZeeDLepton.h
////////////////////////////////////////////////////////
//
// Root includes
#include <TLorentzVector.h>
#include <TObject.h>

#include "ZeeDTools/ZeeDQuantity.h"
#include "ZeeDTools/ZeeDBags.h"
#include "ZeeDMisc/ZeeDMisc.h"

class ZeeDLepton : public TObject {

public:
    ~ZeeDLepton();

    struct LeptonType {enum Value {electron, muon}; };

    LeptonType::Value GetLeptonType() const {return fleptonType;};

    const TLorentzVector& GetFourVector() const { return ffourVector; }
    float getCharge() const { return fcharge; }
    bool  getHasCharge() const { return fhasCharge; }
	bool  getInRefComposition() const {return fInComp;}
    
	
	const ZeeDQuantity<ZeeDLeptonBags::TrackSummary>&   TrackSummary()  const {
        return fTrackSummary;
    }
 
    const ZeeDQuantity<ZeeDLeptonBags::TrackParticle> TrackParticle()     const {
           return fTrackParticle;
    }
    const ZeeDQuantity<ZeeDLeptonBags::MCTruth>&        MCTruth()       const {
        return fMCTruth;
    }
    const ZeeDQuantity<ZeeDLeptonBags::Vertex>&         Vertex()        const {
        return fVertex;
    }
    const ZeeDQuantity<ZeeDLeptonBags::IsEM>&           IsEM()          const {
        return fIsEM;
    }
    const ZeeDQuantity<ZeeDLeptonBags::Trigger>&        Trigger()       const {
        return fTrigger;
    }
    const ZeeDQuantity<ZeeDLeptonBags::IdentificationSF>& IdentificationSF() const {
        return fIdentificationSF;
    }
    const ZeeDQuantity<ZeeDLeptonBags::ReconstructionSF>& ReconstructionSF() const {
        return fReconstructionSF;
    }
    const ZeeDQuantity<ZeeDLeptonBags::IsolationSF>& IsolationSF() const {
        return fIsolationSF;
    }    
    const ZeeDQuantity<ZeeDLeptonBags::TriggerSF>& TriggerSF() const {
        return fTriggerSF;
    }

    Bool_t IsMatchedToTriggerChain(ZeeDEnum::Trigger::Value chain) const;

    // Matching for born electrons:
    const ZeeDGenParticle* GetGenBornMatchedLepton() const;
    const Int_t& GetCollectionIndex() const {
      return fCollectionIndex;
    } 

   
    /** Print out electron info */
    //virtual void Print(Option_t* option = "") const;

protected:
    const Int_t& GetCollectionIndex() {
      return fCollectionIndex;
    } 


    ZeeDLepton();
    void SetCollectionIndex(Int_t value) {
            fCollectionIndex = value;
            }
    // Setters
    void setFourVector(TLorentzVector fourVector) { ffourVector = fourVector; }
    void setCharge(float charge) { fcharge = charge; }
    void setHasCharge(bool hasCharge) { fhasCharge = hasCharge; }	
	void setInRefComposition(bool inComp=true) { fInComp=inComp;}


     ZeeDQuantity<ZeeDLeptonBags::TrackSummary>&  TrackSummary()  {
        return fTrackSummary;
    }
 
        ZeeDQuantity<ZeeDLeptonBags::Trigger>&       Trigger()       {
        return fTrigger;
    }
    ZeeDQuantity<ZeeDLeptonBags::IsEM>&          IsEM()          {
        return fIsEM;
    }
    
    ZeeDQuantity<ZeeDLeptonBags::Vertex>&        Vertex()        {
        return fVertex;
    }
    ZeeDQuantity<ZeeDLeptonBags::MCTruth>&       MCTruth()       {
        return fMCTruth;
    }
    ZeeDQuantity<ZeeDLeptonBags::TrackParticle>& TrackParticle() {
        return fTrackParticle;
    }
      
    ZeeDQuantity<ZeeDLeptonBags::IdentificationSF>& IdentificationSF() {
        return fIdentificationSF;
    }
    ZeeDQuantity<ZeeDLeptonBags::ReconstructionSF>& ReconstructionSF() {
        return fReconstructionSF;
    }
    ZeeDQuantity<ZeeDLeptonBags::IsolationSF>& IsolationSF() {
        return fIsolationSF;
    }  
    ZeeDQuantity<ZeeDLeptonBags::TriggerSF>& TriggerSF() {
        return fTriggerSF;
    }


    ZeeDQuantity<ZeeDLeptonBags::TrackParticle> fTrackParticle; //!
    ZeeDQuantity<ZeeDLeptonBags::MCTruth>       fMCTruth;       //!
    ZeeDQuantity<ZeeDLeptonBags::Vertex>        fVertex;        //!
    ZeeDQuantity<ZeeDLeptonBags::IsEM>          fIsEM;          //!
    ZeeDQuantity<ZeeDLeptonBags::Trigger>       fTrigger;       //!
    ZeeDQuantity<ZeeDLeptonBags::IdentificationSF> fIdentificationSF; //!
    ZeeDQuantity<ZeeDLeptonBags::ReconstructionSF> fReconstructionSF; //!
    ZeeDQuantity<ZeeDLeptonBags::IsolationSF>      fIsolationSF; //!    
    ZeeDQuantity<ZeeDLeptonBags::TriggerSF>        fTriggerSF; //!
    ZeeDQuantity<ZeeDLeptonBags::TrackSummary>  fTrackSummary;  //!


    TLorentzVector	ffourVector;
    bool		fhasCharge;
    float		fcharge;
	bool        fInComp;

    const ZeeDGenParticle* fGenBornMatchedLepton;

    TLorentzVector fCombinedFourVector;

    Int_t fCollectionIndex; //!
    LeptonType::Value fleptonType; 

    friend class ZeeDReadEventInlineZ;
    friend class ZeeDROOTElectron;
    friend class ZeeDROOTMuon;
    friend class ZeeDROOTEvent; //I still think that making a constructor private is a bad idea. George.
    friend class ZeeDReadElecAOD;
    friend class ZeeDReadMuonAOD;
    friend class ZeeDReadEtMissAOD;
    friend class ZeeDCalcElec;
    friend class ZeeDCalcMuon;
    friend class ZeeDCalcEventWeight;
    friend class ZeeDCalcRecWeight;
	friend class ZeeDCalcRecWeightMuon;
    friend class ZeeDReadEventD3PD;
    friend class ZeeDBoson;
    friend class ZeeDCalcEtMiss;
};
