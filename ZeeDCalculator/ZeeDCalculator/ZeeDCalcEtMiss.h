#ifndef __ZEEDCALCETMISS_H
#define __ZEEDCALCETMISS_H

////////////////////////////////////////////////////////
// Name    : ZeeDCalcEtMiss.h
////////////////////////////////////////////////////////

#include <TObject.h>
#include <TRandom3.h>

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "ZeeDTools/ZeeDSystematics.h"
#include "METSystematics/METSystTool.h"
#include "METTrackUtil/TrackMETMaker.h"
class ZeeDEvent;

class METUtility;

/** Class for recalibration of  jets */
class ZeeDCalcEtMiss : public TObject {

public:

    ZeeDCalcEtMiss();
    ~ZeeDCalcEtMiss();

    /**  Recalibrate jets **/
    void Calculate(ZeeDEvent* event);

private:

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;   

    TRandom3* rndm; 
    ZeeDSystematics* fSys; 
    METSyst::METSystTool* fSystTool;
    METUtility* fMETUtility;
	METTrack::TrackMETMaker *fMETTrackUtil;
};

#endif // ZeeDCalcJets
