#include "ZeeDROOTInterface/ZeeDROOTEvent.h"

//#define USE_DEBUG_MESSAGES 1
//#define DEBUG_ON 1
#include "ZeeDMisc/ZeeDMisc.h"

#include "ZeeDROOTInterface/ZeeDROOTElectron.h"
#include "ZeeDROOTInterface/ZeeDROOTJet.h"
#include "ZeeDROOTInterface/ZeeDROOTTrack.h"
#include "ZeeDROOTInterface/ZeeDROOTMuon.h"
#include "ZeeDROOTInterface/ZeeDROOTMuonSpShower.h"
#include "ZeeDROOTInterface/ZeeDROOTVertex.h"
#include "ZeeDROOTInterface/ZeeDROOTEtMiss.h"
#include "ZeeDROOTInterface/ZeeDROOTGenParticle.h"
#include "ZeeDROOTInterface/ZeeDROOTPhoton.h"
#include "ZeeDROOTInterface/ZeeDROOTTau.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDJet.h"
#include "ZeeDEvent/ZeeDTrack.h"
#include "ZeeDEvent/ZeeDMuon.h"
#include "ZeeDEvent/ZeeDMuonSpShower.h"
#include "ZeeDEvent/ZeeDVertex.h"
#include "ZeeDEvent/ZeeDEtMiss.h"
#include "ZeeDEvent/ZeeDGenParticle.h"
#include "ZeeDEvent/ZeeDPhoton.h"
#include "ZeeDEvent/ZeeDTau.h"

#include <boost/foreach.hpp>

ZeeDROOTEvent::ZeeDROOTEvent() : fNvtx(0),fNpv(0),
    fRunNumber(0),
    fEventNumber(0),
    fLB(0),
    fActualInteractionsPerCrossing(0),
    fAverageInteractionsPerCrossing(0),
    fMCChannelNumber(0),
    fEventWeight(0),
    fGenWeight(0),
    fEtDensity_3EM(0),
    fEtDensity_4EM(0),
    fEtDensity_3LC(0),
    fEtDensity_4LC(0),
    fIsData(kFALSE),
    fPDF_id1(0),
    fPDF_id2(0),
    fPDF_x1(0),
    fPDF_x2(0),
    fPDF_scalePDF(0),
    fPDF_pdf1(0),
    fPDF_pdf2(0),
    fLArErrorState(0)
{
    DEBUG_MES_START;
    DEBUG_MES_END;
}

ZeeDROOTEvent::~ZeeDROOTEvent()
{

    DEBUG_MES_START;

    DEBUG_MES_END;
}

void ZeeDROOTEvent::clear()
{

    BOOST_FOREACH(ZeeDROOTElectron* p, fElecArray) {
        delete p;
    }

    BOOST_FOREACH(ZeeDROOTJet* p, fJetArray) {
        delete p;
    }
    BOOST_FOREACH(ZeeDROOTTrack* p, fTrackArray) {
      delete p;
    }

    BOOST_FOREACH(ZeeDROOTVertex* p, fVertexArray) {
        delete p;
    }

    BOOST_FOREACH(ZeeDROOTEtMiss* p, fEtMissArray) {
        delete p;
    }

    BOOST_FOREACH(ZeeDROOTMuon* p, fMuonArray) {
        delete p;
    }

    BOOST_FOREACH(ZeeDROOTMuonSpShower* p, fMuonSpShowerArray) {
        delete p;
    }

    BOOST_FOREACH(ZeeDROOTGenParticle* p, fGenZBosArray) {
        delete p;
    }

    BOOST_FOREACH(ZeeDROOTPhoton* p, fPhotonArray) {
        delete p;
    }

    BOOST_FOREACH(ZeeDROOTTau* p, fTauArray) {
        delete p;
    }

    fElecArray.clear();
    fJetArray.clear();
    fVertexArray.clear();
    fEtMissArray.clear();
    fTrackArray.clear();
    fMuonArray.clear();
    fMuonSpShowerArray.clear();
    fGenZBosArray.clear();
    fPhotonArray.clear();
    fTauArray.clear();

}

