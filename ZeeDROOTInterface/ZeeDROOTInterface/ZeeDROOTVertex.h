#ifndef ZEEDROOTVERTEX_H
#define ZEEDROOTVERTEX_H

#include <TObject.h>
#include <TVector3.h>

#include "SGTools/CLASS_DEF.h"

class ZeeDVertex;

/** Partner class of ZeeDVertex
    @author George Sedov
    @date Wed Feb 9 16:24:57 CEST 2011
*/
class ZeeDROOTVertex {

public:
    ZeeDROOTVertex();
    virtual ~ZeeDROOTVertex();

    Int_t       fillIn (const ZeeDVertex* const vertex);
    Int_t       fillOut(ZeeDVertex* const vertex) const;

private:
    // Fields that must be saved in ROOT. NO POINTERS ALLOWED
    TVector3 fVertexCandidate;
    TVector3 fErrorVertexCandidate;
    Int_t fNTrack;

};

CLASS_DEF( ZeeDROOTVertex , 140212708 , 1 )

#endif
