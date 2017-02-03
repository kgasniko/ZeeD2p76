#ifndef ZEED_IDENTIFIER_H
#define ZEED_IDENTIFIER_H

/** Prototype class for Event Identifier. The class holds
    BosonSelector as well as it returns interpretation of the event
    depending on that boson

    In this brunch class a little bit differs from main brunch.

*/

#include "ZeeDAnalysisCuts/ZeeDFinder.h"
#include "ZeeDCuts/IZeeDCalcWeight.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include <TObjArray.h>

#include <vector>
#include <utility>

class ZeeDEvent;

class ZeeDIdentifier : public TNamed {
public:
    ZeeDIdentifier(TString name);
    virtual ~ZeeDIdentifier();

    // Register the selector:
    void SetFinder(ZeeDFinder* Finder){
        fFinder = Finder;
    };

    // Enforce event ID
    // @param idnumber - identification number
    virtual void SetEventIdentification(ZeeDEvent* event, UInt_t idnumber) const {
      event->SetCurrentBoson((ZeeDBosonZ*)fBosonsAndWeights.at(idnumber).first);
    }

    // Find the boson candidates:
    virtual void IdentifyRecBosons(ZeeDEvent* event);

    // Find the jets;
    virtual void IdentifyJets();

    UInt_t GetNumberOfIdentifications() const {
        return fBosonsAndWeights.size();
    };
    
protected:

    void SetCurrentEvent(ZeeDEvent* event) {
         fEvent=event;
         };
         
    ZeeDFinder* GetFinder (){
          return fFinder;
          }
          
    /*
    // SS: disabled during extensions for several bosons per event
    // Return results:
    const ZeeDBosonZ* GetBoson() const;
    const TObjArray* GetElectrons() const;
    Double_t   GetWeight() const {
        return fWeight;
    }
    */
    
    LinkWeightContainer<const ZeeDBoson*> fBosonsAndWeights;


    ZeeDFinder*     fFinder;
    ZeeDEvent*      fEvent;
    IZeeDCalcWeight* fCalcWeight;

};


#endif
