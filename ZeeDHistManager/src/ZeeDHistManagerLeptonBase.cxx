#include "ZeeDHistManager/ZeeDHistManagerLeptonBase.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDBoson.h"
#include "ZeeDEvent/ZeeDBosonW.h"
#include "ZeeDEvent/ZeeDBosonZ.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

//#define USE_DEBUG_MESSAGES
#include "ZeeDMisc/ZeeDMisc.h"


using std::cout;
using std::endl;


//-----------------------------------------------------
void ZeeDHistManagerLeptonBase::Fill()
{
    // Get event
    const ZeeDEvent* event = GetEvent();

    // Event weight
    Double_t Weight = event->GetWeight();

    // Arrange loop over leptons:
    if (bIsAll) {
        const TObjArray* leptons = bIsElec ? event->GetElectrons() : event->GetMuons();
        if ( leptons != NULL ) FillLeptonHists(leptons, Weight);

    } else {
     TObjArray*  leptons = new TObjArray();
	 if (bIsZ) {
     	if ( event->GetCurrentBoson() != NULL ) {
         
        	leptons->Add((TObject*)(event->GetCurrentBoson()->GetFirstLep()));
         	leptons->Add((TObject*)(event->GetCurrentBoson()->GetSecondLep()));
         	if ( leptons != NULL ) FillLeptonHists( leptons, Weight);
		}
	 }else {
		 if ( event->GetCurrentBosonW() !=NULL ){
			 ZeeDBosonW* bos = (ZeeDBosonW*) event->GetCurrentBosonW();
			 leptons->Add((TObject*)(bos->GetLep()));
    		 if (leptons!=NULL) {
				 FillLeptonHists(leptons, Weight);
			 }
		 }
	 }
	 delete leptons;
	 }
     


}

