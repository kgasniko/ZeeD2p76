#include "ZeeDROOTInterface/ZeeDROOTTau.h"
#include "ZeeDEvent/ZeeDTau.h"

ZeeDROOTTau::ZeeDROOTTau() :
  fFourVector(0,0,0,0),
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

ZeeDROOTTau::~ZeeDROOTTau()
{
}

Int_t ZeeDROOTTau::fillIn (const ZeeDTau* const tau)
{

    // Fills ZeeDROOTTau with the information from ZeeDTau
  fFourVector                 = tau->GetFourVector();
  fCollectionIndex            = tau->GetCollectionIndex();

  fIsTight            = tau->IsTight();
  fIsMedium           = tau->IsMedium();
  fIsLoose            = tau->IsLoose();

  fIsBDTTight         = tau->IsBDTTight();
  fIsBDTMedium        = tau->IsBDTMedium();
  fIsBDTLoose         = tau->IsBDTLoose();

  fIsEleBDTTight      = tau->IsEleBDTTight();
  fIsEleBDTMedium     = tau->IsEleBDTMedium();
  fIsEleBDTLoose      = tau->IsEleBDTLoose();

  fIsLLHTight         = tau->IsLLHTight();
  fIsLLHMedium        = tau->IsLLHMedium();
  fIsLLHLoose         = tau->IsLLHLoose();

  fIsMuonVeto         = tau->IsMuonVeto();

  return 0;
}

//------------------------------------------------------
Int_t ZeeDROOTTau::fillOut(ZeeDTau* const tau) const
{
    // Fills ZeeDTau with the information from ZeeDROOTTau
  tau->SetFourVector( fFourVector );
  tau->GetCollectionIndex()  = fCollectionIndex ;

  tau->SetIsTight( fIsTight );
  tau->SetIsMedium( fIsMedium );
  tau->SetIsLoose( fIsLoose );

  tau->SetIsBDTTight( fIsBDTTight );
  tau->SetIsBDTMedium( fIsBDTMedium );
  tau->SetIsBDTLoose( fIsBDTLoose );

  tau->SetIsEleBDTTight( fIsEleBDTTight );
  tau->SetIsEleBDTMedium( fIsEleBDTMedium );
  tau->SetIsEleBDTLoose( fIsEleBDTLoose );

  tau->SetIsLLHTight( fIsLLHTight );
  tau->SetIsLLHMedium( fIsLLHMedium );
  tau->SetIsLLHLoose( fIsLLHLoose );

  tau->SetIsMuonVeto( fIsMuonVeto );

  return 0;
}
