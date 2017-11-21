#pragma once

// STL includes
#include <string>
#include <vector>

// Standard includes
#include "GaudiKernel/IInterface.h"
#include "GaudiKernel/IToolSvc.h"
#include "GaudiKernel/ITHistSvc.h"
#include "GaudiKernel/ToolHandle.h"

#include "egammaInterfaces/IegammaIsolationMVATopTool.h"
#include "egammaInterfaces/egammaIsolationMVATopToolDefs.h"

#include "JetSelectorTools/AthJetCleaningTool.h"
#include "EventShapeInterface/IEventEtDensityProvider.h"
#include "egammaInterfaces/IegammaSwTool.h"

// ZeeD includes
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDArrays/ZeeDBinGrid.h"
#include "ZeeDArrays/ZeeDSimpleBinGrid.h"
#include "ZeeDArrays/ZeeDHistTools.h"


/** Declaration of the interface ID ( interface id, major version, minor version) */
static const InterfaceID IID_IZeeDAnalysisSvc("IZeeDAnalysisSvc", 0 , 0);

class IZeeDAnalysisSvc : virtual public IInterface {
public:

    /** Retrieve interface ID */
    static const InterfaceID& interfaceID() {
        return IID_IZeeDAnalysisSvc;
    }

    // actual interface definiton

    /*
    // set methods
    */

    // get methods
    virtual double Run_Min()		      const = 0; // cut on the run number
    virtual double Run_Max()		      const = 0; // cut on the run number

    virtual double Elec_EtaFwdMin()           const = 0; // electron eta cut
    virtual double Elec_EtaFwdMax()           const = 0; // electron eta cut

    virtual double Elec_PtCut()               const = 0; // electron pt cut
    virtual double Elec_CentralPtMinCut()     const = 0; // electron pt cut
    virtual double Elec_ForwardPtMinCut()     const = 0; // electron pt cut

    virtual double Z_PtCut()                  const = 0; // electron pt cut
    virtual double Elec_EtaCut()              const = 0; // electron eta cut
    virtual double Elec_DZVtxCut()            const = 0; // electron dzvtx cut

    virtual double Jet_PtCut()                const = 0; // jet Pt cut
    virtual double Jet_EtaCut()               const = 0; // jet Eta cut

    virtual double ZMassCutLooseMin()         const = 0; // blabla
    virtual double ZMassCutLooseMax()         const = 0;
    virtual double ZMassCutVeryLooseMin()     const = 0;
    virtual double ZMassCutVeryLooseMax()     const = 0;
    virtual double ZLowMassCutMin()           const = 0;
    virtual double ZLowMassCutMax()           const = 0;

    virtual double EtaCrackEMBEMEC_Min()      const = 0;
    virtual double EtaCrackEMBEMEC_Max()      const = 0;

    virtual double PriVertexZCut()            const = 0; // Primary Vertex Z position cut
    virtual int    MinNumTrkCut()             const = 0; // cut on minimal number of track in the good vertex
    virtual int    MinNumVertexCut()          const = 0; // cut on minimal number of vertices in the event

    virtual std::string CaloClusterContainer() const = 0;
    virtual std::string ElectronContainer()   const = 0;
    virtual std::string PDFFileName()         const = 0;


    // AOD, Inline, D3PD, ...
    virtual std::string Source()              const = 0;
    virtual std::string MCtype()              const = 0;

    virtual bool ReadEtMiss()                 const = 0;
    virtual bool RecalcEtMiss()               const = 0;
    virtual bool CorrectSumet()               const = 0;
    virtual bool HadrRecoilFull()             const = 0;
    virtual bool DeleteSoftTerm()             const = 0;
    virtual double EtScaleFactor()         const = 0;
    virtual double EtSmearFactor()            const = 0;
    virtual std::string SumetCorFile()        const = 0;

    virtual bool CalcW()                      const = 0;
    virtual bool CalcWmu()                    const = 0;
    virtual bool CalcJets()                   const = 0;
    virtual bool CalcZmumu()                  const = 0;

