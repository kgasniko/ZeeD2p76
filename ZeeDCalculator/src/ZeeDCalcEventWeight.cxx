#include "ZeeDCalculator/ZeeDCalcEventWeight.h"

// Root libraries
#include <TSystem.h>
#include <TFile.h>
#include <TH1D.h>
#include <TMath.h>
#include <TROOT.h>

// ZeeD analysis includes
#include "ZeeDEvent/ZeeDGenParticle.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDTools/ZeeDSystematics.h"
#include "ZeeDEvent/ZeeDVertex.h"

#include "ZeeDArrays/ZeeDBinGrid.h"
#include "ZeeDArrays/ZeeDArrayD.h"

#include "ZeeDExternal/ZeeDPDF.h"
#include "ZeeDExternal/LineShapeTool.h"

#include "ZeeDMisc/ZeeDMisc.h"

//------------------------------------------------------
ZeeDCalcEventWeight::ZeeDCalcEventWeight() :
    fEvent(NULL),
    fEventWeight(1.0),
    fPileupTool(NULL),
    fSvcHelper("ZeeDCalcEventWeight"),
    fAnaOptions(fSvcHelper.GetAnaSvc()),
    zvtxrwTool(NULL) {
        // Constructor
        // init PDF
        if ((*fAnaOptions)->ApplyPDFReweighting() && (*fAnaOptions)->IsMC()) {
            if (!ZeeDPDF::isFromInited()) {
                ZeeDPDF::setFromPDFSet(std::string(ZeeDMisc::FindFile("PDF/CT10.LHgrid").Data()));
            }
            if (!ZeeDPDF::isToInited()) {
                ZeeDPDF::setToPDFSet(std::string(ZeeDMisc::FindFile((*fAnaOptions)->PDFFileName()).Data()), (*fAnaOptions)->PDFSetNumber());
            }
        }

        fSys = ZeeDSystematics::getInstance();

        // init Nvtx weight histogramm
        if ((*fAnaOptions)->IsMC() && (*fAnaOptions)->ApplyNvtxReweighting()) {
            Info("ZeeDCalcEventWeight::ZeeDCalcEventWeight", "Open root file weight.root for NVtx reweighting");

            const TString WeightROOTfile = ZeeDMisc::FindFile("Vtx/weight.root");
            TFile* weightfile = new TFile(WeightROOTfile);

            if (weightfile == NULL) {
                Error("ZeeDCalcEventWeight::ZeeDCalcEventWeight", WeightROOTfile + " not found");
            }

            weighthist = (TH1D*)weightfile->Get("weights");

            if (weighthist == NULL) {
                Error("ZeeDCalcEventWeight::ZeeDCalcEventWeight", "Histogramme: -Nvtx- Not found");
            }

            weighthist->SetDirectory(gROOT);

            weightfile->Close();
            delete weightfile;
        }

        // init NTrkVtx weight histogramm
        if ((*fAnaOptions)->IsMC() && (*fAnaOptions)->ApplyNTrkVtxReweighting()) {
            Info("ZeeDCalcEventWeight::ZeeDCalcEventWeight", "Open root file NTrkVtx.root for NTrkVtx reweighting");

            const TString WeightROOTfile = ZeeDMisc::FindFile("Vtx/NTrkVtx.root");
            TFile* weightfile = new TFile(WeightROOTfile);

            if (weightfile == NULL) {
                Error("ZeeDCalcEventWeight::ZeeDCalcEventWeight", WeightROOTfile + " not found");
            }

            NTrkVtx = (TH1D*) weightfile->Get("NTrkVtx");

            if (NTrkVtx == NULL) {
                Error("ZeeDCalcEventWeight::ZeeDCalcEventWeight", "Histogramme: NTrkVtx Not found");
            }

            NTrkVtx->SetDirectory(gROOT);

            weightfile->Close();
            delete weightfile;
        }

        // init  Pileup reweighting
        if ( ( (*fAnaOptions)->IsMC() && ((*fAnaOptions)->ApplyPileupReweighting()
                        || (*fAnaOptions)->MCAssignRandomRunNumber()  ) ) || (*fAnaOptions)->GenPileupMCFile() )  {

            fPileupTool = new Root::TPileupReweighting("TPileupReweighting");

            if ((*fAnaOptions)->GenPileupMCFile()) {

                this->ConfigurePileupToolToGenerateMCFile();

            } else {

                this->ConfigurePileupToolForReweighting();
            }

            Int_t status = fPileupTool->initialize();

            if (status != 0) {
                Error("ZeeDCalcEventWeight", "Could not initialize PileupReweighting");
                gSystem->Exit(EXIT_FAILURE);
            } else Info("ZeeDCalcEventWeight", "Initialized PileupReweighting");
        }
        if ((*fAnaOptions)->IsMC() && (*fAnaOptions)->ApplyPtZReweighting() && (*fAnaOptions)->FillGenInfo()) {
            //fileSF = new TFile("/afs/desy.de/user/k/kgasniko/ZeeD/share/ptW.root");
            fileSF = new TFile("/afs/desy.de/user/k/kgasniko/ZeeD/share/SFPt.root");
        }
        //init reweighting of the Z position of primary vertex
        if ((*fAnaOptions)->IsMC() && (*fAnaOptions)->ApplyZPosVtxReweighting()) {

            std::string ZvtxRewFileName = (*fAnaOptions)->ZvtxRewFile();
            std::string MCtag	    = (*fAnaOptions)->MCG4SimTag();

            TString WeightROOTfile = ZeeDMisc::FindFile(ZvtxRewFileName);

            zvtxrwTool = new VertexPositionReweightingTool(MCtag, WeightROOTfile.Data());
            Info("ZeeDCalcEventWeight::ZeeDCalcEventWeight", "Initialize ZPosVtx reweighting for %s", MCtag.c_str());
        }

    }

