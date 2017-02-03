#include "ZeeDCalculator/ZeeDCalcVertex.h"

// Root
#include <TVector3.h>
#include <TObjArray.h>
#include <TLorentzVector.h>

// Athena libraries
#include "GaudiKernel/ServiceHandle.h"
#include "GaudiKernel/IToolSvc.h"
#include "StoreGate/StoreGateSvc.h"

// ZeeD analysis includes
#include "ZeeDEvent/ZeeDVertex.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

// Vertex candidate
#include "VxVertex/VxContainer.h"
#include "VxVertex/VxCandidate.h"
#include "TrkEventPrimitives/ErrorMatrix.h"
#include "TrkEventPrimitives/CovarianceMatrix.h"

//#define USE_DEBUG_MESSAGES
#define DEBUG_ON 0
#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDCalcVertex::ZeeDCalcVertex() : fSvcHelper("ZeeDCalcVertex")
{
    // Constructor

    // Initalize services
    StatusCode sc = this->Initialize();

    if ( sc.isFailure() ) {
        Error("ZeeDCalcVertex::ZeeDCalcVertex",
              "Failed to initialize object");
        exit(EXIT_FAILURE);
    }

}

//------------------------------------------------------
ZeeDCalcVertex::~ZeeDCalcVertex()
{
    // Destructor
}

//------------------------------------------------------
StatusCode ZeeDCalcVertex::Initialize()
{
    // Initializer

    //Read Primary Vertex

    ServiceHandle<StoreGateSvc>* storeGate = fSvcHelper.GetStoreGate();
    std::string PriVertexContainerName("VxPrimaryCandidate");

    StatusCode sc = (*storeGate)->retrieve(fPriVertexContainer, PriVertexContainerName );

    if ( sc.isFailure() || fPriVertexContainer == NULL ) {
        Error("ZeeDCalcVertex::Initialize",
              "Vertex container is not found");
    }

    return sc;

}

//------------------------------------------------------
void ZeeDCalcVertex::Calculate(TObjArray* vertices)
{
    // Calculate vertex for given event

    this->Initialize();

    //Try Vertex print out
    //Iterators over the Vertex container
    DataVector<Trk::VxCandidate>::const_iterator VxItr = fPriVertexContainer->begin();
    DataVector<Trk::VxCandidate>::const_iterator VxItrE = fPriVertexContainer->end();

    // Clear vertex arrays
    vertices->Clear();

    for (; VxItr != VxItrE; ++VxItr) {
        Trk::VxCandidate* myVxCandidate = *VxItr;

        // Create new  Vertex
        ZeeDVertex* Vertex = new ZeeDVertex();

        TVector3 VertexCandidate;
        VertexCandidate.SetXYZ((myVxCandidate->recVertex()).position().x(),
                               (myVxCandidate->recVertex()).position().y(),
                               (myVxCandidate->recVertex()).position().z());

        Vertex->SetVertexCandidate(VertexCandidate);


        TVector3 ErrorVertexCandidate;
        ErrorVertexCandidate.SetXYZ((myVxCandidate->recVertex()).errorPosition().error(Trk::x),
                                    (myVxCandidate->recVertex()).errorPosition().error(Trk::y),
                                    (myVxCandidate->recVertex()).errorPosition().error(Trk::z));

        Vertex->SetErrorVertexCandidate(ErrorVertexCandidate);


        Vertex->SetNTrack(myVxCandidate->vxTrackAtVertex()->size());

        //        Vertex->SetVertexOrigin( VxItr );

        vertices->AddLast( Vertex );

    }

}
