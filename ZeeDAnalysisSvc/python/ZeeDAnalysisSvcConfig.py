
from ZeeDAnalysisSvc.ZeeDAnalysisSvcConf import ZeeDAnalysisSvc
from AthenaCommon.SystemOfUnits import GeV


class DefaultZeeDAnalysisSvc(ZeeDAnalysisSvc):
    __slots__ = []

    def __init__(self, name = "DefaultZeeDAnalysisSvc"):
        super( DefaultZeeDAnalysisSvc, self ).__init__( name )

        from AthenaCommon.Logging import logging
        log = logging.getLogger( 'ZeeDAnalysisSvc' )

    def setDefaults(cls, handle):
        pass

class ZeeDAnalysisSvc(DefaultZeeDAnalysisSvc):
    __slots__ = []

    def __init__(self, name = "ZeeDAnalysisSvc"):
        super( DefaultZeeDAnalysisSvc, self ).__init__( name )

        from AthenaCommon.Logging import logging
        log = logging.getLogger( 'ZeeDAnalysisSvc' )

        # set defaults
        self.Run_Min                  = 190120
        self.Run_Max                  = 190120
        self.Elec_EtaCut              = 2.47
        self.Elec_EtaFwdMin           = 2.5
        self.Elec_EtaFwdMax           = 4.9
        self.Elec_PtCut               = 20.
        self.Elec_CentralPtMinCut     = 23.
        self.Elec_ForwardPtMinCut     = 20.
        self.Jet_PtCut                = 15.
        self.Jet_EtaCut               = 2.5
        self.Z_PtCut                  = 120.
        self.Elec_DZVtxCut            = 2.
        self.ZMassCutLooseMin         = 66.
        self.ZMassCutLooseMax         = 116.
        self.ZMassCutVeryLooseMin     = 30.
        self.ZMassCutVeryLooseMax     = 126.
        self.ZLowMassCutMin           = 30.
        self.ZLowMassCutMax           = 70.
        self.EtaCrackEMBEMEC_Min      = 1.37
        self.EtaCrackEMBEMEC_Max      = 1.52
        self.PriVertexZCut            = 150.0
        self.MinNumTrkCut             = 3
        self.MinNumVertexCut          = 3
        self.CaloClusterContainer     = "egClusterCollection"
        self.ElectronContainer        = "ElectronAODCollection"
        self.PDFFileName              = "PDF/ATLAS-epWZ12-EIG.LHgrid"
        self.SelectCalibMethod        = 4
        self.CalibrationInFile        = "../share/Calib/calib.in"
        self.CalibrationOutFile       = "/afs/desy.de/user/k/kgasniko/xxl/official_ZeeD/17.2.7.5.20/ZeeD/ZeeDAnalysis/share/Calib/calib.out"
        self.SmearingFile             = "../share/Calib/smear.in"        
        self.CalibrationInternalFile  = "/afs/desy.de/user/k/kgasniko/xxl/official_ZeeD/17.2.7.5.20/ZeeD/ZeeDAnalysis/shareCalib/calib.intern.out"
        self.Source                   = "TTree"
        self.DoTruthMatching          = True 
        self.FillNoCuts               = True
        self.FillAnalysisChains       = True
        self.ExecuteAnalysisLoop      = True
        self.AnalyseEvent             = True
        self.ReadEtMiss               = True
        self.RecalcEtMiss             = False
        self.CorrectSumet             = False
        self.HadrRecoilFull           = True
        self.DeleteSoftTerm           = False
        self.EtScaleFactor            = 1.0
        self.EtSmearFactor            = 0.0
        self.SumetCorFile             = "../share/HadronRecoil/SF_Pol1_All_Truth.root"
        self.CalcW                    = True
        self.CalcZmumu                = True 
        self.CalcWmu                  = True
        self.CalcJets                 = False 
        self.DoZjet                   = False
        self.DoZjetAna                = False
        self.CalcJetCharge            = False
        self.DoBackgroundChargeAnalysis = False
        self.DoElectronCounting       = False
        self.ZeeDInlineZ_InFileName   = "Inline/ZGen.root"
        self.ZeeDInlineZ_SeedGen      = 1
        self.ZeeDInlineZ_ESmear       = 0.03
        self.ZeeDInlineZ_UseRadiation = 1
        self.TTreeFileList           = []
        self.TTreeOutput             = "TTreeOut.root"
        self.TTreeOutputMaxSize      = 0
        self.WriteTTreeWithCutHistManager = True
        self.TTreeChainName          = ""
        self.EvaluateSystematics    = False 
        self.FillGenInfo              = 1
        self.ReweightSingleParticels  = 0
        self.FirstRunHistogram        = 219257
        self.LastRunHistogram         = 219364
        self.UseTrigger               = False 
        self.ApplyTriggerCorrection   = True
        self.DoTriggerMatching        = False
        self.TriggerEvent             = []
        self.L1TriggerElecMatch       = []
        self.L2TriggerElecMatch       = []
        self.EFTriggerElecMatch       = []
        self.ApplyNvtxReweighting             = False
        self.ApplyElecEnergyScaleCorr         = True
        self.ApplyElecIsoCorr                 = False 
        self.ApplyElecEnergySmearingCorr      = True
        self.ApplyPileupReweighting           = False
        self.ApplyPDFReweighting              = False 
        self.ApplyZPosVtxReweighting          = False
        self.ApplyNTrkVtxReweighting          = False
        self.ApplyCentralIDCorrection         = True
        self.ApplyForwardIDCorrection         = False
        self.ApplyRecoCorrection              = True
        self.ApplyChargeCorrection            = False 
        self.ApplyLineShapeCorrection         = False 
        self.ApplyPrivateElecEnergyScaleCorr         = False
        self.ApplyPrivateElecEnergySmearingCorr      = False                      
        self.PoissonVaultDimension                   = 0
        self.MCSystNumbers                           = 35
        self.SystematicsDetailed                     = 0        
        self.FillCalibrator                          = False
        self.AnalysisCutHistManagerZFillStages       = False
        self.FillBkgHistograms                       = True
        self.FillBitMask                             = False
        self.FillObservable                          = False        
        self.AnalysisCutSelectorBookExtraCuts        = False
        #DO NOT REMOVE THE FOLLOWING LINE!
        #__NEWDEFAULTVALUE__

        self.UseZCCElectronFiltration = False 

        self.UseNNEMCalibration = False 

        self.DoZCF = False

        self.DoZCC = True

        self.CalcEMu = False

        self.AddSingleRejectsHMs = False

        self.AddDetailHM = False
        self.MuonChargedTriggerSF = True
        self.MuonOneBinTriggerSF = True
        self.TriggerEfficiencyElectronFileName = "efficiencySF.e15_loose1.Medium.2012.2.76TeV.rel17p2.GEO20.v01.root"

        self.IdentificationEfficiencyElectronMediumFileName = "efficiencySF.offline.Medium.2012.2.76TeV.rel17p2.GEO20.v01.root" 
        self.IdentificationEfficiencyElectronTightFileName = "efficiencySF.offline.Tight.2012.2.76TeV.rel17p2.GEO20.v01.root"
        self.IdentificationEfficiencyElectronLooseFileName = "efficiencySF.offline.Loose.2012.8TeV.rel17p2.GEO21.v01.root"

        self.IdentificationEfficiencyElectronFwdTightFileName = "efficiencySF.offline.FwdTight.2012.8TeV.rel17p2.v07.root"
        self.ReconstructionEfficiencyElectronFileName = "efficiencySF.offline.RecoTrk.2012.8TeV.rel17p2.GEO20.v08.root"

        self.TriggerEfficiencyMuonFileName = "efficiency.e12Tvh_loose1.Tight.2012.8TeV.rel17p2.GEO21.v01.root"

        self.IdentificationEfficiencyMuonMediumFileName = "efficiencySF.offline.Medium.2012.8TeV.rel17p2.GEO20.v01.root"

        self.IdentificationEfficiencyMuonFwdTightFileName = "efficiencySF.offline.FwdTight.2012.8TeV.rel17p2.v07.root"
        self.ReconstructionEfficiencyMuonFileName = "STACO_CB_2012_SF.txt.gz"


        self.EvalSysExtraCrackCut = False

        self.ElectronMCChargeCorrectorName = "MC11c_G4FullSim/MediumPP"

        self.IDFwdCorrectionTightName = "data11_7TeV_rel170_allyear_FwdTight"

        self.IDFwdCorrectionMediumName = "data11_7TeV_rel170_allyear_FwdMedium"

        self.IDFwdCorrectionLooseName = "data11_7TeV_rel170_allyear_FwdLoose"

        self.IDCorrectionTightPPName = "data11_7TeV_rel170_allyear_TightPP"

        self.IDCorrectionMediumPPName = "data11_7TeV_rel170_allyear_MediumPP"

        self.ApplyGenBosMassCut = False

        self.GenBosMassMirrorCut = False

        self.GenBosMassCutContributesToGenWeight = False

        self.ExtraCrackCutMax = 1.7

        self.ExtraCrackCutMin = 1.6

        self.MCG4SimTag = "s1647"

        self.ZvtxRewFile = "zvtx_weights_2013.root"

        self.ExtraCrackCut = False

        self.CalcEIso = False

        self.FillBitsHists = False

        self.MinimalHistoMode = False

        self.EvalSysTrigCorrMC = True

        self.EvalSysRecoCorrectionMC = True 

        self.EvalSysElecIsoCorrectionMC = False

        self.EvalSysForwardIDCorrectionMC = False

        self.EvalSysCentralIDCorrectionMC = True

        self.WriteEWUnfoldNtuple = False

        self.EvalSysElecEnergyScaleCorr = True

        self.EvalSysSFMC = True
        self.EvalSysEtMiss = True
        self.nTrkVertexJets = 2

        self.RecalibrateJets = False 
        self.RecalibrateJetsConfig       = "XML/ApplyJetCalibration/CalibrationConfigs/JES_Full2012dataset_MC12c_May2014.config"
        self.JESUncertaintiesConfig      = "JES_2012/Final/InsituJES2012_14NP.config"
        self.MultiJESUncertaintiesConfig = "JES_2012/Final/InsituJES2012_14NP.config"
        self.JESUncertaintiesConfig = ""
        self.MultiJESUncertaintiesConfig = ""
        self.PathDirUncertaintiesConfig = "" 
        self.DoPtJetReweighting = False
        self.PtJetReweightingFile = ""

        self.GenBosMassMin = 0

        self.GenBosMassMax = -1

        self.BinGridZCC2011 = "Binning/BosYMassCosTh.txt"

        self.EvalSysPileupReweighting = False

        self.EvalSysZPosVtxReweighting = False

        self.EvalSysTrigCorr = True

        self.EvalSysChargeCorrection = False

        self.EvalSysRecoCorrection = True

        self.EvalSysElecIsoCorr = False 

        self.EvalSysForwardIDCorrection = False

        self.EvalSysCentralIDCorrection = True
        self.EvalSysLineShapeCorrection = False

        self.EvalSysElecEnergySmearingCorr = True

        self.ApplyPtZReweighting        = False
        self.EvalSysPtZReweighting      = False
        self.BosonPtReweightingTarget       = "PowhegPythia8MC11_ZData2011"
        self.BosonPtReweightingAdditionalTargets = []

        self.EvaluateSystematics = True

        self.BookYBinnedExtraPlots = False

        self.BookMuShelves = False

        self.dressedElectronsDeltaR = 0.1

        self.RunZBaselineAna = False

        self.BookBackgroundCuts = False

        self.DoCombToyMC = False

        self.EtaCrackEMECFCAL_Max = 3.35

        self.EtaCrackEMECFCAL_Min = 3.16

        self.PDFSetNumber = 0

        self.CalcZ = True

        self.RunZLowMassAna = False

        self.RunZCFAna = False

        self.RunZCCAna = True

        self.BookInitialStateShelves = False

        self.MCAssignRandomRunNumber = False

        self.MCWithConstantTerm = False

        self.DoSinglePeriods = False

        self.MCCampaign = "mc12a"

        self.GenPileupMCFile = False

        self.PileupMCGenFile = ""

        self.PileupMCFile = "Pileup/mc12a_defaults.prw.root"

        self.PileupDataFile = "Pileup/ilumicalc_histograms_None_200842-215643.root"
        self.DoBCHCleaning = False;
        self.TTileTripReaderFile="";
        self.BCHCleaningFile="";

        self.JERInputFile="";


    def setDefaults(cls, handle):
        pass

class TestZeeDAnalysisSvc(ZeeDAnalysisSvc):
    __slots__ = []

    """
    test class for standalone test
    """

    def __init__(self, name = "ZeeDAnalysisSvc"):
        super( TestZeeDAnalysisSvc, self ).__init__( name )

        from AthenaCommon.Logging import logging
        log = logging.getLogger( 'ZeeDAnalysisSvc' )

        from AthenaCommon.Constants import DEBUG
        self.OutputLevel = DEBUG

        # create service in a standalone job
        from AthenaCommon.AppMgr import theApp
        theApp.CreateSvc += [ "ZeeDAnalysisSvc" ]

