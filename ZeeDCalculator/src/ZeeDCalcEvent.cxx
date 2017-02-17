#include "ZeeDCalculator/ZeeDCalcEvent.h"

// ZeeD analysis includes
#include "ZeeDCalculator/ZeeDCalcElec.h"
#include "ZeeDCalculator/ZeeDCalcMuon.h"
#include "ZeeDCalculator/ZeeDCalcZ.h"
#include "ZeeDCalculator/ZeeDCalcW.h"
#include "ZeeDCalculator/ZeeDCalcEventInfo.h"
#include "ZeeDCalculator/ZeeDCalcJets.h"
#include "ZeeDCalculator/ZeeDCalcEtMiss.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDEvent/ZeeDEvent.h"

// Athena includes
//#include "EventInfo/EventInfo.h"
//#include "EventInfo/EventID.h"

//#include "GaudiKernel/StatusCode.h"
//#include "GaudiKernel/ServiceHandle.h"

//------------------------------------------------------
ZeeDCalcEvent::ZeeDCalcEvent() : 
    fCalcElec(0),
    fCalcMuon(0),
    fCalcZ(0),
    fCalcW(0),
    fCalcEventInfo(0),
    fCalcJets(0),
    fCalcEtMiss(0),
    fSvcHelper("ZeeDCalcEvent"),
    fAnaOptions(fSvcHelper.GetAnaSvc())
{

    /*  Excerpt from TObjArray documentation:

        Note on ownership and copy:
        By default the TObjArray does not own the objects it points to and
        will not delete them unless explicitly asked (via a call to the
        Delete member function).   To assign ownership of the content to
        the array, call:
            myarr->SetOwner(kTRUE);
        When the array owns its content a call to Clear or the deletion of
        the array itself will lead to the deletion of its contents.
     */

    fSys = ZeeDSystematics::getInstance();

    toBeDeleted.SetOwner();

    // Calculators
    fCalcElec = new ZeeDCalcElec();
    toBeDeleted.Add(fCalcElec);

    fCalcMuon = new ZeeDCalcMuon();
    toBeDeleted.Add(fCalcMuon);

    fCalcZ = new ZeeDCalcZ();
    toBeDeleted.Add(fCalcZ);
  
    fCalcEventInfo = new ZeeDCalcEventInfo( );
    toBeDeleted.Add(fCalcEventInfo);

    fCalcJets = new ZeeDCalcJets( );
    toBeDeleted.Add(fCalcJets);
 
    fCalcEtMiss = new ZeeDCalcEtMiss( );
    toBeDeleted.Add(fCalcEtMiss);

    fCalcW = new ZeeDCalcW();
    toBeDeleted.Add(fCalcW);
    

}

//------------------------------------------------------
ZeeDCalcEvent::~ZeeDCalcEvent()
{
    // Destructor
    DEBUG_MES_START;

    // Remove all objects from the array AND delete
    // all heap based objects.
    toBeDeleted.Delete();

    DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDCalcEvent::Calculate(ZeeDEvent* event)
{ 
    // Calculate number of good vertices
    // It is necessary to be done before calculation of the electron
    fCalcEventInfo->CalcNGoodVerticies(event);


    // Calculate event properties
    fCalcElec->Calculate(event);
    fCalcMuon->Calculate(event);    
    // Calculate addition event information after electron information is filled
    fCalcEventInfo->Calculate(event);    

    // Calculate Z boson candidates
    if ( (*fAnaOptions)->CalcZ() ) {
        fCalcZ->Calculate(event);
    }

    // Calculate jets (recalibration)
    if ( (*fAnaOptions)->CalcJets() && ((*fAnaOptions)->RecalibrateJets() == true)) 
      {
	fCalcJets->Calculate(event);
      }

    // Calculate Missing ET
    if ( (*fAnaOptions)->CalcZ() || (*fAnaOptions)->CalcW() || (*fAnaOptions)->CalcJets() ) 
      {
	fCalcEtMiss->Calculate(event);
      }

    // Create W boson candidates
    if ( (*fAnaOptions)->CalcW() == true ) {
        fCalcW->Calculate(event);
    }
    

}
