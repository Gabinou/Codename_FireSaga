
#include "unit/flags.h"
#include "unit/unit.h"

b32 Unit_Sex(Unit *unit) {
    return (unit->flags.sex);
}
b32 Unit_isAlive(Unit *unit) {
    return (unit->flags.alive);
}
b32 Unit_isWaiting(Unit *unit) {
    return (unit->flags.waits);
}
b32 Unit_showsDanger(Unit *unit) {
    return (unit->flags.show_danger);
}
b32 Unit_isDivineShield(Unit *unit) {
    return (unit->flags.divine_shield);
}
b32 Unit_isDualWielding(Unit *unit) {
    // Todo: replace flag with check
    return (unit->flags.isDualWielding);
}
b32 Unit_isUpdateStats(Unit *unit) {
    return (unit->flags.update_stats);
}
