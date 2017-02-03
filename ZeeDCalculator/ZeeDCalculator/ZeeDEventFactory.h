#pragma once

#include <TObject.h>
#include <TString.h>

#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDEvent/ZeeDEvent.h"

class ZeeDSystematics;
class ZeeDCalibrator;
class ZeeDCalcEventWeight;
class ZeeDCalcRecWeight;
class ZeeDCalcRecWeightMuon;

class ZeeDCalcEvent;
class IZeeDReadEvent;


/** Creates event from different sources. Sources can be AOD, Inline, TTree etc.

    @author Sebastian Schmitt
    @date January 2011

 */
class ZeeDEventFactory : public TObject {

public:

  static ZeeDEventFactory* getInstance() {
    if (fInstance != NULL) {
      return fInstance;
    } else {
      fInstance = new ZeeDEventFactory();
      return fInstance;
    }
  }

  /** User owns event and has to delete it.

        @param eventType AOD, InlineZ, TTree
    */

  
    const RawEvent* ReadEvent();

    ZeeDEvent* CalcEvent(const RawEvent* rawEvent);

    void CalcGenWeight(ZeeDEvent* event);
    void CalcRecWeight(ZeeDEvent* event);

    ZeeDEvent* Get();

    ~ZeeDEventFactory();


private:

    ZeeDEventFactory();
    ZeeDEventFactory(const ZeeDEventFactory&);
    void operator=(const ZeeDEventFactory&);

    static ZeeDEventFactory* fInstance;

    ZeeDSvcHelper fSvcHelper;
    ServiceHandle<IZeeDAnalysisSvc>* fAnaOptions;

    ZeeDCalcEvent*         fCalcEvent;
    IZeeDReadEvent*        fReadEvent;

    ZeeDCalcEventWeight* fCalcGenWeight;
    ZeeDCalcRecWeight* fCalcRecWeight;
    ZeeDCalcRecWeightMuon* fCalcRecWeightMuon;
};
