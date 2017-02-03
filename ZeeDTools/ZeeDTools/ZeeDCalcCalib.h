#ifndef ZEEDCALCCALIB_H
#define ZEEDCALCCALIB_H

////////////////////////////////////////////////////////////////////////////////
/// Name    : ZeeDCalcCalib.h
////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>

#include <TObject.h>

#include "ZeeDTools/ZeeDSvcHelper.h"

using std::string;

class ITHistSvc;

class ZeeDBinGrid;

class TLorentzVector;
class TH1;
class TH2;
class TFile;
class TH1F;
class TH2F;
class TH1D;
class TH2D;
namespace CLHEP {

class HepSymMatrix;
class HepVector;

}

/** Zee based energy correction in abstract binning:
    determination application of the corrections.
    @author S. Glazov
    @date 23 July 2008
*/
class ZeeDCalcCalib : public TObject {

public:
    ZeeDCalcCalib(string inFileName = "", int BookHistos = 1);
    ZeeDCalcCalib(string inFileName = "", string dirname = "");
    ~ZeeDCalcCalib();

    // Methods for calculation of the calibration
    /** Add Zee event for the calibrator */
    int AddEvent(const TLorentzVector* Electron1,
                 const TLorentzVector* Electron2);


    /** Fill table with referrences masses */
    int FillRefMassTable(TLorentzVector* Electron1,
                         const TLorentzVector* Electron2);

    /** Write table with referrences masses in TH2D */
    void WriteRefMassTable();

    /** Write output calibration factors */
    int WriteCalibration();

    /** Write internal information for calibration in text file*/
    int WriteInternalArrays();

    /** Calculate calibration constants and errors */
    int GetCalibration();

    // Method for sunning calibration ------------
    /** Read internal information for calibration */
    int ReadInternalArrays(string inFileName);

    /** Sum of the information from subjobs for calibration */
    int ReadAddInternalArrays(string inFileName);

    void RebinCalibration(double nEventMinimal);

private:

    void InitCalibrator(string inFileName, int BookHistos = 1);


    /** Get abstract bin number */
    int GetBin(const TLorentzVector* Electron);


    /** Read table with referrences masses from text file  */
    void ReadRefMassTable();

    /** Return list of histograms. */
    const std::vector<TH1*> HistList () {
        return m_histos;
    }

    ZeeDSvcHelper fSvcHelper;

    int AddEvent(int bin1, int bin2, double mass, double eta, double weight = 1);

    /** Inicialization of the binning information */
    void InitBins();

    int Nbins() {
        return m_nbins;
    }

    /** Get RMS for error estimation. */
    double GetRMS();

    /** Remove internal arrays, for reinitialization */
    void DeleteCalibrator();

    /** Add bin2in to bin1in, set bin2in to unit vector */
    void MergeBins(int bin1, int bin2);

    /** binning paramters */
    int m_nbins;

    /** calibration factors: */
    CLHEP::HepVector* m_factors;

    /** vector "last": */
    CLHEP::HepVector* m_last;

    /** calibraton matrix */
    CLHEP::HepSymMatrix* m_box;

    /** store also stats: */
    CLHEP::HepVector* m_stats;

    CLHEP::HepSymMatrix* m_boxMZ;
    CLHEP::HepSymMatrix* m_boxMZcount;
    CLHEP::HepSymMatrix* m_boxCalib;

    /** output calibration */
    CLHEP::HepVector* cf;

    /** some histos: */
    std::vector<TH1*> m_histos;

    TFile* hfile;

    TH1F* m_ee_mass;
    TH2F* m_Mee_vs_eta;
    TH1D* m_corr_factor;

    TH1D* m_const_in;
    TH1D* m_const_out;

    TH2D* m_MRefMassVal;
    TH2D* m_MRefMassStat;

    TH2D* m_HBoxCalib;
    TH1D* m_Hlast;
    TH1D* m_Hstats;

    /** different functional forms */
    int   m_method;

    bool fAbs;
    /** resolution to rescale errors. */
    double m_rms;

    ZeeDBinGrid* m_binGrid;
    ZeeDBinGrid* m_OutbinGrid;

    /** Bin grid */
    ZeeDBinGrid* binGridCalib;

};

#endif // ZEEDCALIBRATOR
