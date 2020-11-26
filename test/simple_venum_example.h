#pragma once

#include "venum.h"

// Simple virtual-enum declaration
// Creates an virtual-enum with only constants and no associated values except their ordinals and names.
// The first parameter is the name of the virtual-enum, the rest becomes a list of constants.
VENUM_CREATE(CarType,
    Mercedes,
    Volkswagen,
    RollsRoyce
)
