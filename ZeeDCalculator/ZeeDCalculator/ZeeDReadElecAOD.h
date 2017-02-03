#pragma once

////////////////////////////////////////////////////////
// Name    : ZeeDReadElecAOD.h
////////////////////////////////////////////////////////
//
// Contact persons: Sebastian Schmitt, Andrei Nikiforov
//

// std includes
#include <vector>
#include <string>

// Root includes
#include <TObject.h>

// Athena includes
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ToolHandle.h"

#include "egammaInterfaces/IegammaIsolationMVATopTool.h"
#include "egammaInterfaces/egammaIsolationMVATopToolDefs.h"

// ZeeD includes
#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

// etaCalo/phiCalo calculation
// see http://acode-browser.usatlas.bnl.gov/lxr/source/atlas/PhysicsAnalysis/D3PDMaker/CaloD3PDMaker/src/ClusterPositionInCaloFillerTool.cxx
// and also https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/ElectronPhotonFourMomentumCorrection
#include "CaloEvent/CaloCluster.h"
#include "CaloUtils/CaloCellDetPos.h"

// FIXME: hack
#include "TrigDecisionTool/TrigDecisionToolCore.h"

class ZeeDElectron;
class ZeeDEvent;
class TrigMatchTool;
class EMTrackMatch;
class EMShower;
class CaloCluster;
class ElectronContainer;
class CaloClusterContainer;

namespace Analysis {

class Electron;

}

namespace HepMC {

class GenParticle;

}

namespace Trig {

class TrigDecisionTool;

}

namespace Rec {

class TrackParticle;

}

namespace Trk {

class TrackSummary;

class VxCandidate;

}

class StoreGateSvc;

/** Class for Reading of electron quantities from AOD
    @date 2011/02/25
*/

class ZeeDReadElecAOD : public TObject {

public:

    ZeeDReadElecAOD();
    ~ZeeDReadElecAOD();

    /** Read electrons from AOD and add to array
      @param electrons - electrons are added to this array
    */
    void ReadInfo(ZeeDEvent* event);

private:

    StatusCode Initialize();

    void ReadIsEM(const Analysis::Electron* AODelectron);
    void ReadTrackMatch(const EMTrackMatch* );
    void ReadShower(const EMShower* );
    void ReadCluster(const CaloCluster* );
    void ReadTrackParticle(const Rec::TrackParticle* );
    void ReadTrackSummary(const Trk::TrackSummary* );
    void ReadVertex(const Trk::VxCandidate*);
    void ReadMCTruth(const HepMC::GenParticle* genParticle);
    void ReadTrigger(const Analysis::Electron* AODelectron);
    void ReadAuthor(const Analysis::Electron*);
    void ReadCaloIso(const Analysis::Electron* AODelectron);

    /** Pointer to the electron being processed */
    ZeeDElectron* fElectronInProcess;

    /** dR cut for the trigger matching */
    double fdRCut;

    /** Flag to switch on/off trigger filling */
    Bool_t bUseTrigger; //!

    /** Electron container */
    const ElectronContainer* fElecContainer; //!

    /** Calo cluster container */
    const CaloClusterContainer* fCaloClusContainer; //!

    /** Service helper */
    ZeeDSvcHelper fSvcHelper; //!

    /** Service handle to job options file */
    ServiceHandle<IZeeDAnalysisSvc> *fAnaOptions; //!

    /** MC truth classifier tool */
    ToolHandle<IMCTruthClassifier>* fMCTruthClassifier; //!

    /** etaCalo/phiCalo calculation */
    CaloCellDetPos *m_caloCellDetPos;

    /** Store gate */
    ServiceHandle<StoreGateSvc>* fStoreGate; //!

    /** Trigger matching tool */
    ToolHandle<TrigMatchTool>* fTrigMatchTool; //!

    /** Trigger decision tool */
    ToolHandle<Trig::TrigDecisionTool>* fTrigDecisionTool; //!

    /** is MC/Data flag */
    bool fIsMC;

    /** L1 Trigger chain names */
    std::vector<std::string> fL1TrigChainNames; //!

    /** L2 Trigger chain names */
    std::vector<std::string> fL2TrigChainNames; //!

    /** EF Trigger chain names */
    std::vector<std::string> fEFTrigChainNames; //!

    /** Tool for the GEO21 raw cluster creation */
    ToolHandle<IegammaSwTool> fClusterCorrectionTool;

    ToolHandle<IegammaIsolationMVATopTool> f_egammaIsolationMVATopTool;

    // FIXME: hack for the trigger matching
    inline bool IsPassedTrigMatch(	const Analysis::Electron* AODelectron,
					const std::vector< Trig::Feature<CaloCluster> > tfeatures_clusters,
					double fdRcut);

    inline double NormalizeAngle(double dphi);

};
