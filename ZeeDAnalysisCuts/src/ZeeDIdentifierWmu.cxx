#include "ZeeDAnalysisCuts/ZeeDIdentifierWmu.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDAnalysisCuts/ZeeDIdentifier.h"
#include "ZeeDAnalysisCuts/ZeeDFinderWmu.h"


//------------------------------------------------------
void ZeeDIdentifierWmu::IdentifyRecBosons(ZeeDEvent* event)
{
    // Identify the boson candidate, do extra calculations, store the boson

    this->SetCurrentEvent (event);
    fBosonsAndWeights.clear();

    if(this->GetFinder() == NULL) return;

    // save current boson - loop will change it for weight calculation
    const ZeeDBosonW* currentBosonSafe = event->GetCurrentBosonW();
    
    ZeeDFinderWmu* finder = (ZeeDFinderWmu*)this->GetFinder();
    TObjArray selectedBosons = finder->SelectBosons(event);
    //TObjArray selectedBosons = *event->GetWmuBosons();
    //TODO make a flag for 3 different options: all of good bosons, best boson and all bosons in event
    //std::cout << "N Wmu = " << selectedBosons.GetEntriesFast() << std::endl; 
    for(Int_t sb = 0; sb != selectedBosons.GetEntriesFast(); ++sb) {
        const ZeeDBosonW* selectedBoson = static_cast<const ZeeDBosonW*>(selectedBosons[sb]);
        fBosonsAndWeights.push_back(std::make_pair((const ZeeDBoson*) selectedBoson, 0));
    }
    
    // restore current boson
    event->SetCurrentBosonW(currentBosonSafe);

}

