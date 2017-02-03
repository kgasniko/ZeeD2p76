#include "ZeeDHist/ZeeDEffHistManager.h"

#include <TMath.h>

//------------------------------------------------------
ZeeDEffHistManager::ZeeDEffHistManager(TString name) : ZeeDHistManager(name)
{
    // The Named Constructor
}

//------------------------------------------------------
ZeeDEffHistManager::~ZeeDEffHistManager()
{
    // The Destructor
}

//------------------------------------------------------
Double_t* ZeeDEffHistManager::GetLogBinGrid(Int_t n, Double_t xlow, Double_t xhigh)
{
    // Returns logarifmic bin grid
    Double_t* BinGrid = NULL;

    if (n <= 0 || xlow >= xhigh) {
        Error("ZeeDEffHistManager::GetLogBinGrid",
              "Input variable are not correct! n = %i xlow = %f  xhigh = %f", n, xlow, xhigh);
        return BinGrid;
    }

    BinGrid    = new Double_t[n + 1];
    BinGrid[0] = xlow;

    Double_t delta_x_log = (TMath::Log(xhigh) - TMath::Log(xlow)) / n;

    // Put equidistant binwith on a logarithmic scale
    for (Int_t i = 1; i <= n; ++i) {
        BinGrid[i] = TMath::Exp(TMath::Log(BinGrid[i - 1]) + delta_x_log);
    }

    return BinGrid;
}

//------------------------------------------------------
Double_t* ZeeDEffHistManager::GetLinBinGrid(Int_t n, Double_t xlow, Double_t xhigh)
{
    // Returns linear bin grid
    Double_t* BinGrid = NULL;

    if (n <= 0 || xlow >= xhigh) {
        Error("ZeeDEffHistManager::GetLinBinGrid",
              "Input variable are not correct! n = %i xlow = %f  xhigh = %f", n, xlow, xhigh);
        return BinGrid;
    }

    BinGrid    = new Double_t[n + 1];
    BinGrid[0] = xlow;

    Double_t delta_x = (xhigh - xlow) / n;

    // Put equidistant binwith on a linear scale
    for (Int_t i = 1; i <= n; ++i) {
        BinGrid[i] = BinGrid[i - 1] + delta_x;
    }

    return BinGrid;
}


//------------------------------------------------------
ZeeDEffHistManager::Histogram ZeeDEffHistManager::DefineHisto(TString name,
                                                              Int_t nbins, Double_t* bounds, TString title)
{
    // Declares histogram
    Histogram histo;

    histo.Name    = name;
    histo.NXBins  = nbins;
    histo.XLow    = 0.0;
    histo.XUp     = 0.0;
    histo.NYBins  = 0;
    histo.YLow    = 0;
    histo.YUp     = 0;
    histo.XBounds = bounds;
    histo.YBounds = NULL;
    histo.XTitle  = title;
    histo.YTitle  = "";

    return histo;
}

//------------------------------------------------------
ZeeDEffHistManager::Histogram ZeeDEffHistManager::DefineHisto(TString name,
                                                              Int_t nxbins, Double_t* xbounds, Int_t nybins,
                                                              Double_t* ybounds, TString xtitle, TString ytitle)
{
    // Declares histogram
    Histogram histo;

    histo.Name    = name;
    histo.NXBins  = nxbins;
    histo.XLow    = 0.0;
    histo.XUp     = 0.0;
    histo.NYBins  = nybins;
    histo.YLow    = 0;
    histo.YUp     = 0;
    histo.XBounds = xbounds;
    histo.YBounds = ybounds;
    histo.XTitle  = xtitle;
    histo.YTitle  = ytitle;

    return histo;
}

//------------------------------------------------------
ZeeDEffHistManager::Histogram ZeeDEffHistManager::DefineHisto(TString name,
                                                              Int_t nxbins, Double_t xlow, Double_t xup, Int_t nybins,
                                                              Double_t ylow, Double_t yup, TString xtitle, TString ytitle)
{
    // Declares histogram
    Histogram histo;

    histo.Name    = name;
    histo.NXBins  = nxbins;
    histo.XLow    = xlow;
    histo.XUp     = xup;
    histo.NYBins  = nybins;
    histo.YLow    = ylow;
    histo.YUp     = yup;
    histo.XBounds = NULL;
    histo.YBounds = NULL;
    histo.XTitle  = xtitle;
    histo.YTitle  = ytitle;

    return histo;
}

//------------------------------------------------------
void ZeeDEffHistManager::DeclareHistos(TString effName,
                                       Int_t nEntries, Histogram* histos)
{
    // Adds "Top" and "Bot" histos for efficiency calculations to the histarray
    TString TopBot[2];
    TopBot[0] = "Top";
    TopBot[1] = "Bot";

    for (Int_t j = 0; j < 2; ++j) {
        for (Int_t i = 0; i < nEntries; ++i) {

            TString   name   = TString(effName + histos[i].Name + TopBot[j]);

            if(histos[i].NYBins == 0) {
                Int_t     n_bins = histos[i].NXBins;
                Double_t* bounds = histos[i].XBounds;
                TString   xTitle = histos[i].XTitle;
                TString   yTitle = TString("#varepsilon_{" + effName + "}");

                AddTH1D(name, n_bins, bounds, xTitle, yTitle);

            } else if (histos[i].YBounds == NULL) {
                Int_t   nx_bins = histos[i].NXBins;
                Int_t   ny_bins = histos[i].NYBins;
                Double_t xlow    = histos[i].XLow;
                Double_t xup     = histos[i].XUp;
                Double_t ylow    = histos[i].YLow;
                Double_t yup     = histos[i].YUp;
                TString yTitle  = histos[i].YTitle;

                TString xTitle = TString(histos[i].XTitle + ", (" + effName + ")");
                AddTH2D(name, nx_bins, xlow, xup, ny_bins, ylow, yup, xTitle, yTitle);

            } else {

                Int_t   nx_bins   = histos[i].NXBins;
                Int_t   ny_bins   = histos[i].NYBins;
                Double_t* xbounds = histos[i].XBounds;
                Double_t* ybounds = histos[i].YBounds;
                TString yTitle    = histos[i].YTitle;

                TString xTitle = TString(histos[i].XTitle + ", (" + effName + ")");
                AddTH2D(name, nx_bins, xbounds, ny_bins, ybounds, xTitle, yTitle);
            }
        }
    }

    return;
}

//------------------------------------------------------
void ZeeDEffHistManager::FillHistos1D(Int_t nEntries,
                                      Double_t* variables, Double_t weight, Int_t index)
{
    // Fills  1D histos
    for(Int_t i = 0; i < nEntries; ++i) {
        FillTH1(variables[i], weight, TString::Format("%d",index + i));
    }

    return;
}
