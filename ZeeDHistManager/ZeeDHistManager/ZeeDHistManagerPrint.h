#ifndef __ZeeDHistManagerPrint_H
#define __ZeeDHistManagerPrint_H

////////////////////////////////////////////////////////
// Name    : ZeeDHistManagerPrint.h
////////////////////////////////////////////////////////

// Root includes
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TObject.h>
#include "TString.h"
#include <TTree.h>
#include <TFile.h>

// ZeeD
#include "ZeeDHist/ZeeDHistManager.h"

#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDEvent.h"

class ZeeDCutHistManager;

/** Basic class for dump electron information
    @author M. Karnevskiy
    @date 2010/12/20
*/

using std::vector;

class ZeeDHistManagerPrint :  public ZeeDHistManager {

public:

    ZeeDHistManagerPrint(TString name);
    virtual ~ZeeDHistManagerPrint();

    virtual void BookHistos();
    using ZeeDHistManager::Fill;
    virtual void Fill();
    virtual void Finalize();

private:

    TFile* Bfile;
    TTree* treeOut;

    TLorentzVector* e1;
    TLorentzVector* e2;
    TLorentzVector* mZ;

    TLorentzVector* e1Cl;
    TLorentzVector* e2Cl;

    short ISEM[2];
    short Author[2];
    short OQMaps[2];
    short Charge[2];

    int Run;
    int LB;
    int Evt;

};

#endif