//------------------------------------------------------
ZeeDCalcEventWeight::~ZeeDCalcEventWeight() {
    // Destructor
    Info("ZeeDCalcEventWeight::~ZeeDCalcEventWeight", "ZeeDCalcEventWeight d'tor called");
    if ((*fAnaOptions)->IsMC() && (*fAnaOptions)->ApplyPtZReweighting() && (*fAnaOptions)->FillGenInfo()
            && !(fSys->isShiftInUse(ZeeDSystematics::eZPtRewOff))) {
        fileSF->Close();
    }

    if ( //(*fAnaOptions)->IsMC() &&
            (*fAnaOptions)->GenPileupMCFile()) {

        Info("ZeeDCalcEventWeight::~ZeeDCalcEventWeight",
                "Writing MC distribution for PileupReweighting tool to file %s", (*fAnaOptions)->PileupMCGenFile().c_str());

        if (fPileupTool == NULL) {

            Error("ZeeDCalcEventWeight::~ZeeDCalcEventWeight", "Pointer to Pileup Tool (fPileupTool) is NULL, "
                    "but type is MC and GenPileupMCFile is true!");

            gSystem->Exit(EXIT_FAILURE); // Sebastian: I get a segfault despite this line.

        }

        fPileupTool->WriteToFile((*fAnaOptions)->PileupMCGenFile());

    }

}

//------------------------------------------------------
void ZeeDCalcEventWeight::Calculate(ZeeDEvent* event) {
    // Calculate event weights for given event

    fEvent = event;
    fGenWeight = event->GetRawEvent()->GetGenWeight();
    fEventWeight = 1.0;

    // configuration needs MCtype which is picked up from the first event
    static bool isFirst = true;
    if (isFirst) {
        /*        if ((*fAnaOptions)->IsMC() == kTRUE && (*fAnaOptions)->ApplyPtZReweighting()) {
                  ConfigureBosonPtReweightingTool();
                  }*/
        isFirst = false;
    }

    if ((*fAnaOptions)->IsMC() == kTRUE && (*fAnaOptions)->MCAssignRandomRunNumber() == kTRUE) { // keep this block on the top for the run-dependent corrections
        this->MCAssignRandomRunNumber();
    }

    if ((*fAnaOptions)->IsMC() && (*fAnaOptions)->ApplyPtZReweighting() && (*fAnaOptions)->FillGenInfo()
            && !(fSys->isShiftInUse(ZeeDSystematics::eZPtRewOff))) {
        //        this->ApplyOfficialPtWeight();
        this->ApplyUnofficialPtWeight();
    }

    if ((*fAnaOptions)->IsMC() && (*fAnaOptions)->ApplyPDFReweighting()) {
        this->ApplyPDFWeight();
    }

    if ((*fAnaOptions)->IsMC() && (*fAnaOptions)->ApplyNvtxReweighting()
            && !(fSys->isShiftInUse(ZeeDSystematics::eNvtxOff))) {
        this->ApplyMCVtxWeight();
    }

    if ((*fAnaOptions)->IsMC() && (*fAnaOptions)->ApplyZPosVtxReweighting()
            && !(fSys->isShiftInUse(ZeeDSystematics::eZvRewOff))) {
        this->ApplyZvWeight();
    }

    if ((*fAnaOptions)->IsMC() && (*fAnaOptions)->ApplyNTrkVtxReweighting()
            && !(fSys->isShiftInUse(ZeeDSystematics::eNTrkRewOff))) {
        this->ApplyMCTrkVtxWeight();
    }

    if ((*fAnaOptions)->IsMC() && (*fAnaOptions)->ApplyPileupReweighting()
            && !(fSys->isShiftInUse(ZeeDSystematics::ePileUpOff))) {
        this->ApplyPileupWeight();
    }

    if ((*fAnaOptions)->IsMC() && (*fAnaOptions)->ApplyLineShapeCorrection() && (*fAnaOptions)->FillGenInfo()
            && !(fSys->isShiftInUse(ZeeDSystematics::eLineShapeOff))) {
        this->ApplyLineShapeWeight();
    }

    if ((*fAnaOptions)->GenPileupMCFile()) {

        this->GeneratePileupMCWeight();
    }

    if ( (*fAnaOptions)->IsMC() && (*fAnaOptions)->ApplyGenBosMassCut() && (*fAnaOptions)->GenBosMassCutContributesToGenWeight()) {
        this->ApplyGenBosMassWeight();
    }

    // must be last!
    if ((*fAnaOptions)->IsMC()) {
        fEventWeight *= fGenWeight;
    }


    fEvent->SetGenWeight(fGenWeight);
    fEvent->SetWeight(fEventWeight);

}

