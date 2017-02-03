#ifndef I_ZEED_READ_EVENT_H
#define I_ZEED_READ_EVENT_H

#include "ZeeDEvent/ZeeDEvent.h"

class IZeeDReadEvent {
public:
    virtual ~IZeeDReadEvent() {}
    virtual void ReadInfo(RawEvent* event) = 0;
};

#endif
