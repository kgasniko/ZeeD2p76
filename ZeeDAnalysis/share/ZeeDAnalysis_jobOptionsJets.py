# Helper------------------------------------------------------------------------
include("ZeeDAnalysis/ZeeDAnalysis_Helper.py")

class readType:
    AOD = 1
    TTree = 2
    InlineZ = 3

#-------------------------------------------------------------------------------
try:
    checkVariable("ZeeDOutputLevel", ZeeDOutputLevel)
except NameError:
    ZeeDOutputLevel = defaultVariable("ZeeDOutputLevel", WARNING)

#-------------------------------------------------------------------------------
# get a handle on the ServiceManager which holds all the services
from AthenaCommon.AppMgr import ServiceMgr
for i in ServiceMgr: i.OutputLevel = ZeeDOutputLevel

# ZeeDAnalysisSvc/Algorithm-----------------------------------------------------
from ZeeDAnalysisSvc.ZeeDAnalysisSvcInit import ZeeDAnalysisSvcInit
ZeeDAnalysisSvcInit()
from ZeeDAnalysisSvc.ZeeDAnalysisSvcConfig import ZeeDAnalysisSvc

# Read in the files ------------------------------------------------------------
try:
    checkVariable("ZeeDreadType", ZeeDreadType)
except NameError:
    ZeeDreadType = defaultVariable("ZeeDreadType", readType.AOD)

if ZeeDreadType == readType.AOD:
    include("ZeeDAnalysis/ZeeDAnalysis_ReadAODJets.py")
elif ZeeDreadType == readType.TTree:
    include("ZeeDAnalysis/ZeeDAnalysis_ReadTTree.py")
elif ZeeDreadType == readType.InlineZ:
    ZeeDEvtMax = 1000
    include("ZeeDAnalysis/ZeeDAnalysis_ReadInlineZ.py")
else:
    print "Unknown readType", ZeeDreadType
    quit()

if ZeeDreadType == readType.AOD or ZeeDreadType == readType.TTree:
    ServiceMgr.EventSelector.InputCollections = jp.AthenaCommonFlags.FilesInput()
    print "ZeeD INFO: Using file(s)", ServiceMgr.EventSelector.InputCollections


# Et Density  ------------------------------------------------------------------

if ZeeDreadType == readType.AOD :

    from EventShapeTools.EventEtClusterDensityConfig import *
    # -- common tool settings
    commonDict = dict(getDefaultEtDensityClusterDict())
    commonDict['Algorithm']             = "kt"
    commonDict['JetAreaMethod']         = "VoronoiArea"
    commonDict['VoronoiEffectiveRfact'] = 0.9
    commonDict['InclusivePtMin']        = 0.*GeV
    commonDict['EtaMin']                = -2.
    commonDict['EtaMax']                =  2.
    commonDict['UseAreaFourMomentum']   =  True
    del commonDict['SignalState']



    # -- configuration 1: Kt 0.3 LC  for AntiKt4LCTopo
    ConfLC3Dict = dict(commonDict)
    ConfLC3Dict['Radius']      = 0.3
    ConfLC3Dict['SignalState'] = "CALIBRATED"
    ConfLC3Tool                = configureJetAreaDensity("ClusterRhoKt3LC", **ConfLC3Dict)

    # -- configuration 2: kT, R = 0.4, LC, for AntiKt6LCTopo
    ConfLC4Dict = dict(commonDict)
    ConfLC4Dict['Radius']      = 0.4
    ConfLC4Dict['SignalState'] = "CALIBRATED"
    ConfLC4Tool                 = configureJetAreaDensity("ClusterRhoKt4LC",**ConfLC4Dict)

    # # -- configuration 1: Kt 0.3 EM  for AntiKt4TopoEM
    ConfEM3Dict = dict(commonDict)
    ConfEM3Dict['Radius']      = 0.3
    ConfEM3Dict['SignalState'] = "UNCALIBRATED"
    ConfEM3Tool                = configureJetAreaDensity("ClusterRhoKt3EM", **ConfEM3Dict)

    # # -- configuration 2: kT, R = 0.4, EM, for AntiKt6TopoEM
    ConfEM4Dict = dict(commonDict)
    ConfEM4Dict['Radius']      = 0.4
    ConfEM4Dict['SignalState'] = "UNCALIBRATED"
    ConfEM4Tool                 = configureJetAreaDensity("ClusterRhoKt4EM",**ConfEM4Dict)

    #ZeeDAnalysisSvc.ZeeDEtDensity = [ ConfLC3Tool.getFullName(), ConfLC4Tool.getFullName(),ConfEM3Tool.getFullName(), ConfEM4Tool.getFullName() ]
    #ZeeDAnalysisSvc.ZeeDEtDensity += [   ConfLC3Tool, ConfLC4Tool, ConfEM3Tool, ConfEM4Tool  ]

    ToolSvc +=    ConfLC3Tool
    ToolSvc +=    ConfLC4Tool
    ToolSvc +=    ConfEM3Tool
    ToolSvc +=    ConfEM4Tool
    ZeeDAnalysisSvc.ZeeDEtDensityTools =  [ ConfLC3Tool.getFullName(), ConfLC4Tool.getFullName(),ConfEM3Tool.getFullName(), ConfEM4Tool.getFullName() ]


