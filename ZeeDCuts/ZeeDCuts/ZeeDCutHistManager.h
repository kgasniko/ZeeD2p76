#ifndef ZEEDCUTHISTMANAGER
#define ZEEDCUTHISTMANAGER

////////////////////////////////////////////////////////
// Name    : ZeeDCutHistManager.h
////////////////////////////////////////////////////////

#include <vector>

#include <TObject.h>
#include <TString.h>

#include "ZeeDHist/ZeeDHistManager.h"
#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDTools/ZeeDSystematics.h"
#include "ZeeDTools/ZeeDCutBit.h"
#include "ZeeDCuts/IZeeDCalcWeight.h"

class ZeeDCutSelector;
class ZeeDEvent;
class TH1F;
class TH1D;
class TH1I;

/** Helper class to store reference and required cut-mask */
// typedef std::pair< ZeeDCutBit, ZeeDCutBit> ZeeDCutMask;
struct ZeeDCutMask {ZeeDCutBit hmMask; ZeeDCutBit refMask; ZeeDDoWeight doWeight;} ;

/** Helper class to store hist-manager and its Cut-Mask bundle*/
typedef std::pair< ZeeDHistManager*, ZeeDCutMask > ZeeDCutHistPair;


/** An object which combines a set of cuts and a HistManager
    @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
    @date 2008/08/28
*/
class ZeeDCutHistManager : public TObject {
public:
    ZeeDCutHistManager(TString name);

    /** Constructor to analyse control histograms */
    ZeeDCutHistManager(TString file, TString selectorDir);

    virtual ~ZeeDCutHistManager();

    /** set cut selector */
    void SetCutSelector(ZeeDCutSelector* Cuts);

    /** get cut selector */
    ZeeDCutSelector* GetCutSelector() const {
        return m_CutSelector;
    }

    void AddCalcWeighter(IZeeDCalcWeight* calcWeight) {

        fCalcWeightVec.push_back(calcWeight);

    }

    std::vector<IZeeDCalcWeight*> GetCalcWeightVec() const {
        return fCalcWeightVec;
    }

    /** use bit mask */
    template <class T>
    T* AddMask(ZeeDCutBit* Mask, T* Hists, ZeeDCutBit* refMask, ZeeDDoWeight doWeight)
    {

        // Make new pair of the cut-bit mask and the reference:
        ZeeDCutMask fCutMask;
        fCutMask.hmMask = *Mask;
        fCutMask.refMask = *refMask;

        Hists->SetLevelAndMask(this->GetDefaultLevel(), this->GetDefaultLevelMask());

        fCutMask.doWeight = doWeight;

        // Create new ZeeDCutHistPair:
        ZeeDCutHistPair fCutHistPair = ZeeDCutHistPair(Hists,fCutMask);
        fCutHM->push_back(fCutHistPair);

        return Hists;

    }

    /** use bit mask, default doWeight */
    template <class T>
    T* AddMask(ZeeDCutBit* Mask, T* Hists, ZeeDCutBit* refMask)
    {

        this->AddMask(Mask, Hists, refMask, this->GetDefaultDoWeight());

        return Hists;

    }

    //------------------------------------------------------
    template <class T>
    T* AddMask( TString Name, T* Hists, TString refName, ZeeDDoWeight doWeight)
    {
        ZeeDCutBit* Mask    = this->DecodeName(Name);
        ZeeDCutBit* refMask = this->DecodeName(refName);

        this->AddMask(Mask, Hists, refMask, doWeight);

        return Hists;
    }

    /** use name*/
    template <class T>
    T* AddMask( TString Name, T* Hists, TString refName)
    {
        this->AddMask(Name, Hists, refName, this->GetDefaultDoWeight() );

        return Hists;
    }

    /** use name, default doWeight */
    template <class T>
    T* AddMask( TString Name, T* Hists)
    {
        this->AddMask(Name, Hists, "DEFAULT", this->GetDefaultDoWeight());

        return Hists;
    }


    template <class T>
    T* AddMaskLoose( TString Name, T* Hists, ZeeDDoWeight doWeight)
    {
        ZeeDCutBit* Mask = this->DecodeName(Name);

        this->AddMask(Mask, Hists, Mask, doWeight);

        return Hists;

    }

