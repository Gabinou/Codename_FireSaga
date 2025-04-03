
#include "unit/bonus.h"
#include "nmath.h"

void Unit_Bonus_Instant_Decay(struct Unit *unit) {
    /* Any aura/bonus with value <= AURA_REMOVE_ON_MOVE gets removed */
    SDL_assert(unit                 != NULL);
    SDL_assert(unit->stats.bonus_stack    != NULL);

    size_t i = 0;
    while (i < DARR_NUM(unit->stats.bonus_stack)) {
        if (unit->stats.bonus_stack[i].turns <= AURA_REMOVE_ON_MOVE) {
            DARR_DEL(unit->stats.bonus_stack, i);
            continue;
        }
        i++;
    }
}

void Unit_Bonus_Persistent_Decay(struct Unit *unit) {
    /*  1. Any bonus with value == AURA_REMOVE_ON_TURN_END gets removed
        2. Any bonus with value  > AURA_REMOVE_ON_TURN_END gets decremented
    */

    SDL_assert(unit                 != NULL);
    SDL_assert(unit->stats.bonus_stack    != NULL);

    size_t i = 0;
    while (i < DARR_NUM(unit->stats.bonus_stack)) {
        if (unit->stats.bonus_stack[i].turns == AURA_REMOVE_ON_TURN_END) {
            DARR_DEL(unit->stats.bonus_stack, i);
            continue;
        }

        if (unit->stats.bonus_stack[i].turns > AURA_REMOVE_ON_TURN_END) {
            unit->stats.bonus_stack[i].turns--;
        }
        i++;
    }
}

struct Bonus_Stats Aura2Bonus(struct Aura *aura, tnecs_entity unit, u16 item, u16 skill,
                              b32 active, b32 instant) {
    struct Bonus_Stats bonus;
    bonus.unit_stats        = aura->unit_stats;
    bonus.computed_stats    = aura->computed_stats;
    bonus.range             = aura->range;
    bonus.turns             = instant ? AURA_REMOVE_ON_MOVE : aura->turns;
    bonus.source_unit       = unit;
    bonus.source_item       = item;
    bonus.source_skill      = skill;
    bonus.active            = active;
    return (bonus);
}

b32 Bonus_Stats_isEqual(struct Bonus_Stats bonus1, struct Bonus_Stats bonus2) {
    b32 out = true;
    out &= (bonus1.source_unit  == bonus2.source_unit);
    out &= (bonus1.source_item  == bonus2.source_item);
    out &= (bonus1.source_skill == bonus2.source_skill);
    out &= (bonus1.active       == bonus2.active);
    return (out);
}

void Unit_Bonus_Add(struct Unit *unit, struct Bonus_Stats bonus) {
    SDL_assert(unit                 != NULL);
    SDL_assert(unit->stats.bonus_stack    != NULL);
    DARR_PUT(unit->stats.bonus_stack, bonus);
}

void Unit_Bonus_Refresh(struct Unit *unit, struct Bonus_Stats bonus) {
    SDL_assert(unit != NULL);
    for (int i = 0; i < DARR_NUM(unit->stats.bonus_stack); i++) {
        if (Bonus_Stats_isEqual(unit->stats.bonus_stack[i], bonus)) {
            /* Refresh */
            // found = true;
            unit->stats.bonus_stack[i].turns = bonus.turns;
            return;
        }
    }

    Unit_Bonus_Add(unit, bonus);
}

/* --- Supports --- */
struct Bonus_Stats Unit_supportBonus(struct Unit *unit) {
    /* Support bonus given to other unit if in support range (2 tiles) */
    /* TODO Find all bonuses from supports */
    /* TODO Total support bonus depends on both units? */
    return (Bonus_Stats_default);
}
