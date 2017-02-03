ZeeDAnalysisSvc.Source = "AOD"

if not ('ZeeDInputFiles' in dir()):
  #ZeeDPoolAODInput=['/afs/desy.de/user/k/kgasniko/xxl/17.2.7.5.20/ZeeD/ZeeDAnalysis/run/AOD.779180._000005.pool.root.1']
  #ZeeDPoolAODInput=['/scratch/hh/current/atlas/users/star/ZeeD/mc12_8TeV.147806.PowhegPythia8_AU2CT10_Zee.merge.AOD.e1169_s1469_s1470_r3542_r3549_tid779180_00/AOD.779180._001416.pool.root.1'] 
  #ZeeDPoolAODInput=['/scratch/hh/current/atlas/users/star/ZeeD/data12_8TeV.00201556.physics_Egamma.merge.AOD.f437_m1126/data12_8TeV.00201556.physics_Egamma.merge.AOD.f437_m1126._lb0830-lb0831._0001.1']  
  ZeeDPoolAODInput=['/nfs/dust/atlas/user/kgasniko/data13_2p76TeV.00219296.physics_HardProbes.merge.AOD.f519_m1304/data13_2p76TeV.00219296.physics_HardProbes.merge.AOD.f519_m1304._lb0108._0001.1']
  #ZeeDPoolAODInput=['/nfs/dust/atlas/user/kgasniko/mc12_2TeV.147803.PowhegPythia8_AU2CT10_Wminenu.merge.AOD.e1876_s1647_s1586_r4935_r4516_tid01358666_00/AOD.01358666._000008.pool.root.1']
  #ZeeDPoolAODInput=['/nfs/dust/atlas/user/kgasniko/mc12_2TeV.147801.PowhegPythia8_AU2CT10_Wplusmunu.merge.AOD.e1876_s1647_s1586_r4935_r4516_tid01358664_00/AOD.01358664._000035.pool.root.1']
  # ZeeDPoolAODInput=['/pnfs/desy.de/atlas/dq2/atlaslocalgroupdisk/rucio/data13_2p76TeV/06/b3/data13_2p76TeV.00219305.physics_HardProbes.merge.AOD.f519_m1304._lb0339._0001.1']
else:
  ZeeDPoolAODInput=ZeeDInputFiles

from AthenaCommon.AthenaCommonFlags import jobproperties as jp
jp.AthenaCommonFlags.FilesInput = ZeeDPoolAODInput


include( "AthenaPoolCnvSvc/ReadAthenaPool_jobOptions.py" )
include("RecExCond/RecExCommon_flags.py")
include("RecExCond/AllDet_detDescr.py")



ZeeDisData = checkIfData()
ZeeDisMC = not ZeeDisData


if ZeeDisData :
    ZeeDAnalysisSvc.InputType = "data"
else :
    ZeeDAnalysisSvc.InputType = "mc"
ZeeDAnalysisSvc.InputType = "mc"

#algMgr=theApp.getHandle() 	# get the algo manager
#algMgr.algorithms()

from AthenaCommon.AppMgr import ToolSvc
from AthenaCommon.AppMgr import ServiceMgr as svcMgr

from RecExConfig.RecFlags import rec	
from AthenaCommon.SystemOfUnits import GeV
from IOVDbSvc.CondDB import conddb

from JetRec.JetGetters import *
from JetRec.JetRecConf import *
from JetRec.JetRecFlags import jetFlags
from JetRec.JetRec_defaults import *
from JetCalibTools.JetCalibToolsConf import *
from JetCalibTools.MakeCalibSequences import *
from JetCalibTools.SetupJetCalibrators import doJetOriginCorrection
from ApplyJetCalibration.SetupAthenaCalibration import doApplyJES

from RecExConfig.RecAlgsFlags import recAlgs
recAlgs.doMissingET.set_Value_and_Lock(False)

WidthT = JetWidthTool("JetWidth")

from AthenaCommon.AthenaCommonFlags import athenaCommonFlags
athenaCommonFlags.RuntimeStrictness = 'none'
#'abort' 'warn' 'none'
#athenaCommonFlags.property = value

rec.doFloatingPointException = True

#if rec.readAOD():
jetFlags.inputFileType = 'AOD'

