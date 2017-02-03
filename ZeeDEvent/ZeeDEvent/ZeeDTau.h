#ifndef __ZEEDTAU_H
#define __ZEEDTAU_H

////////////////////////////////////////////////////////
// Name    : ZeeDTau.h
////////////////////////////////////////////////////////

#include <TObject.h>
#include "TLorentzVector.h"

class ZeeDTau: public TObject {
public:
  ZeeDTau();
  ~ZeeDTau();

  const TLorentzVector& GetFourVector() const {
    return fFourVector;
  }

  const Int_t& GetCollectionIndex() const {return fCollectionIndex;};


  Bool_t IsTight() const {
        return fIsTight;
  }
  Bool_t IsMedium() const {
        return fIsMedium;
  }
  Bool_t IsLoose() const {
        return fIsLoose;
  }

  Bool_t IsBDTTight() const {
        return fIsBDTTight;
  }
  Bool_t IsBDTMedium() const {
        return fIsBDTMedium;
  }
  Bool_t IsBDTLoose() const {
        return fIsBDTLoose;
  }

  Bool_t IsEleBDTTight() const {
        return fIsEleBDTTight;
  }
  Bool_t IsEleBDTMedium() const {
        return fIsEleBDTMedium;
  }
  Bool_t IsEleBDTLoose() const {
        return fIsEleBDTLoose;
  }

  Bool_t IsLLHTight() const {
        return fIsLLHTight;
  }
  Bool_t IsLLHMedium() const {
        return fIsLLHMedium;
  }
  Bool_t IsLLHLoose() const {
        return fIsLLHLoose;
  }

  Bool_t IsMuonVeto() const {
        return fIsMuonVeto;
  }


private:

  void SetFourVector(TLorentzVector fourVector) {
    fFourVector = fourVector;
  }

  Int_t& GetCollectionIndex(){return fCollectionIndex;};

  void SetIsTight(Bool_t isTight) {
        fIsTight = isTight;
  }
  void SetIsMedium(Bool_t isMedium) {
        fIsMedium = isMedium;
  }
  void SetIsLoose(Bool_t isLoose) {
        fIsLoose = isLoose;
  }

  void SetIsBDTTight(Bool_t isBDTTight) {
        fIsBDTTight = isBDTTight;
  }
  void SetIsBDTMedium(Bool_t isBDTMedium) {
        fIsBDTMedium = isBDTMedium;
  }
  void SetIsBDTLoose(Bool_t isBDTLoose) {
        fIsBDTLoose = isBDTLoose;
  }

  void SetIsEleBDTTight(Bool_t isEleBDTTight) {
        fIsEleBDTTight = isEleBDTTight;
  }
  void SetIsEleBDTMedium(Bool_t isEleBDTMedium) {
        fIsEleBDTMedium = isEleBDTMedium;
  }
  void SetIsEleBDTLoose(Bool_t isEleBDTLoose) {
        fIsEleBDTLoose = isEleBDTLoose;
  }

  void SetIsLLHTight(Bool_t isLLHTight) {
        fIsLLHTight = isLLHTight;
  }
  void SetIsLLHMedium(Bool_t isLLHMedium) {
        fIsLLHMedium = isLLHMedium;
  }
  void SetIsLLHLoose(Bool_t isLLHLoose) {
        fIsLLHLoose = isLLHLoose;
  }
  void SetIsMuonVeto(Bool_t isMuonVeto) {
        fIsMuonVeto = isMuonVeto;
  }


  TLorentzVector fFourVector; 
  Int_t fCollectionIndex;

  Bool_t fIsTight;
  Bool_t fIsMedium;
  Bool_t fIsLoose;

  Bool_t fIsBDTTight;
  Bool_t fIsBDTMedium;
  Bool_t fIsBDTLoose;

  Bool_t fIsEleBDTTight;
  Bool_t fIsEleBDTMedium;
  Bool_t fIsEleBDTLoose;

  Bool_t fIsLLHTight;
  Bool_t fIsLLHMedium;
  Bool_t fIsLLHLoose;

  Bool_t fIsMuonVeto;

  friend class ZeeDReadEtMissAOD;
  friend class ZeeDReadTauAOD;
  friend class ZeeDROOTTau;

};

#endif /* __ZEEDTAU_H */
