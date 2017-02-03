
#include "ZeeDCalculator/ZeeDCalcElec.h"

// Root libraries
#include <TSystem.h>
#include <TMath.h>
#include <TObjArray.h>
#include <TFile.h>
#include <TH1D.h>
#include <TROOT.h>

// Athena libraries
#include "GaudiKernel/ServiceHandle.h"
#include "StoreGate/StoreGateSvc.h"

// ZeeD analysis includes
#include "ZeeDTools/ZeeDSystematics.h"
#include "ZeeDTools/ZeeDCalibrator.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDVertex.h"
#include "ZeeDEvent/ZeeDGenParticle.h"
#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"
#include "egammaAnalysisUtils/fwdISEMDefs.h"
#include "egammaAnalysisUtils/CaloIsoCorrection.h"
#include "egammaAnalysisUtils/IsEMForwardDefs.h"
#include "egammaEvent/egammaPIDdefs.h"
#include "CaloEvent/CaloClusterContainer.h"
#include "Particle/TrackParticleContainer.h"
#include "Particle/TrackParticle.h"
#include "ParticleTruth/TrackParticleTruthCollection.h"

#include "McParticleEvent/TruthParticleContainer.h"


#include "ZeeDTools/ZeeDBags.h"

#include "egammaAnalysisUtils/ElectronMCChargeCorrector.h"

#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDCalcElec::ZeeDCalcElec() :
    fEventInProcess(NULL),
    fCalibrator(NULL),
    fSvcHelper("ZeeDCalcElec"),
    fAnaOptions(fSvcHelper.GetAnaSvc()),
    fEiso((*fAnaOptions)->IsMC())
{
    // Get access to the systematics
    fSys = ZeeDSystematics::getInstance();

    // Initialize private Calibrator if needed
    if ((*fAnaOptions)->FillCalibrator() || (*fAnaOptions)->ApplyPrivateElecEnergySmearingCorr() ||
	(*fAnaOptions)->ApplyPrivateElecEnergyScaleCorr() || (*fAnaOptions)->SelectCalibMethod() != 0) {
      fCalibrator = new ZeeDCalibrator(ZeeDMisc::FindFile((*fAnaOptions)->CalibrationInFile()).Data(),
	  ZeeDMisc::FindFile((*fAnaOptions)->SmearingFile()).Data() );
    }

}

//------------------------------------------------------
ZeeDCalcElec::~ZeeDCalcElec()
{
    // Destructor

    if (fCalibrator) delete fCalibrator;

}

//------------------------------------------------------
void ZeeDCalcElec::Calculate(ZeeDEvent* event)
{
    // Calculates electrons for given event

    fEventInProcess = event;

    // Initialize energy rescaler tool with the first event.
    // Do not mix different periods (data11/data12) in one job.
    static bool isFirstEvent = true;
    if(isFirstEvent) {
      InitializeEnergyRescaler(event->RunNumber().Get());
      isFirstEvent = false;
    }

    const TObjArray* elecArray = event->GetElectrons();

    fEnerRescaler.SetRandomSeed( event->EventNumber().Get() );

    // Loop over electrons
    for ( Int_t i = 0; i < elecArray->GetEntriesFast(); ++i ) {

        // Get electron
        ZeeDElectron* electron = static_cast< ZeeDElectron* > (elecArray->At(i));
        this->CalcFourVector(electron, event->RunNumber().Get());

        // Calculate corrected Electron ID for the forward electrons
        if(!(electron->IsCentral())) {
            this->CalcFwdISEM(electron, event);
        }
	
        // Calculate Electron Isolation
        this->CalcEIso(electron, event);

        this->CalcCaloIsoCorrections(electron, event);

        this->CalcIDCut(electron);

        // Implement very simple DeltaR matching for status code 3 electrons.

        const TLorentzVector& recVector = electron->GetFourVector();

        // Get the pair of the electrons
        const ZeeDGenElectrons elecs = event->GetGenElectrons(ZeeDEnum::MCFSRLevel::Born);

        if (elecs.first == 0 || elecs.second == 0 ) {
            electron->fGenBornMatchedLepton= NULL;
        } else {

            const TLorentzVector& genVector1 = elecs.first->GetMCFourVector();
            const TLorentzVector& genVector2 = elecs.second->GetMCFourVector();

            const Double_t DR1 = recVector.DeltaR(genVector1);
            const Double_t DR2 = recVector.DeltaR(genVector2);

            electron->fGenBornMatchedLepton= (DR1 < DR2) ? elecs.first : elecs.second ;
        }

	// XXX: return the charge correction back with the new correction tools
	/*
	   if( (*fAnaOptions)->ApplyChargeCorrection() && !fSys->isShiftInUse(ZeeDSystematics::eChMIDOff) ){
	   this->CorrectCharge(event, electron);
	   }
	 */

    }

}

