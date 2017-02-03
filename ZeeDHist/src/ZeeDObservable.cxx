#include "ZeeDHist/ZeeDObservable.h"
#include "ZeeDMisc/ZeeDMisc.h"

#include <TMath.h>

//------------------------------------------------------
ZeeDObservable::ZeeDObservable(	TString name,
                                TString binGridName,
                                TString binGridBkgName,
                                ZeeDQuantity<ZeeDArrayD> (*cVarREC)(const ZeeDEvent*),
                                ZeeDQuantity<ZeeDArrayD> (*cVarGEN)(const ZeeDEvent*),
                                ZeeDQuantity<ZeeDArrayD> (*cVarBkg)(const ZeeDEvent*),
                                Bool_t rec,
                                Bool_t gen,
                                Bool_t genrec) : ZeeDHistManager(name),
                                                 bIsMC(kFALSE),
                                                 fVarREC(cVarREC),
                                                 fVarGEN(cVarGEN),
                                                 fVarBkg(cVarBkg),
                                                 fBinGrid(NULL),
                                                 fBinGridBkg(NULL),
                                                 fEvent(NULL),
                                                 fEventWeight(0),
                                                 fGenWeight(0),
                                                 bRec(rec),
                                                 bGen(gen),
                                                 bGenRec(genrec),
                                                 bBkg(kFALSE)
{
  DEBUG_MES_START;

  if( (bRec || bGenRec) && (!cVarREC || binGridName == "")) {

    Error("ZeeDObservable::ZeeDObservable",
          "Missing fVarREC or binGridName: fVarRec: (check it), binGridName %s",
          binGridName.Data());
    gSystem->Exit(EXIT_FAILURE);

  }

  if( (bGen || bGenRec) && (fVarGEN == NULL || binGridName == "")) {

    Error("ZeeDObservable::ZeeDObservable",
          "Missing fVarGEN or binGridName: fVarGEN: (check it), binGridName %s",
          binGridName.Data());
    gSystem->Exit(EXIT_FAILURE);

  }

  // create the measurement bin grid
  fBinGrid  = (*fAnaOptions)->getBinGrid(binGridName.Data());
  nMeasBins = fBinGrid->GetNumOfCells();

  // if binGridBkgName or corresponding function is given check for the other
  if( (binGridBkgName != "" && fVarBkg == NULL) ||
      (fVarBkg && binGridBkgName == "")
      ) {

    Error("ZeeDObservable::ZeeDObservable",
          "Missing fVarBkg or binGridBkgName: fVarBkg: (check it), binGridBkgName %s",
          binGridBkgName.Data());
    gSystem->Exit(EXIT_FAILURE);

  } else if (binGridBkgName != "" && fVarBkg) { // create bkg bin grid
                                                // only if file and
                                                // function are given

    bBkg = kTRUE;

    fBinGridBkg = (*fAnaOptions)->getBinGrid(binGridBkgName.Data());
    nBkgBins	= fBinGridBkg->GetNumOfCells();

  }

  // Do we run on data or MC?
  bIsMC = (*fAnaOptions)->IsMC();

  DEBUG_MES_END;
}