# Systematics ------------------------------------------------------------------
try:
    checkVariable("ZeeDEvaluateSystematics", ZeeDEvaluateSystematics)
except NameError:
    ZeeDEvaluateSystematics = defaultVariable("ZeeDEvaluateSystematics", False)

ZeeDAnalysisSvc.EvaluateSystematics = ZeeDEvaluateSystematics

#if ZeeDEvaluateSystematics == True :
#       ZeeDAnalysisSvc.ApplyElecEnergyScaleCorr    = True
#       ZeeDAnalysisSvc.ApplyElecEnergySmearingCorr = False
#       ZeeDAnalysisSvc.ApplyPtZReweighting         = False
#       ZeeDAnalysisSvc.ApplyCentralIDCorrection    = False
#       ZeeDAnalysisSvc.ApplyForwardIDCorrection    = False
#       ZeeDAnalysisSvc.ApplyElecIsoCorr            = False
#       ZeeDAnalysisSvc.ApplyRecoCorrection         = False
#       ZeeDAnalysisSvc.ApplyChargeCorrection       = False
#       ZeeDAnalysisSvc.ApplyPileupReweighting      = False
#       ZeeDAnalysisSvc.ApplyZPosVtxReweighting     = False


# Jets ------------------------------------------------------------------
try:
    checkVariable("ZeeDCalcJets", ZeeDCalcJets)
except NameError:
    ZeeDCalcJets = defaultVariable("ZeeDCalcJets", False)
    
ZeeDAnalysisSvc.CalcJets = ZeeDCalcJets