//------------------------------------------------------
void ZeeDCalcElec::InitializeEnergyRescaler(int runNumber)
{
  
  ZeeDEnum::Period::Value period = ZeeDMisc::GetPeriod(runNumber);

  if (period == ZeeDEnum::Period::data11_7TeV_A ||
      period == ZeeDEnum::Period::data11_7TeV_B ||
      period == ZeeDEnum::Period::data11_7TeV_D ||
      period == ZeeDEnum::Period::data11_7TeV_E ||
      period == ZeeDEnum::Period::data11_7TeV_F ||
      period == ZeeDEnum::Period::data11_7TeV_G ||
      period == ZeeDEnum::Period::data11_7TeV_H ||
      period == ZeeDEnum::Period::data11_7TeV_I ||
      period == ZeeDEnum::Period::data11_7TeV_J ||
      period == ZeeDEnum::Period::data11_7TeV_K ||
      period == ZeeDEnum::Period::data11_7TeV_L ||
      period == ZeeDEnum::Period::data11_7TeV_M ||
      period == ZeeDEnum::Period::data11_7TeV) {

    Info("ZeeDCalcElec::InitializeEnergyRescaler", "ZeeDCalcElec::ZeeDCalcElec : Set electron energy calibration constants to 2011 year.");
    fEnerRescaler.Init((ZeeDMisc::FindFile("EnergyRescalerData.root")).Data(),"2011","es2011a");

  } else if (period == ZeeDEnum::Period::data12_8TeV_A ||
      period == ZeeDEnum::Period::data12_8TeV_B || 
      period == ZeeDEnum::Period::data12_8TeV_C || 
      period == ZeeDEnum::Period::data12_8TeV_D || 
      period == ZeeDEnum::Period::data12_8TeV_E || 
      period == ZeeDEnum::Period::data12_8TeV_G || 
      period == ZeeDEnum::Period::data12_8TeV_H || 
      period == ZeeDEnum::Period::data12_8TeV_I || 
      period == ZeeDEnum::Period::data12_8TeV_J || 
      period == ZeeDEnum::Period::data12_8TeV_L || 
      period == ZeeDEnum::Period::data12_8TeV   ||
      period == ZeeDEnum::Period::data13_2p76TeV ) {

    Info("ZeeDCalcElec::InitializeEnergyRescaler", "ZeeDCalcElec::ZeeDCalcElec : Set electron energy calibration constants to 2012 year.");
    fEnerRescaler.Init((ZeeDMisc::FindFile("EnergyRescalerData.root")).Data(),"2012","es2012"); // see details on twiki twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/EnergyScaleResolutionRecommendations

  } else {

    Info("ZeeDCalcElec::InitializeEnergyRescaler", "ZeeDCalcElec::ZeeDCalcElec : can't recognize data period from the first event run number. Set electron energy calibration constants to 2011 year.");
    fEnerRescaler.Init((ZeeDMisc::FindFile("EnergyRescalerData.root")).Data(),"2011","es2011a");

  }

  // new EM calibration
  energy_correction_tool.setESModel(egEnergyCorr::es2012c);
  energy_correction_tool.initialize();

}

//------------------------------------------------------
void ZeeDCalcElec::CalcFwdISEM(ZeeDElectron* electron, ZeeDEvent* event)
{

    ZeeDLeptonBags::IsEM IsEMBag = electron->IsEM().Get();

    if ( electron->Cluster().IsSet() &&  event->Nvtx().IsSet()) {
        IsEMBag.isNewFwdLoose = isForward_Loose(
                event->Nvtx().Get(),
                electron->Cluster().Get().fourVector.Eta(),
                electron->Cluster().Get().SecondLambda,
                electron->Cluster().Get().lateral,
                electron->Cluster().Get().longitudinal,
                electron->Cluster().Get().FracMax,
                electron->Cluster().Get().SecondR,
                electron->Cluster().Get().CentralLambda,
                egammaForwardMenu::eg2012);

        IsEMBag.isNewFwdMedium = isForward_Medium(
                event->Nvtx().Get(),
                electron->Cluster().Get().fourVector.Eta(),
                electron->Cluster().Get().SecondLambda,
                electron->Cluster().Get().lateral,
                electron->Cluster().Get().longitudinal,
                electron->Cluster().Get().FracMax,
                electron->Cluster().Get().SecondR,
                electron->Cluster().Get().CentralLambda,
		egammaForwardMenu::eg2012);

        IsEMBag.isNewFwdTight = isForward_Tight(
                event->Nvtx().Get(),
                electron->Cluster().Get().fourVector.Eta(),
                electron->Cluster().Get().SecondLambda,
                electron->Cluster().Get().lateral,
                electron->Cluster().Get().longitudinal,
                electron->Cluster().Get().FracMax,
                electron->Cluster().Get().SecondR,
                electron->Cluster().Get().CentralLambda,
		egammaForwardMenu::eg2012);

    } else {
        IsEMBag.isNewFwdLoose = false;
        IsEMBag.isNewFwdMedium = false;
        IsEMBag.isNewFwdTight = false;
    }

    electron->IsEM().UnLock();
    electron->IsEM().Set(IsEMBag);
    electron->IsEM().Lock();

}

