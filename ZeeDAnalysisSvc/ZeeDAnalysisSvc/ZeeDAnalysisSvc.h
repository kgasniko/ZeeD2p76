#pragma once

// STL includes
#include <string>

// Minimal service includes
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/IToolSvc.h"
#include "GaudiKernel/ITHistSvc.h"
#include "GaudiKernel/ToolHandle.h"

#include "AthenaBaseComps/AthService.h"
#include "egammaInterfaces/IegammaIsolationMVATopTool.h"
#include "egammaInterfaces/egammaIsolationMVATopToolDefs.h"

// local includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDArrays/ZeeDBinGrid.h"
#include "ZeeDArrays/ZeeDSimpleBinGrid.h"


template <class TYPE> class SvcFactory;

/** ZeeDAnalysisSvc
    @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
    @date 2008/09/03
*/
class ZeeDAnalysisSvc : public AthService, virtual public IZeeDAnalysisSvc {

public:

    /// Retrieve interface ID
    static const InterfaceID& interfaceID() {
        return IID_IZeeDAnalysisSvc;
    }

    /// Query interface
    StatusCode queryInterface(const InterfaceID& riid, void** ppvIF);

    StatusCode initialize();
    StatusCode finalize();

    // get methods
    double Run_Min()                                   const {
        return fRun_Min;	// cut on the run number
    }
    double Run_Max()                                   const {
        return fRun_Max;	// cut on the run number
    }
    double Elec_EtaFwdMin()                            const {
        return fElec_EtaFwdMin;	// electron forward eta cut
    }
    double Elec_EtaFwdMax()                            const {
        return fElec_EtaFwdMax; // electron forward eta cut
    }

    double Elec_EtaCut()                               const {
        return fElec_EtaCut;	// electron eta cut
    }
    double Elec_PtCut()                                const {
        return fElec_PtCut;	// electron pt cut
    }
    double Elec_CentralPtMinCut()                      const {
        return fElec_CentralPtMinCut;	// electron pt cut
    }
    double Elec_ForwardPtMinCut()                      const {
        return fElec_ForwardPtMinCut;	// electron pt cut
    }
    double Elec_DZVtxCut()                             const {
        return fElec_DZVtxCut;	// electron dzvtx cut
    }
    double Z_PtCut()                                   const {
        return fZ_PtCut;	// z pt cut
    }

    double Jet_PtCut()                                 const {
        return fJet_PtCut;
    }
    double Jet_EtaCut()                                const {
        return fJet_EtaCut;
    }
    double ZLowMassCutMin()                            const {
        return fZLowMassCutMin;
    }
    double ZLowMassCutMax()                            const {
        return fZLowMassCutMax;
    }
    double ZMassCutLooseMin()                          const {
        return fZMassCutLooseMin;
    }
    double ZMassCutLooseMax(void)                      const {
        return fZMassCutLooseMax;
    }
    double ZMassCutVeryLooseMin(void)                  const {
        return fZMassCutVeryLooseMin;
    }
    double ZMassCutVeryLooseMax(void)                  const {
        return fZMassCutVeryLooseMax;
    }

    double EtaCrackEMBEMEC_Min(void)                   const {
        return fEtaCrackEMBEMEC_Min;
    }
    double EtaCrackEMBEMEC_Max(void)                   const {
        return fEtaCrackEMBEMEC_Max;
    }

    double PriVertexZCut()                             const {
        return fPriVertexZCut;
    }
    int MinNumTrkCut()                                 const {
        return fMinNumTrkCut;
    }
    int MinNumVertexCut()                              const {
        return fMinNumVertexCut;
    }
    std::string CaloClusterContainer()                 const {
        return fCaloClusterContainer;
    }
    std::string ElectronContainer()                    const {
        return fElectronContainer;
    }

    std::string PDFFileName()                          const {
        return fPDFFileName;
    }

    std::string Source()                               const {
        return fSource;
    }
    std::string MCtype()                               const {
        return fMCtype;
    }

    // Flag to switching off analysis ------------------------

    bool ReadEtMiss()                                  const {
        return bReadEtMiss;
    }

    bool RecalcEtMiss()                                  const {
        return bRecalcEtMiss;
    }

    bool HadrRecoilFull()                              const {
        return bHadrRecoilFull;
    }