if ZeeDCalcJets == True :
    try:
        checkVariable("ZeeDJetCollections", ZeeDJetCollections)
    except NameError:
        ZeeDJetCollections = ['AntiKt4TopoEMNewJets','AntiKt6TopoEMNewJets','AntiKt4LCTopoNewJets','AntiKt6LCTopoNewJets']

    ZeeDAnalysisSvc.JetCollectionNames = ZeeDJetCollections


    from JetSelectorTools.ConfiguredAthJetCleaningTools import *
    ToolSvc += ConfiguredAthJetCleaningTool_Loose( "AthJetCleaningTool",OutputLevel = DEBUG )
    ZeeDAnalysisSvc.JetCleaningTool=ToolSvc.AthJetCleaningTool

    try:
        checkVariable("ZeeDTruthJets", ZeeDTruthJets)
    except NameError:
        ZeeDTruthJets=False
        
    if ZeeDTruthJets == True :
        TruthJetsCollections = ['AntiKt4TruthJets','AntiKt6TruthJets',
                                'AntiKt4TruthJets_WZ','AntiKt6TruthJets_WZ']
        ZeeDJetCollections.extend(TruthJetsCollections)

    try:
        checkVariable("ZeeDRecalibrateJets", ZeeDRecalibrateJets )
    except NameError :
        ZeeDRecalibrateJets = False

    if ZeeDRecalibrateJets == True :
          ZeeDAnalysisSvc.RecalibrateJets = ZeeDRecalibrateJets
          #ZeeDAnalysisSvc.RecalibrateJetsConfig = "share/data/CalibrationConfigs/JES_Dec11_2012_EtaIntercalib.config"
          #ZeeDAnalysisSvc.RecalibrateJetsConfig = "./share/data/CalibrationConfigs/JES_March2012.config"
          #ZeeDAnalysisSvc.RecalibrateJetsConfig = "share/data/CalibrationConfigs/GSC_March2013.config"
          ZeeDAnalysisSvc.RecalibrateJetsConfig = "XML/ApplyJetCalibration/CalibrationConfigs/JES_Full2012dataset_MC12c_May2014.config"


    if ZeeDEvaluateSystematics == True :
        ZeeDAnalysisSvc.JESUncertaintiesConfig      = "JES_2012/Final/InsituJES2012_14NP.config"
        ZeeDAnalysisSvc.MultiJESUncertaintiesConfig = "JES_2012/Final/InsituJES2012_14NP.config"
        ZeeDAnalysisSvc.PathDirUncertaintiesConfig  = ""

    ZeeDAnalysisSvc.UseTrigger              = True
    ZeeDAnalysisSvc.DoTriggerMatching       = True

    ZeeDAnalysisSvc.RunZCFAna               = False
    ZeeDAnalysisSvc.RunZCCAna               = True
    ZeeDAnalysisSvc.FillNoCuts              = False

    ZeeDAnalysisSvc.ApplyPtZReweighting     = False
    ZeeDAnalysisSvc.ApplyNvtxReweighting    = False
    ZeeDAnalysisSvc.ApplyPDFReweighting     = False
    ZeeDAnalysisSvc.ApplyZPosVtxReweighting = True
    ZeeDAnalysisSvc.ApplyNvtxReweighting    = False

    ZeeDAnalysisSvc.ApplyElecIsoCorr                 = False
    ZeeDAnalysisSvc.ApplyNTrkVtxReweighting          = False
    ZeeDAnalysisSvc.ApplyCentralIDCorrection         = True
    ZeeDAnalysisSvc.ApplyForwardIDCorrection         = False
    ZeeDAnalysisSvc.ApplyRecoCorrection              = True
    ZeeDAnalysisSvc.ApplyChargeCorrection            = False
    ZeeDAnalysisSvc.ApplyLineShapeCorrection         = False
    ZeeDAnalysisSvc.ApplyElecEnergyScaleCorr     = True
    ZeeDAnalysisSvc.ApplyElecEnergySmearingCorr  = True

    ZeeDAnalysisSvc.EvalSysTrigCorr                  = False
    ZeeDAnalysisSvc.EvalSysTrigCorrMC                = False
    ZeeDAnalysisSvc.EvalSysPileupReweighting         = False

    ZeeDAnalysisSvc.MCAssignRandomRunNumber = False
    ZeeDAnalysisSvc.DoTruthMatching         = False
    ZeeDAnalysisSvc.FillGenInfo             = False

    ZeeDAnalysisSvc.DoZjet             = True
    ZeeDAnalysisSvc.DoZjetAna          = False
    ZeeDAnalysisSvc.DoZplusJet         = False
    ZeeDAnalysisSvc.CalcJetCharge      = False
                                                                                                                         

    
# pileup -------------------------------------------------------------------


ZeeDAnalysisSvc.MCCampaign = 'mc12a'
ZeeDAnalysisSvc.GenPileupMCFile = False
#ZeeDAnalysisSvc.PileupMCFile = 'Pileup/mc12a_PeriodABCDEPowhegSherpa.prw.root'
#ZeeDAnalysisSvc.PileupMCFile = 'Pileup/mc12a_defaults.prw.root'
ZeeDAnalysisSvc.PileupMCFile = 'Pileup/mc12a_PeriodABCDEGPowhegSherpa.prw.root'
ZeeDAnalysisSvc.PileupDataFile = 'Pileup/ilumicalc_histograms_None_200841-212272.root'

ZeeDAnalysisSvc.ApplyPileupReweighting = False

# - GRL setting -----------------------------
# 2012
ZeeDAnalysisSvc.DoManualGRL = True
#ZeeDAnalysisSvc.ManualGRLFilename = 'data12_8TeV.periodAllYear_DetStatus-v49-pro13-03_CoolRunQuery-00-04-08_Eg_standard.xml'
#ZeeDAnalysisSvc.ManualGRLFilename = 'data12_8TeV.periodAllYear_DetStatus-v52-pro13-04_CoolRunQuery-00-04-08_Eg_standard.xml'
#ZeeDAnalysisSvc.ManualGRLFilename = 'data12_8TeV.periodAllYear_DetStatus-v53-pro13-04_CoolRunQuery-00-04-08_Eg_standard.xml'
#ZeeDAnalysisSvc.ManualGRLFilename = 'data12_8TeV.periodAllYear_DetStatus-v54-pro13-04_DQDefects-00-00-33_PHYS_StandardGRL_All_Good.xml'
#ZeeDAnalysisSvc.ManualGRLFilename = '../share/GRL/data12_8TeV.periodAllYear_DetStatus-v61-pro14-02_DQDefects-00-01-00_PHYS_StandardGRL_All_Good.xml'
ZeeDAnalysisSvc.ManualGRLFilename =  '../share/GRL/data13_2p76TeV.periodAllYear_DetStatus-v60-pro15_DQDefects-00-01-00_PHYS_HeavyIonP_All_Good.xml'