//------------------------------------------------------
void ZeeDCalcEventWeight::ApplyMCVtxWeight() {
    // Apply all eff weights
    DEBUG_MES_START;

    Double_t eff_weight = 1.0;

    if (fEvent == NULL) {
        return;
    }

    Int_t Nvtx = fEvent->Nvtx().IsSet() ? fEvent->Nvtx().Get() : 0;

    if (Nvtx > 0) {
        // Do not set upper limit on number of vtx
        eff_weight = weighthist->GetBinContent(Nvtx + 1);
    }

    // Avoid zero values
    if (eff_weight == 0) {
        eff_weight = 1.0;
    }

    // Do systematic for PileUp reweighting
    if (fSys->isShiftInUse(ZeeDSystematics::ePileUpOff)) {
        eff_weight = 1;
    }

    fGenWeight *= eff_weight;

    DEBUG_MES_END;
    return;
}

//------------------------------------------------------
void ZeeDCalcEventWeight::ApplyMCTrkVtxWeight() {
    // Apply all eff weights
    DEBUG_MES_START;

    Double_t eff_weight = 1.0;

    if (fEvent == NULL) {
        return;
    }

    // Get array of vertices
    const TObjArray* vertices = fEvent->GetVertices();

    if (vertices->GetEntriesFast() > 0) {
        ZeeDVertex* vertex = static_cast<ZeeDVertex*>(vertices->At(0) );

        Int_t numTracks = vertex->GetNTrack();
        Int_t binN = NTrkVtx->FindBin(numTracks);
        eff_weight = NTrkVtx -> GetBinContent( binN );
        //std::cout << eff_weight << "  " << numTracks << std::endl;
        if( fSys->isShiftInUse(ZeeDSystematics::eNTrkRewUp) ) {
            eff_weight += NTrkVtx -> GetBinError( numTracks );
        }

        if( fSys->isShiftInUse(ZeeDSystematics::eNTrkRewDown) ) {
            eff_weight -= NTrkVtx -> GetBinError( numTracks );
        }

    }

    // Avoid zero values
    if (eff_weight == 0) {
        eff_weight = 1.0;
    }

    if (fSys->isShiftInUse(ZeeDSystematics::eNTrkRewOff)) {
        eff_weight = 1;
    }

    fGenWeight *= eff_weight;

    DEBUG_MES_END;
    return;
}

