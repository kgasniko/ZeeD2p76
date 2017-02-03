/// the header file

#include "GaudiKernel/MsgStream.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include "ZeeDTools/ZeeDCalcCalib.h"
#include "ZeeDMisc/ZeeDMisc.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include "ZeeDArrays/ZeeDArrayD.h"
#include "ZeeDArrays/ZeeDBinGrid.h"


#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Matrix/SymMatrix.h"

#include "GaudiKernel/ITHistSvc.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include <TROOT.h>
#include <TPostScript.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TSystem.h>

// Root libraries
#include <TLorentzVector.h>
#include "TFile.h"
#include "TH1.h"
#include "TH1.h"
#include "TH2.h"

using namespace CLHEP;

using std::endl;
using std::cout;


//--------------------------------------------------------------------------
ZeeDCalcCalib::ZeeDCalcCalib(string inFileName, int BookHistos ) : fSvcHelper("ZeeDCalcCalib")
{
    this->InitCalibrator(inFileName, BookHistos);
}

//--------------------------------------------------------------------------
ZeeDCalcCalib::ZeeDCalcCalib(string inFileName, string dirname) : fSvcHelper("ZeeDCalcCalib")
{
    this->InitCalibrator(inFileName);

    ServiceHandle<ITHistSvc>* histSvc = fSvcHelper.GetTHistSvc();

    // Register all histos:
    std::vector<TH1*> c_hists = this->HistList();

    for ( std::vector<TH1*>::iterator h = c_hists.begin(); h != c_hists.end(); ++h) {

        const string hname =  (*h)->GetName();

        const string histname = "//ZEED/" + dirname + "/" + hname;

        StatusCode sc =  (*histSvc)->regHist(histname, *h);

        if (sc.isFailure()) {
            Error("ZeeDCalcCalib::ZeeDCalcCalib",
                  TString("Unable to register") + histname);
            gSystem->Exit(EXIT_FAILURE);
        }

    }
}

//--------------------------------------------------------------------------
ZeeDCalcCalib::~ZeeDCalcCalib()
{
}

//--------------------------------------------------------------------------
void ZeeDCalcCalib::DeleteCalibrator()
{
    delete m_factors;
    delete m_last;
    delete m_box;
    delete m_stats;
    delete cf;

    delete m_boxMZ;
    delete m_boxMZcount;
    delete m_boxCalib;

    // Delete all histos:
    std::vector<TH1*> c_hists = this->HistList();

    for ( std::vector<TH1*>::iterator h = c_hists.begin(); h != c_hists.end(); ++h) {
        delete (*h);
    }


}

