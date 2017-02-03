#include "ZeeDAnalysisCuts/ZeeDIdentifierW.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDAnalysisCuts/ZeeDIdentifier.h"
#include "ZeeDAnalysisCuts/ZeeDFinderW.h"


//------------------------------------------------------
void ZeeDIdentifierW::IdentifyRecBosons(ZeeDEvent* event)
{
    // Identify the boson candidate, do extra calculations, store the boson

    this->SetCurrentEvent (event);
    fBosonsAndWeights.clear();

    if(this->GetFinder() == NULL) return;

    // safe current boson - loop will change it for weight calculation
    const ZeeDBosonW* currentBosonSafe = event->GetCurrentBosonW();
    
    ZeeDFinderW* finder = (ZeeDFinderW*)this->GetFinder();
    TObjArray selectedBosons = finder->SelectBosons(event);
    //TObjArray selectedBosons = *event->GetWBosons(); 

    event->SetNGoodW(selectedBosons.GetEntriesFast());
    
    for(Int_t sb = 0; sb != selectedBosons.GetEntriesFast(); ++sb) {
        const ZeeDBosonW* selectedBoson = static_cast<const ZeeDBosonW*>(selectedBosons[sb]);

        fBosonsAndWeights.push_back(std::make_pair((const ZeeDBoson*) selectedBoson, 0));

    }
    
    // restore current boson
    event->SetCurrentBosonW(currentBosonSafe);

}

