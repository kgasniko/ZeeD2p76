#include "ZeeDHistManager/ZeeDChargeAnalysisCutHistManagerZ.h"

#include <vector>
#include <string>

// ZeeD analysis includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorZCC.h"

// Hist managers:
#include "ZeeDHistManager/ZeeDHistManagerElectron.h"
#include "ZeeDHistManager/ZeeDHistManagerBoson.h"
#include "ZeeDHistManager/ZeeDHistManagerEvent.h"
// #include "ZeeDHistManager/ZeeDHistManagerElectronDetails.h"

//#define DEBUG_ON 0
#include "ZeeDMisc/ZeeDMisc.h"

std::vector<std::string> HELPER_insertCutAfter(std::vector<std::string> stages,
                                               const std::string& insertCutName,
                                               const std::string& afterCutName)
{

    // ATTENTION: CRASHES IF AFTERCUTNAME IS NOT IN STAGES
    stages.insert(std::find(stages.begin(),
                            stages.end(),
                            afterCutName) + 1,
                  insertCutName
                 );

    return stages;

}

void HELPER_addHistManagers(ZeeDCutHistManager* cutHistManager,
                            const std::vector<std::string>& stages,
                            const std::string& name)
{

    std::string stage;

    for(unsigned int s = 0; s != stages.size(); ++s) {

        TString bgrHistManagerName;

        //        bgrHistManagerName += s;
        //        bgrHistManagerName += "_";
        bgrHistManagerName += name;
        bgrHistManagerName += "_plus_";
        bgrHistManagerName += stages[s];

        // no plus for the very first one
        if(s != 0) {
            stage += "+";
        }

        stage += stages[s];

        TString prefix("Stage");
        prefix += s;
        prefix += "_";

        cutHistManager->AddMaskLoose(stage,
                                     new ZeeDHistManagerBoson(cutHistManager->getName() + "/" + prefix + "Boson_" + bgrHistManagerName)
                                    );
        cutHistManager->AddMaskLoose(stage,
                                     new ZeeDHistManagerElectron(cutHistManager->getName() + "/" + prefix + "Electron_" + bgrHistManagerName)
                                    );
        cutHistManager->AddMaskLoose(stage,
                                     new ZeeDHistManagerEvent(cutHistManager->getName() + "/" + prefix + "Event_" + bgrHistManagerName)
                                    );
        /*
        cutHistManager->AddMaskLoose(stage,
                                     new ZeeDZChargeIdHM(prefix+"ChargeIdHM_"+bgrHistManagerName)
                                     );

        cutHistManager->AddMaskLoose(stage,
                                     new ZeeDDetailHistManager(prefix+"DetailHM_"+bgrHistManagerName)
                                     );
        */

    }

}


