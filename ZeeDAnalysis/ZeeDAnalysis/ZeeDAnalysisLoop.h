#ifndef __ZEEDANALYSISLOOP_H
#define __ZEEDANALYSISLOOP_H

////////////////////////////////////////////////////////
// Name    : ZeeDAnalysisLoop.h
////////////////////////////////////////////////////////

// Athena includes
#include "AthenaBaseComps/AthAlgorithm.h"

// ZeeD analysisLoop includes
#include "ZeeDTools/ZeeDSvcHelper.h"

#include <TObjArray.h>
#include <TNamed.h>

#include "ZeeDCuts/ZeeDCutSelector.h"
#include "ZeeDAnalysisCuts/ZeeDIdentifier.h"
#include "ZeeDAnalysisCuts/ZeeDFinderW.h"
#include "ZeeDAnalysisCuts/ZeeDFinderWmu.h"
#include "ZeeDAnalysisCuts/ZeeDIdentifierW.h"
#include "ZeeDAnalysisCuts/ZeeDIdentifierWmu.h"
#include "ZeeDAnalysisCuts/ZeeDFinderZmumu.h"
#include "ZeeDAnalysisCuts/ZeeDIdentifierZmumu.h"

class ZeeDHistManager;
class ZeeDCutHistManager;
class ZeeDEvent;
typedef ZeeDEvent RawEvent;
class ZeeDFinder;
class ZeeDIdentifier;



/** Z->ee analysisLoop
    @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
    @date 2008/08/01
*/


/** Pair of cut hist manager and identifier */
struct ZeeDCutCache : public TNamed {

    ZeeDCutCache(const char* name,
                 ZeeDIdentifier* identifier,
                 ZeeDCutSelector* cutSelector) : TNamed(name,name),
                                                 fIdentifier(identifier),
                                                 fCutSelector(cutSelector)
    {}

    void evaluate(ZeeDEvent* event) {
    //    std::cout << "Evaluating cut cache " << std::endl;
        fCutMasks.clear();

        fIdentifier->IdentifyRecBosons(event);

        // Number of reconstructed and selected Z boson candidates 
        fnIdentifications = fIdentifier->GetNumberOfIdentifications();

        // Loop over all reconstructed and selected Z boson candidates
        for(UInt_t idnumber = 0; idnumber != fnIdentifications; ++idnumber) {

            fIdentifier->SetEventIdentification(event, idnumber);

            fCutSelector->evaluate(event);
            fCutMasks.push_back(fCutSelector->getCutMask());

        }
     //   std::cout << fnIdentifications << std::endl;
    }

    ZeeDIdentifier* fIdentifier;
    UInt_t fnIdentifications;
    ZeeDCutSelector* fCutSelector;

    std::vector<ZeeDCutBit> fCutMasks;

};

struct ZeeDAnalysisChain {

    ZeeDAnalysisChain(ZeeDCutCache* cutCache,
                      ZeeDCutHistManager* cutHM) : fCutCache(cutCache),
                                                   fCutHistManager(cutHM)
    {}

    ZeeDCutCache* fCutCache;
    ZeeDCutHistManager* fCutHistManager;

};

typedef std::vector<ZeeDAnalysisChain*> ZeeDAnalysisChainBundle;

typedef std::vector<ZeeDHistManager*> ZeeDNoCutsBundle;

class ZeeDAnalysisLoop : public AthAlgorithm, public TObject {


public:
    ZeeDAnalysisLoop(const std::string& name, ISvcLocator* pSvcLocator);
    ~ZeeDAnalysisLoop();

    virtual StatusCode initialize() = 0;

    /** called by the event loop on event by event. Reads events in */
    StatusCode execute();

    /** delete any memory allocation from the heap */
    virtual StatusCode finalize() = 0;

    void finalizeReport();

    /** Initialize identifiers */
    virtual void initializeIdentifiers() {};

    /** Initialize no-cut histmanagers */
    virtual void initializeNoCutHistManagers() {};

    /** Arrange loop within event. Allows to arrange loop over systematics */
    virtual void analyseEvent(const RawEvent* rawEvent) = 0;

    /** Adds histmanager to the analysis */
    void AddHistManager(ZeeDHistManager* histManager);
    void AddHistManager(ZeeDHistManager* histManager, ZeeDNoCutsBundle* bundle);

    /** Add identifier */
    ZeeDIdentifier* AddIdentifier(ZeeDFinder* finder, TString name="");
    ZeeDIdentifier* AddIdentifier(ZeeDFinderW* finder, TString name="");
    ZeeDIdentifier* AddIdentifier(ZeeDFinderWmu* finder, TString name="");

    /** Add CutHistManager, coupled to identifier */
    void AddCutHistManager(ZeeDCutHistManager* cutHM,  ZeeDCutCache* cutCache, ZeeDAnalysisChainBundle* list = NULL);

    /** Check if event passed manual GRL decision - return true if no manual GRL decision is taken at all */
    bool CheckManualGRL(const ZeeDEvent* event);

protected:

    /** Array of histmanagers */
    std::vector<ZeeDHistManager*> fHistManagers;                 //!

    /** Event Identifiers */
    std::map<TString, ZeeDIdentifier*> fIdentifiers;

    /** Event loop counter */
    Int_t fEventCounter;                      //

    Int_t fEventInGRLCounter;                 //

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;

};

#endif // ZeeDAnalysis_H