Int_t ZeeDROOTEvent::fillOut (RawEvent* const event) const
{
    // Fills ZeeDEvent with the information from ZeeDROOTEvent

    event->fRunNumber.Set(fRunNumber);
    event->fOrigRunNumber.Set(fRunNumber);
    event->fEventNumber.Set(fEventNumber);
    event->fLB.Set(fLB);

    event->fRunNumber.Lock();
    event->fOrigRunNumber.Lock();
    event->fEventNumber.Lock();
    event->fLB.Lock();

    if(fActualInteractionsPerCrossing_isSet) {

        event->fActualInteractionsPerCrossing.Set(fActualInteractionsPerCrossing);

    }

    if(fAverageInteractionsPerCrossing_isSet) {

        event->fAverageInteractionsPerCrossing.Set(fAverageInteractionsPerCrossing);

    }

    if(fMCChannelNumber_isSet) {

        event->fMCChannelNumber.Set(fMCChannelNumber);

    }

    if(fLArErrorState_isSet) {

        event->fLArErrorState.Set(fLArErrorState);

    }

    event->fEventWeight   = fEventWeight;
    event->fGenWeight     = fGenWeight;

    event->fEtDensity_3EM = fEtDensity_3EM;
    event->fEtDensity_4EM = fEtDensity_4EM;
    event->fEtDensity_3LC = fEtDensity_3LC;
    event->fEtDensity_4LC = fEtDensity_4LC;

    event->fIsData        = fIsData;

    ZeeDEventGenBags::PDF pdfBag;

    pdfBag.id1      = fPDF_id1;
    pdfBag.id2      = fPDF_id2;
    pdfBag.x1       = fPDF_x1;
    pdfBag.x2       = fPDF_x2;
    pdfBag.scalePDF = fPDF_scalePDF;
    pdfBag.pdf1     = fPDF_pdf1;
    pdfBag.pdf2     = fPDF_pdf2;

    event->fPDF.Set(pdfBag);
    event->fPDF.Lock();

    if(fTrigger_isSet) {
        //std::cout << "Here!"<< std::endl;
        ZeeDEventBags::Trigger triggerBag;

        for(TriggerROOTMap::const_iterator it = fTrigger_hasPassedChain.begin(),
                stop = fTrigger_hasPassedChain.end();
            it != stop;
            ++it) {
            //std::cout << it->first.c_str() << std::endl;
            /*if (it->second) {
               std::cout << it->second << std::endl;
               }*/
            //ZeeDEnum::Trigger::Value chain = ZeeDEnum::Trigger::ToEnum((it->first).c_str());
            //std::cout <<  ZeeDEnum::Trigger::ToString(chain) ;
            //std::cout << strcmp((it->first).c_str(), "EF_mu24i_tight") << " " << (it->first).c_str() << std::endl;
            if (strcmp((it->first).c_str(), "EF_mu24i_tight") != 0) 
                   triggerBag.hasPassedChain[ZeeDEnum::Trigger::ToEnum((it->first).c_str())] = it->second;
            else 
                   triggerBag.hasPassedChain[ZeeDEnum::Trigger::EF_mu2] = it->second;
         }
        //std::cout << std::endl;
        event->fTrigger.Set(triggerBag);

    }

    if(fVertex_isSet) {
        ZeeDEventGenBags::Vertex vertexBag;

        vertexBag.position  = fVertex_position;

        event->fVertex.Set(vertexBag);
    }
    event->fVertex.Lock();

    event->fTrigger.Lock();

    //std::cout << fElecArray->GetEntries()<< std::endl;
    event->fElecArray->Clear();

    TIter next(event->fJetCollectionArray);
    while (TObjArray* ev_tja  = (TObjArray*)next()) {
        ev_tja->Clear();
    }
    event->fJetCollectionArray->Clear();


    event->fVertexArray->Clear();
    event->fEtMissArray->Clear();
    event->fGenZBosArray->Clear();
    event->fMuonArray->Clear();
    event->fPhotonArray->Clear();
    event->fTauArray->Clear();

    for(UInt_t d=0; d != fTrackArray.size(); ++d) {

      ZeeDTrack* daughter = new ZeeDTrack();
      ZeeDROOTTrack* rootDaughter = fTrackArray[d];
      rootDaughter->fillOut(daughter);
      
      event->fTrackArray->Add(daughter);

    }

    for(UInt_t d=0; d != fElecArray.size(); ++d) {

        ZeeDElectron* daughter = new ZeeDElectron();
        ZeeDROOTElectron* rootDaughter = fElecArray[d];
        rootDaughter->fillOut(daughter);

        event->fElecArray->Add(daughter);

    }

    for(UInt_t d=0; d != fVertexArray.size(); ++d) {

        ZeeDVertex* daughter = new ZeeDVertex();
        ZeeDROOTVertex* rootDaughter = fVertexArray[d];
        rootDaughter->fillOut(daughter);

        event->fVertexArray->Add(daughter);

    }

    for(UInt_t d=0; d != fMuonArray.size(); ++d) {

        ZeeDMuon* daughter = new ZeeDMuon();
        ZeeDROOTMuon* rootDaughter = fMuonArray[d];
        rootDaughter->fillOut(daughter);
        event->fMuonArray->Add(daughter);

    }

    for(UInt_t d=0; d != fMuonSpShowerArray.size(); ++d) {

        ZeeDMuonSpShower* daughter = new ZeeDMuonSpShower();
        ZeeDROOTMuonSpShower* rootDaughter = fMuonSpShowerArray[d];
        rootDaughter->fillOut(daughter);

        event->fMuonSpShowerArray->Add(daughter);

    }

    for(UInt_t d=0; d != fJetArray.size(); ++d) {

        ZeeDJet* daughter = new ZeeDJet();
        ZeeDROOTJet* rootDaughter = fJetArray[d];
        const TString key = rootDaughter->fillOut(daughter);

        TObjArray* tja = (TObjArray*)(event->fJetCollectionArray->FindObject(key));
	
        if(!tja) {
            tja = new TObjArray();
            tja->SetName(key);
            tja->SetOwner(kTRUE);
            event->fJetCollectionArray->Add(tja);
        }

        tja->Add(daughter);

    }


    for(UInt_t d=0; d != fEtMissArray.size(); ++d) {

        ZeeDEtMiss* daughter = new ZeeDEtMiss();
        ZeeDROOTEtMiss* rootDaughter = fEtMissArray[d];
        rootDaughter->fillOut(daughter);

        event->fEtMissArray->Add(daughter);

    }

    for(UInt_t d=0; d != fGenZBosArray.size(); ++d) {

        ZeeDGenParticle* daughter = new ZeeDGenParticle();
        ZeeDROOTGenParticle* rootDaughter = fGenZBosArray[d];
        rootDaughter->fillOut(daughter);

        event->fGenZBosArray->Add(daughter);

    }

    for(UInt_t d=0; d != fPhotonArray.size(); ++d) {

        ZeeDPhoton* daughter = new ZeeDPhoton();
        ZeeDROOTPhoton* rootDaughter = fPhotonArray[d];
        rootDaughter->fillOut(daughter);

        event->fPhotonArray->Add(daughter);

    }

    for(UInt_t d=0; d != fTauArray.size(); ++d) {

        ZeeDTau* daughter = new ZeeDTau();
        ZeeDROOTTau* rootDaughter = fTauArray[d];
        rootDaughter->fillOut(daughter);

        event->fTauArray->Add(daughter);

    }

    return 0;
}

