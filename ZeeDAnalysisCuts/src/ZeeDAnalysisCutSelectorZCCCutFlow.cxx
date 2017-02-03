
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorZCCCutFlow.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisZBosonCuts.h"

#include "ZeeDTools/ZeeDBags.h"

void ZeeDAnalysisCutSelectorZCCCutFlow::BookCuts()
{

    ServiceHandle<IZeeDAnalysisSvc> ZeeDAnaOptions("ZeeDAnalysisSvc", "ZeeDCalcZ");
    StatusCode sc = ZeeDAnaOptions.retrieve();

    if ( sc.isFailure() ) {
        throw std::runtime_error("ZeeDAnalysisCutSelectorZCCCutFlow: cannot retrieve ZeeDAnalysisSvc");
    }

    Double_t elecPtCut   = ZeeDAnaOptions->Elec_PtCut();
    Double_t elecEtaCut  = ZeeDAnaOptions->Elec_EtaCut();

    Double_t ZMassCutMin = ZeeDAnaOptions->ZMassCutLooseMin();
    Double_t ZMassCutMax = ZeeDAnaOptions->ZMassCutLooseMax();

    Double_t EtaCrackMin = ZeeDAnaOptions->EtaCrackEMBEMEC_Min();
    Double_t EtaCrackMax = ZeeDAnaOptions->EtaCrackEMBEMEC_Max();

    Int_t    minNumTrkCut  = ZeeDAnaOptions->MinNumTrkCut();

    this->AddCut(new ZeeDCutPriVtxNtrack("NTracksAtPrimVtx", minNumTrkCut));

    this->AddCut(new ZeeDCutExistZ("ExistZ") );

    this->AddCut(new ZeeDCutAuthorBothElecZ("AuthorBothElecZ"));
    this->AddCut(new ZeeDCutTrackBothLepZ("TrackBothElecZ"));

    this->AddCut(new ZeeDCutPtMinBothLepZ("PtMinBothElecZ", elecPtCut));

    this->AddCut(new ZeeDCutEtaMaxBothElecZ("EtaMaxBothElecZ", elecEtaCut));
    this->AddCut(new ZeeDCutEtaCrackBothElecZ("EtaCrackBothElecZ", EtaCrackMin, EtaCrackMax));
    if(ZeeDAnaOptions->ExtraCrackCut()) {
        this->AddCut(new ZeeDCutEtaCrackBothElecZ("EtaCrack2BothElecZ", ZeeDAnaOptions->ExtraCrackCutMin(), ZeeDAnaOptions->ExtraCrackCutMax()));
    }

    this->AddCut(new ZeeDCutIsEMBothLepZ("IsEMMediumPPBothElecZ", &ZeeDLeptonBags::IsEM::isMediumPP));

    this->AddCut(new ZeeDCutLArEventVeto("ZeeDCutLArEventVeto"));

    this->AddCut(new ZeeDCutTriggerLArHoleBugBothElecZ("TriggerLArHoleBug"));

    this->AddCut(new ZeeDCutOQMaps("OQMaps"));

    this->AddCut(new ZeeDCutChargeBothLepZ("ChargeBothElecOppositeZ"));
    this->AddCut(new ZeeDCutZMassZ("ZMassZ", ZMassCutMin, ZMassCutMax));

    this->AddCut(new ZeeDCutTwoGoodElec("MaxTwoGoodElec"));

    if ( ZeeDAnaOptions->UseTrigger() ) {

        this->AddCut(new ZeeDCutTrigger("TrigDefaultDiElecEvent", ZeeDEnum::Trigger::DefaultDiElec));

        if ( ZeeDAnaOptions->DoTriggerMatching() ) {
            this->AddCut(new ZeeDCutTriggerWithMatching("TrigDefaultDiElecBothElecZ", ZeeDEnum::Trigger::DefaultDiElec));
        }

    }

    /*

    const std::string allPeriods = "DEFGHIJKLM";

    for( std::string::const_iterator it=allPeriods.begin();
         it != allPeriods.end();
         ++it ) {

        const TString period = *it;

        this->AddCut(new ZeeDCutPeriodEvent(period, "data11_7TeV_"+period));

    }

    */

}
