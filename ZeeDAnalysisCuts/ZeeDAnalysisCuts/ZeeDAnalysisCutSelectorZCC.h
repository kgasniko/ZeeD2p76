#pragma once

////////////////////////////////////////////////////////
// Name    : ZeeDAnalysisCutSelectorZCC.h
////////////////////////////////////////////////////////

#include "TString.h"

// ZeeD analysis includes
#include "ZeeDCuts/ZeeDCutSelector.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDCuts/IZeeDCalcWeight.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisZBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"


/** Combination of cuts used in Z analysis
  @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
  @date 2008/08/30
 */
class ZeeDAnalysisCutSelectorZCC : public ZeeDCutSelector {

  public:

    ZeeDAnalysisCutSelectorZCC(TString Name = "Z central central analysis cut selector") :
      ZeeDCutSelector(Name) {}

    void BookCuts();

};