    bool DeleteSoftTerm()                              const {
        return bDeleteSoftTerm;
    }

    bool CalcW()                                       const {
        return bCalcW;
    }

    bool CalcWmu()                                     const {
         return bCalcWmu;
    }
   
    bool CalcZmumu()                                   const {
         return bCalcZmumu;
    }

    bool CalcJets()                                    const {
        return bCalcJets;
    }

    // Flags work only if CalcJets = True ------------------------

    bool DoZjet()                                      const {
        return bDoZjet;
    }

    bool DoZjetAna()                                   const {
        return bDoZjetAna;
    }

    bool DoZplusJet()                                   const {
        return bDoZplusJet;
    }

    bool CalcJetCharge()                               const {
        return bCalcJetCharge;
    }
    // -----------------------------------------------------------
    bool DoBackgroundChargeAnalysis()                  const {
        return bDoBackgroundChargeAnalysis;
    }

    bool DoElectronCounting()                          const {
        return bDoElectronCounting;
    }

    bool DoTruthMatching() const {
        return bDoTruthMatching;
    }

    bool DoTriggerMatching() const {
        return bDoTriggerMatching;
    }

    bool FillNoCuts() const {
        return bFillNoCuts;
    }

    bool FillAnalysisChains() const {
        return bFillAnalysisChains;
    }

    bool ExecuteAnalysisLoop() const {
        return bExecuteAnalysisLoop;
    }

    bool AnalyseEvent() const {
        return bAnalyseEvent;
    }

    bool DoManualGRL()  const {
        return bDoManualGRL;
    }

    std::string ManualGRLFilename()  const {
        return fManualGRLFilename;
    }

// -------------------------------------------------------

    // For ZeeDInlineZ Package -------------------------------
    std::string ZeeDInlineZ_InFileName()               const {
        return fInline_InFileName;
    }
    int ZeeDInlineZ_SeedGen()                          const {
        return fInline_SeedGen;
    }
    double ZeeDInlineZ_ESmear()                        const {
        return fInline_ESmear;
    }
    bool   ZeeDInlineZ_UseRadiation()                  const {
        return fInline_UseRadiation;
    }
    // -------------------------------------------------------


    std::vector<std::string> TTreeFileList()           const {
        return fTTreeFileList;
    }

    virtual std::string TTreeOutput()                  const {
        return fTTreeOutput;
    }
    virtual int TTreeOutputMaxSize()                   const {
        return fTTreeOutputMaxSize;
    }

    bool UseSystematics()                              const {
        return fUseSystematics;
    }
    bool FillGenInfo()                                 const {
        return fFillGenInfo;
    }
    bool RunNumberSim()                                const {
        return fRunNumberSim;
    }
    bool ReweightSingleParticels()                     const {
        return fReweightSingleParticels;
    }


    int SelectCalibMethod()                            const {
        return fSelectCalibMethod;
    }

    int FirstRunHistogram()                            const {
        return fFirstRunHistogram;
    }
    int LastRunHistogram()                             const {
        return fLastRunHistogram;
    }

    bool AnalysisCutSelectorBookExtraCuts()            const {
        return fAnalysisCutSelectorBookExtraCuts;
    }


    std::string CalibrationInFile()                    const {
        return fCalibrationInFile;
    }
    std::string SmearingFile()                         const {
        return fSmearingFile;
    }
    std::string CalibrationOutFile()                   const {
        return fCalibrationOutFile;
    }
    std::string CalibrationInternalFile()              const {
        return fCalibrationInternalFile;
    }

    bool UseTrigger()                                  const {
        return fUseTrigger;
    }
     std::vector<std::string> TriggerEvent()           const {
        return fTriggerEvent;
    }
    const std::map<std::string, int>* getTriggerMap()  const {
        return (&fTriggerMap);
    }
    void setTriggerMap(std::map<std::string, int> trig_map) {
        fTriggerMap = trig_map;
    }
    
    std::vector<std::string> L1TriggerElecMatch()      const {
        return fL1TriggerElecMatch;
    }
    std::vector<std::string> L2TriggerElecMatch()      const {
        return fL2TriggerElecMatch;
    }
    std::vector<std::string> EFTriggerElecMatch()      const {
        return fEFTriggerElecMatch;
    }

