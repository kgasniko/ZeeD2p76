#ifndef __ZEEDCALCJETS_H
#define __ZEEDCALCJETS_H

////////////////////////////////////////////////////////
// Name    : ZeeDCalcJets.h
////////////////////////////////////////////////////////

#include <TObject.h>

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"
#include <THashTable.h>

#include "ZeeDTools/ZeeDSystematics.h"
#include <TLorentzVector.h>

// Jet tools
#include "JetUncertainties/MultijetJESUncertaintyProvider.h"
#include "ApplyJetCalibration/ApplyJetCalibration.h"
#include "BCHCleaningTool/BCHCleaningToolRoot.h"

#include "TileTripReader/TTileTripReader.h"
//#include "TileTripReader/AthTileTripReader.h"

#include "ApplyJetResolutionSmearing/ApplyJetSmearing.h"

class TObjArray;
class ZeeDEvent;
class ZeeDJet;

class MultijetJESUncertaintyProvider;
class JetSmearingTool;

/** Class for recalibration of  jets */
class ZeeDCalcJets : public TObject {

public:

    ZeeDCalcJets();
    ~ZeeDCalcJets();

    /**  Recalibrate jets **/
    void Calculate(ZeeDEvent* event);

private:

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;   

    ZeeDSystematics* fSys;

    std::map<std::string,JetAnalysisCalib::JetCalibrationTool*>  fJetCalibrationToolMap;
    JetAnalysisCalib::JetCalibrationTool*      fJetCalibrationTool;

    /* BCH Cleaning */
    Root::TTileTripReader* fTTileTripReader;
    BCHTool::BCHCleaningToolRoot* fBCHCleaningTool;

    std::map<std::string, MultijetJESUncertaintyProvider*>  fJetMultiJESUncToolMap;
    MultijetJESUncertaintyProvider* fJetMultiJESUncTool;

    JetSmearingTool* fJetSmearTool;
    int NPseudoExp;

    std::map<std::string, std::vector<float> > m_jesUncertaintyMap;  //All JES Uncertainties except pileup and flavour  
    std::vector<float> m_jesUncertainty;

    int Ncomp;

    //***** PileUp + Flavour + PunchThrough uncertainties *****//
    Float_t OffsetMuTerm, OffsetNPVTerm, PtTerm, RhoTopo, FlavourComp, FlavourResponce, PunchThrough;

    Double_t nSegments;

    Double_t Mu, Nvtx;

    //void provideUncertainties(TLorentzVector jetFourVector);
    void provideUncertainties(ZeeDJet *Jet);

    TLorentzVector calculateUncertainties(TLorentzVector jetFourVector);

    Int_t findMuspContainer(float jet_eta, float jet_phi, const TObjArray* muonspShowers) ; // punch through correction

};

#endif // ZeeDCalcJets
