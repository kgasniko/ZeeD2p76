#include "ZeeDAnalysisCuts/ZeeDFinder.h"
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDJet.h"

#include <iostream>

/* --------------------------------------------

  Created 11 Feb 2011 by SG. Loop over boson candidates, select the best

*/


ZeeDFinder::~ZeeDFinder()
{

    for (std::vector<CutWeight*>::const_iterator CWiter = CutWeights.begin();
	CWiter != CutWeights.end(); ++CWiter) delete *CWiter;

}


//------------------------------------------------------
Double_t ZeeDFinder::GetWeight(ZeeDCutBit* mask)
{
    // Returns weight
    Double_t res = 0;
    ZeeDCutBit::maskDataType MV = mask->GetMask();

    for (std::vector<CutWeight*>::iterator CW = CutWeights.begin(); CW != CutWeights.end(); ++CW) {
        if ( (*CW)->Mask.GetMask() & MV ) {
            res +=  (*CW)->Weight;
        }
    }

    return res;
}


//------------------------------------------------------
const ZeeDBosonZ* ZeeDFinder::SelectBestBoson(ZeeDEvent* Event, const TObjArray* Bosons)
{
  // Find the best boson in the event.

  const ZeeDBosonZ* bestBoson = NULL;
  int nBosons = Bosons->GetEntriesFast();

  if (nBosons > 0) {

    // do some acro:
    // this->evaluate(event, mask) evaluates mask on the event
    // so the current event is used to substitute boson
    // and evaluate the cut; then the event's old boson goes back 

    // the hack works fast, but is really unsafe

    // safe current boson
    const ZeeDBosonZ* currentBosonSafe = Event->GetCurrentBoson();

    Double_t BestVal = -1;
    ZeeDCutBit Mask;
    for (Int_t i = 0; i < nBosons; i++) {

      const ZeeDBosonZ* newBoson = static_cast<ZeeDBosonZ*>(Bosons->At(i));
      Event->SetCurrentBoson(newBoson);

      this->evaluate(Event, &Mask);  // Get bit mask
      Double_t val = this->GetWeight(&Mask); // Get weight

      if (val > BestVal) {
	BestVal = val;
	bestBoson = newBoson;
      }
    }

    // restore current boson
    Event->SetCurrentBoson(currentBosonSafe);
  }

  return bestBoson;

}
/*
TObjArray ZeeDFinder::SelectAllGoodBosons(ZeeDEvent* Event, const TObjArray* Bosons)
{
  TObjArray bosonArray;
  ZeeDCutBit Mask;
  
  const ZeeDBosonW* currentBosonSafe = Event->GetCurrentBoson()
}
*/

//------------------------------------------------------
TObjArray ZeeDFinder::SelectAllGoodBosons(ZeeDEvent* Event, const TObjArray* Bosons)
{
  // Find all the bosons satysfying the selection
  TObjArray bosonArray;
  ZeeDCutBit Mask;

  // safe current boson - loop will change it for weight calculation
  //const ZeeDBosonZ* currentBosonSafe = Event->GetCurrentBoson();

  // bit mask for the case all the cuts passed, like: (ncuts == 3) ==> all_bits_1 = 0...0111
  //unsigned int all_bits_1 = all_bits_1_lookup(CutWeights.size());

  for (Int_t i = 0; i < Bosons->GetEntriesFast(); i++) {
    ZeeDBosonZ* boson = static_cast<ZeeDBosonZ*>(Bosons->At(i));
//    Event->SetCurrentBoson(boson);

//   this->evaluate(Event, &Mask);  // Get bit mask
//    if (all_bits_1 == Mask.GetMask()) {
      bosonArray.Add(const_cast<ZeeDBosonZ*>(boson));
    }

  // restore current boson
 // Event->SetCurrentBoson(currentBosonSafe);

  return bosonArray;
}

//------------------------------------------------------
ZeeDJet* ZeeDFinder::SelectBestJet(ZeeDEvent* Event,
                                        TObjArray* Jets)
{
    
    Int_t BestJet= 0;
    Double_t BestVal =-1;
    ZeeDCutBit Mask;

    const ZeeDJet* currentJetSafe = Event->GetCurrentJet();

    for(Int_t i=0; i < Jets->GetEntriesFast(); i++){

        ZeeDJet* jet = static_cast<ZeeDJet*>(Jets->At(i));
        Event->SetCurrentJet(jet);

        this->evaluate(Event, &Mask); //Get bit mask
        Double_t val = this->GetWeight(&Mask);//GetWeight

        if(val > BestVal){
            BestVal = val;
            BestJet = i;
        }
    }
    Event->SetCurrentJet(currentJetSafe);

    return static_cast<ZeeDJet*>(Jets->At(BestJet));
}

//------------------------------------------------------
unsigned int ZeeDFinder::all_bits_1_lookup(unsigned int ncuts) {

  // FIXME: for the bit masks types with fixed number of bits
  // (like "int32_t", not just an "unsigned int") _should_be_ used

  static const unsigned int lookup_table[32] = {0, 1, 3, 7, 15,
   						31, 63, 127, 255, 511,
  						1023, 2047, 4095, 8191, 16383,
  						32767, 65535, 131071, 262143, 524287,
  						1048575, 2097151, 4194303, 8388607, 16777215,
  						33554431, 67108863, 134217727, 268435455, 536870911,
  						1073741823, 2147483647};
  if (ncuts < 32) return lookup_table[ncuts];

  unsigned int all_bits_1 = 0;
  for (unsigned int bit = 0; bit < ncuts; ++bit) {all_bits_1 += (1<<bit);}
  return all_bits_1;

}