from EventShapeTools.EventEtClusterDensityConfig import configureJetAreaDensity
from EventShapeTools.EventEtClusterDensityConfig import getDefaultEtDensityClusterDict
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
ConfLC4Tool                = configureJetAreaDensity("ClusterRhoKt4LC",**ConfLC4Dict)
# -- configuration 1: Kt 0.3 EM  for AntiKt4TopoEM
ConfEM3Dict = dict(commonDict)
ConfEM3Dict['Radius']      = 0.3
ConfEM3Dict['SignalState'] = "UNCALIBRATED"
ConfEM3Tool                = configureJetAreaDensity("ClusterRhoKt3EM", **ConfEM3Dict)

# -- configuration 2: kT, R = 0.4, EM, for AntiKt6TopoEM
ConfEM4Dict = dict(commonDict)
ConfEM4Dict['Radius']      = 0.4
ConfEM4Dict['SignalState'] = "UNCALIBRATED"
ConfEM4Tool                = configureJetAreaDensity("ClusterRhoKt4EM",**ConfEM4Dict)

ToolSvc +=    ConfLC3Tool
ToolSvc +=    ConfLC4Tool
ToolSvc +=    ConfEM3Tool
ToolSvc +=    ConfEM4Tool
ZeeDAnalysisSvc.ZeeDEtDensityTools =  [ ConfLC3Tool.getFullName(), ConfLC4Tool.getFullName(),ConfEM3Tool.getFullName(), ConfEM4Tool.getFullName() ]

from AthenaCommon.AlgSequence import AlgSequence
topSequence = AlgSequence()
##from EventShapeRec.EventShapeRecConf import EventEtDensityTester

# # Check if an alg already exists 
# if hasattr( topSequence, "EventEtDensityTester" ):
#   alg  = topSequence.EventEtDensityTester
# elif hasattr( AlgSequence(), "EventEtDensityTester" ):
#   alg  = seq.EventEtDensityTester
# else:
#   alg = EventEtDensityTester("EventEtDensityTester", FillHistos=False, FillEventStore=True)
#   topSequence += alg        
  
# #rhoTool= areaDensityTool(Radius, SignalState=="CALIBRATED" )
  
# alg.EventEtDensityTools += [ ConfLC3Tool, ConfLC4Tool, ConfEM3Tool, ConfEM4Tool ] 


# from EventShapeRec.EventEtDensityConfig import calculateSimpleEventDensity
# calculateSimpleEventDensity(Radius=0.4, SignalState="UNCALIBRATED")
# calculateSimpleEventDensity(Radius=0.4, SignalState="CALIBRATED")
# calculateSimpleEventDensity(Radius=0.6, SignalState="UNCALIBRATED")
# calculateSimpleEventDensity(Radius=0.6, SignalState="CALIBRATED")


def myjets():


  from AthenaCommon.AlgSequence import AlgSequence
  topSequence = AlgSequence()

  
  from EventShapeRec.EventShapeRecConf import EventEtDensityTester
  
  # Check if an alg already exists 
  if hasattr( topSequence, "EventEtDensityTester" ):
    alg  = topSequence.EventEtDensityTester
  elif hasattr( AlgSequence(), "EventEtDensityTester" ):
    alg  = seq.EventEtDensityTester
  else:
      alg = EventEtDensityTester("EventEtDensityTester", FillHistos=False, FillEventStore=True)
      topSequence += alg        
      
  alg.EventEtDensityTools += [ ConfLC3Tool, ConfLC4Tool, ConfEM3Tool, ConfEM4Tool ] 

  from EventShapeRec.EventEtDensityConfig import calculateSimpleEventDensity
  calculateSimpleEventDensity(Radius=0.4, SignalState="UNCALIBRATED")
  calculateSimpleEventDensity(Radius=0.4, SignalState="CALIBRATED")
  calculateSimpleEventDensity(Radius=0.6, SignalState="UNCALIBRATED")
  calculateSimpleEventDensity(Radius=0.6, SignalState="CALIBRATED")

  from JetRec.JetGetters  import make_StandardJetGetter
  from JetMomentTools.JetMomentsConfigHelpers import *
  
  specialJet('AntiKt',0.4,'LCTopo', doEMCut=False, minPt=4*GeV, doCalib=True, calibName='LC:ApplyAreaOffsetJES',
             addJetQualityMoments=True, writeMoments=True, outputCollectionName='AntiKt4LCTopoNewJets')
  specialJet('AntiKt',0.6,'LCTopo', doEMCut=False, minPt=4*GeV, doCalib=True, calibName='LC:ApplyAreaOffset',
             addJetQualityMoments=True, writeMoments=True, outputCollectionName='AntiKt6LCTopoNewJets')
  specialJet('AntiKt',0.4,'Topo',   doEMCut=False, minPt=4*GeV, doCalib=True, calibName='EM:ApplyAreaOffsetJES',
             addJetQualityMoments=True, writeMoments=True, outputCollectionName='AntiKt4TopoEMNewJets')
  specialJet('AntiKt',0.6,'Topo',   doEMCut=False, minPt=4*GeV, doCalib=True, calibName='EM:ApplyAreaOffset',
               addJetQualityMoments=True, writeMoments=True, outputCollectionName='AntiKt6TopoEMNewJets')

  
