#ifndef __ZEEDEFFEXAMPLEHISTMANAGER_H
#define __ZEEDEFFEXAMPLEHISTMANAGER_H

////////////////////////////////////////////////////////
// Name    : ZeeDEffExampleHistManager.h
////////////////////////////////////////////////////////

// Analysis includes
#include "ZeeDHist/ZeeDEffHistManager.h"
#include "TString.h"

/** Example class for managing efficiency histograms
    @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
    @date 2008/08/14
*/
class ZeeDEffExampleHistManager : public ZeeDEffHistManager {

public:

    ZeeDEffExampleHistManager();
    explicit ZeeDEffExampleHistManager(TString name);
    virtual ~ZeeDEffExampleHistManager();

    virtual void BookHistos();
    virtual void Fill();

};

#endif // ZeeDEffExampleHistManager