    virtual bool DoZjet()                     const = 0;
    virtual bool DoZjetAna()                  const = 0;
    virtual bool DoZplusJet()                  const = 0;
    virtual bool CalcJetCharge()              const = 0;

    virtual bool DoBackgroundChargeAnalysis() const = 0;
    virtual bool DoElectronCounting()         const = 0;

    virtual bool DoTruthMatching()            const = 0;

    virtual bool DoTriggerMatching()          const = 0;

    virtual bool FillNoCuts()                 const = 0;

    virtual bool FillAnalysisChains()         const = 0;

    virtual bool ExecuteAnalysisLoop()        const = 0;
    virtual bool AnalyseEvent()               const = 0;

    virtual bool DoManualGRL()                const = 0;
    virtual std::string ManualGRLFilename()   const = 0;

    virtual std::string ZeeDInlineZ_InFileName()   const = 0;
    virtual int    ZeeDInlineZ_SeedGen()      const = 0;
    virtual int    FirstRunHistogram()        const = 0;
    virtual int    LastRunHistogram()         const = 0;

    virtual bool   AnalysisCutSelectorBookExtraCuts() const = 0;

    virtual double ZeeDInlineZ_ESmear()       const = 0;
    virtual bool   ZeeDInlineZ_UseRadiation() const = 0;

    virtual std::vector<std::string> TTreeFileList() const = 0;

    virtual std::string TTreeOutput()         const = 0;
    virtual int TTreeOutputMaxSize()          const = 0;


    virtual bool   FillGenInfo()              const = 0; // Flag for Reading and filling Gen info
    virtual bool   RunNumberSim()             const = 0;
    virtual int    SelectCalibMethod()        const = 0;
    virtual bool   ReweightSingleParticels()  const = 0;

    virtual std::string CalibrationInFile()        const = 0;
    virtual std::string CalibrationOutFile()       const = 0;
    virtual std::string SmearingFile()             const = 0;
    virtual std::string CalibrationInternalFile()  const = 0;

    virtual bool UseTrigger()                             const = 0;
    virtual std::vector<std::string> TriggerEvent()       const = 0;
    virtual const std::map<std::string, int>* getTriggerMap()  const = 0;
    virtual void setTriggerMap(std::map<std::string, int> trig_map) = 0;
    virtual std::vector<std::string> L1TriggerElecMatch() const = 0;
    virtual std::vector<std::string> L2TriggerElecMatch() const = 0;
    virtual std::vector<std::string> EFTriggerElecMatch() const = 0;



    virtual bool ApplyElecEnergyScaleCorr()                      const = 0;
    virtual bool ApplyElecEnergySmearingCorr()                   const = 0;
    virtual bool ApplyElecIsoCorr()                              const = 0;
    virtual bool ApplyTrigCorr()                                 const = 0;
    virtual bool ApplyPtZReweighting()                           const = 0;
    virtual bool ApplyPDFReweighting()                           const = 0;
    virtual bool ApplyPileupReweighting()                        const = 0;
    virtual bool ApplyNvtxReweighting()                          const = 0;
    virtual bool ApplyZPosVtxReweighting()                       const = 0;
    virtual bool ApplyNTrkVtxReweighting()                       const = 0;  
    virtual bool ApplyCentralIDCorrection()                      const = 0;
    virtual bool ApplyForwardIDCorrection()                      const = 0;
    virtual bool ApplyRecoCorrection()                           const = 0;
    virtual bool ApplyChargeCorrection()                         const = 0;
    virtual bool ApplyLineShapeCorrection()                      const = 0;
    virtual bool ApplyPrivateElecEnergyScaleCorr()               const = 0;
    virtual bool ApplyPrivateElecEnergySmearingCorr()            const = 0;

    virtual bool IsData()                                        const = 0;
    virtual bool IsMC()                                          const = 0;
    virtual bool MCDataAuto()                                    const = 0;
    virtual int  DatasetID()                                     const = 0;

    virtual void SetInputType(ZeeDEnum::InputType::Value value)        = 0;

