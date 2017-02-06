// include header file
#include "ZeeDAnalysisSvc/ZeeDAnalysisSvc.h"

// ROOT includes
#include "TTree.h"
#include "TSystem.h"
#include <TRandom3.h>

// athena includes
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/ServiceHandle.h"

// needed for tool svc
#include "GaudiKernel/IToolSvc.h"

// STL includes
#include <iostream>
#include <sstream>
#include <iomanip>

using std::cout;
using std::endl;

//------------------------------------------------------
// default constructor
ZeeDAnalysisSvc::ZeeDAnalysisSvc(const std::string& name, ISvcLocator* sl)
    : AthService(name, sl),
      fDatasetID(0),
      fPoissonVault(NULL)
{
    declareProperty("Run_Min"		      ,	fRun_Min		 = 190120		   , "Run range: min run number");
    declareProperty("Run_Max"		      ,	fRun_Max		 = 190120		   , "Run range: max run number");

    declareProperty("Elec_EtaFwdMin"          , fElec_EtaFwdMin          = 2.5                     , "Electron forward eta cut");
    declareProperty("Elec_EtaFwdMax"          , fElec_EtaFwdMax          = 4.9                     , "Electron forward eta cut");

    declareProperty("Elec_EtaCut"             , fElec_EtaCut             = 2.37                    , "Electron eta cut");
    declareProperty("Elec_PtCut"              , fElec_PtCut              = 15.0                    , "Electron pt cut");
    declareProperty("Elec_CentralPtMinCut"    , fElec_CentralPtMinCut    = 15.0                    , "Central Electron pt cut");
    declareProperty("Elec_ForwardPtMinCut"    , fElec_ForwardPtMinCut    = 15.0                    , "Forward Electron pt cut");
    declareProperty("Z_PtCut"                 , fZ_PtCut                 = 120.0                   , "Z pt cut");
    declareProperty("Elec_DZVtxCut"           , fElec_DZVtxCut           = 2.0                     , "D Zvtx Electron cut");

    declareProperty("Jet_PtCut"               , fJet_PtCut               = 25                      , "Jet Pt cut");
    declareProperty("Jet_EtaCut"              , fJet_EtaCut              = 2.5                     , "Jet Eta cut");

    declareProperty("ZMassCutLooseMin"        , fZMassCutLooseMin        = 70.0                    , "ee Invariant mass cut low");
    declareProperty("ZMassCutLooseMax"        , fZMassCutLooseMax        = 110.0                   , "ee Invariant mass cut high");

    declareProperty("ZMassCutVeryLooseMin"    , fZMassCutVeryLooseMin    = 50.0                    , "ee Invariant mass cut low");
    declareProperty("ZMassCutVeryLooseMax"    , fZMassCutVeryLooseMax    = 130.0                   , "ee Invariant mass cut high");

    declareProperty("ZLowMassCutMin"          , fZLowMassCutMin          = 20.0		           , "ee Invariant mass cut low");
    declareProperty("ZLowMassCutMax"          , fZLowMassCutMax          = 70.0		           , "ee Invariant mass cut high");

    declareProperty("EtaCrackEMBEMEC_Min"     , fEtaCrackEMBEMEC_Min     = 1.37                    , "EMB-EMEC crack min");
    declareProperty("EtaCrackEMBEMEC_Max"     , fEtaCrackEMBEMEC_Max     = 1.52                    , "EMB-EMEC crack max");

    declareProperty("PriVertexZCut"           , fPriVertexZCut           = 150.0                   , "Primary Vertex Z-position cut");

    declareProperty("MinNumTrkCut"            , fMinNumTrkCut            = 3.0                     , "Minimal number of tracks in the good vertex");
    declareProperty("MinNumVertexCut"         , fMinNumVertexCut         = 3.0                     , "Minimal number of good vertices in the event");

    declareProperty("CaloClusterContainer"    , fCaloClusterContainer    = "egClusterCollection"   , "Main cluster container");
    declareProperty("ElectronContainer"       , fElectronContainer       = "ElectronAODCollection" , "Main electron container");

    declareProperty("PDFFileName"             , fPDFFileName             = "PDF/HERAPDF_MUATLAS.LHgrid" , "LHAPDF file name for reweight");

    declareProperty("Source"                  , fSource                  = "TTree"                   , "AOD, Inline, D3PD, ...");
    declareProperty("MCtype"                  , fMCtype                  = ""                      , "Pythia, MCNLO ...");

    declareProperty("ReadEtMiss"                    , bReadEtMiss                   = false        , "");
    declareProperty("RecalcEtMiss"                  , bRecalcEtMiss                 = false        , "");
    declareProperty("HadrRecoilFull"                , bHadrRecoilFull               = false        , "");
    declareProperty("DeleteSoftTerm"                , bDeleteSoftTerm               = false        , "");
    declareProperty("CalcW"                         , bCalcW                        = false        , "");
    declareProperty("CalcWmu"                       , bCalcWmu                      = false        , "");
    declareProperty("CalcJets"                      , bCalcJets                     = false        , "");
    declareProperty("CalcZmumu"                     , bCalcZmumu                    = false        , "");
 
    declareProperty("DoZjet"                        , bDoZjet                       = false        , "");
    declareProperty("DoZjetAna"                     , bDoZjetAna                    = false        , "");
    declareProperty("DoZplusJet"                    , bDoZplusJet                    = false        , "");
    declareProperty("CalcJetCharge"                 , bCalcJetCharge                = false        , "");

    declareProperty("DoBackgroundChargeAnalysis"    , bDoBackgroundChargeAnalysis   = false        , "");
    declareProperty("DoElectronCounting"            , bDoElectronCounting           = false        , "");
    declareProperty("DoTruthMatching"               , bDoTruthMatching              = true         , "");
    declareProperty("DoTriggerMatching"             , bDoTriggerMatching            = true         , "");
    declareProperty("FillNoCuts"                    , bFillNoCuts                   = true         , "");
    declareProperty("FillAnalysisChains"            , bFillAnalysisChains           = true         , "");

    declareProperty("ExecuteAnalysisLoop"           , bExecuteAnalysisLoop          = true         , "");
    declareProperty("AnalyseEvent"                  , bAnalyseEvent                 = true         , "");
    declareProperty("DoManualGRL"                   , bDoManualGRL                  = false        , "");
    declareProperty("ManualGRLFilename"             , fManualGRLFilename            = ""           , "");

    declareProperty("ZeeDInlineZ_InFileName"  , fInline_InFileName       = "Inline/ZGen.root"    , "Name of file with histograms");
    declareProperty("ZeeDInlineZ_SeedGen"     , fInline_SeedGen          = 1                       , "Seed for ramfom generator");
    declareProperty("ZeeDInlineZ_ESmear"      , fInline_ESmear           = 0.03                    , "Smiring for energy of electron");
    declareProperty("ZeeDInlineZ_UseRadiation", fInline_UseRadiation     = true                    , "Flag for include radiation");

    declareProperty("TTreeFileList"           , fTTreeFileList                                     , "List of input TTree files");

    declareProperty("TTreeOutput"             , fTTreeOutput             = ""                      , "Name of output TTree file (empty for disable)");
    declareProperty("TTreeOutputMaxSize"      , fTTreeOutputMaxSize      = 0                       , "Maximum size per one output files (0 for infinite)");


    declareProperty("UseSystematics"          , fUseSystematics          = true                    , "Switch on / off systematics shifts");
    declareProperty("FillGenInfo"             , fFillGenInfo             = true                    , "Switch on / off Reading Gen info");
    declareProperty("ReweightSingleParticels" , fReweightSingleParticels = false                   ,
                    "Switch on / off reweighting of Pt for single particles");
    declareProperty("SelectCalibMethod"       , fSelectCalibMethod       = 4                       , "Select  calibration method ");

    declareProperty("CalibrationInFile"       , fCalibrationInFile       = "Calib/calib.in"              , "Calibration input file");
    declareProperty("CalibrationOutFile"      , fCalibrationOutFile      = "Calib/calib.out"             , "Calibration output file");
    declareProperty("SmearingFile"            , fSmearingFile            = "Calib/smear.in"              , "Smearing input file");
    declareProperty("CalibrationInternalFile" , fCalibrationInternalFile = "Calib/calib.intern.out"      , "Calibration internal output file");

    declareProperty("FirstRunHistogram", fFirstRunHistogram = -1, " First run for the yield histogram");
    declareProperty("LastRunHistogram", fLastRunHistogram = -1, " Last run for the yield histogram");


    declareProperty("AnalysisCutSelectorBookExtraCuts",fAnalysisCutSelectorBookExtraCuts = false, "Book extra, beyond baseline, cuts for additional studies");

    declareProperty("UseTrigger"              , fUseTrigger              = false                   , "Flag switch on trigger");
    declareProperty("TriggerEvent"            , fTriggerEvent                                      , "List of trigger to be checked if event has passed");
    declareProperty("L1TriggerElecMatch"      , fL1TriggerElecMatch                                , "List of trigger L1 chain names for electron matching");
    declareProperty("L2TriggerElecMatch"      , fL2TriggerElecMatch                                , "List of trigger L2 chain names for electron matching");
    declareProperty("EFTriggerElecMatch"      , fEFTriggerElecMatch                                , "List of trigger EF chain names for electron matching");


    declareProperty("ApplyElecEnergyScaleCorr"   , fApplyElecEnergyScaleCorr      = true   , "Flag to switch official energy correction");
    declareProperty("ApplyElecEnergySmearingCorr", fApplyElecEnergySmearingCorr   = false  , "Flag to switch official MC smearing correction");
    declareProperty("ApplyElecIsoCorr"           , fApplyElecIsoCorr              = true   , "Flag to switch official MC Isolation efficiency correction");
    declareProperty("ApplyTrigCorr"              , fApplyTrigCorr                 = true   , "Flag to switch official Trigger correction");
    declareProperty("ApplyPtZReweighting"        , fApplyPtZReweighting           = true   , "Flag to switch official Pt reweighting ");
    declareProperty("ApplyPDFReweighting"        , fApplyPDFReweighting           = false  , "Flag to switch PDF weight");
    declareProperty("ApplyPileupReweighting"     , fApplyPileupReweighting        = false  , "Flag to switch Pileup weight");
    declareProperty("ApplyNvtxReweighting"       , fApplyNvtxReweighting          = false  , "Flag switch reweight for MC Vtx");
    declareProperty("ApplyNTrkVtxReweighting"    , fApplyNTrkVtxReweighting       = false  , "Flag switch reweight for MC NTrkVtx");
    declareProperty("ApplyZPosVtxReweighting"    , fApplyZPosVtxReweighting       = false  , "Flag switch reweight for Zv distribution");
    declareProperty("ApplyForwardIDCorrection"   , fApplyForwardIDCorrection      = false  , "Flag to switch ID correction for forward electron ");
    declareProperty("ApplyCentralIDCorrection"   , fApplyCentralIDCorrection      = false  , "Flag to switch ID correction for central electron ");
    declareProperty("ApplyRecoCorrection"        , fApplyRecoCorrection           = false  , "Flag to switch Reco correction for electron ");
    declareProperty("ApplyChargeCorrection"      , fApplyChargeCorrection         = false  , "Flag to switch charge correction of the electron ");
    declareProperty("ApplyLineShapeCorrection"   , fApplyLineShapeCorrection      = false  , "Flag to switch Line Shape correction ");
    declareProperty("ApplyPrivateElecEnergyScaleCorr"   , fApplyPrivateElecEnergyScaleCorr = false  , "Flag to switch private zeed additional energy correction");
    declareProperty("ApplyPrivateElecEnergySmearingCorr", fApplyPrivateElecEnergySmearingCorr = false  , "Flag to switch private ZeeD MC smearing correction");


    declareProperty("MCDataAuto"                 , fMCDataAuto = true                      , "Flag to switch MC/Data auto detection");
    declareProperty("InputType"                  , fInputProperty = "data"                 , "Flag to switch MC/Data manually");

    declareProperty("PoissonVaultDimension"   , nPoissonNumbers          = 0                        , "Flag to store per-event generated Poisson random numbers");
    declareProperty("MCSystNumbers"           , nMCSystNumbers           = 0                        , "Flag to store per-event tables for MC systematics");
    declareProperty("DoCombToyMC"             , fDoCombToyMC             = false                    , "Flag to switch on combined ToyMC method");

    declareProperty("AnalysisCutHistManagerZFillStages"     , fAnalysisCutHistManagerZFillStages     = true   , "Flag to book histmanagers for all stages");
    declareProperty("FillCalibrator"                      , fFillCalibrator                      = true   , "Flag to book histmanagers for Calibration");
    declareProperty("FillBkgHistograms"                   , fFillBkgHistograms                   = true   , "Flag to book histmanagers for Bkg Studies");
    declareProperty("FillBitMask"                         , fFillBitMask                         = false  , "Flag to book histmanagers with full bit mask (for cut flow) ");
    declareProperty("FillObservable"                      , fFillObservable                      = false  , "Flag to book ZeeDObservable hist manager ");

    declareProperty("TTreeChainName"          , fTTreeChainName = ""                                , "Name of the TTree file to read");

    declareProperty("FillIsolationDecision"   , bFillIsolationDecision                           = true, "Fill Calorimeter Isolation Cut Decision");
    declareProperty("SystematicsDetailed"     , fSystematicsDetailed                             = 0, "Detail level of the systematics calculation");

    declareProperty("EgammaIsolationMVATopTool", f_egammaIsolationMVATopTool );



    declareProperty("JetCleaningTool", fJetCleaningTool = 0 , "The jet cleaning tool");
    declareProperty("JetCollectionNames", fJetCollectionNames  , " Jet collections names used in the analysis");

    declareProperty("ZeeDEtDensityTools", fEtDensityTools = 0 , "Tools to calculate cluster  Et density in an event");

    // DO NOT REMOVE THE FOLLOWING LINE!
    //__NEWDECLAREPROPERTY__

    declareProperty("UseZCCElectronFiltration", fUseZCCElectronFiltration = false, "Use experimental ZCC filter");


    declareProperty("ClusterCorrectionTool", fClusterCorrectionTool = 0, "");


    declareProperty("UseNNEMCalibration", fUseNNEMCalibration = false, "switch new EM calibration");


    declareProperty("DoZCF", fDoZCF = false, "Do ZCF Analysis?");


    declareProperty("DoZCC", fDoZCC = false, "Do ZCC Analysis?");


    declareProperty("CalcEMu", fCalcEMu = true, "Include EMu fake bosons into analysis");


    declareProperty("AddSingleRejectsHMs", fAddSingleRejectsHMs = false, "Add several HM for events failing one cut");


    declareProperty("AddDetailHM", fAddDetailHM = false, "Add electron detailes hist manager");


    declareProperty("ApplyTriggerCorrection", fApplyTriggerCorrection = true, "Apply trigger correction");


    declareProperty("TriggerEfficiencyElectronFileName", fTriggerEfficiencyElectronFileName = "efficiencySF.e24vhi_medium1_e60_medium1.Tight.2012.8TeV.rel17p2.v01.root", "File name for the electron trigger efficiency (package ElectronEfficiencyCorrection)");


    declareProperty("IdentificationEfficiencyElectronMediumFileName", fIdentificationEfficiencyElectronMediumFileName = "efficiencySF.offline.Medium.2012.8TeV.rel17p2.v02.root", "File name for the electron identification efficiency (package ElectronEfficiencyCorrection)");
    declareProperty("IdentificationEfficiencyElectronTightFileName", fIdentificationEfficiencyElectronTightFileName = "efficiencySF.offline.Tight.2012.8TeV.rel17p2.v02.root", "File name for the electron identification efficiency (package ElectronEfficiencyCorrection)");
    declareProperty("IdentificationEfficiencyElectronLooseFileName", fIdentificationEfficiencyElectronLooseFileName = "efficiencySF.offline.Tight.2012.8TeV.rel17p2.v02.root", "File name for the electron identification efficiency (package ElectronEfficiencyCorrection)");


    declareProperty("IdentificationEfficiencyElectronFwdTightFileName", fIdentificationEfficiencyElectronFwdTightFileName = "efficiencySF.offline.FwdTight.2012.8TeV.rel17p2.v07.root", "File name for the electron identification efficiency (package ElectronEfficiencyCorrection)");

    declareProperty("ReconstructionEfficiencyElectronFileName", fReconstructionEfficiencyElectronFileName = "efficiencySF.offline.RecoTrk.2012.8TeV.rel17p2.v01.root", "File name for the electron reconstruction efficiency (package ElectronEfficiencyCorrection)");

    declareProperty("TriggerEfficiencyMuonFileName", fTriggerEfficiencyMuonFileName = "efficiencySF.e24vhi_medium1_e60_medium1.Tight.2012.8TeV.rel17p2.v01.root", "File name for the muon trigger efficiency (package ElectronEfficiencyCorrection)");


    declareProperty("IdentificationEfficiencyMuonMediumFileName", fIdentificationEfficiencyMuonMediumFileName = "efficiencySF.offline.Medium.2012.8TeV.rel17p2.v02.root", "File name for the muon identification efficiency (package ElectronEfficiencyCorrection)");

    declareProperty("IdentificationEfficiencyMuonFwdTightFileName", fIdentificationEfficiencyMuonFwdTightFileName = "efficiencySF.offline.FwdTight.2012.8TeV.rel17p2.v07.root", "File name for the muon identification efficiency (package ElectronEfficiencyCorrection)");

    declareProperty("ReconstructionEfficiencyMuonFileName", fReconstructionEfficiencyMuonFileName = "efficiencySF.offline.RecoTrk.2012.8TeV.rel17p2.v01.root", "File name for the muon reconstruction efficiency (package ElectronEfficiencyCorrection)");


    declareProperty("EvalSysExtraCrackCut", fEvalSysExtraCrackCut = false, "");


    declareProperty("ElectronMCChargeCorrectorName", fElectronMCChargeCorrectorName = "MC11c_G4FullSim/MediumPP", "Name to find the appropriate correction");


    declareProperty("IDFwdCorrectionTightName", fIDFwdCorrectionTightName = "data11_7TeV_rel170_allyear_FwdTight", "Name to find the appropriate correction");


    declareProperty("IDFwdCorrectionMediumName", fIDFwdCorrectionMediumName = "data11_7TeV_rel170_allyear_FwdMedium", "Name to find the appropriate correction");


    declareProperty("IDFwdCorrectionLooseName", fIDFwdCorrectionLooseName = "data11_7TeV_rel170_allyear_FwdLoose", "Name to find the appropriate correction");


    declareProperty("IDCorrectionTightPPName", fIDCorrectionTightPPName = "data11_7TeV_rel170_allyear_TightPP", "Name to find the appropriate correction");


    declareProperty("IDCorrectionMediumPPName", fIDCorrectionMediumPPName = "data11_7TeV_rel170_allyear_MediumPP", "Name to find the appropriate correction");


    declareProperty("ApplyGenBosMassCut", fApplyGenBosMassCut = false, "");


    declareProperty("GenBosMassMirrorCut", fGenBosMassMirrorCut = false, "");


    declareProperty("GenBosMassCutContributesToGenWeight", fGenBosMassCutContributesToGenWeight = true, "");


    declareProperty("ExtraCrackCutMax", fExtraCrackCutMax = 1.7, "");


    declareProperty("ExtraCrackCutMin", fExtraCrackCutMin = 1.6, "");



    declareProperty("MCG4SimTag", fMCG4SimTag = "s1370", "Type of MC in respect to Zvtx reweghing");


    declareProperty("ZvtxRewFile", fZvtxRewFile = "zvtx_weights_2011_2012.root", "Zvtx reweighing data file");

    declareProperty("ExtraCrackCut", fExtraCrackCut = false, "");



    declareProperty("CalcEIso", fCalcEIso = false, "Calculate electron calorimeter isolation with uniform efficiency");


    declareProperty("FillBitsHists", fFillBitsHists = false, "Fill bits hists");


    declareProperty("MinimalHistoMode", fMinimalHistoMode = true, "");


    declareProperty("EvalSysTrigCorrMC", fEvalSysTrigCorrMC = false, "Evaluate uncorrelated systematics of trigger using Toy MCs");


    declareProperty("EvalSysRecoCorrectionMC", fEvalSysRecoCorrectionMC = false, "Evaluate uncorrelated systematics of electron reconstruction using Toy MCs");


    declareProperty("EvalSysElecIsoCorrectionMC", fEvalSysElecIsoCorrectionMC = false, "Evaluate uncorrelated systematics of electron isolation using Toy MCs");


    declareProperty("EvalSysForwardIDCorrectionMC", fEvalSysForwardIDCorrectionMC = false, "Evaluate uncorrelated systematics of forward electron identification using Toy MCs");


    declareProperty("EvalSysCentralIDCorrectionMC", fEvalSysCentralIDCorrectionMC = false, "Evaluate uncorrelated systematics of central electron identification using Toy MCs");


    declareProperty("WriteEWUnfoldNtuple", fWriteEWUnfoldNtuple = false, "Write EWUnfold ntuples");


    declareProperty("EvalSysElecEnergyScaleCorr", fEvalSysElecEnergyScaleCorr = true, "Evaluate electron energy scale systematic");


    declareProperty("EvalSysSFMC", fEvalSysSFMC = true, "Evaluate scale factor systematics using ToyMC");

    declareProperty("EvalSysEtMiss", fEvalSysEtMiss= true, "Evaluate EtMiss systematics"); 

    declareProperty("nTrkVertexJets", fNTrkVertexJets = 2, "Number of tracks associated to a vertex. Needed for consistency with the cut in JetCalibration");


    declareProperty("RecalibrateJetsConfig", fRecalibrateJetsConfig = "", "Config file for recalibration. Needs to be specified explicitly");


    declareProperty("RecalibrateJets", fRecalibrateJets = false, "Flag to recalibrate jets on fly. Needs config file specified");

    declareProperty("GenBosMassMin", fGenBosMassMin = 0, "Set gen. weight to 0 if generated boson mass is lower");


    declareProperty("GenBosMassMax", fGenBosMassMax = -1, "Set gen. weight to 0 if generated boson mass is higher");


    declareProperty("BinGridZCC2011", fBinGridZCC2011 = "Binning/BosYMassCosTh.txt", "Binning for 2011 ZCC analysis");


    declareProperty("EvalSysPileupReweighting", fEvalSysPileupReweighting = true, "Evaluate pileup reweighting systematic");


    declareProperty("EvalSysZPosVtxReweighting", fEvalSysZPosVtxReweighting = true, "Evaluate vertex z position reweighting systematic");


    declareProperty("EvalSysTrigCorr", fEvalSysTrigCorr = true, "Evaluate trigger systematic");


    declareProperty("EvalSysChargeCorrection", fEvalSysChargeCorrection = true, "Evaluate electron charge systematic");


    declareProperty("EvalSysLineShapeCorrection", fEvalSysLineShapeCorrection = true, "Evaluate Line Shape systematic");


    declareProperty("EvalSysRecoCorrection", fEvalSysRecoCorrection = true, "Evaluate electron reconstruction systematic");


    declareProperty("EvalSysElecIsoCorr", fEvalSysElecIsoCorr = true, "Evaluate electron isolation systematic");


    declareProperty("EvalSysForwardIDCorrection", fEvalSysForwardIDCorrection = true, "Evaluate forward electron identification systematic");


    declareProperty("EvalSysCentralIDCorrection", fEvalSysCentralIDCorrection = true, "Evaluate central electron identification systematic");


    declareProperty("EvalSysPtZReweighting", fEvalSysPtZReweighting = true, "Evaluate boson pt reweighting systematic");


    declareProperty("EvalSysElecEnergySmearingCorr", fEvalSysElecEnergySmearingCorr = true, "Evaluate electron energy smearing systematic");


    declareProperty("BosonPtReweightingAdditionalTargets", fBosonPtReweightingAdditionalTargets, "Additional generators used as target for weighting - evaluated as systematics");


    declareProperty("BosonPtReweightingTarget", fBosonPtReweightingTarget = "PowhegPythia8MC11", "Generator used as target for weighting");


    declareProperty("BookYBinnedExtraPlots", fBookYBinnedExtraPlots = false, "Flag to book extra plots, binned in Y boson rapidity bins");


    declareProperty("BookMuShelves", fBookMuShelves = false, "Book shelves of mu ranges");



    declareProperty("dressedElectronsDeltaR", fDressedElectronsDeltaR = 0.1, "Delta R definition for dressed electrons");

    declareProperty("BookBackgroundCuts", fBookBackgroundCuts = false, "Book cuts used for background selection");



    declareProperty("RunZBaselineAna", fRunZBaselineAna = true, "Run Z baseline Analysis");


    declareProperty("EtaCrackEMECFCAL_Max", fEtaCrackEMECFCAL_Max = 3.35, "Cut on upper edge of the EMEC-FCAL crack");


    declareProperty("EtaCrackEMECFCAL_Min", fEtaCrackEMECFCAL_Min = 3.16, "Cut on low edge of the EMEC-FCAL crack");


    declareProperty("PDFSetNumber", fPDFSetNumber = 0, "PDF Set Number used for reweighting");

    declareProperty("CalcZ", fCalcZ = true, "Calculate Z from electrons");



    declareProperty("RunZLowMassAna", fRunZLowMassAna = true, "Run Z low mass Analysis");




    declareProperty("RunZCFAna", fRunZCFAna = true, "Run Z central-forward Analysis");


    declareProperty("RunZCCAna", fRunZCCAna = true, "Run Z central-central Analysis");



    declareProperty("BookInitialStateShelves", fBookInitialStateShelves = false, "book one shelf per (qqbar) initial state");


    declareProperty("MCAssignRandomRunNumber", fMCAssignRandomRunNumber = false, "assign each MC event its data-period (a data run-number corresponding to the contribution of each run to the total luminosity)");


    declareProperty("MCWithConstantTerm", fMCWithConstantTerm = false, "set if MC sample has calorimeter constant term on or off");


    declareProperty("DoSinglePeriods", fDoSinglePeriods = false, "Book shelves for all single (D,E,...) periods");


    declareProperty("EvaluateSystematics", fEvaluateSystematics = false, "Evaluate systematic shifts");


    declareProperty("MCCampaign", fMCCampaign = "mc11b", "MC production campaign, like mc11a, mc11b, etc.");


    declareProperty("WriteTTreeWithCutHistManager", fWriteTTreeWithCutHistManager = false, "Write TTree with the help of the CutHistManagerWriter");


    declareProperty("GenPileupMCFile", fGenPileupMCFile = true, "MC file generating mode");


    declareProperty("PileupMCGenFile", fPileupMCGenFile = "", "File to which the generated MC distribution of the PileupReweighting tool will be written to");


    declareProperty("PileupMCFile", fPileupMCFile = "", "File containing MC distribution for PileupReweighting tool");


    declareProperty("PileupDataFile", fPileupDataFile = "", "File containing data distribution for PileupReweighting tool");


    declareProperty("JESUncertaintiesConfig", fJESUncertaintiesConfig = "", "Config file for JES uncertainties");
    declareProperty("MultiJESUncertaintiesConfig", fMultiJESUncertaintiesConfig = "", "Config file for MultiJES uncertainties");
    declareProperty("PathDirUncertaintiesConfig", fPathDirUncertaintiesConfig = "", "path to JetUncertainties/share/ directory");
    declareProperty("DoPtJetReweighting", fDoPtJetReweighting = false, "PT(jet) reweighting (QG analysis)");
    declareProperty("PtJetReweightingFile", fPtJetReweightingFile = "", "path to the file for pT(jet) reweighting (QG analysis)");
    declareProperty("DoBCHCleaning", fDoBCHCleaning = false, "Flag to switch BCH Cleaning for jets");
    declareProperty("TTileTripReaderFile", fTTileTripReaderFile = "", "File name for TTileTripReader histograms");
    declareProperty("BCHCleaningFile", fBCHCleaningFile = "", "File name for BCHCleaning histograms");

    declareProperty("JERInputFile", fJERInputFile = "", "File name for JER systematics calculation");



}

