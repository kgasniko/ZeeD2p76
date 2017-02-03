#ifndef __ZEEDHISTMANAGERTAGANDPROBEELECTRON_H
#define __ZEEDHISTMANAGERTAGANDPROBEELECTRON_H

#include "ZeeDHistManager/ZeeDHistManagerLeptonBase.h"
#include "ZeeDHist/ZeeDHistManager.h"
#include "TString.h"

class ZeeDLepton;
class TObjArray;


class ZeeDHistManagerTagAndProbeLepton : public ZeeDHistManagerLeptonBase
{

    Bool_t IdLepCut(const ZeeDLepton * lepton);         // Lepton identifacation
    Bool_t TrigMatchLepCut(const ZeeDLepton * lepton);  // Lepton trigger matching

public:

    explicit ZeeDHistManagerTagAndProbeLepton(TString name) : ZeeDHistManagerLeptonBase(name) {}
   
    virtual void BookHistos();    
    virtual void FillLeptonHists(const TObjArray* leptons, Double_t Weight); 

    virtual Bool_t IsTag(const ZeeDLepton * lepton);
    virtual void FillProbe(const ZeeDLepton * lepton, Double_t Weight);



};

#endif

