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

# pileup -------------------------------------------------------------------


ZeeDAnalysisSvc.MCCampaign             = 'mc12a'
ZeeDAnalysisSvc.ApplyPileupReweighting = False
ZeeDAnalysisSvc.GenPileupMCFile        = True
ZeeDAnalysisSvc.PileupMCFile           = 'Pileup/mc12a_PeriodABPowherSherpa.prw.root'
ZeeDAnalysisSvc.PileupDataFile         = 'Pileup/ilumicalc_histograms_None_200841-210308.root'


ZeeDAnalysisSvc.ApplyPtZReweighting = False

ZeeDAnalysisSvc.UseTrigger = True
ZeeDAnalysisSvc.MCAssignRandomRunNumber = False

ZeeDAnalysisSvc.RunZCFAna= False
ZeeDAnalysisSvc.RunZCCAna= False
        
ZeeDAnalysisSvc.DoTruthMatching = False
ZeeDAnalysisSvc.FillGenInfo = False
        

ZeeDAnalysisSvc.CalcJets = False
ZeeDAnalysisSvc.CalcZ    = False



        
# - GRL setting -----------------------------
# 2012
ZeeDAnalysisSvc.DoManualGRL = True   
ZeeDAnalysisSvc.ManualGRLFilename = 'data12_8TeV.periodAllYear_DetStatus-v54-pro13-04_DQDefects-00-00-33_PHYS_StandardGRL_All_Good.xml'
# TTree Writing ----------------------------------------------------------------
try:
    checkVariable("ZeeDwriteTTree", ZeeDwriteTTree)
except NameError:
    ZeeDwriteTTree = defaultVariable("ZeeDwriteTTree", False)

ZeeDAnalysisSvc.WriteTTreeWithCutHistManager = ZeeDwriteTTree

# Systematics ------------------------------------------------------------------
try:
    checkVariable("ZeeDEvaluateSystematics", ZeeDEvaluateSystematics)
except NameError:
    ZeeDEvaluateSystematics = defaultVariable("ZeeDEvaluateSystematics", False)

ZeeDAnalysisSvc.EvaluateSystematics = ZeeDEvaluateSystematics
if ZeeDEvaluateSystematics == True :
       ZeeDAnalysisSvc.ApplyElecEnergyScaleCorr    = True
       ZeeDAnalysisSvc.ApplyElecEnergySmearingCorr = False
       ZeeDAnalysisSvc.ApplyPtZReweighting         = False
       ZeeDAnalysisSvc.ApplyCentralIDCorrection    = False
       ZeeDAnalysisSvc.ApplyForwardIDCorrection    = False
       ZeeDAnalysisSvc.ApplyElecIsoCorr            = False
       ZeeDAnalysisSvc.ApplyRecoCorrection         = False
       ZeeDAnalysisSvc.ApplyChargeCorrection       = False
       ZeeDAnalysisSvc.ApplyPileupReweighting      = False
       ZeeDAnalysisSvc.ApplyZPosVtxReweighting     = False


if ZeeDwriteTTree == True :
     ZeeDAnalysisSvc.TTreeOutput = "ZeeDAnalysisTTreeOut.root"

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
#ServiceMgr.THistSvc.Output = ["ZEEDPU DATAFILE='TPileupReweighting.prw.root' OPT='RECREATE'"]
#ServiceMgr.THistSvc.MaxFileSize = -1

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
