#include "ZeeDAnalysisCuts/ZeeDAnalysisZBosonCuts.h"

// -----------------------------------------------------------------------------
//              Z boson analysis cuts
// -----------------------------------------------------------------------------

//------------------------------------------------------------------------------
Bool_t ZeeDCutPtMinCentralLepZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDLepton* e = event->GetCurrentBoson()->GetCentralLep();

    const TLorentzVector& elec = e->GetFourVector();

    const double pt = elec.Pt();

    return pt > m_cutval;
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutPtMinForwardLepZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDLepton* e = event->GetCurrentBoson()->GetForwardLep();

    const TLorentzVector& elec = e->GetFourVector();

    const double pt = elec.Pt();

    return pt > m_cutval;
}

Bool_t ZeeDCutEtaMaxBothLepZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }
 
    // Get the 2 electrons:
    const ZeeDLepton* e1 = event->GetCurrentBoson()->GetFirstLep();
    const ZeeDLepton* e2 = event->GetCurrentBoson()->GetSecondLep();
    if (e1->GetFourVector().Pt() == 0 || e2->GetFourVector().Pt() == 0)
        return kFALSE;
    const Double_t eta1 = TMath::Abs(e1->GetFourVector().Eta());
    const Double_t eta2 = TMath::Abs(e2->GetFourVector().Eta());

    const Double_t maxEta = (eta1) > (eta2) ? eta1 : eta2;

    return maxEta < m_cutval;

}


