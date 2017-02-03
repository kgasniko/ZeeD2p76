#pragma once

////////////////////////////////////////////////////////
// Name    : ZeeDHistManager.h
////////////////////////////////////////////////////////

// Root includes
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TObjArray.h>
#include <THashTable.h>
#include <TNamed.h>
#include <TProfile.h>
#include <TProfile3D.h>

#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDTools/ZeeDCutBit.h"
#include "ZeeDTools/ZeeDSystematics.h"
#include "ZeeDArrays/ZeeDBinGrid.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

class ZeeDEvent;
class ZeeDBinGrid;
class ITHistSvc;
class ZeeDPoissonRandomVault;

// Class, which stores options of histograms
class ZeeDHistOptions: public TNamed {
public:

    ZeeDHistOptions(const TString& name, bool BootStrap = false) :
            TNamed(name, name), fOptHist(NULL) {
        fBootstrap = BootStrap;
        fIsInLevel = true;
    }

    virtual ~ZeeDHistOptions() {
    }

    // Get Methods
    bool GetBootstrap() {
        return fBootstrap;
    }

    const TObjArray* GetOptHist() {
        return fOptHist;
    }

    ZeeDLevelBit GetLevelBit() const {
        return fLevelBit;
    }
    bool IsInLevel() const {
        return fIsInLevel;
    }

    // Set Methods
    void SetBootstrap(bool Bootstrap) {
        fBootstrap = Bootstrap;
    }

    void SetOptHist(TObjArray* optHist) {
        fOptHist = optHist;
    }

    void SetLevelBit(const ZeeDLevelBit& levelBit) {
        fLevelBit = levelBit;
    }
    void SetIsInLevel(bool isInLevel) {
        fIsInLevel = isInLevel;
    }

private:
    bool fBootstrap;
    TObjArray* fOptHist;
    ZeeDLevelBit fLevelBit;
    bool fIsInLevel;
};




/** Basic class for managing of histograms
@author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
@date 2008/08/01
*/
class ZeeDHistManager : public TObject {

public:

    virtual void Fill() = 0;
    virtual void Fill(ZeeDCutBit eventMask __attribute__((unused)) ) { Fill(); }

    explicit ZeeDHistManager(const TString& name);

    /*
    ZeeDHistManager(const ZeeDHistManager& rhs);
    ZeeDHistManager& operator=(const ZeeDHistManager& rhs);
    */

    virtual ~ZeeDHistManager();

    /** Deep clone of the original */
    virtual ZeeDHistManager* Clone(const char* newname = "") const;

    virtual void SetName(const TString& name);
    virtual void UseEvent(const ZeeDEvent* event) {
        fEvent = event;
    }

    virtual void Initialize();

    virtual void BookHistos() = 0;

    virtual void Finalize() {}

    /** Get Bin from BinGrid with value "Value"
        at an index position in the GridArray
    */
    int GetBinGrid2D(Double_t x, Double_t y, const char* name);

    /** Tha same for 2D histograms
    */
    int GetBin2D(Double_t x, Double_t y, const char* name);
    int GetBin1D(Double_t x, const char* name);

    /** Returns the index of the histogram called "name" */
    //virtual Int_t GetHistIndex(const TString& name) const;

    /** Returns the index of the grid called "name" */
    //virtual Int_t GetGridIndex(const TString& name) const;

    virtual const ZeeDEvent* GetEvent() const {
        return fEvent;
    }
    virtual TString GetHMName() const {
        return fHistDirName;
    }

    void SetGridified(Bool_t gridified) {

        bGridified = gridified;

    }

    void SetGridBin(UInt_t idx) {

        fGridBin = idx;

    }

    void SetNGridBins(UInt_t ngridbins) {

        fNGridBins = ngridbins;

    }

    void SetBinLabels(const std::vector<TString>& binLabels) {
        fBinLabels = binLabels;
    }

protected:

