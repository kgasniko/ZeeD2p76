#pragma once

/*
 * ZeeDMuon.h
 *
 *  Created on: Aug 7, 2012
 *      Author: morse
 */


#include <TObject.h>
#include "TLorentzVector.h"
#include "ZeeDTools/ZeeDQuantity.h"
#include "ZeeDTools/ZeeDBags.h"
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDLepton.h"

class ZeeDMuon: public ZeeDLepton {

  public:

    virtual ~ZeeDMuon();

    /*const TLorentzVector& GetCombinedMuonVector() const {    
        return fcombinedMuonTrackParticle;
    }*/

    // Global muon properties
    const ZeeDQuantity<ZeeDMuonBags::Properties>& Properties() const {
        return fProperties;
    }

    // Track properties
    const ZeeDQuantity<ZeeDMuonBags::CombTrack>& CombTrack() const {
        return fCombTrack;
    }    
    
    // Isolation
    const ZeeDQuantity<ZeeDMuonBags::Isolation>& Isolation() const {
        return fIsolation;
    }
    
    // MS track
    const ZeeDQuantity<ZeeDMuonBags::MStrack>& MStrack() const {
        return fMStrack;
    }    

    // ID track
     const ZeeDQuantity<ZeeDMuonBags::IDtrack>& IDtrack() const {
        return fIDtrack;
    }

	 const ZeeDQuantity<ZeeDMuonBags::CorMomentum>& CorMomentum() const {
		 return fCorMomentum;
	 }

  private:

     ZeeDMuon();
/*    void SetCombinedMuonVector( const TLorentzVector& combinedMuonTrackParticle){
        fcombinedMuonTrackParticle == combinedMuonTrackParticle;        
    }
    
    
    
 
    TLorentzVector	fcombinedMuonTrackParticle;
*/
// Bag properties
    ZeeDQuantity<ZeeDMuonBags::Properties>& Properties() {
        return fProperties;
    }
    
// Track properties
    ZeeDQuantity<ZeeDMuonBags::CombTrack>& CombTrack()  {
        return fCombTrack;
    }

// Isolation
    ZeeDQuantity<ZeeDMuonBags::Isolation>& Isolation()  {
        return fIsolation;
    }


// MS track
    ZeeDQuantity<ZeeDMuonBags::MStrack>& MStrack()  {
        return fMStrack;
    }    

// ID track
    ZeeDQuantity<ZeeDMuonBags::IDtrack>& IDtrack()  {
        return fIDtrack;
    }

//CorrectedMomentum
	ZeeDQuantity<ZeeDMuonBags::CorMomentum>& CorMomentum() {
		return fCorMomentum;
	}


    ZeeDQuantity<ZeeDMuonBags::Properties> fProperties;
    ZeeDQuantity<ZeeDMuonBags::CombTrack> fCombTrack;    
    ZeeDQuantity<ZeeDMuonBags::Isolation> fIsolation;
    ZeeDQuantity<ZeeDMuonBags::MStrack> fMStrack;
    ZeeDQuantity<ZeeDMuonBags::IDtrack> fIDtrack;
	ZeeDQuantity<ZeeDMuonBags::CorMomentum> fCorMomentum;

    friend class ZeeDReadEtMissAOD;
    friend class ZeeDReadMuonAOD;
    friend class ZeeDROOTMuon;
    friend class ZeeDROOTEvent;
    friend class ZeeDCalcRecWeightMuon;
	friend class ZeeDCalcMuon;
	friend class ZeeDCalcEtMiss;


};