//------------------------------------------------------
void ZeeDCalcElec::CalcCaloIsoCorrections(ZeeDElectron* electron, ZeeDEvent* event)
{

    if(!electron->Shower().IsSet()) return;

    ZeeDElectronBags::CaloIsoCor caloIsoCorBag;

    const TObjArray* vertexArray = event->GetVertices();

    int Nvtx = 0;

    for ( Int_t index = 0; index < vertexArray->GetEntriesFast(); ++index ) {
        ZeeDVertex* vertex = static_cast< ZeeDVertex* >( vertexArray->At(index) );

        if ( vertex->GetNTrack() >= 2 ) {
            ++Nvtx;
        }
    }

    double etcone20   = electron->Shower().Get().etcone20;
    double etcone30   = electron->Shower().Get().etcone30;
    double e_cl       = electron->Cluster().Get().fourVector.E()*1000;
    double eta_cl     = electron->Cluster().Get().fourVector.Eta();
    double eta_tr     = electron->TrackParticle().Get().fourVector.Eta();
    double etas2      = electron->Cluster().Get().etaBE2;

    double etcone20_PtNPVCorrected = CaloIsoCorrection::GetPtNPVCorrectedIsolation(Nvtx, e_cl, etas2, eta_tr, eta_cl, 0.2, (*fAnaOptions)->IsMC(), etcone20);
    double etcone30_PtNPVCorrected = CaloIsoCorrection::GetPtNPVCorrectedIsolation(Nvtx, e_cl, etas2, eta_tr, eta_cl, 0.3, (*fAnaOptions)->IsMC(), etcone30);

    caloIsoCorBag.etcone20_PtNPVCorrected = etcone20_PtNPVCorrected;
    caloIsoCorBag.etcone30_PtNPVCorrected = etcone30_PtNPVCorrected;

    electron->CaloIsoCor().Set(caloIsoCorBag);
    electron->CaloIsoCor().Lock();

}

