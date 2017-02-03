#ifndef __ZEEDSYSTHISTMANAGER_H
#define __ZEEDSYSTHISTMANAGER_H

////////////////////////////////////////////////////////
// Name    : ZeeDSystHistManager.h
////////////////////////////////////////////////////////

// Analysis includes
#include "ZeeDHist/ZeeDHistManager.h"
#include "TString.h"

/** Control plots from "details" container
    @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte, Mikhail Karnevskiy
    @date 2008/09/03
*/
class ZeeDSystHistManager : public ZeeDHistManager {

public:

    explicit ZeeDSystHistManager(TString name);
    ~ZeeDSystHistManager();

    void BookHistos();

    using ZeeDHistManager::Fill;
    void Fill();

private:
    void FillZBosGenHistos();
    void FillZBosRecHistos();

    // unused?
    //double m_Eta_Min, m_Eta_Max, m_Pt;   // Eta range parameter
};

#endif // ZeeDSystHistManager
