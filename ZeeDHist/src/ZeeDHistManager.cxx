#include "ZeeDHist/ZeeDHistManager.h"

// std includes
#include <iostream>

// Root includes
#include <TH1.h>
#include <TH2.h>
#include <TH3D.h>
#include <TProfile.h>
#include <TProfile3D.h>
#include <TString.h>
#include <TUnixSystem.h>
#include <TSystem.h>
#include <TObjArray.h>
#include <TMath.h>

// Athena includes
#include "GaudiKernel/ITHistSvc.h"

// ZeeD includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDArrays/ZeeDBinGrid.h"
#include "ZeeDArrays/ZeeDHistTools.h"
#include "ZeeDMisc/ZeeDMisc.h"

using std::cout;
using std::endl;

/** Fill histogram using gaussian with width sigma and unit area
    Properly fill under/over flow bins */
void FillG(TH1* fHist, Double_t val, Double_t weight, Double_t sigma)
{
    Int_t nB   = fHist->GetNbinsX();
    Double_t* lowEdges = new Double_t [nB + 1];
    fHist->GetLowEdge(lowEdges);
    lowEdges[nB] = lowEdges[nB - 1] + fHist->GetBinWidth(nB);


    // Store values to be added to the histogram
    Double_t* errFuncVals = new Double_t [nB + 1];

    // Loop over nB+1 entries, including the upper boundary of the histogram
    for (Int_t i = 0; i <= nB; i++) {
        errFuncVals[i] = TMath::Erf(TMath::Abs(lowEdges[i] - val) / sigma) / 2.;
    }

    Double_t* vals = new Double_t [nB];

    // Loop over bins (nB entries)
    for ( Int_t i = 0; i < nB; i++) {
        if ( (val >= lowEdges[i]) && (val < lowEdges[i + 1]) ) {
            vals[i] = errFuncVals[i] + errFuncVals[i + 1];
        } else {
            vals[i] = TMath::Abs(errFuncVals[i] - errFuncVals[i + 1]);
        }
    }


    // Also get under/over flow bins:
    Double_t under = 0;
    Double_t over  = 0;

    if (val < lowEdges[0]) {
        over  = TMath::Erfc(TMath::Abs(val - lowEdges[nB]) / sigma) / 2.;
        under = 1. - TMath::Erfc(TMath::Abs(val - lowEdges[0]) / sigma) / 2.;
    } else if ( val > lowEdges[nB]) {
        over  = 1. - TMath::Erfc(TMath::Abs(val - lowEdges[nB]) / sigma) / 2.;
        under = TMath::Erfc(TMath::Abs(val - lowEdges[0]) / sigma) / 2.;
    } else {
        over  = TMath::Erfc(TMath::Abs(val - lowEdges[nB]) / sigma) / 2.;
        under = TMath::Erfc(TMath::Abs(val - lowEdges[0]) / sigma) / 2.;
    }

    /*

    // Check if unitarity is preserved
    Double_t sum = Over+Under;

    std::cout << "-inf " << Under << " "<< lowEdges[0] << "\n";
    for (Int_t i=0; i<nB; i++ ) {
      std::cout << lowEdges[i] << " " << vals[i] << " "<<lowEdges[i+1]<<"\n";
      sum += vals[i];
    }
    std::cout << lowEdges[nB] << " " << Over << " +inf" << "\n";

    std::cout << "Check sum="<<sum <<"\n";
    */

    // Loop over bins (nB entries)
    for (Int_t i = 0; i < nB; i++) {
        fHist->Fill(0.5 * (lowEdges[i] + lowEdges[i + 1]), vals[i]*weight);
    }

    // Also over and underflow
    fHist->Fill(lowEdges[0] - TMath::Abs(lowEdges[1])    , under * weight);
    fHist->Fill(lowEdges[nB] + TMath::Abs(lowEdges[nB - 1]), over * weight);


    delete [] lowEdges;
    delete [] vals;
    delete [] errFuncVals;
}


//------------------------------------------------------
ZeeDHistManager::ZeeDHistManager(const TString& name) : fSvcHelper(name),
    fAnaOptions(fSvcHelper.GetAnaSvc()),
    fRndVault(NULL),
    fHistDirName("/ZEED/"),
    fEvent(NULL),
    fLevel(0),     // everything is off
    fLevelMask(0), // nothing is checked
    bIsInitialized(kFALSE),
    bGridified(kFALSE)
{
    // Constructor
    fHistArray   = new THashTable();
    fGridArray   = new THashTable();
    fOptArray    = new THashTable();
    if (fAnaOptions == NULL)
       std::cout << "PROBLEM" <<std::endl;
    fRndVault    = (*fAnaOptions)->GetPoissonRandomVault();

    fSys = ZeeDSystematics::getInstance();

    this->SetName(name);
}

//------------------------------------------------------
ZeeDHistManager::~ZeeDHistManager()
{
    // Destructor
    if (fHistArray != NULL ) {
        delete fHistArray;
    }

    // Write grids into Text directory
    if (fGridArray != NULL ) {
        TString mkDirCommand = TString("if [ ! -d Text ]; then mkdir Text; fi");
        gSystem->Exec(mkDirCommand);

        TIter iter(fGridArray);

        ZeeDBinGrid* grid = NULL;

        while( (grid = (ZeeDBinGrid*)iter.Next()) ) {
            TString name = fHistDirName;
            name.ReplaceAll( "/", "." );
            name.ReplaceAll( ".ZEED.", "Text/" );
            name.Append( grid->GetName() );
            grid->WriteIntoFile(name);
        }

        delete fGridArray;
    }

    if (fOptArray != NULL) {
        delete fOptArray;
    }
}


