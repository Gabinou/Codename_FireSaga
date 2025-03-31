#ifndef UNIT_FLAGS_H
#define UNIT_FLAGS_H

#include <math.h>
#include "types.h"
#include "enums.h"
#include "structs.h"

// TODO: toggle functions for flags
b32 Unit_Sex(Unit *u);
b32 Unit_isAlive(Unit *u);
b32 Unit_isWaiting(Unit *u);
b32 Unit_showsDanger(Unit *u);
b32 Unit_isUpdateStats(Unit *u);
b32 Unit_isDivineShield(Unit *u);
b32 Unit_isDualWielding(Unit *u);

#endif /* UNIT_H */