    virtual ZeeDEnum::InputType::Value InputType()               const = 0;

    virtual void SetDatasetID(int datasetID)                           = 0;

    // helpers
    virtual const std::string print()                            const = 0;            //!< dump object content

    virtual ZeeDPoissonRandomVault* GetPoissonRandomVault()      const = 0;
    virtual bool genBootstrapRnd()                               const = 0;
    virtual int MCSystNumbers()                                  const = 0;

    virtual ZeeDBinGrid* getBinGrid(std::string name)		       = 0;
    virtual ZeeDSimpleBinGrid* getSimpleBinGrid(std::string name)      = 0;

    virtual bool AnalysisCutHistManagerZFillStages()             const = 0;


    virtual bool FillCalibrator()                                const = 0;
    virtual bool FillBkgHistograms()                             const = 0;
    virtual bool FillBitMask()                                   const = 0;
    virtual bool FillObservable()                                const = 0;

    virtual std::string TTreeChainName()                         const = 0;

    virtual bool FillIsolationDecision()                         const = 0;
    virtual ToolHandle<IegammaIsolationMVATopTool> GetIsolationTool()   const = 0;


    virtual ToolHandle<IAthSelectorTool> JetCleaningTool()       const = 0;
    virtual std::vector<std::string> JetCollectionNames()        const = 0;

    virtual ToolHandleArray<IEventEtDensityProvider> EtDensityTools() const  = 0;

    // DO NOT REMOVE THE FOLLOWING LINE!
    //__NEWFUNCTION__

    virtual bool UseZCCElectronFiltration() const = 0;


    virtual ToolHandle<IegammaSwTool> ClusterCorrectionTool() const = 0;


    virtual bool UseNNEMCalibration() const = 0;


    virtual bool DoZCF() const = 0;


    virtual bool DoZCC() const = 0;


    virtual bool CalcEMu() const = 0;

    virtual bool DoPtJetReweighting() const = 0;
    virtual std::string PtJetReweightingFile() const = 0;


    virtual bool AddSingleRejectsHMs() const = 0;


    virtual bool AddDetailHM() const = 0;


    virtual bool ApplyTriggerCorrection() const = 0;
    virtual bool MuonOneBinTriggerSF() const=0;
    virtual bool MuonChargedTriggerSF() const=0;
    virtual std::string TriggerEfficiencyElectronFileName() const = 0;


    virtual std::string IdentificationEfficiencyElectronMediumFileName() const = 0;
    virtual std::string IdentificationEfficiencyElectronTightFileName() const = 0;
     virtual std::string IdentificationEfficiencyElectronLooseFileName() const = 0;
   
    virtual std::string IdentificationEfficiencyElectronFwdTightFileName() const = 0;

    virtual std::string ReconstructionEfficiencyElectronFileName() const = 0;

    virtual std::string TriggerEfficiencyMuonFileName() const = 0;


    virtual std::string IdentificationEfficiencyMuonMediumFileName() const = 0;

    virtual std::string IdentificationEfficiencyMuonFwdTightFileName() const = 0;

    virtual std::string ReconstructionEfficiencyMuonFileName() const = 0;



    virtual bool EvalSysExtraCrackCut() const = 0;


    virtual std::string ElectronMCChargeCorrectorName() const = 0;


    virtual std::string IDFwdCorrectionTightName() const = 0;


    virtual std::string IDFwdCorrectionMediumName() const = 0;


    virtual std::string IDFwdCorrectionLooseName() const = 0;


    virtual std::string IDCorrectionTightPPName() const = 0;


    virtual std::string IDCorrectionMediumPPName() const = 0;


    virtual bool ApplyGenBosMassCut() const = 0;


    virtual bool GenBosMassMirrorCut() const = 0;


    virtual bool GenBosMassCutContributesToGenWeight() const = 0;


    virtual double ExtraCrackCutMax() const = 0;


    virtual double ExtraCrackCutMin() const = 0;


    virtual std::string MCG4SimTag() const = 0;

    virtual std::string ZvtxRewFile() const = 0;

