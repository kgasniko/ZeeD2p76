#include "ZeeDAnalysis/ZeeDLauncher.h"

// STL includes
#include <string>
#include <iostream>
#include <string>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"

#include "ZeeDHist/ZeeDHistManager.h"
#include "ZeeDCuts/ZeeDCutHistManager.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include "ZeeDCalculator/ZeeDCalcEvent.h"
#include "ZeeDTools/ZeeDCalibrator.h"

#include "ZeeDHistManager/ZeeDAnalysisCutHistManagerZCC.h"
#include "ZeeDHistManager/ZeeDAnalysisCutHistManagerZCF.h"
#include "ZeeDHistManager/ZeeDAnalysisCutHistManagerZBaseline.h"
#include "ZeeDHistManager/ZeeDAnalysisCutHistManagerWriter.h"
#include "ZeeDHistManager/ZeeDAnalysisCutHistManagerCutFlow.h"
#include "ZeeDHistManager/ZeeDAnalysisCutHistManagerW.h"
#include "ZeeDHistManager/ZeeDAnalysisCutHistManagerWmu.h"
#include "ZeeDHistManager/ZeeDAnalysisCutHistManagerZmumu.h"


#include "ZeeDHistManager/ZeeDHistManagerWeights.h"
#include "ZeeDHistManager/ZeeDHistManagerEvent.h"
#include "ZeeDHistManager/ZeeDHistManagerElectron.h"
#include "ZeeDHistManager/ZeeDHistManagerMuon.h"
#include "ZeeDHistManager/ZeeDHistManagerBoson.h"
#include "ZeeDHistManager/ZeeDHistManagerQCDBkg.h"
#include "ZeeDHistManager/ZeeDHistManagerZjet.h"
#include "ZeeDHistManager/ZeeDHistManagerZjetAna.h"
#include "ZeeDHistManager/ZeeDHistManagerZplusJet.h"
#include "ZeeDHistManager/ZeeDHistManagerJetCharge.h"
#include "ZeeDHistManager/ZeeDHistManagerGenInfo.h"

#include "ZeeDAnalysisCuts/ZeeDIdentifier.h"
#include "ZeeDAnalysisCuts/ZeeDFinder.h"
#include "ZeeDAnalysisCuts/ZeeDFinderZCC.h"
#include "ZeeDAnalysisCuts/ZeeDFinderZCF.h"
#include "ZeeDAnalysisCuts/ZeeDFinderW.h"
#include "ZeeDAnalysisCuts/ZeeDFinderWmu.h"
#include "ZeeDAnalysisCuts/ZeeDFinderZmumu.h"

#include "ZeeDAnalysisCuts/ZeeDFinderTrigger.h"

#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorZCC.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorZCF.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorPreSelection.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorZCCCutFlow.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorW.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorWmu.h"
#include "ZeeDAnalysisCuts/ZeeDAnalysisCutSelectorZmumu.h"
#include "ZeeDAnalysisCuts/ZeeDIdentifierW.h"
#include "ZeeDAnalysisCuts/ZeeDIdentifierWmu.h"
#include "ZeeDAnalysisCuts/ZeeDIdentifierZmumu.h"



#include "ZeeDCuts/ZeeDCutSelector.h"

#include "TSystem.h"

//------------------------------------------------------
ZeeDLauncher::ZeeDLauncher(const std::string& name,
                           ISvcLocator* pSvcLocator) :
    ZeeDAnalysisLoop(name, pSvcLocator)
{

}

//------------------------------------------------------
ZeeDLauncher::~ZeeDLauncher()
{

    fCutSelectors.Delete();

}

