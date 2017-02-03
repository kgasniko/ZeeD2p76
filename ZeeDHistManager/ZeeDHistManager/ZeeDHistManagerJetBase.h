#ifndef ZEEDHISTMANAGERJETBASE
#define ZEEDHISTMANAGERJETBASE

///////////////////////////////////////////////
//  Name ZeeDHistManagerJetBase
//  Interface class to provide basic iterators for jet based histograms
//  @author Rohin Narayan
//  @date 2011 November 29
//
//////////////////////////////////////////////

#include "ZeeDHist/ZeeDHistManager.h"
#include "TString.h"

class ZeeJet;
class TObjArray;


class ZeeDHistManagerJetBase: public ZeeDHistManager {
public:

    explicit ZeeDHistManagerJetBase(TString name)
        : ZeeDHistManager(name), bIsMC(kFALSE), bIsAll(kFALSE)
    {}


    void SetProperties(Bool_t isAll) {
        bIsAll = isAll;
    }


    // Arrange loop over jets
    using ZeeDHistManager::Fill;
    virtual void Fill();

    // Fill histos for list of jets
    virtual void FillJetHists(const TObjArray* jets, Double_t Weight) = 0;


protected:
    Bool_t bIsMC;
    Bool_t bIsAll;
};

#endif //ZeeDHistManagerElectronBase

