#pragma once


////////////////////////////////////////////////////////
// Name    : ZeeDReadVertexAOD.h
////////////////////////////////////////////////////////
//
// Contact persons: James Dassoulas, Andrei Nikiforov
//

// Root includes
#include <TObject.h>

// Athena includes
#include "GaudiKernel/StatusCode.h"

// ZeeD includes
#include "ZeeDTools/ZeeDSvcHelper.h"
#include "ZeeDEvent/ZeeDEvent.h"

class IToolSvc;
class VxContainer;

/** Class for reading of vertex information from AOD */
class ZeeDReadVertexAOD : public TObject {

public:

    ZeeDReadVertexAOD();
    ~ZeeDReadVertexAOD();

    void ReadInfo(ZeeDEvent* event);

private:

    /** Initializor */
    StatusCode Initialize();

    /** Vertex container */
    const VxContainer* fPriVertexContainer; //!

    /** Service helper */
    ZeeDSvcHelper fSvcHelper;               //!
};

