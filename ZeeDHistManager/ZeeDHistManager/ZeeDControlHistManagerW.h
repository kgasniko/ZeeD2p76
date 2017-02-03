#ifndef __ZEEDCONTROLHISTMANAGERW_H
#define __ZEEDCONTROLHISTMANAGERW_H

////////////////////////////////////////////////////////
// Name    : ZeeDControlHistManagerW.h
////////////////////////////////////////////////////////

// Analysis includes
#include "ZeeDHist/ZeeDHistManager.h"
#include "TString.h"

/** Class for managing basic control distributions for W boson studies
    @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
    @date 2010/05/18
*/
class ZeeDControlHistManagerW : public ZeeDHistManager {

public:

    explicit ZeeDControlHistManagerW(TString name);
    ~ZeeDControlHistManagerW();

    void BookHistos();
    using ZeeDHistManager::Fill;
    void Fill();

};

#endif // ZeeDControlHistManagerW
