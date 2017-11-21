#include "ZeeDAnalysisCuts/ZeeDIdentifierZCC.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDAnalysisCuts/ZeeDIdentifier.h"
#include "ZeeDAnalysisCuts/ZeeDFinderZCC.h"


//------------------------------------------------------
void ZeeDIdentifierZCC::IdentifyRecBosons(ZeeDEvent* event)
{
    // Identify the boson candidate, do extra calculations, store the boson

    this->SetCurrentEvent (event);
    fBosonsAndWeights.clear();

    if(this->GetFinder() == NULL) return;

    // safe current boson - loop will change it for weight calculation
    const ZeeDBosonZ* currentBosonSafe = event->GetCurrentBoson();
    ZeeDFinderZCC* finder = (ZeeDFinderZCC*)this->GetFinder();
    
    //ZeeDFinderZCC* finder = (ZeeDFinderZCC*)this->GetFinder();
    TObjArray selectedBosons = finder->SelectBosons(event);
    //TObjArray selectedBosons = *event->GetZBosons(); 
    
    for(Int_t sb = 0; sb != selectedBosons.GetEntriesFast(); ++sb) {
        const ZeeDBosonZ* selectedBoson = static_cast<const ZeeDBosonZ*>(selectedBosons[sb]);
        fBosonsAndWeights.push_back(std::make_pair((const ZeeDBoson*) selectedBoson, 0));

    }
    
    // restore current boson
    event->SetCurrentBoson(currentBosonSafe);

}

