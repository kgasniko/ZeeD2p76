#include "ZeeDHistManager/ZeeDHistManagerJet.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>
#include <TH1.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDJet.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDGenParticle.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

//#define USE_DEBUG_MESSAGES
#include "ZeeDMisc/ZeeDMisc.h"


using std::cout;
using std::endl;


//-----------------------------------------------------

void ZeeDHistManagerJet::BookHistos()
{
    // Books histograms

    // Get analysis svc
    ServiceHandle<IZeeDAnalysisSvc>* ZeeDAnaOptions = fSvcHelper.GetAnaSvc();

    // Do we run on data or MC?
    if ( (*ZeeDAnaOptions)->IsMC() ) {
        bIsMC = kTRUE;
    }

    AddTH1D("JetPt", 50,  0.0, 100.0, "P_{T}^{jet} / GeV", "");
    AddTH1D("JetEta",       50,  -5.0,  5.0, "#eta^{jet}",           "" );
    AddTH1D("JetPhi",        50, -200., 200., "#varphi_{jet} / deg",  "" );
    AddTH1D("JetTheta",      50,    0., 200., "#theta_{jet} / deg",   "" );
    AddTH1D("JetDeltaPhi",   50,  -0.1,  0.1, "#Delta#phi", "");
    AddTH1D("JetDeltaEta",   50,  -0.1,  0.1, "#Delta #eta", "" );
    AddTH1D("JetDeltaR",     50,  -0.1,  0.1, "#Delta R", "");
    AddTH2D("JetEtaPhi",    50,-3,3,50,-4,4,"#eta^{jet}","#varphi^{jet}");

    if (bIsMC) 
    {
        AddTH1D("JetPtGen",   50,  0.0, 100.0, "P_{T}^{jet}_{gen} / GeV", "" );
        AddTH1D("JetEtaGen",  50, -5.0,   5.0, "#eta^{jet}_{gen}"       , "" );
    }
}

void ZeeDHistManagerJet::FillJetHists(
    const TObjArray* jets, Double_t Weight)
{
    // Fill histograms for jets
    if (jets == NULL) {
        return;
    }

    // ----- Define pointers to histograms -------------
    /*
    */
    
    // Get event
    const ZeeDEvent* event = GetEvent();
    CHECK_NULL_PTR(event);

    // Check boson Y range if fYmin != fYmax
    if (fYmin != fYmax ) {
        const ZeeDJet* fJet= event->GetCurrentJet();
        Double_t yJet   = fJet->GetFourVector().Rapidity();

        if (yJet>= fYmax ) {
            return;
        }

        if (yJet<  fYmin ) {
            return;
        }
    }

    for ( Int_t i = 0; i < jets->GetEntriesFast(); ++i )
    {
        const ZeeDJet* jet = static_cast< ZeeDJet* > (jets->At(i));

        const TLorentzVector& fourVector = jet->GetFourVector();

        FillTH1(fourVector.Phi()  * ZeeDMisc::Rad2Deg, Weight, "JetPhi");
        FillTH1(fourVector.Theta()* ZeeDMisc::Rad2Deg, Weight, "JetTheta");
        FillTH1(fourVector.Pt(),            Weight, "JetPt");
        FillTH1(fourVector.Eta(),           Weight, "JetEta");
        FillTH2(fourVector.Eta(), fourVector.Phi(), Weight, "JetEtaPhi");

        // Fill MC jet information
        if ( bIsMC) 
        {

            //const TLorentzVector& mcfourVector      = jet->MCTruth().Get().fourVector;
            //
            //FillTH1(mcfourVector.Pt(),  Weight, "JetPtGen");
            //FillTH1(mcfourVector.Eta(), Weight, "JetEtaGen");
        }
    }
}