//------------------------------------------------------
ZeeDHistManager* ZeeDHistManager::Clone(const char* newname) const
{
    // Deep clone of the original
    ZeeDHistManager* cl = static_cast<ZeeDHistManager*> (TObject::Clone());

    ServiceHandle<ITHistSvc>* histService = fSvcHelper.GetTHistSvc();

    cl->SetName(newname);

    // clone all histos
    cl->fHistArray = new THashTable;

    for ( Int_t i = 0; fHistArray->GetEntries(); i++) {
        TObject* ch = fHistArray[0].Clone();
        cl->fHistArray->Add(ch);

        // Register histogramm

        TString fullName = fHistDirName;
        fullName.Append(ch->GetName());
        StatusCode sc = (*histService)->regHist(fullName.Data(), static_cast<TH1*>(ch));

        if ( sc.isFailure() ) {
            Error("ZeeDHistManager::Clone",
                  "Unable to register histogram");
        }

    }

    return cl;
}

//------------------------------------------------------
void ZeeDHistManager::SetName(const TString& name)
{

    fSvcHelper.SetName(name);

    // Sets name
    fHistArray->SetName(name);
    fHistDirName.Append(name);
    fHistDirName.Append("/");

    fGridArray->SetName(name);
}

//------------------------------------------------------
Int_t ZeeDHistManager::FillTH1(Double_t Value, Double_t Weight, const char* name)
{

    Int_t bin = 0;

    if(bGridified) {

        TH2* hist = GetTH2(name);
        if(!hist) return -1;
        bin = hist->Fill(fGridBin, Value, Weight);

    } else {

        TH1* hist = GetTH1(name);
        if(!hist) return -1;
        bin = hist->Fill(Value, Weight);

        // Add check for bootstrap
        ZeeDHistOptions* histOpt = static_cast<ZeeDHistOptions*>( fOptArray->FindObject(name));

        if (histOpt->GetBootstrap()) {
            const TObjArray* bHists = histOpt->GetOptHist();

            for (Int_t i = 0; i < fRndVault->GetNRandom(); i++) {
                // Get Poisson weight:
                Double_t Wbs = fRndVault->GetRandom(i);
                TH1* hist = static_cast<TH1*>(bHists->At(i));
                hist->Fill(Value, Weight * Wbs);
            }
        }

    }

    return bin;
}

//------------------------------------------------------
void ZeeDHistManager::FillTH1Gauss(Double_t Value, Double_t Weight, const char* name, Double_t Sigma)
{
    TH1* hist = GetTH1(name);
    if(!hist) return;
    FillG(hist, Value, Weight, Sigma);

    // Add check for bootstrap
    ZeeDHistOptions* histOpt = static_cast<ZeeDHistOptions*>( fOptArray->FindObject(name));

    if (histOpt->GetBootstrap()) {
        const TObjArray* bHists = histOpt->GetOptHist();

        for (Int_t i = 0; i < fRndVault->GetNRandom(); i++) {
            // Get Poisson weight:
            Double_t Wbs = fRndVault->GetRandom(i);
            TH1* hist = static_cast<TH1*>(bHists->At(i));
            FillG(hist, Value, Weight * Wbs, Sigma);
        }
    }
}




//------------------------------------------------------
Int_t ZeeDHistManager::FillTH1(const char* Name, Double_t Weight, const char* name)
{
    TH1* hist = GetTH1(name);
    if (!hist) return -1;
    Int_t bin = hist->Fill(Name, Weight);

    return bin;
}

//------------------------------------------------------
Int_t ZeeDHistManager::FillTH2(Double_t ValueX, Double_t ValueY,
                              Double_t Weight, const char* name)
{

    TH2* hist = GetTH2(name);
    if(!hist) return -1;

    Int_t bin = hist->Fill(ValueX, ValueY, Weight);

    // Add check for bootstrap
    ZeeDHistOptions* histOpt = static_cast<ZeeDHistOptions*>( fOptArray->FindObject(name));

    if (histOpt->GetBootstrap()) {
        const TObjArray* bHists = histOpt->GetOptHist();

        for (Int_t i = 0; i < fRndVault->GetNRandom(); i++) {
            // Get Poisson weight:
            Double_t Wbs = fRndVault->GetRandom(i);
            TH2* hist = static_cast<TH2*>(bHists->At(i));
            hist->Fill(ValueX, ValueY, Weight * Wbs);
        }
    }

    return bin;

}

//------------------------------------------------------
void ZeeDHistManager::FillTH2(const char* namex, const char* namey,
                              Double_t Weight, const char* name)
{
    TH2* hist = GetTH2(name);
    if(!hist) return;
    hist->Fill(namex, namey, Weight);
}

//------------------------------------------------------
void ZeeDHistManager::FillTH3(Double_t ValueX, Double_t ValueY, Double_t ValueZ,
                              Double_t Weight, const char* name)
{
    TH3* hist = GetTH3(name);
    if(!hist) return;
    hist->Fill(ValueX, ValueY, ValueZ, Weight);

    //  seems return value is not needed(?)
    //ZeeDHistOptions* histOpt = static_cast<ZeeDHistOptions*>( fOptArray->FindObject(name));
    //  if not use the code below
    //(void) fOptArray->FindObject(name);

}
//------------------------------------------------------
void ZeeDHistManager::FillTH3Vector(Double_t ValueX, Double_t ValueY, Double_t ValueZ, Double_t ValueExtra,
                                    Double_t Weight, const char* name)
{
    // Fill a TH1 histogram with value "ValueX", "ValueY" and weight "Weight"
    // at an index position in the HistArray
    if ( index < 0 ) {
        return;
    }
    //Info(__PRETTY_FUNCTION__,"start");
    //std::cout <<"index = "<<index<< std::endl;

    TH1* hist = GetTH1( name );
    if(!hist) return;
    hist->Fill(ValueExtra, Weight);
    Int_t bin = hist->FindBin( ValueExtra );

    if ( bin > hist->GetNbinsX() ) return;

    //std::cout <<"rap = "<<ValueExtra<<" bin = "<<bin<< std::endl;

    TString histName = "";
    histName.Form("%s_bin_%d",hist->GetName(), bin);

    //std::cout <<"name = "<<histName<< std::endl;

    //const Int_t index3D = GetHistIndex(histName);

    //std::cout <<"index 3D= "<<index3D<< std::endl;
    
    TH3* hist3D = GetTH3( histName );
    if(!hist3D) return;
    hist3D->Fill(ValueX, ValueY, ValueZ, Weight);

    //Info(__PRETTY_FUNCTION__,"stop");

}


