#include "ZeeDEvent/ZeeDGenParticle.h"
#include "ZeeDEvent/ZeeDElectron.h"

// std libraries
#include <iostream>

// ZeeD libraries
#include "ZeeDMisc/ZeeDMisc.h"
#define DEBUG_ON 0

//------------------------------------------------------
ZeeDElectron::ZeeDElectron()
{
    fCombinedFourVector.SetXYZT(0., 0., 0., 0.);
    fCollectionIndex = -1;
	fInComp=false;
	fleptonType = ZeeDLepton::LeptonType::electron;
}



//------------------------------------------------------
ZeeDElectron::~ZeeDElectron()
{
    // Destructor
}

