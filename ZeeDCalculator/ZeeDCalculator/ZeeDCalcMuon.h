#pragma once

////////////////////////////////////////////////////////
// Name    : ZeeDCalcElec.h
////////////////////////////////////////////////////////
//
// Contact persons: Andrei Nikiforov, Sebastian Schmitt
//

// Root includes
#include <TObject.h>
#include <TH1D.h>

// Athena includes
#include "GaudiKernel/ToolHandle.h"
#include "egammaAnalysisUtils/EisoTool.h"
#include "MuonMomentumCorrections/SmearingClass.h"


// ZeeD analysis includes
#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDTools/ZeeDBags.h"
#include "ZeeDTools/ZeeDSystematics.h"


class ZeeDCalibrator;
class ZeeDEvent;
class ZeeDMuon;

class ZeeDCalcMuon : public TObject {

public:
	ZeeDCalcMuon();
	~ZeeDCalcMuon();

	void Calculate(ZeeDEvent* event);
private:
	ZeeDEvent* fEventInProcess;
	MuonSmear::SmearingClass* mcp_smear;

	ZeeDSystematics* fSys;
	ZeeDSvcHelper fSvcHelper;
	ServiceHandle<IZeeDAnalysisSvc> *fAnaOptions;

	void CalcMomentum(ZeeDMuon* muon);

};
