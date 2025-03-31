
#include "unit/flags.h"
#include "unit/unit.h"

b32 Unit_isAlive(Unit *unit) {
    return(unit->flags.alive);
}
b32 Unit_isWaiting(Unit *u) {
    return(unit->flags.waits);
}

b32 Unit_isDivineShield(Unit *unit) {
    return(unit->flags.divine_shield);
}