//--------------------------------------------------------------------------
/// Initialization:
void ZeeDCalcCalib::InitCalibrator(string /*inFileName*/, int BookHistos)
{
    // Initialize constants ...

    // Binning definition

    InitBins();
    int nbins      = Nbins();

    m_last         = new HepVector(nbins, 0);
    m_box          = new HepSymMatrix(nbins, 0);
    m_stats        = new HepVector(nbins, 0);

    m_boxMZcount   = new HepSymMatrix(nbins, 0);
    m_boxCalib     = new HepSymMatrix(nbins, 0);

    m_boxMZ        = new HepSymMatrix(nbins, 0);
    m_rms          = 0;

    std::string name = std::string("ZeeDAnalysisSvc");

    ServiceHandle<IZeeDAnalysisSvc> ZeeDAnaOptions(name, "ZeeDCalcZ");
    StatusCode sc = ZeeDAnaOptions.retrieve();

    if (sc.isFailure()) {
        Error("ZeeDCalcCalib::InitCalibrator",
              TString("Unable to retrieve") + name);
        gSystem->Exit(EXIT_FAILURE);
    }

    // Deifne calibration method
    m_method = ZeeDAnaOptions->SelectCalibMethod();
    Info("ZeeDCalcCalib::InitCalibrator", "Calibration method: %i", m_method);

    // Read table with  reff masses
    if (m_method == 4) {
        ReadRefMassTable();
    }

    // Book histos
    if (BookHistos) {
        m_ee_mass = new TH1F("ee_mass", "Calibrator mass", 100, 70.*GeV, 110.*GeV);
        m_histos.push_back(m_ee_mass);

        m_corr_factor = new TH1D("corr_factor", "Correction factor", 100, -0.5, 0.5);
        m_histos.push_back(m_corr_factor);

        m_Mee_vs_eta = new TH2F("ee_mass_eta", "Calibrator mass vs Eta",
                                50, -5., 5., 100, 70.*GeV, 110.*GeV);
        m_histos.push_back(m_Mee_vs_eta);

        m_MRefMassVal = new TH2D("MRefVal", "Ref masses",
                                 nbins, 0., nbins, nbins, 0., nbins);
        m_histos.push_back(m_MRefMassVal);

        m_MRefMassStat = new TH2D("MRefStat", "Ref masses stat",
                                  nbins, 0., nbins, nbins, 0., nbins);
        m_histos.push_back(m_MRefMassStat);

        m_HBoxCalib = new TH2D("BoxCalib", "box calib",
                               nbins, 0., nbins, nbins, 0., nbins);
        m_histos.push_back(m_HBoxCalib);

        m_Hlast = new TH1D("Last", "last",
                           nbins, 0., nbins);
        m_histos.push_back(m_Hlast);

        m_Hstats = new TH1D("Stats", "stst",
                            nbins, 0., nbins);
        m_histos.push_back(m_Hstats);

        m_const_in  = new TH1D("inconst_vs_bin", "Correction vs bin on input", nbins, 0., nbins);
        m_histos.push_back(m_const_in);

        for (int i = 1; i <= nbins; i++) {
            m_const_in->SetBinContent(i, (*m_factors)(i));
        }

        m_const_out = new TH1D("outconst_vs_bin", "Correction vs bin on output", nbins, 0., nbins);
        m_histos.push_back(m_const_out);
    } else {
        m_ee_mass = NULL;
        m_corr_factor = NULL;
        m_Mee_vs_eta = NULL;
        m_const_in = NULL;
        m_const_out = NULL;

        m_HBoxCalib = NULL;
        m_Hlast = NULL;
        m_Hstats = NULL;

        m_MRefMassVal = NULL;
        m_MRefMassStat = NULL;
    }
}

//--------------------------------------------------------------------------
int ZeeDCalcCalib::AddEvent(int bin1, int bin2, double mass, double eta, double weight)
{

    // monitoring:
    m_ee_mass->Fill(mass);
    m_Mee_vs_eta->Fill(eta, mass);

    // Different methods:
    double corrf = 1*weight;
    double addf  = 0.5*weight;

    if ( bin1 == 0 || bin2 == 0) {
        return 0;
    }

    double localMZ = (*m_boxCalib)(bin1, bin2);
    if (localMZ == 0) {
        Info("ZeeDCalcCalib::AddEvent ", "RefMass = 0 for bins: %i and %i", bin1, bin2);
        return 0;
    }
    corrf = ((localMZ - mass) / localMZ) * weight;

    m_corr_factor->Fill(corrf);

    (*m_box)(bin1, bin1) += addf;
    (*m_box)(bin2, bin2) += addf;
    (*m_box)(bin1, bin2) += addf;

    (*m_last)(bin1) += corrf;
    (*m_last)(bin2) += corrf;

    // also increment stats:
    (*m_stats)(bin1) += sqrt(weight);
    (*m_stats)(bin2) += sqrt(weight);

    return 0;
}