    ZeeDSvcHelper fSvcHelper;

    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;

    ZeeDPoissonRandomVault* fRndVault;

    ZeeDSystematics* fSys;

    /** Add a TH1D histogram to the current array
        Sets the number of bins, the range and the axis labels
    */
    virtual void AddTH1D(const TString& name, Int_t nbins,
                         Double_t low, Double_t high, TString xaxis, TString yaxis = "", ZeeDLevelBit levelBit = ZeeDHistLevel::Analysis);

    /** Add a TH1D histogram to the current array
        Sets the number of bins, the range and the axis labels
    */
    virtual void AddTH1D(const TString& name, Int_t nbins,
                         const Double_t* xbins, TString xaxis, TString yaxis = "", ZeeDLevelBit levelBit =  ZeeDHistLevel::Analysis);

    /** Add a TH1D histogram to the current array
        Sets the number of bins, the range and the axis labels
    */
    virtual void AddTH1D(const TString& name, TString filename,
                         TString axis, TString yaxis = "", ZeeDLevelBit levelBit = ZeeDHistLevel::Analysis);

    /** Add a TH2D histogram to the current array
        Sets the number of bins, the range and the axis labels
    */
    virtual void AddTH2D(const TString& name, Int_t nbinsx,
                         Double_t lowx, Double_t highx, Int_t nbinsy, Double_t lowy,
                         Double_t highy, TString xaxis, TString yaxis, ZeeDLevelBit levelBit = ZeeDHistLevel::Analysis);

    /** Add a TH2D histogram to the current array
        Sets the number of bins, the range and the axis labels
    */
    virtual void AddTH2D(const TString& name, Int_t nbinsx,
                         const Double_t* xbins, Int_t nbinsy, Double_t lowy,
                         Double_t highy, TString xaxis, TString yaxis, ZeeDLevelBit levelBit = ZeeDHistLevel::Analysis);

    /** Add a TH2D histogram to the current array
        Sets the number of bins, the range and the axis labels
    */
    virtual void AddTH2D(const TString& name, Int_t nbinsx, Double_t lowx,
                         Double_t highx, Int_t nbinsy, const Double_t* ybins,
                         TString xaxis, TString yaxis, ZeeDLevelBit levelBit = ZeeDHistLevel::Analysis);

    /** Add a TH2D histogram to the current array
        Sets the number of bins, the range and the axis labels
    */
    virtual void AddTH2D(const TString& name, Int_t nbinsx,
                         const Double_t* xbins, Int_t nbinsy, const Double_t* ybins,
                         TString xaxis, TString yaxis, ZeeDLevelBit levelBit = ZeeDHistLevel::Analysis);

    /** Add a TH2D histogram to the current array
        Sets the number of bins, the range by BinGrid File, and the axis labels
    */
    virtual void AddTH2D(const TString& name, TString filename,
                         TString xaxis, TString yaxis , ZeeDLevelBit levelBit = ZeeDHistLevel::Analysis);

    /** Add a TH2D histogram to the current array
        Sets the bins by BinGrid files and the axis labels
    */
    virtual void AddTH2D(const TString& name,
                         const ZeeDBinGrid& binGrid_xaxis,
                         const ZeeDBinGrid& binGrid_yaxis,
                         TString xaxis, TString yaxis,
                         ZeeDLevelBit levelBit = ZeeDHistLevel::Analysis);

    /** Add a TH3D histogram to the current array
        Sets the number of bins, the range by BinGrid File, and the axis labels
    */
    virtual void AddTH3D(const TString& name, TString filename,
                         TString xaxis, TString yaxis, TString zaxis , ZeeDLevelBit levelBit = ZeeDHistLevel::Analysis);

    virtual void AddTH3DVector(TString name, TString filename, TString filenameExtraBinning,
                               TString xaxis, TString yaxis, TString zaxis , TString extraAxis, ZeeDLevelBit levelBit = ZeeDHistLevel::Analysis);


