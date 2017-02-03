#include "ZeeDCalculator/ZeeDReadEventD3PD.h"

#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDMisc/ZeeDMisc.h"
#include <TObjArray.h> 
#include <TSystem.h>


ZeeDReadEventD3PD::ZeeDReadEventD3PD() :
    fSvcHelper("ZeeDReadEventD3PD"),
    fAnaOptions(fSvcHelper.GetAnaSvc())
{
    Info("ZeeDReadEventD3PD::ZeeDReadEventD3PD", " Initialisation ");

}

//------------------------------------------------------                                                                                                                             
void ZeeDReadEventD3PD::ReadInfo(RawEvent* event)
{
}
/*
//------------------------------------------------------
void ZeeDReadEventD3PD::ReadEventInfo(RawEvent* event)
{


    event->EventNumber().Set(EventNumber);
    event->RunNumber().Set(RunNumber);
    event->OrigRunNumber().Set(RunNumber);
    event->LB().Set(lbn);
    
}

//------------------------------------------------------
void ZeeDReadEventD3PD::ReadElecInfo(RawEvent* event)
{    
  
    TObjArray* electrons = event->GetElectrons();
    electrons->Clear();
        
    std::cout << "Vector size: " << el_charge->size() << "\n";
    for (unsigned int i = 0; i <el_charge->size(); i++) {      
        ZeeDElectron* fElectronInProcess = new ZeeDElectron();
    
	fElectronInProcess->setHasCharge(true);
        fElectronInProcess->setCharge( (*el_charge)[i] );
	std::cout << (*el_charge)[i] << "  
    }
    std::cout << "End \n \n ";

*/
