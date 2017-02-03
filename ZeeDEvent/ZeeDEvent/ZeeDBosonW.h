#ifndef __ZEEDBOSONW_H
#define __ZEEDBOSONW_H

////////////////////////////////////////////////////////
// Name    : ZeeDBosonW.h
////////////////////////////////////////////////////////
//
// Contact persons: Andrei Nikiforov
//

// ZeeDAnalysis includes
#include "ZeeDEvent/ZeeDBoson.h"
#include "ZeeDEvent/ZeeDLepton.h"
#include "ZeeDEvent/ZeeDEtMiss.h"

/** Basic class for W boson particles */
class ZeeDBosonW : public ZeeDBoson {

public:
    ~ZeeDBosonW();

    // Getters

    /** Returns transverse W mass */
    Double_t GetMt()   const {
        return fMt;
    }

    /** Returns W's electron index */
    Int_t  GetLepIndex() const {
        return fLepIdx;
    }

    /** Returns pointer to the missing Et */
    const ZeeDEtMiss* GetEtMissPtr() const {
        return fEtMiss;
    }

    /** Returns missing Et */
    Double_t GetEtMiss() const {
        return fEt;;
    }

	void SetEtMiss(double et) {
	    fEt=et;
	}

    /** Returns X component of missing Et */
    Double_t GetEtMissX() const {
        return fEtMiss->GetEtX();
    }

    /** Returns Y component of missing Et */
    Double_t GetEtMissY() const {
        return fEtMiss->GetEtY();
    }

    /** Returns phi of missing Et */
    Double_t GetEtMissPhi() const {
        return fEtMiss->GetPhi();
    }

    /** Prints out W boson properties */
    void Print(Option_t* option = "") const;

    void SetLepton(ZeeDLepton* electron) {fLep->AddLast((TObject*)electron);}
    void SetLepton(const ZeeDLepton* electron) {fLep->AddLast((TObject*)electron);}
    
    Double_t GetWeight()  const {
        return fWeight;
    }    

    ZeeDLepton* GetLep() const {
           if (fLep == NULL) 
               return NULL;
           return (ZeeDLepton*)(fLep->At(0)); 
           }
    
    ZeeDBosonW* CreateW(ZeeDLepton* elec);

private:
    ZeeDBosonW();

    /** Array of electrons */
    TObjArray* fLep; //!

    // Setters
    void SetMt(Double_t mass) {
        fMt = mass;
    }
    void SetLepIndex(Int_t index) {
        fLepIdx = index;
    }
    void SetEtMissPtr( ZeeDEtMiss* etmiss ) {
        fEtMiss = etmiss;
    }

    void SetWeight(Double_t weight) {
        fWeight = weight;
    }

    /** W boson transverse mass */
    Double_t fMt;
	Double_t fEt;
    Double_t fWeight;

    /** Index of the electron */ 
    Int_t fLepIdx;

    /** Missing Et */
    ZeeDEtMiss* fEtMiss;

    friend class ZeeDCalcWmu;
    friend class ZeeDCalcW;
};

#endif // ZeeDBosonW
