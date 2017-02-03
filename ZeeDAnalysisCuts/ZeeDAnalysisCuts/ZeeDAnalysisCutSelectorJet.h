#ifndef ZEEDANALYSISCUTSELECTORZJET
#define ZEEDANALYSISCUTSELECTORZJET

////////////////////////////////////////////////////////
// Name    : ZeeDAnalysisCutSelectorJet.h
////////////////////////////////////////////////////////

#include "ZeeDCuts/ZeeDCutSelector.h"
#include "TString.h"

/** Cuts used in Jet Selection
  @author Rohin Narayan
  @date   Tue November 29 
*/
class ZeeDAnalysisCutSelectorJet : public ZeeDCutSelector {
public:
    explicit ZeeDAnalysisCutSelectorJet(TString name = "Jet Selection Cuts");
    ~ZeeDAnalysisCutSelectorJet();

private:
    void BookCuts();
};

#endif