    /** use name, default refMask, default doWeight */
    template <class T>
    T* AddMaskLoose( TString Name, T* Hists)
    {
        this->AddMaskLoose(Name, Hists, this->GetDefaultDoWeight());
        
        return Hists;
    }

    /*
    // Add hist manager for stages

       @param stages - vector of cuts, just the name, no + sign
       @param Name - name of hist manager, will be prefix with last additional cut
       @param Hists - pointer to hist manager to be added, will be cloned
    //
    void AddMaskLooseStages( const std::vector<TString>& stages, TString ChainName, TString HistsName, ZeeDHistManager* Hists);
    */

    TString getName() const { return m_Name; }

    /** Utility to calculate cut flow */
    double GetCutFlow(TString Name);

    void SetDefaultMask(TString Name);
    ZeeDCutBit GetDefaultMask() { return m_DefMask; }

    void SetDefaultLevel(ZeeDLevelBit defaultLevel) {
        fDefaultLevel = defaultLevel;
    }

    ZeeDLevelBit GetDefaultLevel() const {
        return fDefaultLevel;
    }

    void SetDefaultLevelMask(ZeeDLevelBit defaultLevelMask) {
        fDefaultLevelMask = defaultLevelMask;
    }

    ZeeDLevelBit GetDefaultLevelMask() const {
        return fDefaultLevelMask;
    }

    /** The weight settings given will be used if none are given to
        the AddMask calls.

        ATTENTION: Call this _before_ any AddMask call, otherwise you
        will get a _wrong_ default.
     */
    void SetDefaultDoWeight(ZeeDDoWeight doWeight) {

        fDefaultDoWeight = doWeight;

    }

    ZeeDDoWeight GetDefaultDoWeight() {

        return fDefaultDoWeight;

    }

    virtual void UseEvent(const ZeeDEvent* event);

    virtual void UseEvent(const ZeeDEvent* event, ZeeDCutBit cutMask);

    void Initialize();
    virtual void BookCutHistManager() {}
    virtual void Finalize();

    void SetBookCutControl(Bool_t bookCutControl) {
        bBookCutControl = bookCutControl;
    }

    void SetFillBitMask(Bool_t FillBitMask) {
        bFillBitMask = FillBitMask;
    }

protected:

    void AddHMs(std::vector<ZeeDHistManager*> HMs,
                TString mask,
                TString refMask,
                ZeeDDoWeight doWeight,
                ZeeDLevelBit level,
                ZeeDLevelBit levelMask);

    void AddHMs(std::vector<ZeeDHistManager*> HMs,
                TString mask,
                TString refMask,
                ZeeDDoWeight doWeight);

    void AddHMs(std::vector<ZeeDHistManager*> HMs,
                TString mask,
                ZeeDDoWeight doWeight);

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;      //!

    ZeeDCutBit* DecodeName(TString Name);

private:

    Bool_t bIsInitialized;
    Bool_t bBookCutControl;
    Bool_t bFillBitMask;

    void BookCutControl();
    void FillCutControl(ZeeDCutBit* Mask, Double_t weight=1);

    void DecodeSymbol(TString Name, ZeeDCutBit* Mask);

    /** The cut selector  */
    ZeeDCutSelector* m_CutSelector;

    std::vector<IZeeDCalcWeight*> fCalcWeightVec;


    ZeeDCutBit m_DefMask;

    int m_BitsDefault;         // number of bits in the default mask

    ZeeDDoWeight fDefaultDoWeight;

    TString m_Name;
    TH1F* m_pass_cut;
    TH1F* m_pass_all_but_cut;
    TH1D* m_pass_bit_mask;
    Double_t*  m_pass_bit_mask_val;

    TH1I* m_h_CList;

    /** list of cut-hist managers and their cut bits */
    std::vector<ZeeDCutHistPair>* fCutHM;

    ZeeDLevelBit fDefaultLevel;
    ZeeDLevelBit fDefaultLevelMask;

protected:
    ZeeDSystematics* fSys;

};

#endif
