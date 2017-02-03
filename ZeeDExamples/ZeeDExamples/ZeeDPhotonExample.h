#ifndef __ZEEDPHOTONEXAMPLE_H
#define __ZEEDPHOTONEXAMPLE_H

#include "GaudiKernel/Algorithm.h"

////////////////////////////////////////////////////////
// Name    : ZeeDPhotonExample.h
////////////////////////////////////////////////////////

/** Example class for include radiation in ZeeDInline generators
    @author Mikhail Karnevsky
    @date 2008/11/18
*/
class ZeeDPhotonExample : public Algorithm {

public:

    ZeeDPhotonExample(const std::string& name, ISvcLocator* pSvcLocator);
    virtual ~ZeeDPhotonExample();

    StatusCode initialize();
    StatusCode finalize();
    StatusCode execute();
};

#endif // ZeeDPhotonExample
