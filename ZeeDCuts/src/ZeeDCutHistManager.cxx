#include "ZeeDCuts/ZeeDCutHistManager.h"

// std includes
#include <iostream>
#include <TFile.h>
#include <TROOT.h>
#include <TDirectory.h>
#include <TString.h>
#include <TRegexp.h>
#include <TSystem.h>
#include <fstream>
#include <sstream>
#include <string>
#include <TH1.h>
#include <TH1I.h>
#include <TH1D.h>
#include <TObjArray.h>

#include <boost/foreach.hpp>

// Athena inludes
#include "GaudiKernel/ITHistSvc.h"

// ZeeD includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDCuts/ZeeDCutSelector.h"
#include "ZeeDCuts/IZeeDCalcWeight.h"
#include "ZeeDEvent/ZeeDEvent.h"

using std::cout;
using std::endl;

#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDCutHistManager::ZeeDCutHistManager(TString name) :
    fSvcHelper(name),
    fAnaOptions(fSvcHelper.GetAnaSvc()),
    bIsInitialized(kFALSE),
    bBookCutControl(kTRUE),
    bFillBitMask(kFALSE),
    m_CutSelector(NULL),
    m_Name(name),
    m_pass_cut(NULL),
    m_pass_all_but_cut(NULL),
    m_pass_bit_mask(NULL),
    m_pass_bit_mask_val(NULL),
    fCutHM(NULL)
{
    // Constructor
    fSys = ZeeDSystematics::getInstance();

    m_DefMask.SetMask(0);
    fCutHM       = new std::vector<ZeeDCutHistPair>();
    bFillBitMask =  (*fAnaOptions)->FillBitMask();
}


//------------------------------------------------------
ZeeDCutHistManager::ZeeDCutHistManager( TString file, TString selectorDir)
{
    // Constructor
    std::cout << "Here" <<std::endl;
    fSys = ZeeDSystematics::getInstance();

    m_pass_cut         = NULL;
    m_pass_all_but_cut = NULL;
    m_pass_bit_mask    = NULL;
    m_DefMask.SetMask(0);
    m_Name = selectorDir;
    fCutHM = NULL;

    m_CutSelector      = new ZeeDCutSelector(selectorDir);

    TFile tfile(file);
    TFile* dir = (TFile*)gDirectory->Get(selectorDir);

    if ( dir == NULL ) {
        TString message = TString("Can not find directory ");
        message.Append(selectorDir);

        Error("ZeeDCutHistManager::ZeeDCutHistManager", message);
        gSystem->Exit(EXIT_FAILURE);
    }

    m_pass_bit_mask = (TH1D*)dir->Get("pass_bit_mask");

    if ( m_pass_bit_mask  == NULL ) {
        TString message = TString("Can not find histogram pass_bit_mask");

        Error("ZeeDCutHistManager::ZeeDCutHistManager", message);
        gSystem->Exit(EXIT_FAILURE);
    }

    // Store in array
    m_pass_bit_mask_val = new Double_t[m_pass_bit_mask->GetNbinsX()];

    for (int i = 0; i < m_pass_bit_mask->GetNbinsX(); i++) {
        m_pass_bit_mask_val[i] = m_pass_bit_mask->GetBinContent(i + 1);
    }



    m_pass_cut = (TH1F*)dir->Get("pass_cuts");

    if ( m_pass_bit_mask  == NULL ) {
        TString message = TString("Can not find histogram pass_cuts");

        Error("ZeeDCutHistManager::ZeeDCutHistManager", message);
        gSystem->Exit(EXIT_FAILURE);
    }

    // Book cuts with the same names as stored in m_pass_cut
    int ncut = m_pass_cut->GetNbinsX();

    for (int icut = 0; icut < ncut; icut++) {
        TString cutName = m_pass_cut->GetXaxis()->GetBinLabel(icut + 1);
        m_CutSelector->AddCut(new ZeeDCut(cutName));
    }

    tfile.Close();


}

//------------------------------------------------------
ZeeDCutHistManager::~ZeeDCutHistManager()
{
    // Destructor

    delete m_pass_bit_mask_val;
    delete fCutHM;
}

//------------------------------------------------------
void ZeeDCutHistManager::SetCutSelector(ZeeDCutSelector* Cuts)
{
    m_CutSelector = Cuts;
    m_BitsDefault = Cuts->getCuts()->size();

    // Create default mask. Set to 1 for all cuts
    for ( int i = 0; i < m_BitsDefault; i++) {
        m_DefMask.SetBit(i);
    }
}

