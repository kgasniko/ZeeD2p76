#Tue Jun 13 15:01:33 2017
"""Automatically generated. DO NOT EDIT please"""
from GaudiKernel.GaudiHandles import *
from GaudiKernel.Proxy.Configurable import *

class ZeeDLauncher( ConfigurableAlgorithm ) :
  __slots__ = { 
    'OutputLevel' : 0, # int
    'Enable' : True, # bool
    'ErrorMax' : 1, # int
    'ErrorCount' : 0, # int
    'AuditAlgorithms' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditReinitialize' : False, # bool
    'AuditRestart' : False, # bool
    'AuditExecute' : False, # bool
    'AuditFinalize' : False, # bool
    'AuditBeginRun' : False, # bool
    'AuditEndRun' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'MonitorService' : 'MonitorSvc', # str
    'RegisterForContextService' : False, # bool
    'EvtStore' : ServiceHandle('StoreGateSvc'), # GaudiHandle
    'DetStore' : ServiceHandle('StoreGateSvc/DetectorStore'), # GaudiHandle
    'UserStore' : ServiceHandle('UserDataSvc/UserDataSvc'), # GaudiHandle
  }
  _propertyDocDct = { 
    'DetStore' : """ Handle to a StoreGateSvc/DetectorStore instance: it will be used to retrieve data during the course of the job """,
    'RegisterForContextService' : """ The flag to enforce the registration for Algorithm Context Service """,
    'UserStore' : """ Handle to a UserDataSvc/UserDataSvc instance: it will be used to retrieve user data during the course of the job """,
    'EvtStore' : """ Handle to a StoreGateSvc instance: it will be used to retrieve data during the course of the job """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(ZeeDLauncher, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'ZeeDAnalysis'
  def getType( self ):
      return 'ZeeDLauncher'
  pass # class ZeeDLauncher

class ZeeDPlotter( ConfigurableAlgorithm ) :
  __slots__ = { 
    'OutputLevel' : 0, # int
    'Enable' : True, # bool
    'ErrorMax' : 1, # int
    'ErrorCount' : 0, # int
    'AuditAlgorithms' : False, # bool
    'AuditInitialize' : False, # bool
    'AuditReinitialize' : False, # bool
    'AuditRestart' : False, # bool
    'AuditExecute' : False, # bool
    'AuditFinalize' : False, # bool
    'AuditBeginRun' : False, # bool
    'AuditEndRun' : False, # bool
    'AuditStart' : False, # bool
    'AuditStop' : False, # bool
    'MonitorService' : 'MonitorSvc', # str
    'RegisterForContextService' : False, # bool
    'EvtStore' : ServiceHandle('StoreGateSvc'), # GaudiHandle
    'DetStore' : ServiceHandle('StoreGateSvc/DetectorStore'), # GaudiHandle
    'UserStore' : ServiceHandle('UserDataSvc/UserDataSvc'), # GaudiHandle
    'FileNames' : [  ], # list
    'DirNames' : [  ], # list
    'DataOperation' : '', # str
    'McOperation' : [  ], # list
    'CutSelector' : '', # str
    'CutFlow' : [  ], # list
    'DataName' : 'out.root', # str
    'Luminosity' : 3948.03, # float
    'LuminosityError' : 0.031, # float
    'SignalMCName' : '', # str
    'SystematicsList' : [  ], # list
    'BosonSelector' : [  ], # list
    'RefHistogramRec' : 'Boson/BosY', # str
    'RefHistogramRecToy' : 'BosY', # str
    'RefHistogramGen' : 'BosY', # str
    'RefHistogramGenToy' : 'BosMt', # str
    'GeneratorHistogram' : 'BosY', # str
    'SetLogPlots' : False, # bool
    'SetNormalize' : False, # bool
    'DoSystematicPlots' : False, # bool
    'DoEfficiencyPlots' : False, # bool
    'NPadX' : 4, # int
    'NPadY' : 3, # int
    'QCDFitHistogram' : 'lepPt', # str
    'QCDCutValue' : 0.0, # float
    'makeQCDFit' : False, # bool
    'makePDFSystematics' : False, # bool
    'PDFSelector' : [  ], # list
    'calcXsec' : False, # bool
  }
  _propertyDocDct = { 
    'DetStore' : """ Handle to a StoreGateSvc/DetectorStore instance: it will be used to retrieve data during the course of the job """,
    'RegisterForContextService' : """ The flag to enforce the registration for Algorithm Context Service """,
    'UserStore' : """ Handle to a UserDataSvc/UserDataSvc instance: it will be used to retrieve user data during the course of the job """,
    'EvtStore' : """ Handle to a StoreGateSvc instance: it will be used to retrieve data during the course of the job """,
  }
  def __init__(self, name = Configurable.DefaultName, **kwargs):
      super(ZeeDPlotter, self).__init__(name)
      for n,v in kwargs.items():
         setattr(self, n, v)
  def getDlls( self ):
      return 'ZeeDAnalysis'
  def getType( self ):
      return 'ZeeDPlotter'
  pass # class ZeeDPlotter
