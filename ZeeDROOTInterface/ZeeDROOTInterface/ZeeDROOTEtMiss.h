#ifndef ZEEDROOTETMISS_H
#define ZEEDROOTETMISS_H

#include <TObject.h>

#include "SGTools/CLASS_DEF.h"

class ZeeDEtMiss;

/** Partner class of ZeeDEtMiss
    @author George Sedov
    @date Wed Mar 16 12:24:57 CEST 2011
*/

typedef std::vector<Float_t>  vectorF;
typedef std::vector<UShort_t> vectorI;


class ZeeDROOTEtMiss {

public:

    ZeeDROOTEtMiss();
    virtual ~ZeeDROOTEtMiss();

    Int_t       fillIn (const ZeeDEtMiss* const etMiss);
    Int_t       fillOut(ZeeDEtMiss* const etMiss) const;

private:
    // Fields that must be saved in ROOT. NO POINTERS ALLOWED
    Double_t fEt;
    Double_t fEtX;
    Double_t fEtY;
    Double_t fPhi;
    Double_t fCorRecoilEt;
    Double_t fCorRecoilEtX;
    Double_t fCorRecoilEtY;
    Double_t fCorRecoilPhi;
	Double_t fCorRecoilSumet;

    Float_t fMET_SoftTermETX;
    Float_t fMET_SoftTermETY;
    Float_t fMET_SoftTermETSUM;

    
    vectorF fMETCompositionJet_wet;
    vectorF fMETCompositionJet_wpx;
    vectorF fMETCompositionJet_wpy;
    vectorI fMETCompositionJet_statusWord;
    vectorI fMETCompositionJet_index;

    vectorF fMETCompositionElectron_wet;
    vectorF fMETCompositionElectron_wpx;
    vectorF fMETCompositionElectron_wpy;
    vectorI fMETCompositionElectron_statusWord;
    vectorI fMETCompositionElectron_index;

    vectorF fMETCompositionPhoton_wet;
    vectorF fMETCompositionPhoton_wpx;
    vectorF fMETCompositionPhoton_wpy;
    vectorI fMETCompositionPhoton_statusWord;
    vectorI fMETCompositionPhoton_index;

    vectorF fMETCompositionTau_wet;
    vectorF fMETCompositionTau_wpx;
    vectorF fMETCompositionTau_wpy;
    vectorI fMETCompositionTau_statusWord;
    vectorI fMETCompositionTau_index;

    vectorF fMETCompositionMuon_wet;
    vectorF fMETCompositionMuon_wpx;
    vectorF fMETCompositionMuon_wpy;
    vectorI fMETCompositionMuon_statusWord;
    vectorI fMETCompositionMuon_index;


};

CLASS_DEF( ZeeDROOTEtMiss , 80761233 , 1 )

#endif