//------------------------------------------------------
void ZeeDHistManager::FillTProfile(Double_t XValue, Double_t YValue,
                                   Double_t Weight, const char* name)
{
    TProfile* hist = GetTProfile(name);
    if(!hist) return;
    hist->Fill(XValue, YValue, Weight);
}

//------------------------------------------------------
void ZeeDHistManager::FillTProfile3D(Double_t XValue, Double_t YValue, Double_t ZValue, Double_t TValue,
                                     Double_t Weight, const char* name)
{
    TProfile3D* hist = GetTProfile3D(name);
    if(!hist) return;
    hist->Fill(XValue, YValue, ZValue, TValue, Weight);
}

//------------------------------------------------------
void ZeeDHistManager::FillGrid1D(Double_t Value, Double_t x, const char* name)
{
    // Fill a grid with value "Value" and weight "Weight"
    // at an index position in the GridArray

    ZeeDBinGrid* grid = this->GetGrid(name);
    grid->Fill(Value, x);
}

//------------------------------------------------------
void ZeeDHistManager::FillGrid2D(Double_t Value, Double_t x, Double_t y, const char* name)
{
    // Fill a grid with value "Value" and weight "Weight"
    // at an index position in the GridArray

    ZeeDBinGrid* grid = this->GetGrid(name);
    grid->Fill(Value, x, y);
}

//------------------------------------------------------
void ZeeDHistManager::FillEffHist(Double_t variable, Bool_t monitorCondition, Bool_t signalCondition, Double_t Weight, const char* name, const char* eff)
{
  // Fill the pair of 1D efficiency histograms

  TString nametop = TString("Eff1D_") + name + TString("_") + eff + TString("_top");
  TString namebot = TString("Eff1D_") + name + TString("_") + eff + TString("_bot");

  if (monitorCondition) {
    FillTH1(variable, Weight, namebot);
    if (signalCondition) {
        FillTH1(variable, Weight, nametop);
    }
  }

}
//------------------------------------------------------
void ZeeDHistManager::FillEffHist(Double_t variable1, Double_t variable2, Bool_t monitorCondition, Bool_t signalCondition, Double_t Weight, const char* name, const char* eff1, const char* eff2)
{
  // Fill the pair of 2D efficiency histograms

 TString nametop = TString("Eff2D_") + name + TString("_") + eff1 + TString("_") + eff2 + TString("_top");
 TString namebot = TString("Eff2D_") + name + TString("_") + eff1 + TString("_") + eff2 + TString("_bot");

  if (monitorCondition) {
    FillTH2(variable1, variable2, Weight, namebot);
    if (signalCondition) {
      FillTH2(variable1, variable2, Weight, nametop);
    }
  }

}

//------------------------------------------------------
int ZeeDHistManager::GetBinGrid2D(Double_t x, Double_t y, const char* name)
{
    // Get Bin from BinGrid with value "Value"
    // at an index position in the GridArray

    ZeeDBinGrid* grid = this->GetGrid(name);

    static ZeeDArrayD coordinates(2);
    coordinates[0] = std::min(std::max(x, -2.36999), +2.3699);
    coordinates[1] = std::min(std::max(y, 0.01), 99.999);

    return grid->GetIndex(&coordinates);
}


//------------------------------------------------------
int ZeeDHistManager::GetBin2D(Double_t x, Double_t y, const char* name)
{
    // Get Bin from 2D Hist

    if ( index < 0 ) return 0;

    TH2* hist = GetTH2(name);

    return hist->FindBin(x, y);
}

//------------------------------------------------------
int ZeeDHistManager::GetBin1D(Double_t x, const char* name)
{
    // Get Bin from 1D Hist

    if ( index < 0 ) return 0;

    TH1* hist = GetTH1(name);

    return hist->FindBin(x);
}

Bool_t ZeeDHistManager::CheckLevel(ZeeDLevelBit levelBit) {

    return ( levelBit & fLevel );

}

//------------------------------------------------------
void ZeeDHistManager::AddTH1D(const TString& name, Int_t nbins,
                              Double_t low, Double_t high, TString xaxis, TString yaxis, ZeeDLevelBit levelBit)
{
    // Add a TH1D histogram to the current array
    // Sets the number of bins, the range and the axis labels

    if(!CheckLevel(levelBit)) return;

    if(bGridified) {

        TH2F* hist = new TH2F(name, name, fNGridBins, 0, fNGridBins, nbins, low, high);
        cout<< "Booking histogram " << name << " with "<< fNGridBins<< " " << nbins << endl;
        this->Register(hist, name, xaxis, yaxis, levelBit);

        for(UInt_t nbin=0; nbin != fBinLabels.size(); ++nbin) {
            hist->GetXaxis()->SetBinLabel(nbin+1, fBinLabels[nbin]);
        }

        /*
        for(Int_t nbinx=0; nbinx != hist->GetNbinsX()+1; ++nbinx) {
            for(Int_t nbiny=0; nbiny != hist->GetNbinsY()+1; ++nbiny) {
                hist->SetCellContent(nbinx+1,nbiny+1,0);
            }
        }
        */

    } else {

        TH1D* hist = new TH1D(name, name, nbins, low, high);
        this->Register(hist, name, xaxis, yaxis, levelBit);

    }

}

