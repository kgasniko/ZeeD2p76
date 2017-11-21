#ifndef __ZEEDETMISS_H
#define __ZEEDETMISS_H

////////////////////////////////////////////////////////
// Name    : ZeeDEtMiss.h
////////////////////////////////////////////////////////
//
// Contact persons: Andrei Nikforov
//

// Root includes
#include <TObject.h>

typedef std::vector<Float_t>   vectorF;
typedef std::vector<UShort_t>  vectorI;
    

/** Base class for missing Et objects */
class ZeeDEtMiss : public TObject {

public:
    ZeeDEtMiss();
    //    ZeeDEtMiss(const ZeeDEtMiss& etMiss);

    ~ZeeDEtMiss();

    // Getters
    /** Get missing Et value */
    const Double_t& GetEt() const {
        return fEt;
    }

    /** Get missing EtX value */
    const Double_t& GetEtX() const {
        return fEtX;
    }

    /** Get missing EtY value */
    const Double_t& GetEtY() const {
        return fEtY;
    }

    /** Get phi of missing Et */
    const Double_t& GetPhi() const {
        return fPhi;
    }
    // Getters
    /** Get missing Et value */
    const Double_t& GetCorRecoilEt() const {
        return fCorRecoilEt;
    }

    /** GetCorRecoil missing EtX value */
    const Double_t& GetCorRecoilEtX() const {
        return fCorRecoilEtX;
    }

    /** GetCorRecoil missing EtY value */
    const Double_t& GetCorRecoilEtY() const {
        return fCorRecoilEtY;
    }

	const Double_t& GetCorRecoilSumet() const {
		return fCorRecoilSumet;
	}

    /** GetCorRecoil phi of missing Et */
    const Double_t& GetCorRecoilPhi() const {
        return fCorRecoilPhi;
    }

    vectorF& GetMETCompositionJet_wet()       {return fMETCompositionJet_wet;};
    vectorF& GetMETCompositionJet_wpx()       {return fMETCompositionJet_wpx;};
    vectorF& GetMETCompositionJet_wpy()       {return fMETCompositionJet_wpy;};
    vectorI& GetMETCompositionJet_statusWord(){return fMETCompositionJet_statusWord;};
    vectorI& GetMETCompositionJet_index()     {return fMETCompositionJet_index;};
      
    vectorF& GetMETCompositionElectron_wet()       {return fMETCompositionElectron_wet;};
    vectorF& GetMETCompositionElectron_wpx()       {return fMETCompositionElectron_wpx;};
    vectorF& GetMETCompositionElectron_wpy()       {return fMETCompositionElectron_wpy;};
    vectorI& GetMETCompositionElectron_statusWord(){return fMETCompositionElectron_statusWord;};
    vectorI& GetMETCompositionElectron_index()     {return fMETCompositionElectron_index;};
      
    vectorF& GetMETCompositionPhoton_wet()       {return fMETCompositionPhoton_wet;};
    vectorF& GetMETCompositionPhoton_wpx()       {return fMETCompositionPhoton_wpx;};
    vectorF& GetMETCompositionPhoton_wpy()       {return fMETCompositionPhoton_wpy;};
    vectorI& GetMETCompositionPhoton_statusWord(){return fMETCompositionPhoton_statusWord;};
    vectorI& GetMETCompositionPhoton_index()     {return fMETCompositionPhoton_index;};
      
    vectorF& GetMETCompositionTau_wet()       {return fMETCompositionTau_wet;};
    vectorF& GetMETCompositionTau_wpx()       {return fMETCompositionTau_wpx;};
    vectorF& GetMETCompositionTau_wpy()       {return fMETCompositionTau_wpy;};
    vectorI& GetMETCompositionTau_statusWord(){return fMETCompositionTau_statusWord;};
    vectorI& GetMETCompositionTau_index()     {return fMETCompositionTau_index;};
      
    vectorF& GetMETCompositionMuon_wet()       {return fMETCompositionMuon_wet;};
    vectorF& GetMETCompositionMuon_wpx()       {return fMETCompositionMuon_wpx;};
    vectorF& GetMETCompositionMuon_wpy()       {return fMETCompositionMuon_wpy;};
    vectorI& GetMETCompositionMuon_statusWord(){return fMETCompositionMuon_statusWord;};
    vectorI& GetMETCompositionMuon_index()     {return fMETCompositionMuon_index;};


    const vectorF& GetMETCompositionJet_wet()const       {return fMETCompositionJet_wet;};
    const vectorF& GetMETCompositionJet_wpx()const       {return fMETCompositionJet_wpx;};
    const vectorF& GetMETCompositionJet_wpy()const       {return fMETCompositionJet_wpy;};
    const vectorI& GetMETCompositionJet_statusWord()const{return fMETCompositionJet_statusWord;};
    const vectorI& GetMETCompositionJet_index()const     {return fMETCompositionJet_index;};
      
    const vectorF& GetMETCompositionElectron_wet()const       {return fMETCompositionElectron_wet;};
    const vectorF& GetMETCompositionElectron_wpx()const       {return fMETCompositionElectron_wpx;};
    const vectorF& GetMETCompositionElectron_wpy()const       {return fMETCompositionElectron_wpy;};
    const vectorI& GetMETCompositionElectron_statusWord()const{return fMETCompositionElectron_statusWord;};
    const vectorI& GetMETCompositionElectron_index()const     {return fMETCompositionElectron_index;};
      
