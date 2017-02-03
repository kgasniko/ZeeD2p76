#include "ZeeDCalculator/ZeeDFilters.h"

const TObjArray* ZeeDFilters::filter(const TObjArray* elements, bool (*the_filter)(const TObject*))
{

  TObjArray* output = new TObjArray;

  for (int ielem = 0; ielem < elements->GetEntriesFast(); ++ielem) {

    TObject* obj = elements->At(ielem);
    if (the_filter(obj)) output->Add(obj);
  }

  return output;
}

bool ZeeDFilters::electronFilter(const TObject* electron) {

  // Apply cuts and copy pointer if the electron passes it 
  const ZeeDElectron* elec = static_cast<const ZeeDElectron*>(electron);
  if (elec->GetFourVector().Pt() > 15.) return true;
  // ... and some other basic cuts

  return false;
}


