#ifndef ZEEDANALYSISCUTHISTMANAGERZBaseline
#define ZEEDANALYSISCUTHISTMANAGERZBaseline

////////////////////////////////////////////////////////
// Name    : ZeeDAnalysisCutHistManagerZBaseline.h
///////////////////////////////////////////////////////

#include "ZeeDCuts/ZeeDCutHistManager.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "ZeeDTools/ZeeDCutBit.h"

#include "TString.h"

/** Cut/Histogram manager for Z central central double differential cross section
    @author Sebastian Schmitt
    @date July 2011
*/
class ZeeDAnalysisCutHistManagerZBaseline : public ZeeDCutHistManager {
public:

    struct Mode {

        enum Value {SIGNAL, QCD};

    };

    ZeeDAnalysisCutHistManagerZBaseline(TString name = "ZeeDAnalysisCutHistManagerZBaseline",
                                        Mode::Value mode = Mode::SIGNAL)
        : ZeeDCutHistManager(name),
	  fSvcHelper("ZeeDAnalysisCutHistManagerZBaseline"),
          fAnaOptions(fSvcHelper.GetAnaSvc()),
          fMode(mode)
    {}

    void BookCutHistManager();

    void SetMode(Mode::Value mode) {
        fMode = mode;
    }

    Mode::Value GetMode() const {
        return fMode;
    }

private:

    void BookRec();
    void BookQCD();
    void BookGen();
    void BookGenRec();

    /** sets f*Stages and f*Cuts */
    void SetupCuts();

    void SetupRecCuts();

    void SetupGenCuts();

    void SetupGenRecCuts();

    std::vector<ZeeDHistManager*> GetRecHMs(TString path, TString grid);
    std::vector<ZeeDHistManager*> GetGenHMs(TString path, TString grid);
    std::vector<ZeeDHistManager*> GetGenRecHMs(TString path);

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;      //!

    std::vector<std::string> fRecStages;
    std::string fRecCuts;
    std::string fQCDCuts;

    std::vector<std::string> fGenStages;
    std::string fGenCuts;

    std::string fMinimalGenCuts;

    std::string fGenRecCuts;

    ZeeDLevelBit fDefaultLevel;
    ZeeDLevelBit fDefaultLevelMask;

    Mode::Value fMode;

    std::string fRecoIsReconstructed;
    std::string fTruthIsFiducial;

};

#endif