//------------------------------------------------------
void ZeeDCalcEventWeight::ApplyLineShapeWeight() {
    DEBUG_MES_START;

    const ZeeDGenParticle* Boson = fEvent->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);

    if(Boson == NULL){
        Warning("ZeeDCalcEventWeight::ApplyLineShapeWeight", "Line Shape reweighting could not be performed, GenZ boson is not found");
        return;
    }

    if ( !fEvent->PDF().IsSet() ){
        Warning("ZeeDCalcEventWeight::ApplyLineShapeWeight", "Line Shape reweighting could not be performed, PDF info is not found");
        return;
    }

    double id1 = fEvent->PDF().Get().id1;
    double id2 = fEvent->PDF().Get().id2;

    //   https://twiki.cern.ch/twiki/pub/AtlasProtected/WZGroupInclusiveMeasurement2011/pdfinfo_lineshape.txt
    // double gluon -> just put u ubar/u dbar/ubar d (this will over/underestimate some corrections!)
    if (id1 == 21 && id2 == 21) {
        id1 = 2;
        id2 = -2;
    } else if (id1 == 21) {
        id1 = -id2;
    } else if (id2 == 21) {
        id2 = -id1;
    }

    const TLorentzVector& fourVector = Boson->GetMCFourVector();
    double mz = fourVector.M();

    double weight = 1.;
    const ZeeDEnum::MCtype::Value MCtype = fEvent->MCtype().Get();

    switch(MCtype) {

        case ZeeDEnum::MCtype::PowhegPythia:
        case ZeeDEnum::MCtype::PowhegPythia8:
        case ZeeDEnum::MCtype::PowhegHerwig:
            weight = frwtz.reweightPowhegToImprovedBorn(id1, id2, mz);
            break;
        case ZeeDEnum::MCtype::MCNLO:
            weight = frwtz.reweightMCANLOToImprovedBorn(id1, id2, mz);
            break;
        case ZeeDEnum::MCtype::Pythia:
            weight = frwtz.reweightPythiaToImprovedBorn(id1, id2, mz);
            break;
        default:
            Error("ZeeDCalcEventWeight::ApplyLineShapeWeight", "Line Shape reweighting could not be performed, unhandled MC type: %s", ZeeDEnum::MCtype::ToString(MCtype));
            gSystem->Exit(EXIT_FAILURE);

    }

    fGenWeight *= weight;

    DEBUG_MES_END;
    return;

}

void ZeeDCalcEventWeight::ApplyUnofficialPtWeight(){
    ZeeDSystematics::ZeeDSingleSystematics* zptrew = fSys->isShiftInUse(ZeeDSystematics::eZPtRew);
    const ZeeDGenParticle* Boson = fEvent->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);
    Double_t pt_weight = 1.0;
    TH1D* pCor = NULL;
    if (fSys->isShiftInUse(ZeeDSystematics::eZPtRew)){
        if (Boson != NULL) {
            const TLorentzVector& fourVector = Boson->GetMCFourVector();
            ZeeDGenElectrons genLeptons = fEvent->GetGenLeptons(ZeeDEnum::MCFSRLevel::Born);
            const ZeeDGenParticle* El    = genLeptons.first;
            const ZeeDGenParticle* Pos   = genLeptons.second;


            double pt = (fourVector.Pt()) * 1000;
            double mz = (fourVector.M()) * 1000;
            switch (Boson->GetParticleID()){
                case 23: 
                    pCor = (TH1D*)fileSF->Get("Z");
                    break;
                case 24: 
                    pCor = (TH1D*)fileSF->Get("Wplus");
                    break;
                case -24:
                    pCor = (TH1D*)fileSF->Get("Wminus");
                    break;
                default: 
                    pCor = NULL;
            }
            /*
            switch (El->GetParticleID()){
                case 11:
                  pCor = (TH1D*)fileSF->Get("SFWminusE");
                  break;
                case 13:
                  pCor = (TH1D*)fileSF->Get("SFWminusMu");
                  break;
                case 12:
                  pCor = (TH1D*)fileSF->Get("SFWplusE");
                  break;
                case 14:
                  pCor = (TH1D*)fileSF->Get("SFWplusMu");
                  break;
            }
            */
            /*
            if (pCor != NULL) {
                int binX = pCor->GetXaxis()->FindBin(fourVector.Pt());
                //if (fourVector.Pt() < 10)
                    pt_weight = pCor->GetBinContent(binX);
            }*/
            
            if (pCor != NULL) {

                int binYn = pCor->GetYaxis()->FindBin(fourVector.Rapidity());
                int binXn = pCor->GetXaxis()->FindBin(fourVector.Pt());
                int totBin = pCor->GetBin(binXn, binYn);
                pt_weight = pCor->GetBinContent(totBin);
                double sfErr = pCor->GetBinError(totBin);
               
                /*
                if (fSys->isShiftInUse(ZeeDSystematics::HadrRecoilSumEtUp))     {
                    pt_weight += sfErr;
                } else if (fSys->isShiftInUse(ZeeDSystematics::HadrRecoilSumEtDown)) {
                    pt_weight -= sfErr;
                }
                */

            }

        }
    }
//    std::cout << pt_weight << std::endl;
    fGenWeight *= pt_weight;
}

