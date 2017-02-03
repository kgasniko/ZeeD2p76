#ifndef __ZEEDHISTPLOTTER_H
#define __ZEEDHISTPLOTTER_H

////////////////////////////////////////////////////////
// Name    : ZeeDHistPlotter.h
////////////////////////////////////////////////////////

// Root includes
#include <TObject.h>
#include <TObjArray.h>
#include <TFile.h>
#include "TString.h"

class ZeeDHistArray;
class TLegend;

/** Basic class for plotting of histograms
    @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
    @date 2008/08/14
*/
class ZeeDHistPlotter : public TObject {

public:

    explicit ZeeDHistPlotter(TString filename);
    ZeeDHistPlotter();
    virtual ~ZeeDHistPlotter();

    /** Reads array of histograms from the directory dirname */
    void ReadArray(TString dirname);

    /** Reads array of histograms from the directory dirname of the
        root file filename
    */
    void ReadArray(TString dirname, TString filename);
    void ReadEfficiencyArray(TString dirname);

    /** Adds hist array */
    void AddArray(ZeeDHistArray* histArray);

    /** Add legend */
    void AddLegend(TLegend* legend) {
        fLegend = legend;
    }

    /** Draws histograms into a poscript file filename */
    void MakePlots(TString filename);

    /** Sets number of X and Y pads */
    void SetNPadsXY(Int_t padsX, Int_t padsY) {
        fNPadsX = padsX;
        fNPadsY = padsY;
    }

    /** Sets boolean flag: draw legend */
    void UseLegend(Bool_t flag = kTRUE) {
        fDrawLegend = flag;
    }

    /** Sets boolean flag: draw legend once per page */
    void UseLegendOnce(Bool_t flag = kTRUE) {
        fDrawLegendOnce = flag;
    }

    /** Sets boolean flag: draw statistics box */
    void UseStats(Bool_t flag = kTRUE);

    /** Sets draw option for the first hist array */
    void SetDrawOptionFirst(const char* option) {
        fDrawOptionFirst = option;
    }

    /** Sets draw option for the other hist arrays*/
    void SetDrawOptionOther(const char* option) {
        fDrawOptionOther = option;
    }

    void SetNormalize(Bool_t on) {
        fNormalize = on;
    }
    void SetLogScale(Bool_t on)  {
        fLogScale  = on;
    }


    TFile* GetRootFile() const {
        return fRootFile;
    }

private:

    /**  Array of arrays with histos */
    TObjArray* fHistArrays; //!

    /**  Root file with histos */
    TFile* fRootFile;       //!

    /** Plot's legend */
    TLegend* fLegend;

    /** Boolean flag: draw legend */
    Bool_t fDrawLegend;

    /** Boolean flag: draw legend once per page*/
    Bool_t fDrawLegendOnce;

    /** Boolean flag: draw statistics */
    Bool_t fUseStats;

    /**  Number of pads on X axis */
    Int_t fNPadsX;          //!

    /**  Number of pads on Y axis */
    Int_t fNPadsY;          //!

    /** log scale plots */
    Bool_t fLogScale;

    /** normalize plots */
    Bool_t fNormalize;

    /** Draw option of the first histo */
    TString fDrawOptionFirst;

    /** Draw option of the other histos */
    TString fDrawOptionOther;

    void InitRoot();
};


#endif // ZeeDHistPlotter
