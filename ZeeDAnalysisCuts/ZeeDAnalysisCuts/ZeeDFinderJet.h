#ifndef ZEEDFINDERJET
#define ZEEDFINGERJET

//////////////////////////////////////////
// Name: ZeeDFinderJet.h
/////////////////////////////////////////

#include "ZeeDAnalysisCuts/ZeeDFinder.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDJet.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisJetCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisZBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"

/** Jet Selector
 * @author Rohin Narayan
 */

class ZeeDFinderJet: public ZeeDFinder{
    public:
        ZeeDFinderJet(TString Name ="Jet Finder", std::string coll = "");
        virtual void BookCuts();
        virtual TObjArray SelectBosons(ZeeDEvent* /*event*/)
        {
            //nothing to select 
            TObjArray dummy;
            return dummy;
        }
       TObjArray SelectJets(ZeeDEvent* event)
        {
            const ZeeDJet* bestJet = ZeeDFinder::SelectBestJet(event,event->GetJets(fJetCollection));
            
            TObjArray jetArray; 
            jetArray.Add(const_cast<ZeeDJet*>(bestJet));
            return jetArray;
        }
    std::string fJetCollection;
            
};

#endif
