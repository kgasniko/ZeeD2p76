#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"

// -----------------------------------------------------------------------------
//              General boson specific analysis cuts
// -----------------------------------------------------------------------------

//------------------------------------------------------------------------------
Bool_t ZeeDCutPtZMaxZ::evaluate(const ZeeDEvent* event)
{
    // Get current boson
    ZeeDBoson* boson = (ZeeDBoson*)event->GetCurrentBoson();

    // Return "fail" if no Z:
    if (boson == NULL) return kFALSE;

    // Get four vector of the boson and calculate the Pt_boson 
    const Double_t Pt = boson->GetFourVector().Pt();

    return Pt < m_cutval;

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutPtMinBothLepZ::evaluate(const ZeeDEvent* event)
{

    const ZeeDBosonZ* boson = event->GetCurrentBoson();

    // Return "fail" if no Z:
    if ( boson == NULL ) {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDLepton* e1 = boson->GetFirstLep();
    const ZeeDLepton* e2 = boson->GetSecondLep();

    const double pt1 = (e1->GetFourVector()).Pt();
    const double pt2 = (e2->GetFourVector()).Pt();

    const Double_t minPt = pt1 < pt2 ? pt1 : pt2;

    return minPt > m_cutval;

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutPtMinOneLepZ::evaluate(const ZeeDEvent* event)
{

    // Get list of electrons:
    const TObjArray* electrons = event->GetElectrons();

    Double_t pt = 0.0;
    Double_t ptmax = 0.0;

    // ----- Loop over electrons ------------------------
    for ( Int_t index = 0; index < electrons->GetEntriesFast(); ++index ) {

        const ZeeDElectron* electron =  static_cast< ZeeDElectron* > (electrons->At(index));
        pt = electron->GetFourVector().Pt();

        ptmax = pt > ptmax ? pt : ptmax;
    }

    return ptmax > m_cutval;

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutAsymmetricalLepPt::evaluate(const ZeeDEvent* event)
{

    const ZeeDBosonZ* boson = event->GetCurrentBoson();

    // Return "fail" if no Z:
    if ( boson == NULL ) {
        return kFALSE;
    }

    // Get the 2 electrons:
    // (pt ordering has been done during the boson construction)
    const ZeeDLepton* e1 = boson->GetFirstLep();
    const ZeeDLepton* e2 = boson->GetSecondLep();

    const double ptmin = (e1->GetFourVector()).Pt();
    const double ptmax = (e2->GetFourVector()).Pt();

    return (ptmin > m_min1 && ptmax > m_min2);

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutZYZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }

    const ZeeDBoson* Z = event -> GetCurrentBoson();
    const Double_t y = Z->GetFourVector().Rapidity();

    if ( (y >= m_ywlow) && (y <= m_ywhigh) ) {
        return kTRUE;
    } else {
        return kFALSE;
    }
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutZMassZCharge::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }

    const ZeeDBoson* Z = event -> GetCurrentBoson();
    const Double_t mass = Z->GetFourVector().M();

    //return mass > 40 && ! ( mass > 66 && mass < 116 ) ;
    return ! ( mass > 66 && mass < 116 ) ;

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutTriggerLArHoleBugBothElecZ::evaluate(const ZeeDEvent* event) {

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDElectron* e1 = (ZeeDElectron*)event->GetCurrentBoson()->GetFirstLep();
    const ZeeDElectron* e2 = (ZeeDElectron*)event->GetCurrentBoson()->GetSecondLep();

    const TLorentzVector& elec1 = e1->GetFourVector();
    const TLorentzVector& elec2 = e2->GetFourVector();

    const TString period = event->GetPeriod();

    const Double_t LArHolePhiMin = -0.95;//-0.888;
    const Double_t LArHolePhiMax = -0.48;//-0.492;

    const Double_t LArHoleEtaMin = -0.15;//-0.1;
    const Double_t LArHoleEtaMax = 1.6;//1.55;

    if ( period == "data11_7TeV_I" ||
         period == "data11_7TeV_J" ||
         period == "data11_7TeV_K" ) {

        const Double_t eta1 = elec1.Eta();
        const Double_t eta2 = elec2.Eta();

        const Double_t phi1 = elec1.Phi();
        const Double_t phi2 = elec2.Phi();

        const Bool_t elec1InHole =
            (LArHoleEtaMin < eta1 && eta1 < LArHoleEtaMax)  &&
            (LArHolePhiMin < phi1 && phi1 < LArHolePhiMax);

        const Bool_t elec2InHole =
            (LArHoleEtaMin < eta2 && eta2 < LArHoleEtaMax) &&
            (LArHolePhiMin < phi2 && phi2 < LArHolePhiMax);

        const Bool_t oneElecInHole = elec1InHole || elec2InHole;

        return !oneElecInHole;

    } else {

        return kTRUE;

    }

}


