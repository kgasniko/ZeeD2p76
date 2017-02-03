#include "ZeeDEvent/ZeeDTrack.h"

// std libraries
#include <iostream>

// ZeeD libraries
#include "ZeeDMisc/ZeeDMisc.h"
#define DEBUG_ON 0

//------------------------------------------------------
// Constructor
ZeeDTrack::ZeeDTrack() : fFourVector(0., 0., 0., 0.),
			 fTrackD0(0),
			 fTrackZ0(0),
			 fTrackNumberOfPixelHits(0), 
			 fTrackNumberOfSCTHits(0),
			 fTrackChiSquared(0.0),
			 fTrackNumberDoF(0),
			 fTrackNumberOfPixelHoles(0), 
			 fTrackNumberOfSCTHoles(0),
			 fTrackNumberOfTRTHoles(0),
			 fTrackqOverP(0.0),
			 fTrackTheta(0.0),
			 fTrackPhi0(0.0)
{}

//------------------------------------------------------
ZeeDTrack::~ZeeDTrack()
{
    // Destructor
}

//------------------------------------------------------
void ZeeDTrack::Print(Option_t* option) const
{
    // Prints jet content
    if ( !option || !option[0] ) {
        std::cout << option << std::endl;
    }

    ZeeDMisc::Dump4Vec("track", fFourVector);
}