//------------------------------------------------------
void ZeeDCalcEventWeight::ApplyOfficialPtWeight() {

    BosonPtReweightingTool* rwTool = NULL;
    ZeeDSystematics::ZeeDSingleSystematics* zptrew = fSys->isShiftInUse(ZeeDSystematics::eZPtRew);
    if (zptrew != NULL) {
        rwTool = rwToolMap[zptrew->suffix];
    } else {
        rwTool = rwTool_DefaultTarget;
    }

    const ZeeDGenParticle* Boson = fEvent->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);

    Double_t pt_weight = 1.0;

    if (Boson != NULL) {
        const TLorentzVector& fourVector = Boson->GetMCFourVector();
        double pt = (fourVector.Pt()) * 1000;
        double mz = (fourVector.M()) * 1000;
        switch (Boson->GetParticleID()){
            case 23: 
                pt_weight = rwTool->GetPtWeightZ(pt, mz);
                break;
            case 24: 
                pt_weight = rwTool->GetPtWeightWplus(pt);
                break;
            case -24:
                pt_weight = rwTool->GetPtWeightWminus(pt);
                break;
            default: 
                pt_weight = 1.;
        }
        DEBUG_VAR(pt_weight);

    } else {

        Warning("ZeeDCalcEventWeight::ApplyOfficialPtWeight",
                "Generated boson not found! Is gen info filled and turned on? Weight is set to 1!");

        pt_weight = 1.;
    }

    fGenWeight *= pt_weight;
}

//------------------------------------------------------
void ZeeDCalcEventWeight::ApplyPDFWeight() {
    DEBUG_MES_START;

    const ZeeDGenParticle* Boson = fEvent->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);
    Double_t pdf_weight = 1.0;

    if (Boson == NULL) {
        return;
    }

    if (fEvent->PDF().IsSet()) {

        double w1 = ZeeDPDF::getCorrectionFactor(enumConverter((int) fEvent->PDF().Get().id1), fEvent->PDF().Get().x1, fEvent->PDF().Get().scalePDF);
        double w2 = ZeeDPDF::getCorrectionFactor(enumConverter((int) fEvent->PDF().Get().id2), fEvent->PDF().Get().x2, fEvent->PDF().Get().scalePDF);

        pdf_weight = w1 * w2;
    }
    fGenWeight *= pdf_weight;

    DEBUG_MES_END;
    return;

}

//------------------------------------------------------
void ZeeDCalcEventWeight::ApplyZvWeight() {

    Double_t vtx_z_weight = 1.0;

    if (fEvent == NULL) {
        return;
    }

    if (fEvent->Vertex().IsSet() == false) {
        return;
    }

    // Get event Z vertex:
    TVector3 vtx = fEvent->Vertex().Get().position;

    vtx_z_weight = zvtxrwTool->GetWeight(vtx.Z());

    // Do systematic for Zv reweighting
    if (fSys->isShiftInUse(ZeeDSystematics::eZvRewOff)) {
        vtx_z_weight = 1.;
    }

    fGenWeight *= vtx_z_weight;

    return;
}

//------------------------------------------------------
void ZeeDCalcEventWeight::ApplyGenBosMassWeight() {


    const ZeeDGenParticle* Boson = fEvent->GetGenBoson(ZeeDEnum::MCFSRLevel::Born);

    if(Boson) {
        const Double_t M = Boson->GetMCFourVector().M();

        const Double_t min_mass = (*fAnaOptions)->GenBosMassMin();
        const Double_t max_mass = (*fAnaOptions)->GenBosMassMax();

        // only cut on max mass if it's larger than min mass
        const Bool_t within_window = (min_mass < max_mass) ? (min_mass < M && M < max_mass) : (min_mass < M);
        const Bool_t accept = (*fAnaOptions)->GenBosMassMirrorCut() ? !within_window : within_window;

        fGenWeight = accept ? fGenWeight : 0;

    }

}

//------------------------------------------------------
void ZeeDCalcEventWeight::GeneratePileupMCWeight()
{
    const Double_t mu = fEvent->AverageInteractionsPerCrossing().Get();
    const Int_t origRunNumber = fEvent->OrigRunNumber().Get();
    const Int_t mcChannelNumber =  (*fAnaOptions)->IsMC() ? fEvent->MCChannelNumber().Get() : -1;

    const Int_t mcEventWeight =  (*fAnaOptions)->IsMC() ? fEvent->GetGenWeight() : 1;

    fPileupTool->Fill(origRunNumber,
            mcChannelNumber,
            mcEventWeight ,//mcevt_weight[0][0], // FIXME: retrieve generator event weight of the hard scatter
            mu);

    return;
}