//------------------------------------------------------
void ZeeDCalcElec::CalcEIso(ZeeDElectron* electron, ZeeDEvent* event)
{

    ZeeDElectronBags::EIso eIsoBag;
    
    if( electron->TrackParticle().IsSet() == kFALSE || 
        electron->Cluster().IsSet() == kFALSE ||  
	    electron->Shower().IsSet() == kFALSE ) {

	    electron->EIso().UnLock();
        electron->EIso().Set(eIsoBag);
        electron->EIso().Lock();
	    return;
    }
    
    const TObjArray* vertexArray = event->GetVertices();
    CHECK_NULL_PTR(vertexArray);
    
    int Nvtx = 0;
    
    for ( Int_t index = 0; index < vertexArray->GetEntriesFast(); ++index ) {
        ZeeDVertex* vertex = static_cast< ZeeDVertex* >( vertexArray->At(index) );

        if ( vertex->GetNTrack() >= 2 ) {
            ++Nvtx;
        }
    }

    double e_cl       = electron->Cluster().Get().fourVector.E()*1000;
    double eta_cl     = electron->Cluster().Get().fourVector.Eta();
    double eta_tr     = electron->TrackParticle().Get().fourVector.Eta();
    double etap       = electron->Shower().Get().etap;
    double etas2      = electron->Cluster().Get().etaBE2;

    eIsoBag.Iso95Etcone20 = fEiso.Eiso95Etcone20(Nvtx, e_cl, eta_tr, eta_cl, etas2, etap, electron->Shower().Get().etcone20 );
    eIsoBag.Iso98Etcone20 = fEiso.Eiso98Etcone20(Nvtx, e_cl, eta_tr, eta_cl, etas2, etap, electron->Shower().Get().etcone20 );

    eIsoBag.Iso95Etcone30 = fEiso.Eiso95Etcone30(Nvtx, e_cl, eta_tr, eta_cl, etas2, etap, electron->Shower().Get().etcone30 );
    eIsoBag.Iso98Etcone30 = fEiso.Eiso98Etcone30(Nvtx, e_cl, eta_tr, eta_cl, etas2, etap, electron->Shower().Get().etcone30 );

    eIsoBag.Iso97Ptcone20 = fEiso.Eiso97Ptcone20(e_cl, eta_tr, eta_cl, electron->Shower().Get().ptcone20 );
    eIsoBag.Iso98Ptcone20 = fEiso.Eiso98Ptcone20(e_cl, eta_tr, eta_cl, electron->Shower().Get().ptcone20 );
    eIsoBag.Iso99Ptcone20 = fEiso.Eiso99Ptcone20(e_cl, eta_tr, eta_cl, electron->Shower().Get().ptcone20 );

    eIsoBag.Iso97Ptcone30 = fEiso.Eiso97Ptcone30(e_cl, eta_tr, eta_cl, electron->Shower().Get().ptcone30 );
    eIsoBag.Iso98Ptcone30 = fEiso.Eiso98Ptcone30(e_cl, eta_tr, eta_cl, electron->Shower().Get().ptcone30 );
    eIsoBag.Iso99Ptcone30 = fEiso.Eiso99Ptcone30(e_cl, eta_tr, eta_cl, electron->Shower().Get().ptcone30 );

    eIsoBag.Iso97Ptcone40 = fEiso.Eiso97Ptcone40(e_cl, eta_tr, eta_cl, electron->Shower().Get().ptcone40 );
    eIsoBag.Iso98Ptcone40 = fEiso.Eiso98Ptcone40(e_cl, eta_tr, eta_cl, electron->Shower().Get().ptcone40 );
    eIsoBag.Iso99Ptcone40 = fEiso.Eiso99Ptcone40(e_cl, eta_tr, eta_cl, electron->Shower().Get().ptcone40 );

    electron->EIso().UnLock();
    electron->EIso().Set(eIsoBag);
    electron->EIso().Lock();

}

//------------------------------------------------------
void ZeeDCalcElec::CalcFourVector(ZeeDElectron* electron, int runNumber)
{
    // Calculate electron four vector

    if (electron->TrackParticle().IsSet() == kFALSE &&
            electron->Cluster().IsSet() == kFALSE) {

        Error("ZeeDCalcElec::CalcElecFourVec",
              "Electron does not have cluster nor track!");
	gSystem->Exit(EXIT_FAILURE);
    }

	 // Calibrate cluster if electron energy scale or smearing is specified
    if (((*fAnaOptions)->ApplyElecEnergyScaleCorr() || (*fAnaOptions)->ApplyElecEnergySmearingCorr()) && !(fSys->isShiftInUse(ZeeDSystematics::eElecEnOff))) {
      this->calibrateCluster(electron, runNumber);
    }

    // Create electron four vector using cluster and track information
    Double_t electronE     = 0.0;
    Double_t electronP     = 0.0;
    Double_t electronTheta = 0.0;
    Double_t electronPhi   = 0.0;

    if (electron->TrackParticle().IsSet() == kTRUE &&
           (electron->TrackSummary().Get().numberOfPixelHits + electron->TrackSummary().Get().numberOfSCTHits) > 3 &&
            electron->Cluster().IsSet() == kTRUE ) {
        // Both good track and cluster exist

        const TLorentzVector electronTrack = electron->TrackParticle().Get().fourVector;
        // Take energy and momentum from cluster
	ZeeDElectronBags::Cluster clusterBag 	= electron->Cluster().Get();
	TLorentzVector 		electronCluster	= clusterBag.fourVector;
        electronE = electronCluster.E();
        electronP = electronCluster.P();

        // Take angular position from track
        electronTheta = electronTrack.Theta();
        electronPhi   = electronTrack.Phi();

    } else if ( electron->Cluster().IsSet() == kTRUE ) {
        // Only cluster exists

        // Take energy, momentum and angular position from cluster
	ZeeDElectronBags::Cluster clusterBag 	= electron->Cluster().Get();
	TLorentzVector 		electronCluster	= clusterBag.fourVector;
        electronE     = electronCluster.E();
        electronP     = electronCluster.P();
        electronTheta = electronCluster.Theta();
        electronPhi   = electronCluster.Phi();

    } else if (electron->TrackParticle().IsSet() == kTRUE) {
        // Only track exists - soft and forward electrons

        const TLorentzVector electronTrack = electron->TrackParticle().Get().fourVector;

        // Take energy, momentum and angular position from track
        electronE     = electronTrack.E();
        electronP     = electronTrack.P();
        electronTheta = electronTrack.Theta();
        electronPhi   = electronTrack.Phi();
    }

    Double_t electronPx = electronP * TMath::Sin(electronTheta) * TMath::Cos(electronPhi);
    Double_t electronPy = electronP * TMath::Sin(electronTheta) * TMath::Sin(electronPhi);
    Double_t electronPz = electronP * TMath::Cos(electronTheta);

    TLorentzVector electronFourVector(electronPx, electronPy, electronPz, electronE);
    electron->setFourVector(electronFourVector);

}

