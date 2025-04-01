
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
struct Unit_stats *Unit_Stats_Grown(Unit *unit) {
    return (unit->growth.grown);
}

struct Unit_stats *Unit_Stats_Growths(Unit *unit) {
    return (&unit->growth.rates);
}
u64 Unit_Skills(Unit *unit) {
    return(unit->flags.skills);
}
void Unit_Skill_Add(Unit *unit, u64 skill) {
    unit->flags.skills |= skill;
}
struct dtab *Unit_dtab_Weapons(Unit *unit) {
    return(unit->equipment.weapons_dtab);
}
struct dtab *Unit_dtab_Items(Unit *unit) {
    return(unit->equipment.items_dtab);
}
