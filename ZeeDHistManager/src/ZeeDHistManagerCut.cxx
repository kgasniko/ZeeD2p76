#include "ZeeDHistManager/ZeeDHistManagerCut.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"

#include "ZeeDEvent/ZeeDBosonW.h"
#include "ZeeDEvent/ZeeDTrack.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDJet.h"
#include "ZeeDEvent/ZeeDVertex.h"
#include "ZeeDEvent/ZeeDGenParticle.h"
#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDHistManagerCut::ZeeDHistManagerCut(TString name) : ZeeDHistManager(name)
{
    // Named constructor
}

//------------------------------------------------------
ZeeDHistManagerCut::~ZeeDHistManagerCut()
{
    // Destructor

}

//------------------------------------------------------
void ZeeDHistManagerCut::BookHistos()
{
    // Books histogram
    AddTH1D("nevents",   2,   0,   2, "number of events");
    AddTH1D("nbosons",   2,   0,   2, "number of bosons");
    AddTH1D("nGoodW",    10,  0,  10, "number of good W");
    AddTH1D("nGoodZ",  10,  0,  10, "number of good Z");
    
}

//------------------------------------------------------
void ZeeDHistManagerCut::Fill()
{
    // Fills histos
    DEBUG_MES_START;



    // ----- Calculate event quantities ----------------
    // Get event
    const ZeeDEvent* event = GetEvent();
    CHECK_NULL_PTR(event);
    const Double_t Weight = event->GetWeight();
    FillTH1(1, Weight, "nevents");    
    int nGoodW=event->GetNGoodW();
    int nGoodWmu = event->GetNGoodWmu();
//    FillTH1(nGoodW, Weight, "nGoodW");
//    FillTH1(nGoodWmu, Weight, "nGoodWmu");
    const ZeeDBosonW* bosonW = event->GetCurrentBosonW();
    const ZeeDBosonZ* bosonZ = event->GetCurrentBoson();
/*    if (bosonW == NULL && bosonZ == NULL ){
        return;
    }
    */
    if (bosonW != NULL){
        FillTH1(1, Weight, "nGoodW");
    }
    if (bosonZ != NULL){
        FillTH1(1, Weight, "nGoodZ");
    }
    
    DEBUG_MES_END;
}
