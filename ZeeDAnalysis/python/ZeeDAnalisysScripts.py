__author__  = 'Radist Morse radist.morse@gmail.com'


def InputTTree(filelist):
	#from AthenaCommon.AthenaCommonFlags import jobproperties as jp
	#jp.AthenaCommonFlags.FilesInput = ZeeDTTreeInput
	import AthenaRootComps.ReadAthenaRoot
	from AthenaCommon.AppMgr import ServiceMgr

	ServiceMgr.EventSelector.InputCollections = filelist

	ServiceMgr.EventSelector.TupleName = "T"
	ServiceMgr.EventSelector.ActiveBranches = [ '*' ]

def InputInline():
	from AthenaCommon.AppMgr import ServiceMgr
	ServiceMgr.ZeeDAnalysisSvc.Source = "InlineZ"
	ServiceMgr.ZeeDAnalysisSvc.IsMC = False
	ServiceMgr.ZeeDAnalysisSvc.MCAssignRandomRunNumber = False
	
	trigger = ["EF_e20_medium", "EF_e22_medium",
	           "EF_2e12_medium", "EF_2e12T_medium",
	           "EF_e22vh_medium1", "EF_e45_medium1",
	           "EF_2e12Tvh_medium", "EF_2g20_loose"]
	
	ServiceMgr.ZeeDAnalysisSvc.TriggerEvent= trigger
	
	ServiceMgr.ZeeDAnalysisSvc.L1TriggerElecMatch=[]
	ServiceMgr.ZeeDAnalysisSvc.L2TriggerElecMatch=[]
	ServiceMgr.ZeeDAnalysisSvc.EFTriggerElecMatch= trigger

def InputAOD(filelist):
	from AthenaCommon.AthenaCommonFlags import jobproperties as jp
	jp.AthenaCommonFlags.FilesInput = filelist