//------------------------------------------------------------------------------
Bool_t ZeeDCutEtaMaxBothElecZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }
 
    // Get the 2 electrons:
    const ZeeDElectron* e1 = (ZeeDElectron*)event->GetCurrentBoson()->GetFirstLep();
    const ZeeDElectron* e2 = (ZeeDElectron*)event->GetCurrentBoson()->GetSecondLep();
    // Return "fail" if no cluster:
    if ( e1->Cluster().IsSet() == kFALSE ||
            e2->Cluster().IsSet() == kFALSE ) {
        return kFALSE;
    }
    if (e1->Cluster().Get().fourVector.Pt()==0 ||
            e2->Cluster().Get().fourVector.Pt()==0) {
        return kFALSE;
    }

    const Double_t eta1 = TMath::Abs(e1->Cluster().Get().fourVector.Eta());
    const Double_t eta2 = TMath::Abs(e2->Cluster().Get().fourVector.Eta());

    const Double_t maxEta = (eta1) > (eta2) ? eta1 : eta2;

    return maxEta < m_cutval;

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutEtaCFBothElecZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }


    // Get the 2 electrons:
    const ZeeDElectron* e1 = (ZeeDElectron*)event->GetCurrentBoson()->GetFirstLep();
    const ZeeDElectron* e2 = (ZeeDElectron*)event->GetCurrentBoson()->GetSecondLep();

    // Return "fail" if no cluster:
    if ( e1->Cluster().IsSet() == kFALSE ||
            e2->Cluster().IsSet() == kFALSE ) {
        return kFALSE;
    }
    if (e1->Cluster().Get().fourVector.Pt()==0 ||
            e2->Cluster().Get().fourVector.Pt()==0) {
        return kFALSE;
    }

    const Double_t eta1 = TMath::Abs(e1->Cluster().Get().fourVector.Eta());
    const Double_t eta2 = TMath::Abs(e2->Cluster().Get().fourVector.Eta());

    if ( ((eta1 < m_cutvalmaxEMEC) && (eta2 > m_cutvalmin)  && (eta2 < m_cutvalmax)  ) ||
            ((eta2 < m_cutvalmaxEMEC) && (eta1 > m_cutvalmin)  && (eta1 < m_cutvalmax)  ) ) {
        return kTRUE;
    } else {
        return kFALSE;
    }
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutEtaFFBothElecZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }
 

    // Get the 2 electrons:
    const ZeeDElectron* e1 = (ZeeDElectron*)event->GetCurrentBoson()->GetFirstLep();
    const ZeeDElectron* e2 = (ZeeDElectron*)event->GetCurrentBoson()->GetSecondLep();

    // Return "fail" if no cluster:
    if ( e1->Cluster().IsSet() == kFALSE ||
            e2->Cluster().IsSet() == kFALSE ) {
        return kFALSE;
    }
    if (e1->Cluster().Get().fourVector.Pt()==0 ||
            e2->Cluster().Get().fourVector.Pt()==0) {
        return kFALSE;
    }

    const Double_t eta1 = TMath::Abs(e1->Cluster().Get().fourVector.Eta());
    const Double_t eta2 = TMath::Abs(e2->Cluster().Get().fourVector.Eta());

    if ( (eta1 > m_cutval) && (eta2 > m_cutval)   ) {
        return kTRUE;
    } else {
        return kFALSE;
    }
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutOQMaps::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }
 

    // Get the 2 electrons:
    const ZeeDElectron* e1 = (ZeeDElectron*)event->GetCurrentBoson()->GetFirstLep();
    const ZeeDElectron* e2 = (ZeeDElectron*)event->GetCurrentBoson()->GetSecondLep();


    return (e1->IsGoodOQAOD().IsSet() &&
            e2->IsGoodOQAOD().IsSet() &&
            e1->IsGoodOQAOD().Get() == true &&
            e2->IsGoodOQAOD().Get() == true )
           ? true : kFALSE;

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutTrackBothLepZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }
 

    // Get the 2 electrons:
    const ZeeDElectron* e1 = (ZeeDElectron*)event->GetCurrentBoson()->GetFirstLep();
    const ZeeDElectron* e2 = (ZeeDElectron*)event->GetCurrentBoson()->GetSecondLep();

    if ( (e1->TrackParticle().IsSet() ) && (e2->TrackParticle().IsSet()) ) {
        return kTRUE;
    } else {
        return kFALSE;
    }
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutCentralElecCaloIso::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }
 

    const ZeeDElectron* e = (ZeeDElectron*)event->GetCurrentBoson()->GetCentralLep();

    if ( e->CaloIso().IsSet() && e->CaloIso().Get().*pegammaIsolationType == kTRUE ) {
	return kTRUE;
    } else {
	return kFALSE;
    }

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutCentralElecIso::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }
 

    const ZeeDElectron* e = (ZeeDElectron*)event->GetCurrentBoson()->GetCentralLep();

    if ( e->EIso().IsSet() && e->EIso().Get().*pegammaIsolationType == kTRUE ) {
	return kTRUE;
    } else {
	return kFALSE;
    }

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutBothCentralElecCaloIso::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }
 

    // Get the 2 electrons:
    const ZeeDElectron* e1 = (ZeeDElectron*)event->GetCurrentBoson()->GetFirstLep();
    const ZeeDElectron* e2 = (ZeeDElectron*)event->GetCurrentBoson()->GetSecondLep();

    if ( e1->CaloIso().IsSet() && e1->CaloIso().Get().*pegammaIsolationType == kTRUE && e2->CaloIso().IsSet() && e2->CaloIso().Get().*pegammaIsolationType == kTRUE ) {
        return kTRUE;
    } else {
        return kFALSE;
    }

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutBothElecIso::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDElectron* e1 = (ZeeDElectron*)event->GetCurrentBoson()->GetFirstLep();
    const ZeeDElectron* e2 = (ZeeDElectron*)event->GetCurrentBoson()->GetSecondLep();

    if ( e1->EIso().IsSet() && e1->EIso().Get().*pegammaIsolationType == kTRUE && e2->EIso().IsSet() && e2->EIso().Get().*pegammaIsolationType == kTRUE ) {
        return kTRUE;
    } else {
        return kFALSE;
    }

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutTrackCentralLepZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDLepton* e1 = event->GetCurrentBoson()->GetFirstLep();
    const ZeeDLepton* e2 = event->GetCurrentBoson()->GetSecondLep();

    Bool_t HasTrack;

    if (  TMath::Abs(e1->GetFourVector().Eta()) > TMath::Abs(e2->GetFourVector().Eta()) ) {
        HasTrack = e2->TrackParticle().IsSet();
    } else {
        HasTrack = e1->TrackParticle().IsSet();
    }

    if ( HasTrack ) {
        return kTRUE;
    } else {
        return kFALSE;
    }
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutVertexPresentBothLepZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDLepton* e1 = event->GetCurrentBoson()->GetFirstLep();
    const ZeeDLepton* e2 = event->GetCurrentBoson()->GetSecondLep();

    if ( e1->Vertex().IsSet() && e2->Vertex().IsSet() ) {
        // Both have a vertex:
        if ((TMath::Abs(e1->Vertex().Get().position.Z() - e2->Vertex().Get().position.Z()) < m_cutval)) {
            return kTRUE;
        } else {
            return kFALSE;
        }
    } else {
        // At least one don't have a vertex:
        if ( (e1->Vertex().IsSet() ) || (e2->Vertex().IsSet())) {
            // one has a vertex:
            return kTRUE;
        } else {
            // none has a vertex:
            return kFALSE;
        }
    }
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutChargeBothLepZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDLepton* e1 = event->GetCurrentBoson()->GetFirstLep();
    const ZeeDLepton* e2 = event->GetCurrentBoson()->GetSecondLep();

    // if not both tracks, return fail:
    if ( ! ( (e1->TrackParticle().IsSet() ) && ( e2->TrackParticle().IsSet() ) ) ) {
        return kFALSE;
    }

    const Double_t charge_e1 = e1->getCharge();
    const Double_t charge_e2 = e2->getCharge();

    // mode: both positive (true and true)
    // requirement: both charges > 0
    if( (bcharge_e1 && bcharge_e2) &&
            (charge_e1 > 0 && charge_e2 > 0)
      ) {

        return kTRUE;

    }
    // mode: both negative (false and false)
    // requirement: both charges < 0
    else if ( (!bcharge_e1 && !bcharge_e2) &&
              (charge_e1 < 0 && charge_e2 < 0)
            ) {

        return kTRUE;

    }
    // mode: one positive, one negative (false and true)
    // requirement: charges not equal
    else if ( (bcharge_e1 != bcharge_e2) &&
              (charge_e1  != charge_e2)
            ) {

        return kTRUE;

    } else {

        return kFALSE;

    }

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutSameChargeBothLepZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDLepton* e1 = event->GetCurrentBoson()->GetFirstLep();
    const ZeeDLepton* e2 = event->GetCurrentBoson()->GetSecondLep();

    // if not both electrons have a track associated return fail:
    if ( e1->TrackParticle().IsSet() == kFALSE ||
            e2->TrackParticle().IsSet() == kFALSE ) {
        return kFALSE;
    }

    return e1->getCharge() == e2->getCharge();

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutZMassZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }

    const ZeeDBosonZ* Z = event -> GetCurrentBoson();
    const Double_t mass = Z->GetFourVector().M();

    if ( (mass >= m_mwlow) && (mass <= m_mwhigh) ) {
        return kTRUE;
    } else {
        return kFALSE;
    }
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutTriggerWithMatching2012CC::evaluate(const ZeeDEvent* event)
{
  // The function checks a trigger decision and the matching
  // of the electron(s) for the 2012 CC trigger:
  // (2e12Tvh_loose1 OR 2e12Tvh_loose1_L2StarB) for Data
  // 2e12Tvh_loose1 for MC

  // Return "fail" if no Z:
  if ( event->GetCurrentBoson() == NULL ) {
    return kFALSE;
  }

  const bool pass_trig_2e12      = event->HasPassedTriggerChain(ZeeDEnum::Trigger::EF_2e12Tvh_loose1);
  if (pass_trig_2e12) {
    const ZeeDLepton* e1 = event->GetCurrentBoson()->GetFirstLep();
    const ZeeDLepton* e2 = event->GetCurrentBoson()->GetSecondLep();
    const bool MatchElectron1_2e12 = e1->IsMatchedToTriggerChain(ZeeDEnum::Trigger::EF_2e12Tvh_loose1);
    const bool MatchElectron2_2e12 = e2->IsMatchedToTriggerChain(ZeeDEnum::Trigger::EF_2e12Tvh_loose1);
    if (MatchElectron1_2e12 && MatchElectron2_2e12) return kTRUE;
  }

  if (isMC == kFALSE) {
    const bool pass_trig_L2StarB = event->HasPassedTriggerChain(ZeeDEnum::Trigger::EF_2e12Tvh_loose1_L2StarB);
    if (pass_trig_L2StarB) {
      const ZeeDLepton* e1 = event->GetCurrentBoson()->GetFirstLep();
      const ZeeDLepton* e2 = event->GetCurrentBoson()->GetSecondLep();
      const bool MatchElectron1_L2StarB = e1->IsMatchedToTriggerChain(ZeeDEnum::Trigger::EF_2e12Tvh_loose1_L2StarB);
      const bool MatchElectron2_L2StarB = e2->IsMatchedToTriggerChain(ZeeDEnum::Trigger::EF_2e12Tvh_loose1_L2StarB);
      if (MatchElectron1_L2StarB && MatchElectron2_L2StarB) return kTRUE;
    }
  }

  return kFALSE;
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutTriggerWithMatching2012CF::evaluate(const ZeeDEvent* event)
{
  //CF 2012 Analysis. 
  //Checks if event was triggered by 
  //EF_e24vhi_medium1 OR EF_e60_medium1 trigger
  //and one of electrons match one of these triggers
   
  if ( event->GetCurrentBoson() == NULL ) {
    return kFALSE;
  }  
  // Get the 2 electrons:
  const ZeeDLepton* e1 = event->GetCurrentBoson()->GetFirstLep();
  const ZeeDLepton* e2 = event->GetCurrentBoson()->GetSecondLep();
  
  const bool Pass_OneOfTwoTriggers     = event->HasPassedTriggerChain(ZeeDEnum::Trigger::EF_e24vhi_medium1) ||  event->HasPassedTriggerChain(ZeeDEnum::Trigger::EF_e60_medium1);
  const bool e1_Match_OneOfTwoTriggers = e1->IsMatchedToTriggerChain(ZeeDEnum::Trigger::EF_e24vhi_medium1)  ||  e1->IsMatchedToTriggerChain(ZeeDEnum::Trigger::EF_e60_medium1);
  const bool e2_Match_OneOfTwoTriggers = e2->IsMatchedToTriggerChain(ZeeDEnum::Trigger::EF_e24vhi_medium1)  ||  e1->IsMatchedToTriggerChain(ZeeDEnum::Trigger::EF_e60_medium1);
  
  if (Pass_OneOfTwoTriggers && (e1_Match_OneOfTwoTriggers || e2_Match_OneOfTwoTriggers)) return kTRUE;
  
  return kFALSE;  
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutTriggerWithMatching::evaluate(const ZeeDEvent* event)
{
  // The function checks a trigger decision and the matching
  // of the electron(s) for the trigger specified

  // Keep eye on the type of the Default trigger:
  // DefaultSingleElec is checked as a single electron trigger
  // DefaultDiElec is checked as a di-electron trigger


  // Return "fail" if no Z:
  if ( event->GetCurrentBoson() == NULL ) {
    return kFALSE;
  }

  // Get the 2 electrons:
  const ZeeDLepton* e1 = event->GetCurrentBoson()->GetFirstLep();
  const ZeeDLepton* e2 = event->GetCurrentBoson()->GetSecondLep();

  if (fTriggerChain == ZeeDEnum::Trigger::DefaultSingleElec) {

    ZeeDEnum::Trigger::Value trigger = event->GetDefaultSingleElectronTrigger();
    Bool_t pass_trig = event->HasPassedTriggerChain(trigger);
    const Bool_t MatchElectron1 = e1->IsMatchedToTriggerChain(trigger);
    const Bool_t MatchElectron2 = e2->IsMatchedToTriggerChain(trigger);

    return (pass_trig && (MatchElectron1 || MatchElectron2));

  } else if (fTriggerChain == ZeeDEnum::Trigger::DefaultDiElec) {

    ZeeDEnum::Trigger::Value trigger = event->GetDefaultDiElectronTrigger();
    Bool_t pass_trig = event->HasPassedTriggerChain(trigger);
    const Bool_t MatchElectron1 = e1->IsMatchedToTriggerChain(trigger);
    const Bool_t MatchElectron2 = e2->IsMatchedToTriggerChain(trigger);

    return (pass_trig && (MatchElectron1 && MatchElectron2)); 

  } else if (fTriggerChain == ZeeDEnum::Trigger::UNKNOWN) {

    Error("ZeeDCutTrigger", "ZeeDCutTrigger is unable to cut on UNKNOWN trigger");
    return kTRUE;

  } else {

    Bool_t pass_trig = event->HasPassedTriggerChain(fTriggerChain);
    const Bool_t MatchElectron1 = e1->IsMatchedToTriggerChain(fTriggerChain);
    const Bool_t MatchElectron2 = e2->IsMatchedToTriggerChain(fTriggerChain);

    if (ZeeDEnum::Trigger::GetType(fTriggerChain) == ZeeDEnum::Trigger::DiElec) {
      return (pass_trig && (MatchElectron1 && MatchElectron2)); 
    } else {
      return (pass_trig && (MatchElectron1 || MatchElectron2));
    }

  }

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutEtaCrackBothElecZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }
 

    // Get the 2 electrons:
    const ZeeDElectron* e1 = (ZeeDElectron*)event->GetCurrentBoson()->GetFirstLep();
    const ZeeDElectron* e2 = (ZeeDElectron*)event->GetCurrentBoson()->GetSecondLep();

    // Return "fail" if no cluster:
    if ( e1->Cluster().IsSet() == kFALSE ||
            e2->Cluster().IsSet() == kFALSE ) {
        return kFALSE;
    }
    if (e1->Cluster().Get().fourVector.Pt()==0 ||
            e2->Cluster().Get().fourVector.Pt()==0) {
        return kFALSE;
    }

    const Double_t eta1 = TMath::Abs(e1->Cluster().Get().fourVector.Eta());
    const Double_t eta2 = TMath::Abs(e2->Cluster().Get().fourVector.Eta());

    if ( ((eta1 > m_cutmax) || (eta1 < m_cutmin)) && ((eta2 > m_cutmax) || (eta2 < m_cutmin))) {
        return kTRUE;
    } else {
        return kFALSE;
    }
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutIsEMOneLepZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }
 

    // Get the 2 electrons:
    const ZeeDLepton* e1 =event->GetCurrentBoson()->GetFirstLep();
  const ZeeDLepton* e2 =event->GetCurrentBoson()->GetSecondLep();
   const Bool_t pass =
        (e1->IsEM().IsSet() && e1->IsEM().Get().*pIsEMType == bDecision_IsEMType) || 
        (e2->IsEM().IsSet() && e2->IsEM().Get().*pIsEMType == bDecision_IsEMType);

    return pass;
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutIsEMOnlyOneLepZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDLepton* e1 = event->GetCurrentBoson()->GetFirstLep();
    const ZeeDLepton* e2 = event->GetCurrentBoson()->GetSecondLep();

    const Bool_t pass =
        (e1->IsEM().IsSet() && e1->IsEM().Get().*pIsEMType == kTRUE) ^
        (e2->IsEM().IsSet() && e2->IsEM().Get().*pIsEMType == kTRUE);

    return pass;
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutCheckIDBitsBothElec::evaluate(const ZeeDEvent* event)
{
  // Return "fail" if no Z:
  if ( event->GetCurrentBoson() == NULL ) {
    return kFALSE;
  }

  // Get the 2 electrons:
  const ZeeDElectron* elec1 = (ZeeDElectron*)event->GetCurrentBoson()->GetFirstLep();
  const ZeeDElectron* elec2 = (ZeeDElectron*)event->GetCurrentBoson()->GetSecondLep();

  const Bool_t pass_Conversion = 
    elec1->IDCut().IsSet() && elec1->IDCut().Get().ConversionMatch &&
    elec2->IDCut().IsSet() && elec2->IDCut().Get().ConversionMatch;

  const Bool_t pass_StripsWtot = 
    elec1->IDCut().IsSet() && elec1->IDCut().Get().ClusterStripsWtot &&
    elec2->IDCut().IsSet() && elec2->IDCut().Get().ClusterStripsWtot;
/*
   // this also could be interesting
  const Bool_t pass_TrackTRTratio = 
    elec1->IDCut().IsSet() && elec1->IDCut().Get().TrackTRTratio &&
    elec2->IDCut().IsSet() && elec1->IDCut().Get().TrackTRTratio;
*/
  return (pass_Conversion && pass_StripsWtot /*&& pass_TrackTRTratio*/);
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutIsEMBothLepZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDLepton* e1 = event->GetCurrentBoson()->GetFirstLep();
    const ZeeDLepton* e2 = event->GetCurrentBoson()->GetSecondLep();

    const Bool_t pass =
        (e1->IsEM().IsSet() && e1->IsEM().Get().*pIsEMType == kTRUE) &&
        (e2->IsEM().IsSet() && e2->IsEM().Get().*pIsEMType == kTRUE);

    return pass;

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutIDCutOneElecZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }
 

    // Get the 2 electrons:
    const ZeeDElectron* e1 = (ZeeDElectron*)event->GetCurrentBoson()->GetFirstLep();
    const ZeeDElectron* e2 = (ZeeDElectron*)event->GetCurrentBoson()->GetSecondLep();

    const Bool_t pass =
        (e1->IDCut().IsSet() && e1->IDCut().Get().*pIDCutType == bDecision_IDCutType) ||
        (e2->IDCut().IsSet() && e2->IDCut().Get().*pIDCutType == bDecision_IDCutType);

    return pass;
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutIDCutBothElecZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }
 

    // Get the 2 electrons:
    const ZeeDElectron* e1 = (ZeeDElectron*)event->GetCurrentBoson()->GetFirstLep();
    const ZeeDElectron* e2 = (ZeeDElectron*)event->GetCurrentBoson()->GetSecondLep();

    const Bool_t pass =
        (e1->IDCut().IsSet() && e1->IDCut().Get().*pIDCutType == bDecision_IDCutType) &&
        (e2->IDCut().IsSet() && e2->IDCut().Get().*pIDCutType == bDecision_IDCutType);

    return pass;

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutIsEMCentralForwardLepZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }
 

    // Get the 2 electrons:
    const ZeeDElectron* e1 = (ZeeDElectron*)event->GetCurrentBoson()->GetFirstLep();
    const ZeeDElectron* e2 = (ZeeDElectron*)event->GetCurrentBoson()->GetSecondLep();

    // Select minimum electrons's eta
    if ( e1->Cluster().IsSet() == kFALSE ||
            e2->Cluster().IsSet() == kFALSE ) {
        return kFALSE;
    }

    if (e1->Cluster().Get().fourVector.Pt()==0 ||
            e2->Cluster().Get().fourVector.Pt()==0) {
        return kFALSE;
    }

    const Double_t eta1 = TMath::Abs(e1->Cluster().Get().fourVector.Eta());
    const Double_t eta2 = TMath::Abs(e2->Cluster().Get().fourVector.Eta());

    // Central electron isem selection
    if (m_type == ZeeDEnum::Direction::Central) {
        if ( (eta1 < eta2) &&  e1->IsEM().IsSet() && e1->IsEM().Get().*pIsEMType == true ) {
            return kTRUE;
        }

        if ( (eta2 < eta1) &&  e2->IsEM().IsSet() && e2->IsEM().Get().*pIsEMType == true ) {
            return kTRUE;
        }
    }

    // Forward electron isem selection
    if (m_type == ZeeDEnum::Direction::Forward) {
        if ( (eta1 > eta2) &&  e1->IsEM().IsSet() && e1->IsEM().Get().*pIsEMType == true ) {
            return kTRUE;
        }

        if ( (eta2 > eta1) &&  e2->IsEM().IsSet() && e2->IsEM().Get().*pIsEMType == true ) {
            return kTRUE;
        }
    }

    return kFALSE;

}
/*

//------------------------------------------------------------------------------
Bool_t ZeeDCutIsEMIndexElectonZ::evaluate(const ZeeDEvent* event)
{

    // Get current Z:
    const ZeeDBosonZ* Z = event->GetCurrentBoson();

    // Return "fail" if no Z:
    if (Z == NULL ) {
        return kFALSE;
    }

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }
 

    // Get the  electron:
    const ZeeDElectron* e = (ZeeDElectron*)(m_ElectronIdx == 0) ?
                            event->GetCurrentBoson()->GetFirstLep() :
                            event->GetCurrentBoson()->GetSecondLep();

    return e->IsEM().Get().*pIsEMType == kTRUE;

}
*/
//------------------------------------------------------------------------------
Bool_t ZeeDCutAuthorBothElecZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }

 

    // Get the 2 electrons:
    const ZeeDElectron* e1 = (ZeeDElectron*)event->GetCurrentBoson()->GetFirstLep();
    const ZeeDElectron* e2 = (ZeeDElectron*)event->GetCurrentBoson()->GetSecondLep();

    if(e1->Author().IsSet() == kFALSE ||
            e2->Author().IsSet() == kFALSE) {

        return kFALSE;

    }

    if(e1->Author().Get().isElectron == kTRUE &&
            e2->Author().Get().isElectron == kTRUE) {

        return kTRUE;

    } else {

        return kFALSE;

    }

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutAuthorCFElecZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }
 

    // Get the 2 electrons:
    const ZeeDElectron* e1 = (ZeeDElectron*)event->GetCurrentBoson()->GetFirstLep();
    const ZeeDElectron* e2 = (ZeeDElectron*)event->GetCurrentBoson()->GetSecondLep();


    if(e1->Author().IsSet() == kFALSE ||
            e2->Author().IsSet() == kFALSE) {
        return kFALSE;
    }

    if ( e1->Cluster().IsSet() == kFALSE ||
            e2->Cluster().IsSet() == kFALSE ) {
        return kFALSE;
    }
    if (e1->Cluster().Get().fourVector.Pt()==0 ||
            e2->Cluster().Get().fourVector.Pt()==0) {
        return kFALSE;
    }

    const Double_t eta1 = TMath::Abs(e1->Cluster().Get().fourVector.Eta());
    const Double_t eta2 = TMath::Abs(e2->Cluster().Get().fourVector.Eta());

    if ( (eta1 < eta2) && e1->Author().Get().isElectron  &&  e2->Author().Get().isFrwd) {
        return kTRUE;
    }

    if ( (eta2 < eta1) && e2->Author().Get().isElectron  &&  e1->Author().Get().isFrwd) {
        return kTRUE;
    }

    return kFALSE;

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutTruthMatchBothElecZ::evaluate(const ZeeDEvent* event)
{

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }

    // Get the 2 electrons:
    const ZeeDLepton* e1 = event->GetCurrentBoson()->GetFirstLep();
    const ZeeDLepton* e2 = event->GetCurrentBoson()->GetSecondLep();

    // both electrons have to match truth (deltaR<0.2)
    // @TODO: is IsSet enough?
    if ( (e1->MCTruth().IsSet()) && (e2->MCTruth().IsSet())  ) {
        return kTRUE;
    } else {
        return kFALSE;
    }

}