//--------------------------------------------------------------------------
int ZeeDCalcCalib::AddEvent(const TLorentzVector* Electron1, const TLorentzVector* Electron2)
{
    // Bins for the two electrons:
    int bin1 = GetBin(Electron1);
    int bin2 = GetBin(Electron2);

    // Current invariant mass
    double mass = ( *Electron1 + *Electron2 ).M();
    double eta  = ( *Electron1 + *Electron2 ).Eta();
    double eta1 = fabs(( *Electron1).Eta());
    double eta2 = fabs(( *Electron2).Eta());
    //if we are in the crack, downweight
    double weight = 1;

    if ( (eta1 > 1.37 && eta1 < 1.52) && (eta2 > 1.37 && eta2 < 1.52)  ){ //both in crack
      weight = 0.0;
    }

    else if ( (eta1 > 1.37 && eta1 < 1.52) || (eta2 > 1.37 && eta2 < 1.52)  ){ //only one in crack
      weight = 0.1;
    }

    return AddEvent(bin1, bin2, mass, eta, weight);
}

//--------------------------------------------------------------------------
double ZeeDCalcCalib::GetRMS()
{
    if ((m_rms == 0) && (m_corr_factor == NULL ) ) {
        return 0.037;
    }

    if (m_rms != 0) {
        return m_rms;
    }

    return m_corr_factor->GetRMS();
}

//--------------------------------------------------------------------------
int ZeeDCalcCalib::GetCalibration()
{

    // invert box:
    for (int i = 1; i <= Nbins(); i++) {
        if ( (*m_box)(i, i) == 0 ) {
            (*m_box)(i, i) = 1;
        }
    }

    int ifail;
    m_box->invert(ifail);

    Info("ZeeDCalcCalib::GetCalibration", "InvertCalibMatrix=%i", ifail);

    // result:
    HepVector cf_iter = (*m_box) * (*m_last);

    // New cf:
    (*cf) += cf_iter;

    // Histogram
    if ( m_const_out != NULL ) {
        for (int i = 1; i <= Nbins(); i++) {
            m_const_out->SetBinContent(i, (*cf)(i));
        }
    }

    return 0;
}

//--------------------------------------------------------------------------
int ZeeDCalcCalib::WriteCalibration()
{
    for (int i = 1; i <= Nbins(); i++) {
        m_OutbinGrid->Set(i - 1, (*cf)(i) );
    }

    m_OutbinGrid->SetTitleX("#eta");
    m_OutbinGrid->SetTitleY("p_{T}");
    m_OutbinGrid->SetName("Calibration");
    m_OutbinGrid->WriteIntoFile("Calib_out.txt");

    m_OutbinGrid->Reset(0.0);

    for (int i = 1; i <= Nbins(); i++) {
        m_OutbinGrid->Set(i - 1, (sqrt( (*m_box)(i, i) ) * (this->GetRMS()) ));
    }

    m_OutbinGrid->SetTitleX("#eta");
    m_OutbinGrid->SetTitleY("p_{T}");
    m_OutbinGrid->SetName("ErrorCalibration");
    m_OutbinGrid->WriteIntoFile("Error_Calib_out.txt");

    return 0;
}

//--------------------------------------------------------------------------
int ZeeDCalcCalib::WriteInternalArrays()
{
    // Fill Histograms
    for ( int i = 1; i <= Nbins(); i++ ) {
        for ( int k = 1; k <= Nbins(); k++ ) {
            m_HBoxCalib->SetBinContent(i, k, (*m_box)(i, k));
        }

        m_Hlast->SetBinContent(i, (*m_last)(i));
        m_Hstats->SetBinContent(i, (*m_stats)(i));
    }

    return 0;
}

//--------------------------------------------------------------------------
int ZeeDCalcCalib::ReadInternalArrays(string outFileName)
{

    TFile* CalibFile = new TFile("CalibFile.root");

    if (CalibFile == NULL ) {
        std::cout << "File: CalibFile.root Not found \n";
    }

    std::string Hname = outFileName;
    TH1D* Hm_box = (TH1D*)CalibFile->Get((Hname + "/BoxCalib").c_str());
    TH1D* Hm_last = (TH1D*)CalibFile->Get((Hname + "/Last").c_str());
    TH1D* Hm_stats = (TH1D*)CalibFile->Get((Hname + "/Stats").c_str());

    for ( int i = 1; i <= Nbins(); i++ ) {
        for ( int j = 1; j <= Nbins(); j++) {
            if ( i <= j) {
                (*m_box)(i, j) = Hm_box->GetBinContent(i, j);
            }
        }

        (*m_last)(i)  = Hm_last->GetBinContent(i);
        (*m_stats)(i) = Hm_stats->GetBinContent(i);
    }

    return 0;
}