//------------------------------------------------------
ZeeDAnalysisSvc::~ZeeDAnalysisSvc()
{
    // Default destructor
}

//------------------------------------------------------
StatusCode ZeeDAnalysisSvc::initialize()
{
    // Initialization

    MsgStream log(msgSvc(), name());
    log << MSG::INFO << "Initializing " << name()
        << " - package version " << PACKAGE_VERSION << endreq;

    fInput = ZeeDMisc::strToInputType(fInputProperty);

    const TString baseDirectory(fBaseDirectory);

    if ( fSource == "AOD" ){
        StatusCode sc = f_egammaIsolationMVATopTool.retrieve();

        if ( sc.isFailure() ) {
            Error("ZeeDAnalysisSvc::initialize", "Can't get a handle on the IsolationMVATool");
            gSystem->Exit(EXIT_FAILURE);
        }

        // trigger map setup
        // if AOD is used make the map,
        // else leave it to the TTree reader
        for (unsigned int itrig = 0; itrig < fTriggerEvent.size(); ++itrig) {
//            fTriggerMap[fTriggerEvent.at(itrig)] = itrig;  // FIXME: this is how it should be
            fTriggerMap[fTriggerEvent.at(itrig)] = ZeeDEnum::Trigger::ToEnum(fTriggerEvent.at(itrig).c_str());  // FIXME: save the enum values for now


        }
    }

    if (nPoissonNumbers > 0) {
        fPoissonVault = new ZeeDPoissonRandomVault(nPoissonNumbers);
    }

    // list all properties
    Info("ZeeDAnalysisSvc::initialize", ("ZeeDAnalysisSvc::initialize list of propiertes start \n"+ this->print()).c_str() );
    Info("ZeeDAnalysisSvc::initialize", "ZeeDAnalysisSvc::initialize list of propiertes end " );

    return StatusCode::SUCCESS;
}

