#ifndef __ZEEDEFFHISTMANAGER
#define __ZEEDEFFHISTMANAGER

////////////////////////////////////////////////////////
// Name    : ZeeDEffHistManager.h
////////////////////////////////////////////////////////

#include "ZeeDHist/ZeeDHistManager.h"
#include "TString.h"

/** Basic class for managing of histograms for efficiency calculations
    @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
    @date 2008/08/14
*/
class ZeeDEffHistManager : public ZeeDHistManager {

public:

    ZeeDEffHistManager();
    explicit ZeeDEffHistManager(TString name);
    virtual ~ZeeDEffHistManager();

    using ZeeDHistManager::Fill;
    virtual void Fill() = 0;

    /**  Returns logarithmic bin grid */
    virtual Double_t* GetLogBinGrid(Int_t n, Double_t xlow, Double_t xhigh);

    /** Returns linear bin grid */
    virtual Double_t* GetLinBinGrid(Int_t n, Double_t xlow, Double_t xhigh);

protected:

    virtual void BookHistos() {}

    class Histogram {      //!
    public:
        TString Name;      //!
        Int_t   NXBins;    //!
        Double_t XLow;      //!
        Double_t XUp;       //!
        Int_t   NYBins;    //!
        Double_t YLow;      //!
        Double_t YUp;       //!
        Double_t* XBounds; //!
        Double_t* YBounds; //!
        TString XTitle;    //!
        TString YTitle;    //!
    };                     //!


    virtual Histogram DefineHisto(TString name,
                                  Int_t nbins, Double_t* bounds, TString title);

    virtual Histogram DefineHisto(TString name,
                                  Int_t nxbins, Double_t xlow, Double_t xup,
                                  Int_t nybins, Double_t ylow, Double_t yup,
                                  TString xtitle, TString ytitle);

    virtual Histogram DefineHisto(TString name,
                                  Int_t nxbins, Double_t* xbounds, Int_t nybins,
                                  Double_t* ybounds, TString xtitle, TString ytitle);

    virtual void DeclareHistos(TString effName,
                               Int_t nEntries, Histogram* histos);

    virtual void FillHistos1D(Int_t nEntries,
                              Double_t* variables, Double_t weight, Int_t index);

};

#endif