    // ZeeD corrections Flags --------------------------------
    bool ApplyNvtxReweighting()                        const {
        return fApplyNvtxReweighting;
    }
    bool ApplyZPosVtxReweighting()                     const {
        return fApplyZPosVtxReweighting;
    }
    bool ApplyNTrkVtxReweighting()                     const {
        return fApplyNTrkVtxReweighting;
    } 
    bool ApplyPDFReweighting()                         const {
        return fApplyPDFReweighting;
    }
    bool ApplyPileupReweighting()                      const {
      return fApplyPileupReweighting;
    }
    bool ApplyElecEnergyScaleCorr()                    const {
        return fApplyElecEnergyScaleCorr;
    }
    bool ApplyElecEnergySmearingCorr()                 const {
        return fApplyElecEnergySmearingCorr;
    }
    bool ApplyElecIsoCorr()                            const {
        return fApplyElecIsoCorr;
    }
    bool ApplyTrigCorr()                               const {
        return fApplyTrigCorr;
    }
    bool ApplyPtZReweighting()                         const {
        return fApplyPtZReweighting;
    }
    bool ApplyCentralIDCorrection()                    const {
        return fApplyCentralIDCorrection;
    }
    bool ApplyForwardIDCorrection()                    const {
        return fApplyForwardIDCorrection;
    }
    bool ApplyRecoCorrection()                         const {
        return fApplyRecoCorrection;
    }
    bool ApplyChargeCorrection()                       const {
        return fApplyChargeCorrection;
    }
    bool ApplyLineShapeCorrection()                    const {
        return fApplyLineShapeCorrection;
    }

    bool ApplyPrivateElecEnergyScaleCorr()             const {
        return fApplyPrivateElecEnergyScaleCorr;
    }
    bool ApplyPrivateElecEnergySmearingCorr()          const {
        return fApplyPrivateElecEnergySmearingCorr;
    }
    // -------------------------------------------------------

    bool AnalysisCutHistManagerZFillStages()               const {
         return fAnalysisCutHistManagerZFillStages;
    }
    bool FillCalibrator()                                  const {
         return fFillCalibrator;
    }
    bool FillBkgHistograms()                               const {
         return fFillBkgHistograms;
    }
    bool FillBitMask()                                     const {
         return fFillBitMask;
    }
    bool FillObservable()                                  const {
         return fFillObservable;
    }

    // -------------------------------------------------------
    ZeeDPoissonRandomVault* GetPoissonRandomVault()    const {
        return fPoissonVault;
    }
    bool genBootstrapRnd()                             const {
        return (nPoissonNumbers > 0 ? true : false);
    }

    int MCSystNumbers()                                const {
        return nMCSystNumbers;
    }

    bool DoPtJetReweighting() const {
      return fDoPtJetReweighting;
    }

    std::string PtJetReweightingFile() const {
       return fPtJetReweightingFile;
    }

    // -------------------------------------------------------
    ZeeDBinGrid* getBinGrid(std::string name);
    ZeeDSimpleBinGrid* getSimpleBinGrid(std::string name);

    // -------------------------------------------------------

    std::string TTreeChainName()                       const {
        return fTTreeChainName;
    }

    virtual bool FillIsolationDecision()               const {
        return bFillIsolationDecision;
    }

    ToolHandle<IegammaIsolationMVATopTool> GetIsolationTool()  const {
        return f_egammaIsolationMVATopTool;
    }


    // Helpers
    const std::string print()                         const;   //!< dump object content

    bool         IsData()                             const {
        return fInput == ZeeDEnum::InputType::Data;
    }

    // if it is not data it is MC
    bool         IsMC()                               const {
        //return true;
        return fInput == ZeeDEnum::InputType::MC;
    }

    // is automatic data/mc detection requested?
    bool        MCDataAuto()                         const {
    	return fMCDataAuto;
    }

    // will not be declared as property -> not availabe from jO
    int         DatasetID()                          const {
        return fDatasetID;
    }

    void SetInputType(const ZeeDEnum::InputType::Value value) {

    	if (fMCDataAuto) {
    		fInput = value;
    	} else {
    		// no auto detection requested, which means that changing this would be against user's will
    		// Should we show some message perhaps? Or may be an exception.
    	}
    }

    // return the raw InputType
    ZeeDEnum::InputType::Value InputType() const {
    	return fInput;
    }