//--------------------------------------------------------------------------
int ZeeDCalcCalib::ReadAddInternalArrays(string outFileName)
{

    TFile* CalibFile = new TFile("CalibFile.root");

    if (CalibFile == NULL ) {
        std::cout << "File: CalibFile.root Not found \n";
    }

    std::string Hname = outFileName;
    TH1D* Hm_box = (TH1D*)CalibFile->Get((Hname + "/BoxCalib").c_str());
    TH1D* Hm_last = (TH1D*)CalibFile->Get((Hname + "/Last").c_str());
    TH1D* Hm_stats = (TH1D*)CalibFile->Get((Hname + "/Stats").c_str());

    for ( int i = 1; i <= Nbins(); i++ ) {
        for ( int j = 1; j <= Nbins(); j++) {
            if ( i <= j) {
                (*m_box)(i, j) += Hm_box->GetBinContent(i, j);
            }
        }

        (*m_last)(i)  += Hm_last->GetBinContent(i);
        (*m_stats)(i) += Hm_stats->GetBinContent(i);
    }

    return 0;

}

//--------------------------------------------------------------------------
int ZeeDCalcCalib::GetBin(const TLorentzVector* Electron)
{
    double eta = Electron->Eta();
    double phi = Electron->Phi();

    static ZeeDArrayD coordinates(2);

    if (fAbs)        {
        eta = fabs(eta);
	phi = fabs(phi);
    }

    coordinates[0] = eta;
    coordinates[1] = phi;

    Int_t bin = (m_binGrid->GetIndex(&coordinates)) + 1;

    if (bin == 0){
        Info("ZeeDCalcCalib::GetBin ", "Bin number = 0 for eta=%f  and Phi=%f", eta, phi);
    }

    return bin;
}

//--------------------------------------------------------------------------
void ZeeDCalcCalib::InitBins()
{
    // Initialize  bin Grid:
    m_binGrid = new ZeeDBinGrid();
    m_OutbinGrid = new ZeeDBinGrid();

    const TString ElecEtaPhiBinGridFile = ZeeDMisc::FindFile("Calib/calib.in");

    m_binGrid    -> ReadFromFile(ElecEtaPhiBinGridFile);
    m_OutbinGrid -> ReadFromFile(ElecEtaPhiBinGridFile);

    Info("ZeeDCalcCalib::initBins", "NCELLs=%i", m_binGrid->GetNumOfCells());

    m_nbins = m_binGrid->GetNumOfCells();
    int nbins = Nbins();

    ZeeDGridCell* cell = m_binGrid->GetCell(0);
    const ZeeDArrayD* low  = cell->GetCoordLow();

    if ( (low->At(0)) < 0 ) {
        fAbs = false;
    } else {
        fAbs = true;
    }

    m_factors = new HepVector(nbins, 1);
    cf        = new HepVector(nbins, 0);

    // Fill vector of init calibration -----
    for ( int i = 1; i <= Nbins(); i++ ) {
        (*m_factors)(i) = m_binGrid->GetValue(i - 1);
    }

    (*cf) = (*m_factors);
}

//--------------------------------------------------------------------------
void ZeeDCalcCalib::MergeBins(int bin1in, int bin2in)
{
    // Add bin2in to bin1in, set bin2in to unit vector
    int bin1 = bin1in;
    int bin2 = bin2in;

    if ( bin1 < bin2 ) {
        bin1 = bin2in;
        bin2 = bin1in;
    }

    if ( (bin1 > Nbins())  || (bin2 > Nbins() ) ) {
        cout << "Can not merge bins " << bin1 << "," << bin2 << ", index outside the total number of bins" << Nbins() << endl;
        return;
    }

    // Transform m_box:
    for (int i = 1; i <= Nbins(); i++) {
        (*m_box)(bin1, i) += (*m_box)(bin2, i);
    }

    for (int i = 1; i <= Nbins(); i++) {
        (*m_box)(bin2, i) = 0;
    }

    (*m_box)(bin2, bin2) = 1.0;

    (*m_last)(bin1) += (*m_last)(bin2);
    (*m_last)(bin2) = 0;
}