//------------------------------------------------------
ZeeDBinGrid* ZeeDAnalysisSvc::getBinGrid(std::string name)
{
  // returns bin grid for the given name
  // avoiding recreation of identical bin grids
  // for different replicas of the analysis

  std::map<std::string, ZeeDBinGrid*>::const_iterator itgrid = binGrids.find(name);

  if (itgrid == binGrids.end()) {
    ZeeDBinGrid* binGrid = new ZeeDBinGrid(name);
    binGrid->ReadFromFile(ZeeDMisc::FindFile(name), kFALSE);
    binGrids[name] = binGrid;
    return binGrid;
  } else {
    return itgrid->second; 
  }
}

//------------------------------------------------------
ZeeDSimpleBinGrid* ZeeDAnalysisSvc::getSimpleBinGrid(std::string name)
{
  // returns bin grid for the given name
  // avoiding recreation of identical bin grids
  // for different replicas of the analysis

  std::map<std::string, ZeeDSimpleBinGrid*>::const_iterator itgrid = simpleBinGrids.find(name);

  if (itgrid == simpleBinGrids.end()) {
    ZeeDSimpleBinGrid* simpleBinGrid = new ZeeDSimpleBinGrid();
    simpleBinGrid->ReadFromFile(ZeeDMisc::FindFile(name));
    simpleBinGrids[name] = simpleBinGrid;
    return simpleBinGrid;
  } else {
    return itgrid->second; 
  }
}

