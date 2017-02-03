#include "ZeeDAnalysis/ZeeDLauncher.h"
#include "ZeeDAnalysis/ZeeDPlotter.h"

#include "GaudiKernel/DeclareFactoryEntries.h"

DECLARE_ALGORITHM_FACTORY( ZeeDLauncher )
DECLARE_ALGORITHM_FACTORY( ZeeDPlotter )

DECLARE_FACTORY_ENTRIES( ZeeDAnalysis )
{
    DECLARE_ALGORITHM( ZeeDLauncher )
    DECLARE_ALGORITHM( ZeeDPlotter )
}