//------------------------------------------------------
void ZeeDHistManager::AddTH1D(const TString& name, Int_t nbins,
                              const Double_t* xbins, TString xaxis, TString yaxis, ZeeDLevelBit levelBit)
{
    // Add a TH1D histogram to the current array
    // Sets the number of bins, the range and the axis labels

    if(!CheckLevel(levelBit)) return;

    if(bGridified) {
        Warning("ZeeDHistManager::AddTH1D","No support for variable bin width in gridified HistManager, %s ignored.", name.Data());
        return;
    }

    TH1D* hist = new TH1D(name, name, nbins, xbins);
    this->Register(hist, name, xaxis, yaxis, levelBit);
}

//------------------------------------------------------
void ZeeDHistManager::AddTH1D(const TString& name, TString filename,
                              TString xaxis, TString yaxis, ZeeDLevelBit levelBit)
{
    // Add a TH1D histogram to the current array
    // Sets the number of bins, the range  by BinGrid File, and the axis labels

    if(!CheckLevel(levelBit)) return;

    if(bGridified) {
        Warning("ZeeDHistManager::AddTH1D","No support for variable bin width in gridified HistManager, %s ignored.", name.Data());
        return;
    }

    // Get edge information
    TObjArray* BinEdges = (*fAnaOptions)->getBinGrid(filename.Data())->GetBinEdges();

    if ( BinEdges->GetEntriesFast() != 1 ) {
        Info("ZeeDHistManager::AddTH1D", "Wrong array dimension for histogram");
    }

    ZeeDArrayD* array_x = static_cast<ZeeDArrayD*>(BinEdges->At(0));

    Int_t nbins          = array_x -> GetMaxElement();
    const Double_t* bins = array_x -> GetArray();

    TH1D* hist = new TH1D(name, name, nbins, bins);
    this->Register(hist, name, xaxis, yaxis, levelBit);
}

//------------------------------------------------------
void ZeeDHistManager::AddTH2D(const TString& name, Int_t nbinsx,
                              Double_t lowx, Double_t highx, Int_t nbinsy, Double_t lowy,
                              Double_t highy, TString xaxis, TString yaxis, ZeeDLevelBit levelBit)
{
    // Add a TH2D histogram to the current array
    // Sets the number of bins, the range and the axis labels

    if(!CheckLevel(levelBit)) return;

    if(bGridified) {
        Warning("ZeeDHistManager::AddTH1D","No support for 2D histograms in gridified HistManager, %s ignored.", name.Data());
        return;
    }

    TH2D* hist = new TH2D(name, name, nbinsx, lowx, highx, nbinsy, lowy, highy);
    this->Register(hist, name, xaxis, yaxis, levelBit);
}

//------------------------------------------------------
void ZeeDHistManager::AddTH2D(const TString& name, Int_t nbinsx,
                              const Double_t* xbins, Int_t nbinsy, Double_t lowy,
                              Double_t highy, TString xaxis, TString yaxis, ZeeDLevelBit levelBit)
{
    // Add a TH2D histogram to the current array
    // Sets the number of bins, the range and the axis labels

    if(!CheckLevel(levelBit)) return;

    if(bGridified) {
        Warning("ZeeDHistManager::AddTH1D","No support for 2D histograms in gridified HistManager, %s ignored.", name.Data());
        return;
    }

    TH2D* hist = new TH2D(name, name, nbinsx, xbins, nbinsy, lowy, highy);
    this->Register(hist, name, xaxis, yaxis, levelBit);
}

//------------------------------------------------------
void ZeeDHistManager::AddTH2D(const TString& name, Int_t nbinsx,
                              Double_t lowx, Double_t highx, Int_t nbinsy,
                              const Double_t* ybins, TString xaxis, TString yaxis, ZeeDLevelBit levelBit)
{
    // Add a TH2D histogram to the current array
    // Sets the number of bins, the range and the axis labels

    if(!CheckLevel(levelBit)) return;

    if(bGridified) {
        Warning("ZeeDHistManager::AddTH1D","No support for 2D histograms in gridified HistManager, %s ignored.", name.Data());
        return;
    }

    TH2D* hist = new TH2D(name, name, nbinsx, lowx, highx, nbinsy, ybins);
    this->Register(hist, name, xaxis, yaxis, levelBit);
}

//------------------------------------------------------
void ZeeDHistManager::AddTH2D(const TString& name, Int_t nbinsx,
                              const Double_t* xbins, Int_t nbinsy, const Double_t* ybins,
                              TString xaxis, TString yaxis, ZeeDLevelBit levelBit)
{
    // Add a TH2D histogram to the current array
    // Sets the number of bins, the range and the axis labels

    if(!CheckLevel(levelBit)) return;

    if(bGridified) {
        Warning("ZeeDHistManager::AddTH1D","No support for 2D histograms in gridified HistManager, %s ignored.", name.Data());
        return;
    }

    TH2D* hist = new TH2D(name, name, nbinsx, xbins, nbinsy, ybins);
    this->Register(hist, name, xaxis, yaxis, levelBit);
}