//------------------------------------------------------
StatusCode ZeeDLauncher::initialize()
{

    Info("ZeeDLauncher::initialize","initializing");

    // Create Event Factory
    if( (*fAnaOptions)->Source() == "TTree" ) this->setMetaData();

    // initialize Finders:
    // define cuts and use them to choose the best boson
    this->initializeIdentifiers();

    // initialize CutSelectors:
    // define cuts for the analysis
    this->initializeCutSelectors();

    // pair Finders and CutSelectors
    this->initializeCutCaches();

    // Access to the ZeeDSystematics object 
    fSys = ZeeDSystematics::getInstance();

    // Access to EventFactory
    fEventFactory = ZeeDEventFactory::getInstance();

    // Create shelves for the base analysis
    this->addShelf("All");

    return StatusCode::SUCCESS;
}

//------------------------------------------------------
void ZeeDLauncher::setMetaData()
{
  // read and set meta data in case of TTree source

  StoreGateSvc * m_MetaDataStore;

  if(service("StoreGateSvc/MetaDataStore", m_MetaDataStore).isSuccess()) {

    const std::map<std::string, int>* pTriggerMap = 0;
    if(m_MetaDataStore->retrieve(pTriggerMap, "MetaTree/triggerMap").isSuccess()) {

      Info("ZeeDLauncher::setMetaData", "Read trigger map index from metadata");

      (*fAnaOptions)->setTriggerMap(*pTriggerMap);


    } else {

      Warning("ZeeDLauncher::setMetaData", "no trigger map info");

    }

    //----------------------------------------------------------------------
    if ((*fAnaOptions)->MCDataAuto()) {

      const std::string* pInputType = 0;

      if(m_MetaDataStore->retrieve(pInputType, "MetaTree/inputType").isSuccess()) {

	Info("ZeeDLauncher::setMetaData", "Read inputType from metadata: %s",
	    (*pInputType).c_str());

	(*fAnaOptions)->SetInputType(ZeeDMisc::strToInputType(*pInputType));

      } else {

	Warning("ZeeDLauncher::setMetaData",
	    "Cannot read inputType from MetaData, using value from jobOptions: %s",
	    ZeeDMisc::inputTypeToStr((*fAnaOptions)->InputType()).Data());

      }
    }

    //----------------------------------------------------------------------
    const Int_t* pDatasetID = NULL;

    if(m_MetaDataStore->retrieve(pDatasetID, "MetaTree/datasetID").isSuccess()) {

      Info("ZeeDLauncher::setMetaData", "Setting datasetID to %d",
	  *pDatasetID);
      (*fAnaOptions)->SetDatasetID(*pDatasetID);

    } else {
      Warning("ZeeDLauncher::setMetaData",
	  "Cannot read datasetID from MetaData, using MCType from jobOptions: %s",
	  ((*fAnaOptions)->MCtype()).c_str());
    }

  } else {
    Warning("ZeeDLauncher::setMetaData", "Cannot open MetaDataStore");
  }


  // Initialize and register a storage for the lumi information
  h_lumi = new TH1D("lumi_info", "lumi_info", 5, -0.5, 4.5);
  h_lumi->GetXaxis()->SetBinLabel(1, "total_sumN");
  h_lumi->GetXaxis()->SetBinLabel(2, "total_sumWgen");
  h_lumi->GetXaxis()->SetBinLabel(3, "total_sumWzeedgen");
  h_lumi->GetXaxis()->SetBinLabel(4, "GRL_sumN");
  h_lumi->GetXaxis()->SetBinLabel(5, "GRL_sumWgen");
  ServiceHandle<ITHistSvc>* histService = fSvcHelper.GetTHistSvc();
  (void)(*histService)->regHist("/ZEED/lumi_info", h_lumi);

}

//------------------------------------------------------
void ZeeDLauncher::addShelf(const TString& shelfName) {

  // create bundles with corresponding systematics attached
  vector<ZeeDSystematics::ZeeDSingleSystematics*>::iterator ishift = ZeeDSystematics::allSystematics.begin();
  for ( ; ishift != ZeeDSystematics::allSystematics.end(); ++ishift) {

    TString sysName = TString(((*ishift)->name).c_str());

    fChainBundles[*ishift] = initializeAnalysisChainsReplica(sysName, shelfName);

    // for central value add NoCuts
    if ( (*fAnaOptions)->FillNoCuts() && (*ishift)->systematics_type == ZeeDSystematics::eNoShift) {

      fNoCutsBundles[*ishift] = initializeNoCutsReplica(sysName, shelfName);
    }


  }

}