//------------------------------------------------------
void ZeeDCalcElec::calibrateCluster(ZeeDElectron* electron, int runNumber)
{
  // Apply electron cluster calibration
  // Be careful: official and private scale corrections are not mutually exclusive.

  // do not calibrate em cluster without a cluster
  if (!(electron->Cluster().IsSet())) return;

  // get the cluster
  ZeeDElectronBags::Cluster clusterBag = electron->Cluster().Get();

  // initialize values to be corrected
  TLorentzVector clusterFourVector = clusterBag.fourVector;
  double old_cluster_E = clusterFourVector.E() * 1000.;
  double new_cluster_E = old_cluster_E;
  double smearing_factor = 1.; 

  // use old or new em calibration?
  if ((*fAnaOptions)->UseNNEMCalibration() && electron->TrackParticle().IsSet()) {

    // The "new style NN em calibration" is choosen which means, that
    // 1. this is data or at least mc12c
    // 2. em candidate has a track

    // get all the variables are needed for the new calibration
    PATCore::ParticleDataType::DataType dataType = ((*fAnaOptions)->IsMC())? PATCore::ParticleDataType::Full : PATCore::ParticleDataType::Data;
    float rawcl_Es0	= clusterBag.rawcl_Es0 * 1000.;
    float rawcl_Es1	= clusterBag.rawcl_Es1 * 1000.;
    float rawcl_Es2	= clusterBag.rawcl_Es2 * 1000.;
    float rawcl_Es3	= clusterBag.rawcl_Es3 * 1000.;
    float cl_eta	= clusterFourVector.Eta();
    float cl_phi	= clusterFourVector.Phi();
    float trk_eta	= electron->TrackParticle().Get().fourVector.Eta();
    float cl_E		= clusterFourVector.E() * 1000.;
    float cl_etaCalo	= clusterBag.cl_etaCalo;
    float cl_phiCalo	= clusterBag.cl_phiCalo;

	 egEnergyCorr::Scale::Variation scaleVariation;
	 egEnergyCorr::Resolution::Variation resolutionVariation;
	 if ((*fAnaOptions)->IsMC()) {
	 	scaleVariation = this->getGEO21EnergyScaleVariation();
	 	resolutionVariation = this->getGEO21EnergyResolutionVariation();
	 }
	 else {
	 	scaleVariation = egEnergyCorr::Scale::Nominal;
	 	resolutionVariation = egEnergyCorr::Resolution::None;
	 }
	 // std::cout << energy_correction_tool.variationName(scaleVariation) << "  " << energy_correction_tool.variationName(resolutionVariation) << std::endl;

    egEnergyCorr::Resolution::resolutionType resolutionType = egEnergyCorr::Resolution::SigmaEff90; // default
    double varSF = 1.0; // default

    // get the new cluster energy
    if (rawcl_Es0 > 0. || rawcl_Es1 > 0. || rawcl_Es2 > 0. || rawcl_Es3 > 0.) {	// XXX: does it really needed?
      new_cluster_E = energy_correction_tool.getCorrectedEnergyElectron(
	  runNumber,
	  dataType,
	  rawcl_Es0, rawcl_Es1, rawcl_Es2, rawcl_Es3,
	  cl_eta, cl_phi,
	  trk_eta,
	  cl_E, cl_etaCalo, cl_phiCalo,
	  scaleVariation, resolutionVariation, resolutionType, varSF);
    }


  } else {

    // The "old std style em calibration" is choosen which means, that
    // 1. this is data or mc (mc bkg probably)
    // 2. OR em candidate has no track

    const double cl_Eta	= clusterFourVector.Eta();
    const double cl_E	= clusterFourVector.E()*1000.;      // GeV->MeV


    if ((*fAnaOptions)->IsMC()) {

      // calculate corrected energy
      double da = fEnerRescaler.getAlphaValue(cl_Eta, cl_E, egRescaler::EnergyRescalerUpgrade::Electron, this->getGEO20EnergyScaleVariation());
      new_cluster_E *= (1 + da);

      // calculate the resolution smearing
      if ((*fAnaOptions)->ApplyElecEnergySmearingCorr() && electron->IsCentral()) {

	smearing_factor = fEnerRescaler.getSmearingCorrection(cl_Eta, cl_E, this->getGEO20EnergyResolutionVariation());
      }

    }

    /*
    // FIXME: restore the private calibration block here
    // Apply private calibration factor for the electron cluster, if calibration method != 0
    if ( ((*fAnaOptions)->SelectCalibMethod() != 0 || (*fAnaOptions)->ApplyPrivateElecEnergyScaleCorr())
    &&  (*fAnaOptions)->IsData()  ) {
    electronCluster *= fCalibrator->CorrectionFactor( &electronCluster );
    }
    // Apply private electron energy smearing
    if ((*fAnaOptions)->ApplyPrivateElecEnergySmearingCorr()) electronCluster *= fCalibrator->SmearFactor(&electronCluster);
     */

  }

  // correct cluster four-vector 
  if (new_cluster_E > 0.) clusterFourVector *= (new_cluster_E / old_cluster_E) * smearing_factor;
 // std::cout << new_cluster_E << " " << old_cluster_E << " " << smearing_factor<< std::endl; 
  // set corrected cluster four-vector for the electron
  clusterBag.fourVector = clusterFourVector;
  electron->Cluster().UnLock();
  electron->Cluster().Set(clusterBag);
  electron->Cluster().Lock();

}

