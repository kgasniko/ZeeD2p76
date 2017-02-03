#ifndef ZEEDEFF_H
#define ZEEDEFF_H

////////////////////////////////////////////////////////////////////////////////
/// Name    : ZeeDEff.h
////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>

namespace CLHEP {

class HepVector;

}

class TH1;
class TH2;
class TH1F;
class TH2F;

namespace Analysis {

class Electron;

}

/** Zee based electron resconstruction efficiency.
    @author S. Glazov
    @date 30 July 2008
*/
class ZeeDEff {

public:
    ZeeDEff(std::string inFileName = "");
    ~ZeeDEff();

    /** Get efficiency for a given electron track */
    double Efficiency(const Analysis::Electron* Electron);

    /** Add Zee event for the efficiency estimation */
    int AddEvent(const Analysis::Electron* Electron1,
                 const Analysis::Electron* Electron2);

    /** Calculate efficiency constants */
    int GetCalibration();
    int ReadCalibration(std::string inFileName);
    int WriteCalibration(std::string outFileName);

    /** Get abstract bin number */
    int GetBin(const Analysis::Electron* Electron);

    double GetFactorBin(int ibin);

    /** Return list of histograms. */
    const std::vector<TH1*> HistList () {
        return m_histos;
    }

private:
    void initBins();
    int Nbins() {
        return m_nbins;
    }

    /** binning paramters */
    int m_nbins;

    /** efficiency factors: */
    CLHEP::HepVector* m_factors;

    /** output efficiency */
    CLHEP::HepVector* cf;

    /** some histos: */
    std::vector<TH1*> m_histos;

    TH2F* m_isEM_vs_bin;
    TH1F* m_ref;
};

#endif // ZEEDEFF
