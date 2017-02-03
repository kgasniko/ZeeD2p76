#pragma once

////////////////////////////////////////////////////////
// Name    : ZeeDElectron.h
////////////////////////////////////////////////////////
//
// Contact persons: Andrei Nikiforov, Sebastian Schmitt
//

// Root includes
#include <TLorentzVector.h>
#include <TObject.h>

#include "ZeeDTools/ZeeDQuantity.h"
#include "ZeeDTools/ZeeDBags.h"
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDLepton.h"

/** Basic class for electron particles */
class ZeeDElectron : public ZeeDLepton {

public:

    //ZeeDElectron(const ZeeDElectron& elec);
    //ZeeDElectron(ZeeDElectron& elec);

    ~ZeeDElectron();

    // Getters


    bool IsCentral() {
      // FIXME: handmade definition
      return (fabs(this->Cluster().Get().fourVector.Eta()) < 2.50);
    }

    const ZeeDQuantity<ZeeDElectronBags::Shower>&         Shower()        const {
        return fShower;
    }
    const ZeeDQuantity<ZeeDElectronBags::Cluster>&        Cluster()       const {
        return fCluster;
    }
    const ZeeDQuantity<ZeeDElectronBags::TrackMatch>&     TrackMatch()    const {
        return fTrackMatch;
    }
   const ZeeDQuantity<ZeeDElectronBags::CaloIso>&        CaloIso()       const  {
    	return fcaloIso;
    }
    const ZeeDQuantity<ZeeDElectronBags::CaloIsoCor>&     CaloIsoCor()    const  {
    	return fcaloIsoCor;
    }
    const ZeeDQuantity<ZeeDElectronBags::IDCut>&     IDCut()    const  {
    	return fIDCut;
    }
    const ZeeDQuantity<ZeeDElectronBags::EIso>&            EIso()         const  {
    	return fEIso; 
    } 
    const ZeeDQuantity<bool> IsGoodOQAOD() const {
        return fIsGoodOQAOD;
    }
    const ZeeDQuantity<ZeeDElectronBags::Author>&         Author()        const {
        return fAuthor;
    }





private:

    ZeeDElectron();
    ZeeDQuantity<ZeeDElectronBags::Author>&        Author()        {
        return fAuthor;
    }
    ZeeDQuantity<ZeeDElectronBags::TrackMatch>&    TrackMatch()    {
        return fTrackMatch;
    }
    ZeeDQuantity<ZeeDElectronBags::Cluster>&       Cluster()       {
        return fCluster;
    }
    ZeeDQuantity<ZeeDElectronBags::Shower>&        Shower()        {
        return fShower;
    }
    ZeeDQuantity<ZeeDElectronBags::CaloIso>&   CaloIso()           {
        return fcaloIso;
    }
    ZeeDQuantity<ZeeDElectronBags::CaloIsoCor>&   CaloIsoCor()     {
        return fcaloIsoCor;
    }
    ZeeDQuantity<ZeeDElectronBags::IDCut>&   IDCut()     {
        return fIDCut;
    }
    ZeeDQuantity<ZeeDElectronBags::EIso>&   EIso()                 {
        return fEIso;
    }
    ZeeDQuantity<bool>&                            IsGoodOQAOD()      {
        return fIsGoodOQAOD;
    }

    ZeeDQuantity<ZeeDElectronBags::Shower>        fShower;        //!
    ZeeDQuantity<ZeeDElectronBags::Cluster>       fCluster;       //!
    ZeeDQuantity<ZeeDElectronBags::TrackMatch>    fTrackMatch;    //!
      ZeeDQuantity<ZeeDElectronBags::CaloIso>       fcaloIso;       //!
    ZeeDQuantity<ZeeDElectronBags::CaloIsoCor>    fcaloIsoCor;       //!
    ZeeDQuantity<ZeeDElectronBags::EIso>          fEIso;          //! 
    ZeeDQuantity<ZeeDElectronBags::IDCut>    fIDCut;       //!
    ZeeDQuantity<bool>                            fIsGoodOQAOD;   //!
    ZeeDQuantity<ZeeDElectronBags::Author>        fAuthor;        //!
 
    friend class ZeeDReadEventInlineZ;
    friend class ZeeDROOTElectron;
    friend class ZeeDROOTEvent; //I still think that making a constructor private is a bad idea. George.
    friend class ZeeDReadElecAOD;
    friend class ZeeDReadEtMissAOD;
    friend class ZeeDCalcEtMiss;
	friend class ZeeDCalcElec;
    friend class ZeeDCalcEventWeight;
    friend class ZeeDCalcRecWeight;
    friend class ZeeDReadEventD3PD;
    friend class ZeeDBoson; 
};