egEnergyCorr::Scale::Variation ZeeDCalcElec::getGEO21EnergyScaleVariation()
{
  switch (fSys->getCurrentShift()) {

    case ZeeDSystematics::eNoShift 		: return egEnergyCorr::Scale::None;
    case ZeeDSystematics::eElecEnZeeStatUp 	: return egEnergyCorr::Scale::ZeeStatUp;
    case ZeeDSystematics::eElecEnZeeStatDown 	: return egEnergyCorr::Scale::ZeeStatDown;
    case ZeeDSystematics::eElecEnZeeSystUp 	: return egEnergyCorr::Scale::ZeeSystUp;
    case ZeeDSystematics::eElecEnZeeSystDown 	: return egEnergyCorr::Scale::ZeeSystDown;
    case ZeeDSystematics::eElecEnPSUp 		: return egEnergyCorr::Scale::PSUp;
    case ZeeDSystematics::eElecEnPSDown 	: return egEnergyCorr::Scale::PSDown;
    case ZeeDSystematics::eElecEnS12Up 		: return egEnergyCorr::Scale::S12Up;
    case ZeeDSystematics::eElecEnS12Down 	: return egEnergyCorr::Scale::S12Down;
    case ZeeDSystematics::eElecEnMatIDUp 	: return egEnergyCorr::Scale::MatIDUp;
    case ZeeDSystematics::eElecEnMatIDDown 	: return egEnergyCorr::Scale::MatIDDown;
    case ZeeDSystematics::eElecEnMatCryoUp 	: return egEnergyCorr::Scale::MatCryoUp;
    case ZeeDSystematics::eElecEnMatCryoDown 	: return egEnergyCorr::Scale::MatCryoDown;
    case ZeeDSystematics::eElecEnMatCaloUp 	: return egEnergyCorr::Scale::MatCaloUp;
    case ZeeDSystematics::eElecEnMatCaloDown 	: return egEnergyCorr::Scale::MatCaloDown;
    case ZeeDSystematics::eElecEnLArCalibUp 	: return egEnergyCorr::Scale::LArCalibUp;
    case ZeeDSystematics::eElecEnLArCalibDown 	: return egEnergyCorr::Scale::LArCalibDown;
    case ZeeDSystematics::eElecEnLArUnconvCalibUp 	: return egEnergyCorr::Scale::LArUnconvCalibUp;
    case ZeeDSystematics::eElecEnLArUnconvCalibDown 	: return egEnergyCorr::Scale::LArUnconvCalibDown;
	 case ZeeDSystematics::eElecEnLArElecUnconvUp 	: return egEnergyCorr::Scale::LArElecUnconvUp;
	 case ZeeDSystematics::eElecEnLArElecUnconvDown 	: return egEnergyCorr::Scale::LArElecUnconvDown;
	 case ZeeDSystematics::eElecEnLArElecCalibUp     : return egEnergyCorr::Scale::LArElecCalibUp;
	 case ZeeDSystematics::eElecEnLArElecCalibDown   : return egEnergyCorr::Scale::LArElecCalibDown;
	 case ZeeDSystematics::eElecEnL1GainUp 	: return egEnergyCorr::Scale::L1GainUp;
	 case ZeeDSystematics::eElecEnL1GainDown 	: return egEnergyCorr::Scale::L1GainDown;
	 case ZeeDSystematics::eElecEnL2GainUp 	: return egEnergyCorr::Scale::L2GainUp;
	 case ZeeDSystematics::eElecEnL2GainDown 	: return egEnergyCorr::Scale::L2GainDown;
	 case ZeeDSystematics::eElecEnG4Up 		: return egEnergyCorr::Scale::G4Up;
	 case ZeeDSystematics::eElecEnG4Down 	: return egEnergyCorr::Scale::G4Down;
	 case ZeeDSystematics::eElecPedestalUp      	: return egEnergyCorr::Scale::PedestalUp;
	 case ZeeDSystematics::eElecPedestalDown 	: return egEnergyCorr::Scale::PedestalDown;

    default: return egEnergyCorr::Scale::None;
  }
}

