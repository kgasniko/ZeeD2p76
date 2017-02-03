#include "ZeeDCalculator/ZeeDReadEventInlineZ.h"

#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDVertex.h"

#include "ZeeDMisc/ZeeDMisc.h"

#include <TObjArray.h>

//------------------------------------------------------
ZeeDReadEventInlineZ::ZeeDReadEventInlineZ() :
    fZeeDInlineZ(NULL),
    fSvcHelper("ZeeDReadEventInlineZ"),
    fAnaOptions(fSvcHelper.GetAnaSvc())
{
    DEBUG_MES_START;
    Info("ZeeDReadEventInlineZ::ZeeDReadEventInlineZ", " Initialisation ");

    fZeeDInlineZ = new ZeeDInlineZ;
    DEBUG_MES_END;
}

//------------------------------------------------------
ZeeDReadEventInlineZ::~ZeeDReadEventInlineZ()
{
    // Destructor
    DEBUG_MES_START;

    delete fZeeDInlineZ;

    DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDReadEventInlineZ::ReadInfo(RawEvent* event)
{
    DEBUG_MES_START;

    // Calc new inline event
    fZeeDInlineZ->CalcEvent();

    // Read electron information
    this->ReadElecInfo(event);

    // Fill fake event information
    this->ReadEventInfo(event);

    // Fill fake vertex information
    this->ReadVertexInfo(event);

    DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDReadEventInlineZ::ReadElecInfo(RawEvent* event)
{
    DEBUG_MES_START;

    TObjArray* electrons = event->GetElectrons();

    electrons->Clear();

    for (Int_t i = 0; i < 2; i++) {

        // Create new electron
        ZeeDElectron* fElectronInProcess = new ZeeDElectron();

	fElectronInProcess->setHasCharge(true);
        fElectronInProcess->setCharge( (i * 2) - 1 );

        // OQ Maps
        fElectronInProcess->IsGoodOQAOD().Set(kTRUE);
        fElectronInProcess->IsGoodOQAOD().Lock();

        // Author
        ZeeDElectronBags::Author authorBag;

        authorBag.bits       = 1;
        authorBag.isUnknown  = kTRUE;
        authorBag.isElectron = kTRUE;
        authorBag.isSofte    = kTRUE;
        authorBag.isPhoton   = kTRUE;
        authorBag.isFrwd     = kTRUE;
        authorBag.isRConv    = kTRUE;

        fElectronInProcess->Author().Set(authorBag);
        fElectronInProcess->Author().Lock();

        // ISEM
        ZeeDLeptonBags::IsEM IsEMBag;

        IsEMBag.bits = 9999;


        IsEMBag.isLoose        = fZeeDInlineZ->GetIsEM(i);
        IsEMBag.isLoosePP      = fZeeDInlineZ->GetIsEM(i);
        IsEMBag.isMedium       = fZeeDInlineZ->GetIsEM(i);
        IsEMBag.isMediumPP     = fZeeDInlineZ->GetIsEM(i);
        IsEMBag.isTight        = fZeeDInlineZ->GetIsEM(i);
        IsEMBag.isTightPP      = fZeeDInlineZ->GetIsEM(i);

        IsEMBag.isFrwdLoose    = fZeeDInlineZ->GetIsEM(i);
        IsEMBag.isFrwdTight    = fZeeDInlineZ->GetIsEM(i);

        fElectronInProcess->IsEM().Set(IsEMBag);
        fElectronInProcess->IsEM().Lock();

        // Calo Iso
        ZeeDElectronBags::CaloIso CaloIsoBag;

        CaloIsoBag.CaloIso98  = kTRUE;
        CaloIsoBag.CaloIso95  = kTRUE;

        fElectronInProcess->CaloIso().Set(CaloIsoBag);
        fElectronInProcess->CaloIso().Lock();


        // Track Match
        ZeeDElectronBags::TrackMatch trackMatchBag;

        trackMatchBag.deltaEta1 = 0;
        trackMatchBag.deltaEta2 = 0;
        trackMatchBag.deltaPhi2 = 0;

        trackMatchBag.expectHitInBLayer = 10;

        fElectronInProcess->TrackMatch().Set(trackMatchBag);
        fElectronInProcess->TrackMatch().Lock();


        // Shower
        ZeeDElectronBags::Shower showerBag;

        showerBag.f1      = 0;
        showerBag.f1core  = 0;
        showerBag.e2tsts1 = 0;
        showerBag.emins1  = 0;
        showerBag.weta1   = 0;
        showerBag.wtots1  = 0;
        showerBag.fracs1  = 0;
        showerBag.widths2 = 0;
        showerBag.f3core  = 0;
        showerBag.e237    = 0;
        showerBag.e233    = 0;
        showerBag.e277    = 0;
        showerBag.weta2   = 0;

        fElectronInProcess->Shower().Set(showerBag);
        fElectronInProcess->Shower().Lock();


        // Cluster
        ZeeDElectronBags::Cluster clusterBag;

        clusterBag.etaBE2           = (fZeeDInlineZ->GetGenElecSmear(i)).Eta();
        clusterBag.energy           = (fZeeDInlineZ->GetGenElecSmear(i)).E();
        clusterBag.eT               = clusterBag.energy / TMath::CosH( TMath::Abs(clusterBag.etaBE2) );
        clusterBag.fourVector       = fZeeDInlineZ->GetGenElecSmear(i);
        clusterBag.CentralLambda    = 0.0;
        clusterBag.SecondLambda     = 0.0;
        clusterBag.SecondR          = 0.0;
        clusterBag.longitudinal     = 0.0;
        clusterBag.lateral          = 0.0;
        clusterBag.FracMax          = 0.0;

        fElectronInProcess->Cluster().Set(clusterBag);
        fElectronInProcess->Cluster().Lock();


        // Track
        ZeeDLeptonBags::TrackParticle trackParticleBag;

        trackParticleBag.pt         = (fZeeDInlineZ->GetGenElecSmear(i)).Pt();
        trackParticleBag.eta        = (fZeeDInlineZ->GetGenElecSmear(i)).Eta();
        trackParticleBag.fourVector = fZeeDInlineZ->GetGenElecSmear(i);

        fElectronInProcess->TrackParticle().Set(trackParticleBag);
        fElectronInProcess->TrackParticle().Lock();

        // Track Summary
        ZeeDLeptonBags::TrackSummary trackSummaryBag;

        trackSummaryBag.numberOfBLayerHits = 5;
        trackSummaryBag.numberOfPixelHits  = 5;
        trackSummaryBag.numberOfSCTHits    = 5;

        fElectronInProcess->TrackSummary().Set(trackSummaryBag);
        fElectronInProcess->TrackSummary().Lock();

        // Vertex
        ZeeDLeptonBags::Vertex vertexBag;

        vertexBag.position.SetXYZ(0.1, 0.2, 0.3);
        vertexBag.Err2.SetXYZ(0.1, 0.1, 0.1);


        fElectronInProcess->Vertex().Set(vertexBag);
        fElectronInProcess->Vertex().Lock();

        // Trigger
        ZeeDLeptonBags::Trigger electronTriggerBag;

        std::vector<std::string> triggerElecMatch = (*fAnaOptions)->EFTriggerElecMatch();

        for(UInt_t nchain = 0; nchain != triggerElecMatch.size(); ++nchain) {

            electronTriggerBag.isMatchedToChain.insert(std::make_pair(ZeeDEnum::Trigger::ToEnum(triggerElecMatch[nchain]), kTRUE));

        }

        fElectronInProcess->Trigger().Set(electronTriggerBag);
        fElectronInProcess->Trigger().Lock();

        // Store loose electrons
        electrons->AddLast( fElectronInProcess );
    }

    DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDReadEventInlineZ::ReadEventInfo(RawEvent* event)
{
    // Get event run number and event number
    DEBUG_MES_START;

    event->EventNumber().Set(999999);
    event->RunNumber().Set(000001);
    event->OrigRunNumber().Set(000001);
    event->LB().Set(1);

    std::vector<std::string> triggerEvent = (*fAnaOptions)->TriggerEvent();

    ZeeDEventBags::Trigger eventTriggerBag;

    for(UInt_t nchain = 0; nchain != triggerEvent.size(); ++nchain) {

        eventTriggerBag.hasPassedChain.insert(std::make_pair(ZeeDEnum::Trigger::ToEnum(triggerEvent[nchain]), kTRUE));

    }

    event->Trigger().Set(eventTriggerBag);
    event->Trigger().Lock();

    event->LArErrorState().Set(0);
    event->LArErrorState().Lock();

    /*
	event->bcid().Set(100);
	event->LArError().Set(0);
	event->IperBCID().Set(5);
	event->nIperBCID().Set(5);
    */

    DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDReadEventInlineZ::ReadVertexInfo(RawEvent* event)
{
    // Get event run number and event number
    DEBUG_MES_START;

    // Get vertex array from event to be filled in
    TObjArray* vertexArray = event->GetVertices();

    // Clear vertex arrays
    vertexArray->Clear();

    for (int i=0; i<5; i++) {
        // Create vertex candidate
        ZeeDVertex* vertex = new ZeeDVertex();

        TVector3 vertexCandidate;
        vertexCandidate.SetXYZ(0.1,0.1,0.1);
        vertex->SetVertexCandidate(vertexCandidate);

        // Set vertex error vector
        TVector3 errorVertexCandidate;
        errorVertexCandidate.SetXYZ(0.1,0.1,0.1);
        vertex->SetErrorVertexCandidate(errorVertexCandidate);

        // Set number of track fitted to the vertex
        vertex->SetNTrack(20);

        // Add vertex to the array
        vertexArray->AddLast( vertex );
    }

    DEBUG_MES_END;
}


