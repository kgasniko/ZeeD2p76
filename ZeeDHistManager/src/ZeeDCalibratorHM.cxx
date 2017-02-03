#include "ZeeDHistManager/ZeeDCalibratorHM.h"

// Std includes
#include <iostream>

// Root includes
#include "TObjArray.h"

// ZeeD Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDBosonZ.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"


//#define USE_DEBUG_MESSAGES
#include "ZeeDMisc/ZeeDMisc.h"

using std::cout;
using std::endl;



//------------------------------------------------------
ZeeDCalibratorHM::ZeeDCalibratorHM(TString name, double MassMin, double MassMax) : ZeeDHistManager(name)
{
    // Constructor
    fZeeCal            = NULL;

    fAllEvt            = 0;
    fSelEvt            = 0;
    fCalib_Method      = 0;
    fCalib_In          = "";
    fdirname           = name;

    ZMassMin = MassMin;
    ZMassMax = MassMax;
}

//------------------------------------------------------
ZeeDCalibratorHM::~ZeeDCalibratorHM()
{
    // Destructor
    if (fZeeCal != NULL ) {
        delete fZeeCal;
    }
}

//------------------------------------------------------
void ZeeDCalibratorHM::BookHistos()
{
    DEBUG_MES_START;
    // Books histograms

    // Get names from service
    ServiceHandle<IZeeDAnalysisSvc>* ZeeDAnaOptions = fSvcHelper.GetAnaSvc();

    fCalib_In                       = (*ZeeDAnaOptions)->CalibrationInFile();
    fCalib_Method                   = (*ZeeDAnaOptions)->SelectCalibMethod();


    fZeeCal = new ZeeDCalcCalib(ZeeDMisc::FindFile(fCalib_In).Data(), fdirname);

    DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDCalibratorHM::Fill()
{
    DEBUG_MES_START

    // Fills histograms
    CHECK_NULL_PTR(fZeeCal);

    // Get event
    const ZeeDEvent* Event = GetEvent();

    if ( Event->GetCurrentBoson() == NULL ) {
        return;
    }

    const ZeeDBosonZ* Z = Event -> GetCurrentBoson();

    // Return if no Z
    if (Z == NULL) {
        return;
    }

    // Get the 2 electrons:
    const ZeeDElectron* e1 =(ZeeDElectron*) Z->GetFirstLep();
    const ZeeDElectron* e2 =(ZeeDElectron*) Z->GetSecondLep();

    ++fAllEvt; // Counter for all events

    TLorentzVector* Elec1 = new TLorentzVector( (e1 -> GetFourVector()) * 1000 );
    TLorentzVector* Elec2 = new TLorentzVector( (e2 -> GetFourVector()) * 1000 );

    if ( ((*Elec1 + *Elec2).M() > ZMassMax * 1000) ||  ((*Elec1 + *Elec2).M() < ZMassMin * 1000) ) {
        return;
    }

    if (fCalib_Method == 4 ) {
        if ( fZeeCal->AddEvent(Elec1, Elec2) == 0) {
            ++fSelEvt; // Counter for events which were used for calibration
        }
    }

    if (fCalib_Method == 0 ) {
        fZeeCal->FillRefMassTable(Elec1, Elec2);
    }

    delete Elec1;
    delete Elec2;

    DEBUG_MES_END
}

//------------------------------------------------------
void ZeeDCalibratorHM::Finalize()
{
    // Get Calibration:
    if (fCalib_Method == 4 ) {
        fZeeCal->WriteInternalArrays();
        fZeeCal->GetCalibration();
        fZeeCal->WriteCalibration();

        Info("ZeeDCalibratorHM:Finalize",
             "name: %s all events: %i, used for calibration: %i", fdirname.c_str(), fAllEvt, fSelEvt);
    }

    if (fCalib_Method == 0 ) {
        fZeeCal->WriteRefMassTable();
    }

}

