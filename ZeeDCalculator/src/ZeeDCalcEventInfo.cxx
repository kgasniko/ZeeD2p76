

#include "ZeeDCalculator/ZeeDCalcEventInfo.h"

// Root libraries
#include <TSystem.h>
#include <TMath.h>

// ZeeD analysis includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"

#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDVertex.h"
#include "ZeeDEvent/ZeeDGenParticle.h"

#include "GoodRunsLists/TGoodRunsListReader.h"

#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDCalcEventInfo::ZeeDCalcEventInfo() :
    fEventInProcess(NULL),
    fSvcHelper("ZeeDCalcEventInfo"),
    fAnaOptions(fSvcHelper.GetAnaSvc()),
    fGRLReader(0)
{
    // Constructor

    if( (*fAnaOptions)->DoManualGRL() && ( (*fAnaOptions)->IsData() == kTRUE ) ) {

        fGRLReader = new Root::TGoodRunsListReader();

        const TString xmlToLoad = (*fAnaOptions)->ManualGRLFilename();

        fGRLReader->SetXMLFile(xmlToLoad);
        const Bool_t status_GRLReader = fGRLReader->Interpret();

        if(status_GRLReader == kFALSE) {

            Error("ZeeDCalcEventInfo::ZeeDCalcEventInfo", "TGoodRunsListReader::Interpret() failed!");

            gSystem->Exit(EXIT_FAILURE);

        }

        fGRL = fGRLReader->GetMergedGoodRunsList();

        fGRL.Summary(kFALSE);

    }

}

//------------------------------------------------------
ZeeDCalcEventInfo::~ZeeDCalcEventInfo()
{
    // Destructor
    delete fGRLReader;

}

//------------------------------------------------------
void ZeeDCalcEventInfo::Calculate(ZeeDEvent* event)
{
    fEventInProcess = event;

    // Calculate MCtype - must be first!
    if( (*fAnaOptions)->IsMC() ) {
        this->CalcMCtype();
    }

    if( (*fAnaOptions)->DoManualGRL()  ) {
        if ( (*fAnaOptions)->IsData() == kTRUE ) {
            this->CalcManualGRL();
        } else {
            fEventInProcess->IsInGRL().Set( kTRUE );
            fEventInProcess->IsInGRL().Lock();
        }
    }

    if( (*fAnaOptions)->IsData() == kFALSE &&
            (*fAnaOptions)->FillGenInfo() == kTRUE) {

        this->CalcGenBosons();
        this->CalcGenLeptons();

        // this->CalcDressing(); // Add dressed objects too.


    }

}

//------------------------------------------------------
void ZeeDCalcEventInfo::CalcNGoodVerticies(ZeeDEvent* event)
{
    // Calculates the number of good verticies
    DEBUG_MES_START;

    const TObjArray* vertexArray = event->GetVertices();
    CHECK_NULL_PTR(vertexArray);

    Int_t nVtx = 0, nPV = 0;

    for ( Int_t index = 0; index < vertexArray->GetEntriesFast(); ++index ) {
        ZeeDVertex* vertex = static_cast< ZeeDVertex* >( vertexArray->At(index) );

        if ( vertex->GetNTrack() >= (*fAnaOptions)->MinNumTrkCut() ) {
            ++nVtx;
        }
        if ( (*fAnaOptions)->CalcJets() )
            if ( (unsigned int)vertex->GetNTrack() >= (*fAnaOptions)->nTrkVertexJets() ) {
                ++nPV;	  
            }

    }

    event->Nvtx().Set(nVtx);

    if ( (*fAnaOptions)->CalcJets() ) event->NpvJets().Set( nPV );

    DEBUG_MES_END;
}