//------------------------------------------------------
void ZeeDCalcEventWeight::MCAssignRandomRunNumber() {

    const int eventNumber = fEvent->EventNumber().Get();
    const int origRunNumber = fEvent->OrigRunNumber().Get();
    const int mcChannelNumber = fEvent->MCChannelNumber().Get();

    fPileupTool->SetRandomSeed(314159 + mcChannelNumber * 2718 + eventNumber); // recommendation from the official PileupReweighting page 

    int runNumber = fPileupTool->GetRandomRunNumber(origRunNumber);

    if (runNumber != 0) {
        fEvent->RunNumber().UnLock();
        fEvent->RunNumber().Set(runNumber);
        fEvent->RunNumber().Lock();
    } else {
        Error("ZeeDCalcEventWeight::MCAssignRandomRunNumber", "The new assigned run number is 0, this could mess up the run-dependent corrections. Check the PileupReweighting tool setup.");
        gSystem->Exit(EXIT_FAILURE);
    }

} 

//------------------------------------------------------
void ZeeDCalcEventWeight::ApplyPileupWeight() {

    DEBUG_MES_START;

    const double mu = fEvent->AverageInteractionsPerCrossing().Get();
    const int   origRunNumber = fEvent->OrigRunNumber().Get();
    const int   mcChannelNumber = fEvent->MCChannelNumber().Get();

    DEBUG_VAR(origRunNumber);
    DEBUG_VAR(mcChannelNumber);
    DEBUG_VAR(mu);

    Double_t combinedWeight = 0;

    if ((*fAnaOptions)->DoSinglePeriods() && (*fAnaOptions)->MCAssignRandomRunNumber()) {

        int runNumber = fEvent->RunNumber().Get();

        std::pair<int, int> periodStartEnd = ZeeDMisc::GetPeriodStartEnd(ZeeDMisc::GetPeriod(runNumber));
        const int periodStart = periodStartEnd.first;
        const int periodEnd = periodStartEnd.second;
        const int periodNumber = fPileupTool->GetPeriodNumber(origRunNumber, periodStart, periodEnd);

        combinedWeight = fPileupTool->GetCombinedWeight(periodNumber, mcChannelNumber, mu);
        combinedWeight *= fPileupTool->GetIntegratedLumiFraction(origRunNumber, periodStart, periodEnd);

    } else {

        combinedWeight = fPileupTool->GetCombinedWeight(origRunNumber, mcChannelNumber, mu);

    }

    //     Info(__PRETTY_FUNCTION__,"applyiing weight");
    //     std::cout <<" fGen  = "<<fGenWeight<< std::endl;
    //     std::cout <<"  PU w = "<<combinedWeight<< std::endl;

    DEBUG_VAR(combinedWeight);

    fGenWeight *= combinedWeight;

    DEBUG_MES_END;

    //    std::cout <<" fGen = "<<fGenWeight<< std::endl;

    return; 
}

//------------------------------------------------------
ZeeDPDF::Flavour ZeeDCalcEventWeight::enumConverter(int id) {

    if (id == 21) {
        return ZeeDPDF::GLUON;
    }
    if (id == 1) {
        return ZeeDPDF::DOWN;
    }
    if (id == 2) {
        return ZeeDPDF::UP;
    }
    if (id == 3) {
        return ZeeDPDF::STRANGE;
    }
    if (id == 4) {
        return ZeeDPDF::CHARM;
    }
    if (id == 5) {
        return ZeeDPDF::BOTTOM;
    }
    if (id == 6) {
        return ZeeDPDF::TOP;
    }
    if (id == -1) {
        return ZeeDPDF::DBAR;
    }
    if (id == -2) {
        return ZeeDPDF::UBAR;
    }
    if (id == -3) {
        return ZeeDPDF::SBAR;
    }
    if (id == -4) {
        return ZeeDPDF::CBAR;
    }
    if (id == -5) {
        return ZeeDPDF::BBAR;
    }
    if (id == -6) {
        return ZeeDPDF::TBAR;
    }

    Info("ZeeDCalcEventWeight::enumConverter", "Unknown id");
    return ZeeDPDF::GLUON;

}

