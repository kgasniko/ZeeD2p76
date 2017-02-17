#include "ZeeDHistManager/ZeeDHistManagerEvent.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>
#include <TH1.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDVertex.h"
#include "ZeeDTools/ZeeDBags.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

//#define USE_DEBUG_MESSAGES
#include "ZeeDMisc/ZeeDMisc.h"

#include "ZeeDTools/ZeeDCutBit.h"

//-----------------------------------------------------
void ZeeDHistManagerEvent::BookHistos()
{
    // Books histograms

    // Get analysis svc
    ServiceHandle<IZeeDAnalysisSvc>* ZeeDAnaOptions = fSvcHelper.GetAnaSvc();

    // Do we run on data or MC?
    if ( (*ZeeDAnaOptions)->IsMC() ) {
        bIsMC = kTRUE;
    }
    AddTH1D("nevents",       2, 0, 2, "nevents", "");
    //vertex histograms
    AddTH1D("VtxXPos",       50,    -1.,    1., "X_{vtx} [mm]",     "");
    AddTH1D("VtxYPos",       50,     0.,    2., "Y_{vtx} [mm]",     "");
    AddTH1D("VtxZPos",       50,  -200.,  200., "Z_{vtx} [mm]",     "");

    AddTH1D("Nvtx"      ,         6,     0.0,   6.0, "N_{vtx}",      "");
    AddTH1D("Mu"        ,        30,    -0.0,   1.0, "Mu"        , "Average Interactions Per Crossing");
    AddTH1D("ActualMu"  ,       100 ,    -0.0,   1.0, "Mu(actual)", "Actual  Interactions Per Crossing");
    
    AddTH1D("EtDensity_3EM",  200,      0.,    1., "#rho(3EM) [GeV]",         "Et density #rho 3EM");
    AddTH1D("EtDensity_3LC",  200,      0.,    1., "#rho(3LC) [GeV]",         "Et density #rho 3LC");
    AddTH1D("EtDensity_4EM",  200,      0.,    1., "#rho(4EM) [GeV]",         "Et density #rho 4EM");
    AddTH1D("EtDensity_4LC",  200,      0.,    1., "#rho(4LC) [GeV]",         "Et density #rho 4LC");
    
    AddTH1D("WMassTransv", 200, 0., 100., "M_{transv}", " M_{transverce} for W candidate");
    AddTH1D("WPTransv", 200, 0., 100., "P_{transv}", "P_{transverce} for W candidate");

    AddTH1D("VertexTracks",  50,     0.,  150., "# vertex tracks", "");
    AddTH1D("ZvDiff",       200,  -100.,  100., "Z_{vtx} - Z_{vtx}^{boson} [mm]", "");
    AddTH1D("EtMiss",      50,    0.0, 100.0, "E_{T}^{miss} [GeV]",     "" );
    UInt_t nChains = (*ZeeDAnaOptions)->TriggerEvent().size();

    AddTH1D("Trigger",        nChains,    0.0,   nChains, "Chain", "Passed");

    AddTH1D("EventWeight",        100 ,    -2.,   4.0,  "Event weight",    "");
    AddTH1D("EventWeightHuge",  10000,    -2e8,   2e8,  "Event weights. Huge range ",    "");
    AddTH1D("LAr",                  4,    -0.5,   3.5,  "LAr event flag",  "");

    AddTH1D("Period",      1,    0.0,  1, "Period", "");
    AddTH2D("mu_nVtx",    50,    0.0,  1.0,  6, 0.0,  6.0, "Mu", "nVtx");

    if(bIsMC) {

        AddTH1D("MCtype", 1, 0, 1, "MCtype", "");

    }

    // Run range histogram
    Int_t runfirst = (*ZeeDAnaOptions)->FirstRunHistogram();
    Int_t runlast  = (*ZeeDAnaOptions)->LastRunHistogram();
    
    AddTH1D("RunNumber", runlast - runfirst + 1, runfirst, runlast + 1, " Run Number", "");


    AddTH1D("LArErrorState", 40, -20, 20, "LArErrorState", "");

    if((*ZeeDAnaOptions)->DoElectronCounting()) {

        AddTH1D("NElectrons",          50,     -0.5,   49.5, "N_{Electrons}",         "");
        AddTH1D("NElectrons_medium",   50,     -0.5,   49.5, "N_{Electrons_medium}",  "");
        AddTH1D("NElectrons_tight",    50,     -0.5,   49.5, "N_{Electrons_tight}",   "");
        AddTH1D("NElectrons_loose",    50,     -0.5,   49.5, "N_{Electrons_loose}",   "");

        AddTH1D("NElectrons_positive",          50,     -0.5,   49.5, "N_{Electrons_positive}",         "");
        AddTH1D("NElectrons_medium_positive",   50,     -0.5,   49.5, "N_{Electrons_medium_positive}",  "");
        AddTH1D("NElectrons_tight_positive",    50,     -0.5,   49.5, "N_{Electrons_tight_positive}",   "");
        AddTH1D("NElectrons_loose_positive",    50,     -0.5,   49.5, "N_{Electrons_loose_positive}",   "");

        AddTH1D("NElectrons_negative",          50,     -0.5,   49.5, "N_{Electrons_negative}",         "");
        AddTH1D("NElectrons_medium_negative",   50,     -0.5,   49.5, "N_{Electrons_medium_negative}",  "");
        AddTH1D("NElectrons_tight_negative",    50,     -0.5,   49.5, "N_{Electrons_tight_negative}",   "");
        AddTH1D("NElectrons_loose_negative",    50,     -0.5,   49.5, "N_{Electrons_loose_negative}",   "");

        AddTH2D("NEl_pos_vs_neg", 50, -0.5, 49.5, 50, -0.5, 49.5, "NEl_pos", "NEl_neg");
        AddTH2D("NEl_loose_pos_vs_neg", 50, -0.5, 49.5, 50, -0.5, 49.5, "NEl_loose_pos", "NEl_loose_neg");
        AddTH2D("NEl_medium_pos_vs_neg", 50, -0.5, 49.5, 50, -0.5, 49.5, "NEl_medium_pos", "NEl_medium_neg");
        AddTH2D("NEl_tight_pos_vs_neg", 50, -0.5, 49.5, 50, -0.5, 49.5, "NEl_tight_pos", "NEl_tight_neg");

        AddTH2D("NEl_pos_vs_Nel", 50, -0.5, 49.5, 50, -0.5, 49.5, "NEl_pos", "NEl_Nel");
        AddTH2D("NEl_loose_pos_vs_Nel", 50, -0.5, 49.5, 50, -0.5, 49.5, "NEl_loose_pos", "NEl_loose_Nel");
        AddTH2D("NEl_medium_pos_vs_Nel", 50, -0.5, 49.5, 50, -0.5, 49.5, "NEl_medium_pos", "NEl_medium_Nel");
        AddTH2D("NEl_tight_pos_vs_Nel", 50, -0.5, 49.5, 50, -0.5, 49.5, "NEl_tight_pos", "NEl_tight_Nel");

        AddTH1D("NElectrons_pos_minus_neg",          50,     -25.5, 24.5, "N_{Electrons_pos_minus_neg}",         "");
        AddTH1D("NElectrons_medium_pos_minus_neg",   50,     -25.5, 24.5, "N_{Electrons_medium_pos_minus_neg}",  "");
        AddTH1D("NElectrons_tight_pos_minus_neg",    50,     -25.5, 24.5, "N_{Electrons_tight_pos_minus_neg}",   "");
        AddTH1D("NElectrons_loose_pos_minus_neg",    50,     -25.5, 24.5, "N_{Electrons_loose_pos_minus_neg}",   "");

        AddTH2D("NEl_pos_minus_neg_vs_NEl",       50, -25.5, 24.5, 50, -0.5, 49.5, "NEl_pos_minus_neg", "NEl");
        AddTH2D("NEl_loose_pos_minus_neg_vs_NEl", 50, -25.5, 24.5, 50, -0.5, 49.5, "NEl_loose_pos_minus_neg", "NEl");
        AddTH2D("NEl_medium_pos_minus_neg_vs_NEl",50, -25.5, 24.5, 50, -0.5, 49.5, "NEl_medium_pos_minus_neg", "NEl");
        AddTH2D("NEl_tight_pos_minus_neg_vs_NEl", 50, -25.5, 24.5, 50, -0.5, 49.5, "NEl_tight_pos_minus_neg", "NEl");

    }

    AddTH1D("NZBoson",          50,     -0.5,   49.5, "N_{ZBoson}",         "");
    AddTH1D("NWBoson",          50,     -0.5,   49.5, "N_{WBoson}",         "", ZeeDHistLevel::Technical);

}

