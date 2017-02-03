ZeeDAnalysisSvc.Source = "AOD"

if not ('ZeeDInputFiles' in dir()):
  # reference file is a ttbar event, see:
  # /afs/cern.ch/atlas/maxidisk/d33/referencefiles/aod/README
  # ZeeDPoolAODInput=['/afs/cern.ch/atlas/maxidisk/d33/referencefiles/aod/AOD-17.0.6/AOD-17.0.6-full.pool.root']
  #ZeeDPoolAODInput=['/scratch/hh/current/atlas/users/sschmitt/ZeeD/samples/data11_7TeV.00189207.physics_Egamma.merge.AOD.f404_m980/data11_7TeV.00189207.physics_Egamma.merge.AOD.f404_m980._lb0015._0001.1']
  ZeeDPoolAODInput=['/scratch/hh/current/atlas/users/star/ZeeD/mc12_8TeV.147806.PowhegPythia8_AU2CT10_Zee.merge.AOD.e1169_s1469_s1470_r3542_r3549_tid779180_00/AOD.779180._001416.pool.root.1'] 
  #ZeeDPoolAODInput=['/scratch/hh/current/atlas/users/star/ZeeD/data12_8TeV.00201556.physics_Egamma.merge.AOD.f437_m1126/data12_8TeV.00201556.physics_Egamma.merge.AOD.f437_m1126._lb0830-lb0831._0001.1']
else:
  ZeeDPoolAODInput=ZeeDInputFiles

from AthenaCommon.AthenaCommonFlags import jobproperties as jp
jp.AthenaCommonFlags.FilesInput = ZeeDPoolAODInput

#from RecExConfig.RecFlags import rec

include("RecExCond/RecExCommon_flags.py")
include("RecExCond/AllDet_detDescr.py")
include( "AthenaPoolCnvSvc/ReadAthenaPool_jobOptions.py" )

ZeeDisData = checkIfData()
ZeeDisMC = not ZeeDisData

if ZeeDisData :
    ZeeDAnalysisSvc.InputType = "data"
else :
    ZeeDAnalysisSvc.InputType = "mc"



# Lumi--------------------------------------------------------------------------
try:
    checkVariable("ZeeDdoLumi", ZeeDdoLumi)
except NameError:
    ZeeDdoLumi = defaultVariable("ZeeDdoLumi", False)

if ZeeDdoLumi and ZeeDisData:
    include("ZeeDAnalysis/ZeeDAnalysis_Lumi.py")
#-------------------------------------------------------------------------------

# Trigger ----------------------------------------------------------------------
try:
    checkVariable("ZeeDdoTrigger", ZeeDdoTrigger)
except NameError:
    ZeeDdoTrigger = defaultVariable("ZeeDdoTrigger", True)

if ZeeDdoTrigger:
    include("ZeeDAnalysis/ZeeDAnalysis_Trigger.py")
    # move to ZeeDAnalysis_Trigger.py?
    from TriggerJobOpts.TriggerConfigGetter import TriggerConfigGetter
    cfg = TriggerConfigGetter("ReadPool")

    trigger = ["EF_e20_medium", "EF_e22_medium",
               "EF_2e12_medium", "EF_2e12T_medium",
               "EF_e22vh_medium1", "EF_e45_medium1",
               "EF_2e12Tvh_medium", "EF_2g20_loose",
               "EF_e24vhi_medium1", "EF_2e12Tvh_loose1",
               "EF_2e12Tvh_loose1_L2StarB", "EF_e60_medium1",
               "EF_e12Tvh_medium1_mu8"]

    ZeeDAnalysisSvc.TriggerEvent= trigger

    ZeeDAnalysisSvc.L1TriggerElecMatch=[]
    ZeeDAnalysisSvc.L2TriggerElecMatch=[]
    ZeeDAnalysisSvc.EFTriggerElecMatch= trigger
#-------------------------------------------------------------------------------

# MCTruthClassifier ------------------------------------------------------------
if ZeeDisMC:
  import MCTruthClassifier.MCTruthClassifierBase
  from  MCTruthClassifier.MCTruthClassifierConf import MCTruthClassifier
  ToolSvc += MCTruthClassifier()
#-------------------------------------------------------------------------------

# egammaIsolationMVATopTool ----------------------------------------------------
from egammaPIDTools.egammaIsolationMVATopToolBase import egammaIsolationMVATopToolBase
theEgammaIsolationMVAToolBase=egammaIsolationMVATopToolBase("egammaIsolationMVATopToolBase")
theEgammaIsolationMVAToolBase.EMShowerContainerName = "egDetailAOD"
ToolSvc+=theEgammaIsolationMVAToolBase
ZeeDAnalysisSvc.EgammaIsolationMVATopTool = theEgammaIsolationMVAToolBase

# GRL---------------------------------------------------------------------------
try:
    checkVariable("ZeeDdoGRL", ZeeDdoGRL)
except NameError:
    ZeeDdoGRL = defaultVariable("ZeeDdoGRL", False)

if ZeeDdoGRL and ZeeDisData:
    include("ZeeDAnalysis/ZeeDAnalysis_GRL.py")
#-------------------------------------------------------------------------------

ServiceMgr.EventSelector.BackNavigation   = False
