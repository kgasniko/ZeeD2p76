
#include "ZeeDHistManager/ZeeDHistManagerZjetAna.h"

// Root includes
#include <TLorentzVector.h>
#include <TObjArray.h>
#include <TFile.h>

// Analysis includes
#include "ZeeDEvent/ZeeDEvent.h"
#include "ZeeDEvent/ZeeDElectron.h"
#include "ZeeDEvent/ZeeDBosonZ.h"
#include "ZeeDEvent/ZeeDJet.h"
#include "ZeeDEvent/ZeeDTrack.h"
#include "ZeeDEvent/ZeeDGenParticle.h"
#include "ZeeDEvent/ZeeDVertex.h"

#include "ZeeDTools/ZeeDBags.h"
#include "ZeeDTools/ZeeDSystematics.h"

#include "ZeeDAnalysisSvc/IZeeDAnalysisSvc.h"


//#define USE_DEBUG_MESSAGES
//#define DEBUG_ON 1
#include "ZeeDMisc/ZeeDMisc.h"

using std::cout;
using std::endl;



//-----------------------------------------------------
void  ZeeDHistManagerZjetAna::bookAnalysisCuts()
{

}

//-----------------------------------------------------
void ZeeDHistManagerZjetAna::BookHistos()
{
  // Do we run on data or MC?
    
      //************************************Subeading Jet***************************************//
}
//-----------------------------------------------------
void ZeeDHistManagerZjetAna::Fill()
{

  }
//-----------------------------------------------------

void ZeeDHistManagerZjetAna::FillJetHists(const TObjArray* jets, Double_t Weight )
{
       
}

// Analysis cuts
Bool_t ZeeDHistManagerZjetAna::passedCuts( ZeeDJet*& selected_jet ){
  //electron isolation 0.35

  return true;
}

// tool for isolation jet cut: no other jets in dR = 0.8
void ZeeDHistManagerZjetAna::JetIsolationCut( ZeeDJet*& selected_jet,  TString collectionName ){
  return;
}

// tool for matching truth jet to reco jet
void ZeeDHistManagerZjetAna::GetMatchedTruth(  ZeeDJet*& reco )
{
  return;
}

// tool for finding the distance between nearest jet which passed all cuts and given jet (nearestJetDR)
void ZeeDHistManagerZjetAna::GetNearestJet( ZeeDJet*& selected_jet,  TString collectionName ){


  return;
}
