#ifndef ZEEDCUT
#define ZEEDCUT

////////////////////////////////////////////////////////
// Name    : ZeeDCut.h
////////////////////////////////////////////////////////

// Root includes
#include <TObject.h>
#include <TString.h>
#include "ZeeDTools/ZeeDCutBit.h"

class ZeeDEvent;

/** Basic class to define cuts
    @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
    @date 2008/08/27
*/
class ZeeDCut : public TObject {
public:
    ZeeDCut(TString CutName);
    virtual ~ZeeDCut();
    /* pure virtual does not work,
       because ZeeDCutHistManager::ZeeDCutHistManager does new ZeeDCut(cutName)
    */
    virtual bool evaluate(const ZeeDEvent* /*event*/) {
        return true;
    }
    TString getname();
private:
    TString m_name;
};

class ZeeDCompoundCut : public ZeeDCut {

public:

    ZeeDCompoundCut(TString CutName) : ZeeDCut(CutName) {}

    void addCut(ZeeDCut* cut) {

        fCuts.push_back(cut);
    }

    bool evaluate(const ZeeDEvent* event) {

        for(UInt_t n=0; n != fCuts.size(); ++n) {
            if(fCuts[n]->evaluate(event) == kFALSE) {
                return kFALSE;
            }
        }

        return kTRUE;

    }

private:

    std::vector<ZeeDCut*> fCuts;

};

#endif