//------------------------------------------------------------------------------
Bool_t ZeeDCutIsEMIsEMCaloIsoOneElecZ::evaluate(const ZeeDEvent* event) {

    // Return "fail" if no Z:
    if ( event->GetCurrentBoson() == NULL ) {
        return kFALSE;
    }
 

    // Get the 2 electrons:
    const ZeeDElectron* e1 = (ZeeDElectron*)event->GetCurrentBoson()->GetFirstLep();
    const ZeeDElectron* e2 = (ZeeDElectron*)event->GetCurrentBoson()->GetSecondLep();

    if( !e1 ||
        !e2 ||
        !e1->IsEM().IsSet() ||
        !e2->IsEM().IsSet() ||
        !e1->CaloIso().IsSet() ||
        !e2->CaloIso().IsSet()
        ) {
        return kFALSE;
    }

    const Bool_t e1_IsEMType1 = e1->IsEM().Get().*fIsEMType1;
    const Bool_t e1_IsEMType2 = e1->IsEM().Get().*fIsEMType2;
    const Bool_t e1_CaloIso   = e1->CaloIso().Get().*fEgammaIsolationType;

    const Bool_t e2_IsEMType1 = e2->IsEM().Get().*fIsEMType1;
    const Bool_t e2_IsEMType2 = e2->IsEM().Get().*fIsEMType2;
    const Bool_t e2_CaloIso   = e2->CaloIso().Get().*fEgammaIsolationType;

    DEBUG_VAR(e1_IsEMType1);
    DEBUG_VAR(e1_IsEMType2);
    DEBUG_VAR(e1_CaloIso);

    DEBUG_VAR(e2_IsEMType1);
    DEBUG_VAR(e2_IsEMType2);
    DEBUG_VAR(e2_CaloIso);

    const Bool_t comb_decision_1 =
        (e1_IsEMType1 == bDecision_IsEMType1) &&
        (e1_IsEMType2 == bDecision_IsEMType2) &&
        (e1_CaloIso   == bDecision_CaloIso);

    const Bool_t comb_decision_2 =
        (e2_IsEMType1 == bDecision_IsEMType1) &&
        (e2_IsEMType2 == bDecision_IsEMType2) &&
        (e2_CaloIso   == bDecision_CaloIso);

    DEBUG_VAR(comb_decision_1);
    DEBUG_VAR(comb_decision_2);

    if( comb_decision_1 || comb_decision_2 ) {
        return kTRUE;
    } else {
        return kFALSE;
    }

}


