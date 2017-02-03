#include "ZeeDAnalysisCuts/ZeeDAnalysisJetCuts.h"


// -----------------------------------------------------------------------------
//              Jets specific analysis cuts
// -----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
Bool_t ZeeDCutPtMinJet::evaluate(const ZeeDEvent* event)
{
    const ZeeDJet* jet = event->GetCurrentJet();
    if(jet->GetFourVector().Perp() < fCutval){
        return kFALSE;
    }else {
        return kTRUE;
    }
}

//------------------------------------------------------------------------------
Bool_t ZeeDCutEtaMaxJet::evaluate(const ZeeDEvent* event)
{
    const ZeeDJet* jet = event->GetCurrentJet();
    if(jet->GetFourVector().Eta() <fCutval){
        return kTRUE;
    }
    else{
        return kFALSE;
    }
}
