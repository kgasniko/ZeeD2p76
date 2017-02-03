#ifndef ZEED_FINDER_H
#define ZEED_FINDER_H

#include <TObject.h>
#include "ZeeDEvent/ZeeDBoson.h"
#include "ZeeDEvent/ZeeDBosonW.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDJet.h"
#include "ZeeDTools/ZeeDCutBit.h"
#include "ZeeDCuts/ZeeDCutSelector.h"
#include <vector>
#include "TString.h"

class CutWeight {
public:
    CutWeight(TString N, Double_t d, ZeeDCutBit I) {
        CutName = N;
        Weight = d;
        Mask = I;
    }
    TString CutName;
    Double_t  Weight;
    ZeeDCutBit Mask;
};


/** Prototype class for Boson Finder
    Created 11 Feb 2011 by SG.
*/


class ZeeDEvent;

class ZeeDFinder : public ZeeDCutSelector {

public:
    ZeeDFinder(TString Name = "ZeeD Finder") : ZeeDCutSelector(Name) {}

    virtual ~ZeeDFinder();

    /** For a given bit pattern, return "weight".
            Bigger weight means more likelly good Z.
    */
    virtual Double_t  GetWeight(ZeeDCutBit* mask);

    virtual TObjArray SelectBosons(ZeeDEvent* event) = 0;

protected :
    std::vector<CutWeight*> CutWeights;

    const ZeeDBosonZ* SelectBestBoson(ZeeDEvent* Event, const TObjArray* Bosons);
    ZeeDJet*    SelectBestJet(ZeeDEvent* event, TObjArray* Jets);
    virtual TObjArray	SelectAllGoodBosons(ZeeDEvent* event, const TObjArray* Bosons);

inline unsigned int all_bits_1_lookup(unsigned int ncuts);

};

#endif // ZEED_FINDER_H
