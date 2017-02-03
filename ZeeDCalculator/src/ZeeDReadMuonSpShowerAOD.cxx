/*
 * ZeeDReadMuonSpShowerAOD.cxx
 *
 *  Created on: 2013.12.19
 *      Author: SSh
 */

#include "ZeeDCalculator/ZeeDReadMuonSpShowerAOD.h"
#include "muonEvent/MuonParamDefs.h"
#include "MuonCombinedToolInterfaces/IMuonPropertiesTool.h"

#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDMuonSpShower.h"


ZeeDReadMuonSpShowerAOD::ZeeDReadMuonSpShowerAOD() :
  fSvcHelper("ZeeDReadMuonSpShowerAOD"),
  fStoreGate(fSvcHelper.GetStoreGate()),
  fMuonSpShowerContainer(NULL)
{

}

ZeeDReadMuonSpShowerAOD::~ZeeDReadMuonSpShowerAOD() {

}

void ZeeDReadMuonSpShowerAOD::Initialize() {

  StatusCode sc(StatusCode::SUCCESS);

  //Retrieve muon sp shower container
  std::string muonSpShowerContainerKey = "MuonSpShowers";
  if ((*fStoreGate)->contains<Rec::MuonSpShowerContainer>(muonSpShowerContainerKey)) {
    sc = (*fStoreGate)->retrieve(fMuonSpShowerContainer, muonSpShowerContainerKey);
    if ( !sc.isSuccess() ) { 
      REPORT_MESSAGE (MSG::WARNING) << "Could not retrieve muonSpShowerContainer";
      return; 
    }
  } else {
    REPORT_MESSAGE (MSG::WARNING) << "No muonSpShowerContainer found in the StoreGate";
    return;
  }

}

void ZeeDReadMuonSpShowerAOD::ReadInfo(ZeeDEvent* event)
{

  this->Initialize();

  event->ClearMuonSpShowers();

  for (Rec::MuonSpShowerContainer::const_iterator imuon = fMuonSpShowerContainer->begin();
      imuon != fMuonSpShowerContainer->end(); ++imuon) {

    ZeeDMuonSpShower * curmuon = new ZeeDMuonSpShower();

    curmuon->feta = (*imuon)->eta();
    curmuon->fphi = (*imuon)->phi();
    curmuon->ftrigHits = (*imuon)->numberOfTriggerHits();
    curmuon->finnerHits = (*imuon)->numberOfInnerHits();
    curmuon->fmiddleHits = (*imuon)->numberOfMiddleHits();
    curmuon->fouterHits = (*imuon)->numberOfOuterHits();
    curmuon->finnerSegments = (*imuon)->numberOfInnerSegments();
    curmuon->fmiddleSegments = (*imuon)->numberOfMiddleSegments();
    curmuon->fouterSegments = (*imuon)->numberOfOuterSegments();

    // store the muon
    event->AddMuonSpShower(curmuon);
  }

}
