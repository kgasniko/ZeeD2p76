#ifndef __ZEEDCALCEVENTWEIGHT_H
#define __ZEEDCALCEVENTWEIGHT_H

////////////////////////////////////////////////////////
// Name    : ZeeDCalcEventWeight.h
////////////////////////////////////////////////////////

// Root includes
#include <TObject.h>
#include <TH1D.h>
#include <TFile.h>
#include <TRandom.h>

// ZeeD analysis includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "ZeeDEvent/ZeeDElectron.h"

#include "ZeeDExternal/ZeeDPDF.h"
#include "PileupReweighting/TPileupReweighting.h"
#include "egammaAnalysisUtils/VertexPositionReweightingTool.h"
#include "egammaAnalysisUtils/BosonPtReweightingTool.h"

#include "ZeeDExternal/LineShapeTool.h"

class ZeeDSystematics;
class ZeeDEvent;
class ZeeDArrayD;
class ZeeDBinGrid;

/** brief Class for calculation of event weights
  @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte, Sebastian Schmitt
  @date   Thu May 19 08:44:41 CEST 2010
*/
class ZeeDCalcEventWeight : public TObject {

public:
    ZeeDCalcEventWeight();
    ~ZeeDCalcEventWeight();

    /** Calculate gen weights */
    void Calculate(ZeeDEvent* event);

private:

    /** ZeeD event */
    ZeeDEvent* fEvent;

    /** Event weight */
    Double_t fEventWeight;

    /** Gen weight */
    Double_t fGenWeight;

    /**  Pointer to systematics manager */
    ZeeDSystematics* fSys;

    Root::TPileupReweighting* fPileupTool;

    void MCAssignRandomRunNumber();
    void ApplyMCVtxWeight();
    void ApplyMCTrkVtxWeight();    
    void ApplyOfficialPtWeight();
    void ApplyPDFWeight();
    void ApplyPileupWeight();
    void ApplyLineShapeWeight();
    void ApplyUnofficialPtWeight();
    void GeneratePileupMCWeight();
    void CorrectSumet();
    /**  Zvtx weight for 2010 data */
    void ApplyZvWeight();

    void ApplyGenBosMassWeight();

    void ConfigurePileupToolForReweighting();
    void ConfigurePileupToolToGenerateMCFile();
    void ConfigurePileupToolToReweightToIndividualPeriods();

    /* Pt reweighing tool */
    void ConfigureBosonPtReweightingTool();
    void AddBosonPtReweightingTool(std::string targetName, bool defaultTool);
    BosonPtReweightingTool* rwTool_DefaultTarget;
    std::map<TString, BosonPtReweightingTool*> rwToolMap;
    TFile* fileSF;
    TObjArray* correctSumet, *toys;
    TH1D* NTrkVtx;

    ZeeDPDF::Flavour enumConverter(int id);

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;      //!

    TH1D* weighthist;
    TRandom* rndm; 

    VertexPositionReweightingTool* zvtxrwTool;

    LineShapeTool frwtz;

};

#endif // ZeeDCalcEventWeight