    /** Add a TProfile histogram to the current array
        Sets the number of bins, the range and the axis labels
    */
    virtual void AddTProfile(const TString& name, Int_t nbins,
                             Double_t low, Double_t high, TString xaxis,
                             TString yaxis, Option_t* error, ZeeDLevelBit levelBit = ZeeDHistLevel::Analysis);

    /** Add a TProfile histogram to the current array
        Sets the number of bins, the range and the axis labels
    */
    virtual void AddTProfile(const TString& name, Int_t nbins,
                             Double_t xlow, Double_t xhigh, Double_t ylow, Double_t yhigh,
                             TString xaxis, TString yaxis, Option_t* error, ZeeDLevelBit levelBit = ZeeDHistLevel::Analysis);

    /** Add a TProfile histogram to the current array
        Sets the number of bins, the range and the axis labels
    */
    virtual void AddTProfile(const TString& name, Int_t nbins,
                             const Double_t* xbins, Double_t ylow, Double_t yhigh,
                             TString xaxis, TString yaxis, Option_t* error, ZeeDLevelBit levelBit = ZeeDHistLevel::Analysis);

    virtual void AddTProfile3D(const TString& name, TString filename,
                               TString xaxis, TString yaxis, TString zaxis , ZeeDLevelBit levelBit = ZeeDHistLevel::Analysis);


    /** Add a Grid to the grid array. Sets name and the axis labels */
    virtual void AddGrid(const TString& name, TString filename,
                         TString xaxis = "", TString yaxis = "");

    virtual void AddHistoFromGrid(const TString& name, const TString& filename, ZeeDLevelBit levelBit = ZeeDHistLevel::Analysis);
    virtual void AddResponseHistoFromGrid(const TString& name, const TString& filename, Bool_t unfolding_in_y, ZeeDLevelBit levelBit = ZeeDHistLevel::Analysis);

    /** Add pair of histograms for 1D efficiency calculation */
    virtual void AddEff1D(const TString& name,
			  Int_t nbins, Double_t xmin, Double_t xmax, TString effname);

    /** Add pair of histograms for 2D efficiency calculation */
    virtual void AddEff2D(const TString& name,
			  Int_t nbinsx, Double_t xmin, Double_t xmax, TString effname1,
			  Int_t nbinsy, Double_t ymin, Double_t ymax, TString effname2);


    /** Book histograms for bootstrap method */
    virtual void DoBootstrap(const TString& name);

    /** Fill a TH1 histogram with value "Value" and weight "Weight"
    */
    virtual Int_t FillTH1(Double_t Value, Double_t Weight, const char* name);

    /** Fill a TH1 histogram with value "Value" and weight "Weight"
        at an index position in the HistArray using Gaussian with sigma
        "Sigma"
    */
    virtual void FillTH1Gauss(Double_t Value, Double_t Weight, const char* name,
                              Double_t Sigma);

    /** Fill a TH1 histogram with a "Name" and weight "Weight"
        at an index position in the HistArray
    */
    virtual Int_t FillTH1(const char* Name, Double_t Weight, const char* name);


    /** Fill a TH2 histogram with value "ValueX", "ValueY" and weight "Weight"
        at an index position in the HistArray
    */
    virtual Int_t FillTH2(Double_t ValueX, Double_t ValueY,
                         Double_t Weight, const char* name);

    virtual void FillTH2(const char* namex, const char* namey,
                         Double_t Weight, const char* name);

    /** Fill a TH3 histogram with value "ValueX", "ValueY", "ValueZ" and weight "Weight"
        at an index position in the HistArray
    */
    virtual void FillTH3(Double_t ValueX, Double_t ValueY, Double_t ValueZ,
                         Double_t Weight, const char* name);

    virtual void FillTProfile3D(Double_t ValueX, Double_t ValueY, Double_t ValueZ, Double_t T,
                                Double_t Weight, const char* name);

