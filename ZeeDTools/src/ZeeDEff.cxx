/// the header file

#include "GaudiKernel/MsgStream.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include "ZeeDTools/ZeeDEff.h"
#include <fstream>
#include <math.h>
#include "CLHEP/Matrix/Vector.h"

#include "TH1.h"
#include "TH2.h"

#include "egammaEvent/Electron.h"

///////
/// Constructor
ZeeDEff::ZeeDEff(std::string inFileName)
{
    // Initialize constants ...
    initBins();
    int nbins = Nbins();
    m_factors = new HepVector(nbins, 1);
    cf        = new HepVector(nbins, 1);

    if ( inFileName != "") {
        ReadCalibration(inFileName);
    }

    // Book histos
    m_isEM_vs_bin = new TH2F("is_em_bin", "isEM bit vs bin", nbins, 0., nbins, 34, -1.5, 32.5);
    m_histos.push_back(m_isEM_vs_bin);

    m_ref = new TH1F("is_em_ref", "efficiency reference", nbins, 0., nbins);
    m_histos.push_back(m_ref);

}


/////////////////////////////////////////////////////////////////////////////////////
/// Destructor - check up memory allocation
/// delete any memory allocation on the heap
ZeeDEff::~ZeeDEff()
{
    // Cleanup allocated arrays...
}


/////////////////////////////////////////////////////////////////////////////////////
double ZeeDEff::Efficiency(const Analysis::Electron* Electron)
{
    int bin    = GetBin(Electron);
    double res = GetFactorBin(bin);
    return res;
}

int ZeeDEff::AddEvent(const Analysis::Electron* Electron1,
                      const Analysis::Electron* Electron2)
{


    const Analysis::Electron* EleRef = 0;
    const Analysis::Electron* EleTst = 0;

    // Find the tight electron:
    if (Electron1->isem(egammaPID::ElectronTight) == 0) {
        EleRef = Electron1;
        EleTst = Electron2;
    }

    if ( ! EleRef && (Electron1->isem(egammaPID::ElectronTight) == 0) ) {
        EleRef = Electron2;
        EleTst = Electron1;
    }

    if (!EleRef) {
        return 1;
    }

    // Got reference and trial:
    // Bin:
    int bin = GetBin(EleTst);

    int bitPosition = 1;
    int isEM = EleTst->isem();

    m_ref->Fill(bin - 0.5);

    if (isEM == 0) {
        m_isEM_vs_bin->Fill(bin - 0.5, (isEM - 1.));
    }

    if (isEM > 0) {
        for (int i = 0; i < 32; ++i) {
            if (isEM & bitPosition) {
                m_isEM_vs_bin->Fill(bin - 0.5, i);
            }

            bitPosition *= 2;
        }
    }

    return 0;
}

int ZeeDEff::GetCalibration()
{
    // Dump everything:

    return 0;
}

int ZeeDEff::ReadCalibration(std::string inFileName)
{
    ifstream file;
    file.open(inFileName.c_str(), std::ios::in);

    for ( int i = 1; i <= Nbins(); i++ ) {
        double a;
        file >> a;
        (*m_factors)(i) = a;
    }

    (*cf) = (*m_factors);

    // SHOULD BE DEBUG
    //    std::cout << "Read calibration:" << std::endl << (*m_factors) << std::endl;

    file.close();
    return 0;
}

int ZeeDEff::WriteCalibration(std::string outFileName)
{
    ofstream file;
    file.open(outFileName.c_str());
    file << (*cf) << std::endl;
    file.close();
    return 0;
}

int ZeeDEff::GetBin(const Analysis::Electron* Electron)
{
    double eta = Electron->eta();
    //    double phi = Electron->phi();

    int bin = (int) (eta + 2.5) + 1;

    if (bin > 5) {
        bin = 5;
    }

    if (bin < 1) {
        bin = 1;
    }

    return bin;
}


double ZeeDEff::GetFactorBin(int ibin)
{
    double fact = (*m_factors)(ibin);
    return fact;
}

void ZeeDEff::initBins()
{
    m_nbins = 5;
}