//------------------------------------------------------
void ZeeDCalcEventWeight::ConfigurePileupToolToGenerateMCFile() {
    Info(__PRETTY_FUNCTION__, " Configuring ...");

    if ((*fAnaOptions)->MCCampaign() == "mc11a") {

        fPileupTool->UsePeriodConfig("MC11a");

    } else if ((*fAnaOptions)->MCCampaign() == "mc11b" || (*fAnaOptions)->MCCampaign() == "mc11c") {

        fPileupTool->UsePeriodConfig("MC11b");

    } else if ((*fAnaOptions)->MCCampaign() == "mc12a") {

        Info(__PRETTY_FUNCTION__, "In there MC12a");
        fPileupTool->UsePeriodConfig("MC12a");

    } else if ((*fAnaOptions)->MCCampaign() == "mc12b") {

        Info(__PRETTY_FUNCTION__, "In there MC12b");
        fPileupTool->UsePeriodConfig("MC12b");

    } else {

        Error("ZeeDCalcEventWeight::ZeeDCalcEventWeight", "Unkown MC campaing: %s", (*fAnaOptions)->MCCampaign().c_str());
        gSystem->Exit(EXIT_FAILURE);

    }
    Info(__PRETTY_FUNCTION__, " Configured ...%s", (*fAnaOptions)->MCCampaign().c_str());

}

//------------------------------------------------------
void ZeeDCalcEventWeight::ConfigurePileupToolForReweighting() {

    if ((*fAnaOptions)->PileupMCFile() == "" || (*fAnaOptions)->PileupDataFile() == "") {

        Error("ZeeDCalcEventWeight::ZeeDCalcEventWeight", "At least one pileup reweighting file is missing:\n PileupMCFile: %s\n PileupDataFile: %s",
                (*fAnaOptions)->PileupMCFile().c_str(),
                (*fAnaOptions)->PileupDataFile().c_str());
        gSystem->Exit(EXIT_FAILURE);

    }

    if ((*fAnaOptions)->DoSinglePeriods() && (*fAnaOptions)->MCAssignRandomRunNumber()) {

        this->ConfigurePileupToolToReweightToIndividualPeriods();

    }

    const std::string locationOfMCFile(ZeeDMisc::FindFile((*fAnaOptions)->PileupMCFile()).Data());
    Info("ZeeDCalcEventWeight::ZeeDCalcEventWeight", "Open MC root files for Pileup Reweighting : %s", locationOfMCFile.c_str());

    const std::string locationOfDataFile(ZeeDMisc::FindFile((*fAnaOptions)->PileupDataFile()).Data());
    Info("ZeeDCalcEventWeight::ZeeDCalcEventWeight", "Open Data root files for Pileup Reweighting : %s", locationOfDataFile.c_str());

    fPileupTool->SetDefaultChannel(106046); // set PythiaZee as default channel

    fPileupTool->SetDefaultChannel(0);

    fPileupTool->AddConfigFile(locationOfMCFile);

    if ((*fAnaOptions)->MCCampaign() == "mc12a") {
        // luminosity tag: OflLumi-8TeV-002
        fPileupTool->SetDataScaleFactors(1. / 1.11);  // https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/InDetTrackingPerformanceGuidelines
        fPileupTool->SetDefaultChannel( 0 );          // set default mc channel to the one in default prw file from the Pileupreweighting tool
    } else if ((*fAnaOptions)->MCCampaign() == "mc12b") {
        // luminosity tag: OflLumi-8TeV-003
        // see https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/ExtendedPileupReweighting#Recipe_A_MC12a_Pileup_Reweightin
        fPileupTool->SetDataScaleFactors(1. / 1.09);
        fPileupTool->SetDefaultChannel( 0 );
    }

    fPileupTool->AddLumiCalcFile(locationOfDataFile);

    fPileupTool->SetUnrepresentedDataAction(2);


}