//------------------------------------------------------
Int_t ZeeDROOTEvent::fillIn(const RawEvent* const event)
{
    // Fills ZeeDROOTEvent with the information from ZeeDEvent

    if(event->fNvtx.IsSet()) {
        fNvtx        = event->fNvtx.Get();
        fNvtx_isSet  = kTRUE;
    }
    if(event->fNpvJets.IsSet()) {
        fNpv        = event->fNpvJets.Get();
        fNpv_isSet  = kTRUE;
    }

    if(event->fRunNumber.IsSet()) {
        fRunNumber       = event->fRunNumber.Get();
        fRunNumber_isSet = kTRUE;
    }

    if(event->fEventNumber.IsSet()) {
        fEventNumber       = event->fEventNumber.Get();
        fEventNumber_isSet = kTRUE;
    }

    if(event->fLB.IsSet()) {
        fLB          = event->fLB.Get();
        fLB_isSet    = kTRUE;
    }

    if(event->fActualInteractionsPerCrossing.IsSet()) {
        fActualInteractionsPerCrossing       = event->fActualInteractionsPerCrossing.Get();
        fActualInteractionsPerCrossing_isSet = kTRUE;
    }

    if(event->fAverageInteractionsPerCrossing.IsSet()) {
        fAverageInteractionsPerCrossing       = event->fAverageInteractionsPerCrossing.Get();
        fAverageInteractionsPerCrossing_isSet = kTRUE;
    }

    if(event->fMCChannelNumber.IsSet()) {
        fMCChannelNumber       = event->fMCChannelNumber.Get();
        fMCChannelNumber_isSet = kTRUE;
    }

    if(event->fLArErrorState.IsSet()) {
        fLArErrorState       = event->fLArErrorState.Get();
        fLArErrorState_isSet = kTRUE;
    }

    fEventWeight = event->fEventWeight;
    fGenWeight   = event->fGenWeight;

    fEtDensity_3EM = event->fEtDensity_3EM;
    fEtDensity_4EM = event->fEtDensity_4EM;
    fEtDensity_3LC = event->fEtDensity_3LC;
    fEtDensity_4LC = event->fEtDensity_4LC;

    fIsData      = event->fIsData;

    if(event->fPDF.IsSet()) {

        fPDF_id1       = event->fPDF.Get().id1;
        fPDF_id2       = event->fPDF.Get().id2;
        fPDF_x1        = event->fPDF.Get().x1;
        fPDF_x2        = event->fPDF.Get().x2;
        fPDF_scalePDF  = event->fPDF.Get().scalePDF;
        fPDF_pdf1      = event->fPDF.Get().pdf1;
        fPDF_pdf2      = event->fPDF.Get().pdf2;

        fPDF_isSet     = kTRUE;
    }

    fVertex_isSet                    = event->fVertex.IsSet();
    if(fVertex_isSet) {
        fVertex_position             = event->fVertex.Get().position;
    }

    if(event->fTrigger.IsSet()) {

        for(ZeeDEventBags::Trigger::TriggerMap::const_iterator it = event->fTrigger.Get().hasPassedChain.begin(),
                stop = event->fTrigger.Get().hasPassedChain.end();
            it != stop;
            ++it) {
            fTrigger_hasPassedChain[ZeeDEnum::Trigger::ToString(it->first)] = it->second;
        }

        fTrigger_isSet          = event->fTrigger.IsSet();
    }

    TIter nextEl(event->fElecArray);

    ZeeDElectron* electron;

    while(( electron = (ZeeDElectron*) nextEl())) {
        ZeeDROOTElectron* rootElectron = new ZeeDROOTElectron();
        rootElectron->fillIn(electron);
        fElecArray.push_back(rootElectron);
    }

    TIter nextTr(event->fTrackArray);

    ZeeDTrack* track;

    while(( track = (ZeeDTrack*) nextTr())) {
      ZeeDROOTTrack* rootTrack = new ZeeDROOTTrack();
      rootTrack->fillIn(track);
      fTrackArray.push_back(rootTrack);

      //      std::cout <<"pt = "<<track->GetFourVector().Pt()<< std::endl;
    }

    TIter nextVer(event->fVertexArray);

    ZeeDVertex* vertex;

    while(( vertex = (ZeeDVertex*) nextVer())) {
        ZeeDROOTVertex* rootVertex = new ZeeDROOTVertex();
        rootVertex->fillIn(vertex);
        fVertexArray.push_back(rootVertex);
    }

    TIter nextMuon(event->fMuonArray);

    ZeeDMuon* muon;

    while(( muon = (ZeeDMuon*) nextMuon())) {
        ZeeDROOTMuon* rootMuon = new ZeeDROOTMuon();
        rootMuon->fillIn(muon);
        fMuonArray.push_back(rootMuon);
    }

    TIter nextMuonSpShower(event->fMuonSpShowerArray);

    ZeeDMuonSpShower* muonSpShower;

    while(( muonSpShower = (ZeeDMuonSpShower*) nextMuonSpShower())) {
        ZeeDROOTMuonSpShower* rootMuonSpShower = new ZeeDROOTMuonSpShower();
        rootMuonSpShower->fillIn(muonSpShower);
        fMuonSpShowerArray.push_back(rootMuonSpShower);
    }

    TIter nextJetCol = TIter(event->fJetCollectionArray);
    while (TObjArray* ev_tja  = (TObjArray*)nextJetCol()) {

        TIter nextJet(ev_tja);
        ZeeDJet* jet;
        while((jet = (ZeeDJet*) nextJet())) {
            //            std::cout << "filling in jet from col " << ev_tja->GetName() << std::endl;
            ZeeDROOTJet* rootJet = new ZeeDROOTJet();
            rootJet->fillIn(jet, ev_tja->GetName());
            this->fJetArray.push_back(rootJet);
        }
    }

    TIter nextEtMiss(event->fEtMissArray);

    ZeeDEtMiss* etMiss;

    while(( etMiss = (ZeeDEtMiss*) nextEtMiss())) {
        ZeeDROOTEtMiss* rootEtMiss = new ZeeDROOTEtMiss();
        rootEtMiss->fillIn(etMiss);
        fEtMissArray.push_back(rootEtMiss);
    }

    TIter nextGenZBos(event->fGenZBosArray);

    ZeeDGenParticle* genZBos;

    while(( genZBos = (ZeeDGenParticle*) nextGenZBos())) {
        ZeeDROOTGenParticle* rootGenZBos = new ZeeDROOTGenParticle();
        rootGenZBos->fillIn(genZBos);
        fGenZBosArray.push_back(rootGenZBos);
    }

    TIter nextPhoton(event->fPhotonArray);

    ZeeDPhoton* photon;

    while(( photon = (ZeeDPhoton*) nextPhoton())) {
        ZeeDROOTPhoton* rootPhoton = new ZeeDROOTPhoton();
        rootPhoton->fillIn(photon);
        fPhotonArray.push_back(rootPhoton);
    }

    TIter nextTau(event->fTauArray);

    ZeeDTau* tau;

    while(( tau = (ZeeDTau*) nextTau())) {
        ZeeDROOTTau* rootTau = new ZeeDROOTTau();
        rootTau->fillIn(tau);
        fTauArray.push_back(rootTau);
    }

    return 0;
}