    void SetDatasetID(int datasetID) {

        fDatasetID = datasetID;

    }

    ToolHandle<IAthSelectorTool> JetCleaningTool() const {
      return fJetCleaningTool;
    }
    
    
    std::vector<std::string> JetCollectionNames() const {
      return fJetCollectionNames;
    }

   ToolHandleArray<IEventEtDensityProvider> EtDensityTools() const {
      return fEtDensityTools;
   }


    // DO NOT REMOVE THE FOLLOWING LINE!
    //__NEWFUNCTION__

    bool UseZCCElectronFiltration() const {
        return fUseZCCElectronFiltration;
    }


    ToolHandle<IegammaSwTool> ClusterCorrectionTool() const {
        return fClusterCorrectionTool;
    }


    bool UseNNEMCalibration() const {
        return fUseNNEMCalibration;
    }


    bool DoZCF() const {
        return fDoZCF;
    }


    bool DoZCC() const {
        return fDoZCC;
    }


    bool CalcEMu() const {
        return fCalcEMu;
    }


    bool AddSingleRejectsHMs() const {
        return fAddSingleRejectsHMs;
    }


    bool AddDetailHM() const {
        return fAddDetailHM;
    }


    bool ApplyTriggerCorrection() const {
        return fApplyTriggerCorrection;
    }
    
    bool MuonOneBinTriggerSF() const {
        return fMuonOneBinTriggerSF;
    }

    bool MuonChargedTriggerSF() const{
        return fMuonChargedTriggerSF;
    }

    std::string TriggerEfficiencyElectronFileName() const {
        return fTriggerEfficiencyElectronFileName;
    }


    std::string IdentificationEfficiencyElectronMediumFileName() const {
        return fIdentificationEfficiencyElectronMediumFileName;
    }

    std::string IdentificationEfficiencyElectronTightFileName() const {
        return fIdentificationEfficiencyElectronTightFileName;
    }

    std::string IdentificationEfficiencyElectronLooseFileName() const {
        return fIdentificationEfficiencyElectronLooseFileName;
    }

    std::string IdentificationEfficiencyElectronFwdTightFileName() const {
        return fIdentificationEfficiencyElectronFwdTightFileName;
    }
    
    std::string ReconstructionEfficiencyElectronFileName() const {
        return fReconstructionEfficiencyElectronFileName;
    }

    std::string TriggerEfficiencyMuonFileName() const {
        return fTriggerEfficiencyMuonFileName;
    }


    std::string IdentificationEfficiencyMuonMediumFileName() const {
        return fIdentificationEfficiencyMuonMediumFileName;
    }


    std::string IdentificationEfficiencyMuonFwdTightFileName() const {
        return fIdentificationEfficiencyMuonFwdTightFileName;
    }

    std::string ReconstructionEfficiencyMuonFileName() const {
        return fReconstructionEfficiencyMuonFileName;
    }


    bool EvalSysExtraCrackCut() const {
        return fEvalSysExtraCrackCut;
    }


    std::string ElectronMCChargeCorrectorName() const {
        return fElectronMCChargeCorrectorName;
    }


    std::string IDFwdCorrectionTightName() const {
        return fIDFwdCorrectionTightName;
    }


    std::string IDFwdCorrectionMediumName() const {
        return fIDFwdCorrectionMediumName;
    }


    std::string IDFwdCorrectionLooseName() const {
        return fIDFwdCorrectionLooseName;
    }


    std::string IDCorrectionTightPPName() const {
        return fIDCorrectionTightPPName;
    }


    std::string IDCorrectionMediumPPName() const {
        return fIDCorrectionMediumPPName;
    }


    bool ApplyGenBosMassCut() const {
        return fApplyGenBosMassCut;
    }


    bool GenBosMassMirrorCut() const {
        return fGenBosMassMirrorCut;
    }


    bool GenBosMassCutContributesToGenWeight() const {
        return fGenBosMassCutContributesToGenWeight;
    }


    double ExtraCrackCutMax() const {
        return fExtraCrackCutMax;
    }


    double ExtraCrackCutMin() const {
        return fExtraCrackCutMin;
    }


    std::string MCG4SimTag() const {
        return fMCG4SimTag;
    }


    std::string ZvtxRewFile() const {
        return fZvtxRewFile;
    }