#   from JetRec.JetGetters  import make_StandardJetGetter

#   make_StandardJetGetter('AntiKt',0.4,'LCTopo', doEMCut=False, minPt=4*GeV, doCalib=True, calibName='LC:ApplyAreaOffsetJES',
#                          addJetQualityMoments=True, writeMoments=True, outputCollectionName='AntiKt4LCTopoNewJets')
#   make_StandardJetGetter('AntiKt',0.6,'LCTopo', doEMCut=False, minPt=4*GeV, doCalib=True, calibName='LC:ApplyAreaOffset',
#                          addJetQualityMoments=True, writeMoments=True, outputCollectionName='AntiKt6LCTopoNewJets')
#   make_StandardJetGetter('AntiKt',0.4,'Topo',   doEMCut=False, minPt=4*GeV, doCalib=True, calibName='EM:ApplyAreaOffsetJES',
#                          addJetQualityMoments=True, writeMoments=True, outputCollectionName='AntiKt4TopoEMNewJets')
#   make_StandardJetGetter('AntiKt',0.6,'Topo',   doEMCut=False, minPt=4*GeV, doCalib=True, calibName='EM:ApplyAreaOffset',
#                          addJetQualityMoments=True, writeMoments=True, outputCollectionName='AntiKt6TopoEMNewJets')


#   from JetMomentTools.JetMomentsConfigHelpers import *

  recommendedAreaAndJVFMoments("AntiKt4TopoEMNewJets")
  specialMoment("AntiKt4TopoEMNewJets")
  recommendedAreaAndJVFMoments("AntiKt6TopoEMNewJets")
  specialMoment("AntiKt6TopoEMNewJets")
  recommendedAreaAndJVFMoments("AntiKt4LCTopoNewJets")
  specialMoment("AntiKt4LCTopoNewJets")
  recommendedAreaAndJVFMoments("AntiKt6LCTopoNewJets")
  specialMoment("AntiKt6LCTopoNewJets")

