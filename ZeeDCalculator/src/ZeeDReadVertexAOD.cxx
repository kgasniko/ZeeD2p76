#include "ZeeDCalculator/ZeeDReadVertexAOD.h"

// Root includes
#include <TVector3.h>
#include <TObjArray.h>

// Athena libraries
#include "GaudiKernel/ServiceHandle.h"
#include "GaudiKernel/IToolSvc.h"
#include "StoreGate/StoreGateSvc.h"

// ZeeD analysis includes
#include "ZeeDEvent/ZeeDVertex.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include "ZeeDMisc/ZeeDMisc.h"

#include "VxVertex/VxContainer.h"
#include "VxVertex/VxCandidate.h"
#include "TrkEventPrimitives/ErrorMatrix.h"
#include "TrkEventPrimitives/CovarianceMatrix.h"

//------------------------------------------------------
ZeeDReadVertexAOD::ZeeDReadVertexAOD() : fSvcHelper("ZeeDReadVertexAOD")
{
    // Constructor

    // Initalize services
    StatusCode sc = this->Initialize();

    if ( sc.isFailure() ) {
        Error("ZeeDReadVertexAOD::ZeeDReadVertexAOD",
              "Failed to initialize object");
        exit(EXIT_FAILURE);
    }
}

//------------------------------------------------------
ZeeDReadVertexAOD::~ZeeDReadVertexAOD()
{
    // Destructor
}

//------------------------------------------------------
StatusCode ZeeDReadVertexAOD::Initialize()
{
    // Initializer
    return StatusCode::SUCCESS;
}

//------------------------------------------------------
void ZeeDReadVertexAOD::ReadInfo(ZeeDEvent* event)
{
    // Calculate vertex for given event
    DEBUG_MES_START;

    this->Initialize();

    // Get vertex array from event to be filled in
    TObjArray* vertexArray = event->GetVertices();

    //Iterators over the Vertex container
    const ServiceHandle<StoreGateSvc>& storeGate = *(fSvcHelper.GetStoreGate());
    static const std::string PriVertexContainerName("VxPrimaryCandidate");

    StatusCode sc = storeGate->retrieve(fPriVertexContainer, PriVertexContainerName );

    if ( sc.isFailure() || fPriVertexContainer == NULL ) {
      vertexArray->Clear();
      return;
    }

    DataVector<Trk::VxCandidate>::const_iterator vtxItr  = fPriVertexContainer->begin();
    DataVector<Trk::VxCandidate>::const_iterator vtxItrE = fPriVertexContainer->end();

    // Clear vertex arrays
    vertexArray->Clear();

    for (; vtxItr != vtxItrE; ++vtxItr) {
        Trk::VxCandidate* myVxCandidate = *vtxItr;

        // Create vertex candidate
        ZeeDVertex* vertex = new ZeeDVertex();

        const CLHEP::Hep3Vector position = ( myVxCandidate->recVertex() ).position() / millimeter;

        TVector3 vertexCandidate;
        vertexCandidate.SetXYZ( position.x(), position.y(), position.z() );

        vertex->SetVertexCandidate(vertexCandidate);

        // Vertex errors
        const Trk::ErrorMatrix vtxError = ( myVxCandidate->recVertex() ).errorPosition();

        TVector3 errorVertexCandidate;

        errorVertexCandidate.SetXYZ( vtxError.error(Trk::x) / millimeter,
                                     vtxError.error(Trk::y) / millimeter,
                                     vtxError.error(Trk::z) / millimeter );

        // Set vertex error vector
        vertex->SetErrorVertexCandidate(errorVertexCandidate);

        // Set number of track fitted to the vertex
        vertex->SetNTrack(myVxCandidate->vxTrackAtVertex()->size());

        // Add vertex to the array
        vertexArray->AddLast( vertex );
    }

    DEBUG_MES_END;
}
