#ifndef __ZeeDReadTracksAOD_H
#define __ZeeDReadTracksAOD_H

////////////////////////////////////////////////////////
// Name    : ZeeDReadTracksAOD.h
////////////////////////////////////////////////////////

#include <TObject.h>

#include <string>

#include "ZeeDTools/ZeeDSvcHelper.h"
#include "StoreGate/StoreGateSvc.h"
#include "TruthUtils/IHforTool.h"

#include "Particle/TrackParticleContainer.h"

class ZeeDEvent;
class TObjArray;
class ZeeDTrack;

/** Class for calculation of jet quantities
  @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte, Sebastian Schmitt, Rohin Narayan
  @date   Wed Jun 23 14:13:20 CEST 2010
*/

class ZeeDReadTracksAOD : public TObject {

public:

    ZeeDReadTracksAOD();
    ~ZeeDReadTracksAOD();

    /** Calculate track candidates for the given event */
    void ReadInfo(ZeeDEvent* event);

private:

    /** Initialise necessary containers */
    void Initialize();

    Bool_t fIsMC;
    const ZeeDEvent* fEvent;
    ZeeDTrack* fTrackInProcess;
    const Rec::TrackParticleContainer* fTrackContainer;

    /** flag: is data or is MC */


    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc> *fAnaOptions; //!
    
    /** Store Gate*/
    ServiceHandle<StoreGateSvc>* fStoreGate;


};

#endif // ZeeDReadTracksAOD