//-----------------------------------------------------
void ZeeDHistManagerEvent::Fill()
{
    DEBUG_MES_START;
    DEBUG_MES(this->GetHMName());

    // Get analysis svc
    ServiceHandle<IZeeDAnalysisSvc>* ZeeDAnaOptions = fSvcHelper.GetAnaSvc();

    // ----- Define pointers to histograms -------------
    // todo: make "" variables lower case








    // Get event
    const ZeeDEvent* event = GetEvent();
    CHECK_NULL_PTR(event);

    // Event weight
    Double_t Weight = event->GetWeight();
    FillTH1(Weight, 1., "EventWeight");
    FillTH1(Weight, 1., "EventWeightHuge");
    FillTH1(Weight, 1., "nevents");
    

    // Get array of vertices
    const TObjArray* vertices = event->GetVertices();

    if ( event->Nvtx().IsSet() ) {
        Int_t NumVtx = event->Nvtx().Get();
        FillTH1(NumVtx, Weight, "Nvtx");
    }
    if (event->GetEtMissArray() != NULL ) {
         double etMiss= ((ZeeDEtMiss*) event->GetEtMissArray()->At(0))->GetEt();
         FillTH1 (etMiss, Weight, "EtMiss");
         }
    if ( event->AverageInteractionsPerCrossing().IsSet() ) {
      const Double_t Mu = event->AverageInteractionsPerCrossing().Get();
      FillTH1(Mu, Weight, "Mu");
      FillTH1(Mu, 1.0, "ActualMu");
      if (event->Nvtx().IsSet()) 
          FillTH2 (Mu, event->Nvtx().Get(), Weight, "mu_nVtx");
      }
    
    const ZeeDBosonW* w = event->GetW();
    if (w!= NULL) 
           FillTH1(w->GetMt(), Weight, "WMassTransv");
    
    FillTH1( event->GetEtDensity_3EM(), Weight, "EtDensity_3EM" );
    FillTH1( event->GetEtDensity_3LC(), Weight, "EtDensity_3LC" );
    FillTH1( event->GetEtDensity_4EM(), Weight, "EtDensity_4EM" );
    FillTH1( event->GetEtDensity_4LC(), Weight, "EtDensity_4LC" );

    // Take the first vertex
    // FIXME: A.N should it not be a primary event vertex(icies)
    //
    if ( vertices->GetEntriesFast() > 0 ) {
        ZeeDVertex* vertex = static_cast< ZeeDVertex* >( vertices->At(0) );
        TVector3 vtx3Vector = vertex->GetVertexCandidate();

        // Fill vertex position
        FillTH1(vtx3Vector.X(), Weight, "VtxXPos");
        FillTH1(vtx3Vector.Y(), Weight, "VtxYPos");
        FillTH1(vtx3Vector.Z(), Weight, "VtxZPos");

        // Fill number of vertex tracks
        Int_t numTracks = vertex->GetNTrack();
        FillTH1(numTracks, Weight, "VertexTracks");

        // Primary Vertex Z - Z Vertex Z
        const ZeeDBosonZ* boson = event->GetCurrentBoson();

        if ( boson != NULL) {
            TVector3 bosonVtx = boson->GetVertex();

            FillTH1(vtx3Vector.Z() - bosonVtx.Z(),  Weight, "ZvDiff");
        }
    }

    // Fill Run number
    if ( event->RunNumber().IsSet() ) {
        Int_t runNumber = event->RunNumber().Get();
        FillTH1(runNumber, Weight, "RunNumber");
    }

    if ( event->Trigger().IsSet() ) {

        const ZeeDEventBags::Trigger triggerBag = event->Trigger().Get();
        ZeeDEventBags::Trigger::TriggerMap::const_iterator iter = triggerBag.hasPassedChain.begin();
        ZeeDEventBags::Trigger::TriggerMap::const_iterator iterEnd = triggerBag.hasPassedChain.end();

        for(; iter != iterEnd; ++iter) {

            TString chain     = ZeeDEnum::Trigger::ToString(iter->first);
            bool    hasPassed = iter->second;
            //std::cout << "HasPassed =" << hasPassed << "  Weight of event =" << Weight << "   chain= " << chain <<std::endl;
            FillTH1(chain, hasPassed * Weight, "Trigger");

        }

    }

    const TString period = ZeeDEnum::Period::ToString(event->GetPeriod());

    FillTH1(period, Weight, "Period");

    if(bIsMC && event->MCtype().IsSet()) {


        const TString MCtype = ZeeDEnum::MCtype::ToString(event->MCtype().Get());

        FillTH1(MCtype, Weight, "MCtype");

    }

    if(event->LArErrorState().IsSet()) {

        const Int_t LArErrorState = event->LArErrorState().Get();
        FillTH1(LArErrorState, Weight, "LArErrorState");

    }

    if((*ZeeDAnaOptions)->DoElectronCounting()) {








        const TObjArray* elecArray = event->GetElectrons();
        //const Int_t nElectrons = elecArray->GetEntriesFast();
        //FillTH1(nElectrons, Weight, "NElectrons");

        Int_t nLoose  = 0;
        Int_t nMedium = 0;
        Int_t nTight  = 0;

        Int_t nLoose_positive  = 0;
        Int_t nMedium_positive = 0;
        Int_t nTight_positive  = 0;

        Int_t nLoose_negative  = 0;
        Int_t nMedium_negative = 0;
        Int_t nTight_negative  = 0;

        Int_t nPositive = 0;
        Int_t nNegative = 0;

        for(Int_t i=0; i != elecArray->GetEntriesFast(); ++i) {

            const ZeeDElectron* elec = static_cast<const ZeeDElectron*>(elecArray->At(i));

            if(elec->Author().IsSet() == kFALSE ||
               elec->Author().Get().isElectron == kFALSE) continue;

            const Bool_t chargeKnown = elec->TrackParticle().IsSet();
            const Double_t charge    = elec->getCharge();

            if(chargeKnown && charge > 0) {
                ++nPositive;
            } else {
                ++nNegative;
            }

            if(elec->IsEM().IsSet()) {

                if(elec->IsEM().Get().isLoose) {

                    ++nLoose;

                    if(chargeKnown && charge > 0) {
                        ++nLoose_positive;
                    } else {
                        ++nLoose_negative;
                    }

                }

                if(elec->IsEM().Get().isMedium) {

                    ++nMedium;

                    if(chargeKnown && charge > 0) {
                        ++nMedium_positive;
                    } else {
                        ++nMedium_negative;
                    }


                }

                if(elec->IsEM().Get().isTight) {

                    ++nTight;

                    if(chargeKnown && charge > 0) {
                        ++nTight_positive;
                    } else {
                        ++nTight_negative;
                    }

                }

            }

        }

        const Int_t nElectrons = nPositive + nNegative;
        FillTH1(nElectrons, Weight, "NElectrons");

        FillTH1(nLoose,  Weight, "NElectrons_loose");
        FillTH1(nMedium, Weight, "NElectrons_medium");
        FillTH1(nTight,  Weight, "NElectrons_tight");

        FillTH1(nPositive,  Weight, "NElectrons_positive");

        FillTH1(nLoose_positive,  Weight, "NElectrons_loose_positive");
        FillTH1(nMedium_positive, Weight, "NElectrons_medium_positive");
        FillTH1(nTight_positive,  Weight, "NElectrons_tight_positive");

        FillTH1(nNegative,  Weight, "NElectrons_negative");

        FillTH1(nLoose_negative,  Weight, "NElectrons_loose_negative");
        FillTH1(nMedium_negative, Weight, "NElectrons_medium_negative");
        FillTH1(nTight_negative,  Weight, "NElectrons_tight_negative");

        FillTH2(nPositive, nNegative,  Weight, "NEl_pos_vs_neg");
        FillTH2(nLoose_positive, nLoose_negative,  Weight, "NEl_loose_pos_vs_neg");
        FillTH2(nMedium_positive, nMedium_negative,  Weight, "NEl_medium_pos_vs_neg");
        FillTH2(nTight_positive, nTight_negative,  Weight, "NEl_tight_pos_vs_neg");

        FillTH2(nPositive, nNegative+nPositive,  Weight, "NEl_pos_vs_Nel");
        FillTH2(nLoose_positive, nLoose_negative+nLoose_positive,  Weight, "NEl_loose_pos_vs_Nel");
        FillTH2(nMedium_positive, nMedium_negative+nMedium_positive,  Weight, "NEl_medium_pos_vs_Nel");
        FillTH2(nTight_positive, nTight_negative+nTight_positive,  Weight, "NEl_tight_pos_vs_Nel");

        FillTH1(nPositive-nNegative,  Weight, "NElectrons_pos_minus_neg");

        FillTH1(nLoose_positive - nLoose_negative,  Weight, "NElectrons_loose_pos_minus_neg");
        FillTH1(nMedium_positive - nMedium_negative, Weight, "NElectrons_medium_pos_minus_neg");
        FillTH1(nTight_positive - nTight_negative,  Weight, "NElectrons_tight_pos_minus_neg");

        FillTH2(nPositive - nNegative, nElectrons,  Weight, "NEl_pos_minus_neg_vs_NEl");
        FillTH2(nLoose_positive - nLoose_negative, nLoose_positive + nLoose_negative,  Weight, "NEl_loose_pos_minus_neg_vs_NEl");
        FillTH2(nMedium_positive - nMedium_negative,  nMedium_positive + nMedium_negative, Weight, "NEl_medium_pos_minus_neg_vs_NEl");
        FillTH2(nTight_positive - nTight_negative, nTight_positive + nTight_negative, Weight, "NEl_tight_pos_minus_neg_vs_NEl");

    }

    FillTH1(event->GetZBosons()->GetEntriesFast(),  Weight, "NZBoson");
    FillTH1(event->GetWBosons()->GetEntriesFast(),  Weight, "NWBoson");

    DEBUG_MES_END;
}



