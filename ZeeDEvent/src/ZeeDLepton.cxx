#include "ZeeDEvent/ZeeDLepton.h"

// std libraries
#include <iostream>

// ZeeD libraries
#include "ZeeDMisc/ZeeDMisc.h"
#define DEBUG_ON 0

ZeeDLepton::ZeeDLepton()
{
    fCombinedFourVector.SetXYZT(0., 0., 0., 0.);
    fcharge = 0;
    fCollectionIndex=-1;
    fhasCharge=false;
	fInComp=false;
}

ZeeDLepton::~ZeeDLepton()
{

}

//------------------------------------------------------
Bool_t ZeeDLepton::IsMatchedToTriggerChain(ZeeDEnum::Trigger::Value chain) const {

    Bool_t isMatchedToChain = kFALSE;

    if(this->Trigger().IsSet() == kFALSE) {

      // In case of electron trigger info absence do not crash the whole job, just print a warning
      Error("ZeeDLepton::IsMatchedToTriggerChain","this lepton does not have any trigger matching information");
      return kFALSE;

    }

    ZeeDLeptonBags::Trigger::TriggerMap::const_iterator it = this->Trigger().Get().isMatchedToChain.find(chain);

    if( it == this->Trigger().Get().isMatchedToChain.end() ) {

      // In case of electron matching info absence do not crash the whole job, just print a warning
      Error("ZeeDLepton::IsMatchedToTriggerChain","no trigger matching information found for the chain \"%s\"", ZeeDEnum::Trigger::ToString(chain));
      return kFALSE;

    } else {

        isMatchedToChain = it->second;

    }

    return isMatchedToChain;

}

//------------------------------------------------------
const ZeeDGenParticle* ZeeDLepton::GetGenBornMatchedLepton() const
{

    return fGenBornMatchedLepton;

}