//----------------------------------------------
StatusCode ZeeDLauncher::finalize()
{
  // Finalize all the stuff

  std::map<ZeeDSystematics::ZeeDSingleSystematics*, ZeeDNoCutsBundle*>::const_iterator itnb = fNoCutsBundles.begin();
  for (; itnb != fNoCutsBundles.end(); ++itnb) {

    ZeeDNoCutsBundle::const_iterator ithm = itnb->second->begin();
    for (; ithm != itnb->second->end(); ++ithm) {
      (*ithm)->Finalize();
    }
  }

  std::map<ZeeDSystematics::ZeeDSingleSystematics*, ZeeDAnalysisChainBundle*>::const_iterator itb = fChainBundles.begin();
  for (; itb != fChainBundles.end(); ++itb) {

    ZeeDAnalysisChainBundle::const_iterator ithm = itb->second->begin();
    for (; ithm != itb->second->end(); ++ithm) {
      (*ithm)->fCutHistManager->Finalize();
    }
  }
  
  delete fEventFactory;
  this->finalizeReport();
  StatusCode sc = StatusCode::SUCCESS;
  return sc;
}

//----------------------------------------------
void ZeeDLauncher::initializeIdentifiers()
{
// Adding of selectors for best boson candidates 

    this->AddIdentifier(new ZeeDFinderZCC("FinderZCC"), "IdentifierZCC");

    this->AddIdentifier(new ZeeDFinderZCC("FinderZCC_QCD"), "IdentifierZCC_QCD");

    this->AddIdentifier(new ZeeDFinderZCF("FinderZCF"), "IdentifierZCF");
 
    this->AddIdentifier(new ZeeDFinderZmumu("FinderZmumu"), "IdentifierZmumu");


    this->AddIdentifier(new ZeeDFinderTrigger("FinderTrigger"), "IdentifierTrigger");
    
    this->AddIdentifier(new ZeeDFinderW("FinderW"), "IdentifierW");

    this->AddIdentifier(new ZeeDFinderWmu("FinderWmu"), "IdentifierWmu");

    //this->AddIdentifier(NULL, "NullIdentifier");

}

//------------------------------------------------
ZeeDIdentifier* ZeeDLauncher::getIdentifier(const TString& name) {

  std::map<TString, ZeeDIdentifier*>::const_iterator identifier = fIdentifiers.find(name);
  if (identifier == fIdentifiers.end()) {

    Error("ZeeDLauncher::getIdentifier",
	"Identifier not found: %s", name.Data());

    gSystem->Exit(EXIT_FAILURE);

  }

  return (*identifier).second;

}

//------------------------------------------------
void ZeeDLauncher::addCutSelector(ZeeDCutSelector* cutSelector)
{

    cutSelector->BookCuts();
    fCutSelectors.Add(cutSelector);

}

//------------------------------------------------
ZeeDCutSelector* ZeeDLauncher::getCutSelector(const TString& name)
{

    ZeeDCutSelector* cutSelector = (ZeeDCutSelector*)(fCutSelectors(name));

    if (cutSelector == NULL) {

            Error("ZeeDLauncher::getCutSelector",
                  "CutSelector not found: %s", name.Data());

            gSystem->Exit(EXIT_FAILURE);

    }

    return cutSelector;

}

//------------------------------------------------
void ZeeDLauncher::initializeCutSelectors()
{
// Adding of cut selectors for bosons 

    this->addCutSelector(new ZeeDAnalysisCutSelectorZCC("ZCC"));
    this->addCutSelector(new ZeeDAnalysisCutSelectorZCCCutFlow("ZCCCutFlow"));
    this->addCutSelector(new ZeeDAnalysisCutSelectorZCF("ZCF"));
    this->addCutSelector(new ZeeDAnalysisCutSelectorPreSelection("Preselection"));
    this->addCutSelector(new ZeeDAnalysisCutSelectorW("W"));
    this->addCutSelector(new ZeeDAnalysisCutSelectorWmu("Wmu"));
    this->addCutSelector(new ZeeDAnalysisCutSelectorZmumu("Zmumu"));


}

