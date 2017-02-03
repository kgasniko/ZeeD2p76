#include "ZeeDAnalysisCuts/ZeeDIdentifier.h"
#include "ZeeDEvent/ZeeDEvent.h"

//------------------------------------------------------
ZeeDIdentifier::ZeeDIdentifier(TString name)
    : TNamed(name, name),
      fFinder(NULL),
      fEvent(NULL),
      fCalcWeight(0)
{
    // Constructor
}

//------------------------------------------------------
ZeeDIdentifier::~ZeeDIdentifier()
{
    // Destructor
    fFinder->Delete();

}

//------------------------------------------------------
void ZeeDIdentifier::IdentifyRecBosons(ZeeDEvent* event)
{
    // Identify the boson candidate, do extra calculations, store the boson

    fEvent = event;
    fBosonsAndWeights.clear();

    if(this->fFinder == NULL) return;

    TObjArray selectedBosons = this->fFinder->SelectBosons(fEvent);

    // safe current boson - loop will change it for weight calculation
    const ZeeDBosonZ* currentBosonSafe = fEvent->GetCurrentBoson();

    for(Int_t sb = 0; sb != selectedBosons.GetEntriesFast(); ++sb) {

        const ZeeDBosonZ* selectedBoson = static_cast<const ZeeDBosonZ*>(selectedBosons[sb]);

        fBosonsAndWeights.push_back(std::make_pair((ZeeDBoson*)selectedBoson, 0));

    }

    // restore current boson
    fEvent->SetCurrentBoson(currentBosonSafe);

}

//------------------------------------------------------
void ZeeDIdentifier::IdentifyJets()
{

}