//------------------------------------------------------
void ZeeDHistManager::AddTH2D(const TString& name, TString filename,
                              TString xaxis, TString yaxis, ZeeDLevelBit levelBit )
{
    // Add a TH2D histogram to the current array
    // Sets the number of bins, the range  by BinGrid File, and the axis labels

    if(!CheckLevel(levelBit)) return;

    if(bGridified) {
        Warning("ZeeDHistManager::AddTH1D","No support for 2D histograms in gridified HistManager, %s ignored.", name.Data());
        return;
    }

    // Get edge information
    TObjArray* BinEdges = (*fAnaOptions)->getBinGrid(filename.Data())->GetBinEdges();

    if ( BinEdges->GetEntriesFast() != 2 ) {
        Info("ZeeDHistManager::AddTH1D", "Wrong array dimension for histogram");
    }

    ZeeDArrayD* array_x = static_cast<ZeeDArrayD*>(BinEdges->At(0));
    ZeeDArrayD* array_y = static_cast<ZeeDArrayD*>(BinEdges->At(1));

    Int_t nbinsx = array_x -> GetMaxElement();
    Int_t nbinsy = array_y -> GetMaxElement();

    const Double_t* xbins = array_x -> GetArray();
    const Double_t* ybins = array_y -> GetArray();

    TH2D* hist = new TH2D(name, name, nbinsx, xbins, nbinsy, ybins);
    this->Register(hist, name, xaxis, yaxis, levelBit);
}

void ZeeDHistManager::AddTH2D(const TString& name,
                              const ZeeDBinGrid& binGrid_xaxis,
                              const ZeeDBinGrid& binGrid_yaxis,
                              TString xaxis, TString yaxis,
                              ZeeDLevelBit levelBit)
{

    if(!CheckLevel(levelBit)) return;

    if(bGridified) {
        Warning("ZeeDHistManager::AddTH1D","No support for 2D histograms in gridified HistManager, %s ignored.", name.Data());
        return;
    }

    // xaxis
    const TObjArray* BinEdges_xaxis = binGrid_xaxis.GetBinEdges();

     if ( BinEdges_xaxis->GetEntriesFast() != 1 ) {
        Error("ZeeDHistManager::AddTH2D",
              "BinGrid for xaxis has dimension %i but dimension 1 is required",
              BinEdges_xaxis->GetEntriesFast());
        gSystem->Exit(EXIT_FAILURE);
    }

    const ZeeDArrayD* array_x = static_cast<ZeeDArrayD*>(BinEdges_xaxis->At(0));
    const Int_t nbinsx = array_x -> GetMaxElement();
    const Double_t* xbins = array_x -> GetArray();

    // yaxis
    const TObjArray* BinEdges_yaxis = binGrid_yaxis.GetBinEdges();

    if ( BinEdges_yaxis->GetEntriesFast() != 1 ) {
        Error("ZeeDHistManager::AddTH2D",
              "BinGrid for yaxis has dimension %i but dimension 1 is required",
              BinEdges_yaxis->GetEntriesFast());
        gSystem->Exit(EXIT_FAILURE);
    }

    const ZeeDArrayD* array_y = static_cast<ZeeDArrayD*>(BinEdges_yaxis->At(0));
    const Int_t nbinsy = array_y -> GetMaxElement();
    const Double_t* ybins = array_y -> GetArray();

    TH2D* hist = new TH2D(name, name, nbinsx, xbins, nbinsy, ybins);
    this->Register(hist, name, xaxis, yaxis, levelBit);
}

//------------------------------------------------------
void ZeeDHistManager::AddTH3D(const TString& name, TString filename,
                              TString xaxis, TString yaxis, TString zaxis, ZeeDLevelBit levelBit)
{

    if(!CheckLevel(levelBit)) return;

    if(bGridified) {
        Warning("ZeeDHistManager::AddTH1D","No support for 3D histograms in gridified HistManager, %s ignored.", name.Data());
        return;
    }

    DEBUG_MES_START;

    // Add a TH3D histogram to the current array
    // Sets the number of bins, the range  by BinGrid File, and the axis labels

    // Get edge information
    TObjArray* BinEdges = (*fAnaOptions)->getBinGrid(filename.Data())->GetBinEdges();

    if ( BinEdges->GetEntriesFast() != 3 ) {
        Info("ZeeDHistManager::AddTH3D", "Wrong array dimension for histogram");
    }

    ZeeDArrayD* array_x = static_cast<ZeeDArrayD*>(BinEdges->At(0));
    ZeeDArrayD* array_y = static_cast<ZeeDArrayD*>(BinEdges->At(1));
    ZeeDArrayD* array_z = static_cast<ZeeDArrayD*>(BinEdges->At(2));

    Int_t nbinsx = array_x -> GetMaxElement();
    Int_t nbinsy = array_y -> GetMaxElement();
    Int_t nbinsz = array_z -> GetMaxElement();

    const Double_t* xbins = array_x -> GetArray();
    const Double_t* ybins = array_y -> GetArray();
    const Double_t* zbins = array_z -> GetArray();

    TH3D* hist = new TH3D(name, name, nbinsx, xbins, nbinsy, ybins, nbinsz, zbins);
    this->Register(hist, name, xaxis, yaxis, zaxis, levelBit );

    DEBUG_MES_END;

}

//------------------------------------------------------

void ZeeDHistManager::AddTH3DVector(TString name, TString filename, TString filenameExtraBinning,
                                    TString xaxis, TString yaxis, TString zaxis, TString extraAxis, ZeeDLevelBit levelBit)
{

    if(!CheckLevel(levelBit)) return;

    if(bGridified) {
        Warning("ZeeDHistManager::AddTH1D","No support for 3D histograms in gridified HistManager, %s ignored.", name.Data());
        return;
    }

    DEBUG_MES_START;

    // Add a TH3D histogram to the current array
    // Sets the number of bins, the range  by BinGrid File, and the axis labels

    // Get edge information
    TObjArray* BinEdges = (*fAnaOptions)->getBinGrid(filenameExtraBinning.Data())->GetBinEdges();

    if ( BinEdges->GetEntriesFast() != 1 ) {
        Info("ZeeDHistManager::AddTH3D", "Wrong array dimension for histogram");
    }

    ZeeDArrayD* array_extra = static_cast<ZeeDArrayD*>(BinEdges->At(0));
    Int_t nbinsextra = array_extra -> GetMaxElement();
    const Double_t* vectorextra = array_extra -> GetArray();

    AddTH1D(name, nbinsextra, vectorextra, extraAxis, "", levelBit );

    for (int i=0; i<nbinsextra; i++ )
      {
        TString histName = "";
        histName.Form("%s_bin_%d",name.Data(), i+1);
        AddTH3D( histName, filename, xaxis, yaxis, zaxis, levelBit);
      }


    DEBUG_MES_END;

}