    virtual bool ExtraCrackCut() const = 0;

    virtual bool CalcEIso() const = 0;


    virtual bool FillBitsHists() const = 0;


    virtual bool MinimalHistoMode() const = 0;


    virtual bool EvalSysTrigCorrMC() const = 0;


    virtual bool EvalSysRecoCorrectionMC() const = 0;


    virtual bool EvalSysElecIsoCorrectionMC() const = 0;


    virtual bool EvalSysForwardIDCorrectionMC() const = 0;


    virtual bool EvalSysCentralIDCorrectionMC() const = 0;


    virtual bool WriteEWUnfoldNtuple() const = 0;


    virtual bool EvalSysElecEnergyScaleCorr() const = 0;


    virtual bool EvalSysSFMC() const = 0;

    virtual bool EvalSysEtMiss() const = 0;

    virtual unsigned int nTrkVertexJets() const = 0;


    virtual std::string RecalibrateJetsConfig() const = 0;


    virtual bool RecalibrateJets() const = 0;

    virtual double GenBosMassMin() const = 0;


    virtual double GenBosMassMax() const = 0;


    virtual std::string BinGridZCC2011() const = 0;


    virtual bool EvalSysPileupReweighting() const = 0;


    virtual bool EvalSysZPosVtxReweighting() const = 0;


    virtual bool EvalSysTrigCorr() const = 0;


    virtual bool EvalSysNvtxReweighting() const = 0;


    virtual bool EvalSysChargeCorrection() const = 0;


    virtual bool EvalSysRecoCorrection() const = 0;
    virtual bool EvalSysLineShapeCorrection() const = 0;

    virtual bool EvalSysElecIsoCorr() const = 0;


    virtual bool EvalSysForwardIDCorrection() const = 0;


    virtual bool EvalSysCentralIDCorrection() const = 0;


    virtual bool EvalSysPtZReweighting() const = 0;


    virtual bool EvalSysElecEnergySmearingCorr() const = 0;


    virtual std::vector<std::string> BosonPtReweightingAdditionalTargets() const = 0;


    virtual std::string BosonPtReweightingTarget() const = 0;

    virtual bool BookYBinnedExtraPlots() const = 0;


    virtual bool BookMuShelves() const = 0;



    virtual double dressedElectronsDeltaR() const = 0;

    virtual bool BookBackgroundCuts() const = 0;


    virtual bool RunZBaselineAna() const = 0;


    virtual bool IsInlineZ() const = 0;



    virtual double EtaCrackEMECFCAL_Max() const = 0;


    virtual double EtaCrackEMECFCAL_Min() const = 0;


    virtual int PDFSetNumber() const = 0;


    virtual bool CalcZ() const = 0;



    virtual bool RunZLowMassAna() const = 0;




    virtual bool RunZCFAna() const = 0;


    virtual bool RunZCCAna() const = 0;


    virtual bool BookInitialStateShelves() const = 0;


    virtual bool MCAssignRandomRunNumber() const = 0;


    virtual bool MCWithConstantTerm() const = 0;


    virtual bool DoSinglePeriods() const = 0;


    virtual bool EvaluateSystematics() const = 0;


    virtual std::string MCCampaign() const = 0;


    virtual bool WriteTTreeWithCutHistManager() const = 0;


    virtual bool GenPileupMCFile() const = 0;


    virtual std::string PileupMCGenFile() const = 0;


    virtual std::string PileupMCFile() const = 0;


    virtual std::string PileupDataFile() const = 0;

    virtual int SystematicsDetailed() const = 0;

    virtual bool DoCombToyMC() const = 0;

    virtual std::string JESUncertaintiesConfig() const = 0;
   
    virtual std::string MultiJESUncertaintiesConfig() const = 0;
    
    virtual std::string PathDirUncertaintiesConfig() const = 0;
    virtual bool DoBCHCleaning() const = 0;
   
    virtual std::string TTileTripReaderFile() const = 0;

    virtual std::string BCHCleaningFile() const = 0;

    virtual std::string JERInputFile() const = 0;



};

