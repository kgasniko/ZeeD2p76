#ifndef ZEED_CALC_EVENT_INLINE_Z_H
#define ZEED_CALC_EVENT_INLINE_Z_H

#include <TObject.h>
#include "ZeeDInlineZ/ZeeDInlineZ.h"
#include "ZeeDCalculator/IZeeDReadEvent.h"
#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

class ZeeDInlineZ;
class ZeeDEvent;
class ZeeDVertex;

/**
   @todo do we need calibrator and systematics for inline?
 */
class ZeeDReadEventInlineZ : public TObject, public IZeeDReadEvent  {

public:

    ZeeDReadEventInlineZ();
    ~ZeeDReadEventInlineZ();
    void ReadInfo(RawEvent* event);

private:

    void ReadElecInfo(RawEvent* event);
    void ReadEventInfo(RawEvent* event);
    void ReadVertexInfo(RawEvent* event);
    ZeeDInlineZ* fZeeDInlineZ;

    /** Service helper */
    ZeeDSvcHelper fSvcHelper; //!

    /** Service handle to job options file */
    ServiceHandle<IZeeDAnalysisSvc> *fAnaOptions; //!

};

#endif
