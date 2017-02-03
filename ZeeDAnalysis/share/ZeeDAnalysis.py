#!/usr/bin/env physh

from optparse import OptionParser
parser = OptionParser(usage = "usage: %prog", version="%prog $Id: ZeeDAnalisys.py 2200 2012-07-09 13:09:06Z sedov $")
parser.add_option("-o","--output",dest="output", help="Name of the output file")
parser.add_option("-i","--inputType",	dest="type",choices=["inline","ttree","aod"],						help="Type of input ('inline','ttree','aod') (default %default)")
parser.add_option("--isData",			dest="isdata",choices=["data","mc","auto"],							help="Is input MC or data ('data','mc','auto') (default %default)")
parser.add_option("--outputTTree",		dest="ttreeOut",													help="File, to write output TTree. Leave blank to disable.")
parser.add_option("-n","--maxEvt",		dest="maxevents",type=int,											help="Number of events to proceed, -1 for all. Must be set for inline generator (default %default)")
parser.add_option("--outputLevel",		dest="verbose",choices=["DEBUG","INFO","WARNING","ERROR","FATAL"],	help="Verbosity level")
parser.add_option("-q","--quiet",		dest="verbose",action="store_const",const="ERROR",					help="Set verbosity to minimum")
parser.add_option("-v","--verbose",		dest="verbose",action="store_const",const="ALL",					help="Set verbosity to maximum")
parser.add_option("-p","--parameters",	dest="paramfile",													help="Input file with parameters")

parser.set_defaults(output="ZeeDHistograms.root", type="ttree", isdata="auto",ttreeOut="",maxevents=-1,verbose="INFO",paramfile="")

(options, args) = parser.parse_args()

if ((len(args)) and (options.type != "inline")) == 0 :
	print "ERROR! No input provided"
	import sys
	sys.exit(1)

from ZeeDAnalysis.ZeeDAnalisysScripts import *

from AthenaCommon.AppMgr import ServiceMgr
from AthenaCommon.AlgSequence import AlgSequence
topSequence = AlgSequence()

from AthenaCommon import Constants
ServiceMgr.MessageSvc.OutputLevel = Constants.__dict__[options.verbose]

from ZeeDAnalysis.ZeeDAnalysisConf import ZeeDLauncher
topSequence += ZeeDLauncher()

from AthenaServices.AthenaServicesConf import AthenaEventLoopMgr
ServiceMgr += AthenaEventLoopMgr()
ServiceMgr.AthenaEventLoopMgr.EventPrintoutInterval = 1000

theApp.CreateSvc += ["FPEControlSvc"]

theApp.EvtMax = options.maxevents

from ZeeDAnalysisSvc.ZeeDAnalysisSvcInit import ZeeDAnalysisSvcInit
ZeeDAnalysisSvcInit()

if options.isdata == "data" :
	ServiceMgr.ZeeDAnalysisSvc.MCDataAuto = False
	ServiceMgr.ZeeDAnalysisSvc.InputType = "data"
elif options.isdata == "mc" :
	ServiceMgr.ZeeDAnalysisSvc.MCDataAuto = False
	ServiceMgr.ZeeDAnalysisSvc.InputType = "mc"
else :
	ServiceMgr.ZeeDAnalysisSvc.MCDataAuto = True

if options.type == "ttree" :
	ServiceMgr.ZeeDAnalysisSvc.Source = "TTree"
	InputTTree(args[1:])
elif options.type == "inline" :
	ServiceMgr.ZeeDAnalysisSvc.Source = "InlineZ"
	InputInline()
	if theApp.EvtMax < 1 :
		theApp.EvtMax = 1000
elif options.type == "aod" :
	ServiceMgr.ZeeDAnalysisSvc.Source = "AOD"
	InputAOD(args)
else :
	print "Unknown input."
	import sys
	sys.exit(0)

if options.paramfile :
	import ConfigParser
	from ast import literal_eval
	cp = ConfigParser.SafeConfigParser()
	cp.optionxform=str
	cp.read(options.paramfile)
	for key,value in cp.items("ZeeDAnalysisSvc"):
		try:
			ServiceMgr.ZeeDAnalysisSvc.__setattr__(key,literal_eval(value))
		except AttributeError:
			print "Invalid option in",options.paramfile+":",key

from GaudiSvc.GaudiSvcConf import THistSvc
ServiceMgr += THistSvc()
ServiceMgr.THistSvc.Output = ["ZEED DATAFILE='"+options.output+"' OPT='RECREATE'"]