//------------------------------------------------------
ZeeDObservable::~ZeeDObservable()
{
  // The Destructor
  DEBUG_MES_START;
  DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDObservable::BookHistos()
{

  DEBUG_MES_START;

  if(bRec) {
    this->BookRec();
  }

  if(bBkg) {
    this->BookBkg();
  }

  if(bGen) {
    this->BookGen();
  }

  if(bGenRec) {
    this->BookGenRec();
  }

  // save the bin grids as histograms and text files
  Int_t nEdges = fBinGrid->GetBinEdges()->GetEntriesFast();
  for (Int_t dim = 0; dim < nEdges; ++dim) {
    ZeeDArrayD* edges = static_cast <ZeeDArrayD*> (fBinGrid->GetBinEdges()->At(dim));
    TString name("BinGridVar");
    name += dim;
    AddTH1D(name, edges->GetEntriesFast()-1, edges->GetArray(), "var", "", ZeeDHistLevel::ToyMC);
  }
  fBinGrid->WriteIntoFile("TheBinning.txt");

  DEBUG_MES_END;

}

//------------------------------------------------------
void ZeeDObservable::BookRec()
{

  DEBUG_MES_START;

  AddTH1D("MeasREC",	nMeasBins+1, -1.5, nMeasBins-0.5, "bin", "", ZeeDHistLevel::ToyMC);

  // do bootstrap if needed
  if ( (*fAnaOptions)->genBootstrapRnd() ) {
    DoBootstrap("MeasREC");
  }

  DEBUG_MES_END;

}

//------------------------------------------------------
void ZeeDObservable::BookBkg()
{

  DEBUG_MES_START;

  AddTH2D("MeasVsBkg",	nMeasBins+1, -1.5, nMeasBins-0.5, nBkgBins+1,	-1.5, nBkgBins-0.5,  "meas","bkg", ZeeDHistLevel::ToyMC);

  fBinGridBkg->WriteIntoFile("TheBinningBkg.txt");
  Int_t nEdgesBkg = fBinGridBkg->GetBinEdges()->GetEntriesFast();
  for (Int_t dim = 0; dim < nEdgesBkg; ++dim) {
    ZeeDArrayD* edges = static_cast <ZeeDArrayD*> (fBinGridBkg->GetBinEdges()->At(dim));
    TString name("BinGridBkgVar");
    name += dim;
    AddTH1D(name, edges->GetEntriesFast()-1, edges->GetArray(), "var", "", ZeeDHistLevel::ToyMC);
  }

}

//------------------------------------------------------
void ZeeDObservable::BookGen()
{

  DEBUG_MES_START;

  AddTH1D("MeasGEN",	nMeasBins+1, -1.5, nMeasBins-0.5, "bin", "", ZeeDHistLevel::ToyMC);

  // do bootstrap if needed
  if ( (*fAnaOptions)->genBootstrapRnd() ) {
    DoBootstrap("MeasGEN");
  }

  DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDObservable::BookGenRec()
{
  DEBUG_MES_START;

  AddTH1D("MeasGENREC",	nMeasBins+1, -1.5, nMeasBins-0.5, "bin", "", ZeeDHistLevel::ToyMC);
  AddTH2D("RECvsGEN",	nMeasBins+1, -1.5, nMeasBins-0.5, nMeasBins+1,	-1.5, nMeasBins-0.5, "rec", "gen", ZeeDHistLevel::ToyMC);
  AddTH1D("RECandGEN",	nMeasBins+1, -1.5, nMeasBins-0.5, "rec", "", ZeeDHistLevel::ToyMC);

  DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDObservable::Fill()
{
  DEBUG_MES_START;

  // Get event
  fEvent       = GetEvent();

  CHECK_NULL_PTR(fEvent);

  // Get weights
  fEventWeight = fEvent->GetWeight();
  fGenWeight   = bIsMC ? fEvent->GetGenWeight() : 0;

  if(bBkg && fBinGridBkg) {

    fArrayBkgQuantity = (*fVarBkg)(fEvent);

    if (fArrayBkgQuantity.IsSet()) {
        fArrayBkg = fArrayBkgQuantity.Get();
        fCellBkg  = fBinGridBkg->GetIndex(&fArrayBkg);
    }

  }

  if(bRec || bGenRec) {

    fArrayRecQuantity = (*fVarREC)(fEvent);

    if (fArrayRecQuantity.IsSet()) {
        fArrayRec = fArrayRecQuantity.Get();
        fCellREC  = fBinGrid->GetIndex(&fArrayRec);
    }

  }

  if(bGen || bGenRec) {

    fArrayGenQuantity = (*fVarGEN)(fEvent);

    if (fArrayGenQuantity.IsSet()) {
        fArrayGen = fArrayGenQuantity.Get();
        fCellGEN  = fBinGrid->GetIndex(&fArrayGen);
    }

  }

  if(bRec && fArrayRecQuantity.IsSet()) {

    this->FillRec();

  }

  if(bBkg && fArrayBkgQuantity.IsSet()) {

    this->FillBkg();

  }

  if(bGen && fArrayGenQuantity.IsSet()) {

    this->FillGen();

  }

  if(bGenRec && fArrayGenQuantity.IsSet() && fArrayRecQuantity.IsSet()) {

    this->FillGenRec();

  }

  DEBUG_MES_END;

}

void ZeeDObservable::FillRec()
{

  DEBUG_MES_START;


  // Fill the measurement
  FillTH1(fCellREC, fEventWeight, "MeasREC");

  DEBUG_MES_END;

}

void ZeeDObservable::FillBkg()
{

  DEBUG_MES_START;


  // Fill the background info
  FillTH2(fCellREC, fCellBkg, fEventWeight, "MeasVsBkg");

  DEBUG_MES_END;

}

void ZeeDObservable::FillGen()
{

  DEBUG_MES_START;


  FillTH1(fCellGEN, fGenWeight, "MeasGEN");

  DEBUG_MES_END;

}

void ZeeDObservable::FillGenRec()
{

  DEBUG_MES_START;


  FillTH1(fCellGEN, fEventWeight, "MeasGENREC");

  FillTH2(fCellREC, fCellGEN, fEventWeight, "RECvsGEN");

  if(fCellREC == fCellGEN) {

    FillTH1(fCellREC, fEventWeight, "RECandGEN");

  }

  DEBUG_MES_END;

}