//------------------------------------------------------

void ZeeDHistManager::AddTProfile3D(const TString& name, TString filename,
                                    TString xaxis, TString yaxis, TString zaxis, ZeeDLevelBit levelBit)
{

    DEBUG_MES_START;

    // Add a Profile3D histogram to the current array
    // Sets the number of bins, the range  by BinGrid File, and the axis labels

    // Get edge information
    TObjArray* BinEdges = (*fAnaOptions)->getBinGrid(filename.Data())->GetBinEdges();

    if ( BinEdges->GetEntriesFast() != 3 ) {
        Info("ZeeDHistManager::AddProfile3D", "Wrong array dimension for histogram");
    }

    ZeeDArrayD* array_x = static_cast<ZeeDArrayD*>(BinEdges->At(0));
    ZeeDArrayD* array_y = static_cast<ZeeDArrayD*>(BinEdges->At(1));
    ZeeDArrayD* array_z = static_cast<ZeeDArrayD*>(BinEdges->At(2));

    Int_t nbinsx = array_x -> GetMaxElement();
    Int_t nbinsy = array_y -> GetMaxElement();
    Int_t nbinsz = array_z -> GetMaxElement();

    const Double_t* xbins = array_x -> GetArray();
    const Double_t* ybins = array_y -> GetArray();
    const Double_t* zbins = array_z -> GetArray();


    TProfile3D* prof = new TProfile3D(name, name, nbinsx, xbins, nbinsy, ybins, nbinsz, zbins);
    this->Register(prof, name, xaxis, yaxis, zaxis, levelBit );

    DEBUG_MES_END;

}


//------------------------------------------------------
void ZeeDHistManager::AddTProfile(const TString& name, Int_t nbins,
                                  Double_t low, Double_t high, TString xaxis,
                                  TString yaxis, Option_t* error, ZeeDLevelBit levelBit)
{
    // Add a TProfile histogram to the current array
    // Sets the number of bins, the range and the axis labels

    if(!CheckLevel(levelBit)) return;

    if(bGridified) {
        Warning("ZeeDHistManager::AddTH1D","No support for TProfile histograms in gridified HistManager, %s ignored.", name.Data());
        return;
    }

    TProfile* hist = new TProfile(name, name, nbins, low, high, error);
    this->Register(hist, name, xaxis, yaxis, levelBit);
}

//------------------------------------------------------
void ZeeDHistManager::AddTProfile(const TString& name, Int_t nbins,
                                  Double_t xlow, Double_t xhigh, Double_t ylow, Double_t yhigh,
                                  TString xaxis, TString yaxis, Option_t* error, ZeeDLevelBit levelBit)
{
    // Add a TProfile histogram to the current array
    // Sets the number of bins, the range and the axis labels

    if(!CheckLevel(levelBit)) return;

    if(bGridified) {
        Warning("ZeeDHistManager::AddTH1D","No support for TProfile histograms in gridified HistManager, %s ignored.", name.Data());
        return;
    }

    TProfile* hist = new TProfile(name, name, nbins, xlow, xhigh, ylow, yhigh, error);
    this->Register(hist, name, xaxis, yaxis, levelBit);
}

//------------------------------------------------------
void ZeeDHistManager::AddTProfile(const TString& name, Int_t nbins,
                                  const Double_t* xbins, Double_t ylow, Double_t yhigh,
                                  TString xaxis, TString yaxis, Option_t* error, ZeeDLevelBit levelBit)
{
    // Add a TProfile histogram to the current array
    // Sets the number of bins, the range and the axis labels

    if(!CheckLevel(levelBit)) return;

    if(bGridified) {
        Warning("ZeeDHistManager::AddTH1D","No support for TProfile histograms in gridified HistManager, %s ignored.", name.Data());
        return;
    }

    TProfile* hist = new TProfile(name, name, nbins, xbins, ylow, yhigh, error);
    this->Register(hist, name, xaxis, yaxis,  levelBit);
}

//------------------------------------------------------
void ZeeDHistManager::AddGrid(const TString& name, TString filename,
                              TString xaxis, TString yaxis)
{
    // Add a Grid to the grid array. Sets name and the axis labels

    if(bGridified) {
        Warning("ZeeDHistManager::AddTH1D","No support for Grid in gridified HistManager, %s ignored.", name.Data());
        return;
    }

    ZeeDBinGrid* grid = new ZeeDBinGrid(name);
    grid->ReadFromFile(filename);

    grid->SetTitleX(xaxis);
    grid->SetTitleY(yaxis);

    fGridArray->Add(grid);
}