//------------------------------------------------------
ZeeDCutBit* ZeeDCutHistManager::DecodeName(TString Name)
{

    ZeeDCutBit* Mask = new ZeeDCutBit();

    Mask->SetMask(0);

    ZeeDCutBit localMask;
    TString local = Name;
    const static TRegexp Operations  = TRegexp("[+-]");
    TString oper = "";

    while (local.Length() > 0) {
        int OpIdx = local.Index(Operations);

        if (OpIdx > 0) {
            TString operloc  = local[OpIdx];
            TString left     = local(0, OpIdx);
            TString right    = local(OpIdx + 1, local.Length());

            // Decode to mask:
            DecodeSymbol(left, &localMask);

            if (oper == "") {
                // No previous operation
                *Mask = localMask;
            } else {
                if ( oper == "+" ) {
                    Mask->Add(&localMask);
                } else {
                    Mask->Sub(&localMask);
                }
            }

            oper = operloc;
            local = right;
        } else {
            // no operation found ...
            DecodeSymbol(local, &localMask);

            if (oper == "") {
                // No previous operation
                *Mask = localMask;
            } else {
                if ( oper == "+" ) {
                    Mask->Add(&localMask);
                } else {
                    Mask->Sub(&localMask);
                }
            }

            break;
        }
    }

    return Mask;

}

//------------------------------------------------------
void ZeeDCutHistManager::DecodeSymbol(TString Name, ZeeDCutBit* Mask)
{
    const std::map<TString, ZeeDCut*>* cutsList = m_CutSelector->getCuts();

    if (Name == "ALL") {
        Mask->SetMaskAll(cutsList->size());
    } else if (Name == "DEFAULT") {
        *Mask = m_DefMask;
    } else if (Name == "NONE") {
        Mask->SetMask(0);
    } else {
        Mask->SetMask(0);
        std::map<TString, ZeeDCut*>::const_iterator itcut = cutsList->find(Name);
        if (itcut == cutsList->end()) {
            Error("ZeeDCutHistManager::DecodeSymbol", "No cut %s found", Name.Data());
            std::cout << "Available cuts: " << std::endl;
            for (itcut = cutsList->begin(); itcut != cutsList->end(); ++itcut) {
                std::cout << "\t" << itcut->first << std::endl;
            }
            gSystem->Exit(EXIT_FAILURE);
        }

        Mask->SetBit(std::distance(cutsList->begin(), itcut));
    }
}

//------------------------------------------------------
void ZeeDCutHistManager::UseEvent(const ZeeDEvent* event){

    m_CutSelector->evaluate(event);

    this->UseEvent(event, m_CutSelector->getCutMask());

}

//------------------------------------------------------
void ZeeDCutHistManager::UseEvent(const ZeeDEvent* event, ZeeDCutBit cutMask){

    // safe weight not depending on cuts
    const Double_t oldWeight = event->GetWeight();
    // Fill control histos:
    // FIXME: How to take cut dependend weights into account?
    if(bBookCutControl) {
        this->FillCutControl(&cutMask, oldWeight);
    }

    if (cutMask.GetMask() == cutMask.All()) {
        std::cout << cutMask.GetMask() << "   " <<cutMask.All() << std::endl;
    }

    // Run over all histogram managers
    for ( UInt_t iHM = 0, cutHMsize=fCutHM->size();
            iHM < cutHMsize; ++iHM) {

        ZeeDHistManager* histManager = fCutHM->at(iHM).first;
        ZeeDCutMask maskPair = fCutHM->at(iHM).second;

        // Run over all bit-mask pairs for the hist manager

        const ZeeDCutBit::maskDataType hmMask  = maskPair.hmMask.GetMask();
        const ZeeDCutBit::maskDataType refMask = maskPair.refMask.GetMask();
        //std::cout << histManager->GetHMName() <<std::endl;
        //std::cout << hmMask << "    " << refMask << std::endl;
        //std::cout << ( hmMask & refMask ) <<std::endl;

        if ( ( cutMask.GetMask() & refMask ) == ( hmMask & refMask )) {

            ZeeDDoWeight doWeight= maskPair.doWeight;

            for(unsigned int icw=0, calcWeightVecSize=fCalcWeightVec.size();
                    icw != calcWeightVecSize; ++icw) {

                IZeeDCalcWeight* calcWeight = fCalcWeightVec[icw];

                // set to the weighting settings that have been booked
                calcWeight->UseDoWeight(doWeight);

                const Double_t weight = calcWeight->CalculateEventWeight(event);

                event->SetWeight(weight);
                //event->IncreaseNGoodWBosons();

            }

            histManager->UseEvent(event);
            histManager->Fill(cutMask.GetMask());
        }

        event->SetWeight(oldWeight); // restore weight

    }

}

