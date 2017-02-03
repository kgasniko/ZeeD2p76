#ifndef __ZEEDTRACK_H
#define __ZEEDTRACK_H

////////////////////////////////////////////////////////
// Name    : ZeeDTrack.h
////////////////////////////////////////////////////////
//
// Contact persons: Andrei Nikforov
//

// Root includes
#include <TLorentzVector.h>
#include <TObject.h>
#include <string>
#include <vector>
#include "ZeeDTools/ZeeDQuantity.h"

/** Base jet class */
class ZeeDTrack : public TObject {

 public:
  
  ZeeDTrack();
  ~ZeeDTrack();
  
  virtual Bool_t IsSortable()const {return true;};
  virtual Int_t Compare(const TObject* j) const
  {
    Double_t ptj = (static_cast<const ZeeDTrack*>(j))->GetFourVector().Pt();
    Double_t pt = this->GetFourVector().Pt();
    
    if (pt < ptj)
      return 1;
    else if (pt > ptj)
      return -1;
    return 0;
  }

  
  // Getters
  /** Get track four vector */
  const TLorentzVector& GetFourVector() const {
    return fFourVector;
  }

  
  const Double_t& GetTrackD0() const {
    return fTrackD0;
  }
  
  const Double_t& GetTrackZ0() const {
    return fTrackZ0;
  }
  
  const Double_t& GetTrackNumberOfPixelHits() const {
    return fTrackNumberOfPixelHits;
  }
  
  const Double_t& GetTrackNumberOfSCTHits() const {
    return fTrackNumberOfSCTHits;
  }
  
  const Double_t& GetTrackChiSquared() const {     
    return fTrackChiSquared;
  }
  const Double_t& GetTrackNumberDoF() const {  
    return fTrackNumberDoF;
  }
  const Double_t& GetTrackNumberOfPixelHoles() const {      
    return fTrackNumberOfPixelHoles;
  }
  const Double_t& GetTrackNumberOfSCTHoles() const {      
    return fTrackNumberOfSCTHoles;
  }
  const Double_t& GetTrackNumberOfTRTHoles() const {      
    return fTrackNumberOfTRTHoles;
  }
  const Double_t& GetTrackqOverP() const {      
    return fTrackqOverP;
  }
  const Double_t& GetTrackTheta() const {     
    return fTrackTheta;
  } 
  const Double_t& GetTrackPhi0() const {     
    return fTrackPhi0;
  } 
   
  /** Blue print */
  void Print(Option_t* option = "") const;
  
 private:
  
  // Setters
  /** Set track four vector */
  void SetFourVector(TLorentzVector fourVector) {
    fFourVector = fourVector;
  }
  
  
  void SetTrackD0( Double_t frac ) {
    fTrackD0 = frac;
    return;
  }
  void SetTrackZ0( Double_t frac ) {
    fTrackZ0 = frac;
    return;
  }
  void SetTrackNumberOfPixelHits( Double_t frac ) {
    fTrackNumberOfPixelHits = frac;
    return;
  }
  void SetTrackNumberOfSCTHits( Double_t frac ) {
    fTrackNumberOfSCTHits = frac;
    return;
  }
  void SetTrackChiSquared( Double_t frac ) {
    fTrackChiSquared = frac;
    return;
  }
  void SetTrackNumberDoF( Double_t frac ) {
    fTrackNumberDoF = frac;
    return;
  }
  void SetTrackNumberOfPixelHoles( Double_t frac ) {
    fTrackNumberOfPixelHoles = frac;
    return;
  }
  void SetTrackNumberOfSCTHoles( Double_t frac ) {
    fTrackNumberOfSCTHoles = frac;
    return;
  }
  void SetTrackNumberOfTRTHoles( Double_t frac ) {
    fTrackNumberOfTRTHoles = frac;
    return;
  }
  void SetTrackqOverP( Double_t frac ) {
    fTrackqOverP = frac;
    return;
  }
  void SetTrackTheta( Double_t frac ) {
    fTrackTheta = frac;
    return;
  }
  void SetTrackPhi0( Double_t frac ) {
    fTrackPhi0 = frac;
    return;
  }
  
  /** Jet four-vector */
  
  TLorentzVector fFourVector;
  
  Double_t fTrackD0;
  Double_t fTrackZ0;
  Double_t fTrackNumberOfPixelHits;
  Double_t fTrackNumberOfSCTHits;  
  Double_t fTrackChiSquared;
  Double_t fTrackNumberDoF;
  Double_t fTrackNumberOfPixelHoles;
  Double_t fTrackNumberOfSCTHoles;
  Double_t fTrackNumberOfTRTHoles;
  Double_t fTrackqOverP;
  Double_t fTrackTheta;
  Double_t fTrackPhi0;
  
  friend class ZeeDReadTracksAOD;
  friend class ZeeDCalcTracks;
  friend class ZeeDROOTTrack;
};

#endif // ZeeDTrack