void ZeeDChargeAnalysisCutHistManagerZ::BookCutHistManager()
{

    // Create/book cut selector:
    ZeeDAnalysisCutSelectorZCC* AnalysisCuts = new ZeeDAnalysisCutSelectorZCC("Z Analysis Selection");
    AnalysisCuts->BookCuts();
    this->SetCutSelector(AnalysisCuts);

    std::vector<std::string> Stages;

    Stages.push_back("ExistZ"); //
    Stages.push_back("MinNVertex");//
    Stages.push_back("OQMaps"); //

    Stages.push_back("TrackBothElecZ"); //

    // charge 2

    Stages.push_back("AuthorBothElecZ"); //

    Stages.push_back("EtaMaxBothElecZ"); //
    Stages.push_back("EtaCrackBothElecZ"); //

    Stages.push_back("PtMinBothElecZ"); //

    Stages.push_back("IsEMMediumOneElecZ"); //
    Stages.push_back("IsEMMediumBothElecZ"); //

    //    Stages.push_back("MaxTwoGoodElec");

    //    Stages.push_back("IsEMTightOneElecZ"); //
    //    Stages.push_back("IsEMTightBothElecZ"); //

    // ServiceHandle<IZeeDAnalysisSvc>* ZeeDAnaOptions = fSvcHelper.GetAnaSvc();

    // if( (*ZeeDAnaOptions)->UseTrigger() ) {
    //     Stages.push_back("TrigDefaultEvent"); //
    // }


    //    Stages.push_back("ZMassZ"); //9

    std::vector<std::string> BothPositive_Stages =
        HELPER_insertCutAfter(Stages,
                              "ChargeBothElecPositiveZ",
                              "TrackBothElecZ");

    std::vector<std::string> BothNegative_Stages =
        HELPER_insertCutAfter(Stages,
                              "ChargeBothElecNegativeZ",
                              "TrackBothElecZ");

    std::vector<std::string> BothOpposite_Stages =
        HELPER_insertCutAfter(Stages,
                              "ChargeBothElecOppositeZ",
                              "TrackBothElecZ");

    std::vector<std::string> BothPositive_Stages_MassCut;
    BothPositive_Stages_MassCut.push_back("ZMassZCharge");
    BothPositive_Stages_MassCut.insert(BothPositive_Stages_MassCut.end(),
                                       BothPositive_Stages.begin(),
                                       BothPositive_Stages.end());

    std::vector<std::string> BothNegative_Stages_MassCut;
    BothNegative_Stages_MassCut.push_back("ZMassZCharge");
    BothNegative_Stages_MassCut.insert(BothNegative_Stages_MassCut.end(),
                                       BothNegative_Stages.begin(),
                                       BothNegative_Stages.end());

    std::vector<std::string> BothOpposite_Stages_MassCut;
    BothOpposite_Stages_MassCut.push_back("ZMassZCharge");
    BothOpposite_Stages_MassCut.insert(BothOpposite_Stages_MassCut.end(),
                                       BothOpposite_Stages.begin(),
                                       BothOpposite_Stages.end());

    std::vector<std::string> BothPositive_Stages_MassCut_LowerBoundOnly;
    BothPositive_Stages_MassCut_LowerBoundOnly.push_back("ZMassZChargeLowerBoundOnly");
    BothPositive_Stages_MassCut_LowerBoundOnly.insert(BothPositive_Stages_MassCut_LowerBoundOnly.end(),
                                                      BothPositive_Stages.begin(),
                                                      BothPositive_Stages.end());

    std::vector<std::string> BothNegative_Stages_MassCut_LowerBoundOnly;
    BothNegative_Stages_MassCut_LowerBoundOnly.push_back("ZMassZChargeLowerBoundOnly");
    BothNegative_Stages_MassCut_LowerBoundOnly.insert(BothNegative_Stages_MassCut_LowerBoundOnly.end(),
                                                      BothNegative_Stages.begin(),
                                                      BothNegative_Stages.end());

    std::vector<std::string> BothOpposite_Stages_MassCut_LowerBoundOnly;
    BothOpposite_Stages_MassCut_LowerBoundOnly.push_back("ZMassZChargeLowerBoundOnly");
    BothOpposite_Stages_MassCut_LowerBoundOnly.insert(BothOpposite_Stages_MassCut_LowerBoundOnly.end(),
                                                      BothOpposite_Stages.begin(),
                                                      BothOpposite_Stages.end());

    HELPER_addHistManagers(this,
                           BothPositive_Stages,
                           "BothPositive");

    HELPER_addHistManagers(this,
                           BothNegative_Stages,
                           "BothNegative");

    HELPER_addHistManagers(this,
                           BothOpposite_Stages,
                           "BothOpposite");

    HELPER_addHistManagers(this,
                           BothPositive_Stages_MassCut,
                           "BothPositive_MassCut");

    HELPER_addHistManagers(this,
                           BothNegative_Stages_MassCut,
                           "BothNegative_MassCut");

    HELPER_addHistManagers(this,
                           BothOpposite_Stages_MassCut,
                           "BothOpposite_MassCut");

    HELPER_addHistManagers(this,
                           BothPositive_Stages_MassCut_LowerBoundOnly,
                           "BothPositive_MassCut_LowerBoundOnly");

    HELPER_addHistManagers(this,
                           BothNegative_Stages_MassCut_LowerBoundOnly,
                           "BothNegative_MassCut_LowerBoundOnly");

    HELPER_addHistManagers(this,
                           BothOpposite_Stages_MassCut_LowerBoundOnly,
                           "BothOpposite_MassCut_LowerBoundOnly");

}