    virtual void FillTH3Vector(Double_t ValueX, Double_t ValueY, Double_t ValueZ, Double_t ValueExtra,
                               Double_t Weight, const char* name);

    /** Fill a TProfile histogram with x-value "XValue",
        y-value "YValue" and weight "Weight" at an index position in the HistArray
    */
    virtual void FillTProfile(Double_t XValue, Double_t YValue,
                              Double_t Weight, const char* name);

    /** Fill a grid with value "Value" and weight "Weight"
        at an index position in the GridArray
    */
    virtual void FillGrid1D(Double_t Value, Double_t x, const char* name);

    /** Fill a grid with value "Value" and weight "Weight"
        at an index position in the GridArray
    */
    virtual void FillGrid2D(Double_t Value, Double_t x, Double_t y, const char* name);

    /** Fill efficiency 1D histograms */
    virtual void FillEffHist(Double_t variable, Bool_t monitorCondition, Bool_t signalCondition, Double_t Weight, const char* name, const char* eff);

    /** Fill efficiency 2D histograms */
    virtual void FillEffHist(Double_t variable1, Double_t variable2, Bool_t monitorCondition, Bool_t signalCondition, Double_t Weight, const char* name, const char* eff1, const char* eff2);

    virtual Int_t NoSuchHist(const char* name, TCollection* array) const;
    virtual TH1* IndexOutOfBounds() const;

    /** Sets the range, axis labels and register histogram
        in the athena hist service
    */
    virtual void Register(TH1* hist, const TString& name,
                          TString xaxis, TString yaxis, ZeeDLevelBit levelBit);

    virtual void Register(TH2* hist, const TString& name,
                          TString xaxis, TString yaxis, ZeeDLevelBit levelBit);

    virtual void Register(TH3* hist, const TString& name,
                          TString xaxis, TString yaxis, TString zaxis, ZeeDLevelBit levelBit);

private:

    /**  Name of the directory to store histos */
    TString fHistDirName;    //!

    /**  Event object */
    const ZeeDEvent* fEvent;       //!

    ZeeDLevelBit fLevel;
    ZeeDLevelBit fLevelMask;

    Bool_t bIsInitialized;

    Bool_t bGridified;

    /**  The TObjArray of histograms */
    /* TObjArray* fHistArray;   //! */
    THashTable* fHistArray;   //!

    /**  The TObjArray of grids */
    THashTable* fGridArray;   //!

    /** Options for histogram */
    THashTable* fOptArray ;   // !

    Bool_t CheckLevel(ZeeDLevelBit levelBit);

    UInt_t fGridBin;

    UInt_t fNGridBins;

    std::vector<TString> fBinLabels;

public:

    /** Returns grid stored with index = index */
    virtual ZeeDBinGrid* GetGrid(const char* name) const;

    virtual TH1* GetTH1(const char* name) const;

    virtual TH2* GetTH2(const char* name) const;

    virtual TH3* GetTH3(const char* name) const;

    virtual TProfile* GetTProfile(const char* name) const;

    virtual TProfile3D* GetTProfile3D(const char* name) const;

    virtual void SetLevelMask(ZeeDLevelBit levelMask) {
        fLevelMask = levelMask;
    }

    virtual void SetLevel(ZeeDLevelBit level) {
        fLevel = level;
    }

    virtual void SetLevelAndMask(ZeeDLevelBit level, ZeeDLevelBit levelMask) {

        this->SetLevel(level);
        this->SetLevelMask(levelMask);

    }

};


inline TH1* ZeeDHistManager::GetTH1(const char* name) const
{
    return ((TH1*)fHistArray->FindObject(name));
}

inline TH2* ZeeDHistManager::GetTH2(const char* name) const
{
    return ((TH2*)fHistArray->FindObject(name));
}

inline TH3* ZeeDHistManager::GetTH3(const char* name) const
{
    return ((TH3*)fHistArray->FindObject(name));
}