egEnergyCorr::Resolution::Variation ZeeDCalcElec::getGEO21EnergyResolutionVariation()
{
  switch (fSys->getCurrentShift()) {

    case ZeeDSystematics::eNoShift 			: return egEnergyCorr::Resolution::Nominal;
    case ZeeDSystematics::eElecEnZSmearingUp		: return egEnergyCorr::Resolution::ZSmearingUp;
    case ZeeDSystematics::eElecEnZSmearingDown		: return egEnergyCorr::Resolution::ZSmearingDown;
    case ZeeDSystematics::eElecEnSamplingTermUp		: return egEnergyCorr::Resolution::SamplingTermUp;
    case ZeeDSystematics::eElecEnSamplingTermDown	: return egEnergyCorr::Resolution::SamplingTermDown;
    case ZeeDSystematics::eElecEnMaterialIDUp		: return egEnergyCorr::Resolution::MaterialIDUp;
    case ZeeDSystematics::eElecEnMaterialIDDown		: return egEnergyCorr::Resolution::MaterialIDDown;
    case ZeeDSystematics::eElecEnMaterialCaloUp		: return egEnergyCorr::Resolution::MaterialCaloUp;
    case ZeeDSystematics::eElecEnMaterialCaloDown	: return egEnergyCorr::Resolution::MaterialCaloDown;
    case ZeeDSystematics::eElecEnMaterialGapUp		: return egEnergyCorr::Resolution::MaterialGapUp;
    case ZeeDSystematics::eElecEnMaterialGapDown 	: return egEnergyCorr::Resolution::MaterialGapDown;
    case ZeeDSystematics::eElecEnMaterialCryoUp 	: return egEnergyCorr::Resolution::MaterialCryoUp;
    case ZeeDSystematics::eElecEnMaterialCryoDown 	: return egEnergyCorr::Resolution::MaterialCryoDown;
    case ZeeDSystematics::eElecEnPileUpUp		: return egEnergyCorr::Resolution::PileUpUp;
    case ZeeDSystematics::eElecEnPileUpDown 		: return egEnergyCorr::Resolution::PileUpDown;

    default: return egEnergyCorr::Resolution::Nominal;
  }
}

egRescaler::EnergyRescalerUpgrade::Variation ZeeDCalcElec::getGEO20EnergyScaleVariation()
{
  switch (fSys->getCurrentShift()) {

    case ZeeDSystematics::eElecEnZeeStatUp 	: return egRescaler::EnergyRescalerUpgrade::ZeeStatUp;
    case ZeeDSystematics::eElecEnZeeStatDown 	: return egRescaler::EnergyRescalerUpgrade::ZeeStatDown;
    case ZeeDSystematics::eElecEnZeeMethodUp 	: return egRescaler::EnergyRescalerUpgrade::ZeeMethodUp;
    case ZeeDSystematics::eElecEnZeeMethodDown 	: return egRescaler::EnergyRescalerUpgrade::ZeeMethodDown;
    case ZeeDSystematics::eElecEnZeeGenUp 	: return egRescaler::EnergyRescalerUpgrade::ZeeGenUp;
    case ZeeDSystematics::eElecEnZeeGenDown 	: return egRescaler::EnergyRescalerUpgrade::ZeeGenDown;
    case ZeeDSystematics::eElecEnZeeAllUp 	: return egRescaler::EnergyRescalerUpgrade::ZeeAllUp;
    case ZeeDSystematics::eElecEnZeeAllDown 	: return egRescaler::EnergyRescalerUpgrade::ZeeAllDown;
    case ZeeDSystematics::eElecEnR12StatUp 	: return egRescaler::EnergyRescalerUpgrade::R12StatUp;
    case ZeeDSystematics::eElecEnR12StatDown 	: return egRescaler::EnergyRescalerUpgrade::R12StatDown;
    case ZeeDSystematics::eElecEnPSStatUp 	: return egRescaler::EnergyRescalerUpgrade::PSStatUp;
    case ZeeDSystematics::eElecEnPSStatDown 	: return egRescaler::EnergyRescalerUpgrade::PSStatDown;
    case ZeeDSystematics::eElecEnLowPtUp 	: return egRescaler::EnergyRescalerUpgrade::LowPtUp;
    case ZeeDSystematics::eElecEnLowPtDown 	: return egRescaler::EnergyRescalerUpgrade::LowPtDown;

    default: return egRescaler::EnergyRescalerUpgrade::Nominal;
  }
}

