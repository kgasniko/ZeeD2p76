# Helper------------------------------------------------------------------------
include("ZeeDAnalysis/ZeeDAnalysis_Helper.py")
ZeeDOutputLevel=ERROR
ZeeDEvtMax=-1
ZeeDCalcJets=False
ZeeDTruthJets=False
ZeeDUseTrigger=True
ZeeDDoTriggerMatching=True
ZeeDreadType=2
ZeeDOneBinMuonSF = True
ZeeDChargedMuonSD= False

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

#Muon trigger SF
try:
    checkVariable("ZeeDOneBinMuonSF", ZeeDOneBinMuonSF)
except NameError:
    ZeeDOneBinMuonSF=defaultVariable("ZeeDOneBinMuonSF", True)
ZeeDAnalysisSvc.MuonOneBinTriggerSF=ZeeDOneBinMuonSF

try:
    checkVariable("ZeeDChargedMuonSD",ZeeDChargedMuonSD)
except NameError:
    ZeeDChargedMuonSD=defaultVariable("ZeeDChargedMuonSD", False)
ZeeDAnalysisSvc.MuonChargedTriggerSF=ZeeDChargedMuonSD

# Read in the files ------------------------------------------------------------
try:
    checkVariable("ZeeDreadType", ZeeDreadType)
except NameError:
    ZeeDreadType = defaultVariable("ZeeDreadType", readType.AOD)

if ZeeDreadType == readType.AOD:
    include("ZeeDAnalysis/ZeeDAnalysis_ReadAOD.py")
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

# TTree Writing ----------------------------------------------------------------
try:
    checkVariable("ZeeDwriteTTree", ZeeDwriteTTree)
except NameError:
    ZeeDwriteTTree = defaultVariable("ZeeDwriteTTree", False)

ZeeDAnalysisSvc.WriteTTreeWithCutHistManager = ZeeDwriteTTree

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
    #ZeeDAnalysisSvc.DoTruthMatching = False
    #ZeeDAnalysisSvc.FillGenInfo = False
    ZeeDAnalysisSvc.ApplyPtZReweighting = False

    
# Systematics ------------------------------------------------------------------
try:
    checkVariable("ZeeDEvaluateSystematics", ZeeDEvaluateSystematics)
except NameError:
    ZeeDEvaluateSystematics = defaultVariable("ZeeDEvaluateSystematics", False)

try:
    checkVariable("ZeeDMakeToyMC", ZeeDMakeToyMC)
except NameError:
    ZeeDMakeToyMC = defaultVariable("ZeeDMakeToyMC", False)

ZeeDAnalysisSvc.DoCombToyMC=ZeeDMakeToyMC


ZeeDAnalysisSvc.EvaluateSystematics = ZeeDEvaluateSystematics

# add ZeedLauncher as algorithm ------------------------------------------------
# prepare topSequence
from AthenaCommon.AlgSequence import AlgSequence
from ZeeDAnalysis.ZeeDAnalysisConf import ZeeDLauncher
topSequence = AlgSequence()
topSequence += ZeeDLauncher()

# message level for ZeeDLauncher algorithm
topSequence.ZeeDLauncher.OutputLevel = ZeeDOutputLevel

# ServiceMgr -------------------------------------------------------------------
# Set output level threshold (2=DEBUG, 3=INFO, 4=WARNING, 5=ERROR, 6=FATAL )
ServiceMgr.MessageSvc.OutputLevel = ZeeDOutputLevel

# Configure AthenaEventLoopMgr here
from AthenaServices.AthenaServicesConf import AthenaEventLoopMgr
ServiceMgr += AthenaEventLoopMgr()
ServiceMgr.AthenaEventLoopMgr.EventPrintoutInterval = 1000
ServiceMgr.AthenaEventLoopMgr.OutputLevel = INFO

# THistSvc----------------------------------------------------------------------
# setup TTree registration Service
# save ROOT histograms and Tuple
from GaudiSvc.GaudiSvcConf import THistSvc
ServiceMgr += THistSvc()
ServiceMgr.THistSvc.Output = ["ZEED DATAFILE='ZeeDAnalysis.root' OPT='RECREATE'"]
ServiceMgr.THistSvc.MaxFileSize = -1

# Limit output -----------------------------------------------------------------
for n,c in Configurable.allConfigurables.iteritems():
    if n not in ("ZeeDAnalysisSvc", "AthenaEventLoopMgr", "LumiBlockMetaDataTool") :
#    if True in [s in n for s in ("ToolSvc", "MagFieldAthenaSvc", "IOVDbSvc", "Muon", "Mu")]:
        c.OutputLevel = ZeeDOutputLevel
        print "quiet [%s]" % (n,)

# Floating Point Exception Service---------------------------------------------
theApp.CreateSvc += ["FPEControlSvc"]

# abort when there is an unchecked status code --------------------------------
StatusCodeSvc.AbortOnError=False

# - GRL setting -----------------------------
# 2012
ZeeDAnalysisSvc.DoManualGRL = True 
ZeeDAnalysisSvc.ManualGRLFilename =  '../share/GRL/data13_2p76TeV.periodAllYear_DetStatus-v60-pro15_DQDefects-00-01-00_PHYS_HeavyIonP_All_Good.xml'
#ZeeDAnalysisSvc.ManualGRLFilename =  '../share/GRL/data12_8TeV.periodAllYear_DetStatus-v61-pro14-02_DQDefects-00-01-00_PHYS_StandardGRL_All_Good.xml'


ZeeDAnalysisSvc.MCCampaign = 'HI'
ZeeDAnalysisSvc.GenPileupMCFile = False
ZeeDAnalysisSvc.PileupDataFile = '../share/ilumicalc_histograms_EF_e10_medium1_219171-219364.root'
ZeeDAnalysisSvc.MCAssignRandomRunNumber = False 
ZeeDAnalysisSvc. ApplyPileupReweighting = False

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
