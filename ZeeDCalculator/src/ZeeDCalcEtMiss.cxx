#include "ZeeDCalculator/ZeeDCalcEtMiss.h"

// Root libraries
#include <TSystem.h>
#include <TLorentzVector.h>

// Athena includes
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/ServiceHandle.h"

// ZeeD analysis includes
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "ZeeDMisc/ZeeDMisc.h"
#include "ZeeDEvent/ZeeDLepton.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDMuon.h"
#include "ZeeDEvent/ZeeDTau.h"
#include "ZeeDEvent/ZeeDPhoton.h"
#include "ZeeDEvent/ZeeDJet.h"
#include "ZeeDEvent/ZeeDTrack.h"

// MET Utility
#include "MissingETUtility/METUtility.h"
#include "MissingETUtility/METMuonSelection.h"
#include "METSystematics/METSystTool.h"
//#include "METTrackUtil/TrackMETMaker.h"
// Functions:
#include "ZeeDCalculator/ZeeDFunctions.h"
using namespace METSyst;

//------------------------------------------------------
ZeeDCalcEtMiss::ZeeDCalcEtMiss() : fSvcHelper("ZeeDCalcEtMiss"),
    fAnaOptions(fSvcHelper.GetAnaSvc())

{
    fSys = ZeeDSystematics::getInstance();

    fMETUtility = new METUtility();

    fMETUtility->setVerbosity(false);
    //fMETUtility->setDoMuEloss(true);
    fMETUtility->setDoMuEloss(false);
    fMETUtility->setIsMuons(true);
    fMETTrackUtil = new METTrack::TrackMETMaker;
    // fMETTrackUtil->init("TST_NTUP_SWMZ.config");
    fMETTrackUtil->init("RefJetJVFCUT.config", "/afs/desy.de/user/k/kgasniko/ZeeD/Reconstruction/MET/METTrackUtil/share");
    fSystTool = new METSystTool();
    fSystTool->initialise("METTrack_2012.config");

    fSys = ZeeDSystematics::getInstance();
    //fMETUtility->setSoftJetCut(20.); // Equivalent to switching to  GeV 
}

//------------------------------------------------------
ZeeDCalcEtMiss::~ZeeDCalcEtMiss()
{
    if ( fMETUtility != NULL ) delete fMETUtility;
    fMETUtility= NULL;
    if ( fSystTool != NULL ) delete fSystTool;
    if (fMETTrackUtil != NULL ) delete fMETTrackUtil;
}

