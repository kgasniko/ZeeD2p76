#include "ZeeDHist/ZeeDSimpleMeasurement.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

//------------------------------------------------------
ZeeDSimpleMeasurement::ZeeDSimpleMeasurement( TString name,
                   TString recBinGridName,
		   ZeeDCutBit* recMask,
                   ZeeDQuantity<ZeeDArrayD> (*cVarRec)(const ZeeDEvent*),
                   TString genBinGridName,
		   ZeeDCutBit* genMask,
                   ZeeDQuantity<ZeeDArrayD> (*cVarGen)(const ZeeDEvent*),
		   bool isMC) :
  ZeeDHistManager(name),
  fRecMask(recMask),
  fVarRec(cVarRec),
  fGenMask(genMask),
  fVarGen(cVarGen),
  bIsMC(isMC)
{

  // for Data:	process Rec only
  // for MC:	process Rec/Gen/Gen_Rec

  if (recBinGridName == "") {
    Error("ZeeDSimpleMeasurement::ZeeDSimpleMeasurement",
          "No Rec bin grid name (recBinGridName) gived, measurement name is \"%s\"", name.Data());
    gSystem->Exit(EXIT_FAILURE);
  }

  if (recMask == NULL) {
    Error("ZeeDSimpleMeasurement::ZeeDSimpleMeasurement",
          "No rec cuts (recMask) given, measurement name is \"%s\"", name.Data());
    gSystem->Exit(EXIT_FAILURE);
  }

  if (fVarRec == NULL) {
    Error("ZeeDSimpleMeasurement::ZeeDSimpleMeasurement",
          "No reconstructed variables (cVarRec) given, measurement name is \"%s\"", name.Data());
    gSystem->Exit(EXIT_FAILURE);
  }

  if (isMC) {
    if (genBinGridName == "") {
      Error("ZeeDSimpleMeasurement::ZeeDSimpleMeasurement",
	  "No Gen bin grid name (genBinGridName) gived, measurement name is \"%s\"", name.Data());
      gSystem->Exit(EXIT_FAILURE);
    }

    if (genMask == NULL) {
      Error("ZeeDSimpleMeasurement::ZeeDSimpleMeasurement",
	  "No gen cuts (genMask) given, measurement name is \"%s\"", name.Data());
      gSystem->Exit(EXIT_FAILURE);
    }

    if (fVarGen == NULL) {
      Error("ZeeDSimpleMeasurement::ZeeDSimpleMeasurement",
	  "No generated variables (cVarGen) given, measurement name is \"%s\"", name.Data());
      gSystem->Exit(EXIT_FAILURE);
    }
  }

  // create the measurement bin grid
  // create the normal bin grids
  ZeeDSvcHelper fSvcHelper("ZeeDSimpleMeasurement");
  ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions = fSvcHelper.GetAnaSvc();

  fRecBinGridAcc = (*fAnaOptions)->getSimpleBinGrid(recBinGridName.Data());
  if (bIsMC) {
    fGenBinGridAcc = (*fAnaOptions)->getSimpleBinGrid(genBinGridName.Data());
  }

}

//------------------------------------------------------
ZeeDSimpleMeasurement::~ZeeDSimpleMeasurement()
{
  // The Destructor
  if (fRecBinGridAcc) delete fRecBinGridAcc;
  if (fGenBinGridAcc) delete fGenBinGridAcc;

}

