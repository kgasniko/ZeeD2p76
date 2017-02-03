#pragma once


////////////////////////////////////////////////////////
// Name    : ZeeDCalcRecWeightMuon.h
////////////////////////////////////////////////////////

// Root includes
#include <TObject.h>
#include <TH1D.h>
#include <string>

#include "MuonEfficiencyCorrections/AnalysisMuonConfigurableScaleFactors.h"

// Athena includes
#include "GaudiKernel/ToolHandle.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "ZeeDTools/ZeeDBags.h"

class ZeeDSystematics;
class ZeeDEvent;
class ZeeDMuon;
class AnalysisMuonConfigurableScaleFactors;

class ZeeDCalcRecWeightMuon {

public:

  ZeeDCalcRecWeightMuon();
  ~ZeeDCalcRecWeightMuon();

  void Calculate(ZeeDEvent* event);

private:

    /**  Pointer to the systematics manager */
    ZeeDSystematics* fSys;

    /** Calculate identification and reconstruction scale factors */
    void CalcIDRecSF (ZeeDMuon* muon, int charge, const TLorentzVector & lv);
	void CalcTriggerSF(ZeeDMuon* muon);
    void bookToyMC();
    double getToySF(int toyN, int charge, TLorentzVector lv); 
    /** Calculate isolation scale factors */
    //TODO: hope one day i will do it
    //void CalcTriggerSF(ZeeDMuon* muon);
    void bookToyMCTrig();
    double getToySFTrig(int toyN, int charge); 

       
	TH1D* hPlus;
	TH1D* hMinus;
    Analysis::AnalysisMuonConfigurableScaleFactors *m_MCPsf;
    std::vector<double> TrigPlus;
    std::vector<double> TrigMinus;
    std::vector<Analysis::AnalysisMuonConfigurableScaleFactors*> m_MCPsfVec; 

    /** Service helper */
    ZeeDSvcHelper fSvcHelper; //!

    /** Analysis options service */
    ServiceHandle<IZeeDAnalysisSvc> *fAnaOptions; //!


};

