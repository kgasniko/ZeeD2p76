from TrigDecisionTool.TrigDecisionToolConf import Trig__TrigDecisionTool
tdt =Trig__TrigDecisionTool("TrigDecisionTool")
ToolSvc += tdt
tdt.OutputLevel = WARNING
ToolSvc.TrigDecisionTool.Navigation.OutputLevel = ERROR
