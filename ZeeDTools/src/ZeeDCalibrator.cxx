/// the header file

#include "GaudiKernel/MsgStream.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include "ZeeDTools/ZeeDCalibrator.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include "ZeeDArrays/ZeeDArrayD.h"
#include "ZeeDArrays/ZeeDBinGrid.h"

#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include <TROOT.h>
#include <TPostScript.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TSystem.h>

#include "TFile.h"
#include "TH1.h"


#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Matrix/SymMatrix.h"
#include <vector>

#include "TFile.h"
#include "TH1.h"

#include "GaudiKernel/ITHistSvc.h"

// Root libraries
#include <TLorentzVector.h>

using namespace CLHEP;

using std::endl;
using std::cout;


//--------------------------------------------------------------------------
ZeeDCalibrator::ZeeDCalibrator(std::string inFileCalib, std::string inFileSmear){

    // Read Bining
    m_Calib = new ZeeDBinGrid();
    m_Calib->ReadFromFile(inFileCalib);

    if (inFileSmear != "") {
        m_Smear = new ZeeDBinGrid();
        m_Smear->ReadFromFile(inFileSmear);
    }

    Info("ZeeDCalibrator::InitBins", "NCELLs=%i", m_Calib->GetNumOfCells());

    // Check, is it absolute binning or not
    ZeeDGridCell* cell = m_Calib->GetCell(0);
    const ZeeDArrayD* low  = cell->GetCoordLow();

    if ( (low->At(0)) < 0 ) {
        fAbs = false;
    } else {
        fAbs = true;
    }

    m_Rndm = new TRandom3(1);

}


//--------------------------------------------------------------------------
ZeeDCalibrator::~ZeeDCalibrator()
{

}

//--------------------------------------------------------------------------
double ZeeDCalibrator::CorrectionFactor(const TLorentzVector* Electron) {

    double eta = Electron->Eta();
    double phi = Electron->Phi();

    if (fAbs) {
        eta = fabs(eta);
        phi = fabs(phi);
    }

    double factor = m_Calib -> GetValue(eta, phi);

    return factor;
}

//--------------------------------------------------------------------------
double ZeeDCalibrator::SmearFactor(const TLorentzVector* Electron) {

    if(m_Smear == NULL) return 1.0;
    double eta = Electron->Eta();

    if (fAbs) {
        eta = fabs(eta);
    }

    double factor = 1 + (m_Rndm->Gaus(0, 1) * (m_Smear -> GetValue(eta) ) );

    return factor;

}
