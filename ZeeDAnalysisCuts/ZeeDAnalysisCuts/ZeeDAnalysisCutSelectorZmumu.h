#ifndef __ZEEDANALYSISCUTSELECTORZMUMU
#define __ZEEDANALYSISCUTSELECTORZMUMU


////////////////////////////////////////////////////////
// Name    : ZeeDAnalysisCutSelectorZmumu.h
////////////////////////////////////////////////////////

#include "TString.h"

// ZeeD analysis includes
#include "ZeeDCuts/ZeeDCutSelector.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDCuts/IZeeDCalcWeight.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisBosonCuts.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisEventCuts.h"


/** Combination of cuts used in Z analysis
  @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
  @date 2008/08/30
 */
class ZeeDAnalysisCutSelectorZmumu : public ZeeDCutSelector {

  public:

    ZeeDAnalysisCutSelectorZmumu(TString Name = "Z central central analysis cut selector") :
      ZeeDCutSelector(Name) { this->BookCuts();}

    void BookCuts();

};
#endif 
