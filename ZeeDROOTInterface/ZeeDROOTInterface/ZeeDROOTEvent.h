#ifndef ZEEDROOTEVENT_H
#define ZEEDROOTEVENT_H

#include <TObject.h>
#include <TObjArray.h>
#include <TVector3.h>
#include <map>

#include "SGTools/CLASS_DEF.h"

//struct RawEvent;
class ZeeDEvent;

class ZeeDROOTElectron;
class ZeeDROOTJet;
class ZeeDROOTTrack;
class ZeeDROOTMuon;
class ZeeDROOTMuonSpShower;
class ZeeDROOTVertex;
class ZeeDROOTEtMiss;
class ZeeDROOTGenParticle;
class ZeeDROOTPhoton;
class ZeeDROOTTau;

typedef ZeeDEvent RawEvent;
/** Partner class of ZeeDEvent
    @author George Sedov
    @date Wed Feb 9 16:24:57 CEST 2011
*/
class ZeeDROOTEvent {

public:

    ZeeDROOTEvent();
    virtual ~ZeeDROOTEvent();

    Int_t       fillIn (const RawEvent* const event);
    Int_t       fillOut(RawEvent* const event) const;

    void        clear();

private:
    // Fields that must be saved in ROOT. NO POINTERS ALLOWED
    std::vector<ZeeDROOTElectron*>      fElecArray;
    std::vector<ZeeDROOTJet*>           fJetArray;
    std::vector<ZeeDROOTTrack*>         fTrackArray;
    std::vector<ZeeDROOTVertex*>        fVertexArray;
    std::vector<ZeeDROOTEtMiss*>        fEtMissArray;
    std::vector<ZeeDROOTGenParticle*>   fGenZBosArray;
    std::vector<ZeeDROOTMuon*>		fMuonArray;
    std::vector<ZeeDROOTMuonSpShower*>	fMuonSpShowerArray;
    std::vector<ZeeDROOTPhoton*>	fPhotonArray;
    std::vector<ZeeDROOTTau*>		fTauArray;

    Int_t       fNvtx;
    Bool_t      fNvtx_isSet;

    Int_t       fNpv;
    Bool_t      fNpv_isSet;

    Int_t       fRunNumber;
    Bool_t      fRunNumber_isSet;

    Int_t       fEventNumber;
    Bool_t      fEventNumber_isSet;

    Int_t       fLB;
    Bool_t      fLB_isSet;

    Double_t    fActualInteractionsPerCrossing;
    Bool_t      fActualInteractionsPerCrossing_isSet;

    Double_t     fAverageInteractionsPerCrossing;
    Bool_t      fAverageInteractionsPerCrossing_isSet;

    Int_t       fMCChannelNumber;
    Bool_t      fMCChannelNumber_isSet;

    TString     fMCtype;
    Bool_t      fMCtype_isSet;

    Double_t     fEventWeight;
    Double_t     fGenWeight;

    Double_t     fEtDensity_3EM;
    Double_t     fEtDensity_4EM;
    Double_t     fEtDensity_3LC;
    Double_t     fEtDensity_4LC;

    Bool_t      fIsData;

    //Fields of fVertex
    TVector3    fVertex_position;
    Bool_t      fVertex_isSet;

    typedef std::map<std::string, int> TriggerROOTMap;
    TriggerROOTMap fTrigger_hasPassedChain;
    Bool_t         fTrigger_isSet;

    Double_t     fPDF_id1;
    Double_t     fPDF_id2;
    Double_t     fPDF_x1;
    Double_t     fPDF_x2;
    Double_t     fPDF_scalePDF;
    Double_t     fPDF_pdf1;
    Double_t     fPDF_pdf2;
    Bool_t      fPDF_isSet;

    Int_t       fLArErrorState;
    Bool_t      fLArErrorState_isSet;

    friend class ZeeDHistManagerWriter;

};

CLASS_DEF( ZeeDROOTEvent, 239482934 , 1 )

CLASS_DEF( std::vector<ZeeDROOTElectron*> , 125817562 , 1 )
CLASS_DEF( std::vector<ZeeDROOTJet*> , 181610962 , 1 )
CLASS_DEF( std::vector<ZeeDROOTTrack*> , 51083456, 1 )
CLASS_DEF( std::vector<ZeeDROOTMuon*> , 1388929 , 1 )
CLASS_DEF( std::vector<ZeeDROOTMuonSpShower*> , 1388930 , 1 )
CLASS_DEF( std::vector<ZeeDROOTVertex*> , 6992128 , 1 )
CLASS_DEF( std::vector<ZeeDROOTEtMiss*> , 192905825 , 1 )
CLASS_DEF( std::vector<ZeeDROOTGenParticle*> , 92857475 , 1 )
CLASS_DEF( std::vector<ZeeDROOTPhoton*> , 25827588, 1 )
CLASS_DEF( std::vector<ZeeDROOTTau*> , 12746049 , 1 )



#endif