    bool ExtraCrackCut() const {
        return fExtraCrackCut;
    }


    bool CalcEIso() const {
        return fCalcEIso;
    }


    bool FillBitsHists() const {
        return fFillBitsHists;
    }


    bool MinimalHistoMode() const {
        return fMinimalHistoMode;
    }


    bool EvalSysTrigCorrMC() const {
        return fEvalSysTrigCorrMC;
    }


    bool EvalSysRecoCorrectionMC() const {
        return fEvalSysRecoCorrectionMC;
    }


    bool EvalSysElecIsoCorrectionMC() const {
        return fEvalSysElecIsoCorrectionMC;
    }


    bool EvalSysForwardIDCorrectionMC() const {
        return fEvalSysForwardIDCorrectionMC;
    }


    bool EvalSysCentralIDCorrectionMC() const {
        return fEvalSysCentralIDCorrectionMC;
    }


    bool WriteEWUnfoldNtuple() const {
        return fWriteEWUnfoldNtuple;
    }


    bool EvalSysElecEnergyScaleCorr() const {
        return fApplyElecEnergyScaleCorr && fEvalSysElecEnergyScaleCorr;
    }


    bool EvalSysSFMC() const {
        return fEvalSysSFMC;
    }

    bool EvalSysEtMiss() const {
        return fEvalSysEtMiss;
    }

    unsigned int nTrkVertexJets() const {
        return fNTrkVertexJets;
    }


    std::string RecalibrateJetsConfig() const {
        return fRecalibrateJetsConfig;
    }


    bool RecalibrateJets() const {
        return fRecalibrateJets;
    }


    double GenBosMassMin() const {
        return fGenBosMassMin;
    }


    double GenBosMassMax() const {
        return fGenBosMassMax;
    }


    std::string BinGridZCC2011() const {
        return fBinGridZCC2011;
    }


    bool EvalSysPileupReweighting() const {
        return fApplyPileupReweighting && fEvalSysPileupReweighting;
    }


    bool EvalSysZPosVtxReweighting() const {
        return fApplyZPosVtxReweighting && fEvalSysZPosVtxReweighting;
    }


    bool EvalSysTrigCorr() const {
        return fApplyTrigCorr && fEvalSysTrigCorr;
    }


    bool EvalSysNvtxReweighting() const {
        return fApplyNvtxReweighting && fEvalSysNvtxReweighting;
    }


    bool EvalSysChargeCorrection() const {
        return fApplyChargeCorrection && fEvalSysChargeCorrection;
    }


    bool EvalSysRecoCorrection() const {
        return fApplyRecoCorrection && fEvalSysRecoCorrection;
    }

    bool EvalSysLineShapeCorrection() const {
        return fApplyLineShapeCorrection && fEvalSysLineShapeCorrection;
    }

    bool EvalSysElecIsoCorr() const {
        return fApplyElecIsoCorr && fEvalSysElecIsoCorr;
    }


    bool EvalSysForwardIDCorrection() const {
        return fApplyForwardIDCorrection && fEvalSysForwardIDCorrection;
    }


    bool EvalSysCentralIDCorrection() const {
        return fApplyCentralIDCorrection && fEvalSysCentralIDCorrection;
    }


    bool EvalSysPtZReweighting() const {
        return fApplyPtZReweighting && fEvalSysPtZReweighting;
    }


    bool EvalSysElecEnergySmearingCorr() const {
        return fApplyElecEnergySmearingCorr && fEvalSysElecEnergySmearingCorr;
    }


    std::vector<std::string> BosonPtReweightingAdditionalTargets() const {
        return fBosonPtReweightingAdditionalTargets;
    }


    std::string BosonPtReweightingTarget() const {
        return fBosonPtReweightingTarget;
    }


    bool BookYBinnedExtraPlots() const {
        return fBookYBinnedExtraPlots;
    }


    bool BookMuShelves() const {
        return fBookMuShelves;
    }



    double dressedElectronsDeltaR() const {
        return fDressedElectronsDeltaR;
    }

    bool BookBackgroundCuts() const {
        return fBookBackgroundCuts;
    }


    bool RunZBaselineAna() const {
        return fRunZBaselineAna;
    }


    bool IsInlineZ() const {
        return fInput == ZeeDEnum::InputType::Inline;
    }




