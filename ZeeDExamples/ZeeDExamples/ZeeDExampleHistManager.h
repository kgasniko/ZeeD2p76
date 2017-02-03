#ifndef __ZEEDEXAMPLEHISTMANAGER_H
#define __ZEEDEXAMPLEHISTMANAGER_H

////////////////////////////////////////////////////////
// Name    : ZeeDExampleHistManager.h
////////////////////////////////////////////////////////

// Analysis includes
#include "ZeeDHist/ZeeDHistManager.h"
#include "TString.h"

/** Example class for managing histograms and access
    of variable withing ZeeD framework
    @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
    @date 2008/08/14
*/
class ZeeDExampleHistManager : public ZeeDHistManager {

public:

    ZeeDExampleHistManager();
    explicit ZeeDExampleHistManager(TString name);
    virtual ~ZeeDExampleHistManager();

    virtual void BookHistos();
    virtual void Fill();

};

#endif // ZeeDExampleHistManager
