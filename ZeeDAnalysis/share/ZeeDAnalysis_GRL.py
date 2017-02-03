# Configure the goodrunslist selector tool
from GoodRunsLists.GoodRunsListsConf import *
ToolSvc += GoodRunsListSelectorTool()

if 'ZeeDGRLVec' in dir():
  print "ZeeDGRLVec is set to:",ZeeDGRLVec
else:
  print "ZeeDGRLVec is not set. This does not work! Exiting..."
  exit()

GoodRunsListSelectorTool.GoodRunsListVec = [ ZeeDGRLVec ]

## AthFilterSeq is always executed before the top sequence, and is configured such that
## any follow-up sequence (eg. top sequence) is not executed in case GRLTriggerAlg1 does
## not pass the event
## In short, the sequence AthFilterSeq makes sure that all algs in the job sequence
## are skipped when an event gets rejects
from AthenaCommon.AlgSequence import AthSequencer
from GoodRunsListsUser.GoodRunsListsUserConf import *
seq = AthSequencer("AthFilterSeq")
seq += GRLTriggerSelectorAlg('GRLTriggerAlg1')

if 'ZeeDGRLArray' in dir():
  print "ZeeDGRLArray is set to:",ZeeDGRLArray
else:
  print "ZeeDGRLArray is not set. This does not work! Exiting..."
  exit()

seq.GRLTriggerAlg1.GoodRunsListArray = [ ZeeDGRLArray ]        ## pick up correct name from inside xml file!
