#include "ZeeDROOTInterface/ZeeDROOTVertex.h"
#include "ZeeDEvent/ZeeDVertex.h"

ZeeDROOTVertex::ZeeDROOTVertex() : fNTrack(0)
{
}

ZeeDROOTVertex::~ZeeDROOTVertex()
{
}

Int_t ZeeDROOTVertex::fillOut (ZeeDVertex* const vertex) const
{
    // Fills ZeeDVertex with the information from ZeeDROOTVertex

    vertex->fVertexCandidate      = fVertexCandidate;
    vertex->fErrorVertexCandidate = fErrorVertexCandidate;
    vertex->fNTrack               = fNTrack;

    return 0;
}

Int_t ZeeDROOTVertex::fillIn(const ZeeDVertex* const vertex)
{
    // Fills ZeeDROOTVertex with the information from ZeeDVertex

    fVertexCandidate      = vertex->fVertexCandidate;
    fErrorVertexCandidate = vertex->fErrorVertexCandidate;
    fNTrack               = vertex->fNTrack;

    return 0;
}