//------------------------------------------------------
StatusCode ZeeDAnalysisSvc::finalize()
{
    MsgStream log(msgSvc(), name());
    log << MSG::INFO << "Finalizing ZeeD analysis service. " << endreq;
    return StatusCode::SUCCESS;
}


//------------------------------------------------------
StatusCode ZeeDAnalysisSvc::queryInterface(const InterfaceID& riid, void** ppvIF)
{
    // Query interface
    if ( IID_IZeeDAnalysisSvc == riid ) {
        *ppvIF = dynamic_cast<IZeeDAnalysisSvc*> (this);
    } else {
        return AthService::queryInterface(riid, ppvIF);
    }

    addRef();
    return StatusCode::SUCCESS;
}


//------------------------------------------------------
const std::string ZeeDAnalysisSvc::print() const
{
    std::ostringstream s;

    s << " ------------------------------------------------------------------- \n";
    s << " *** *** This is the object of type "   << this->name() << " *** *** \n";
    s << " ------------------------------------------------------------------- \n \n";

    s << " Values of Kinematics cuts:                                             \n \n";
    s << " Electron forward eta max = "           << this->Elec_EtaFwdMax() << "\n";
    s << " Electron forward eta min = "           << this->Elec_EtaFwdMin() << "\n";
    s << " Electron eta = "                       << this->Elec_EtaCut()    << "\n";
    s << " and pt = "                             << this->Elec_PtCut()     << "\n";
    s << " Central Elec pt = "                    << this->Elec_CentralPtMinCut()     << "\n";
    s << " Forward Elec pt = "                    << this->Elec_ForwardPtMinCut()     << "\n";
    s << " and Z pt = "                           << this->Z_PtCut()        << "\n";
    s << " Eta crack cut: "                       << EtaCrackEMBEMEC_Min();
    s << "< |eta| <"                              << EtaCrackEMBEMEC_Max()         << std::endl;
    s << " Z mass loose cuts: "                   << this->ZMassCutLooseMin();
    s << " < M(ee) < "                            << this->ZMassCutLooseMax()      << std::endl;
    s << " Z mass very loose cuts: "              << this->ZMassCutVeryLooseMin();
    s << " < M(ee) < "                            << this->ZMassCutVeryLooseMax()  << std::endl;
    s << " Z mass Low cuts: "                     << this->ZLowMassCutMin();
    s << " < M(ee) < "                            << this->ZLowMassCutMax()        << std::endl;

    s << " \n Value of vertex cuts:\n\n";
    s << " Primary Vertex Vertex Z position cut: |Z| < "                   << this->PriVertexZCut()     << "\n";
    s << " Minimal Number of Tracks in the Vertex : NtrkMin <= "           << this->MinNumTrkCut()      << "\n";
    s << " Minimal Number of Vertices cut: NvtxMin <= "                    << this->MinNumVertexCut()   << "\n";
    s << " Dzvtx of Electrons = "                 << this->Elec_DZVtxCut()         << std::endl;

    s << " \n \n Calibration Parameters: "        << std::endl;
    s << " Calibration Input file: "              << CalibrationInFile()       << std::endl;
    s << " Smearing Input file: "                 << SmearingFile()            << std::endl;
    s << " Calibration Method: "                  << SelectCalibMethod()       << std::endl;

    s << " \n \n Input information \n";
    s << " Flag IsMC: "                           << IsMC()                    << std::endl;
    s << " Flag IsData: "                         << IsData()                  << std::endl;
    s << " Flag IsInlineZ: "				      << IsInlineZ()               << std::endl;
    s << " Source: "                              << Source()                  << std::endl;
    s << " MC type: "                             << MCtype()                  << std::endl;

    if (Source() == "TTree") {
        s << " TTree Chain: "                     << TTreeChainName()          << std::endl;
    }



    s << " \n \n Information for AOD input \n";
    s << " Electron Container: "                  << ElectronContainer()       << std::endl;
    s << " CaloCluster Container: "               << CaloClusterContainer()    << std::endl;

    s << " PDF file name for reweighting: "       << PDFFileName()             << std::endl;

    s << " \n \n Information for Inlune Generator \n ";
    s << " Name of Input file with  histogram for InLine Generator: "  << ZeeDInlineZ_InFileName()  << std::endl;
    s << " Seeg for random generator: "                                << ZeeDInlineZ_SeedGen()     << std::endl;
    s << " Electron energy smearing : "                                << ZeeDInlineZ_ESmear()      << std::endl;
    s << " Include Radiation  : "                                      << ZeeDInlineZ_UseRadiation() << std::endl;


    s << " \n \n Other Flags \n";
    s << " Use systematics shifts: "              << UseSystematics()          << std::endl;
    s << " Readinf and Filling Gen info: "        << FillGenInfo()             << std::endl;
    s << " Flag to use trigger: "                 << UseTrigger()              << std::endl;
    s << " Number of MC systematics: "            << MCSystNumbers()           << std::endl;
    s << " Perform combined ToyMC  : "            << DoCombToyMC()             << std::endl;

    s << " \n \n Flags for corrections \n";
    s << " MC N Vtx reweighting                "                   << ApplyNvtxReweighting()             	<< std::endl;
    s << " MC Zv      reweighting              "                   << ApplyZPosVtxReweighting()            	<< std::endl;
    s << " MC NTrkVtx  reweighting             "                   << ApplyNTrkVtxReweighting()             	<< std::endl;
    s << " MC PDF reweighting                  "                   << ApplyPDFReweighting()       		<< std::endl;
    s << " MC Pileup reweighting               "                   << ApplyPileupReweighting()           	<< std::endl;
    s << " Data official energy correction:    " 	           << ApplyElecEnergyScaleCorr()		<< std::endl;
    s << " Data additional energy correction:  "                   << ApplyPrivateElecEnergyScaleCorr()		<< std::endl;
    s << " MC Iso efficiency correction:       "            	   << ApplyElecIsoCorr()			<< std::endl;
    s << " MC Trigger efficiency correction:   "        	   << ApplyTrigCorr()				<< std::endl;
    s << " MC smearing correction:             "	           << ApplyElecEnergySmearingCorr()		<< std::endl;
    s << " MC additional smearing correction:  "                   << ApplyPrivateElecEnergySmearingCorr()    	<< std::endl;
    s << " MC PtZ Reweighting:                 "		   << ApplyPtZReweighting() 			<< std::endl;
    s << " MC Central ID correction:           "		   << ApplyCentralIDCorrection()		<< std::endl;
    s << " MC Forward ID correction:           "		   << ApplyForwardIDCorrection()		<< std::endl;
    s << " MC Reco correction:                 "		   << ApplyRecoCorrection()  			<< std::endl;
    s << " MC Line Shape correction:           "             	   << ApplyLineShapeCorrection()		<< std::endl;
    s << " MC Charge correction:               "		   << ApplyChargeCorrection()			<< std::endl;


    s << " Details of the systematics:         "		      << SystematicsDetailed()  		  << std::endl;

    s << " \n \n Flags for tupes of Analysis \n	";
    s << " Read EtMiss "                              << ReadEtMiss()                             << std::endl;
    s << " Delete Soft Term "                         << DeleteSoftTerm()                         << std::endl;
    s << " Calculate hadron recoil using all of the leptons in EtMiss" << HadrRecoilFull()        << std::endl;
    s << " W "                                        << CalcW()                                  << std::endl;
    s << " Wmu "                                      << CalcWmu()                                << std::endl;
    s << " Zmumu "                                    << CalcZmumu()                              << std::endl;
    s << " Truth Matching "	                      << DoTruthMatching()                        << std::endl;
    s << " Trigger Matching "                         << DoTriggerMatching()                      << std::endl;
    s << " Fill NoCuts "                              << FillNoCuts()                             << std::endl;
    s << " Fill AnalysisChains "                      << FillAnalysisChains()                     << std::endl;
    s << " Execute Analysis Loop "                    << ExecuteAnalysisLoop()                    << std::endl;
    s << " Analyse Event "                            << AnalyseEvent()                           << std::endl;
    s << " Do Manual GRL "                            << DoManualGRL()                            << std::endl;
    s << " Manual GRL Filename "                      << ManualGRLFilename()                      << std::endl;
    s << " Jets "                                     << CalcJets()                               << std::endl;

    s << " Z+jet analysis (balance) "                 << DoZjet()                                 << std::endl;
    s << " Q/G jet tagger  "                          << DoZjetAna()                              << std::endl;
    s << " Z + jet analysis  "                        << DoZplusJet()                             << std::endl;
    s << " Jet charge calculation  "                  << CalcJetCharge()                          << std::endl;

    s << " Background Charge analysis "               << DoBackgroundChargeAnalysis()             << std::endl;

    s << " \n \n Flags for Filling Histograms \n";
    s << " Z Filling Stages "                         << AnalysisCutHistManagerZFillStages()      << std::endl;
    s << " Filling Bkg studies Histograms "           << FillBkgHistograms()                      << std::endl;
    s << " Calibrator "                               << FillCalibrator()                         << std::endl;
    s << " Extra Histograms for Cut flow"             << FillBitMask()                            << std::endl;
    s << " Extra Histograms for ZeeDObservable "      << FillObservable()                         << std::endl;
    s << " Fill Calorimeter Isolation Cut Decision "  << FillIsolationDecision()                  << std::endl;


    s << " The jet cleaning tool: " << JetCleaningTool() << std::endl;
    s << "  Jet collections names used in the analysis:   \t  ( " ;
    for ( unsigned int i = 0;  i < (JetCollectionNames()).size(); i++ )  s << (JetCollectionNames())[i] <<" ";
    s <<" ) "<< std::endl;



   // DO NOT REMOVE THE FOLLOWING LINE!
    //__NEWPRINT__

    s << " Use experimental ZCC filter: " << UseZCCElectronFiltration() << std::endl;


    s << " ClusterCorrectionTool: " << ClusterCorrectionTool() << std::endl;


    s << " switch new EM calibration: " << UseNNEMCalibration() << std::endl;


    s << " Do ZCF Analysis?: " << DoZCF() << std::endl;


    s << " Do ZCC Analysis?: " << DoZCC() << std::endl;


    s << " Include EMu fake bosons into analysis: " << CalcEMu() << std::endl;


    s << " Add several HM for events failing one cut: " << AddSingleRejectsHMs() << std::endl;


    s << " Add electron detailes hist manager: " << AddDetailHM() << std::endl;


    s << " Apply trigger correction: " << ApplyTriggerCorrection() << std::endl;


    s << " File name for the electron trigger efficiency (package ElectronEfficiencyCorrection): " << TriggerEfficiencyElectronFileName() << std::endl;


    s << " File name for the electron identification efficiency medium (package ElectronEfficiencyCorrection): " << IdentificationEfficiencyElectronMediumFileName() << std::endl;
    s << " File name for the electron identification efficiency tight (package ElectronEfficiencyCorrection): " << IdentificationEfficiencyElectronTightFileName() << std::endl;
    s << " File name for the electron identification efficiency tight (package ElectronEfficiencyCorrection): " << IdentificationEfficiencyElectronLooseFileName() << std::endl;


    s << " File name for the electron identification efficiency forward tight (package ElectronEfficiencyCorrection): " << IdentificationEfficiencyElectronFwdTightFileName() << std::endl;

    s << " File name for the electron reconstruction efficiency (package ElectronEfficiencyCorrection): " << ReconstructionEfficiencyElectronFileName() << std::endl;

    s << " File name for the muon trigger efficiency (package ElectronEfficiencyCorrection): " << TriggerEfficiencyMuonFileName() << std::endl;


    s << " File name for the muon identification efficiency medium (package ElectronEfficiencyCorrection): " << IdentificationEfficiencyMuonMediumFileName() << std::endl;

    s << " File name for the muon identification efficiency forward tight (package ElectronEfficiencyCorrection): " << IdentificationEfficiencyMuonFwdTightFileName() << std::endl;

    s << " File name for the muon reconstruction efficiency (package ElectronEfficiencyCorrection): " << ReconstructionEfficiencyMuonFileName() << std::endl;



    s << " : " << EvalSysExtraCrackCut() << std::endl;


    s << " Name to find the appropriate correction: " << ElectronMCChargeCorrectorName() << std::endl;


    s << " Name to find the appropriate correction: " << IDFwdCorrectionTightName() << std::endl;


    s << " Name to find the appropriate correction: " << IDFwdCorrectionMediumName() << std::endl;


    s << " Name to find the appropriate correction: " << IDFwdCorrectionLooseName() << std::endl;


    s << " Name to find the appropriate correction: " << IDCorrectionTightPPName() << std::endl;


    s << " Name to find the appropriate correction: " << IDCorrectionMediumPPName() << std::endl;


    s << " : " << ApplyGenBosMassCut() << std::endl;


    s << " : " << GenBosMassMirrorCut() << std::endl;


    s << " : " << GenBosMassCutContributesToGenWeight() << std::endl;


    s << " : " << ExtraCrackCutMax() << std::endl;


    s << " : " << ExtraCrackCutMin() << std::endl;


    s << " The production tag for G4 simulation (needed for Zvtx reweighing): " << MCG4SimTag() << std::endl;


    s << " Zvtx reweighing file: " << ZvtxRewFile() << std::endl;

    s << " : " << ExtraCrackCut() << std::endl;

    s << " Calculate electron calorimeter isolation with uniform efficiency: " << CalcEIso() << std::endl;


    s << " Fill bits hists: " << FillBitsHists() << std::endl;


    s << " MinimalHistoMode : " << MinimalHistoMode() << std::endl;


    s << " Evaluate uncorrelated systematics of trigger using Toy MCs: " << EvalSysTrigCorrMC() << std::endl;


    s << " Evaluate uncorrelated systematics of electron reconstruction using Toy MCs: " << EvalSysRecoCorrectionMC() << std::endl;


    s << " Evaluate uncorrelated systematics of electron isolation using Toy MCs: " << EvalSysElecIsoCorrectionMC() << std::endl;


    s << " Evaluate uncorrelated systematics of forward electron identification using Toy MCs: " << EvalSysForwardIDCorrectionMC() << std::endl;


    s << " Evaluate uncorrelated systematics of central electron identification using Toy MCs: " << EvalSysCentralIDCorrectionMC() << std::endl;


    s << " Write EWUnfold ntuples: " << WriteEWUnfoldNtuple() << std::endl;


    s << " Evaluate electron energy scale systematic: " << EvalSysElecEnergyScaleCorr() << std::endl;


    s << " Evaluate scale factor systematics using ToyMC: " << EvalSysSFMC() << std::endl;

    s << " Evaluate etMiss systematics " << EvalSysEtMiss() << std::endl;

    s << " Number of tracks associated to a vertex. Needed for consistency with the cut in JetCalibration: " << nTrkVertexJets() << std::endl;


    s << " Config file for recalibration. Needs to be specified explicitly: " << RecalibrateJetsConfig() << std::endl;


    s << " Flag to recalibrate jets on fly. Needs config file specified: " << RecalibrateJets() << std::endl;


    s << " Set gen. weight to 0 if generated boson mass is lower: " << GenBosMassMin() << std::endl;


    s << " Set gen. weight to 0 if generated boson mass is higher: " << GenBosMassMax() << std::endl;


    s << " Binning for 2011 ZCC analysis: " << BinGridZCC2011() << std::endl;


    s << "Evaluate pileup reweighting systematic: " << EvalSysPileupReweighting() << std::endl;


    s << "Evaluate vertex z position reweighting systematic: " << EvalSysZPosVtxReweighting() << std::endl;


    s << "Evaluate trigger systematic: " << EvalSysTrigCorr() << std::endl;
    s << "Evaluate Line Shape systematic: " << EvalSysLineShapeCorrection() << std::endl;


    s << "Evaluate electron charge systematic: " << EvalSysChargeCorrection() << std::endl;


    s << "Evaluate electron reconstruction systematic: " << EvalSysRecoCorrection() << std::endl;


    s << "Evaluate electron isolation systematic: " << EvalSysElecIsoCorr() << std::endl;


    s << "Evaluate forward electron identification systematic: " << EvalSysForwardIDCorrection() << std::endl;


    s << "Evaluate central electron identification systematic: " << EvalSysCentralIDCorrection() << std::endl;


    s << "Evaluate boson pt reweighting systematic: " << EvalSysPtZReweighting() << std::endl;


    s << "Evaluate electron energy smearing systematic: " << EvalSysElecEnergySmearingCorr() << std::endl;


    s << " Additional generators used as target for weighting - evaluated as systematics: " << std::endl;
    for(unsigned int i=0; i != BosonPtReweightingAdditionalTargets().size(); ++i) {
        std::cout << "                                                                                " << BosonPtReweightingAdditionalTargets()[i] << std::endl;
    }

    s << " Generator used as target for weighting: " << BosonPtReweightingTarget() << std::endl;


    s << " Flag to book extra plots, binned in Y boson rapidity bins: " << BookYBinnedExtraPlots() << std::endl;


    s << " Book shelves of mu ranges: " << BookMuShelves() << std::endl;



    s << " Delta R definition for dressed electrons: " << dressedElectronsDeltaR() << std::endl;

    s << " Book cuts used for background selection: " << BookBackgroundCuts() << std::endl;


    s << " Run Z baseline Analysis: " << RunZBaselineAna() << std::endl;


    s << " Cut on upper edge of the EMEC-FCAL crack: " << EtaCrackEMECFCAL_Max() << std::endl;


    s << " Cut on low edge of the EMEC-FCAL crack: " << EtaCrackEMECFCAL_Min() << std::endl;


    s << " PDF Set Number used for reweighting: " << PDFSetNumber() << std::endl;

    s << " Calculate Z from electrons: " << CalcZ() << std::endl;



    s << " Run Z low mass Analysis: " << RunZLowMassAna() << std::endl;




    s << " Run Z central-forward Analysis: " << RunZCFAna() << std::endl;


    s << " Run Z central-central Analysis: " << RunZCCAna() << std::endl;



    s << " book one shelf per (qqbar) initial state: " << BookInitialStateShelves() << std::endl;


    s << " assign each MC event its data-period (a data run-number corresponding to the contribution of each run to the total luminosity): " << MCAssignRandomRunNumber() << std::endl;


    s << " set if MC sample has calorimeter constant term on or off: " << MCWithConstantTerm() << std::endl;


    s << " Book shelves for all sub periods: " << DoSinglePeriods() << std::endl;


    s << " Evaluate systematic shifts: " << EvaluateSystematics() << std::endl;


    s << " MC production campaign, like mc11a, mc11b, etc.: " << MCCampaign() << std::endl;


    s << " Write TTree with the help of the CutHistManagerWriter: " << WriteTTreeWithCutHistManager() << std::endl;


    s << " MC file generating mode: " << GenPileupMCFile() << std::endl;


    s << " File to which the generated MC distribution of the PileupReweighting tool will be written to: " << PileupMCGenFile() << std::endl;


    s << "File containing MC distribution for PileupReweighting tool " << PileupMCFile() << std::endl;


    s << "File containing data distribution for PileupReweighting tool " << PileupDataFile() << std::endl;
    s << "Do BCHCleaning for jets" << DoBCHCleaning() << std::endl;
    s << "File name for TTileTripReader histograms" << TTileTripReaderFile() << std::endl;
    s << "File name for BCHCleaning histograms" << BCHCleaningFile() << std::endl;

    s << "File name for JER systematics" << JERInputFile() << std::endl;



    s << "\n \n Trigger information \n";
    s << "  Additional trigger chains for per event decision: ";

    std::vector<std::string>::const_iterator itrtrig = fTriggerEvent.begin();

    for( ; itrtrig != fTriggerEvent.end(); ++itrtrig) {
        s << *itrtrig << " ";
    }

    s << "\n";

    s << "  Additional L1 trigger chains for electron matching: ";
    itrtrig = fL1TriggerElecMatch.begin();

    for( ; itrtrig != fL1TriggerElecMatch.end(); ++itrtrig) {
        s << *itrtrig << " ";
    }

    s << "\n";

    s << "  Additional L2 trigger chains for electron matching: ";
    itrtrig = fL2TriggerElecMatch.begin();

    for( ; itrtrig != fL2TriggerElecMatch.end(); ++itrtrig) {
        s << *itrtrig << " ";
    }

    s << "\n";

    s << "  Additional EF trigger chains for electron matching: ";
    itrtrig = fEFTriggerElecMatch.begin();

    for( ; itrtrig != fEFTriggerElecMatch.end(); ++itrtrig) {
        s << *itrtrig << " ";
    }

    s << std::endl;

    return s.str();
}
