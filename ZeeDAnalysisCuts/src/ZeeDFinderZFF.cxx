#include "ZeeDAnalysisCuts/ZeeDFinderZFF.h"

#include <TObjArray.h>

#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include "ZeeDEvent/ZeeDEvent.h"


//------------------------------------------------------
void ZeeDFinderZFF:: BookCuts()
{
    // Books selection requirements

    std::string name = std::string("ZeeDAnalysisSvc");

    ServiceHandle<IZeeDAnalysisSvc> ZeeDAnaOptions(name, "ZeeDCalcZ");
    StatusCode sc = ZeeDAnaOptions.retrieve();

    if ( sc.isFailure() ) {
        throw std::runtime_error("ZeeDCalcZ: cannot retrieve " + name + " service");
    }

    Double_t elecPtCut		= ZeeDAnaOptions->Elec_PtCut();
    Double_t elecEtaCut		= ZeeDAnaOptions->Elec_EtaCut();
    Double_t ZMassCutMin	= ZeeDAnaOptions->ZMassCutVeryLooseMin();
    Double_t ZMassCutMax	= ZeeDAnaOptions->ZMassCutVeryLooseMax();
    Double_t vertexZCut		= ZeeDAnaOptions->PriVertexZCut();
    Int_t minNumVertexCut	= ZeeDAnaOptions->MinNumVertexCut();

    // Book cuts:

    this->AddCut(new ZeeDCutPtMinBothLepZ ("PtMinBothElecZ" , elecPtCut));
    this->AddCut(new ZeeDCutZMassZ         ("ZMassZ", ZMassCutMin, ZMassCutMax));
    this->AddCut(new ZeeDCutPriVtxZ        ("PriVtxZ", vertexZCut));
    this->AddCut(new ZeeDCutMinNVertex("MinNVertex", minNumVertexCut));
    this->AddCut(new ZeeDCutEtaFFBothElecZ ("EtaFFBothElecZ", elecEtaCut));

    // Rank cuts differently:

    CutWeights.push_back(new CutWeight("ZMassZ",             10., getBitMask("ZMassZ")));  // LARGE priority
    CutWeights.push_back(new CutWeight("PtMinBothElecZ",      1., getBitMask("PtMinBothElecZ")));
    CutWeights.push_back(new CutWeight("PriVtxZ",             1., getBitMask("PriVtxZ")));
    CutWeights.push_back(new CutWeight("MinNVertex",           1., getBitMask("MinNVertex")));
    CutWeights.push_back(new CutWeight("EtaFFBothElecZ",      1., getBitMask("EtaFFBothElecZ")));

}
