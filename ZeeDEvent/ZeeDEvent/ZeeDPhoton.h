#ifndef __ZEEDPHOTON_H
#define __ZEEDPHOTON_H

////////////////////////////////////////////////////////
// Name    : ZeeDPhoton.h
////////////////////////////////////////////////////////

#include <TObject.h>
#include "TLorentzVector.h"

class ZeeDPhoton: public TObject {
public:
  ZeeDPhoton();
  ~ZeeDPhoton();

  const TLorentzVector& GetFourVector() const {
    return fFourVector;
  }
  
  const Int_t& GetCollectionIndex() const {return fCollectionIndex;}
  
private:

  void SetFourVector(TLorentzVector fourVector) {
    fFourVector = fourVector;
  }

  Int_t& GetCollectionIndex() {return fCollectionIndex;}

  TLorentzVector fFourVector; 
  Int_t fCollectionIndex;

  friend class ZeeDReadEtMissAOD;
  friend class ZeeDReadPhotonAOD;
  friend class ZeeDROOTPhoton;

};

#endif /* __ZEEDPHOTON_H */
