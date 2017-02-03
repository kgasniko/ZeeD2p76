#include "ZeeDHistManager/ZeeDHistManagerJetBase.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDJet.h"
#include "ZeeDEvent/ZeeDBosonZ.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

//#define USE_DEBUG_MESSAGES
#include "ZeeDMisc/ZeeDMisc.h"


using std::cout;
using std::endl;


//-----------------------------------------------------


void ZeeDHistManagerJetBase::Fill()
{
    DEBUG_MES_START;
    DEBUG_MES(this->GetHMName());

    // Get event
    const ZeeDEvent* event = GetEvent();
    CHECK_NULL_PTR(event);

    // Event weight
    Double_t Weight = event->GetWeight();



    // Arrange loop over electrons:
    const TObjArray* jets=  event->GetJets();

    if (jets != NULL ) {
        FillJetHists(jets, Weight);
    }

    DEBUG_MES_END;
}
