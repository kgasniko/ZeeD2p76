#pragma once

// Root includes
#include <TString.h>

#include "ZeeDTools/ZeeDQuantity.h"
#include "ZeeDHist/ZeeDHistManager.h"
#include "ZeeDArrays/ZeeDBinGrid.h"
#include "ZeeDArrays/ZeeDArrayD.h"

#include "ZeeDMisc/ZeeDMisc.h"

#include <boost/foreach.hpp>

/** Class to book and fill ZeeDHistManager(s) based on a bin grid file

*/


class ZeeDGridHistManager : public ZeeDHistManager
{
public:
    ZeeDGridHistManager(TString name,  TString binGridName,  ZeeDQuantity<ZeeDArrayD> (*cFunction)(const ZeeDEvent*));

    Int_t GetNBins();

    void AttachHM(ZeeDHistManager* histManager);

    void AttachGridifiedHM(ZeeDHistManager* histManager) {
        fGridifiedHistManagers.push_back(histManager);
    }

    virtual void BookHistos() {};

    virtual void Initialize();

    virtual void UseEvent(const ZeeDEvent* event);

    using ZeeDHistManager::Fill;
    virtual void Fill();

    virtual void Finalize();

    virtual void SetLevelMask(ZeeDLevelBit levelMask) {

        BOOST_FOREACH(ZeeDHistManager* hm, fHistManagers) hm->SetLevelMask(levelMask);
        BOOST_FOREACH(ZeeDHistManager* hm, fGridifiedHistManagers) hm->SetLevelMask(levelMask);

    }

    virtual void SetLevel(ZeeDLevelBit level) {

        BOOST_FOREACH(ZeeDHistManager* hm, fHistManagers) hm->SetLevel(level);
        BOOST_FOREACH(ZeeDHistManager* hm, fGridifiedHistManagers) hm->SetLevel(level);

    }

    virtual void SetLevelAndMask(ZeeDLevelBit level, ZeeDLevelBit levelMask) {

        BOOST_FOREACH(ZeeDHistManager* hm, fHistManagers) hm->SetLevelAndMask(level, levelMask);
        BOOST_FOREACH(ZeeDHistManager* hm, fGridifiedHistManagers) hm->SetLevelAndMask(level, levelMask);

    }

    template <class HM>
    std::vector<HM*> RegisterHM (TString baseName)
    {

        nRegisteredHM += 1;

        std::vector<HM*> HMs;

        for(UInt_t nbin=0; nbin != fBinLabels.size(); ++nbin) {

            HM* hm = new HM(baseName + "/" + fBinLabels[nbin]);
            HMs.push_back(hm);
            this->AttachHM(hm);

        }

        return HMs;

    }

    template <class HM>
    HM* RegisterGridifiedHM (TString baseName)
    {

        HM* hm = new HM(baseName+"_gridified");
        hm->SetGridified(kTRUE);
        hm->SetNGridBins(this->GetNBins());
        hm->SetBinLabels(fBinLabels);
        this->AttachGridifiedHM(hm);

        return hm;

    }

    std::vector<ZeeDHistManager*> GetHistManagers() {

        return fHistManagers;

    }


private:
    TString fBaseName;
    std::vector<ZeeDHistManager*> fHistManagers;
    std::vector<ZeeDHistManager*> fGridifiedHistManagers;
    ZeeDBinGrid* fBinGrid;
    ZeeDQuantity<ZeeDArrayD> fValue;
    ZeeDQuantity<ZeeDArrayD> (*fFunction)(const ZeeDEvent*);
    UInt_t nRegisteredHM;
    std::vector<TString> fBinLabels;
};

