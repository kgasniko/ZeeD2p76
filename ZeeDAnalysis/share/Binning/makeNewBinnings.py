import os
names=["etMiss", "LepPt", "BosMassTransv", "LepEta"]
ln= len(names)
extension = ".txt"
for i in range(ln):
    fileName1=names[i]+extension
    for j in range(i+1, ln):
        fileName2 = names[j]+extension
        fileName = names[i]+names[j]+extension
        print "Making file ", fileName
        os.system("cat "+fileName1+"  > " + fileName)
        os.system("cat "+fileName2+" >> "+fileName)


