#ifndef __ZEEDVERTEX_H
#define __ZEEDVERTEX_H

////////////////////////////////////////////////////////
// Name    : ZeeDVertex.h
////////////////////////////////////////////////////////
//
// Contact person: James Dassoulas, Andrei Nikiforov
//

// ROOT
#include <TVector3.h>

/** Class for event vertex object */
class ZeeDVertex  : public TObject {
public:

    ZeeDVertex();
    ZeeDVertex(const ZeeDVertex& vertex);
    ~ZeeDVertex();

    // Getters
    TVector3 GetVertexCandidate() const {
        return fVertexCandidate;
    }
    TVector3 GetErrorVertexCandidate() const {
        return fErrorVertexCandidate;
    }
    Int_t GetNTrack() const {
        return fNTrack;
    }

    // Setters
    void SetVertexCandidate(TVector3 Vector) {
        fVertexCandidate = Vector;
    }
    void SetErrorVertexCandidate(TVector3 Vector) {
        fErrorVertexCandidate = Vector;
    }
    void SetNTrack(Int_t NTrack) {
        fNTrack = NTrack;
    }

    /** Blue print */
    void Print(Option_t* option = "") const;

private:

    /** 3 vector of vertex candidate */
    TVector3 fVertexCandidate; //!

    /** Error vector */
    TVector3 fErrorVertexCandidate; //!

    /** Number of tracks fitted to the vertex */
    Int_t fNTrack; //!

    friend class ZeeDROOTVertex;
};

#endif // ZeeDVertex
