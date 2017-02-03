#include "ZeeDExamples/ZeeDArraysExample.h"
#include "ZeeDExamples/ZeeDPhotonExample.h"

#include "GaudiKernel/DeclareFactoryEntries.h"

DECLARE_ALGORITHM_FACTORY( ZeeDArraysExample )
DECLARE_ALGORITHM_FACTORY( ZeeDPhotonExample )

DECLARE_FACTORY_ENTRIES( ZeeDExamples )
{
    DECLARE_ALGORITHM( ZeeDArraysExample )
    DECLARE_ALGORITHM( ZeeDPhotonExample )
}