//---------------------------------------------------------------------
void ZeeDCalcCalib::RebinCalibration(double nEventMinimal)
{
    // Rebin grid to a minimal number of entries nEvevntMin
    for (int i = 0; i < Nbins(); ++i) {
        m_binGrid->Set(i, (*m_stats)[i]);
    }

    // Make canvas
    TCanvas* can = new TCanvas("plots", "Plots", 800, 600);
    m_binGrid->Draw("Before merging");
    can -> Update();
    can -> Print("Plots/merge.ps(");
    m_binGrid->Merge(nEventMinimal);
    m_binGrid->Draw("After merging");
    can -> Update();
    can -> Print("Plots/merge.ps)");
    // Combined cells
    int nCombinedCells = m_binGrid->GetNumOfMergedCells();

    for (int i = 0; i < nCombinedCells; i++) {
        ZeeDMergedCell* mCell   = m_binGrid->GetMergedCell(i);
        const ZeeDArrayI* mList =  mCell->GetCells();
        // Merge all to the bin with the first index:
        int idxFirst = (*mList)[0] + 1;

        for (int i = 1; i < mList->GetEntriesFast(); i++) {
            int idxSecond = (*mList)[i] + 1;
            cout << "merge: " << idxFirst << "," << idxSecond << endl;
            this -> MergeBins(idxFirst, idxSecond);
        }
    }

    cout << "Merged last=" << endl << (*m_last) << endl;
    cout << "Merged box" << endl << (*m_box) << endl;
}

//--------------------------------------------------------------------------
// Filling table with ref masses
int ZeeDCalcCalib::FillRefMassTable(TLorentzVector* Electron1, const TLorentzVector* Electron2)
{

    // Bins for the two electrons:
    int bin1 = GetBin(Electron1);
    int bin2 = GetBin(Electron2);

    // Current invariant mass
    double localMZ = ( *Electron1 + *Electron2 ).M();

    (*m_boxMZ)(bin1, bin2) += localMZ;
    (*m_boxMZcount)(bin1, bin2) += 1;

    return 0;

}
//--------------------------------------------------------------------------
// Write table with ref mass.
void ZeeDCalcCalib::WriteRefMassTable()
{

    for (int i = 1; i <= Nbins(); ++i) {
        for (int k = 1; k <= Nbins(); ++k) {
            m_MRefMassVal ->SetBinContent(i, k, ((*m_boxMZ)(i, k)) );
            m_MRefMassStat->SetBinContent(i, k, ((*m_boxMZcount)(i, k)) );
        }
    }
}


//--------------------------------------------------------------------------
// Read table with reff mass
void ZeeDCalcCalib::ReadRefMassTable()
{

    const TString RefMassFilename = ZeeDMisc::FindFile("Calib/RefMass.root");

    TFile* RefMassFile = new TFile(RefMassFilename);
    if (RefMassFile == NULL ) {
        Error("ZeeDCalcCalib::ReadCalibrator",
              "Couldn't open file " + RefMassFilename);
        gSystem->Exit(EXIT_FAILURE);
    } else {
        Info("ZeeDCalcCalib::ReadCalibrator",  "file is opened " + RefMassFilename);
    }

    TH2D* HRefMass = (TH2D*)RefMassFile->Get("RefMass");

    for (int i = 1; i <= Nbins(); ++i) {
        for (int k = 1; k <= Nbins(); ++k) {
            (*m_boxCalib)(i, k) = HRefMass->GetBinContent(i, k);
        }
    }

}