//------------------------------------------------------
void ZeeDCutHistManager::Finalize()
{
    for (UInt_t iHM = 0; iHM < fCutHM->size(); ++iHM) {
        fCutHM->at(iHM).first->Finalize();
    }

}

//------------------------------------------------------
void ZeeDCutHistManager::Initialize()
{

    DEBUG_MES_START;

    // avoid double initializing
    if(bIsInitialized) {
        return;
    }

    this->BookCutHistManager();

    if(bBookCutControl) {

        DEBUG_MES("Start to book cut control");
        this->BookCutControl();
        DEBUG_MES("Done");
    }


    // Do initialization:
    DEBUG_MES("Initializing HistManagers");
    for (UInt_t iHM = 0; iHM < fCutHM->size(); ++iHM) {
        DEBUG_VAR(iHM);
        DEBUG_VAR(fCutHM->at(iHM).first);
        fCutHM->at(iHM).first->Initialize();
    }
    DEBUG_MES("Done");

    bIsInitialized = kTRUE;

    DEBUG_MES_END;

}

//------------------------------------------------------
void  ZeeDCutHistManager::BookCutControl()
{
    const std::map<TString, ZeeDCut*>* cutsList = m_CutSelector->getCuts();
    unsigned int NBins = cutsList->size();

    m_pass_cut = new TH1F("pass_cuts", "pass cuts", NBins, 0, NBins);

    std::map<TString, ZeeDCut*>::const_iterator itcut = cutsList->begin();
    for (unsigned int icut = 0; icut < NBins; ++icut) {
        m_pass_cut->GetXaxis()->SetBinLabel( icut + 1, itcut->first);
        ++itcut;
    }

    m_pass_all_but_cut = new TH1F("pass_all_but_cut", "pass all but cut", NBins + 1, 0, NBins + 1);
    m_pass_all_but_cut->GetXaxis()->SetBinLabel(1, "All");

    itcut = cutsList->begin();
    for (unsigned int icut = 0; icut < NBins; ++icut) {
        m_pass_all_but_cut->GetXaxis()->SetBinLabel( icut + 2, itcut->first);
        ++itcut;
    }

    // Add histogram for events passing given bit mask
    if (bFillBitMask) {
        m_pass_bit_mask = new TH1D("pass_bit_mask", "pass bit mask", 2 << (NBins - 1), 0, 2 << (NBins - 1) );
    }

    m_h_CList = new TH1I("h_clist", "h_clist", NBins, 0, NBins);

    m_h_CList->SetBit(TH1::kIsAverage);

    itcut = cutsList->begin();
    for (unsigned int icut = 0; icut < NBins; ++icut) {
        m_h_CList->SetBinContent(icut+1, icut);
        m_h_CList->GetXaxis()->SetBinLabel(icut+1, itcut->first);
        ++itcut;
    }

    StatusCode sc = StatusCode::SUCCESS;

    ServiceHandle<ITHistSvc>* histSrv = fSvcHelper.GetTHistSvc();

    if ((*histSrv) == NULL ) {
        Error("ZeeDCutHistManager::BookCutHistManager",
                " HistSvc is not initialized");
        gSystem->Exit(EXIT_FAILURE);
    }

    TString hname = "//ZEED/" + m_Name + "/" + m_pass_cut->GetName();
    sc =  (*histSrv)->regHist(hname.Data(), m_pass_cut);


    if ( sc.isFailure() ) {
        Error("ZeeDCutHistManager::BookCutHistManager",
                "Unable to register " + hname);
        gSystem->Exit(EXIT_FAILURE);
    }

    hname =  "//ZEED/" + m_Name + "/" + m_pass_all_but_cut->GetName();
    sc =  (*histSrv)->regHist(hname.Data(), m_pass_all_but_cut);

    if ( sc.isFailure() ) {
        Error("ZeeDCutHistManager::BookCutHistManager",
                "Unable to register " + hname);
        gSystem->Exit(EXIT_FAILURE);
    }


    if (bFillBitMask) {

        hname = "//ZEED/" + m_Name + "/" + m_pass_bit_mask->GetName();
        sc =  (*histSrv)->regHist(hname.Data(), m_pass_bit_mask);

        if ( sc.isFailure() ) {
            Error("ZeeDCutHistManager::BookCutHistManager",
                    "Unable to register " + hname);
            gSystem->Exit(EXIT_FAILURE);
        }
    }

    hname = "//ZEED/" + m_Name + "/" + m_h_CList->GetName();
    sc = (*histSrv)->regHist(hname.Data(), m_h_CList);
    if ( sc.isFailure() ) {
        Error("ZeeDCutHistManager::BookCutHistManager",
                "Unable to register " + hname);
        gSystem->Exit(EXIT_FAILURE);
    }


}