//------------------------------------------------
void ZeeDLauncher::initializeCutCaches()
{

    this->addCutCache(new ZeeDCutCache("ZCC",
                                       this->getIdentifier("IdentifierZCC"),
                                       this->getCutSelector("ZCC")));

    this->addCutCache(new ZeeDCutCache("ZCCCutFlow",
                                       this->getIdentifier("IdentifierZCC"),
                                       this->getCutSelector("ZCCCutFlow")));

    this->addCutCache(new ZeeDCutCache("ZCCQCD",
                                       this->getIdentifier("IdentifierTrigger"),
                                       this->getCutSelector("ZCC")));

    this->addCutCache(new ZeeDCutCache("ZCF",
                                       this->getIdentifier("IdentifierZCF"),
                                       this->getCutSelector("ZCF")));
   
    this->addCutCache(new ZeeDCutCache("W",
                                       (ZeeDIdentifierW*) this->getIdentifier("IdentifierW"),
                                       this->getCutSelector("W")));

    this->addCutCache(new ZeeDCutCache("Wmu",
                                       (ZeeDIdentifierWmu*) this->getIdentifier("IdentifierWmu"),
                                       this->getCutSelector("Wmu")));

    this->addCutCache(new ZeeDCutCache("Zmumu",
                                       this->getIdentifier("IdentifierZmumu"),
                                       this->getCutSelector("Zmumu")));


/*
    this->addCutCache(new ZeeDCutCache("Preselection",
                                       this->getIdentifier("IdentifierW"),
                                       this->getCutSelector("Preselection")));
*/
}

//------------------------------------------------
void ZeeDLauncher::addCutCache(ZeeDCutCache* cutSelector)
{
    fCutCaches.Add(cutSelector);
}

//------------------------------------------------
ZeeDCutCache* ZeeDLauncher::getCutCache(const TString& name)
{

    ZeeDCutCache* cutSelector = (ZeeDCutCache*)(fCutCaches(name));

    if (cutSelector == NULL) {

            Error("ZeeDLauncher::getCutCache",
                  "CutCache not found: %s", name.Data());

            gSystem->Exit(EXIT_FAILURE);

    }

    return cutSelector;

}

