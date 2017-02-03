#include "ZeeDHistManager/ZeeDHistManagerElectronDetails.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>
#include <Rtypes.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"

#include "ZeeDMisc/ZeeDMisc.h"

using std::cout;
using std::endl;

//------------------------------------------------------
ZeeDHistManagerElectronDetails::ZeeDHistManagerElectronDetails(TString name, Double_t EtaMin, Double_t EtaMax)
    : ZeeDHistManagerLeptonBase(name),
      mEtaMin(EtaMin),
      mEtaMax(EtaMax)
{
    // Named constructor
}


//------------------------------------------------------
void ZeeDHistManagerElectronDetails::BookHistos()
{
    // Books histogram
    AddTH1D("ElecF1",        100, 0., 1.     , "F1 (E frac in s1)", "");
    AddTH1D("ElecF1core",    100, 0., 1.     , "F1core (in core s1)", "");
    AddTH1D("ElecE2tsts1",   100, 0., 1.     , "e2tsts1 (2nd max in strips)", "");
    AddTH1D("ElecEmins1",    100, 0., 1.     , "emins1 (E with min between max 1&2)", "");
    AddTH1D("ElecWeta1",     100, 0., 1.     , "weta1 (corr w in 3 strips in s1)", "");
    AddTH1D("ElecWtots1",    100, 0., 5.     , "wtots1 (tot w in em 1s in 20 strips)", "");

    AddTH1D("ElecFracs1",    100, 0., 3.     , "fracs1 (E outside core (E7-E3)/E7", "");
    AddTH1D("ElecE233oe277", 100, 0.3, 1.    , "e233/e277", "");
    AddTH1D("ElecE237oe277", 100, 0.3, 1.    , "e237/e277", "");
    AddTH1D("ElecWeta2",     100, 0., 0.1    , "weta2 (corr w in 3x5 cells in s2)", "");
    AddTH1D("ElecWidths2",   100, 0., 0.1    , "widths2 (uncorr w in em s2 in 3x5)", "");
    AddTH1D("ElecF3core",    100, 0., 1.     , "f3core (E frac in core s3)", "");

    AddTH1D("ElecDeltaphi2", 100, -0.02, 0.02, "#Delta#phi2 (rad)", "");
    AddTH1D("ElecDeltaeta1", 100, -0.01, 0.01, "#Delta#eta2 (rad)", "");
    AddTH1D("ElecDeltaeta2", 100, -0.02, 0.02, "#Delta#eta2 (rad)", "");

    AddTH1D("ElecEtCone20", 40,0.,1.," Et cone / Et cluster","");
    AddTH1D("ElecEtCone30", 40,0.,1.," Et cone / Et cluster","");
    AddTH1D("ElecEtCone40", 40,0.,1.," Et cone / Et cluster","");
    AddTH1D("ElecPtCone20", 40,0.,1.," Pt cone / Et cluster","");
    AddTH1D("ElecPtCone30", 40,0.,1.," Pt cone / Et cluster","");
    AddTH1D("ElecPtCone40", 40,0.,1.," Pt cone / Et cluster","");
    
    
}


//-----------------------------------------------------
void ZeeDHistManagerElectronDetails::FillLeptonHists(const TObjArray* electrons, Double_t Weight)
{
    for ( Int_t i = 0; i < electrons->GetEntriesFast(); ++i ) {

        const ZeeDElectron* electron =  static_cast< ZeeDElectron* > (electrons->At(i));
        const TLorentzVector& fourVector = electron->GetFourVector();

        Double_t elecPt = fourVector.Pt();        

// Check eta range
        if ( (fourVector.Eta() > mEtaMin) && (fourVector.Eta() < mEtaMax) ) {
            if (electron->Shower().IsSet() ) {
                FillTH1( electron->Shower().Get().f1, Weight, "ElecF1");
                FillTH1( electron->Shower().Get().f1core, Weight, "ElecF1core");
                FillTH1( electron->Shower().Get().e2tsts1, Weight, "ElecE2tsts1");
                FillTH1( electron->Shower().Get().emins1, Weight, "ElecEmins1");
                FillTH1( electron->Shower().Get().weta1, Weight, "ElecWeta1");
                FillTH1( electron->Shower().Get().wtots1, Weight, "ElecWtots1");

                FillTH1( electron->Shower().Get().fracs1, Weight, "ElecFracs1");
                FillTH1( electron->Shower().Get().e233 / electron->Shower().Get().e277, Weight, "ElecE233oe277");
                FillTH1( electron->Shower().Get().e237 / electron->Shower().Get().e277, Weight, "ElecE237oe277");
                FillTH1( electron->Shower().Get().weta2, Weight, "ElecWeta2");
                FillTH1( electron->Shower().Get().widths2, Weight, "ElecWidths2");
                FillTH1( electron->Shower().Get().f3core, Weight, "ElecF3core");


//                std::cout <<"et20:"<<electron->Shower().Get().etcone20<<" pt20:"<<electron->Shower().Get().ptcone20/GeV<<std::endl;
                
                
                FillTH1( electron->Shower().Get().etcone20/elecPt*0.001, Weight, "ElecEtCone20");
                FillTH1( electron->Shower().Get().etcone30/elecPt*0.001, Weight, "ElecEtCone30");
                FillTH1( electron->Shower().Get().etcone40/elecPt*0.001, Weight, "ElecEtCone40");
                FillTH1( electron->Shower().Get().ptcone20/elecPt*0.001, Weight, "ElecPtCone20");
                FillTH1( electron->Shower().Get().ptcone30/elecPt*0.001, Weight, "ElecPtCone30");
                FillTH1( electron->Shower().Get().ptcone40/elecPt*0.001, Weight, "ElecPtCone40");
                
                
            }

            if (electron->TrackMatch().IsSet() ) {

                FillTH1( electron->TrackMatch().Get().deltaEta1, Weight, "ElecDeltaeta1");
                FillTH1( electron->TrackMatch().Get().deltaEta2, Weight, "ElecDeltaeta2");
                FillTH1( electron->TrackMatch().Get().deltaPhi2, Weight, "ElecDeltaphi2");

            }
        }

    }

}


