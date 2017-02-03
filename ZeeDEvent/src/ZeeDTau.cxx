#include "ZeeDEvent/ZeeDTau.h"

// std libraries
#include <iostream>

ZeeDTau::ZeeDTau() : 
  fFourVector(0., 0., 0., 0.), 
  fCollectionIndex(-1),
  fIsTight(kFALSE),
  fIsMedium(kFALSE),
  fIsLoose(kFALSE),
  fIsBDTTight(kFALSE),
  fIsBDTMedium(kFALSE),
  fIsBDTLoose(kFALSE),
  fIsEleBDTTight(kFALSE),
  fIsEleBDTMedium(kFALSE),
  fIsEleBDTLoose(kFALSE),
  fIsLLHTight(kFALSE),
  fIsLLHMedium(kFALSE),
  fIsLLHLoose(kFALSE),
  fIsMuonVeto(kFALSE)
{}

ZeeDTau::~ZeeDTau() 
{
  // Destructor
}