    const vectorF& GetMETCompositionPhoton_wet()const       {return fMETCompositionPhoton_wet;};
    const vectorF& GetMETCompositionPhoton_wpx()const       {return fMETCompositionPhoton_wpx;};
    const vectorF& GetMETCompositionPhoton_wpy()const       {return fMETCompositionPhoton_wpy;};
    const vectorI& GetMETCompositionPhoton_statusWord()const{return fMETCompositionPhoton_statusWord;};
    const vectorI& GetMETCompositionPhoton_index()const     {return fMETCompositionPhoton_index;};
      
    const vectorF& GetMETCompositionTau_wet()const       {return fMETCompositionTau_wet;};
    const vectorF& GetMETCompositionTau_wpx()const       {return fMETCompositionTau_wpx;};
    const vectorF& GetMETCompositionTau_wpy()const       {return fMETCompositionTau_wpy;};
    const vectorI& GetMETCompositionTau_statusWord()const{return fMETCompositionTau_statusWord;};
    const vectorI& GetMETCompositionTau_index()const     {return fMETCompositionTau_index;};
      
    const vectorF& GetMETCompositionMuon_wet()const       {return fMETCompositionMuon_wet;};
    const vectorF& GetMETCompositionMuon_wpx()const       {return fMETCompositionMuon_wpx;};
    const vectorF& GetMETCompositionMuon_wpy()const       {return fMETCompositionMuon_wpy;};
    const vectorI& GetMETCompositionMuon_statusWord()const{return fMETCompositionMuon_statusWord;};
    const vectorI& GetMETCompositionMuon_index()const     {return fMETCompositionMuon_index;};
      
    void SetMET_SoftTermETX  (Float_t a){fMET_SoftTermETX   = a;};
    void SetMET_SoftTermETY  (Float_t a){fMET_SoftTermETY   = a;};
    void SetMET_SoftTermETSUM(Float_t a){fMET_SoftTermETSUM = a;};

    const Float_t& GetMET_SoftTermETX  () const {return fMET_SoftTermETX  ;};
    const Float_t& GetMET_SoftTermETY  () const {return fMET_SoftTermETY  ;};
    const Float_t& GetMET_SoftTermETSUM() const {return fMET_SoftTermETSUM;};

    Float_t& GetMET_SoftTermETX  () {return fMET_SoftTermETX  ;};
    Float_t& GetMET_SoftTermETY  () {return fMET_SoftTermETY  ;};
    Float_t& GetMET_SoftTermETSUM() {return fMET_SoftTermETSUM;};

    int GetnMETElec(){
        return fnMETElec;
    }
    
    void SetnMETElec(int n){
        fnMETElec=n;
    }

    int GetnMETMuon(){
        return fnMETMuon;
    }

    void SetnMETMuon(int n){
        fnMETMuon=n;
    }

    Double_t GetCorRecoilUPar(){
        return fCorRecoilUPar;
    }
    Double_t GetCorRecoilUPerp(){
        return fCorRecoilUPerp;
    }

    Double_t GetCorRecoilUParU(){
        return fCorRecoilUParU;
    }

    /** Blue print */
    void Print(Option_t* option = "") const;

private:

    // Setters
    /** Set missing Et value */
    void SetEt( Double_t et ) {
        fEt = et;
    }
    

    /** Set missing EtX value */
    void SetEtX( Double_t etx ) {
        fEtX = etx;
    }

    /** Set missing EtY value */
    void SetEtY( Double_t ety ) {
        fEtY = ety;
    }

    /** Set phi of missing Et */
    void SetPhi( Double_t phi ) {
        fPhi = phi;
    }
    // Setters
    /** SetCorRecoil missing Et value */
    void SetCorRecoilEt( Double_t et ) {
        fCorRecoilEt = et;
    }

    /** SetCorRecoil missing EtX value */
    void SetCorRecoilEtX( Double_t etx ) {
        fCorRecoilEtX = etx;
    }

    /** SetCorRecoil missing EtY value */
    void SetCorRecoilEtY( Double_t ety ) {
        fCorRecoilEtY = ety;
    }

    /** SetCorRecoil phi of missing Et */
    void SetCorRecoilPhi( Double_t phi ) {
        fCorRecoilPhi = phi;
    }

    void SetCorRecoilSumet (Double_t sumet) {
		fCorRecoilSumet=sumet;
	}
    
    void SetCorRecoilUPerp (Double_t uPerp){
        fCorRecoilUPerp=uPerp;
    }

    void SetCorRecoilUPar (Double_t uPar){
        fCorRecoilUPar=uPar;
    }

    void SetCorRecoilUParU (Double_t uParU){
        fCorRecoilUParU = uParU;
    }

    /** Missing Et */
    Double_t fEt;
	Double_t fCorRecoilEt;

    /** X projection of missing Et */
    Double_t fEtX;
	Double_t fCorRecoilEtX;

    /** Y projection of missing Et */
    Double_t fEtY;
	Double_t fCorRecoilEtY;

    Double_t fCorRecoilUPar;
    Double_t fCorRecoilUPerp;
    Double_t fCorRecoilUParU;
    /** Phi angle of missing Et */
    Double_t fPhi;
	Double_t fCorRecoilPhi;

	/** SumEt of corRecoil missing Et*/
	Double_t fCorRecoilSumet;
    
    Int_t fnMETElec;
    Int_t fnMETMuon;

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
    
    friend class ZeeDCalcW;
	friend class ZeeDCalcEtMiss;
    friend class ZeeDReadEtMissAOD;
    friend class ZeeDROOTEtMiss;
};

#endif // ZeeDEtMiss
