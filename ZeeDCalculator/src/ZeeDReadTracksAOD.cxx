#define DEBUG_ON 0

#include "ZeeDCalculator/ZeeDReadTracksAOD.h"

// std libraries
#include <iostream>
#include <string>

// Root libraries
#include <TLorentzVector.h>
#include <TSystem.h>
#include <TObjArray.h>

// Athena libraries
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"
#include "Particle/TrackParticle.h"


// ZeeD analysis includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDEvent/ZeeDJet.h"
#include "ZeeDEvent/ZeeDTrack.h"
#include "ZeeDEvent/ZeeDVertex.h"
#include "JetSelectorTools/AthJetCleaningTool.h"

//------------------------------------------------------
ZeeDReadTracksAOD::ZeeDReadTracksAOD() :
    fIsMC(kFALSE),
    fEvent(NULL),
    fTrackInProcess(NULL),
    fTrackContainer(NULL),
    fSvcHelper("ZeeDReadTracksAOD"),
    fAnaOptions(fSvcHelper.GetAnaSvc()),
    fStoreGate(fSvcHelper.GetStoreGate())
{
}

//------------------------------------------------------
ZeeDReadTracksAOD::~ZeeDReadTracksAOD()
{
    // Destructor
}

//------------------------------------------------------
void ZeeDReadTracksAOD::Initialize()
{
  
  std::string trackContainerName = "TrackParticleCandidate";
  StatusCode sc = (*fStoreGate)->retrieve(fTrackContainer, trackContainerName);
  
  if (sc.isFailure() || !fTrackContainer ) {
    Error("ZeeDReadTracksAOD::Initialize",
	  "Unable to open ZeeDAnalysisSvc");
    gSystem->Exit(EXIT_FAILURE);  
  }
  fIsMC = (*fAnaOptions)->IsMC();
}

//------------------------------------------------------
void ZeeDReadTracksAOD::ReadInfo(ZeeDEvent* event)
{
  //  Info(__PRETTY_FUNCTION__,"start");
  this->Initialize();

  TObjArray* tracks = event->GetTracks();
  tracks->Clear();
  
  Rec::TrackParticleContainer::const_iterator trackIter = fTrackContainer->begin();
  Rec::TrackParticleContainer::const_iterator trackEnd = fTrackContainer->end();  
  
  for ( ; trackIter != trackEnd; ++trackIter ){ 
    
    if ( (*trackIter)->pt() < 500. ) continue;
    
    fTrackInProcess = new ZeeDTrack();
    fTrackInProcess->SetFourVector( ZeeDMisc::FormTLorentzVec( (*trackIter)->hlv() / GeV ) );

    const Trk::FitQuality * fFitQuality = (*trackIter)->fitQuality();
    if (fFitQuality){

      Double_t TrackChiSquared = (fFitQuality->chiSquared());
      fTrackInProcess->SetTrackChiSquared(TrackChiSquared);

      Double_t TrackNumberDoF = (fFitQuality->numberDoF());
      fTrackInProcess->SetTrackNumberDoF(TrackNumberDoF);
    }


    const Trk::MeasuredPerigee * fMeasuredPerigee = (*trackIter)->measuredPerigee();
    if (fMeasuredPerigee){
     
      Double_t TrackZ0 = (fMeasuredPerigee->parameters()[Trk::z0]);
      fTrackInProcess->SetTrackZ0( TrackZ0);
    
      Double_t TrackD0 = (fMeasuredPerigee->parameters()[Trk::d0]);
      fTrackInProcess->SetTrackD0( TrackD0);

      Double_t TrackqOverP = (fMeasuredPerigee->parameters()[Trk::qOverP]);
      fTrackInProcess->SetTrackqOverP( TrackqOverP);

      Double_t TrackTheta = (fMeasuredPerigee->parameters()[Trk::theta]);
      fTrackInProcess->SetTrackTheta( TrackTheta);

      Double_t TrackPhi0 = (fMeasuredPerigee->parameters()[Trk::phi0]);
      fTrackInProcess->SetTrackPhi0( TrackPhi0);
    }

    const Trk::TrackSummary * fSummary = (*trackIter)->trackSummary();
     if(fSummary){
      
      Double_t  TrackNumberOfPixelHits = (fSummary->get(Trk::numberOfPixelHits));
       fTrackInProcess->SetTrackNumberOfPixelHits( TrackNumberOfPixelHits);

       Double_t  TrackNumberOfSCTHits = (fSummary->get(Trk::numberOfSCTHits));
       fTrackInProcess->SetTrackNumberOfSCTHits( TrackNumberOfSCTHits);

     
      Double_t  TrackNumberOfPixelHoles = (fSummary->get(Trk::numberOfPixelHoles));
       fTrackInProcess->SetTrackNumberOfPixelHoles( TrackNumberOfPixelHoles);

       Double_t  TrackNumberOfSCTHoles = (fSummary->get(Trk::numberOfSCTHoles));
       fTrackInProcess->SetTrackNumberOfSCTHoles( TrackNumberOfSCTHoles);

       Double_t  TrackNumberOfTRTHoles = (fSummary->get(Trk::numberOfTRTHoles));
       fTrackInProcess->SetTrackNumberOfTRTHoles( TrackNumberOfTRTHoles);

     } else{
       Double_t TrackNumberOfPixelHits = (-1);
	fTrackInProcess->SetTrackNumberOfPixelHits( TrackNumberOfPixelHits);
	
	Double_t TrackNumberOfSCTHits = (-1);
	fTrackInProcess->SetTrackNumberOfSCTHits( TrackNumberOfSCTHits);

     
      Double_t  TrackNumberOfPixelHoles = (-1);
       fTrackInProcess->SetTrackNumberOfPixelHoles( TrackNumberOfPixelHoles);

       Double_t  TrackNumberOfSCTHoles  = (-1);
       fTrackInProcess->SetTrackNumberOfSCTHoles( TrackNumberOfSCTHoles);

       Double_t  TrackNumberOfTRTHoles = (-1);
       fTrackInProcess->SetTrackNumberOfTRTHoles( TrackNumberOfTRTHoles);
     }


    tracks->AddLast( fTrackInProcess );
    fTrackInProcess = NULL; 
    
  }

  //  tracks->Sort();
   
  //  cout << "size = "<< tracks->GetEntries() << endl;
 
  //  Info(__PRETTY_FUNCTION__,"stop");
}




//-----------------------------------------------------