    double EtaCrackEMECFCAL_Max() const {
        return fEtaCrackEMECFCAL_Max;
    }


    double EtaCrackEMECFCAL_Min() const {
        return fEtaCrackEMECFCAL_Min;
    }


    int PDFSetNumber() const {
        return fPDFSetNumber;
    }

    bool CalcZ() const {
        return fCalcZ;
    }



    bool RunZLowMassAna() const {
        return fRunZLowMassAna;
    }


    bool RunZCFAna() const {
        return fRunZCFAna;
    }


    bool RunZCCAna() const {
        return fRunZCCAna;
    }


    bool BookInitialStateShelves() const {
        return fBookInitialStateShelves;
    }


    bool MCAssignRandomRunNumber() const {
        return fMCAssignRandomRunNumber;
    }


    bool MCWithConstantTerm() const {
        return fMCWithConstantTerm;
    }


    bool DoSinglePeriods() const {
        return fDoSinglePeriods;
    }


    bool EvaluateSystematics() const {
        return fEvaluateSystematics;
    }


    std::string MCCampaign() const {
        return fMCCampaign;
    }


    bool WriteTTreeWithCutHistManager() const {
        return fWriteTTreeWithCutHistManager;
    }


    bool GenPileupMCFile() const {
        return fGenPileupMCFile;
    }


    std::string PileupMCGenFile() const {
        return fPileupMCGenFile;
    }


    std::string PileupMCFile() const {
        return fPileupMCFile;
    }


    std::string PileupDataFile() const {
        return fPileupDataFile;
    }

    int SystematicsDetailed() const {
        return fSystematicsDetailed;
    }

    bool DoCombToyMC() const {
        return fDoCombToyMC;
    }


    std::string JESUncertaintiesConfig() const {
        return fJESUncertaintiesConfig;
    }

    std::string MultiJESUncertaintiesConfig() const {
        return fMultiJESUncertaintiesConfig;
    }

    std::string PathDirUncertaintiesConfig() const {
        return fPathDirUncertaintiesConfig;
    }
    bool DoBCHCleaning() const {
      return fDoBCHCleaning;
    }

    std::string TTileTripReaderFile() const {
      return fTTileTripReaderFile;
    }

   std::string BCHCleaningFile() const {
      return fBCHCleaningFile;
    }

   std::string JERInputFile() const {
      return fJERInputFile;
    }


protected:

    friend class SvcFactory<ZeeDAnalysisSvc>;

    /// Standard Constructor
    ZeeDAnalysisSvc(const std::string& name, ISvcLocator* sl);

    /// Standard Destructor
    ~ZeeDAnalysisSvc();

private:

    ToolHandle<IegammaIsolationMVATopTool> f_egammaIsolationMVATopTool;

    // data members
    int    fRun_Min;		// cut on the run number
    int	   fRun_Max;		// cut on the run number
    double fElec_EtaFwdMin;    // eta cut for forward electrons
    double fElec_EtaFwdMax;    // eta cut for forward electrons
    double fElec_EtaCut;          // eta cut for electrons
    double fElec_PtCut;           // pt cut for electrons
    double fElec_CentralPtMinCut;           // pt cut for electrons
    double fElec_ForwardPtMinCut;           // pt cut for electrons
    double fZ_PtCut;              // pt cut for z
    double fElec_DZVtxCut;        // dzvtx of electrons
    bool fDoPtJetReweighting;
    std::string fPtJetReweightingFile;

    double fJet_PtCut;            //Pt cut for jet
    double fJet_EtaCut;           //Eta cut for jet

    double fZLowMassCutMin;     // Low mass cut
    double fZLowMassCutMax;

    double fZMassCutLooseMin;     // loose mass cut
    double fZMassCutLooseMax;

    double fZMassCutVeryLooseMin; // very loose mass cut
    double fZMassCutVeryLooseMax;

    double fEtaCrackEMBEMEC_Min;  // Eta crack cut
    double fEtaCrackEMBEMEC_Max;

    double fPriVertexZCut;	// cut on the Z position of the primary vertex
    int    fMinNumTrkCut;	// cut on the minimal number of tracks in the good vertex
    int    fMinNumVertexCut;	// cut on the minimal number of the good vertices in the event

