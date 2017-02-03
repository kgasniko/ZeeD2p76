#ifndef __ZEEDANALYSISCUTHISTMANAGER_ZCF
#define __ZEEDANALYSISCUTHISTMANAGER_ZCF

////////////////////////////////////////////////////////
// Name    : ZeeDAnalysisCutHistManagerZCF.h
///////////////////////////////////////////////////////

#include "ZeeDCuts/ZeeDCutHistManager.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "ZeeDTools/ZeeDCutBit.h"

#include "TString.h"

/** Z CF boson cut-histmanager
  @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte, Sebastian Schmitt
  @date   Wed Jul 21 16:36:24 CEST 2010
*/
class ZeeDAnalysisCutHistManagerZCF : public ZeeDCutHistManager {
public:

    struct Mode {

        enum Value {SIGNAL, QCD};

    };
    
    
    ZeeDAnalysisCutHistManagerZCF(TString name = "ZeeDAnalysisCutHistManagerZCF",
                                        Mode::Value mode = Mode::SIGNAL)
        : ZeeDCutHistManager(name),
	  fSvcHelper("ZeeDAnalysisCutHistManagerZCF"),
          fAnaOptions(fSvcHelper.GetAnaSvc()),
          fMode(mode)
    {}    

    void BookCutHistManager();
    
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

    std::vector<ZeeDHistManager*> GetRecHMs(TString path, TString grid, Bool_t useSys=kFALSE);
    std::vector<ZeeDHistManager*> GetGenHMs(TString path, TString grid);
    std::vector<ZeeDHistManager*> GetGenRecHMs(TString path);

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions; //!

    std::vector<std::string> fRecStages;
    std::vector<std::string> fGenStages;

    typedef std::map<std::string, ZeeDDoWeight> DoWeightMap;
    DoWeightMap fDoWeightMap;

    typedef std::map<std::string, std::string> StringStringMap;
    typedef StringStringMap CutsMap;

    CutsMap fGenCutsMap;
    CutsMap fGenRecCutsMap;
    CutsMap fRecCutsMap;

    struct ana {

    ana(std::string c, std::string m, std::string g, ZeeDDoWeight d, std::string s) : cut(c), mask(m), grid(g), doWeight(d), suffix(s) {}

    std::string cut;
    std::string mask;
    std::string grid;
    ZeeDDoWeight doWeight;
    std::string suffix;

    };

    std::vector<ana> fAnaVec;

    std::string fQCDCuts;
    
    ZeeDLevelBit fDefaultLevel;
    ZeeDLevelBit fDefaultLevelMask;
    
    Mode::Value fMode;
    
    };

#endif
