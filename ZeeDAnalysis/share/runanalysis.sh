#!/bin/zsh -f
#
# runanalysis.sh v.1.0
# Author: A.Nikiforov
###########################################

OldFilesDir=.OldFiles
RootFile=ZeeDAnalysis.root
OutFile=ZeeDAnalysis.out
PsFile=ZeeDAnalysis.ps

# remove old files
rm -fr $OldFilesDir > /dev/null 2> /dev/null
mkdir $OldFilesDir

echo "\e[1;34m========================================================================\e[0m"

echo "\e[1;36m ------ Moving old files to $OldFilesDir ------\e[0m"
if [ -f $RootFile ]
    then
        mv $RootFile $OldFilesDir 2> /dev/null
fi

if [ -f $PsFile ]
    then
        mv $PsFile $OldFilesDir 2> /dev/null
fi

if [ -f $OutFile ]
    then
        mv $OutFile $OldFilesDir 2> /dev/null
fi

ls -al $OldFilesDir/

echo "\e[1;34m========================================================================\e[0m"

cd ../cmt

echo "\e[1;36m ------ Compiling libraries ---------\e[0m"
if ! gmake all; then
	echo "\e[1;31m ----- Compilation failed -----------\e[0m"
    cd ../run
	exit 1
fi

echo "\e[1;34m========================================================================\e[0m"
echo "\e[1;36m ------ Running Analysis ------------\e[0m"


cd ../run
athena.py ZeeDAnalysis.py 2>&1 | grep -v "AthenaEventLoopMgr   INFO   ===>>>" | tee $OutFile
