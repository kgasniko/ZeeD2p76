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
    ZeeDOutputLevel = defaultVariable("ZeeDOutputLevel", ERROR)

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
else:
    print "Wrong readType", ZeeDreadType
    quit()

if ZeeDreadType == readType.AOD or ZeeDreadType == readType.TTree:
    ServiceMgr.EventSelector.InputCollections = jp.AthenaCommonFlags.FilesInput()
    print "ZeeD INFO: Using file(s)", ServiceMgr.EventSelector.InputCollections

# TTree Writing ----------------------------------------------------------------
try:
    checkVariable("ZeeDwriteTTree", ZeeDwriteTTree)
except NameError:
    ZeeDwriteTTree = defaultVariable("ZeeDwriteTTree", False)

ZeeDAnalysisSvc.WriteTTreeWithCutHistManager = ZeeDwriteTTree


# GEO21 em correction ----------------------------------------------------------
from egammaD3PDAnalysis.egammaSwTool import egammaSwToolNone

ZeeDAnalysisSvc.ClusterCorrectionTool = egammaSwToolNone()


# Jets ------------------------------------------------------------------

try:
    checkVariable("ZeeDCalcJets", ZeeDCalcJets)
except NameError:
    ZeeDCalcJets = defaultVariable("ZeeDCalcJets", True)
    
ZeeDAnalysisSvc.CalcJets = ZeeDCalcJets

if ZeeDCalcJets == True :
    try:
        checkVariable("ZeeDJetCollections", ZeeDJetCollections)
        
    except NameError:
        ZeeDJetCollections = [ 'AntiKt4TopoEMNewJets','AntiKt6TopoEMNewJets','AntiKt4LCTopoNewJets','AntiKt6LCTopoNewJets' ]
        #ZeeDJetCollections = [ 'AntiKt4TopoEMJets','AntiKt6TopoEMJets','AntiKt4LCTopoJets','AntiKt6LCTopoJets' ]

    ZeeDAnalysisSvc.JetCollectionNames = ZeeDJetCollections


    from JetSelectorTools.ConfiguredAthJetCleaningTools import *
    ToolSvc += ConfiguredAthJetCleaningTool_Loose( "AthJetCleaningTool",OutputLevel = DEBUG )
    ZeeDAnalysisSvc.JetCleaningTool=ToolSvc.AthJetCleaningTool

    try:
        checkVariable("ZeeDTruthJets", ZeeDTruthJets)
    except NameError:
        ZeeDTruthJets=False
        
    if ZeeDTruthJets == True :
        TruthJetsCollections = ['AntiKt4TruthJets','AntiKt6TruthJets', 'AntiKt4TruthJets_WZ','AntiKt6TruthJets_WZ' ]
        ZeeDJetCollections.extend(TruthJetsCollections)

    try:
        checkVariable("ZeeDRecalibrateJets", ZeeDRecalibrateJets )
    except NameError :
        ZeeDRecalibrateJets = False

    if ZeeDRecalibrateJets == True :
          ZeeDAnalysisSvc.RecalibrateJets = ZeeDRecalibrateJets
          ZeeDAnalysisSvc.RecalibrateJetsConfig = "share/data/CalibrationConfigs/GSC_March2013.config"

    try:
        checkVariable("ZeeDUseTrigger", ZeeDUseTrigger )
    except NameError :
        ZeeDUseTrigger = True

    ZeeDAnalysisSvc.UseTrigger              = ZeeDUseTrigger

    try:
        checkVariable("ZeeDDoTriggerMatching", ZeeDDoTriggerMatching )
    except NameError :
        ZeeDDoTriggerMatching = True

    ZeeDAnalysisSvc.DoTriggerMatching       = ZeeDDoTriggerMatching

    try:
        checkVariable("ZeeDDoTruthMatching", ZeeDDoTruthMatching )
    except NameError :
        ZeeDDoTruthMatching = False

    ZeeDAnalysisSvc.DoTruthMatching         = ZeeDDoTruthMatching
    
    try:
        checkVariable("ZeeDFillGenInfo", ZeeDFillGenInfo )
    except NameError :
        ZeeDFillGenInfo = False
    
    ZeeDAnalysisSvc.FillGenInfo             = ZeeDFillGenInfo

    ZeeDAnalysisSvc.RunZCFAna               = False
    ZeeDAnalysisSvc.RunZCCAna               = False
    ZeeDAnalysisSvc.FillNoCuts              = False

    ZeeDAnalysisSvc.ApplyPtZReweighting     = False
    ZeeDAnalysisSvc.ApplyNvtxReweighting    = False
    ZeeDAnalysisSvc.ApplyPDFReweighting     = False
    ZeeDAnalysisSvc.ApplyZPosVtxReweighting = False
    ZeeDAnalysisSvc.ApplyNvtxReweighting    = False

    ZeeDAnalysisSvc.ApplyElecIsoCorr                 = False
    ZeeDAnalysisSvc.ApplyNTrkVtxReweighting          = False
    ZeeDAnalysisSvc.ApplyCentralIDCorrection         = False
    ZeeDAnalysisSvc.ApplyForwardIDCorrection         = False
    ZeeDAnalysisSvc.ApplyRecoCorrection              = False
    ZeeDAnalysisSvc.ApplyChargeCorrection            = False
    ZeeDAnalysisSvc.ApplyLineShapeCorrection         = False
    ZeeDAnalysisSvc.ApplyElecEnergyScaleCorr         = False
    ZeeDAnalysisSvc.ApplyElecEnergySmearingCorr      = False
    ZeeDAnalysisSvc.ApplyZeeDElecEnergyScaleCorr         = False
    ZeeDAnalysisSvc.ApplyZeeDElecEnergySmearingCorr      = False

    ZeeDAnalysisSvc.EvalSysTrigCorr                  = False
    ZeeDAnalysisSvc.EvalSysTrigCorrMC                = False
    ZeeDAnalysisSvc.EvalSysPileupReweighting         = False

    ZeeDAnalysisSvc.MCAssignRandomRunNumber = False


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
ZeeDAnalysisSvc.DoManualGRL = False
#ZeeDAnalysisSvc.ManualGRLFilename = 'data12_8TeV.periodAllYear_DetStatus-v58-pro14-01_DQDefects-00-00-33_PHYS_StandardGRL_All_Good.xml'
ZeeDAnalysisSvc.ManualGRLFilename = 'data12_8TeV.periodAllYear_DetStatus-v61-pro14-02_DQDefects-00-01-00_PHYS_StandardGRL_All_Good.xml'

# TTree Writing ----------------------------------------------------------------
try:
    checkVariable("ZeeDwriteTTree", ZeeDwriteTTree)
except NameError:
    ZeeDwriteTTree = defaultVariable("ZeeDwriteTTree", False)

if ZeeDwriteTTree == True : 
    ZeeDAnalysisSvc.TTreeOutput = "ZeeDAnalysisTTreeOut.root"


ZeeDAnalysisSvc.WriteTTreeWithCutHistManager = ZeeDwriteTTree


# Systematics ------------------------------------------------------------------
try:
    checkVariable("ZeeDEvaluateSystematics", ZeeDEvaluateSystematics)
except NameError:
    ZeeDEvaluateSystematics = defaultVariable("ZeeDEvaluateSystematics", False)

ZeeDAnalysisSvc.EvaluateSystematics = ZeeDEvaluateSystematics

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
