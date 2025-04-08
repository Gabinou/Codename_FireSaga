

#include "unit/flags.h"
#include "unit/unit.h"

b32 Unit_Sex(const Unit *unit) {
    if (unit == NULL)
        return (1);
    return (unit->flags.sex);
}
b32 Unit_isAlive(const Unit *unit) {
    if (unit == NULL)
        return (0);
    return (unit->flags.alive);
}
b32 Unit_isWaiting(const Unit *unit) {
    if (unit == NULL)
        return (1);
    return (unit->flags.waits);
}
b32 Unit_showsDanger(const Unit *unit) {
    if (unit == NULL)
        return (0);
    return (unit->flags.show_danger);
}
b32 Unit_isDivineShield(const Unit *unit) {
    if (unit == NULL)
        return (0);
    return (unit->flags.divine_shield);
}
b32 Unit_isDualWielding(const Unit *unit) {
    if (unit == NULL)
        return (0);
    // Todo: replace flag with check
    return (unit->flags.isDualWielding);
}
struct Unit_stats *Unit_Stats_Grown(const Unit *unit) {
    if (unit == NULL)
        return (NULL);
    return (unit->growth.grown);
}

struct Unit_stats *Unit_Stats_Growths(Unit *unit) {
    if (unit == NULL)
        return (NULL);
    return (&unit->growth.rates);
}
u64 Unit_Skills(const Unit *unit) {
    if (unit == NULL)
        return (0);
    return (unit->flags.skills);
}
void Unit_Skill_Add(Unit *unit, u64 skill) {
    if (unit == NULL)
        return;
    unit->flags.skills |= skill;
}
struct dtab *Unit_dtab_Weapons(const Unit *unit) {
    if (unit == NULL)
        return (NULL);
    return (unit->equipment.weapons_dtab);
}
struct dtab *Unit_dtab_Items(const Unit *unit) {
    if (unit == NULL)
        return (NULL);
    return (unit->equipment.items_dtab);
}
i32 Unit_Current_HP(const Unit *unit) {
    if (unit == NULL)
        return (0);
    return (unit->counters.hp);
}
i32 Unit_Current_Agony(const Unit *unit) {
    if (unit == NULL)
        return (0);
    return (unit->counters.agony);
}
i32 Unit_Current_Regrets(const Unit *unit) {
    if (unit == NULL)
        return (0);
    return (unit->counters.regrets);
}
i32 Unit_Army(const Unit *unit) {
    if (unit == NULL)
        return (0);
    return (unit->id.army);
}
void Unit_Army_set(Unit *unit, i32 army) {
    if (unit == NULL)
        return;
    unit->id.army = army;
}
i32 Unit_Class(const Unit *unit) {
    if (unit == NULL)
        return (0);
    return (unit->id.class);
}
void Unit_Class_set(Unit *unit, i32 class) {
    if (unit == NULL)
        return;
    unit->id.class = class;
}
i32 Unit_AI(const Unit *unit) {
    if (unit == NULL)
        return (0);
    return (unit->id.ai);
}
void Unit_AI_set(Unit *unit, i32 ai) {
    if (unit == NULL)
        return;
    unit->id.ai = ai;
}
