#ifndef __ZeeDHistManagerWriter_H
#define __ZeeDHistManagerWriter_H

////////////////////////////////////////////////////////
// Name    : ZeeDHistManagerWriter.h
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

#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDROOTInterface/ZeeDROOTEvent.h"

class ZeeDCutHistManager;

/** Basic class for write electron information in TTree
    @author M. Karnevskiy, G. Sedov
    @date 2010/12/20
*/

class ZeeDHistManagerWriter :  public ZeeDHistManager {

public:

    ZeeDHistManagerWriter(TString name, int maxSize=-1);

    virtual ~ZeeDHistManagerWriter();

    virtual void BookHistos();

    using ZeeDHistManager::Fill;
    virtual void Fill();

    virtual void Finalize();

private:

    TTree* treeOut;
    TTree* metaTreeOut;

    std::string fInputType;
    std::map<std::string, int> fTriggerMap;
    Int_t  fDatasetID;

    ZeeDROOTEvent rootEvent;

    Bool_t bFirstEvent;

};

#endif // ZeeDKinePrint