void ZeeDHistManager::AddHistoFromGrid(const TString& name, const TString& filename, ZeeDLevelBit levelBit) {

    if(!CheckLevel(levelBit)) return;

    if(bGridified) {
        Warning("ZeeDHistManager::AddGridAsHisto","No support for Grid in gridified HistManager, %s ignored.", name.Data());
        return;
    }

    // Get edge information
    TObjArray* BinEdges = (*fAnaOptions)->getBinGrid(filename.Data())->GetBinEdges();

    UInt_t dim = BinEdges->GetEntriesFast();

    switch (dim) {

    case 1: {

        ZeeDArrayD* array_x = static_cast<ZeeDArrayD*>(BinEdges->At(0));

        Int_t nbins          = array_x -> GetMaxElement();
        const Double_t* bins = array_x -> GetArray();

        TH1D* hist = new TH1D(name, name, nbins, bins);
        this->Register(hist, name, "", "", levelBit);

        break;

    }

    case 2: {

        ZeeDArrayD* array_x = static_cast<ZeeDArrayD*>(BinEdges->At(0));
        ZeeDArrayD* array_y = static_cast<ZeeDArrayD*>(BinEdges->At(1));

        Int_t nbins_x          = array_x -> GetMaxElement();
        const Double_t* bins_x = array_x -> GetArray();

        Int_t nbins_y          = array_y -> GetMaxElement();
        const Double_t* bins_y = array_y -> GetArray();

        TH2D* hist = new TH2D(name, name, nbins_x, bins_x, nbins_y, bins_y);
        this->Register(hist, name, "", "", levelBit);

        break;

    }

    default:

        Error("ZeeDHistManager::AddGridAsHisto","No support for Grid of dimension %i", dim);
        gSystem->Exit(EXIT_FAILURE);

    }

}

void ZeeDHistManager::AddResponseHistoFromGrid(const TString& name, const TString& filename, Bool_t unfolding_in_y, ZeeDLevelBit levelBit) {

    if(!CheckLevel(levelBit)) return;

    if(bGridified) {
        Warning("ZeeDHistManager::AddGridAsHisto","No support for Grid in gridified HistManager, %s ignored.", name.Data());
        return;
    }

    // Get edge information
    TObjArray* BinEdges = (*fAnaOptions)->getBinGrid(filename.Data())->GetBinEdges();

    UInt_t dim = BinEdges->GetEntriesFast();

    switch (dim) {

    case 1: {

        ZeeDArrayD* array_x = static_cast<ZeeDArrayD*>(BinEdges->At(0));

        Int_t nbins          = array_x -> GetMaxElement();
        const Double_t* bins = array_x -> GetArray();

        TH2D* hist = new TH2D(name, name, nbins, bins, nbins, bins);
        this->Register(hist, name, "", "", levelBit);

        break;

    }

    case 2: {

        ZeeDArrayD* array_x = static_cast<ZeeDArrayD*>(BinEdges->At(0));
        ZeeDArrayD* array_y = static_cast<ZeeDArrayD*>(BinEdges->At(1));

        Int_t nbins_x          = array_x -> GetMaxElement();
        const Double_t* bins_x = array_x -> GetArray();

        Int_t nbins_y          = array_y -> GetMaxElement();
        const Double_t* bins_y = array_y -> GetArray();

        TH3D* hist = NULL;

        if(unfolding_in_y) {
            hist = new TH3D(name, name, nbins_y, bins_y, nbins_y, bins_y, nbins_x, bins_x);
        } else {
            hist = new TH3D(name, name, nbins_x, bins_x, nbins_x, bins_x, nbins_y, bins_y);
        }

        this->Register(hist, name, "", "", "", levelBit);

        break;

    }

    default:

        Error("ZeeDHistManager::AddGridAsHisto","No support for Grid of dimension %i", dim);
        gSystem->Exit(EXIT_FAILURE);

    }

}

//------------------------------------------------------
void ZeeDHistManager::AddEff1D(const TString& name,
    			       Int_t nbins, Double_t xmin, Double_t xmax, TString effname)
{
  // Add pair of 1D efficiency histograms
  TString effname_top = TString("Eff1D_") + name + TString("_") + effname + TString("_top");
  TString effname_bot = TString("Eff1D_") + name + TString("_") + effname + TString("_bot");

    if(bGridified) {
        Warning("ZeeDHistManager::AddTH1D","No support for Eff1D in gridified HistManager, %s ignored.", name.Data());
        return;
    }

  AddTH1D(effname_top, nbins, xmin, xmax, effname, "");
  AddTH1D(effname_bot, nbins, xmin, xmax, effname, "");

}

//------------------------------------------------------
void ZeeDHistManager::AddEff2D(const TString& name,
    			       Int_t nbinsx, Double_t xmin, Double_t xmax, TString effname1,
			       Int_t nbinsy, Double_t ymin, Double_t ymax, TString effname2)
{
  // Add pair of 2D efficiency histograms
  TString effname_top = TString("Eff2D_") + name + TString("_") + effname1 + TString("_") + effname2 + TString("_top");
  TString effname_bot = TString("Eff2D_") + name + TString("_") + effname1 + TString("_") + effname2 + TString("_bot");

    if(bGridified) {
        Warning("ZeeDHistManager::AddTH1D","No support for Eff2D in gridified HistManager, %s ignored.", name.Data());
        return;
    }

  AddTH2D(effname_top, nbinsx, xmin, xmax, nbinsy, ymin, ymax, effname1, effname2);
  AddTH2D(effname_bot, nbinsx, xmin, xmax, nbinsy, ymin, ymax, effname1, effname2);
}


