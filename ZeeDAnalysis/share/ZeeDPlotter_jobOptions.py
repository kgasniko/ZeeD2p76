# Options for the ZeeDPlotter

from AthenaCommon.AlgSequence import AlgSequence
topSequence = AlgSequence()

from ZeeDAnalysis.ZeeDAnalysisConf import ZeeDPlotter
topSequence += ZeeDPlotter()

topSequence.ZeeDPlotter.SetLogPlots = False

topSequence.ZeeDPlotter.FileNames = ["out.root"]
topSequence.ZeeDPlotter.DirNames  = ["All/NoShift/W"]

topSequence.ZeeDPlotter.CutSelector = "All/NoShift/ZCC"
topSequence.ZeeDPlotter.CutFlow   = [
"",
"LAr"
"PriVtxZ",
"NTracksAtPrimVtx",
"ElecClustEtaMaxW",
"EtaCrackElecW",
"ElecClustEtMinW",
"ElMediumPP",
"TriggersW",
"EtMissMinW",
"MassTransvW",
]


topSequence.ZeeDPlotter.DataOperation = ""
topSequence.ZeeDPlotter.McOperation = []

# message level
topSequence.ZeeDPlotter.OutputLevel = WARNING

# Number of Events to process
theApp.EvtMax = 1
