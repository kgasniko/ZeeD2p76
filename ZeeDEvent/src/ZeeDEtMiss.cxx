#include "ZeeDEvent/ZeeDEtMiss.h"

// std libraries
#include <iostream>

//------------------------------------------------------
ZeeDEtMiss::ZeeDEtMiss():
    fEt(0),
    fEtX(0),
    fEtY(0),
    fPhi(0),
	fCorRecoilEt(0),
	fCorRecoilEtX(0),
	fCorRecoilEtY(0),
	fCorRecoilPhi(0),
	fCorRecoilSumet(0),
    fMET_SoftTermETX(0),
    fMET_SoftTermETY(0),
    fMET_SoftTermETSUM(0)
{
    // Constructor
 
  fMETCompositionJet_wet.reserve(10);
  fMETCompositionJet_wpx.reserve(10);
  fMETCompositionJet_wpy.reserve(10);
  fMETCompositionJet_statusWord.reserve(10);
  fMETCompositionJet_index.reserve(10);
  
  fMETCompositionElectron_wet.reserve(10);
  fMETCompositionElectron_wpx.reserve(10);
  fMETCompositionElectron_wpy.reserve(10);
  fMETCompositionElectron_statusWord.reserve(10);
  fMETCompositionElectron_index.reserve(10);
  
  fMETCompositionPhoton_wet.reserve(10);
  fMETCompositionPhoton_wpx.reserve(10);
  fMETCompositionPhoton_wpy.reserve(10);
  fMETCompositionPhoton_statusWord.reserve(10);
  fMETCompositionPhoton_index.reserve(10);
  
  fMETCompositionTau_wet.reserve(10);
  fMETCompositionTau_wpx.reserve(10);
  fMETCompositionTau_wpy.reserve(10);
  fMETCompositionTau_statusWord.reserve(10);
  fMETCompositionTau_index.reserve(10);
  
  fMETCompositionMuon_wet.reserve(10);
  fMETCompositionMuon_wpx.reserve(10);
  fMETCompositionMuon_wpy.reserve(10);
  fMETCompositionMuon_statusWord.reserve(10);
  fMETCompositionMuon_index.reserve(10);

}

//------------------------------------------------------
// ZeeDEtMiss::ZeeDEtMiss(const ZeeDEtMiss& etMiss) : TObject(etMiss)
// {
//     // Copy constructor
//     fEt  = etMiss.fEt;
//     fEtX = etMiss.fEtX;
//     fEtY = etMiss.fEtY;
//     fPhi = etMiss.fPhi;
// }

//------------------------------------------------------
ZeeDEtMiss::~ZeeDEtMiss()
{
    // Destructor
}

//------------------------------------------------------
void ZeeDEtMiss::Print(Option_t* option) const
{
    // Prints EtMiss content
    if ( !option || !option[0] ) {
        std::cout << option << std::endl;
    }

    std::cout << "etmiss (et, phi) = (";
    std::cout << fEt << ", " << fPhi;
    std::cout << ")" << std::endl;
}