// Initialization of NoCuts folder in output 
//------------------------------------------------------
ZeeDNoCutsBundle* ZeeDLauncher::initializeNoCutsReplica(TString sysName, TString shelfName)
{

    ZeeDNoCutsBundle* bundle = new ZeeDNoCutsBundle;

    TString name          = TString(shelfName+"/"+sysName+"/NoCuts/Weights/");
    this->AddHistManager(new ZeeDHistManagerWeights(name), bundle);

    TString name_HM_event = TString(shelfName+"/"+sysName+"/NoCuts/Event/");
    this->AddHistManager(new ZeeDHistManagerEvent(name_HM_event), bundle);

    TString name_HM_electron = TString(shelfName+"/"+sysName+"/NoCuts/Electron/");
    ZeeDHistManagerElectron* electronHM = new ZeeDHistManagerElectron(name_HM_electron, kTRUE);
    electronHM->SetProperties(kTRUE);
    this->AddHistManager(electronHM, bundle);

    TString name_HM_muon = TString (shelfName+"/"+sysName+"/NoCuts/Muon/");
    ZeeDHistManagerMuon* muonHM= new ZeeDHistManagerMuon(name_HM_muon, kTRUE);
    //muonHM->SetProperties(kTRUE);
    this->AddHistManager(muonHM, bundle);



    TString name_HM_boson = TString(shelfName+"/"+sysName+"/NoCuts/Boson/");
    ZeeDHistManagerBoson* bosonHM = new ZeeDHistManagerBoson(name_HM_boson);
    bosonHM->SetProperties(kFALSE);
    this->AddHistManager(bosonHM, bundle);


    if( (*fAnaOptions)->CalcJets() ) {

      if ((*fAnaOptions)->DoZjet()){
	TString name_HM_Zjet = TString(shelfName+"/"+sysName+"/NoCuts/Zjet/");
	ZeeDHistManagerZjet* ZjetHM = new ZeeDHistManagerZjet(name_HM_Zjet);
	ZjetHM->SetProperties(kTRUE,kFALSE);
	this->AddHistManager(ZjetHM, bundle);
      }
      if ((*fAnaOptions)->DoZjetAna()){
	TString name_HM_ZjetAna = TString(shelfName+"/"+sysName+"/NoCuts/ZjetAna/");
	ZeeDHistManagerZjetAna* ZjetAnaHM = new ZeeDHistManagerZjetAna(name_HM_ZjetAna);
	ZjetAnaHM->SetProperties(kTRUE,kFALSE);
	this->AddHistManager(ZjetAnaHM, bundle);
      }
      if ((*fAnaOptions)->DoZplusJet()){
	TString name_HM_ZplusJet = TString(shelfName+"/"+sysName+"/NoCuts/ZplusJet/");
	ZeeDHistManagerZplusJet* ZplusJetHM = new ZeeDHistManagerZplusJet(name_HM_ZplusJet);
	ZplusJetHM->SetProperties(kTRUE,kFALSE);
	this->AddHistManager(ZplusJetHM, bundle);

      }

      if ((*fAnaOptions)->CalcJetCharge()){
	TString name_HM_JetCharge = TString(shelfName+"/"+sysName+"/NoCuts/JetCharge/");
	ZeeDHistManagerJetCharge* JetChargeHM = new ZeeDHistManagerJetCharge(name_HM_JetCharge);
	JetChargeHM->SetProperties(kTRUE,kFALSE);
	this->AddHistManager(JetChargeHM, bundle);

      }

    }

    if( (*fAnaOptions)->FillGenInfo() && (*fAnaOptions)->IsMC() ) {
        TString name_HM_gen = TString(shelfName+"/"+sysName+"/NoCuts/GenInfo/");
        this->AddHistManager(new ZeeDHistManagerGenInfo(name_HM_gen), bundle);
    }
    
         
    return bundle;
}

