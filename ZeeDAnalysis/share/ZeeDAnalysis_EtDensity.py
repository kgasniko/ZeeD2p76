from AthenaCommon.SystemOfUnits import *


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

# -- configuration 1: Kt 0.3 EM  for AntiKt4TopoEM
ConfEM3Dict = dict(commonDict)
ConfEM3Dict['Radius']      = 0.3
ConfEM3Dict['SignalState'] = "UNCALIBRATED"
ConfEM3Tool                = configureJetAreaDensity("ClusterRhoKt3EM", **ConfEM3Dict)

# -- configuration 2: kT, R = 0.4, EM, for AntiKt6TopoEM
ConfEM4Dict = dict(commonDict)
ConfEM4Dict['Radius']      = 0.4
ConfEM4Dict['SignalState'] = "UNCALIBRATED"
ConfEM4Tool                 = configureJetAreaDensity("ClusterRhoKt4EM",**ConfEM4Dict)




ZeeDEtDensity.EventEtDensityTools = [ ConfLC3Tool.getFullName(), 
                                      ConfLC4Tool.getFullName(), 
                                      ConfEM3Tool.getFullName(), 
                                      ConfEM4Tool.getFullName() ]
ZeeDEtDensity += [  ConfLC3Tool, ConfLC4Tool, ConfEM3Tool, ConfEM4Tool ]


