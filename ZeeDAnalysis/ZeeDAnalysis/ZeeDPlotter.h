#ifndef __ZEEDPLOTTER_H
#define __ZEEDPLOTTER_H

////////////////////////////////////////////////////////
// Name    : ZeeDPlotter.h
////////////////////////////////////////////////////////

//#include "GaudiKernel/ToolHandle.h"
//#include "GaudiKernel/Algorithm.h"
//#include "CLHEP/Units/SystemOfUnits.h"
//#include "StoreGate/StoreGateSvc.h"

#include "AthenaBaseComps/AthAlgorithm.h"

#include "ZeeDArrays/ZeeDBinGrid.h"
#include "ZeeDArrays/ZeeDGridCell.h"

#include <TObjArray.h>
#include <TString.h>
#include <TH1D.h>

using std::vector;
using std::string;
using std::map;
/** Class for plotting of histograms
    @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
    @date 2008/08/14
*/
class ZeeDPlotter : public AthAlgorithm {

public:
    ZeeDPlotter(const std::string& name, ISvcLocator* pSvcLocator);
    ~ZeeDPlotter();

    StatusCode initialize();
    StatusCode finalize();
    StatusCode execute();
    void RunEfficiencies();
    void GetDir(TString dir, vector<string> &files);
    void GetEff(ZeeDBinGrid* Top, ZeeDBinGrid* Bot, ZeeDBinGrid*& Eff, ZeeDBinGrid*& ErrEff);
	
private:
    void RunCutFlow();
    void DrawOverlayPlotsW();
    void GetCZSyst();
	void GetASyst();
	void GetXSection();	
	void GetQCD();
	void MakeQCDFile(string boson, string path, double eventNum, string fold); 
    void ScaleHist(TH1D*& hist, TH1D* totHist, double xSec);	
	void EraseBins(TH1D*& hist, int cut=-1, bool inv=false);	
	// name of root file
    vector<string> m_filenames;
    vector<string> m_dirnames;
    string m_GenHist;
    string m_boson;
    string m_path;
    string m_nameBos;

    Bool_t m_logscale;
    Bool_t m_normalize;
	Bool_t m_makeQCDFit;
    Bool_t m_calcXSection;

    Bool_t bDoSystematicPlots;
    Bool_t bDoEfficiencyPlots;
    Bool_t bDrawOverlayPlotsW;

    // for parser plotter:
    string m_dataoperation;
    vector<string> m_mcoperation;
    vector<string> m_cutflow;    
    string m_cutselector;
	string m_QCDfitHist;
	double m_QCDcut;
	int m_QCDcutBin;

    // for C_Z systematic calculator
    string m_mcname;
    vector<string> m_selectonCZ;
    string m_refHistogramRec;
    string m_refHistogramRecToy;
    string m_refHistogramGenToy;
    string m_refHistogramGen;
    vector<string> m_systlist;

	double m_czvalue;
    double m_czstat;
    double m_czsystUp;
	double m_czsystDown;
	
	double m_Az;
	double m_AzErr;

    double m_QCD;
    double m_QCDErr;

	double m_lumi;
    double m_lumi_err;
    string m_dataname;
    string m_refHistoNoCuts;

	bool m_makePDFSyst;
	vector<string> m_selectonPDF;

    ZeeDBinGrid* EffBinGridTop;
    ZeeDBinGrid* EffBinGridBot;
    ZeeDBinGrid* EffBinGrid;
    ZeeDBinGrid* ErrEffBinGrid;

    Double_t nevt_old_cut;
    int m_npadX;
    int m_npadY;
    
};

#endif // ZeeDPlotter