//------------------------------------------------------
void ZeeDHistManager::DoBootstrap(const TString& name)
{
    // Enable bootstrap histograms

    // Create histos to keep bootstrap copies

    if ( (*fAnaOptions)->genBootstrapRnd() ) {


        // Check if already booked
        ZeeDHistOptions* histOpt = static_cast<ZeeDHistOptions*>( fOptArray->FindObject(name));

        // Check if histogram is present
        if (histOpt == NULL) {
            Warning("ZeeDHistManager::DoBootstrap", "Histogram "+name+" not found");
            return;
        }

        if (histOpt->GetBootstrap()) {
            //Already booked, return
            Info("ZeeDHistManager::DoBootstrap", "Histogram "+name+" already booked");
            return;
        }

        // New list
        TObjArray* bList = new TObjArray();

        // Original histogram
        TH1* hOrig = (TH1*)fHistArray->FindObject(name);
        TString name = hOrig->GetName();
        // Base name
        TString fullName = fHistDirName;
        fullName.Append(name);
        fullName.Append("_bs/");

        ServiceHandle<ITHistSvc>* histService = fSvcHelper.GetTHistSvc();

        for (Int_t i = 0; i < fRndVault->GetNRandom(); i++) {
            // Clone histograms, register them
            //
            TString nameClone = "b";
            nameClone += i;
            TH1* bHist = (TH1*)hOrig->Clone(nameClone);
            bList->Add(bHist);
            TString fname = fullName;
            fname.Append(nameClone);
            StatusCode sc = (*histService)->regHist(fname.Data(), bHist);
            if ( sc.isFailure() ) {
                Error("ZeeDHistManager::DoBootstrap",
                      "Unable to register histogram");
            }

        }

        histOpt->SetOptHist(bList);

        // Set flag
        static_cast<ZeeDHistOptions*>( fOptArray->FindObject(name))->SetBootstrap(kTRUE);
    } else {
        Info("ZeeDHistManager::DoBootstrap", "No Poisson Vault activated. Check PoissonVaultDimension flag");
    }

}

//------------------------------------------------------
void ZeeDHistManager::Register(TH1* hist, const TString& name,
                               TString xaxis, TString yaxis, ZeeDLevelBit levelBit)
{
    // Sets the range, axis labels and register histogram
    // in the athena hist service

    hist->GetXaxis()->SetTitleOffset(1.8);
    hist->GetYaxis()->SetTitleOffset(1.5);
    hist->SetXTitle(xaxis);
    hist->SetYTitle(yaxis);

    if ( hist->IsA() != TProfile::Class() ) {
        hist->Sumw2();
    }

    // Register options
    ZeeDHistOptions* hOptions = new ZeeDHistOptions(name);
    hOptions->SetLevelBit(levelBit);
    fOptArray->Add(hOptions);

    // Register histogramm
    if ( levelBit & fLevel ) {
        fHistArray->Add(hist);

        ServiceHandle < ITHistSvc > *histService = fSvcHelper.GetTHistSvc();

        TString fullName = fHistDirName;
        fullName.Append(name);
        StatusCode sc = (*histService)->regHist(fullName.Data(), hist);

        if (sc.isFailure()) {
            Error("ZeeDHistManager::Register TH1/TH2", "Unable to register histogram");
        }
    } else {
        hOptions->SetIsInLevel(kFALSE);
        //hist->SetDirectory(0);
        delete hist;

    }


}

//------------------------------------------------------
void ZeeDHistManager::Register(TH2* hist, const TString& name,
                               TString xaxis, TString yaxis, ZeeDLevelBit levelBit)
{

    this->Register( ((TH1*)hist), name, xaxis, yaxis, levelBit);

}

//------------------------------------------------------
void ZeeDHistManager::Register(TH3* hist, const TString& name,
                               TString xaxis, TString yaxis, TString zaxis, ZeeDLevelBit levelBit)
{
    // Sets the range, axis labels and register histogram
    // in the athena hist service

    hist->GetXaxis()->SetTitleOffset(1.8);
    hist->GetYaxis()->SetTitleOffset(1.5);
    hist->SetXTitle(xaxis);
    hist->SetYTitle(yaxis);
    hist->SetZTitle(zaxis);

    if ( hist->IsA() != TProfile::Class() ) {
        hist->Sumw2();
    }

    // Register options
    ZeeDHistOptions* hOptions = new ZeeDHistOptions(name);
    hOptions->SetLevelBit(levelBit);
    fOptArray->Add(hOptions);

    if( levelBit & fLevel ) {

        fHistArray->Add(hist);

        // Register histogramm

        ServiceHandle<ITHistSvc>* histService = fSvcHelper.GetTHistSvc();

        TString fullName = fHistDirName;
        fullName.Append(name);
        StatusCode sc = (*histService)->regHist(fullName.Data(), hist);

        if ( sc.isFailure() ) {
            Error("ZeeDHistManager::Register", "Unable to register histogram TH3");
        }

    } else {

        hOptions->SetIsInLevel(kFALSE);
        hist->SetDirectory(0);

    }

}

//------------------------------------------------------
ZeeDBinGrid* ZeeDHistManager::GetGrid(const char* name) const
{

    return ((ZeeDBinGrid*)fGridArray->FindObject(name));

}

//------------------------------------------------------
Int_t ZeeDHistManager::NoSuchHist(const char* name, TCollection* array) const
{
    // Error message for no such histogram called  "name"
    Warning("NoSuchHist", "There is no such histogram named %s.", name);

    Info("NoSuchHist", "Printing contents of the current array named %s.",
         array->GetName());

    array->ls();

    gSystem->Exit(EXIT_FAILURE);
    return 0;
}

//------------------------------------------------------
TH1* ZeeDHistManager::IndexOutOfBounds() const
{
    Warning("IndexOutOfBounds", "Index out of bounds %i filling hist array %s.",
            fHistArray->GetEntries(), fHistArray->GetName());
    Info("IndexOutOfBounds", "Printing contents of the current HistArray named %s.",
         fHistArray->GetName());

    fHistArray->ls();

    gSystem->Exit(EXIT_FAILURE);
    return NULL;
}

TProfile* ZeeDHistManager::GetTProfile(const char* name) const
{
    return ((TProfile*)fHistArray->FindObject(name));
}

TProfile3D* ZeeDHistManager::GetTProfile3D(const char* name) const
{
    return ((TProfile3D*)fHistArray->FindObject(name));
}

void ZeeDHistManager::Initialize()
{
    if (bIsInitialized) {
        return;
    }

    BookHistos();

    fHistArray->Rehash(fHistArray->GetEntries(), kFALSE);

    fOptArray->Rehash(fOptArray->GetEntries(), kFALSE);

    bIsInitialized = kTRUE;
}
