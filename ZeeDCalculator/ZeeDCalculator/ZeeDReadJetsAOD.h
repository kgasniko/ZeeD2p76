#ifndef __ZeeDReadJetsAOD_H
#define __ZeeDReadJetsAOD_H

////////////////////////////////////////////////////////
// Name    : ZeeDReadJetsAOD.h
////////////////////////////////////////////////////////

#include <TObject.h>
#include <TLorentzVector.h>
#include <TObjArray.h>

#include <string>

#include "ZeeDTools/ZeeDSvcHelper.h"
#include "StoreGate/StoreGateSvc.h"
#include "TruthUtils/IHforTool.h"

#include "GeneratorObjects/McEventCollection.h"
#include "StoreGate/DataHandle.h"

class ZeeDEvent;

class TObjArray;

class JetCollection;
class Jet;
class ZeeDJet;

/** Class for calculation of jet quantities
  @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte, Sebastian Schmitt, Rohin Narayan
  @date   Wed Jun 23 14:13:20 CEST 2010
*/

class ZeeDReadJetsAOD : public TObject {

public:

    ZeeDReadJetsAOD();
    ~ZeeDReadJetsAOD();

    /** Calculate jet candidates for the given event */
    void ReadInfo(ZeeDEvent* event);

private:

    /** Initialise necessary containers */
    void Initialize();

    /** Load JetContainer from Storegate */
    void RetrieveJetContainer();


    /** ZeeD event */
    const ZeeDEvent* fEvent;

    /** flag: is data or is MC */
    Bool_t fIsMC;

    /** Jet collection */
    const JetCollection* fJetCollection;

    /** Jet Collection Name */
    std::string fJetCollName;

    ZeeDSvcHelper fSvcHelper;

    /** Store Gate*/
    ServiceHandle<StoreGateSvc>* fStoreGate;
    /** Service handle to job options file */
    ServiceHandle<IZeeDAnalysisSvc> *fAnaOptions; //!

    
  int CalculateQGLabel( TLorentzVector jet_4momentum  );

  const DataHandle<McEventCollection> fMcEvent;

  TObjArray* genParticle;
  TObjArray toBeDeleted;

  Double_t jetCone;

  std::map<std::string,Double_t> jetConeMap;

  void  bookCuts();

};

#endif // ZeeDReadJetsAOD
