#ifndef __ZEEDOBSERVABLE
#define __ZEEDOBSERVABLE

////////////////////////////////////////////////////////
// Name    : ZeeDObservable.h
////////////////////////////////////////////////////////

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDHist/ZeeDHistManager.h"
#include "ZeeDArrays/ZeeDBinGrid.h"
#include "ZeeDArrays/ZeeDArrayD.h"
#include "ZeeDEvent/ZeeDEvent.h"

#include "TString.h"

/** Basic class for managing of the cross section measurement
  @author SSh, SG, SS
  @date 2012/03/12
 */
class ZeeDObservable : public ZeeDHistManager {

  public:

    /**
       @param name name of the HistManager
       @param binGridName filename of the (multidimensional) BinGrid in which the cross section is determined
       @param binGridBkgName filename of the (multidimensional) BinGrid for background estimation
       @param cVarREC function ptr returning array of reconstructed quantities, has to match the binGridName dimension
       @param cVarGEN function ptr returning array of generated quantities, has to match the binGridName dimension
       @param cVarBkg function ptr returning array of quantities for background estimation, has to match the binGridBkgName dimension
       @param bRec if true code runs that requires reconstructed information to be available
       @param bGen if true code runs that requires generated information to be available
       @param bGenRec if true code runs that requires reconstructed _and_ generated information to be available
     */
    ZeeDObservable(TString name,
                   TString binGridName,
                   TString binGridBkgName,
                   ZeeDQuantity<ZeeDArrayD> (*cVarREC)(const ZeeDEvent*),
                   ZeeDQuantity<ZeeDArrayD> (*cVarGEN)(const ZeeDEvent*),
                   ZeeDQuantity<ZeeDArrayD> (*cVarBkg)(const ZeeDEvent*),
                   Bool_t bRec=kTRUE,
                   Bool_t bGen=kFALSE,
                   Bool_t bGenRec=kFALSE);

    virtual ~ZeeDObservable();

    virtual void BookHistos();

    using ZeeDHistManager::Fill;
    virtual void Fill();

  private:

    void BookRec();
    void BookGen();
    void BookGenRec();

    void BookBkg();

    void FillRec();
    void FillGen();
    void FillGenRec();

    void FillBkg();

    Bool_t bIsMC;

    ZeeDQuantity<ZeeDArrayD> (*fVarREC)(const ZeeDEvent*);
    ZeeDQuantity<ZeeDArrayD> (*fVarGEN)(const ZeeDEvent*);
    ZeeDQuantity<ZeeDArrayD> (*fVarBkg)(const ZeeDEvent*);

    /** current (set at each event) array of reconstructed quantities */
    ZeeDQuantity<ZeeDArrayD> fArrayRecQuantity;
    ZeeDArrayD fArrayRec;

    /** current (set at each event) array of generated quantities */
    ZeeDQuantity<ZeeDArrayD> fArrayGenQuantity;
    ZeeDArrayD fArrayGen;

    /** current (set at each event) array of background quantities */
    ZeeDQuantity<ZeeDArrayD> fArrayBkgQuantity;
    ZeeDArrayD fArrayBkg;

    Int_t fCellREC;
    Int_t fCellGEN;
    Int_t fCellBkg;

    /** ptr to BinGrid constructed from binGridName */
    ZeeDBinGrid* fBinGrid;

    /** ptr to BinGrid constructed from binGridBkgName */
    ZeeDBinGrid* fBinGridBkg;

    /** number of bins in the binGridName BinGrid*/
    Int_t nMeasBins;

    /** number of bins in the binGridBkgName BinGrid*/
    Int_t nBkgBins;

    const ZeeDEvent* fEvent;

    Double_t fEventWeight;
    Double_t fGenWeight;

    const Bool_t bRec;
    const Bool_t bGen;
    const Bool_t bGenRec;

    Bool_t bBkg;

};

#endif
