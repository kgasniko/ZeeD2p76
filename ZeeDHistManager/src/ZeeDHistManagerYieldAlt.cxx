#include "ZeeDHistManager/ZeeDHistManagerYieldAlt.h"

//#define USE_DEBUG_MESSAGES
#include "ZeeDMisc/ZeeDMisc.h"

#include "ZeeDEvent/ZeeDEvent.h"

#include <fstream>
#include <utility>

#include <TSystem.h>

#include <iostream>

//-----------------------------------------------------
ZeeDHistManagerYieldAlt::ZeeDHistManagerYieldAlt(TString name)
  : ZeeDHistManager(name),
    fSvcHelper(name),
    fAnaOptions(fSvcHelper.GetAnaSvc())
{

    std::ifstream fin(ZeeDMisc::FindFile("RunLumi/RunLumi.txt").Data());

    UInt_t run;
    Double_t lumi;

    while (fin >> run >> lumi) {
        fRunLumiMap.insert(std::make_pair(run,lumi));
    }

}

//-----------------------------------------------------
void ZeeDHistManagerYieldAlt::BookHistos()
{
    DEBUG_MES_START;

    AddTH1D("Yield",   1, 0.0,  1, "Yield", "");
    AddTH1D("Nevents", 1, 0.0,  1, "Nevents", "");

    DEBUG_MES_END;
}

//-----------------------------------------------------
void ZeeDHistManagerYieldAlt::Fill()
{
    DEBUG_MES_START;
    DEBUG_MES(this->GetHMName());


    // Get event
    const ZeeDEvent* event = GetEvent();
    CHECK_NULL_PTR(event);

    // Event run, lb, weight
    const UInt_t run = event->RunNumber().Get();
    const Double_t weight = event->GetWeight();

    runLumiMap_const_it rlm_it = fRunLumiMap.find(run);

    if((*fAnaOptions)->DoManualGRL() && rlm_it == fRunLumiMap.end()) {

        Error("ZeeDHistManagerYieldAlt::Fill",
              "No lumi found for run %d!", run);

        Error("ZeeDHistManagerYieldAlt::Fill",
              "Dumping run lumi map:");

        for(runLumiMap_const_it it = fRunLumiMap.begin();
            it != fRunLumiMap.end();
            ++it) {

            std::cerr << "    run: " << it->first << " lumi: " << it->second << std::endl;

        }

        gSystem->Exit(EXIT_FAILURE);

    }

    const Double_t lumi = rlm_it->second;

    TString runString;
    runString += run;

    FillTH1(runString, weight/lumi, "Yield");
    FillTH1(runString, weight, "Nevents");

    DEBUG_MES_END;
}