    std::string fCaloClusterContainer;
    std::string fElectronContainer;
    std::string fPDFFileName;

    std::string fInline_InFileName;
    std::string fSource;
    std::string fMCtype;

    std::vector<std::string> fTTreeFileList;
    std::string fTTreeOutput;
    int fTTreeOutputMaxSize;


    bool bReadEtMiss;
    bool bRecalcEtMiss;
    bool bHadrRecoilFull;
    bool bDeleteSoftTerm;
    bool bCalcW;
    bool bCalcWmu;
    bool bCalcZmumu;
    bool bCalcJets;

    bool bDoZjet;
    bool bDoZjetAna;
    bool bDoZplusJet;
    bool bCalcJetCharge;

    bool bDoTruthMatching;
    bool bDoTriggerMatching;
    bool bFillNoCuts;
    bool bFillAnalysisChains;
    bool bExecuteAnalysisLoop;
    bool bAnalyseEvent;
    bool bDoManualGRL;
    std::string fManualGRLFilename;

    bool bDoBackgroundChargeAnalysis;
    bool bDoElectronCounting;
    bool bDoInclusive;
    bool bDoDoubleDifferential;

    int    fInline_SeedGen;
    double fInline_ESmear;
    bool   fInline_UseRadiation;

    bool   fUseSystematics;
    bool   fFillGenInfo;
    bool   fRunNumberSim;
    bool   fReweightSingleParticels;

    int    fSelectCalibMethod;
    int    fSystematicsDetailed;


    int    fFirstRunHistogram;
    int    fLastRunHistogram;

    bool   fAnalysisCutSelectorBookExtraCuts;

    std::string fCalibrationInFile;
    std::string fCalibrationOutFile;
    std::string fSmearingFile;
    std::string fCalibrationInternalFile;

    bool fUseTrigger;
    std::vector<std::string> fTriggerEvent;
    std::vector<std::string> fL1TriggerElecMatch;
    std::vector<std::string> fL2TriggerElecMatch;
    std::vector<std::string> fEFTriggerElecMatch;
    std::map<std::string, int> fTriggerMap;

    bool fApplyElecEnergyScaleCorr;
    bool fApplyElecEnergySmearingCorr;
    bool fApplyCentralIDCorrection;
    bool fApplyForwardIDCorrection;
    bool fApplyRecoCorrection;
    bool fApplyElecIsoCorr;
    bool fApplyTrigCorr; 
    bool fApplyPtZReweighting;
    bool fApplyNvtxReweighting;
    bool fApplyZPosVtxReweighting;
    bool fApplyNTrkVtxReweighting;    
    bool fApplyPDFReweighting;
    bool fApplyPileupReweighting;
    bool fApplyChargeCorrection;
    bool fApplyLineShapeCorrection;
    bool fApplyPrivateElecEnergyScaleCorr;
    bool fApplyPrivateElecEnergySmearingCorr;

    std::string fBaseDirectory;

    std::string fInputProperty;
    ZeeDEnum::InputType::Value fInput;
    bool fMCDataAuto;
    int  fDatasetID;

    // Add random vault(s)
    int  nPoissonNumbers;
    int  nMCSystNumbers;
    bool fDoCombToyMC;
    ZeeDPoissonRandomVault* fPoissonVault;

    // bin grids
    std::map<std::string, ZeeDBinGrid*> binGrids;
    std::map<std::string, ZeeDSimpleBinGrid*> simpleBinGrids;

    bool fAnalysisCutHistManagerZFillStages;
    bool fFillCalibrator;
    bool fFillBkgHistograms;
    bool fFillBitMask;
    bool fFillObservable;

    ToolHandle<IAthSelectorTool> fJetCleaningTool;
    std::vector<std::string> fJetCollectionNames;

    ToolHandleArray<IEventEtDensityProvider> fEtDensityTools;
  

   // TTree file name
    std::string fTTreeChainName;

    // DO NOT REMOVE THE FOLLOWING LINE!
    //__NEWDATAMEMBER__

    bool fUseZCCElectronFiltration;


    ToolHandle<IegammaSwTool> fClusterCorrectionTool;


    bool fUseNNEMCalibration;


    bool fDoZCF;


    bool fDoZCC;


    bool fCalcEMu;


    bool fAddSingleRejectsHMs;


    bool fAddDetailHM;