// Initialization of analysis folders in output
//------------------------------------------------------
ZeeDAnalysisChainBundle* ZeeDLauncher::initializeAnalysisChainsReplica(TString sysName, TString shelfName)
{

    // - Create analysis chains

    // Create new bundle to hold analysis chains:
    ZeeDAnalysisChainBundle* bundle = new ZeeDAnalysisChainBundle;


    if ( (*fAnaOptions)->CalcZ() && (*fAnaOptions)->RunZCCAna() ) {

        // Add cut hist manager for Z-CC selection:
        TString nameCC = TString(shelfName+"/"+sysName+"/ZCC/");

        ZeeDAnalysisCutHistManagerZCC* CutsHistsZCC = new ZeeDAnalysisCutHistManagerZCC(nameCC);

        // Bundle cut hist manager with CC identifier:
        this->AddCutHistManager(CutsHistsZCC, this->getCutCache("ZCC"), bundle);
    }
    
    
    if( (*fAnaOptions)->CalcW() ) {
         TString nameW = TString (shelfName+"/"+sysName+"/W/");
         ZeeDAnalysisCutHistManagerW* CutHistsW = new ZeeDAnalysisCutHistManagerW(nameW);
         this->AddCutHistManager(CutHistsW, this->getCutCache("W"), bundle);

    }
   
    if( (*fAnaOptions)->CalcWmu() ) {
         TString nameWmu = TString (shelfName+"/"+sysName+"/Wmu/");
         ZeeDAnalysisCutHistManagerWmu* CutHistsWmu = new ZeeDAnalysisCutHistManagerWmu(nameWmu);
         this->AddCutHistManager(CutHistsWmu, this->getCutCache("Wmu"), bundle);
    }

     if( (*fAnaOptions)->CalcZmumu() ) {
         TString nameZmumu = TString (shelfName+"/"+sysName+"/Zmumu/");
         ZeeDAnalysisCutHistManagerZmumu* CutHistsZmumu = new ZeeDAnalysisCutHistManagerZmumu(nameZmumu);
         this->AddCutHistManager(CutHistsZmumu, this->getCutCache("Zmumu"), bundle);
    }


       
    if( (*fAnaOptions)->CalcZ() && (*fAnaOptions)->RunZCFAna() ) {
        // Add Z CF analysis cut-histogram mamanger
        TString nameCF = TString(shelfName+"/"+sysName+"/ZCF/");

        ZeeDAnalysisCutHistManagerZCF* CutsHistsZCF = new ZeeDAnalysisCutHistManagerZCF(nameCF);

        // Bundle cut hist manager with CF identifier:
        this->AddCutHistManager(CutsHistsZCF, this->getCutCache("ZCF"), bundle);
    }

    if( (*fAnaOptions)->CalcZ() && (*fAnaOptions)->RunZBaselineAna() ) {

        TString nameZBaseline(shelfName+"/"+sysName);

        ZeeDAnalysisCutHistManagerZBaseline* CutsHistsZBaseline = new ZeeDAnalysisCutHistManagerZBaseline(nameZBaseline+"/ZBaseline/");
        this->AddCutHistManager(CutsHistsZBaseline, this->getCutCache("ZCC"), bundle);

        ZeeDAnalysisCutHistManagerCutFlow* CutsHistsZBaselineCutFlow = new ZeeDAnalysisCutHistManagerCutFlow(nameZBaseline+"/ZBaselineCutFlow/");
        this->AddCutHistManager(CutsHistsZBaselineCutFlow, this->getCutCache("ZCCCutFlow"), bundle);

        if( (*fAnaOptions)->BookBackgroundCuts() ) {

            ZeeDAnalysisCutHistManagerZBaseline* CutsHistsZBaseline_QCD = new ZeeDAnalysisCutHistManagerZBaseline(nameZBaseline+"/ZBaseline_QCD/",
                                                                                                                  ZeeDAnalysisCutHistManagerZBaseline::Mode::QCD);
            this->AddCutHistManager(CutsHistsZBaseline_QCD, this->getCutCache("ZCCQCD"), bundle);
        }



    }

    if( (*fAnaOptions)->WriteTTreeWithCutHistManager() ) {
        ZeeDAnalysisCutHistManagerWriter* CutsHistsWriter = new ZeeDAnalysisCutHistManagerWriter("Writer");
        this->AddCutHistManager(CutsHistsWriter, this->getCutCache("Preselection"), bundle);
    }

    return bundle;
}

//------------------------------------------------------
void ZeeDLauncher::analyseEvent(const RawEvent* rawEvent)
{

  ZeeDEvent* event = NULL;
  bool passedGRLCut = false;
  float centralGenWeight = 0.;

  // This loop over all systematics
  for (vector<ZeeDSystematics::ZeeDSingleSystematics*>::const_iterator ishift = ZeeDSystematics::allSystematics.begin();
      ishift != ZeeDSystematics::allSystematics.end(); ++ishift) {

    // Reset shifts
    fSys->resetAllShifts();
    fSys->enableShift(*ishift);


    if (event == NULL || (*ishift)->shift_type == ZeeDSystematics::phaseSpaceShift) {

      // "heavy" re-calculation of the whole event: electron kinematics,
      // e-e/e-mu/... pairs construction, weights calculation
      if (event != NULL) delete event;
      event = fEventFactory->CalcEvent(rawEvent);
      fCutCaches.R__FOR_EACH(ZeeDCutCache, evaluate)(event);
      fEventFactory->CalcGenWeight(event);
      fEventFactory->CalcRecWeight(event);

    } else if ((*ishift)->shift_type == ZeeDSystematics::weightShift) {

      // "light" re-calculation for the event gen weights
      fEventFactory->CalcGenWeight(event);
      // "light" re-calculation of the electron rec weights only
      fEventFactory->CalcRecWeight(event);
    }

    // check a GRL condititon and analyse the event
    if ((*ishift)->systematics_type == ZeeDSystematics::eNoShift) centralGenWeight = event->GetGenWeight();
    if( this->CheckManualGRL(event) ) {

      this->DoAnalysis(event, *ishift);
      passedGRLCut = true;
    }

    // delete event if it was "heavily" changed
    if ((*ishift)->shift_type == ZeeDSystematics::phaseSpaceShift) {
      delete event;
      event = NULL;
    }

  }

  if (event != NULL) delete event;

  // Fill lumi information
  float genWeight = rawEvent->GetGenWeight();
  h_lumi->AddBinContent(1, 1.);
  h_lumi->AddBinContent(2, genWeight);
  h_lumi->AddBinContent(3, centralGenWeight);
  if (passedGRLCut) {
    h_lumi->AddBinContent(4, 1.);
    h_lumi->AddBinContent(5, genWeight);
  }

}

