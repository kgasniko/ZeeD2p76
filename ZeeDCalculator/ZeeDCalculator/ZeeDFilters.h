#pragma once

#include <TObject.h>
#include <TObjArray.h>
#include "ZeeDEvent/ZeeDElectron.h"

namespace ZeeDFilters {

  const TObjArray* filter(const TObjArray* elements, bool (*the_filter)(const TObject*));

  bool electronFilter(const TObject* electron);

}