egRescaler::EnergyRescalerUpgrade::CorrType ZeeDCalcElec::getGEO20EnergyResolutionVariation()
{
  switch (fSys->getCurrentShift()) {

    case ZeeDSystematics::eSmearUp	: return egRescaler::EnergyRescalerUpgrade::ERR_UP;
    case ZeeDSystematics::eSmearDown	: return egRescaler::EnergyRescalerUpgrade::ERR_DOWN;

    default: return egRescaler::EnergyRescalerUpgrade::NOMINAL;
  }
}

//------------------------------------------------------
void ZeeDCalcElec::CalcIDCut(ZeeDElectron* electron)
{

    ZeeDElectronBags::IDCut IDCutBag;

    UInt_t bits = electron->IsEM().Get().bits;
    UInt_t bits_Loose = electron->IsEM().Get().bits_Loose;

    IDCutBag.ClusterStripsWtot      = (bits &       (1 << egammaPID::ClusterStripsWtot_Electron))      == 0;
    IDCutBag.ClusterMiddleEratio37  = (bits &       (1 << egammaPID::ClusterMiddleEratio37_Electron))  == 0;
    IDCutBag.ClusterHadronicLeakage = (bits_Loose & (1 << egammaPID::ClusterHadronicLeakage_Electron)) == 0;
    IDCutBag.TrackTRTratio          = (bits &       (1 << egammaPID::TrackTRTratio_Electron))          == 0;
    IDCutBag.ConversionMatch	    = (bits &       (1 << egammaPID::ConversionMatch_Electron))        == 0;

    electron->IDCut().Set(IDCutBag);
    electron->IDCut().Lock();

}
/*
// XXX; return the corrector back with the new tool 

//------------------------------------------------------
void ZeeDCalcElec::CorrectCharge(ZeeDEvent* event, ZeeDElectron* electron) {

    if (electron->Cluster().IsSet() == kFALSE) {
        Error("ZeeDCalcElec::CorrectCharge", "Electron does not have cluster! ");
    }

    double elecID = 0;
    int charge_Truth = 0;

    if (!electron->MCTruth().IsSet()) {
        return;
    }
	const HepMC::GenParticle* part = electron->MCTruth().Get().trueLepton;
    if (part->pdg_id() == 11) {
        charge_Truth = -1;
    }
    if (part->pdg_id() == -11) {
        charge_Truth = 1;
    }

    const TLorentzVector& cluster = electron->Cluster().Get().fourVector;
    const Double_t eta_cluster = cluster.Eta();

    if (electron->getHasCharge()) {

        ElectronMCChargeCorrector *chMID = ;
        chMID->SetRndSeed( event->EventNumber().Get() );

        double charge = electron->getCharge();

        int charge_correct = chMID->GetCorrectedCharge(eta_cluster, charge, charge_Truth);

        if (fSys->isShiftInUse(ZeeDSystematics::eChMIDUp)) {
            charge_correct = chMID->GetCorrectedCharge(eta_cluster, charge, charge_Truth, ElectronMCChargeCorrector::chg_sysup);
        }
        if (fSys->isShiftInUse(ZeeDSystematics::eChMIDDown)) {
            charge_correct = chMID->GetCorrectedCharge(eta_cluster, charge, charge_Truth, ElectronMCChargeCorrector::chg_sysdown);
        }

        electron->Charge().UnLock();
        electron->Charge().Set(charge_correct);
        electron->Charge().Lock();

		electron->setCharge(charge_correct);
    }

}
*/