#   specialMoment("AntiKt4TopoEMNewJets", ["origin"])
#   specialMoment("AntiKt6TopoEMNewJets", ["origin"])
#   specialMoment("AntiKt4LCTopoNewJets", ["origin"])
#   specialMoment("AntiKt6LCTopoNewJets", ["origin"])

  
  from JetMomentTools.JetCopyMomentsAlg import JetCopyMomentsAlg
  topSequence+= JetCopyMomentsAlg('AntiKt4LCTopoJetsCopyMomAlg',
                                  JetCollectionSrc  = 'AntiKt4LCTopoJets',
                                  JetCollectionDest = 'AntiKt4LCTopoNewJets',
                                  Moments=  [ 'BCH_CORR_CELL', 'BCH_CORR_JET', 'BCH_CORR_DOTX',
                                              'LArQuality','HECQuality',
                                              'isBadLooseMinus','isBadLoose','isBadMedium','isBadTight',
                                              'EtaOrigin', 'PhiOrigin', 'MOrigin',
                                              'EtaOriginEM', 'PhiOriginEM', 'MOriginEM',
                                              'N_BAD_CELLS', 'ENG_BAD_CELLS', 'n90', 'NumTowers', 'ootFracCells5', 'ootFracCells10'] )
  topSequence+= JetCopyMomentsAlg('AntiKt6LCTopoJetsCopyMomAlg',
                                  JetCollectionSrc  = 'AntiKt6LCTopoJets',
                                  JetCollectionDest = 'AntiKt6LCTopoNewJets',
                                  Moments= [ 'BCH_CORR_CELL', 'BCH_CORR_JET', 'BCH_CORR_DOTX',
                                             'LArQuality','HECQuality',
                                             'isBadLooseMinus','isBadLoose','isBadMedium','isBadTight',
                                              'EtaOrigin', 'PhiOrigin', 'MOrigin',
                                              'EtaOriginEM', 'PhiOriginEM', 'MOriginEM',
                                             'N_BAD_CELLS', 'ENG_BAD_CELLS', 'n90', 'NumTowers', 'ootFracCells5', 'ootFracCells10'] )
  topSequence+= JetCopyMomentsAlg('AntiKt4TopoEMJetsCopyMomAlg',
                                  JetCollectionSrc  = 'AntiKt4TopoEMJets',
                                  JetCollectionDest = 'AntiKt4TopoEMNewJets',
                                  Moments=  [ 'BCH_CORR_CELL', 'BCH_CORR_JET', 'BCH_CORR_DOTX',
                                              'LArQuality','HECQuality',
                                              'isBadLooseMinus','isBadLoose','isBadMedium','isBadTight',
                                              'EtaOrigin', 'PhiOrigin', 'MOrigin',
                                              'EtaOriginEM', 'PhiOriginEM', 'MOriginEM',
                                              'N_BAD_CELLS', 'ENG_BAD_CELLS', 'n90', 'NumTowers', 'ootFracCells5', 'ootFracCells10'] )
  topSequence+= JetCopyMomentsAlg('AntiKt6TopoEMJetsCopyMomAlg',
                                  JetCollectionSrc  = 'AntiKt6TopoEMJets',
                                  JetCollectionDest = 'AntiKt6TopoEMNewJets',
                                  Moments= [ 'BCH_CORR_CELL', 'BCH_CORR_JET', 'BCH_CORR_DOTX',
                                             'LArQuality','HECQuality',
                                             'isBadLooseMinus','isBadLoose','isBadMedium','isBadTight',
                                              'EtaOrigin', 'PhiOrigin', 'MOrigin',
                                              'EtaOriginEM', 'PhiOriginEM', 'MOriginEM',
                                             'N_BAD_CELLS', 'ENG_BAD_CELLS', 'n90', 'NumTowers', 'ootFracCells5', 'ootFracCells10'] )

  from JetD3PDAnalysis.JetMomentCalcConfig import JetMomentCalcConfig
  from JetRec.JetRecFlags import jetFlags
  jetFlags.jetPerformanceJob = True

#   JetMomentCalcConfig ("AntiKt4LCTopoNewJets", 'all', topSequence)
#   JetMomentCalcConfig ("AntiKt6LCTopoNewJets", 'all', topSequence)
#   JetMomentCalcConfig ("AntiKt4TopoEMNewJets", 'all', topSequence)
#   JetMomentCalcConfig ("AntiKt6TopoEMNewJets", 'all', topSequence)


  # truth WZ jets
  if ZeeDisMC :
      make_StandardJetGetter('AntiKt',0.6,'Truth')
      
      from TruthD3PDMaker.TruthJetFilterConfig import TruthJetFilterConfig
      TruthJetFilterConfig (topSequence, writePartons = False, writeHadrons = True, excludeWZdecays = True, photonCone=0.1)
      from ParticleBuilderOptions.AODFlags import AODFlags
      AODFlags.MissingEtTruth = True
      AODFlags.TruthParticleJet = True
      AODFlags.McEventKey="GEN_EVENT"
      antikt4truthAlgWZ = make_StandardJetGetter('AntiKt',0.4,'Truth',disable=False,
                                                 inputCollectionNames=['FilteredD3PDTruth'],
                                                 outputCollectionName='AntiKt4TruthJets_WZ',
                                                 useInteractingOnly=False,
                                                 includeMuons=True
                                                 ).jetAlgorithmHandle()
      antikt6truthAlgWZ = make_StandardJetGetter('AntiKt',0.6,'Truth',disable=False,
                                                 inputCollectionNames=['FilteredD3PDTruth'],
                                                 outputCollectionName='AntiKt6TruthJets_WZ',
                                                 useInteractingOnly=False,
                                                 includeMuons=True
                                                 ).jetAlgorithmHandle()
      




#include("JetMomentsConfigHelpers.py")
rec.UserExecs = ["myjets()"]
include('RecExCommon/RecExCommon_topOptions.py')



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


    trigger = ["EF_e15_loose0", "EF_e15_loose1", "EF_e10_medium1",
               "EF_xe20_FEB_L1EM5", "EF_e7_loose1", "EF_e9_tight1"]

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
