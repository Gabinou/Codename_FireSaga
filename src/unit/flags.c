
#include "unit/flags.h"
#include "unit/unit.h"

b32 Unit_Sex(const Unit *unit) {
    return (unit->flags.sex);
}
b32 Unit_isAlive(const Unit *unit) {
    return (unit->flags.alive);
}
b32 Unit_isWaiting(const Unit *unit) {
    return (unit->flags.waits);
}
b32 Unit_showsDanger(const Unit *unit) {
    return (unit->flags.show_danger);
}
b32 Unit_isDivineShield(const Unit *unit) {
    return (unit->flags.divine_shield);
}
b32 Unit_isDualWielding(const Unit *unit) {
    // Todo: replace flag with check
    return (unit->flags.isDualWielding);
}
b32 Unit_isUpdateStats(const Unit *unit) {
    return (unit->flags.update_stats);
}
struct Unit_stats *Unit_Stats_Grown(const Unit *unit) {
    return (unit->growth.grown);
}

struct Unit_stats *Unit_Stats_Growths(Unit *unit) {
    return (&unit->growth.rates);
}
u64 Unit_Skills(const Unit *unit) {
    return (unit->flags.skills);
}
void Unit_Skill_Add(Unit *unit, u64 skill) {
    unit->flags.skills |= skill;
}
struct dtab *Unit_dtab_Weapons(const Unit *unit) {
    return (unit->equipment.weapons_dtab);
}
struct dtab *Unit_dtab_Items(const Unit *unit) {
    return (unit->equipment.items_dtab);
}
i32 Unit_Current_HP(const Unit *unit) {
    return (unit->counters.hp);
}
i32 Unit_Current_Agony(const Unit *unit) {
    return (unit->counters.agony);
}
i32 Unit_Current_Regrets(const Unit *unit) {
    return (unit->counters.regrets);
}
i32 Unit_Army(const Unit *unit) {
    return (Unit_Army(unit));
}
i32 Unit_Class(const Unit *unit) {
    return (unit->id.class);
}
i32 Unit_AI(const Unit *unit) {
    return (unit->id.ai);
}
