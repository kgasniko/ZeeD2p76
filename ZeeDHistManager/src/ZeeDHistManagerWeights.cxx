#include "ZeeDHistManager/ZeeDHistManagerWeights.h"

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"

//#define USE_DEBUG_MESSAGES
#include "ZeeDMisc/ZeeDMisc.h"


//-----------------------------------------------------
ZeeDHistManagerWeights::ZeeDHistManagerWeights(TString name)  : ZeeDHistManager(name),
                                                                fSvcHelper(name),
                                                                fAnaOptions(fSvcHelper.GetAnaSvc())
{}

//-----------------------------------------------------
void ZeeDHistManagerWeights::BookHistos()
{
    // Books histograms

    AddTH1D("RecEventWeight",  100, -10,   10,  "Rec Event Weight", "");

    // sum of rec event weights accumulated for each fill
    // implemented as histogram with only one bin
    // in analysis use Integral() to retrieve the sum of weights
    AddTH1D("SumOfRecEventWeights",  1, 0, 1,  "Sum Of Rec Event Weights", "");

    if( (*fAnaOptions)->IsMC() ) {
        AddTH1D("GenEventWeight",  100, -10,   10,  "Gen Event Weight", "");

        // see sum of rec events
        AddTH1D("SumOfGenEventWeights",  1, 0, 1,  "Sum Of Gen Event Weights", "");
    }

}

//-----------------------------------------------------
void ZeeDHistManagerWeights::Fill()
{
    DEBUG_MES_START;
    DEBUG_MES(this->GetHMName());


    // Get event
    const ZeeDEvent* event = GetEvent();
    CHECK_NULL_PTR(event);

    // Rec Event weight
    const Double_t recWeight = event->GetWeight();
    FillTH1(recWeight, 1., "RecEventWeight");

    TH1* sumOfRecWeightsHist = GetTH1("SumOfRecEventWeights");
    if ( sumOfRecWeightsHist != NULL ) {
        Double_t sumOfRecWeights = sumOfRecWeightsHist->GetBinContent(1);
        sumOfRecWeights += recWeight;
        sumOfRecWeightsHist->SetBinContent(1, sumOfRecWeights);
    }

    if( (*fAnaOptions)->IsMC() ) {


        // Gen Event weight
        const Double_t genWeight = event->GetGenWeight();
        DEBUG_VAR(genWeight);
        FillTH1(genWeight, 1., "GenEventWeight");

        TH1* sumOfGenWeightsHist = GetTH1("SumOfGenEventWeights");
        if ( sumOfGenWeightsHist != NULL ) {
            Double_t sumOfGenWeights = sumOfGenWeightsHist->GetBinContent(1);
            sumOfGenWeights += genWeight;
            sumOfGenWeightsHist->SetBinContent(1, sumOfGenWeights);
        }

    }

    DEBUG_MES_END;
}



