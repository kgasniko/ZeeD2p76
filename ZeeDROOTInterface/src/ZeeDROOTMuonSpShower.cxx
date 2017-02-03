/*
 * ZeeDROOTMuonSpShower.cxx
 *
 *  Created on: 2013.12.19
 *      Author: SSh 
 */

#include "ZeeDROOTInterface/ZeeDROOTMuonSpShower.h"
#include "ZeeDEvent/ZeeDMuonSpShower.h"

ZeeDROOTMuonSpShower::ZeeDROOTMuonSpShower() :
  feta(0.),
  fphi(0.),
  ftrigHits(0),
  finnerHits(0),
  fmiddleHits(0),
  fouterHits(0),
  finnerSegments(0),
  fmiddleSegments(0),
  fouterSegments(0)
{
  // TODO Auto-generated constructor stub

}

ZeeDROOTMuonSpShower::~ZeeDROOTMuonSpShower() {
	// TODO Auto-generated destructor stub
}

Int_t ZeeDROOTMuonSpShower::fillIn(const ZeeDMuonSpShower* const muonSpShower) {
    // Fills ZeeDROOTElectron with the information from ZeeDMuonSpShower
  feta			= muonSpShower->feta;
  fphi			= muonSpShower->fphi;
  ftrigHits		= muonSpShower->ftrigHits;
  finnerHits		= muonSpShower->finnerHits;
  fmiddleHits		= muonSpShower->fmiddleHits;
  fouterHits		= muonSpShower->fouterHits;
  finnerSegments	= muonSpShower->finnerSegments;
  fmiddleSegments	= muonSpShower->fmiddleSegments;
  fouterSegments	= muonSpShower->fouterSegments;

  return 0;
}

Int_t ZeeDROOTMuonSpShower::fillOut(ZeeDMuonSpShower* const muonSpShower) const {

  // Fills ZeeDMuonSpShower with the information from ZeeDROOTMuonSpShower
  muonSpShower->feta		= feta;
  muonSpShower->fphi		= fphi;
  muonSpShower->ftrigHits	= ftrigHits;
  muonSpShower->finnerHits	= finnerHits;
  muonSpShower->fmiddleHits	= fmiddleHits;
  muonSpShower->fouterHits	= fouterHits;
  muonSpShower->finnerSegments	= finnerSegments;
  muonSpShower->fmiddleSegments	= fmiddleSegments;
  muonSpShower->fouterSegments	= fouterSegments;

  return 0;
}