    bool fApplyTriggerCorrection;
    bool fMuonOneBinTriggerSF;
    bool fMuonChargedTriggerSF;
    std::string fTriggerEfficiencyElectronFileName;


    std::string fIdentificationEfficiencyElectronMediumFileName;
    std::string fIdentificationEfficiencyElectronTightFileName;
    std::string fIdentificationEfficiencyElectronLooseFileName;

    std::string fIdentificationEfficiencyElectronFwdTightFileName;
    
    
    std::string fReconstructionEfficiencyElectronFileName;

    std::string fTriggerEfficiencyMuonFileName;


    std::string fIdentificationEfficiencyMuonMediumFileName;

    std::string fIdentificationEfficiencyMuonFwdTightFileName;


    std::string fReconstructionEfficiencyMuonFileName;



    bool fEvalSysExtraCrackCut;


    std::string fElectronMCChargeCorrectorName;


    std::string fIDFwdCorrectionTightName;


    std::string fIDFwdCorrectionMediumName;


    std::string fIDFwdCorrectionLooseName;


    std::string fIDCorrectionTightPPName;


    std::string fIDCorrectionMediumPPName;


    bool fApplyGenBosMassCut;


    bool fGenBosMassMirrorCut;


    bool fGenBosMassCutContributesToGenWeight;


    double fExtraCrackCutMax;


    double fExtraCrackCutMin;


    std::string fMCG4SimTag;

    std::string fZvtxRewFile;

    bool fExtraCrackCut;

    bool bFillIsolationDecision;

    bool fCalcEIso;


    bool fFillBitsHists;


    bool fMinimalHistoMode;


    bool fEvalSysTrigCorrMC;


    bool fEvalSysRecoCorrectionMC;


    bool fEvalSysElecIsoCorrectionMC;


    bool fEvalSysForwardIDCorrectionMC;


    bool fEvalSysCentralIDCorrectionMC;


    bool fWriteEWUnfoldNtuple;


    bool fEvalSysElecEnergyScaleCorr;


    bool fEvalSysSFMC;

    bool fEvalSysEtMiss;

    unsigned int fNTrkVertexJets;


    std::string fRecalibrateJetsConfig;


    bool fRecalibrateJets;

    double fGenBosMassMin;

    double fGenBosMassMax;

    std::string fBinGridZCC2011;

    bool fEvalSysPileupReweighting;

    bool fEvalSysZPosVtxReweighting;

    bool fEvalSysTrigCorr;
    bool fEvalSysNvtxReweighting;
    bool fEvalSysChargeCorrection;
    bool fEvalSysRecoCorrection;
    bool fEvalSysLineShapeCorrection;
    bool fEvalSysElecIsoCorr;

    bool fEvalSysForwardIDCorrection;

    bool fEvalSysCentralIDCorrection;

    bool fEvalSysPtZReweighting;

    bool fEvalSysElecEnergySmearingCorr;


    std::vector<std::string> fBosonPtReweightingAdditionalTargets;


    std::string fBosonPtReweightingTarget;


    bool fBookYBinnedExtraPlots;


    bool fBookMuShelves;


    double fDressedElectronsDeltaR;


    bool fBookBackgroundCuts;


    bool fRunZBaselineAna;


    double fEtaCrackEMECFCAL_Max;


    double fEtaCrackEMECFCAL_Min;


    int fPDFSetNumber;


    bool fCalcZ;


    bool fRunZLowMassAna;


    bool fRunZCFAna;


    bool fRunZCCAna;


    bool fBookInitialStateShelves;


    bool fMCAssignRandomRunNumber;


    bool fMCWithConstantTerm;


    bool fDoSinglePeriods;


    bool fEvaluateSystematics;


    std::string fMCCampaign;


    bool fWriteTTreeWithCutHistManager;


    bool fGenPileupMCFile;


    std::string fPileupMCGenFile;


    std::string fPileupMCFile;


    std::string fPileupDataFile;


    std::string fJESUncertaintiesConfig;
   
    std::string fMultiJESUncertaintiesConfig;
    
    std::string fPathDirUncertaintiesConfig;
    bool fDoBCHCleaning;
    std::string fTTileTripReaderFile;
    std::string  fBCHCleaningFile;

    std::string  fJERInputFile;
 

};

