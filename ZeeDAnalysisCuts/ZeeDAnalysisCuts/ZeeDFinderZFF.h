#ifndef ZEEDFINDERZFF
#define ZEEDFINDERZFF

////////////////////////////////////////////////////////
// Name    : ZeeDFinderZFF.h
////////////////////////////////////////////////////////

#include "ZeeDAnalysisCuts/ZeeDFinder.h"
#include "TString.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisZBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"

/** Z Forward-Forward selection (?)
    Modified by SG 12 Feb 2012
*/
class ZeeDFinderZFF :  public ZeeDFinder {
public:
    ZeeDFinderZFF(TString Name = "ZFF Finder") : ZeeDFinder(Name) {}
    void BookCuts();

};

#endif
