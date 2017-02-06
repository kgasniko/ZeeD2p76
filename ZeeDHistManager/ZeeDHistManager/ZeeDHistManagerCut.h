#ifndef __ZEEDHISTMANAGERCUT_H
#define __ZEEDHISTMANAGERCUT_H

////////////////////////////////////////////////////////
// Name    : ZeeDHistManagerCut.h
////////////////////////////////////////////////////////

// Analysis includes
#include "ZeeDHist/ZeeDHistManager.h"
#include "TString.h"

/** Class for managing basic control distributions for W boson studies
    @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
    @date 2010/05/18
*/
class ZeeDHistManagerCut : public ZeeDHistManager {

public:

    explicit ZeeDHistManagerCut(TString name);
    ~ZeeDHistManagerCut();

    void BookHistos();
    using ZeeDHistManager::Fill;
    void Fill();

};

#endif // ZeeDHistManagerCut
