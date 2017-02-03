#pragma once

////////////////////////////////////////////////////////
// Name    : ZeeDLauncher.h
////////////////////////////////////////////////////////

// STL includes
#include <iostream>

// ROOT includes
#include "TH1D.h"

// Analysis includes
#include "ZeeDAnalysis/ZeeDAnalysisLoop.h"
#include "ZeeDArrays/ZeeDArrayI.h"
#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDTools/ZeeDSystematics.h"
#include "ZeeDTools/ZeeDCutBit.h"
#include "ZeeDCalculator/ZeeDEventFactory.h"
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDEvent/ZeeDEvent.h"

class ZeeDCutCache;
class ZeeDCutSelector;
class ZeeDIdentifier;


class ZeeDLauncher : public ZeeDAnalysisLoop {

public:

    ZeeDLauncher(const std::string& name, ISvcLocator* pSvcLocator);
    ~ZeeDLauncher();

    StatusCode initialize();

    StatusCode finalize();

    /** Initialize identifiers */
    void initializeIdentifiers();

    /** Initialize CutSelectors */
    void initializeCutSelectors();

    void addCutSelector(ZeeDCutSelector* cutSelector);

    ZeeDCutSelector* getCutSelector(const TString& name);

    void addCutCache(ZeeDCutCache* cutCache);

    ZeeDCutCache* getCutCache(const TString& name);

    void initializeCutCaches();

    ZeeDIdentifier* getIdentifier(const TString& name);
    //ZeeDIdentifierW* getIdentifierW(const TString& name);

    /** Initialize analysis chains */
    ZeeDAnalysisChainBundle* initializeAnalysisChainsReplica(TString baseName, TString trackName);

    ZeeDNoCutsBundle* initializeNoCutsReplica(TString baseName, TString trackName);

    void addShelf(const TString& shelfName);

    /** Adds source to the list of systematics shifts */
    void AddSysSource(ZeeDSystematics::Systematics source);

private:

    ZeeDEventFactory* fEventFactory;

    /**  Array of systematics shifts */
    ZeeDArrayI        fSysArray;     //!
    vector<ZeeDSystematics::Systematics> fSysArrayN;     //!

    /**  Array of MC systematics shifts */
    ZeeDArrayI        fSysMCArray;   //!

    /**  Systematics manager */
    ZeeDSystematics* fSys;

    TObjArray fCutSelectors;
    TObjArray fCutCaches;

    std::map<ZeeDSystematics::ZeeDSingleSystematics*, ZeeDNoCutsBundle*> fNoCutsBundles;

    std::map<ZeeDSystematics::ZeeDSingleSystematics*, ZeeDAnalysisChainBundle*> fChainBundles;
    
    /** Read and set MetaData from TTree */
    void setMetaData();

    /** Apply systematic shifts, bulid calculated event*/
    void analyseEvent(const RawEvent* rawEvent);

    void DoAnalysis(ZeeDEvent* event, ZeeDSystematics::ZeeDSingleSystematics* sys);

    TH1D* h_lumi;

};
