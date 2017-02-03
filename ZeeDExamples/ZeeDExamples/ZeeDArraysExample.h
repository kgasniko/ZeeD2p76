#ifndef __ZEEDARRAYSEXAMPLE_H
#define __ZEEDARRAYSEXAMPLE_H

#include "GaudiKernel/Algorithm.h"

////////////////////////////////////////////////////////
// Name    : ZeeDArraysExample.h
////////////////////////////////////////////////////////

class ZeeDArraysExample : public Algorithm {

public:

    ZeeDArraysExample(const std::string& name, ISvcLocator* pSvcLocator);
    virtual ~ZeeDArraysExample();

    StatusCode initialize();
    StatusCode finalize();
    StatusCode execute();
};

#endif // ZeeDArraysExample
