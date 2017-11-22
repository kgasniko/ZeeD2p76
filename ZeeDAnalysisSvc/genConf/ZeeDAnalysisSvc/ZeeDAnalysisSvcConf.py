#Tue Mar  7 18:47:18 2017
"""Automatically generated. DO NOT EDIT please"""
from GaudiKernel.GaudiHandles import *
from GaudiKernel.Proxy.Configurable import *

class ZeeDAnalysisSvc( ConfigurableService ) :
  __slots__ = { 
    'OutputLevel' : 7, # int
    'AuditServices' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'AuditFinalize' : False, # bool
    'AuditReInitialize' : False, # bool
    'AuditReStart' : False, # bool
    'Run_Min' : 190120, # int
    'Run_Max' : 190120, # int
    'Elec_EtaFwdMin' : 2.5, # float
    'Elec_EtaFwdMax' : 4.9, # float
    'Elec_EtaCut' : 2.37, # float
    'Elec_PtCut' : 15.0, # float
    'Elec_CentralPtMinCut' : 15.0, # float
    'Elec_ForwardPtMinCut' : 15.0, # float
    'Z_PtCut' : 120.0, # float
    'Elec_DZVtxCut' : 2.0, # float
    'Jet_PtCut' : 25.0, # float
    'Jet_EtaCut' : 2.5, # float
    'ZMassCutLooseMin' : 70.0, # float
    'ZMassCutLooseMax' : 110.0, # float
    'ZMassCutVeryLooseMin' : 50.0, # float
    'ZMassCutVeryLooseMax' : 130.0, # float
    'ZLowMassCutMin' : 20.0, # float
    'ZLowMassCutMax' : 70.0, # float
    'EtaCrackEMBEMEC_Min' : 1.37, # float
    'EtaCrackEMBEMEC_Max' : 1.52, # float
    'PriVertexZCut' : 150.0, # float
    'MinNumTrkCut' : 3, # int
    'MinNumVertexCut' : 3, # int
    'CaloClusterContainer' : 'egClusterCollection', # str
    'ElectronContainer' : 'ElectronAODCollection', # str
    'PDFFileName' : 'PDF/HERAPDF_MUATLAS.LHgrid', # str
    'Source' : 'TTree', # str
    'MCtype' : '', # str
    'ReadEtMiss' : False, # bool
    'RecalcEtMiss' : False, # bool
    'CorrectSumet' : False, # bool
    'HadrRecoilFull' : False, # bool
    'DeleteSoftTerm' : False, # bool
    'EtScaleFactor' : 1.0, # float
    'EtSmearFactor' : 0.0, # float
    'SumetCorFile' : '/afs/desy.de/user/k/kgasniko/ZeeD/share/HadronRecoil/SF_Pol2.root', # str
    'CalcW' : False, # bool
    'CalcWmu' : False, # bool
    'CalcJets' : False, # bool
    'CalcZmumu' : False, # bool
    'DoZjet' : False, # bool
    'DoZjetAna' : False, # bool
    'DoZplusJet' : False, # bool
    'CalcJetCharge' : False, # bool
    'DoBackgroundChargeAnalysis' : False, # bool
    'DoElectronCounting' : False, # bool
    'DoTruthMatching' : True, # bool
    'DoTriggerMatching' : True, # bool
    'FillNoCuts' : True, # bool
    'FillAnalysisChains' : True, # bool
    'ExecuteAnalysisLoop' : True, # bool
    'AnalyseEvent' : True, # bool
    'DoManualGRL' : False, # bool
    'ManualGRLFilename' : '', # str
    'ZeeDInlineZ_InFileName' : 'Inline/ZGen.root', # str
    'ZeeDInlineZ_SeedGen' : 1, # int
    'ZeeDInlineZ_ESmear' : 0.03, # float
    'ZeeDInlineZ_UseRadiation' : True, # bool
    'TTreeFileList' : [  ], # list
    'TTreeOutput' : '', # str
    'TTreeOutputMaxSize' : 0, # int
    'UseSystematics' : True, # bool
    'FillGenInfo' : True, # bool
    'ReweightSingleParticels' : False, # bool
    'SelectCalibMethod' : 4, # int
    'CalibrationInFile' : 'Calib/calib.in', # str
    'CalibrationOutFile' : 'Calib/calib.out', # str
    'SmearingFile' : 'Calib/smear.in', # str
    'CalibrationInternalFile' : 'Calib/calib.intern.out', # str
    'FirstRunHistogram' : -1, # int
    'LastRunHistogram' : -1, # int
    'AnalysisCutSelectorBookExtraCuts' : False, # bool
    'UseTrigger' : False, # bool
    'TriggerEvent' : [  ], # list
    'L1TriggerElecMatch' : [  ], # list
    'L2TriggerElecMatch' : [  ], # list
    'EFTriggerElecMatch' : [  ], # list
    'ApplyElecEnergyScaleCorr' : True, # bool
    'ApplyElecEnergySmearingCorr' : False, # bool
    'ApplyElecIsoCorr' : True, # bool
    'ApplyTrigCorr' : True, # bool
    'ApplyPtZReweighting' : True, # bool
    'ApplyPDFReweighting' : False, # bool
    'ApplyPileupReweighting' : False, # bool
    'ApplyNvtxReweighting' : False, # bool
    'ApplyNTrkVtxReweighting' : False, # bool
    'ApplyZPosVtxReweighting' : False, # bool
    'ApplyForwardIDCorrection' : False, # bool
    'ApplyCentralIDCorrection' : False, # bool
    'ApplyRecoCorrection' : False, # bool
    'ApplyChargeCorrection' : False, # bool
    'ApplyLineShapeCorrection' : False, # bool
    'ApplyPrivateElecEnergyScaleCorr' : False, # bool
    'ApplyPrivateElecEnergySmearingCorr' : False, # bool
    'MCDataAuto' : True, # bool
    'InputType' : 'data', # str
    'PoissonVaultDimension' : 0, # int
    'MCSystNumbers' : 0, # int
    'DoCombToyMC' : False, # bool
    'AnalysisCutHistManagerZFillStages' : True, # bool
    'FillCalibrator' : True, # bool
    'FillBkgHistograms' : True, # bool
    'FillBitMask' : False, # bool
    'FillObservable' : False, # bool
    'TTreeChainName' : '', # str
    'FillIsolationDecision' : True, # bool
    'SystematicsDetailed' : 0, # int
    'EgammaIsolationMVATopTool' : PublicToolHandle('IegammaIsolationMVATopTool'), # GaudiHandle
    'JetCleaningTool' : PublicToolHandle('IAthSelectorTool'), # GaudiHandle
    'JetCollectionNames' : [  ], # list
    'ZeeDEtDensityTools' : PublicToolHandleArray([]), # GaudiHandleArray
    'UseZCCElectronFiltration' : False, # bool
    'ClusterCorrectionTool' : PublicToolHandle('IegammaSwTool'), # GaudiHandle
    'UseNNEMCalibration' : False, # bool
    'DoZCF' : False, # bool
    'DoZCC' : False, # bool
    'CalcEMu' : True, # bool
    'AddSingleRejectsHMs' : False, # bool
    'AddDetailHM' : False, # bool
    'ApplyTriggerCorrection' : True, # bool
    'MuonOneBinTriggerSF' : False, # bool
    'MuonChargedTriggerSF' : True, # bool
    'TriggerEfficiencyElectronFileName' : 'efficiencySF.e24vhi_medium1_e60_medium1.Tight.2012.8TeV.rel17p2.v01.root', # str
    'IdentificationEfficiencyElectronMediumFileName' : 'efficiencySF.offline.Medium.2012.8TeV.rel17p2.v02.root', # str
    'IdentificationEfficiencyElectronTightFileName' : 'efficiencySF.offline.Tight.2012.8TeV.rel17p2.v02.root', # str
    'IdentificationEfficiencyElectronLooseFileName' : 'efficiencySF.offline.Tight.2012.8TeV.rel17p2.v02.root', # str
    'IdentificationEfficiencyElectronFwdTightFileName' : 'efficiencySF.offline.FwdTight.2012.8TeV.rel17p2.v07.root', # str
    'ReconstructionEfficiencyElectronFileName' : 'efficiencySF.offline.RecoTrk.2012.8TeV.rel17p2.v01.root', # str
    'TriggerEfficiencyMuonFileName' : 'efficiencySF.e24vhi_medium1_e60_medium1.Tight.2012.8TeV.rel17p2.v01.root', # str
    'IdentificationEfficiencyMuonMediumFileName' : 'efficiencySF.offline.Medium.2012.8TeV.rel17p2.v02.root', # str
    'IdentificationEfficiencyMuonFwdTightFileName' : 'efficiencySF.offline.FwdTight.2012.8TeV.rel17p2.v07.root', # str
    'ReconstructionEfficiencyMuonFileName' : 'efficiencySF.offline.RecoTrk.2012.8TeV.rel17p2.v01.root', # str
    'EvalSysExtraCrackCut' : False, # bool
    'ElectronMCChargeCorrectorName' : 'MC11c_G4FullSim/MediumPP', # str
    'IDFwdCorrectionTightName' : 'data11_7TeV_rel170_allyear_FwdTight', # str
    'IDFwdCorrectionMediumName' : 'data11_7TeV_rel170_allyear_FwdMedium', # str
    'IDFwdCorrectionLooseName' : 'data11_7TeV_rel170_allyear_FwdLoose', # str
    'IDCorrectionTightPPName' : 'data11_7TeV_rel170_allyear_TightPP', # str
    'IDCorrectionMediumPPName' : 'data11_7TeV_rel170_allyear_MediumPP', # str
    'ApplyGenBosMassCut' : False, # bool
    'GenBosMassMirrorCut' : False, # bool
    'GenBosMassCutContributesToGenWeight' : True, # bool
    'ExtraCrackCutMax' : 1.7, # float
    'ExtraCrackCutMin' : 1.6, # float
    'MCG4SimTag' : 's1370', # str
    'ZvtxRewFile' : 'zvtx_weights_2011_2012.root', # str
    'ExtraCrackCut' : False, # bool
    'CalcEIso' : False, # bool
    'FillBitsHists' : False, # bool
    'MinimalHistoMode' : True, # bool
    'EvalSysTrigCorrMC' : False, # bool
    'EvalSysRecoCorrectionMC' : False, # bool
    'EvalSysElecIsoCorrectionMC' : False, # bool
    'EvalSysForwardIDCorrectionMC' : False, # bool
    'EvalSysCentralIDCorrectionMC' : False, # bool
    'WriteEWUnfoldNtuple' : False, # bool
    'EvalSysElecEnergyScaleCorr' : True, # bool
    'EvalSysSFMC' : True, # bool
    'EvalSysEtMiss' : True, # bool
    'nTrkVertexJets' : 2, # int
    'RecalibrateJetsConfig' : '', # str
    'RecalibrateJets' : False, # bool
    'GenBosMassMin' : 0.0, # float
    'GenBosMassMax' : -1.0, # float
    'BinGridZCC2011' : 'Binning/BosYMassCosTh.txt', # str
    'EvalSysPileupReweighting' : True, # bool
    'EvalSysZPosVtxReweighting' : True, # bool
    'EvalSysTrigCorr' : True, # bool
    'EvalSysChargeCorrection' : True, # bool
    'EvalSysLineShapeCorrection' : True, # bool
    'EvalSysRecoCorrection' : True, # bool
    'EvalSysElecIsoCorr' : True, # bool
    'EvalSysForwardIDCorrection' : True, # bool
    'EvalSysCentralIDCorrection' : True, # bool
    'EvalSysPtZReweighting' : True, # bool
    'EvalSysElecEnergySmearingCorr' : True, # bool
    'BosonPtReweightingAdditionalTargets' : [  ], # list
    'BosonPtReweightingTarget' : 'PowhegPythia8MC11', # str
    'BookYBinnedExtraPlots' : False, # bool
    'BookMuShelves' : False, # bool
    'dressedElectronsDeltaR' : 0.1, # float
    'BookBackgroundCuts' : False, # bool
    'RunZBaselineAna' : True, # bool
    'EtaCrackEMECFCAL_Max' : 3.35, # float
    'EtaCrackEMECFCAL_Min' : 3.16, # float
    'PDFSetNumber' : 0, # int
    'CalcZ' : True, # bool
    'RunZLowMassAna' : True, # bool
    'RunZCFAna' : True, # bool
    'RunZCCAna' : True, # bool
    'BookInitialStateShelves' : False, # bool
    'MCAssignRandomRunNumber' : False, # bool
    'MCWithConstantTerm' : False, # bool
    'DoSinglePeriods' : False, # bool
    'EvaluateSystematics' : False, # bool
    'MCCampaign' : 'mc11b', # str
    'WriteTTreeWithCutHistManager' : False, # bool
    'GenPileupMCFile' : True, # bool
    'PileupMCGenFile' : '', # str
    'PileupMCFile' : '', # str
    'PileupDataFile' : '', # str
    'JESUncertaintiesConfig' : '', # str
    'MultiJESUncertaintiesConfig' : '', # str
    'PathDirUncertaintiesConfig' : '', # str
    'DoPtJetReweighting' : False, # bool
    'PtJetReweightingFile' : '', # str
    'DoBCHCleaning' : False, # bool
    'TTileTripReaderFile' : '', # str
    'BCHCleaningFile' : '', # str
    'JERInputFile' : '', # str
  }
  _propertyDocDct = { 
    'IDFwdCorrectionMediumName' : """ Name to find the appropriate correction """,
    'FillNoCuts' : """  """,
    'AnalyseEvent' : """  """,
    'GenPileupMCFile' : """ MC file generating mode """,
    'ApplyElecIsoCorr' : """ Flag to switch official MC Isolation efficiency correction """,
    'ApplyForwardIDCorrection' : """ Flag to switch ID correction for forward electron  """,
    'EvalSysPtZReweighting' : """ Evaluate boson pt reweighting systematic """,
    'nTrkVertexJets' : """ Number of tracks associated to a vertex. Needed for consistency with the cut in JetCalibration """,
    'WriteTTreeWithCutHistManager' : """ Write TTree with the help of the CutHistManagerWriter """,
    'ApplyPtZReweighting' : """ Flag to switch official Pt reweighting  """,
    'ElectronMCChargeCorrectorName' : """ Name to find the appropriate correction """,
    'SumetCorFile' : """  """,
    'PoissonVaultDimension' : """ Flag to store per-event generated Poisson random numbers """,
    'GenBosMassMin' : """ Set gen. weight to 0 if generated boson mass is lower """,
    'GenBosMassCutContributesToGenWeight' : """  """,
    'CalcJets' : """  """,
    'CalcJetCharge' : """  """,
    'IdentificationEfficiencyMuonMediumFileName' : """ File name for the muon identification efficiency (package ElectronEfficiencyCorrection) """,
    'LastRunHistogram' : """  Last run for the yield histogram """,
    'MultiJESUncertaintiesConfig' : """ Config file for MultiJES uncertainties """,
    'JetCleaningTool' : """ The jet cleaning tool """,
    'MCG4SimTag' : """ Type of MC in respect to Zvtx reweghing """,
    'DoBCHCleaning' : """ Flag to switch BCH Cleaning for jets """,
    'ApplyGenBosMassCut' : """  """,
    'ExtraCrackCut' : """  """,
    'Elec_EtaFwdMin' : """ Electron forward eta cut """,
    'RunZCFAna' : """ Run Z central-forward Analysis """,
    'EtScaleFactor' : """  """,
    'ZeeDInlineZ_InFileName' : """ Name of file with histograms """,
    'PileupDataFile' : """ File containing data distribution for PileupReweighting tool """,
    'GenBosMassMax' : """ Set gen. weight to 0 if generated boson mass is higher """,
    'DoPtJetReweighting' : """ PT(jet) reweighting (QG analysis) """,
    'RunZCCAna' : """ Run Z central-central Analysis """,
    'CaloClusterContainer' : """ Main cluster container """,
    'ApplyElecEnergySmearingCorr' : """ Flag to switch official MC smearing correction """,
    'ZLowMassCutMin' : """ ee Invariant mass cut low """,
    'EvalSysElecEnergySmearingCorr' : """ Evaluate electron energy smearing systematic """,
    'ApplyRecoCorrection' : """ Flag to switch Reco correction for electron  """,
    'EvalSysElecEnergyScaleCorr' : """ Evaluate electron energy scale systematic """,
    'Z_PtCut' : """ Z pt cut """,
    'Run_Min' : """ Run range: min run number """,
    'ApplyNvtxReweighting' : """ Flag switch reweight for MC Vtx """,
    'ReweightSingleParticels' : """ Switch on / off reweighting of Pt for single particles """,
    'DoTriggerMatching' : """  """,
    'ApplyZPosVtxReweighting' : """ Flag switch reweight for Zv distribution """,
    'MuonOneBinTriggerSF' : """ make one bin muon trigger scale factors """,
    'ZeeDInlineZ_UseRadiation' : """ Flag for include radiation """,
    'ClusterCorrectionTool' : """  """,
    'MCDataAuto' : """ Flag to switch MC/Data auto detection """,
    'GenBosMassMirrorCut' : """  """,
    'BookYBinnedExtraPlots' : """ Flag to book extra plots, binned in Y boson rapidity bins """,
    'ApplyPDFReweighting' : """ Flag to switch PDF weight """,
    'ExecuteAnalysisLoop' : """  """,
    'MinNumVertexCut' : """ Minimal number of good vertices in the event """,
    'BCHCleaningFile' : """ File name for BCHCleaning histograms """,
    'MinimalHistoMode' : """  """,
    'InputType' : """ Flag to switch MC/Data manually """,
    'Elec_DZVtxCut' : """ D Zvtx Electron cut """,
    'TTileTripReaderFile' : """ File name for TTileTripReader histograms """,
    'BookBackgroundCuts' : """ Book cuts used for background selection """,
    'FillBitsHists' : """ Fill bits hists """,
    'EtaCrackEMECFCAL_Max' : """ Cut on upper edge of the EMEC-FCAL crack """,
    'PtJetReweightingFile' : """ path to the file for pT(jet) reweighting (QG analysis) """,
    'Source' : """ AOD, Inline, D3PD, ... """,
    'CalcZmumu' : """  """,
    'IDFwdCorrectionTightName' : """ Name to find the appropriate correction """,
    'IdentificationEfficiencyElectronLooseFileName' : """ File name for the electron identification efficiency (package ElectronEfficiencyCorrection) """,
    'ApplyPrivateElecEnergyScaleCorr' : """ Flag to switch private zeed additional energy correction """,
    'AnalysisCutHistManagerZFillStages' : """ Flag to book histmanagers for all stages """,
    'EvalSysEtMiss' : """ Evaluate EtMiss systematics """,
    'EvalSysTrigCorrMC' : """ Evaluate uncorrelated systematics of trigger using Toy MCs """,
    'RecalcEtMiss' : """  """,
    'IDCorrectionTightPPName' : """ Name to find the appropriate correction """,
    'CalcWmu' : """  """,
    'ZeeDInlineZ_SeedGen' : """ Seed for ramfom generator """,
    'DoZjet' : """  """,
    'SmearingFile' : """ Smearing input file """,
    'DoTruthMatching' : """  """,
    'ApplyLineShapeCorrection' : """ Flag to switch Line Shape correction  """,
    'FillGenInfo' : """ Switch on / off Reading Gen info """,
    'EvalSysPileupReweighting' : """ Evaluate pileup reweighting systematic """,
    'Jet_EtaCut' : """ Jet Eta cut """,
    'ZMassCutLooseMax' : """ ee Invariant mass cut high """,
    'ZeeDInlineZ_ESmear' : """ Smiring for energy of electron """,
    'TTreeFileList' : """ List of input TTree files """,
    'DoElectronCounting' : """  """,
    'EtSmearFactor' : """  """,
    'IdentificationEfficiencyElectronMediumFileName' : """ File name for the electron identification efficiency (package ElectronEfficiencyCorrection) """,
    'SelectCalibMethod' : """ Select  calibration method  """,
    'CalcEMu' : """ Include EMu fake bosons into analysis """,
    'EvalSysSFMC' : """ Evaluate scale factor systematics using ToyMC """,
    'IdentificationEfficiencyMuonFwdTightFileName' : """ File name for the muon identification efficiency (package ElectronEfficiencyCorrection) """,
    'IDCorrectionMediumPPName' : """ Name to find the appropriate correction """,
    'ReconstructionEfficiencyMuonFileName' : """ File name for the muon reconstruction efficiency (package ElectronEfficiencyCorrection) """,
    'FillBkgHistograms' : """ Flag to book histmanagers for Bkg Studies """,
    'EvalSysExtraCrackCut' : """  """,
    'EFTriggerElecMatch' : """ List of trigger EF chain names for electron matching """,
    'dressedElectronsDeltaR' : """ Delta R definition for dressed electrons """,
    'EvalSysTrigCorr' : """ Evaluate trigger systematic """,
    'HadrRecoilFull' : """  """,
    'ApplyTriggerCorrection' : """ Apply trigger correction """,
    'EvalSysCentralIDCorrection' : """ Evaluate central electron identification systematic """,
    'ExtraCrackCutMin' : """  """,
    'PDFSetNumber' : """ PDF Set Number used for reweighting """,
    'CalcEIso' : """ Calculate electron calorimeter isolation with uniform efficiency """,
    'PDFFileName' : """ LHAPDF file name for reweight """,
    'CalibrationInFile' : """ Calibration input file """,
    'MCtype' : """ Pythia, MCNLO ... """,
    'CalibrationOutFile' : """ Calibration output file """,
    'EtaCrackEMBEMEC_Min' : """ EMB-EMEC crack min """,
    'FillCalibrator' : """ Flag to book histmanagers for Calibration """,
    'IdentificationEfficiencyElectronFwdTightFileName' : """ File name for the electron identification efficiency (package ElectronEfficiencyCorrection) """,
    'UseSystematics' : """ Switch on / off systematics shifts """,
    'MCAssignRandomRunNumber' : """ assign each MC event its data-period (a data run-number corresponding to the contribution of each run to the total luminosity) """,
    'BosonPtReweightingTarget' : """ Generator used as target for weighting """,
    'DoSinglePeriods' : """ Book shelves for all single (D,E,...) periods """,
    'EvalSysZPosVtxReweighting' : """ Evaluate vertex z position reweighting systematic """,
    'BookInitialStateShelves' : """ book one shelf per (qqbar) initial state """,
    'UseTrigger' : """ Flag switch on trigger """,
    'IDFwdCorrectionLooseName' : """ Name to find the appropriate correction """,
    'FillBitMask' : """ Flag to book histmanagers with full bit mask (for cut flow)  """,
    'CalcZ' : """ Calculate Z from electrons """,
    'TriggerEfficiencyMuonFileName' : """ File name for the muon trigger efficiency (package ElectronEfficiencyCorrection) """,
    'AddDetailHM' : """ Add electron detailes hist manager """,
    'Elec_EtaFwdMax' : """ Electron forward eta cut """,
    'ApplyChargeCorrection' : """ Flag to switch charge correction of the electron  """,
    'SystematicsDetailed' : """ Detail level of the systematics calculation """,
    'DoZCF' : """ Do ZCF Analysis? """,
    'TriggerEfficiencyElectronFileName' : """ File name for the electron trigger efficiency (package ElectronEfficiencyCorrection) """,
    'RecalibrateJetsConfig' : """ Config file for recalibration. Needs to be specified explicitly """,
    'MuonChargedTriggerSF' : """ make charge dependent muon trigger scale factors """,
    'DoZCC' : """ Do ZCC Analysis? """,
    'CalcW' : """  """,
    'PathDirUncertaintiesConfig' : """ path to JetUncertainties/share/ directory """,
    'DoBackgroundChargeAnalysis' : """  """,
    'EtaCrackEMECFCAL_Min' : """ Cut on low edge of the EMEC-FCAL crack """,
    'TTreeOutput' : """ Name of output TTree file (empty for disable) """,
    'AddSingleRejectsHMs' : """ Add several HM for events failing one cut """,
    'ZMassCutVeryLooseMin' : """ ee Invariant mass cut low """,
    'ApplyCentralIDCorrection' : """ Flag to switch ID correction for central electron  """,
    'Jet_PtCut' : """ Jet Pt cut """,
    'ZvtxRewFile' : """ Zvtx reweighing data file """,
    'PriVertexZCut' : """ Primary Vertex Z-position cut """,
    'RunZLowMassAna' : """ Run Z low mass Analysis """,
    'DoManualGRL' : """  """,
    'EvalSysElecIsoCorrectionMC' : """ Evaluate uncorrelated systematics of electron isolation using Toy MCs """,
    'FirstRunHistogram' : """  First run for the yield histogram """,
    'ZLowMassCutMax' : """ ee Invariant mass cut high """,
    'ReconstructionEfficiencyElectronFileName' : """ File name for the electron reconstruction efficiency (package ElectronEfficiencyCorrection) """,
    'ZMassCutVeryLooseMax' : """ ee Invariant mass cut high """,
    'Elec_PtCut' : """ Electron pt cut """,
    'L2TriggerElecMatch' : """ List of trigger L2 chain names for electron matching """,
    'DoCombToyMC' : """ Flag to switch on combined ToyMC method """,
    'DeleteSoftTerm' : """  """,
    'ApplyPileupReweighting' : """ Flag to switch Pileup weight """,
    'WriteEWUnfoldNtuple' : """ Write EWUnfold ntuples """,
    'FillIsolationDecision' : """ Fill Calorimeter Isolation Cut Decision """,
    'JERInputFile' : """ File name for JER systematics calculation """,
    'ApplyPrivateElecEnergySmearingCorr' : """ Flag to switch private ZeeD MC smearing correction """,
    'DoZjetAna' : """  """,
    'Elec_CentralPtMinCut' : """ Central Electron pt cut """,
    'MCCampaign' : """ MC production campaign, like mc11a, mc11b, etc. """,
    'MinNumTrkCut' : """ Minimal number of tracks in the good vertex """,
    'CalibrationInternalFile' : """ Calibration internal output file """,
    'ZMassCutLooseMin' : """ ee Invariant mass cut low """,
    'ReadEtMiss' : """  """,
    'ZeeDEtDensityTools' : """ Tools to calculate cluster  Et density in an event """,
    'Elec_ForwardPtMinCut' : """ Forward Electron pt cut """,
    'L1TriggerElecMatch' : """ List of trigger L1 chain names for electron matching """,
    'ApplyNTrkVtxReweighting' : """ Flag switch reweight for MC NTrkVtx """,
    'PileupMCGenFile' : """ File to which the generated MC distribution of the PileupReweighting tool will be written to """,
    'EvalSysRecoCorrection' : """ Evaluate electron reconstruction systematic """,
    'EtaCrackEMBEMEC_Max' : """ EMB-EMEC crack max """,
    'IdentificationEfficiencyElectronTightFileName' : """ File name for the electron identification efficiency (package ElectronEfficiencyCorrection) """,
    'EvalSysElecIsoCorr' : """ Evaluate electron isolation systematic """,
    'Run_Max' : """ Run range: max run number """,
    'BosonPtReweightingAdditionalTargets' : """ Additional generators used as target for weighting - evaluated as systematics """,
    'RunZBaselineAna' : """ Run Z baseline Analysis """,
    'ManualGRLFilename' : """  """,
    'EvalSysRecoCorrectionMC' : """ Evaluate uncorrelated systematics of electron reconstruction using Toy MCs """,
    'AnalysisCutSelectorBookExtraCuts' : """ Book extra, beyond baseline, cuts for additional studies """,
    'RecalibrateJets' : """ Flag to recalibrate jets on fly. Needs config file specified """,
    'TTreeOutputMaxSize' : """ Maximum size per one output files (0 for infinite) """,
    'BinGridZCC2011' : """ Binning for 2011 ZCC analysis """,
    'EvalSysCentralIDCorrectionMC' : """ Evaluate uncorrelated systematics of central electron identification using Toy MCs """,
    'FillObservable' : """ Flag to book ZeeDObservable hist manager  """,
    'ExtraCrackCutMax' : """  """,
    'MCWithConstantTerm' : """ set if MC sample has calorimeter constant term on or off """,
    'UseZCCElectronFiltration' : """ Use experimental ZCC filter """,
    'EvalSysForwardIDCorrectionMC' : """ Evaluate uncorrelated systematics of forward electron identification using Toy MCs """,
    'TriggerEvent' : """ List of trigger to be checked if event has passed """,
    'EvaluateSystematics' : """ Evaluate systematic shifts """,
    'UseNNEMCalibration' : """ switch new EM calibration """,
    'EvalSysForwardIDCorrection' : """ Evaluate forward electron identification systematic """,
    'PileupMCFile' : """ File containing MC distribution for PileupReweighting tool """,
    'ApplyElecEnergyScaleCorr' : """ Flag to switch official energy correction """,
    'EvalSysLineShapeCorrection' : """ Evaluate Line Shape systematic """,
    'EvalSysChargeCorrection' : """ Evaluate electron charge systematic """,
    'TTreeChainName' : """ Name of the TTree file to read """,
    'MCSystNumbers' : """ Flag to store per-event tables for MC systematics """,
    'DoZplusJet' : """  """,
    'BookMuShelves' : """ Book shelves of mu ranges """,
    'FillAnalysisChains' : """  """,
    'JetCollectionNames' : """  Jet collections names used in the analysis """,
    'JESUncertaintiesConfig' : """ Config file for JES uncertainties """,
    'Elec_EtaCut' : """ Electron eta cut """,
    'ElectronContainer' : """ Main electron container """,
    'CorrectSumet' : """  """,
    'ApplyTrigCorr' : """ Flag to switch official Trigger correction """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(ZeeDAnalysisSvc, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'ZeeDAnalysisSvc'
  def getType( self ):
      return 'ZeeDAnalysisSvc'
  pass # class ZeeDAnalysisSvc
