#pragma once

////////////////////////////////////////////////////////
// Name    : ZeeDSimpleMeasurement.h
////////////////////////////////////////////////////////

// ROOT includes
#include "TMath.h"
#include "TString.h"

// ZeeD includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDHist/ZeeDHistManager.h"
//#include "ZeeDArrays/ZeeDBinGrid.h"
#include "ZeeDArrays/ZeeDSimpleBinGrid.h"
#include "ZeeDArrays/ZeeDArrayD.h"
#include "ZeeDEvent/ZeeDEvent.h"


/** Basic class for managing of the cross section measurement
    (this time really basic)
  @author SSh
  @date 2013-12-04 
 */

class ZeeDSimpleMeasurement: public ZeeDHistManager {

  public:

    /**
       @param name name of the HistManager
       @param binGridName filename of the binning in which the cross section is determined
       @param cVarRec function ptr returning array of reconstructed quantities, has to match the binGridName dimension
       @param cVarGen function ptr returning array of generated quantities, has to match the binGridName dimension
     */
    ZeeDSimpleMeasurement(TString name,
                   TString recBinGridName,
		   ZeeDCutBit* recMask,
                   ZeeDQuantity<ZeeDArrayD> (*cVarRec)(const ZeeDEvent*),
                   TString genBinGridName = "",
		   ZeeDCutBit* genMask = NULL,
                   ZeeDQuantity<ZeeDArrayD> (*cVarGen)(const ZeeDEvent*) = NULL,
		   bool isMC = false);

    virtual ~ZeeDSimpleMeasurement();

    virtual void BookHistos();

    virtual void Fill() {
      Error("ZeeDSimpleMeasurement", "Filler is called without the event bit mask. Do nothing and exit.");
      gSystem->Exit(EXIT_FAILURE);
    }

    void Fill(ZeeDCutBit eventBitMask);

  private:

    // cut bit masks, bin grids and 
    // functions to get gen/rec "coordinates" of the event 
    // for gen and rec
   
    ZeeDCutBit* fRecMask;
    ZeeDQuantity<ZeeDArrayD> (*fVarRec)(const ZeeDEvent*);
    ZeeDSimpleBinGrid* fRecBinGridAcc;

    ZeeDCutBit* fGenMask;
    ZeeDQuantity<ZeeDArrayD> (*fVarGen)(const ZeeDEvent*);
    ZeeDSimpleBinGrid* fGenBinGridAcc;

    // is MC or Data?
    bool bIsMC;

};
