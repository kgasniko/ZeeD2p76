#ifndef __ZEEDCALCVERTEX_H
#define __ZEEDCALCVERTEX_H

////////////////////////////////////////////////////////
// Name    : ZeeDCalcVertex.h
////////////////////////////////////////////////////////

#include <TObject.h>

// Athena includes
#include "GaudiKernel/StatusCode.h"

// Vertex candidate

#include "ZeeDTools/ZeeDSvcHelper.h"

class IToolSvc;

class VxContainer;

class TObjArray;

/** Class for
    @author Andrei Nikiforov, Alexander Glazov, Ringaile Placakyte
    @date 2008/08/01
*/
class ZeeDPryVertex;

class ZeeDCalcVertex : public TObject {

public:

    ZeeDCalcVertex();
    ~ZeeDCalcVertex();

    void Calculate(TObjArray* vertices);

private:

    /** Vertex container */
    const VxContainer* fPriVertexContainer;     //!

    StatusCode Initialize();

    ZeeDSvcHelper fSvcHelper;

};

#endif // ZeeDCalcVertex