//------------------------------------------------------
void ZeeDCalcEventInfo::CalcManualGRL()
{

    if(fEventInProcess->RunNumber().IsSet() && fEventInProcess->LB().IsSet()) {

        const int runNumber = fEventInProcess->RunNumber().Get();
        const int LB = fEventInProcess->LB().Get();

        Bool_t isInGRL = fGRL.HasRunLumiBlock(runNumber, LB);

        fEventInProcess->IsInGRL().Set(isInGRL);
        fEventInProcess->IsInGRL().Lock();

    } else {

        Error("ZeeDCalcEventInfo::CalcManualGRL", "GRL decision can not be checked - run number or LB missing");

        Error("ZeeDCalcEventInfo::CalcManualGRL", "RunNumber().IsSet(): %i", fEventInProcess->RunNumber().IsSet());

        if(fEventInProcess->RunNumber().IsSet()) {
            Error("ZeeDCalcEventInfo::CalcManualGRL", "RunNumber().Get(): %i  ", fEventInProcess->RunNumber().Get());
        }

        Error("ZeeDCalcEventInfo::CalcManualGRL", "LB().IsSet(): %i", fEventInProcess->LB().IsSet());

        if(fEventInProcess->LB().IsSet()) {
            Error("ZeeDCalcEventInfo::CalcManualGRL", "LB().Get(): %i  ", fEventInProcess->LB().Get());
        }

        gSystem->Exit(EXIT_FAILURE);

    }

}

void ZeeDCalcEventInfo::CalcGenBosons()
{

    const TObjArray* genBosonArray = fEventInProcess->GetGenBosons();

    DEBUG_VAR(genBosonArray);
    fEventInProcess->SetGenBoson(NULL,ZeeDEnum::MCFSRLevel::Born);
    fEventInProcess->SetGenBoson(NULL,ZeeDEnum::MCFSRLevel::Bare);
    fEventInProcess->SetGenBoson(NULL,ZeeDEnum::MCFSRLevel::Dressed);

    if ( genBosonArray == NULL ) {
        Error("ZeeDCalcEventInfo::CalcGenBosons", "No generated Z boson information found");
        gSystem->Exit(EXIT_FAILURE);

        return;
    }

    // Useful to specify loop over Born abd Bare  explicitly:
    //const ZeeDEnum::MCFSRLevel::Value FSRLevels[] = {ZeeDEnum::MCFSRLevel::Born , ZeeDEnum::MCFSRLevel::Bare};
    const ZeeDEnum::MCFSRLevel::Value FSRLevels[] = {ZeeDEnum::MCFSRLevel::Born};
    for (int i =0; i<1; i++) {
        ZeeDEnum::MCFSRLevel::Value FSRLevel = FSRLevels[i];
        if (genBosonArray->GetEntriesFast() == 1){
            ZeeDGenParticle* pGenBoson = static_cast< ZeeDGenParticle* >( genBosonArray->At(0) );
            //std::cout << pGenBoson->GetParticleID() << std::endl;
            fEventInProcess->SetGenBoson(pGenBoson, FSRLevel);
        }else {
            for ( Int_t boson_idx = 0; boson_idx < genBosonArray->GetEntriesFast(); ++boson_idx ) {   
                ZeeDGenParticle* pGenBoson  = static_cast< ZeeDGenParticle* >( genBosonArray->At(boson_idx) );

                const TLorentzVector vec = pGenBoson->GetMCFourVector();
                //std::cout << vec.Pt() << std::endl;
                if ( pGenBoson->GetParticleStatus() == FSRLevel ) {
                    fEventInProcess->SetGenBoson(pGenBoson, FSRLevel);
                }
            }
        }
    }
    DEBUG_VAR(pGenBoson);
}