//------------------------------------------------------
void ZeeDCalcEventWeight::ConfigurePileupToolToReweightToIndividualPeriods() {

    fPileupTool->IgnoreConfigFilePeriods(true);

    if ((*fAnaOptions)->MCCampaign() == "mc11a") {

        DEBUG_MES("Adding period assignments for MC campaign mc11a");

        fPileupTool->AddPeriod(180164, 179710, 180481); // D

        fPileupTool->AddPeriod(183003, 180614, 180776); // E
        fPileupTool->AddPeriod(183003, 182013, 182519); // F
        fPileupTool->AddPeriod(183003, 182726, 183462); // G
        fPileupTool->AddPeriod(183003, 183544, 184169); // H

        fPileupTool->AddPeriod(185649, 185353, 186493); // I
        fPileupTool->AddPeriod(185649, 186516, 186755); // J
        fPileupTool->AddPeriod(185649, 186873, 187815); // K

        fPileupTool->AddPeriod(185761, 188902, 190343); // L
        fPileupTool->AddPeriod(185761, 190503, 191933); // M

    } else if ((*fAnaOptions)->MCCampaign() == "mc11b" ||
            (*fAnaOptions)->MCCampaign() == "mc11c") {

        DEBUG_MES("Adding period assignments for MC campaign mc11a");

        fPileupTool->AddPeriod(180164, 179710, 180481); // D

        fPileupTool->AddPeriod(183003, 180614, 180776); // E
        fPileupTool->AddPeriod(183003, 182013, 182519); // F
        fPileupTool->AddPeriod(183003, 182726, 183462); // G
        fPileupTool->AddPeriod(183003, 183544, 184169); // H

        fPileupTool->AddPeriod(186169, 185353, 186493); // I
        fPileupTool->AddPeriod(186169, 186516, 186755); // J
        fPileupTool->AddPeriod(186169, 186873, 187815); // K

        fPileupTool->AddPeriod(189751, 188902, 190343); // L
        fPileupTool->AddPeriod(189751, 190503, 191933); // M

    } else if ((*fAnaOptions)->MCCampaign() == "mc12a") {

        DEBUG_MES("Adding period assignments for MC campaign mc12a");

        fPileupTool->AddPeriod(195847, 200804, 201556); // A
        fPileupTool->AddPeriod(195847, 202660, 205113); // B
        fPileupTool->AddPeriod(195847, 206248, 207397); // C
        fPileupTool->AddPeriod(195847, 207447, 209025); // D
        fPileupTool->AddPeriod(195847, 209074, 210308); // E
        fPileupTool->AddPeriod(195847, 211522, 212272); // G
        fPileupTool->AddPeriod(195847, 212619, 213359); // H 
        fPileupTool->AddPeriod(195847, 213431, 213819); // I
        fPileupTool->AddPeriod(195847, 213900, 215091); // J
        fPileupTool->AddPeriod(195847, 215414, 215643); // L

    } else {

        Error("ZeeDCalcEventWeight::ZeeDCalcEventWeight", "Unkown MC campaing: %s", (*fAnaOptions)->MCCampaign().c_str());
        gSystem->Exit(EXIT_FAILURE);

    }

}

//------------------------------------------------------
void ZeeDCalcEventWeight::ConfigureBosonPtReweightingTool() {

    if (fEvent->MCtype().IsSet() == kFALSE) {
        Error("ZeeDCalcEventWeight::ConfigureBosonPtReweightingTool", "MCtype is not set. Set MCtype or switch off BosonPtReweightingTool.");
        gSystem->Exit(EXIT_FAILURE);
    }

    // Add all Pt reweighing tools
    AddBosonPtReweightingTool((*fAnaOptions)->BosonPtReweightingTarget(), true);

    if ((*fAnaOptions)->EvalSysPtZReweighting()) {
        std::vector<std::string> fAdditionalTargets = (*fAnaOptions)->BosonPtReweightingAdditionalTargets();
        for (std::vector<std::string>::iterator it = fAdditionalTargets.begin(); it != fAdditionalTargets.end(); ++it) {
            if ((*it).length() > 0) {
                AddBosonPtReweightingTool(*it, false);
            } else {
                Warning("ZeeDCalcEventWeight::ConfigureBosonPtReweightingTool", "The BosonPtReweightingAdditionalTargets list contains empty target name (namely [...,\"\",...]); no rwTool will be created for this target.");
            }
        }
    }

}

//------------------------------------------------------
void ZeeDCalcEventWeight::AddBosonPtReweightingTool(std::string targetName, bool defaultTool)
{
    const TString WeightROOTfile = ZeeDMisc::FindFile("WZBosonKinematicsReweight.root").Data();
    const Int_t datasetID = fEvent->MCChannelNumber().IsSet() ? fEvent->MCChannelNumber().Get() : (*fAnaOptions)->DatasetID();
    const TString datasetID_string = TString::Format("%i", datasetID);
    Info("ZeeDCalcEventWeight::AddBosonPtReweightingTool", "Add Pt reweighing tool for the \"%s\" target.", targetName.c_str());

    // the tool reweights from datasetID (MC) to targetName 
    BosonPtReweightingTool* rwTool = new BosonPtReweightingTool(datasetID_string.Data(), targetName, BosonPtReweightingTool::MassBinnedWeight, WeightROOTfile.Data());

    rwToolMap[targetName] = rwTool;

    if (defaultTool) rwTool_DefaultTarget = rwTool;

}
