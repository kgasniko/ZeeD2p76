#include "ZeeDHistManager/ZeeDControlHistManagerW.h"

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
ZeeDControlHistManagerW::ZeeDControlHistManagerW(TString name) : ZeeDHistManager(name)
{
    // Named constructor
}

//------------------------------------------------------
ZeeDControlHistManagerW::~ZeeDControlHistManagerW()
{
    // Destructor

}

//------------------------------------------------------
void ZeeDControlHistManagerW::BookHistos()
{
    // Books histogram
    AddTH1D("nevents",   2,   0,   2, "number of events");

}

//------------------------------------------------------
void ZeeDControlHistManagerW::Fill()
{
    // Fills histos
    DEBUG_MES_START;



    // ----- Calculate event quantities ----------------
    // Get event
    const ZeeDEvent* event = GetEvent();
    CHECK_NULL_PTR(event);
        const Double_t Weight = event->GetWeight();
    FillTH1(1, Weight, "nevents");    
    
    DEBUG_MES_END;
}
