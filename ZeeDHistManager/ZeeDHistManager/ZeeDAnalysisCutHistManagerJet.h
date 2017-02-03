#ifndef ZEEDANALYSISCUTHISTMANAGERJET
#define ZEEDANALYSISCUTHISTMANAGERJET

///////////////////////////////////////////////
// Name: ZeeDAnalysisCutHistManagerJet
// @author Rohin Narayan
// @date 29 November 2011
// ///////////////////////////////////////////

#include "ZeeDCuts/ZeeDCutHistManager.h"
#include "TString.h"

class ZeeDAnalysisCutHistManagerJet:public ZeeDCutHistManager
{
    public:
        explicit  ZeeDAnalysisCutHistManagerJet(TString Name ="Jet Selection");
        ~ZeeDAnalysisCutHistManagerJet();
        void BookCutHistManager();
};

#endif