void ZeeDCalcEventInfo::CalcGenLeptons() 
{
    // Useful to specify loop over Born, Bare and Dressed explicitly:
    //   const ZeeDEnum::MCFSRLevel::Value FSRLevels[] = {ZeeDEnum::MCFSRLevel::Born , ZeeDEnum::MCFSRLevel::Bare, ZeeDEnum::MCFSRLevel::Dressed};
    const ZeeDEnum::MCFSRLevel::Value FSRLevels[] = {ZeeDEnum::MCFSRLevel::Born};
    for (int i =0; i<1; i++) {
        ZeeDEnum::MCFSRLevel::Value FSRLevel = FSRLevels[i];

        ZeeDGenParticle* lep1 = NULL;
        ZeeDGenParticle* lep2 = NULL;

        const ZeeDGenParticle* boson = fEventInProcess->GetGenBoson( FSRLevel );

        DEBUG_VAR(boson);


        if (boson == NULL) {
            fEventInProcess->SetGenLeptons(ZeeDGenElectrons(lep1, lep2), FSRLevel);
            return;
        }

        // Decay products:
        TObjArray* genDecayProducts = boson->GetDaughters();

        Bool_t lep1Found = kFALSE;
        Bool_t lep2Found = kFALSE;


        for (Int_t elec_idx = 0; elec_idx < genDecayProducts->GetEntriesFast(); ++elec_idx) {
            ZeeDGenParticle* PgenDecayProducts =
                static_cast< ZeeDGenParticle* >( genDecayProducts->At(elec_idx) );

            Int_t partID     = PgenDecayProducts->GetParticleID();

            // Select leptons
            if ( abs(partID) == ZeeDEnum::PID::electron
                    || abs(partID) == ZeeDEnum::PID::muon
                    || abs(partID) == ZeeDEnum::PID::enu
                    || abs(partID) == ZeeDEnum::PID::emu
               ) {


                if ( partID > 0) {            

                    if(lep1Found  ) {
                        Error("ZeeDCalcEventInfo::CalcLeptons", "Lepton 1 already found");
                    }

                    lep1      = PgenDecayProducts;
                    lep1Found = kTRUE;            
                }

                if ( partID < 0) {


                    if(lep2Found ) {
                        Error("ZeeDCalcEventInfo::CalcLeptons", "Lepton 2 already found");
                    }

                    lep2 = PgenDecayProducts;
                    lep2Found = kTRUE;

                }
            }

        }
        if (lep1Found && lep2Found){
            //std::cout << "Two leptons found " << std::endl;
           // std::cout << lep1->GetParticleID() << " " <<lep2->GetParticleID() <<std::endl;
          
            const TLorentzVector& ElFV  = lep1->GetMCFourVector();
            const TLorentzVector& PosFV = lep2->GetMCFourVector();
            Double_t elecPt = ElFV.Pt();
            Double_t posPt  = PosFV.Pt();
            Double_t elecPhi = ElFV.Phi();
            Double_t posPhi = PosFV.Phi();
            Double_t Mt = 2 * elecPt * (posPt);
            Mt *= 1 - TMath::Cos(elecPhi - posPhi);
            Mt = TMath::Sqrt(Mt);
            const TObjArray* genBosonArray = fEventInProcess->GetGenBosons();
            if (genBosonArray->GetEntriesFast() == 1){
                ZeeDGenParticle* pGenBoson = static_cast< ZeeDGenParticle* >( genBosonArray->At(0) );
                pGenBoson->SetMt(Mt);
            }else {
                for ( Int_t boson_idx = 0; boson_idx < genBosonArray->GetEntriesFast(); ++boson_idx ) {   
                    ZeeDGenParticle* pGenBoson  = static_cast< ZeeDGenParticle* >( genBosonArray->At(boson_idx) );
                    if ( pGenBoson->GetParticleStatus() == FSRLevel ) {
                        pGenBoson->SetMt(Mt);
                    }
                }
            }
        DEBUG_VAR(lep1);
        DEBUG_VAR(lep2);

        const ZeeDGenElectrons leptons(lep1, lep2);

        fEventInProcess->SetGenLeptons(leptons, FSRLevel );
        }
    }    
}

