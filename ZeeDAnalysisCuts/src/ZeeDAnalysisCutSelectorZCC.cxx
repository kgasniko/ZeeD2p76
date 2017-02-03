
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorZCC.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisZBosonCuts.h"

#include "ZeeDTools/ZeeDBags.h"

void ZeeDAnalysisCutSelectorZCC::BookCuts()
{

    //  ----------------------------------------
    // get handle to fast sim service
    std::string name = std::string("ZeeDAnalysisSvc");

    ServiceHandle<IZeeDAnalysisSvc> ZeeDAnaOptions(name, "ZeeDCalcZ");
    StatusCode sc = ZeeDAnaOptions.retrieve();

    if ( sc.isFailure() ) {
        throw std::runtime_error("ZeeDCalcZ: cannot retrieve " + name + " service");
    }

    //Double_t elecPtCut   = ZeeDAnaOptions->Elec_PtCut();
    //Double_t elecEtaCut  = ZeeDAnaOptions->Elec_EtaCut();
    //Double_t elecVtxCut  = ZeeDAnaOptions->Elec_DZVtxCut();

    //Double_t ZMassCutMin = ZeeDAnaOptions->ZMassCutLooseMin();
    //Double_t ZMassCutMax = ZeeDAnaOptions->ZMassCutLooseMax();

    //Double_t EtaCrackMin = ZeeDAnaOptions->EtaCrackEMBEMEC_Min();
    //Double_t EtaCrackMax = ZeeDAnaOptions->EtaCrackEMBEMEC_Max();

    //Double_t vertexZCut = ZeeDAnaOptions->PriVertexZCut();

    Int_t    minNumTrkCut  = ZeeDAnaOptions->MinNumTrkCut();

    //Bool_t   bookExtraCuts = ZeeDAnaOptions->AnalysisCutSelectorBookExtraCuts();


    //********************************************
    //  BASELINE SELECTION
    //

    // Two electrons with Mee>30 are present:
    this->AddCut(new ZeeDCutExistZ("ExistZ") );

    // Author of both electons is from cluster algorithm
    //this->AddCut(new ZeeDCutAuthorBothElecZ("AuthorBothElecZ"));

    // Pt of both electrons is bigger than elecPtCut
    this->AddCut(new ZeeDCutAsymmetricalLepPt("PtMinBothElecZ", 20., 20.));

    // Max |Eta| over two electrons is smaller than elecEtaCut
    this->AddCut(new ZeeDCutEtaMaxBothElecZ("EtaMaxBothElecZ", 2.47));

    // Both Electrons are outside Eta crack defined by EtaCrackMin,EtaCrackMax
    this->AddCut(new ZeeDCutEtaCrackBothElecZ("EtaCrackBothElecZ", 1.37, 1.52));
    //if(ZeeDAnaOptions->ExtraCrackCut()) {
    //    this->AddCut(new ZeeDCutEtaCrackBothElecZ("EtaCrack2BothElecZ", ZeeDAnaOptions->ExtraCrackCutMin(), ZeeDAnaOptions->ExtraCrackCutMax()));
    //}

	this->AddCut(new ZeeDCutTrigger("EF_e15_loose1", ZeeDEnum::Trigger::EF_e15_loose1));
	this->AddCut(new ZeeDCutTriggerWithMatching("ElecTrig", ZeeDEnum::Trigger::EF_e15_loose1));
    // Medium cut:
    this->AddCut(new ZeeDCutIsEMBothLepZ("IsEMMediumPPBothElecZ", &ZeeDLeptonBags::IsEM::isMediumPP));

//    this->AddCut(new ZeeDCutIsEMBothElecZ("IsEMMediumBothElecZ", &ZeeDElectronBags::IsEM::isMedium));

    // Both electrons in the good region of the calorimeter
    this->AddCut(new ZeeDCutOQMaps("OQMaps"));

    // Electrons have opposite charge
    this->AddCut(new ZeeDCutChargeBothLepZ("ChargeBothElecOppositeZ"));

    // Invariant mass of the two electrons falls into the mass window ZMassCutMin,ZMassCutMax
    this->AddCut(new ZeeDCutZMassZ("ZMassZ", 66, 116));

    // Cut on number of Tracks at Primary Vertex
    this->AddCut(new ZeeDCutPriVtxNtrack("NTracksAtPrimVtx", minNumTrkCut));

    // Veto on two medium electrons
    //this->AddCut(new ZeeDCutTwoGoodElec("MaxTwoGoodElec"));

    // Event missing Et
    this->AddCut(new ZeeDCutEventMaxMissEt("EventEtMiss", 25));

    // LAr hole veto:
    this->AddCut(new ZeeDCutLArEventVeto("ZeeDCutLArEventVeto"));
    if (ZeeDAnaOptions->IsMC() && ZeeDAnaOptions->FillGenInfo()) {
        this->AddCut(new ZeeDCutEtaMaxBothBornElecZ("GenEta", 2.47));
        this->AddCut(new ZeeDCutPtMinBothBornElecZ("GenPt", 20.));
        this->AddCut(new ZeeDCutMassBornBoson("GenMt", 66, 116));
        this->AddCut(new ZeeDCutEtaCrackBothBornElecZ("GenCrack", 1.37, 1.52));
        this->AddCut(new ZeeDCutEtaMaxBothBornElecZ("GenEtaNEW", 2.5));
        this->AddCut(new ZeeDCutPtMinBothBornElecZ("GenPtNEW", 25.));
         
        }
 
    
    //**************************************************************************
    //
    // Additional cuts for extra studies:
    //
    //*************************************************************************
    /*if ( bookExtraCuts) {
    // Both electrons have track
        this->AddCut(new ZeeDCutTrackBothElecZ("TrackBothElecZ"));

     
// Add calo and track isolation cuts:
    // Calo:
        this->AddCut(new ZeeDCutBothElecIso("Iso98Etcone20",&ZeeDElectronBags::EIso::Iso98Etcone20));
        // Track:
        this->AddCut(new ZeeDCutBothElecIso("Iso97Ptcone40",&ZeeDElectronBags::EIso::Iso97Ptcone40));

        // Isolation cut for forward electron
        this->AddCut(new ZeeDCutCentralElecCaloIso("CaloIso98", &ZeeDElectronBags::CaloIso::CaloIso98));

        // Z Vertex of the electrons is separated by less than elecVtxCut
        this->AddCut(new ZeeDCutVertexPresentBothElecZ("VertexPresentBothElecZ", elecVtxCut));



        this->AddCut(new ZeeDCutCentralElecCaloIso("CaloIso95", &ZeeDElectronBags::CaloIso::CaloIso95));
        this->AddCut(new ZeeDCutIsEMOneElecZ("IsEMLooseOneElecZ", &ZeeDElectronBags::IsEM::isLoose));
        this->AddCut(new ZeeDCutIsEMBothElecZ("IsEMLooseBothElecZ", &ZeeDElectronBags::IsEM::isLoose));

        this->AddCut(new ZeeDCutIsEMOneElecZ("IsEMLoosePPOneElecZ", &ZeeDElectronBags::IsEM::isLoosePP));
        this->AddCut(new ZeeDCutIsEMBothElecZ("IsEMLoosePPBothElecZ", &ZeeDElectronBags::IsEM::isLoosePP));

        this->AddCut(new ZeeDCutIsEMOneElecZ("IsEMMediumOneElecZ", &ZeeDElectronBags::IsEM::isMedium));

        this->AddCut(new ZeeDCutIsEMOneElecZ("IsEMMediumPPOneElecZ", &ZeeDElectronBags::IsEM::isMediumPP));


        this->AddCut(new ZeeDCutIsEMOneElecZ("IsEMFailMediumPPOneElecZ", &ZeeDElectronBags::IsEM::isMediumPP, kFALSE));

        this->AddCut(new ZeeDCutIsEMOneElecZ("IsEMTightOneElecZ", &ZeeDElectronBags::IsEM::isTight));
        this->AddCut(new ZeeDCutIsEMBothElecZ("IsEMTightBothElecZ", &ZeeDElectronBags::IsEM::isTight));

        this->AddCut(new ZeeDCutIsEMOneElecZ("IsEMTightPPOneElecZ", &ZeeDElectronBags::IsEM::isTightPP));
        this->AddCut(new ZeeDCutIsEMBothElecZ("IsEMTightPPBothElecZ", &ZeeDElectronBags::IsEM::isTightPP));


        this->AddCut(new ZeeDCutIDCutOneElecZ("IDCutFailTrackTRTratioOneElecZ" , &ZeeDElectronBags::IDCut::TrackTRTratio, kFALSE));
        this->AddCut(new ZeeDCutIDCutOneElecZ("IDCutFailClusterMiddleEratio37OneElecZ"  , &ZeeDElectronBags::IDCut::ClusterMiddleEratio37, kFALSE));
        this->AddCut(new ZeeDCutIDCutOneElecZ("IDCutFailHadronicLeakageOneElecZ"        , &ZeeDElectronBags::IDCut::ClusterHadronicLeakage, kFALSE));
        this->AddCut(new ZeeDCutIDCutOneElecZ("IDCutFailClusterStripsWtotOneElecZ"      , &ZeeDElectronBags::IDCut::ClusterStripsWtot, kFALSE));

        this->AddCut(new ZeeDCutRunNumber("Run189280",189280,189280));

        // Electrons have Same charge
        this->AddCut(new ZeeDCutSameChargeBothElecZ("ChargeBothElecSameZ"));

        // Electrons have negative charge
        this->AddCut(new ZeeDCutChargeBothElecZ("ChargeBothElecNegativeZ"
                                            , false, kFALSE));

        // Electrons have positive charge
        this->AddCut(new ZeeDCutChargeBothElecZ("ChargeBothElecPositiveZ"
                                            , true, kTRUE));


        this->AddCut(new ZeeDCutZMassZ("ZMassZChargeLowerBoundOnly", 40, 1000));

        this->AddCut(new ZeeDCutZMassZCharge("ZMassZCharge"));


        // Z position of primary vertex |VtxZ| < 150mm
        this->AddCut(new ZeeDCutPriVtxZ("PriVtxZ", vertexZCut));

    }
*/
    /*
    // Use Trigger Cut
    if ( ZeeDAnaOptions->UseTrigger() ) {
      this->AddCut(new ZeeDCutTriggerWithMatching2012CC("Trigger_2e12loose1_OR_L2StarB", ZeeDAnaOptions->IsMC()));

      this->AddCut(new ZeeDCutTrigger("TrigDefaultEvent", ZeeDEnum::Trigger::DefaultSingleElec));

      if ( ZeeDAnaOptions->FillBkgHistograms() || ZeeDAnaOptions->BookBackgroundCuts() ) {

	this->AddCut(new ZeeDCutTrigger("Trigger_EF2g20_loose", ZeeDEnum::Trigger::EF_2g20_loose));


      }

      this->AddCut(new ZeeDCutTrigger("TrigDefaultDiElecEvent", ZeeDEnum::Trigger::DefaultDiElec));


      if ( ZeeDAnaOptions->DoTriggerMatching() ) {

	this->AddCut(new ZeeDCutTriggerWithMatching("TrigDefaultOneElecZ", ZeeDEnum::Trigger::DefaultSingleElec));
	this->AddCut(new ZeeDCutTriggerWithMatching("TrigDefaultDiElecBothElecZ", ZeeDEnum::Trigger::DefaultDiElec));

	this->AddCut(new ZeeDCutTriggerWithMatching("Trig_EF_2e12_medium_matching", ZeeDEnum::Trigger::EF_2e12_medium));

      }

    }
    

    if ( ZeeDAnaOptions->FillGenInfo() ) {
            // FIXME!
        // only a very little number of events survive with this cut ->
        // disabled for now
        //this->AddCut(new ZeeDCutTruthMatchBothElecZ());
    }

    if (ZeeDAnaOptions->IsMC() && ZeeDAnaOptions->FillGenInfo() ) {

        this->AddCut(new ZeeDCutExistBornBoson("ExistBornBoson") );

        this->AddCut(new ZeeDCutExistBornElectrons("ExistTwoBornElectrons", 2) );

        this->AddCut(new ZeeDCutPtMinBothBornElecZ("PtMinBothBornElecZ", elecPtCut));

        this->AddCut(new ZeeDCutEtaMaxBothBornElecZ("EtaMaxBothBornElecZ", elecEtaCut));

        this->AddCut(new ZeeDCutEtaMaxBothBornElecZ("EtaMaxFidBothBornElecZ", 2.5));

        this->AddCut(new ZeeDCutEtaCrackBothBornElecZ("EtaCrackBothBornElecZ", EtaCrackMin, EtaCrackMax));
        if(ZeeDAnaOptions->ExtraCrackCut()) {
            this->AddCut(new ZeeDCutEtaCrackBothBornElecZ("EtaCrack2BothBornElecZ", ZeeDAnaOptions->ExtraCrackCutMin(), ZeeDAnaOptions->ExtraCrackCutMax()));
        }

        this->AddCut(new ZeeDCutMassBornBoson("MassBornBoson", ZMassCutMin, ZMassCutMax));

        this->AddCut(new ZeeDCutMassBornBoson("MassGenBornBoson", ZeeDAnaOptions->GenBosMassMin(), ZeeDAnaOptions->GenBosMassMax(), ZeeDAnaOptions->GenBosMassMirrorCut()));

    }
    */

}