# TTree Writing ----------------------------------------------------------------
try:
    checkVariable("ZeeDwriteTTree", ZeeDwriteTTree)
except NameError:
    ZeeDwriteTTree = defaultVariable("ZeeDwriteTTree", False)

if ZeeDwriteTTree == True : 
    ZeeDAnalysisSvc.TTreeOutput = "ZeeDAnalysisTTreeOut.root"


ZeeDAnalysisSvc.WriteTTreeWithCutHistManager = ZeeDwriteTTree


# add ZeedLauncher as algorithm ------------------------------------------------
# prepare topSequence
from AthenaCommon.AlgSequence import AlgSequence
topSequence = AlgSequence()


from ZeeDAnalysis.ZeeDAnalysisConf import ZeeDLauncher
topSequence += ZeeDLauncher()

# message level for ZeeDLauncher algorithm
topSequence.ZeeDLauncher.OutputLevel = ZeeDOutputLevel

# ServiceMgr -------------------------------------------------------------------
# Set output level threshold (2=DEBUG, 3=INFO, 4=WARNING, 5=ERROR, 6=FATAL )
ServiceMgr.MessageSvc.OutputLevel = ZeeDOutputLevel

# Configure AthenaEventLoopMgr here
from AthenaServices.AthenaServicesConf import AthenaEventLoopMgr
ServiceMgr += AthenaEventLoopMgr()
ServiceMgr.AthenaEventLoopMgr.EventPrintoutInterval = 100
ServiceMgr.AthenaEventLoopMgr.OutputLevel = INFO

# THistSvc----------------------------------------------------------------------
# setup TTree registration Service
# save ROOT histograms and Tuple
from GaudiSvc.GaudiSvcConf import THistSvc
ServiceMgr += THistSvc()
ServiceMgr.THistSvc.Output = ["ZEED DATAFILE='ZeeDAnalysis.root' OPT='RECREATE'"]
ServiceMgr.THistSvc.MaxFileSize = -1

# Limit output -----------------------------------------------------------------
#for n,c in Configurable.allConfigurables.iteritems():
#    if n not in ("ZeeDAnalysisSvc", "AthenaEventLoopMgr", "LumiBlockMetaDataTool") :
#    if True in [s in n for s in ("ToolSvc", "MagFieldAthenaSvc", "IOVDbSvc", "Muon", "Mu")]:
#        c.OutputLevel = ZeeDOutputLevel
#        print "quiet [%s]" % (n,)

# Floating Point Exception Service---------------------------------------------
theApp.CreateSvc += ["FPEControlSvc"]

# abort when there is an unchecked status code --------------------------------
StatusCodeSvc.AbortOnError=False

# Number of events to be processed---------------------------------------------
if 'ZeeDEvtMax' in dir():
    theApp.EvtMax = ZeeDEvtMax
else:
    theApp.EvtMax = -1

#settings_ZeeDAnalysisSvc = {"MCtype": "foo"}

try:
    settings_ZeeDAnalysisSvc

    #map(lambda x: setattr(*x), [(ZeeDAnalysisSvc,key,value) for key,value in settings_ZeeDAnalysisSvc.iteritems() if value != "<no value>"])

    for key,value in settings_ZeeDAnalysisSvc.iteritems():
        if value != "<no value>":
            print "setting ZeeDAnalysisSvc.%s = %s" % (key, value)
            if ZeeDAnalysisSvc().properties().has_key(key):
                setattr(ZeeDAnalysisSvc, key, value)
            else:
                print "Cannot set ZeeDAnalysisSvc.%s to %s" % (key, value)
                print "ZeeDAnalysisSvc does not have the property:", key
                theApp.exit(1)

except NameError:
    pass
