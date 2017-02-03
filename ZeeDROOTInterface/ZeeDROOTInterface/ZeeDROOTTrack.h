#ifndef ZEEDROOTTRACK_H
#define ZEEDROOTTRACK_H

#include <TObject.h>
#include <TLorentzVector.h>

#include "SGTools/CLASS_DEF.h"

class ZeeDTrack;

/** Partner class of ZeeDTrack
    @author George Sedov, Rohin Narayan
    @date Wed September 28 2011
*/
class ZeeDROOTTrack {

public:

    ZeeDROOTTrack();
    virtual ~ZeeDROOTTrack();

    Int_t        fillIn (const ZeeDTrack* const track);
    Int_t        fillOut(ZeeDTrack* const track) const;
    
 private:
    // Fields that must be saved in ROOT. NO POINTERS ALLOWED
    TLorentzVector fFourVector;
    Double_t fTrackD0;
    Double_t fTrackZ0;
    Double_t fTrackNumberOfPixelHits;
    Double_t fTrackNumberOfSCTHits;   
    Double_t fTrackChiSquared;
    Double_t fTrackNumberDoF;
    Double_t fTrackNumberOfPixelHoles; 
    Double_t fTrackNumberOfSCTHoles;
    Double_t fTrackNumberOfTRTHoles;
    Double_t fTrackqOverP;
    Double_t fTrackTheta;
    Double_t fTrackPhi0;

};

CLASS_DEF( ZeeDROOTTrack , 12055648 , 1 )

#endif
