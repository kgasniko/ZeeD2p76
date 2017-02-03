#include "ZeeDROOTInterface/ZeeDROOTPhoton.h"
#include "ZeeDEvent/ZeeDPhoton.h"

ZeeDROOTPhoton::ZeeDROOTPhoton() :
  fFourVector(0,0,0,0),
  fCollectionIndex(-1)
{}

ZeeDROOTPhoton::~ZeeDROOTPhoton()
{
}

Int_t ZeeDROOTPhoton::fillIn (const ZeeDPhoton* const photon)
{

    // Fills ZeeDROOTPhoton with the information from ZeeDPhoton
  fFourVector                 = photon->GetFourVector();
  fCollectionIndex            = photon->GetCollectionIndex();
  return 0;
}

//------------------------------------------------------
Int_t ZeeDROOTPhoton::fillOut(ZeeDPhoton* const photon) const
{
    // Fills ZeeDPhoton with the information from ZeeDROOTPhoton
  photon->SetFourVector   ( fFourVector );
  photon->GetCollectionIndex()  = fCollectionIndex ;

  return 0;
}
