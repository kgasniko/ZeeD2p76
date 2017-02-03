#include "ZeeDHistManager/ZeeDHistManagerElectron.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>
#include <TH1.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDGenParticle.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include "ZeeDMisc/ZeeDMisc.h"

#include "ZeeDTools/ZeeDCutBit.h"

using std::cout;
using std::endl;


//-----------------------------------------------------

void ZeeDHistManagerElectron::BookHistos()
{
    // Books histograms

    // Get analysis svc
    ServiceHandle<IZeeDAnalysisSvc>* ZeeDAnaOptions = fSvcHelper.GetAnaSvc();

    // Do we run on data or MC?
    bIsMC = (*ZeeDAnaOptions)->IsMC();


    // kinematics
    AddTH1D("ElecPt",  ZeeDMisc::FindFile("Binning/LepPt.txt") , "P_{T}^{elec} / GeV", "", ZeeDHistLevel::Systematics);
    AddTH1D("ElecEta", ZeeDMisc::FindFile("Binning/LepEta.txt"), "#eta^{elec}",           "", ZeeDHistLevel::Systematics);
    AddTH1D("ElecCharge",      5,  -2.5,  2.5, "Electron charge",       "" );

    AddTH1D("ElecPhi",        50, -200., 200., "#varphi_{elec} / deg",  "" );
    AddTH1D("ElecTheta",      50,    0., 200., "#theta_{elec} / deg",   "" );
    AddTH1D("ElecEOverP",     50,   0.0,  2.0, "E_{clust} / P_{track}", "" );

    AddTH1D("ElecDeltaPhi",   50,  -0.1,  0.1, "#varphi_{track} - #varphi_{clust} / rad", "");
    AddTH1D("ElecDeltaR",     50,  -0.1,  0.1, "#Delta R_{track, cluster}", "");
    AddTH1D("ElecDeltaEta",   50,  -0.1,  0.1, "#eta_{track} - #eta_{clust}", "");

    AddTH1D("ElecEmBit",      36,  -1.5, 34.5, "isEM bit pattern", "" );
    AddTH1D("ElecEmBitLoose",      36,  -1.5, 34.5, "isEM bit pattern", "" );
    AddTH1D("ElecEmBitMedium",      36,  -1.5, 34.5, "isEM bit pattern", "" );
    AddTH1D("ElecEmBitTight",      36,  -1.5, 34.5, "isEM bit pattern", "" );

    AddTH1D("ElecZvtx", 100, -200., 200., "ElecZvtx", "");
    AddTH1D("ElecXvtx", 100, -10., 10., "ElecXvtx", "");
    AddTH1D("ElecYvtx", 100, -10., 10., "ElecYvtx", "");


    // track
    AddTH1D("TrackPt",        50,  0.0, 100.0, "P_{T}^{track} / GeV", "");
    AddTH1D("TrackEta",       50,  -5.0,  5.0, "#eta^{track}",           "" );
    AddTH1D("TrackPhi",       50, -200., 200., "#varphi_{track} / deg",  "" );

    // cluster
    AddTH1D("ClustPt",        50,  0.0, 100.0, "P_{T}^{clust} / GeV", "");
    AddTH1D("ClustEta",       50,  -5.0,  5.0, "#eta^{clust}",           "" );
    AddTH1D("ClustPhi",       50, -200., 200., "#varphi_{clust} / deg",  "" );

    // MC
    if (bIsMC) {
        AddTH1D("ElecPtGen",   50,  0.0, 100.0, "P_{T}^{elec}_{gen} / GeV", "" );
        AddTH1D("ElecEtaGen",  50, -5.0,   5.0, "#eta^{elec}_{gen}"       , "" );
        AddTH2D("EoverEgenVsEta",    100, -5., 5., 80, -0.2, 0.2, "#eta_{elec}", "E / E_{gen} - 1");
        AddTH2D("EoverEgenBornVsEta", 100, -5., 5., 80, -0.2, 0.2, "#eta_{elec}", "E / E_{gen} - 1");
    }

    // Extra histos for low/high pt electrons:
    AddTH1D("ElecPtMin",       50,  0.0, 100.0, "P_{T}^{elec} / GeV", "");
    AddTH1D("ElecPtMax",       50,  0.0, 100.0, "P_{T}^{elec} / GeV", "");
    AddTH1D("ElecEtaMinPt",   100, -5.0,   5.0, "#eta^{elec}",        "");
    AddTH1D("ElecEtaMaxPt",   100, -5.0,   5.0, "#eta^{elec}",        "");

    // Extra histos for positive/negative charge electrons:
    AddTH1D("ElecPtPos",   50,  0.0, 100.0, "P_{T}^{elec} / GeV", "" );
    AddTH1D("ElecPtNeg",   50,  0.0, 100.0, "P_{T}^{elec} / GeV", "" );
    AddTH1D("ElecEtaPos", 100, -5.0,   5.0, "#eta^{elec}",        "" );
    AddTH1D("ElecEtaNeg", 100, -5.0,   5.0, "#eta^{elec}",        "" );

    // Trigger
    UInt_t nChains = (*ZeeDAnaOptions)->L1TriggerElecMatch().size();
    nChains += (*ZeeDAnaOptions)->L2TriggerElecMatch().size();
    nChains += (*ZeeDAnaOptions)->EFTriggerElecMatch().size();

    AddTH1D("Trigger",        nChains,    0.0,   nChains, "Chain", "Matched");

    AddTH2D("ElecEtaPhi", 50, -3, 3, 50, -200, 200, "#eta^{elec}", "#varphi^{elec} / deg");

    AddTH2D("ElecEtaPhiBadOQAOD", 50, -3, 3, 50, -200, 200, "#eta^{elec}", "#varphi^{elec} / deg");

    AddTH2D("ElecEtaPhiGoodOQAOD", 50, -3, 3, 50, -200, 200, "#eta^{elec}", "#varphi^{elec} / deg");

}