//-----------------------------------------------------
void ZeeDLauncher::DoAnalysis(ZeeDEvent* event, ZeeDSystematics::ZeeDSingleSystematics* sys)
{

  // Loop over all NoCuts bundles of HistManages
  // do NoCuts for central value only
  if ((*fAnaOptions)->FillNoCuts() == true && sys->systematics_type == ZeeDSystematics::eNoShift) {

    ZeeDNoCutsBundle* noCutsBundle = fNoCutsBundles[sys];

    // Loop over all HistMaganers in a given bundle
    for ( unsigned index = 0; index < noCutsBundle->size(); ++index ) {

      ZeeDHistManager* histManager = noCutsBundle->at(index);

      histManager->SetLevel(sys->level);
      histManager->Initialize();


      histManager->UseEvent(event);
      // XXX: remove it
      histManager->Fill((ZeeDCutBit()));

    } // loop over HistManagers
  }


  // Loop over all bundles of CutHistManages

  ZeeDAnalysisChainBundle* chains = fChainBundles[sys];
  // For each analysis chain, set event identification and
  // loop over all CutHistManagers in a given bundle
  vector<ZeeDAnalysisChain*>::const_iterator it_chain = chains->begin();
  for (; it_chain != chains->end(); ++it_chain) {

    ZeeDCutCache*       cutCache       = (*it_chain)->fCutCache;
    ZeeDCutHistManager* cutHistManager = (*it_chain)->fCutHistManager;

    cutHistManager->SetDefaultLevel(sys->level);
    if(sys->systematics_type != ZeeDSystematics::eNoShift) {
      cutHistManager->SetBookCutControl(kFALSE);
    }

    cutHistManager->Initialize();


    // Loop over all reconstructed and selected Z boson candidates
    unsigned int nIdentifications = cutCache->fnIdentifications;

    if (nIdentifications == 0) {
      // It could be, that there are no reconstructed bosons in the event.
      // For such cases all the HM still should be called in order to
      // fill GEN quantities
      if (cutCache->fnIdentifications == 0) {
        // You need to set all of the current bosons in event
		event->SetCurrentBoson((const ZeeDBosonZ*)(NULL));
		event->SetCurrentBosonW((const ZeeDBosonW*)(NULL));
		ZeeDCutSelector* cutSelector = cutCache->fCutSelector;
		cutSelector->evaluate(event);
		cutHistManager->UseEvent(event, cutSelector->getCutMask());
      }
    } else {
      // Normal case: run the HM's on the reconstructed bosons
      for(UInt_t idnumber = 0; idnumber != cutCache->fnIdentifications; ++idnumber) {
		cutCache->fIdentifier->SetEventIdentification(event, idnumber);
		cutHistManager->UseEvent(event, cutCache->fCutMasks.at(idnumber));
      }
    }

  } // loop over CutHistManagers

}
