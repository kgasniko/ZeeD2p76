#ifndef ZEEDHISTMANAGERJET
#define ZEEDHISTMANAGERJET

///////////////////////////////////////////////
//  Name ZeeDHistManagerJet
//  Basic histograms for Jets
//  @author Rohin Narayan
//  @date 29 November 2011
//////////////////////////////////////////////

#include "ZeeDHistManager/ZeeDHistManagerJetBase.h"
#include "ZeeDHist/ZeeDHistManager.h"
#include "TString.h"

class ZeeDJet;
class TObjArray;


class ZeeDHistManagerJet: public ZeeDHistManagerJetBase{
public:

    explicit ZeeDHistManagerJet(TString name, Double_t Ymin = 0., Double_t Ymax = 0.)
        : ZeeDHistManagerJetBase(name),
          fYmin(Ymin),
          fYmax(Ymax)
    {}

    // Book all histograms:
    virtual void BookHistos();

    // Fill histos for list of electrons
    virtual void FillJetHists(const TObjArray* jets, Double_t Weight);

private:
    // Y ranges for the boson
    Double_t fYmin;
    Double_t fYmax;

};

#endif //ZeeDHistManagerElectron