void ZeeDCalcEventInfo::CalcDressing() 
{

    // DR match:
    const double drMatch = (*fAnaOptions)->dressedElectronsDeltaR() ;

    // Start with the bare list:
    const ZeeDGenParticle* boson = fEventInProcess->GetGenBoson( ZeeDEnum::MCFSRLevel::Bare );
    //std::cout << boson->GetParticleID() << std::endl;
    if (boson == NULL)
        return;
    // Get the two leptons:
    const ZeeDGenElectrons leptons =  fEventInProcess->GetGenLeptons(  ZeeDEnum::MCFSRLevel::Bare );

    if (leptons.first == NULL || leptons.second == NULL){
        return;
    }

    // copy
    ZeeDGenParticle* bosDressed = new ZeeDGenParticle( *boson );
    ZeeDGenParticle* bosBare    = new ZeeDGenParticle( *boson );

    // Update bare-boson four momentum:
    bosBare->SetMCFourVector( leptons.first->GetMCFourVector() + leptons.second->GetMCFourVector() );
    fEventInProcess->SetGenBoson( bosBare,  ZeeDEnum::MCFSRLevel::Bare );

    // Create a copy of the leptons:
    ZeeDGenParticle* lepDressed1 = new ZeeDGenParticle( *leptons.first);
    ZeeDGenParticle* lepDressed2 = new ZeeDGenParticle( *leptons.second);

    // Match photons to charged leptons (mu and el) only:
    bool match1 = (abs(lepDressed1->GetParticleID()) == ZeeDEnum::PID::electron ) || (abs(lepDressed1->GetParticleID()) == ZeeDEnum::PID::muon );
    bool match2 = (abs(lepDressed2->GetParticleID()) == ZeeDEnum::PID::electron ) || (abs(lepDressed2->GetParticleID()) == ZeeDEnum::PID::muon );



    // Loop over decay products, find photons:
    TObjArray* genDecayProducts = boson->GetDaughters();
    for (Int_t elec_idx = 0; elec_idx < genDecayProducts->GetEntriesFast(); ++elec_idx) {
        ZeeDGenParticle* PgenDecayProducts =
            static_cast< ZeeDGenParticle* >( genDecayProducts->At(elec_idx) );

        Int_t partID     = PgenDecayProducts->GetParticleID();

        double DR1 = drMatch + 1.0;
        double DR2 = drMatch + 1.0;

        if ( partID == ZeeDEnum::PID::gamma ) {
            if ( match1 ) {
                DR1 = leptons.first->GetMCFourVector().DeltaR( PgenDecayProducts->GetMCFourVector());                
            }            
            if ( match2 ) {
                DR2 = leptons.second->GetMCFourVector().DeltaR( PgenDecayProducts->GetMCFourVector());                
            }


            if ( DR1 < DR2 ) {
                if (DR1 < drMatch ) {
                    // Add four vector to first lepton:
                    lepDressed1->SetMCFourVector(lepDressed1->GetMCFourVector()+PgenDecayProducts->GetMCFourVector());
                }
            }
            else {
                if (DR2 < drMatch ) {
                    // Add four vector to second lepton
                    lepDressed2->SetMCFourVector(lepDressed2->GetMCFourVector()+PgenDecayProducts->GetMCFourVector());
                }
            }
        }
    }

    const ZeeDGenElectrons leptonsPair(lepDressed1, lepDressed2);
    fEventInProcess->SetGenLeptons(leptonsPair,ZeeDEnum::MCFSRLevel::Dressed);

    // Sum of two leptons momenta is the boson momentum:

    bosDressed->SetMCFourVector( lepDressed1->GetMCFourVector() + lepDressed2->GetMCFourVector());
    fEventInProcess->SetGenBoson(bosDressed,ZeeDEnum::MCFSRLevel::Dressed);

}



//-----------------------------------------------------------------------------
void ZeeDCalcEventInfo::CalcMCtype() {

    // if MCtype is given use it
    if( (*fAnaOptions)->MCtype() != "" ) {

        const TString MCtype = (*fAnaOptions)->MCtype();
        fEventInProcess->MCtype().Set(ZeeDEnum::MCtype::ToEnum(MCtype));

    } else { // otherwise guess it from the datasetID from event or MetaData

        const Int_t datasetID = fEventInProcess->MCChannelNumber().IsSet() ? fEventInProcess->MCChannelNumber().Get() : (*fAnaOptions)->DatasetID();
        const ZeeDEnum::MCtype::Value MCtype = ZeeDMisc::MCtype(datasetID);

        fEventInProcess->MCtype().Set(MCtype);

    }

}