//------------------------------------------------------
void ZeeDSimpleMeasurement::BookHistos()
{
  // Book and fill the distributions according to
  // https://indico.cern.ch/getFile.py/access?contribId=9&sessionId=1&resId=0&materialId=slides&confId=210995

  if (bIsMC) {	// MC

    int nGenMeasBinsAcc = fGenBinGridAcc->GetNumOfCells();
    int nRecMeasBinsAcc = fRecBinGridAcc->GetNumOfCells();

    // superfluous booking?
    AddTH1D("effCorrNum",   nGenMeasBinsAcc, -0.5, nGenMeasBinsAcc-0.5, "bin", "", ZeeDHistLevel::ToyMC);
    AddTH1D("effCorrDenom", nGenMeasBinsAcc, -0.5, nGenMeasBinsAcc-0.5, "bin", "", ZeeDHistLevel::ToyMC);
    AddTH1D("fidCorrNum",   nRecMeasBinsAcc, -0.5, nRecMeasBinsAcc-0.5, "bin", "", ZeeDHistLevel::ToyMC);
    AddTH1D("fidCorrDenom", nRecMeasBinsAcc, -0.5, nRecMeasBinsAcc-0.5, "bin", "", ZeeDHistLevel::ToyMC);
    AddTH1D("purityNum",    nRecMeasBinsAcc, -0.5, nRecMeasBinsAcc-0.5, "bin", "", ZeeDHistLevel::ToyMC); // valid for fRecBinGridAcc == fGenBinGridAcc only
    AddTH1D("purityDenom",  nGenMeasBinsAcc, -0.5, nGenMeasBinsAcc-0.5, "bin", "", ZeeDHistLevel::ToyMC); // valid for fRecBinGridAcc == fGenBinGridAcc only

    // FIXME: keep eye on the order of gen/rec
    AddTH2D("ResponseMatrix", nRecMeasBinsAcc, -0.5, nRecMeasBinsAcc-0.5,
	nGenMeasBinsAcc, -0.5, nGenMeasBinsAcc-0.5, "rec", "gen", ZeeDHistLevel::ToyMC);

  } else {	// Data

    int nRecMeasBinsAcc = fRecBinGridAcc->GetNumOfCells();
    AddTH1D("MeasRec",	nRecMeasBinsAcc, -0.5, nRecMeasBinsAcc-0.5, "bin", "", ZeeDHistLevel::ToyMC);

  }

  // save the bin grids
  // FIXME: do it properly
  //if (bIsMC) fGenBinGridAcc->WriteIntoFile("EWUnfoldingBinningGen.txt");
  //fRecBinGridAcc->WriteIntoFile("EWUnfoldingBinningRec.txt");

}

//------------------------------------------------------
void ZeeDSimpleMeasurement::Fill(ZeeDCutBit eventBitMask)
{
  // Book and fill the distributions according to
  // https://indico.cern.ch/getFile.py/access?contribId=9&sessionId=1&resId=0&materialId=slides&confId=210995

  if (bIsMC) {	// MC

    bool passedGenCuts = ((eventBitMask & *fGenMask) == *fGenMask);
    bool passedRecCuts = ((eventBitMask & *fRecMask) == *fRecMask);

    const ZeeDEvent* fEvent = GetEvent(); // get event

    const double genWeight = fEvent->GetGenWeight();	// get gen weight
    const double recWeight = fEvent->GetWeight();	// get rec weight

    int cellIndexGen    = -1;
    int cellIndexRec    = -1;

    if (passedGenCuts) {
      cellIndexGen = fGenBinGridAcc->GetIndex(&(((*fVarGen)(fEvent)).Get())); // this way the code will stop on non-set variable as it should
    }

    if (passedRecCuts) {
      cellIndexRec = fRecBinGridAcc->GetIndex(&(((*fVarRec)(fEvent)).Get()));
    }

    if (cellIndexGen > -1 && cellIndexRec > -1) {
      // ROOT hist bins count from 1!
      FillTH1(cellIndexGen, recWeight, "effCorrNum");
      FillTH1(cellIndexRec, recWeight, "fidCorrNum");
      FillTH1(cellIndexGen, recWeight, "purityDenom"); // valid for fRecBinGridAcc == fGenBinGridAcc only
      FillTH2(cellIndexRec, cellIndexGen, recWeight, "ResponseMatrix");

      if (cellIndexRec == cellIndexGen) FillTH1(cellIndexRec, recWeight, "purityNum");
    }

    if (cellIndexGen > -1) FillTH1(cellIndexGen, genWeight, "effCorrDenom");
    if (cellIndexRec > -1) FillTH1(cellIndexRec, recWeight, "fidCorrDenom");
  
  } else {	// Data

    bool passedRecCuts = ((eventBitMask & *fRecMask) == *fRecMask);

    if (passedRecCuts) {
      const ZeeDEvent* fEvent = GetEvent(); // get event
      int cellIndexRec  = fRecBinGridAcc->GetIndex(&(((*fVarRec)(fEvent)).Get()));

      if (cellIndexRec > -1) {
	const double recWeight = fEvent->GetWeight(); // get rec weight
	FillTH1(cellIndexRec, recWeight, "MeasRec");
      }
    }

  }

}
