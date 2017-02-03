#include "ZeeDEvent/ZeeDVertex.h"

//#define USE_DEBUG_MESSAGES
#define DEBUG_ON 0
#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDVertex::ZeeDVertex() : fNTrack(-999)
{
    // Constructor
    fVertexCandidate.SetXYZ(0, 0, 0);
    fErrorVertexCandidate.SetXYZ(0, 0, 0);
}

//------------------------------------------------------
ZeeDVertex::ZeeDVertex(const ZeeDVertex& vertex) : TObject(vertex)
{
    // Copy constructor

    fVertexCandidate      = vertex.fVertexCandidate;
    fErrorVertexCandidate = vertex.fErrorVertexCandidate;
    fNTrack               = vertex.fNTrack;
}

//------------------------------------------------------
ZeeDVertex::~ZeeDVertex()
{
    // Destructor
}

//------------------------------------------------------
void ZeeDVertex::Print(Option_t* option) const
{
    // Blue print
    if ( !option || !option[0] ) {
        std::cout << option << std::endl;
    }

    ZeeDMisc::Dump3Vec("vertex      ", fVertexCandidate);
    ZeeDMisc::Dump3Vec("vertex_err2 ", fErrorVertexCandidate);
}