//------------------------------------------------------
void ZeeDCalcEtMiss::Calculate(ZeeDEvent* event)
{
    Bool_t test(false);
    static Int_t countWarnings(0);
    //return;

    if (test ) 
    {
        std::cout <<std::endl<<std::endl<<std::flush;
        Info(__PRETTY_FUNCTION__,"start"); 
        std::cout <<std::endl<<std::endl<<std::flush;
    }
    // Get Et miss
    TObjArray* etMissArray = event->GetEtMissArray();

    // NOTE: currently only one etMiss object is filled
    if (etMissArray == NULL || etMissArray->GetEntriesFast() <= 0 ) 
    {
        Error("ZeeDCalcW::ReadEtMiss", "Et miss objects are not filled");
        gSystem->Exit(EXIT_FAILURE);
    }

    if (etMissArray->At(1) != NULL ) delete etMissArray->At(1);
    //ZeeDEtMiss* recalculatedEtMiss = new ZeeDEtMiss();
    ZeeDEtMiss* etMiss = static_cast<ZeeDEtMiss*>( etMissArray->At(0) );

    // Neutrino
    Double_t neutrinoPt  = etMiss->GetEt();
    Double_t neutrinoPhi = etMiss->GetPhi();

    if (test)
    {
        std::cout <<__PRETTY_FUNCTION__
            << " EtMiss Et = " << neutrinoPt << " GeV "
            << " Phi = "<< neutrinoPhi
            <<std::endl;
    }
    //
    //--------------------------------------------------------------------------
    //
    if ((*fAnaOptions)->RecalcEtMiss()) {
        std::vector<std::string> collections = (*fAnaOptions)->JetCollectionNames();
        std::string jetCollection("");
        for (unsigned int i = 0; i < collections.size(); i++)
        {
            //	std::cout << "name = " <<collections[i] << std::endl;	
            if ( collections[i].find("AntiKt4LCTopo") != std::string::npos ) 
            {
                jetCollection = collections[i].c_str();
                break;
            }
        }

        if ( (jetCollection.size() == 0)  && ( countWarnings < 5) )
        {
            countWarnings++;
            Error(__PRETTY_FUNCTION__, "AntiKt4LCTopo Jet collection is needed for Missing ET calculations");
            Info(__PRETTY_FUNCTION__, " Using the default MET from AOD");
            return;
        }

        fMETUtility->reset();
        fSystTool->reset();
        fMETTrackUtil->reset();
        //fMETUtility->setDoJetJVF(true);
        //fMETUtility->setIsMuid(true);

        const TObjArray* tracks = event->GetTracks();
        std::vector<Float_t> trk_pt; trk_pt.reserve(tracks->GetEntries());
        std::vector<Float_t> trk_eta; trk_eta.reserve(tracks->GetEntries());
        std::vector<Float_t> trk_phiPV; trk_phiPV.reserve(tracks->GetEntries());
        std::vector<Float_t> trk_d0PV; trk_d0PV.reserve(tracks->GetEntries());
        std::vector<Float_t> trk_z0PV; trk_z0PV.reserve(tracks->GetEntries());
        std::vector<Int_t> trk_nPixHits; trk_nPixHits.reserve(tracks->GetEntries());
        std::vector<Int_t> trk_nSCTHits; trk_nSCTHits.reserve(tracks->GetEntries());
        std::vector<Float_t> trk_cov; trk_cov.reserve(tracks->GetEntries());
        std::vector<Float_t> trk_qoverp; trk_qoverp.reserve(tracks->GetEntries());
        std::vector<Float_t> trk_cl_pt; trk_cl_pt.reserve(tracks->GetEntries());
        std::vector<Float_t> trk_cl_eta; trk_cl_eta.reserve(tracks->GetEntries());
        std::vector<Float_t> trk_cl_phi; trk_cl_phi.reserve(tracks->GetEntries());

        for (Int_t iZeeD = 0; iZeeD < tracks->GetEntries(); iZeeD++){
            ZeeDTrack* track = (ZeeDTrack*)tracks->At(iZeeD);
            trk_pt.push_back(track->GetFourVector().Pt()*GeV);
            trk_eta.push_back(track->GetFourVector().Eta());
            trk_phiPV.push_back(track->GetTrackPhi0());
            trk_d0PV.push_back(track->GetTrackD0());
            trk_z0PV.push_back(track->GetTrackZ0());
            trk_nPixHits.push_back(track->GetTrackNumberOfPixelHits());
            trk_nSCTHits.push_back(track->GetTrackNumberOfSCTHits());
        }
        fMETTrackUtil->fillTracks(&trk_pt, &trk_eta, &trk_phiPV, &trk_d0PV, &trk_z0PV,
                &trk_nPixHits, &trk_nSCTHits);
        fMETTrackUtil->filterTracks(&trk_cov, &trk_qoverp, &trk_cl_pt, &trk_cl_eta, &trk_cl_phi);


        //
        //--------------------------------------------------------------------------
        //

        Int_t nObjMET(etMiss->GetMETCompositionElectron_index().size());

        std::vector<Float_t> el_pt;  el_pt. reserve(nObjMET);
        std::vector<Float_t> el_eta; el_eta.reserve(nObjMET);
        std::vector<Float_t> el_phi; el_phi.reserve(nObjMET);
        std::vector<Float_t> el_wet; el_wet.reserve(nObjMET);
        std::vector<std::vector<Float_t> > el_wet2; el_wet2.reserve(nObjMET);
        std::vector<Float_t> el_wpx; el_wpx.reserve(nObjMET);
        std::vector<Float_t> el_wpy; el_wpy.reserve(nObjMET);
        std::vector<UShort_t>  el_swd; el_swd.reserve(nObjMET);
        std::vector<UInt_t>  el_swd2; el_swd2.reserve(nObjMET);
        std::vector<Float_t> el_cl_E; el_cl_E.reserve(nObjMET);
        std::vector<Int_t> el_auth; el_auth.reserve(nObjMET);
        std::vector<Float_t> el_ptUn; el_ptUn.reserve(nObjMET);
        std::vector<Float_t> el_etaUn; el_etaUn.reserve(nObjMET);
        std::vector<Float_t> el_phiUn; el_phiUn.reserve(nObjMET);
        const TObjArray* electrons = event->GetElectrons();
        double sumElecPx=0, sumElecPy=0, sumElecPt=0;
        for ( Int_t iMET = 0; iMET < nObjMET; iMET++ )
        {
            Int_t zeedIndex(-1);
            const ZeeDElectron* electron(NULL);
            for ( Int_t iZeeD = 0; iZeeD < electrons->GetEntries(); iZeeD++)
            {
                electron = dynamic_cast<const ZeeDElectron*>( electrons->At( iZeeD ) ); 
                // if (test )   std::cout <<"electron = " << electron << std::endl<<std::flush;
                Int_t test1( electron->GetCollectionIndex() );
                //std::cout <<"electron test1 = " << test1 << std::endl<<std::flush;

                Int_t test2( etMiss->GetMETCompositionElectron_index().at(iMET) );
                //if (test ) std::cout <<"electron test2 = " << test2 << std::endl<<std::flush;
                if (  test2 == test1 ) 
                {
                    zeedIndex = iMET;
                    break;
                }
            }
            if (test ) 	std::cout <<" Electron Found for " << iMET+1 <<" out of "<< nObjMET
                << " index = "<<zeedIndex
                    <<std::endl;
            if ( zeedIndex != -1 )
            {	
                sumElecPx+=electron->GetFourVector().Px();
                sumElecPy+=electron->GetFourVector().Py();
                sumElecPt+=electron->GetFourVector().Pt();
                el_pt .push_back( electron->GetFourVector().Pt()*GeV );
                el_eta.push_back( electron->GetFourVector().Eta() );
                el_phi.push_back( electron->GetFourVector().Phi() );
                el_cl_E.push_back( electron->Cluster().Get().energy*GeV);
                el_auth.push_back ( electron->Author().Get().bits);
                el_ptUn.push_back(electron->TrackParticle().Get().pt*GeV);
                el_etaUn.push_back(electron->TrackParticle().Get().eta);
                el_phiUn.push_back(electron->TrackParticle().Get().fourVector.Phi());
                //	 	std::cout << "iMET = " << iMET << " wpx = "  << etMiss->GetMETCompositionElectron_wpx()[ iMET ] << " wpy = "<<  etMiss->GetMETCompositionElectron_wpy()[ iMET ] << std::endl; 
                el_wet.push_back( etMiss->GetMETCompositionElectron_wet()[ iMET ] );
                el_wpx.push_back( etMiss->GetMETCompositionElectron_wpx()[ iMET ] );
                el_wpy.push_back( etMiss->GetMETCompositionElectron_wpy()[ iMET ] );
                el_swd.push_back( etMiss->GetMETCompositionElectron_statusWord()[ iMET ] );
                el_swd2.push_back(  etMiss->GetMETCompositionElectron_statusWord()[ iMET ]);
                std::vector<Float_t> bufwet; bufwet.push_back(etMiss->GetMETCompositionElectron_wet()[ iMET ] );
                el_wet2.push_back( bufwet);
            }
        }

        //     const std::vector<Float_t> *el_ptP = el_pt, *el_etaP = *el_eta, *el_phiP = *el_phi, *el_wetP = *el_wet, *el_wpxP = *el_wpx, *el_wpyP = *el_wpy;
        //     const std::vector<UInt_t> *el_swdP = *el_swd;
        if (test )  Info(__PRETTY_FUNCTION__,"setting MET elec"); std::cout <<std::flush;

        fMETUtility -> setElectronParameters( &el_pt, &el_eta, &el_phi, &el_wet, &el_wpx, &el_wpy, &el_swd );
        fMETTrackUtil-> setElectrons( &el_wet, &el_swd2, &el_cl_E, &el_auth, 
                &el_eta, &el_phi, &el_ptUn, 
                &el_etaUn, &el_phiUn);
        //if (test )  Info(__PRETTY_FUNCTION__,"setting MET elec done"); std::cout <<std::flush;

        //
        //--------------------------------------------------------------------------
        //

        nObjMET = (etMiss->GetMETCompositionPhoton_index().size());

        std::vector<Float_t> ph_pt;  ph_pt. reserve(nObjMET);
        std::vector<Float_t> ph_eta; ph_eta.reserve(nObjMET);
        std::vector<Float_t> ph_phi; ph_phi.reserve(nObjMET);
        std::vector<Float_t> ph_wet; ph_wet.reserve(nObjMET);
        std::vector<Float_t> ph_wpx; ph_wpx.reserve(nObjMET);
        std::vector<Float_t> ph_wpy; ph_wpy.reserve(nObjMET);
        std::vector<UShort_t>  ph_swd; ph_swd.reserve(nObjMET);

        const TObjArray* photons = event->GetPhotons();
        for ( Int_t iMET = 0; iMET < nObjMET; iMET++ )
        {
            Int_t zeedIndex(-1);
            const ZeeDPhoton* photon(NULL);
            for ( Int_t iZeeD = 0; iZeeD < photons->GetEntries(); iZeeD++)
            {
                photon = dynamic_cast<const ZeeDPhoton*>( photons->At( iZeeD ) ); 
                if (test ) std::cout <<"photon = " << photon << std::endl<<std::flush;
                Int_t test1( photon->GetCollectionIndex() );
                if (test )  std::cout <<"photon test1 = " << test1 << std::endl<<std::flush;

                Int_t test2( etMiss->GetMETCompositionPhoton_index().at(iMET) );
                if (test ) std::cout <<"photon test2 = " << test2 << std::endl<<std::flush;
                if (  test2 == test1 ) 
                {
                    zeedIndex = iMET;
                    break;
                }
            }
            if (test ) std::cout <<" Photon Found for " << iMET+1 <<" out of "<< nObjMET
                << " inex = "<<zeedIndex
                    <<std::endl;
            if ( zeedIndex != -1 )
            {
                ph_pt .push_back( photon->GetFourVector().Pt()*GeV );
                ph_eta.push_back( photon->GetFourVector().Eta() );
                ph_phi.push_back( photon->GetFourVector().Phi() );

                ph_wet.push_back( etMiss->GetMETCompositionPhoton_wet()[ iMET ] );
                ph_wpx.push_back( etMiss->GetMETCompositionPhoton_wpx()[ iMET ] );
                ph_wpy.push_back( etMiss->GetMETCompositionPhoton_wpy()[ iMET ] );
                ph_swd.push_back( etMiss->GetMETCompositionPhoton_statusWord()[ iMET ] );
            }
        }

        //     const std::vector<Float_t> *ph_ptP = ph_pt, *ph_etaP = *ph_eta, *ph_phiP = *ph_phi, *ph_wetP = *ph_wet, *ph_wpxP = *ph_wpx, *ph_wpyP = *ph_wpy;
        //     const std::vector<UInt_t> *ph_swdP = *ph_swd;
        if (test )  Info(__PRETTY_FUNCTION__,"setting MET photon"); std::cout <<std::flush;

        fMETUtility -> setPhotonParameters( &ph_pt, &ph_eta, &ph_phi, &ph_wet, &ph_wpx, &ph_wpy, &ph_swd );

        //    if (test )  Info(__PRETTY_FUNCTION__,"setting MET done"); std::cout <<std::flush;

        //
        //--------------------------------------------------------------------------
        //

        nObjMET = (etMiss->GetMETCompositionTau_index().size());

        std::vector<Float_t> tau_pt;  tau_pt. reserve(nObjMET);
        std::vector<Float_t> tau_eta; tau_eta.reserve(nObjMET);
        std::vector<Float_t> tau_phi; tau_phi.reserve(nObjMET);
        std::vector<Float_t> tau_wet; tau_wet.reserve(nObjMET);
        std::vector<Float_t> tau_wpx; tau_wpx.reserve(nObjMET);
        std::vector<Float_t> tau_wpy; tau_wpy.reserve(nObjMET);
        std::vector<UShort_t>  tau_swd; tau_swd.reserve(nObjMET);

        const TObjArray* taus = event->GetTaus();
        for ( Int_t iMET = 0; iMET < nObjMET; iMET++ )
        {
            Int_t zeedIndex(-1);
            const ZeeDTau* tau(NULL);
            for ( Int_t iZeeD = 0; iZeeD < taus->GetEntries(); iZeeD++){
                tau = dynamic_cast<const ZeeDTau*>( taus->At( iZeeD ) ); 
                if (test )  std::cout <<"tau = " << tau << std::endl<<std::flush;
                Int_t test1( tau->GetCollectionIndex() );
                if (test )  std::cout <<"tau test1 = " << test1 << std::endl<<std::flush;

                Int_t test2( etMiss->GetMETCompositionTau_index().at(iMET) );
                if (test ) std::cout <<"tau test2 = " << test2 << std::endl<<std::flush;
                if (  test2 == test1 ) 
                {
                    zeedIndex = iMET;
                    break;
                }
            }
            if (test ) 	std::cout <<" Tau Found for " << iMET+1 <<" out of "<< nObjMET
                << " inex = "<<zeedIndex
                    <<std::endl;
            if ( zeedIndex != -1 )
            {
                tau_pt .push_back( tau->GetFourVector().Pt()*GeV );
                tau_eta.push_back( tau->GetFourVector().Eta() );
                tau_phi.push_back( tau->GetFourVector().Phi() );

                tau_wet.push_back( etMiss->GetMETCompositionTau_wet()[ iMET ] );
                tau_wpx.push_back( etMiss->GetMETCompositionTau_wpx()[ iMET ] );
                tau_wpy.push_back( etMiss->GetMETCompositionTau_wpy()[ iMET ] );
                tau_swd.push_back( etMiss->GetMETCompositionTau_statusWord()[ iMET ] );
            }
        }

        if (test )  Info(__PRETTY_FUNCTION__,"setting MET tau"); std::cout <<std::flush;

        fMETUtility -> setTauParameters( &tau_pt, &tau_eta, &tau_phi, &tau_wet, &tau_wpx, &tau_wpy, &tau_swd );

        //  if (test ) Info(__PRETTY_FUNCTION__,"setting MET done"); std::cout <<std::flush;

        //
        //--------------------------------------------------------------------------
        //

        nObjMET = (etMiss->GetMETCompositionMuon_index().size());

        std::vector<Float_t> muon_pt;  muon_pt. reserve(nObjMET);
        std::vector<Float_t> muon_eta; muon_eta.reserve(nObjMET);
        std::vector<Float_t> muon_phi; muon_phi.reserve(nObjMET);
        std::vector<Float_t> muon_wet; muon_wet.reserve(nObjMET); 
        std::vector<Float_t> muon_wpx; muon_wpx.reserve(nObjMET);
        std::vector<Float_t> muon_wpy; muon_wpy.reserve(nObjMET);
        std::vector<UShort_t> muon_swd; muon_swd.reserve(nObjMET);
        std::vector<UInt_t> muon_swd2; muon_swd2.reserve(nObjMET);
        std::vector<Float_t> muon_IDQoverP; muon_IDQoverP.reserve(nObjMET);
        std::vector<Float_t> muon_IDTheta; muon_IDTheta.reserve(nObjMET);
        std::vector<Float_t> muon_IDPhi; muon_IDPhi.reserve(nObjMET);
        std::vector<Float_t> muon_mspt;     muon_mspt.   reserve(nObjMET);
        std::vector<Float_t> muon_mstheta;  muon_mstheta.reserve(nObjMET);
        std::vector<Float_t> muon_msphi;    muon_msphi.  reserve(nObjMET);
        std::vector<Float_t> muon_eLoss;    muon_eLoss.  reserve(nObjMET);
        std::vector<Int_t> muon_segment;  muon_segment.reserve(nObjMET);

        const TObjArray* Muons = event->GetMuons();

        if (test )   
        {
            std::cout <<"Muon = entries" <<  Muons->GetEntries() << std::endl<<std::flush;
            std::cout <<"Muon MET = entries" <<  nObjMET << std::endl<<std::flush;
        }
        std::vector<Float_t> mu_MET_wet_new; mu_MET_wet_new.reserve(nObjMET);
        std::vector<Float_t> mu_MET_wpx_new; mu_MET_wpx_new.reserve(nObjMET);
        std::vector<Float_t> mu_MET_wpy_new; mu_MET_wpy_new.reserve(nObjMET);
        std::vector<UShort_t> mu_MET_statusWord_new; mu_MET_statusWord_new.reserve(nObjMET);
        std::vector<int> mu_loose; mu_loose.reserve(nObjMET);
        std::vector<int> mu_comb;  mu_comb.reserve(nObjMET);
        std::vector<int> mu_segm;  mu_segm.reserve(nObjMET);
        std::vector<int> mu_alone; mu_alone .reserve(nObjMET);
        std::vector<int> mu_Pix;   mu_Pix.reserve(nObjMET);
        std::vector<int> mu_SCT;     mu_SCT.reserve(nObjMET);
        std::vector<int> mu_Csc;     mu_Csc.reserve(nObjMET);

        for ( Int_t iMET = 0; iMET < nObjMET; iMET++ )
        {
            Int_t zeedIndex(-1);
            const ZeeDMuon* Muon(NULL);
            for ( Int_t iZeeD = 0; iZeeD < Muons->GetEntries(); iZeeD++)
            {
                Muon = dynamic_cast<const ZeeDMuon*>( Muons->At( iZeeD ) ); 
                if (test )   std::cout <<"Muon = " << Muon << std::endl<<std::flush;
                Int_t test1( Muon->GetCollectionIndex());
                if (test )  std::cout <<"Muon test1 = " << test1 << std::endl<<std::flush;

                Int_t test2( etMiss->GetMETCompositionMuon_index().at(iMET) );
                if (test )  std::cout <<"Muon test2 = " << test2 << std::endl<<std::flush;
                if (  test2 == test1 ) 
                {
                    zeedIndex = iMET;
                    break;
                }
            }
            if (test ) 	std::cout <<" Muon Found for " << iMET+1 <<" out of "<< nObjMET
                << " inex = "<<zeedIndex
                    <<std::endl;
            if ( zeedIndex != -1 )
            {
                //recomputing new weights for muons
                if (Muon->IsEM().IsSet() && Muon->TrackSummary().IsSet() && Muon->Properties().IsSet()) {
                    mu_loose.push_back( Muon->IsEM().Get().isLoosePP );
                    mu_comb.push_back ( Muon->Properties().Get().fisCombinedMuon );
                    mu_segm.push_back ( Muon->Properties().Get().fisSegmentTaggedMuon );
                    mu_alone.push_back( Muon->Properties().Get().fisStandAloneMuon );
                    mu_Pix.push_back  ( Muon->TrackSummary().Get().numberOfPixelHits );
                    mu_SCT.push_back  ( Muon->TrackSummary().Get().numberOfSCTHits );
                    mu_Csc.push_back  ( Muon->TrackSummary().Get().numberOfCscEtaHits );
                }
                /*
                   vector<Float_t> muon_wet_row(1);muon_wet_row.push_back( etMiss->GetMETCompositionMuon_wet()[ iMET ] );
                   vector<Float_t> muon_wpx_row(1);muon_wpx_row.push_back( etMiss->GetMETCompositionMuon_wpx()[ iMET ] );
                   vector<Float_t> muon_wpy_row(1); muon_wpy_row.push_back( etMiss->GetMETCompositionMuon_wpy()[ iMET ] );
                   vector<UShort_t> muon_swd_row(1);muon_swd_row.push_back( etMiss->GetMETCompositionMuon_statusWord()[ iMET ] );
                   */
                muon_wet.push_back( etMiss->GetMETCompositionMuon_wet()[ iMET ]  );
                muon_wpx.push_back( etMiss->GetMETCompositionMuon_wpx()[ iMET ] );
                muon_wpy.push_back( etMiss->GetMETCompositionMuon_wpy()[ iMET ] );
                muon_swd.push_back( etMiss->GetMETCompositionMuon_statusWord()[ iMET ] );
                muon_swd2.push_back( etMiss->GetMETCompositionMuon_statusWord()[ iMET ] );

                muon_pt .push_back( Muon->GetFourVector().Pt()*GeV );
                muon_eta.push_back( Muon->GetFourVector().Eta() );
                muon_phi.push_back( Muon->GetFourVector().Phi() );
                if (Muon->CorMomentum().IsSet()){
                    muon_mspt.push_back   ( Muon->CorMomentum().Get().fMSPtCor*GeV );
                    muon_mstheta.push_back( Muon->MStrack().Get().fmstheta_exPV );
                    muon_msphi.push_back  ( Muon->MStrack().Get().fmsphi_exPV );
                }else if (Muon->MStrack().IsSet()){
                    double ptMS = fabs(sin(Muon->MStrack().Get().fmstheta_exPV)/Muon->MStrack().Get().fmsqoverp_exPV);
                    muon_mspt.push_back   ( ptMS*GeV );
                    muon_mstheta.push_back( Muon->MStrack().Get().fmstheta_exPV );
                    muon_msphi.push_back  ( Muon->MStrack().Get().fmstheta_exPV );
                } 
                else {
                    //std::cout << "SOMETHING WRONG!" << std::endl;
                    muon_mspt.push_back   ( 0.0);
                    muon_mstheta.push_back( 0.0);
                    muon_msphi.push_back  ( 0.0);
                }
                if (Muon->IDtrack().IsSet()){
                    muon_IDTheta.push_back( Muon->IDtrack().Get().fidtheta_exPV);
                    muon_IDPhi.push_back (Muon->IDtrack().Get().fidphi_exPV);
                    muon_IDQoverP.push_back (Muon->IDtrack().Get().fidqoverp_exPV/GeV);
                    muon_eLoss.push_back  ( Muon->IDtrack().Get().fCaloEnergyLoss*GeV );
                }
                else  {
                    //std::cout << "SOMETHING WRONG! " << std::endl;
                    muon_eLoss.push_back  (0.0);
                }
                if (Muon->Properties().IsSet())
                    muon_segment.push_back( Muon->Properties().Get().fisSegmentTaggedMuon );
                else{ 
                    std::cout << "SOMETHING WRONG!" << std::endl;
                    muon_segment.push_back(0);
                }
                if (test)
                {
                    std::cout << " muon    phi "<<Muon->GetFourVector().Phi()
                        << " muon    pt  "<<Muon->GetFourVector().Pt()
                        << " muon ms phi "<<Muon->MStrack().Get().fmstrackphi
                        << " muon ms pt  "<<Muon->MStrack().Get().fmstrackpt
                        << " muon id phi "<<Muon->IDtrack().Get().fidtrackphi
                        << " muon id pt  "<<Muon->IDtrack().Get().fidtrackpt
                        <<std::endl;
                }
            }
        }

        //     const std::vector<Float_t> *muon_ptP = muon_pt, *muon_etaP = *muon_eta, *muon_phiP = *muon_phi, *muon_wetP = *muon_wet, *muon_wpxP = *muon_wpx, *muon_wpyP = *muon_wpy;
        //     const std::vector<UInt_t> *muon_swdP = *muon_swd;
        if (test )  Info(__PRETTY_FUNCTION__,"setting MET muon "); std::cout <<std::flush;
        METMuons::recompute_Muons_MET_weights( 
                &muon_pt, &muon_eta, &mu_loose,
                &mu_comb, &mu_segm, &mu_alone,
                &mu_Pix, &mu_SCT, &mu_Csc,
                &muon_wet, &muon_wpx, &muon_wpy, &muon_swd,
                mu_MET_wet_new, mu_MET_wpx_new, mu_MET_wpy_new, mu_MET_statusWord_new);

        /*std::vector<Float_t> muon_wet_new = mu_MET_wet_new.front();
          std::cout << muon_wet_new[1] <<std::endl; 
          std::vector<Float_t> muon_wpx_new = mu_MET_wpx_new.front();
          std::vector<Float_t> muon_wpy_new = mu_MET_wpy_new.front();
          std::vector<UShort_t> muon_swd_new (mu_MET_statusWord_new.front());
          */


        if (Muons->GetEntries() != 0) {
            fMETUtility -> setMuonParameters( &muon_pt, &muon_eta, &muon_phi, &mu_MET_wet_new, &mu_MET_wpx_new, &mu_MET_wpy_new, &mu_MET_statusWord_new);
            //  fMETUtility -> setMuonParameters( &muon_pt, &muon_eta, &muon_phi, &muon_wet_new, &muon_wpx_new, &muon_wpy_new, &muon_swd_new);
            // **** FIXME still needs to be implemented
            fMETTrackUtil->setMuons(&muon_wet, &muon_swd2, &muon_IDQoverP, &muon_IDTheta, &muon_IDPhi); 
            fMETUtility -> setExtraMuonParameters( &muon_mspt, &muon_mstheta, &muon_msphi );
            //fMETUtility -> setMuonEloss( &muon_eLoss );
            fMETUtility -> configMuTagFix( true, &muon_segment );
        }
        //  if (test )  Info(__PRETTY_FUNCTION__,"setting MET done"); std::cout <<std::flush;

        //
        //--------------------------------------------------------------------------
        //


        nObjMET = (etMiss->GetMETCompositionJet_index().size());

        std::vector<Float_t> jet_pt;  jet_pt. reserve(nObjMET);
        std::vector<Float_t> jet_e ;  jet_e.  reserve(nObjMET);
        std::vector<Float_t> jet_eta; jet_eta.reserve(nObjMET);
        std::vector<Float_t> jet_phi; jet_phi.reserve(nObjMET);
        std::vector<Float_t> jet_wet; jet_wet.reserve(nObjMET);
        std::vector<Float_t> jet_wpx; jet_wpx.reserve(nObjMET);
        std::vector<Float_t> jet_wpy; jet_wpy.reserve(nObjMET);
        std::vector<UShort_t>  jet_swd; jet_swd.reserve(nObjMET);
        //test = kTRUE;
        const TObjArray* jets = event->GetJets( jetCollection, kFALSE );
        for ( Int_t iMET = 0; iMET < nObjMET; iMET++ )
        {
            Int_t zeedIndex(-1);
            const ZeeDJet* jet(NULL);
            for ( Int_t iZeeD = 0; iZeeD < jets->GetEntries(); iZeeD++)
            {
                jet = dynamic_cast<const ZeeDJet*>( jets->At( iZeeD ) ); 
                if (test ) std::cout <<"jet = " << jet << std::endl<<std::flush;
                Int_t test1( jet->GetCollectionIndex() );
                if (test )  std::cout <<"jet test1 = " << test1 << std::endl<<std::flush;

                Int_t test2( etMiss->GetMETCompositionJet_index().at(iMET) );
                if (test ) std::cout <<"jet test2 = " << test2 << std::endl<<std::flush;
                if (  test2 == test1 ) 
                {
                    zeedIndex = iMET;
                    break;
                }
            }
            if (test ) std::cout <<" Jet Found for " << iMET+1 <<" out of "<< nObjMET
                << " inex = "<<zeedIndex
                    <<std::endl;
            if ( zeedIndex != -1 )
            {
                jet_pt .push_back( jet->GetFourVector().Pt()*GeV );
                jet_e  .push_back( jet->GetFourVector().E() *GeV );
                jet_eta.push_back( jet->GetFourVector().Eta() );
                jet_phi.push_back( jet->GetFourVector().Phi() );

                jet_wet.push_back( etMiss->GetMETCompositionJet_wet()[ iMET ] );
                jet_wpx.push_back( etMiss->GetMETCompositionJet_wpx()[ iMET ] );
                jet_wpy.push_back( etMiss->GetMETCompositionJet_wpy()[ iMET ] );
                jet_swd.push_back( etMiss->GetMETCompositionJet_statusWord()[ iMET ] );
            }
        }


        if (test )  Info(__PRETTY_FUNCTION__,"setting MET jet"); std::cout <<std::flush;

        fMETUtility -> setJetParameters( &jet_pt, &jet_eta, &jet_phi, &jet_e, &jet_wet, &jet_wpx, &jet_wpy, &jet_swd );
        fMETTrackUtil->assignTracks();

        double TrackMETSoft_Pt = fMETTrackUtil->getTrackMET().et()/1000.;
        double TrackMETSoft_Px = fMETTrackUtil->getTrackMET().etx()/1000.;
        double TrackMETSoft_Py = fMETTrackUtil->getTrackMET().ety()/1000.;
        double TrackMETSoft_SumPt = fMETTrackUtil->getTrackMET().sumet()/1000.;
        //	std::cout << TrackMETSoft_Px << "   " << TrackMETSoft_SumPt << std::endl;
        //------------------------------------------------------
        if ((*fAnaOptions)->DeleteSoftTerm()){
            fMETUtility ->  setMETTerm( METUtil::SoftTerms, 
                    0,
                    0,
                    0 
                    );
        } else 
            fMETUtility ->  setMETTerm( METUtil::SoftTerms, 
                    etMiss->GetMET_SoftTermETX() * GeV,
                    etMiss->GetMET_SoftTermETY() * GeV,
                    etMiss->GetMET_SoftTermETSUM() * GeV    
                    );
        /*	else 
            fMETUtility->setMETTerm(METUtil::SoftTerms, TrackMETSoft_Px,TrackMETSoft_Py,TrackMETSoft_SumPt);
            *///fMETUtility->setMETTerm(METUtil::RefEle, sumElecPx, sumElecPy, sumElecPt);		

        METUtil::METObject refFinal; 
        if (fSys->isShiftInUse(ZeeDSystematics::eEtMissScaleUp)) {
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::EESUp);
        }else if (fSys->isShiftInUse(ZeeDSystematics::eEtMissScaleDown)) {
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::EESDown);
        }else if (fSys->isShiftInUse(ZeeDSystematics::muEtMissScaleUp)) {
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::MESUp);
        }else if (fSys->isShiftInUse(ZeeDSystematics::muEtMissScaleDown)) {
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::MESDown);
        }else if (fSys->isShiftInUse(ZeeDSystematics::eEtMissResoUp)) {
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::EERUp);
        }else if (fSys->isShiftInUse(ZeeDSystematics::eEtMissResoDown)) {
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::EERDown);
        }else if (fSys->isShiftInUse(ZeeDSystematics::muEtMissResoIDUp)) {
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::MERIDUp);
        }else if (fSys->isShiftInUse(ZeeDSystematics::muEtMissResoIDDown)) {
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::MERIDDown);
        }else if (fSys->isShiftInUse(ZeeDSystematics::muEtMissResoMSUp)) {
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::MERMSUp);
        }else if (fSys->isShiftInUse(ZeeDSystematics::muEtMissResoMSDown)) {
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::MERMSDown);
        } else if (fSys->isShiftInUse(ZeeDSystematics::jetEtMissScaleUp)){
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::JESUp);
        } else if (fSys->isShiftInUse(ZeeDSystematics::jetEtMissScaleDown)){
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::JESDown);
        } else if (fSys->isShiftInUse(ZeeDSystematics::jetEtMissResoUp)){
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::JERUp);
        } else if (fSys->isShiftInUse(ZeeDSystematics::jetEtMissResoDown)){
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::JERDown);
        } else if (fSys->isShiftInUse(ZeeDSystematics::EtMissClustResUp)){
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::CERUp);
        } else if (fSys->isShiftInUse(ZeeDSystematics::EtMissClustResDown)){
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::CERDown);
        } else if (fSys->isShiftInUse(ZeeDSystematics::EtMissClustScaleUp)){
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::CESUp);
        } else if (fSys->isShiftInUse(ZeeDSystematics::EtMissClustScaleDown)){
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::CESDown);
        } else if (fSys->isShiftInUse(ZeeDSystematics::EtMissScaleSoftTermUp)){
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::ScaleSoftTermsUp);
        } else if (fSys->isShiftInUse(ZeeDSystematics::EtMissScaleSoftTermDown)){
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::ScaleSoftTermsDown);
        } else if (fSys->isShiftInUse(ZeeDSystematics::EtMissResoSoftTermUp)){
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::ResoSoftTermsUp);
        } else if (fSys->isShiftInUse(ZeeDSystematics::EtMissResoSoftTermDown)){
            refFinal = fMETUtility->getMissingET(METUtil::RefFinal,METUtil::ResoSoftTermsDown);
        }
        else refFinal = fMETUtility -> getMissingET(METUtil::RefFinal);




        Double_t refFinal_et = refFinal.et()/GeV;
        Double_t refFinal_etx = refFinal.etx()/GeV;
        Double_t refFinal_ety = refFinal.ety()/GeV;
        Double_t refFinal_phi = refFinal.phi()/radian;
        //etMiss -> SetMET_Truth (etMiss->GetEt());
        etMiss -> SetEt(refFinal_et);
        etMiss -> SetEtX(refFinal_etx);
        etMiss -> SetEtY(refFinal_ety);
        etMiss -> SetPhi(refFinal_phi);
        /*    etMiss -> SetMET_ElecET(fMETUtility->getMissingET(METUtil::RefEle).et()/GeV);
              etMiss -> SetMET_MuonET(fMETUtility->getMissingET(METUtil::MuonTotal).et()/GeV);  
              etMiss -> SetMET_PhotonET(fMETUtility->getMissingET(METUtil::RefGamma).et()/GeV);  
              etMiss -> SetMET_TauET(fMETUtility->getMissingET(METUtil::RefTau).et()/GeV);  
              etMiss -> SetMET_SoftTermETSUM(fMETUtility->getMissingET(METUtil::SoftTerms).et()/GeV);  
              etMiss -> SetMET_JetET(fMETUtility->getMissingET(METUtil::RefJet).et()/GeV);  
              */
    } else if ((*fAnaOptions)->HadrRecoilFull()) {

        Int_t nObjMET(etMiss->GetMETCompositionElectron_index().size());

        TLorentzVector lepPt;
        int i_elec = 0;
        const TObjArray* electrons = event->GetElectrons();
        double sumElecPx=0, sumElecPy=0, sumElecPt=0;
        for ( Int_t iMET = 0; iMET < nObjMET; iMET++ )
        {
            Int_t zeedIndex(-1);
            const ZeeDElectron* electron(NULL);
            for ( Int_t iZeeD = 0; iZeeD < electrons->GetEntries(); iZeeD++)
            {
                electron = dynamic_cast<const ZeeDElectron*>( electrons->At( iZeeD ) ); 
                Int_t test1( electron->GetCollectionIndex() );

                Int_t test2( etMiss->GetMETCompositionElectron_index().at(iMET) );
                if (  test2 == test1 ) 
                {
                    zeedIndex = iMET;
                    break;
                }
            }

            if ( zeedIndex != -1 )
            {
                lepPt+=electron->GetFourVector();
                i_elec+=1;
            }
        }
        etMiss->SetnMETElec(i_elec);

        nObjMET = (etMiss->GetMETCompositionMuon_index().size());
        const TObjArray* Muons = event->GetMuons();
        int i_muon = 0;
        for ( Int_t iMET = 0; iMET < nObjMET; iMET++ )
        {
            Int_t zeedIndex(-1);
            const ZeeDMuon* Muon(NULL);
            for ( Int_t iZeeD = 0; iZeeD < Muons->GetEntries(); iZeeD++)
            {
                Muon = dynamic_cast<const ZeeDMuon*>( Muons->At( iZeeD ) ); 
                if (test )   std::cout <<"Muon = " << Muon << std::endl<<std::flush;
                Int_t test1( Muon->GetCollectionIndex());
                if (test )  std::cout <<"Muon test1 = " << test1 << std::endl<<std::flush;

                Int_t test2( etMiss->GetMETCompositionMuon_index().at(iMET) );
                if (test )  std::cout <<"Muon test2 = " << test2 << std::endl<<std::flush;
                if (  test2 == test1 ) 
                {
                    zeedIndex = iMET;
                    break;
                }
            }
            if ( zeedIndex != -1 )
            {
                lepPt+=Muon->GetFourVector();
                i_muon+=1;
            }
            etMiss->SetnMETMuon(i_muon);
        }

        TLorentzVector newLv;
        newLv.SetPxPyPzE(etMiss->GetCorRecoilEtX(), etMiss->GetCorRecoilEtY(), 0, etMiss->GetCorRecoilEt());
        newLv+=lepPt;

        double Et2 =newLv.Px()*newLv.Px()+newLv.Py()*newLv.Py();
        Et2=TMath::Sqrt(Et2);
        newLv.SetPxPyPzE(-newLv.Px(),-newLv.Py(), 0, Et2);
        etMiss->SetEt(Et2);
        etMiss->SetEtX(newLv.Px());
        etMiss->SetEtY(newLv.Py());
        etMiss->SetPhi(newLv.Phi());

        //Calculating corrections for hadrRecoil

    } else { //leaving in Hadron Recoil just part with lepton in W
        
    }
    //------------------------------------------------------

    //etMissArray->AddLast( etMissRec );

    /*    if (test) 
          {
          std::cout <<"********************************"<<std::endl;
          std::cout <<__PRETTY_FUNCTION__
          << " EtMiss Et = " << neutrinoPt << " GeV "
          << " Phi = "<< neutrinoPhi
          <<std::endl;
          std::cout <<__PRETTY_FUNCTION__
          << " EtMiss New Et = " << refFinal_CST.et()/GeV << " GeV "
          << " Phi = "<< refFinal_CST.phi()
          << " Nconst = "<< refFinal_CST.Nconst()
          << " \t ratioNew = " << neutrinoPt/(refFinal_CST.et()/GeV)
          <<std::endl;
          std::cout <<"********************************"<<std::endl;
          }
          if (test )
          {
          std::cout <<std::endl << std::endl << std::flush;
          Info(__PRETTY_FUNCTION__,"stop"); 
          std::cout <<std::endl << std::endl << std::flush;
          }
          */
    return;
}

//------------------------------------------------------

