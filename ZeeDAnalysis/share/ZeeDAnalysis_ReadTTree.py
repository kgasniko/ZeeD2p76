ZeeDAnalysisSvc.Source = "TTree"

import AthenaRootComps.ReadAthenaRoot
import glob

if not ('ZeeDInputFiles' in dir()):
#  ZeeDTTreeInput=["What's the point of providing a DEFAULT INPUT FILE?!?!?! Seriously. And it's long gone anyway"]
#  ZeeDTTreeInput=["/nfs/dust/atlas/user/kgasniko/data/data18_11/TTree_150.root"]
  #ZeeDTTreeInput=["/nfs/dust/atlas/user/kgasniko/data/mc2/user.kgasniko.v1333.mc12_2TeV.147804.PowhegPythia8_AU2CT10_Wminmunu.merge.AOD.e1876_s1647_s1586_r4935_r4516.z2747_EXT0.10308146/user.kgasniko.4418272.EXT0._000022.ZeeDAnalysisTTreeOut.root"]
  #ZeeDTTreeInput=["~/dust/data/mc2/user.kgasniko.v14.mc12_2TeV.147801.PowhegPythia8_AU2CT10_Wplusmunu.merge.AOD.e1876_s1647_s1586_r4935_r4516.z2747_EXT0.10397023/user.kgasniko.4423647.EXT0._000038.ZeeDAnalysisTTreeOut.root"]
  #ZeeDTTreeInput=["/afs/desy.de/user/k/kgasniko/dust/data/mc2/user.kgasniko.v14.mc12_2TeV.147800.PowhegPythia8_AU2CT10_Wplusenu.merge.AOD.e1876_s1647_s1586_r4935_r4516.z2747_EXT0.10400668/user.kgasniko.4423634.EXT0._000040.ZeeDAnalysisTTreeOut.root"]
  #ZeeDTTreeInput=["/afs/desy.de/user/k/kgasniko/dust/dust/dataStaco/TTreeSt.root"]
 #ZeeDTTreeInput=["/pnfs/desy.de/atlas/dq2/atlaslocalgroupdisk/rucio/user/kgasniko/67/db/user.kgasniko.5770950.EXT0._001823.ZeeDAnalysisTTreeOut.root"]
#  ZeeDTTreeInput=["/nfs/dust/atlas/user/kgasniko/data/mc3/user.kgasniko.v26.mc12_2TeV.147800.PowhegPythia8_AU2CT10_Wplusenu.merge.AOD.e1876_s1647_s1586_r4935_r4516.z2747_EXT0.32643863/user.kgasniko.5770858.EXT0._000013.ZeeDAnalysisTTreeOut.root"]
#	ZeeDTTreeInput=["/afs/desy.de/user/k/kgasniko/dust/data/mc3/user.kgasniko.v27.mc12_2TeV.147801.PowhegPythia8_AU2CT10_Wplusmunu.merge.AOD.e1876_s1647_s1586_r4935_r4516.z2747_EXT0.34199377/"]
	#ZeeDTTreeInput=["/afs/desy.de/user/k/kgasniko/dust/data/mc3/user.kgasniko.v27.mc12_2TeV.147801.PowhegPythia8_AU2CT10_Wplusmunu.merge.AOD.e1876_s1647_s1586_r4935_r4516.z2747_EXT0.34199377/user.kgasniko.5887679.EXT0._000001.ZeeDAnalysisTTreeOut.root"]
	ZeeDTTreeInput=["/afs/desy.de/user/k/kgasniko/TreeMaker/run/ZeeDAnalysisTTreeOut.root"]
else:
  ZeeDTTreeInput = ZeeDInputFiles

try:
     checkVariable("ZeeDInputDir", ZeeDInputDir)
     ZeeDTTreeInput=glob.glob(ZeeDInputDir + "/*root*")
except NameError:
     ZeeDInputDir=defaultVariable("ZeeDInputDir","./")

from AthenaCommon.AthenaCommonFlags import jobproperties as jp
jp.AthenaCommonFlags.FilesInput = ZeeDTTreeInput

ServiceMgr.EventSelector.InputCollections = jp.AthenaCommonFlags.FilesInput()

svcMgr.EventSelector.TupleName = "T"
svcMgr.EventSelector.ActiveBranches = [ '*' ]
