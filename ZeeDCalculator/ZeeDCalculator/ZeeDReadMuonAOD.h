/*
 * ZeeDReadMuonAOD.h
 *
 *  Created on: Aug 7, 2012
 *      Author: morse
 */
#pragma once


#include <TObject.h>
#include "GaudiKernel/ToolHandle.h"
#include "StoreGate/StoreGateSvc.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"
#include "muonEvent/MuonContainer.h"
#include "muonEvent/MuonParamDefs.h"
#include "MuonRecHelperTools/MuonEDMHelperTool.h"
#include "MuonRecHelperTools/MuonIdHelperTool.h"
#include "MuonCalibITools/IIdToFixedIdTool.h"
#include "MuonCombinedToolInterfaces/IMuonPropertiesTool.h"
#include "ITrackToVertex/ITrackToVertex.h"
#include "MuonUtils/IMuonQualityTool.h"
#include "muonEvent/MuonContainer.h"
#include "ZeeDEvent/ZeeDMuon.h"


class ZeeDEvent;
class TrigMatchTool;
class VxContainer;


class StoreGateSvc;


class ZeeDReadMuonAOD: public TObject {

  public:
    ZeeDReadMuonAOD();
    virtual ~ZeeDReadMuonAOD();

    void Initialize();

    /** Calculate jet candidates for the given event */
    void ReadInfo(ZeeDEvent* event);
    void ReadTrigger(const Analysis::Muon* AODmuon);
    void ReadProperties(const Analysis::Muon* iMuon);
    void ReadCombTrack(const Rec::TrackParticle* track, const Analysis::Muon* iMuon);
    void ReadIDtrack(const Rec::TrackParticle* track, const Analysis::Muon* iMuon);
    void ReadMStrack(const Rec::TrackParticle* track);
    void ReadIsolation(const Analysis::Muon* iMuon);
    void ReadIsEM(const Analysis::Muon* iMoun);
    void ReadTrackParticle(const Rec::TrackParticle* );
    void ReadTrackSummary(const Trk::TrackSummary* );
    void ReadVertex(const Trk::VxCandidate*);
    void ReadMCTruth(const HepMC::GenParticle* genParticle);
 

  private:
    
    ZeeDMuon* curmuon;

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<StoreGateSvc>* fStoreGate;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;
    ToolHandle<Rec::IMuonPropertiesTool>  m_muonPropertiesTool;
    ToolHandle<Reco::ITrackToVertex> m_trackToVertexTool;
    ToolHandle<IMuonQualityTool> m_qualityTool;
    /** MC truth classifier tool */
    ToolHandle<IMCTruthClassifier>* fMCTruthClassifier; //!



    const Analysis::MuonContainer* fMuonContainer;
    const VxContainer* vxContainer;
    Bool_t bUseTrigger;

    std::string m_muonContainerKey;
    bool m_matchMuons;
    bool m_matchEF;


    /** Trigger decision tool */
    ToolHandle< TrigMatchTool > m_matchTool;

    /** L1 Trigger chain names */
    std::vector<std::string> fL1TrigChainNames; //!

    /** L2 Trigger chain names */
    std::vector<std::string> fL2TrigChainNames; //!

    /** EF Trigger chain names */
    std::vector<std::string> fEFTrigChainNames; //!

};

