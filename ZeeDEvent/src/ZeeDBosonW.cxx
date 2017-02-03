#include "ZeeDEvent/ZeeDBosonW.h"

// std libraries
#include <iostream>

// ZeeD includes
#include "ZeeDEvent/ZeeDLepton.h"
#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDBosonW::ZeeDBosonW() :  ZeeDBoson(),
    fMt(0.0),
    fLepIdx(-1),
    fEtMiss(NULL), 
    fWeight(1)
{
    fLep = new TObjArray();
    fEtMiss = NULL;
    //fbosonType= BosonType::W;
    
    fPtPair.first= NULL;
    fPtPair.second= NULL;
    
    fPosNegPair.first = NULL;
    fPosNegPair.second= NULL;
    
    fCentralForwardPair.first = NULL;
    fCentralForwardPair.second = NULL;
    
    //fCosTHStarVec.resize(ZeeDENum::CosThStar::N);
       // Constructor
}

//------------------------------------------------------
ZeeDBosonW::~ZeeDBosonW()
{
    // Destructor
}

//------------------------------------------------------
void ZeeDBosonW::Print(Option_t* option) const
{
    // Prints out boson properties
    if ( !option || !option[0] ) {
        std::cout << option << std::endl;
    }

    const ZeeDLepton* elec = GetLep();
    std::cout << "Mt = " << fMt << std::endl;

    if (elec != NULL) {
        ZeeDMisc::Dump4Vec("electron ", elec->GetFourVector() );
    }

    if (fEtMiss != NULL) {
        fEtMiss->Print();
    }
}
