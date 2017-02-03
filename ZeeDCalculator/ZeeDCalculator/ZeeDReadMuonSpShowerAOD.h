#pragma once

/*
 * ZeeDReadMuonSpShowerAOD.h
 *
 *  Created on: 2013.12.19
 *      Author: SSh 
 */


#include <TObject.h>
#include "GaudiKernel/ToolHandle.h"
#include "StoreGate/StoreGateSvc.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDTools/ZeeDSvcHelper.h"

#include "muonEvent/MuonSpShowerContainer.h"


class ZeeDEvent;

class ZeeDReadMuonSpShowerAOD: public TObject {

  public:
    ZeeDReadMuonSpShowerAOD();
    virtual ~ZeeDReadMuonSpShowerAOD();

    void Initialize();

    void ReadInfo(ZeeDEvent* event);

  private:

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<StoreGateSvc>* fStoreGate;

    const Rec::MuonSpShowerContainer* fMuonSpShowerContainer;

};