//------------------------------------------------------
void  ZeeDCutHistManager::SetDefaultMask(TString Name)
{
    m_DefMask = *(this->DecodeName(Name));

    // Count bits in the default mask
    m_BitsDefault = 0;

    int NBins = m_CutSelector->getCuts()->size();

    ZeeDCutBit::maskDataType val = m_DefMask.GetMask();

    ZeeDCutBit::maskDataType Bit = 1;

    for ( int i = 0; i < NBins; i++ ) {
        if ( val & Bit ) {
            ++m_BitsDefault;
        }

        Bit *= 2;
    }

}

//------------------------------------------------------
void  ZeeDCutHistManager::FillCutControl(ZeeDCutBit* Mask, Double_t weight)
{

    DEBUG_MES_START;

    /** Fill control histograms  */
    int NBins = m_CutSelector->getCuts()->size();
    int NSet = 0;
    int NSet_wrt_default = 0; // amount of passed cuts from default

    DEBUG_VAR(NBins);

    ZeeDCutBit::maskDataType val = Mask->GetMask();
    ZeeDCutBit::maskDataType def = m_DefMask.GetMask();

    DEBUG_VAR(val);
    DEBUG_VAR(def);

    if (bFillBitMask) {
        m_pass_bit_mask->Fill(val, weight);
    }


    ZeeDCutBit::maskDataType Bit = 1;

    // Remove & def, to monitor all cuts:
    for ( int i = 0; i < NBins; i++ ) {
        if ( val & Bit  ) {
            m_pass_cut->Fill(i, weight);
            ++NSet;
        }

        if ( val & def & Bit ) {
            ++NSet_wrt_default;
        }

        Bit *= 2;
    }

    DEBUG_VAR(NSet);
    DEBUG_VAR(NSet_wrt_default);

    DEBUG_VAR(m_BitsDefault);

    if ( (val & def) == def ) {
        // All pass
        DEBUG_MES("All pass");
        m_pass_all_but_cut->Fill(0);
    }


    else if (NSet_wrt_default == m_BitsDefault - 1) {
        // Find bit which fails
        ZeeDCutBit::maskDataType Bit = 1;

        for ( int i = 0; i < NBins; i++ ) {
            if ( ((val  & def & Bit) == 0) && (( def & Bit ) != 0) ) {
                DEBUG_MES("All but");
                DEBUG_VAR(Bit);
                DEBUG_VAR(i+1);
                m_pass_all_but_cut->Fill(i + 1, weight);
                break;
            }

            Bit *= 2;
        }
    }

    DEBUG_MES_END;

}

//------------------------------------------------------

double ZeeDCutHistManager::GetCutFlow(TString Name)
{
    ZeeDCutBit* Mask = this->DecodeName(Name);

    ZeeDCutBit::maskDataType val = Mask->GetMask() ;

    // Run over all masks
    int nCut = m_CutSelector->getCuts()->size();

    double Events = 0;

    for (ZeeDCutBit::maskDataType iMask = 0; iMask < (unsigned int)(2 << (nCut - 1)); iMask++) {
        if ( (val & iMask) == val ) {
            Events += m_pass_bit_mask_val[iMask];
        }

    }

    delete Mask;
    return Events;
}

//------------------------------------------------------
void ZeeDCutHistManager::AddHMs(std::vector<ZeeDHistManager*> HMs,
        TString mask,
        TString refMask,
        ZeeDDoWeight doWeight,
        ZeeDLevelBit level,
        ZeeDLevelBit levelMask) {

    BOOST_FOREACH(ZeeDHistManager* hm, HMs) {

        this->AddMask(mask, hm, refMask, doWeight);
        hm->SetLevelAndMask(level, levelMask);  // after AddMask, since AddMask sets defaults

    }

}

//------------------------------------------------------
void ZeeDCutHistManager::AddHMs(std::vector<ZeeDHistManager*> HMs,
        TString mask,
        ZeeDDoWeight doWeight) {

    BOOST_FOREACH(ZeeDHistManager* hm, HMs) {

        this->AddMaskLoose(mask, hm, doWeight);

    }

}

//------------------------------------------------------
void ZeeDCutHistManager::AddHMs(std::vector<ZeeDHistManager*> HMs,
        TString mask,
        TString refMask,
        ZeeDDoWeight doWeight) {

    BOOST_FOREACH(ZeeDHistManager* hm, HMs) {

        this->AddMask(mask, hm, refMask, doWeight);

    }

}
