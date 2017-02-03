#include "ZeeDROOTInterface/ZeeDROOTTrack.h"
#include "ZeeDEvent/ZeeDTrack.h"

ZeeDROOTTrack::ZeeDROOTTrack() :
    fFourVector(0,0,0,0)
{
}

ZeeDROOTTrack::~ZeeDROOTTrack()
{
}

Int_t ZeeDROOTTrack::fillIn (const ZeeDTrack* const track)
{
  fFourVector                 = track->GetFourVector();
  fTrackD0                    = track->GetTrackD0();
  fTrackZ0                    = track->GetTrackZ0();
  fTrackNumberOfPixelHits     = track->GetTrackNumberOfPixelHits();
  fTrackNumberOfSCTHits       = track->GetTrackNumberOfSCTHits();
  fTrackChiSquared            = track->GetTrackChiSquared();
  fTrackNumberDoF             = track->GetTrackNumberDoF();
  fTrackNumberOfPixelHoles    = track->GetTrackNumberOfPixelHoles(); 
  fTrackNumberOfSCTHoles      = track->GetTrackNumberOfSCTHoles();
  fTrackNumberOfTRTHoles      = track->GetTrackNumberOfTRTHoles();
  fTrackqOverP                = track->GetTrackqOverP();
  fTrackTheta                 = track->GetTrackTheta();
  fTrackPhi0                  = track->GetTrackPhi0();

  return 0;
}

//------------------------------------------------------
Int_t ZeeDROOTTrack::fillOut(ZeeDTrack* const track) const
{
  // Fills ZeeDJet with the information from ZeeDROOTTrack

  track->SetFourVector   ( fFourVector );
  
  track->SetTrackD0( fTrackD0);
  track->SetTrackZ0( fTrackZ0);
  track->SetTrackNumberOfPixelHits( fTrackNumberOfPixelHits);
  track->SetTrackNumberOfSCTHits( fTrackNumberOfSCTHits);
  track->SetTrackChiSquared( fTrackChiSquared );
  track->SetTrackNumberDoF( fTrackNumberDoF );
  track->SetTrackNumberOfPixelHoles( fTrackNumberOfPixelHoles ); 
  track->SetTrackNumberOfSCTHoles( fTrackNumberOfSCTHoles );
  track->SetTrackNumberOfTRTHoles( fTrackNumberOfTRTHoles );
  track->SetTrackqOverP( fTrackqOverP );
  track->SetTrackTheta( fTrackTheta );
  track->SetTrackPhi0( fTrackPhi0 );

  return 0;
}