void ZeeDHistManagerElectron::FillLeptonHists(
    const TObjArray* electrons, Double_t Weight)
{
    // Fill histograms for electrons
    if (electrons == NULL) {
        return;
    }

    Double_t ptMax = 0;
    Double_t ptMin = FLT_MAX;
    Double_t etaMinPt = 0;
    Double_t etaMaxPt = 0;

    
    for ( Int_t i = 0; i < electrons->GetEntriesFast(); ++i ) {
        const ZeeDElectron* electron = static_cast< ZeeDElectron* > (electrons->At(i));

        const TLorentzVector& fourVector = electron->GetFourVector();
        //std::cout << fourVector.Px() << " " <<fourVector.Py() << " " << fourVector.Pz() << " " <<fourVector.E() << " " << std::endl;

        if (fourVector.Pt() < ptMin) {
            ptMin    = fourVector.Pt();
            etaMinPt = fourVector.Eta();
        }

        if (fourVector.Pt() > ptMax) {
            ptMax    = fourVector.Pt();
            etaMaxPt = fourVector.Eta();
        }

        FillTH1(fourVector.Phi()  * ZeeDMisc::Rad2Deg, Weight, "ElecPhi");
        FillTH1(fourVector.Theta()* ZeeDMisc::Rad2Deg, Weight, "ElecTheta");
        FillTH1(fourVector.Pt(),            Weight, "ElecPt");
        FillTH1(fourVector.Eta(),           Weight, "ElecEta");

        FillTH2(fourVector.Eta(), fourVector.Phi() * ZeeDMisc::Rad2Deg, Weight, "ElecEtaPhi");

	if (electron->Vertex().IsSet()) {
	  TVector3 elec_vtx = electron->Vertex().Get().position;
	  FillTH1(elec_vtx.Pz(), Weight, "ElecZvtx");
	  FillTH1(elec_vtx.Px(), Weight, "ElecXvtx");
	  FillTH1(elec_vtx.Py(), Weight, "ElecYvtx");
	}

        // electron has bad OQ
        if(electron->IsGoodOQAOD().IsSet() &&
           electron->IsGoodOQAOD().Get() == kFALSE) {
            FillTH2(fourVector.Eta(), fourVector.Phi() * ZeeDMisc::Rad2Deg, Weight, "ElecEtaPhiBadOQAOD");
        }

        // electron has good OQ
        if(electron->IsGoodOQAOD().IsSet() &&
           electron->IsGoodOQAOD().Get() == kTRUE) {
            FillTH2(fourVector.Eta(), fourVector.Phi() * ZeeDMisc::Rad2Deg, Weight, "ElecEtaPhiGoodOQAOD");
        }

        const TLorentzVector& Cluster = electron->Cluster().Get().fourVector;

        // Fill MC electron information

        if ( bIsMC && electron->MCTruth().IsSet() ) {


            const TLorentzVector& mcfourVector = electron->MCTruth().Get().fourVector;

            FillTH1(mcfourVector.Pt(),  Weight, "ElecPtGen");
            FillTH1(mcfourVector.Eta(), Weight, "ElecEtaGen");
            FillTH2(fourVector.Eta(),
                    Cluster.E() / mcfourVector.E() - 1., Weight, "EoverEgenVsEta");
        }

        if (bIsMC) {
            // Also Born level match:
            const ZeeDGenParticle* fElecBornMatch = electron->GetGenBornMatchedLepton();

            if (fElecBornMatch != NULL) {
                const TLorentzVector& mcBornFourVector = fElecBornMatch->GetMCFourVector();
                FillTH2(mcBornFourVector.Eta(), Cluster.E() / mcBornFourVector.E() - 1., Weight, "EoverEgenBornVsEta");
            }
        }

        // Fill track histos
        if ( electron->TrackParticle().IsSet() ) {

            const TLorentzVector& electronTrack   = electron->TrackParticle().Get().fourVector;

            FillTH1(electronTrack.Pt(),  Weight, "TrackPt");
            FillTH1(electronTrack.Eta(), Weight, "TrackEta");
            FillTH1(electronTrack.Phi()*ZeeDMisc::Rad2Deg, Weight, "TrackPhi");
        }

        // Fill cluster histos
        if ( electron->Cluster().IsSet() ) {

            const TLorentzVector& electronCluster = electron->Cluster().Get().fourVector;
            FillTH1(electronCluster.Pt(),  Weight, "ClustPt");
            FillTH1(electronCluster.Eta(), Weight, "ClustEta");
            FillTH1(electronCluster.Phi()*ZeeDMisc::Rad2Deg, Weight, "ClustPhi");
        }

        if ( electron->TrackParticle().IsSet() && electron->Cluster().IsSet() ) {
            FillTH1(electron->getCharge(), Weight, "ElecCharge");

            const TLorentzVector& electronCluster = electron->Cluster().Get().fourVector;

            const TLorentzVector& electronTrack   = electron->TrackParticle().Get().fourVector;

            // Fill E_cluster / P track
            FillTH1(electronCluster.E() / electronTrack.P(), Weight, "ElecEOverP");


            if (electron->getCharge() > 0) {
                FillTH1(fourVector.Pt(),            Weight, "ElecPtPos");
                FillTH1(fourVector.Eta(),           Weight, "ElecEtaPos");
            } else {
                FillTH1(fourVector.Pt(),            Weight, "ElecPtNeg");
                FillTH1(fourVector.Eta(),           Weight, "ElecEtaNeg");
            }

            // Get electron track
            const TLorentzVector&  Track   = electron->TrackParticle().Get().fourVector;

            Double_t DeltaEta = Track.Eta() - Cluster.Eta();
            Double_t DeltaPhi = Track.DeltaPhi( Cluster );
            Double_t DeltaR   = Track.DeltaR( Cluster );

            FillTH1(DeltaEta, Weight, "ElecDeltaEta");
            FillTH1(DeltaPhi, Weight, "ElecDeltaPhi");
            FillTH1(DeltaR,   Weight, "ElecDeltaR");

            if((*fAnaOptions)->FillBitsHists()) {
                this->FillBitsHist(electron->IsEM().Get().bits,        Weight, "ElecEmBit");
                this->FillBitsHist(electron->IsEM().Get().bits_Loose,  Weight, "ElecEmBitLoose");
                this->FillBitsHist(electron->IsEM().Get().bits_Medium, Weight, "ElecEmBitMedium");
                this->FillBitsHist(electron->IsEM().Get().bits_Tight,  Weight, "ElecEmBitTight");
            }
        }


        if ( electron->Trigger().IsSet() ) {

            const ZeeDLeptonBags::Trigger& triggerBag = electron->Trigger().Get();
            ZeeDLeptonBags::Trigger::TriggerMap::const_iterator iter = triggerBag.isMatchedToChain.begin();
            ZeeDLeptonBags::Trigger::TriggerMap::const_iterator iterEnd = triggerBag.isMatchedToChain.end();

            for(; iter != iterEnd; ++iter) {

                const ZeeDEnum::Trigger::Value chain = iter->first;
                bool    isMatchedTo = iter->second;

                FillTH1(ZeeDEnum::Trigger::ToString(chain), isMatchedTo * Weight, "Trigger");

            }

        }

    }

    // Fill max/min histos:
    FillTH1(ptMax,              Weight, "ElecPtMax");
    FillTH1(etaMaxPt,           Weight, "ElecEtaMaxPt");
    FillTH1(ptMin,              Weight, "ElecPtMin");
    FillTH1(etaMinPt,           Weight, "ElecEtaMinPt");

}

//-----------------------------------------------------
void ZeeDHistManagerElectron::FillBitsHist(UInt_t bits,
                                           Double_t Weight,
                                           const TString& hist) {

    Int_t bitPosition = 1;

    if ( bits == 0 ) {
        FillTH1( -1.0, Weight, hist);
    }

    if (bits > 0) {
        for (Int_t i = 0; i < 32; ++i) {

            if (bits & bitPosition) {
                FillTH1( i, Weight, hist);
            }

            bitPosition *= 2;

        }
    }

}
