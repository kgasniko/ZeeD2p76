#ifndef __ZEEDHISTMANAGERMCTRIGEFFELECTRON_H
#define __ZEEDHISTMANAGERMCTRIGEFFELECTRON_H


#include "ZeeDHistManager/ZeeDHistManagerLeptonBase.h"
#include "ZeeDHist/ZeeDHistManager.h"
#include "TString.h"

class ZeeDLepton;
class TObjArray;


class ZeeDHistManagerMCTrigEffLepton : public ZeeDHistManagerLeptonBase {

public:

    explicit ZeeDHistManagerMCTrigEffLepton(TString name) : ZeeDHistManagerLeptonBase(name) {}    
    virtual void BookHistos();
    virtual void FillLeptonHists(const TObjArray* leptons, Double_t Weight);


};

#endif

