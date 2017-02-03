ZeeDAnalysisSvc.Source = "InlineZ"
ZeeDAnalysisSvc.InputType = "InlineZ"
ZeeDAnalysisSvc.MCAssignRandomRunNumber = False

trigger = ["EF_e20_medium", "EF_e22_medium",
           "EF_2e12_medium", "EF_2e12T_medium",
           "EF_e22vh_medium1", "EF_e45_medium1",
           "EF_2e12Tvh_medium", "EF_2g20_loose"]

ZeeDAnalysisSvc.TriggerEvent= trigger

ZeeDAnalysisSvc.L1TriggerElecMatch=[]
ZeeDAnalysisSvc.L2TriggerElecMatch=[]
ZeeDAnalysisSvc.EFTriggerElecMatch= trigger
