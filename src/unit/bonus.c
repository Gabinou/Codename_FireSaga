
#include "unit/bonus.h"

void Unit_Bonus_Decay(struct Unit *unit) {
    SDL_assert(unit                 != NULL);
    SDL_assert(unit->bonus_stack    != NULL);

    size_t i = 0;
    while (i < DARR_NUM(unit->bonus_stack)) {
        if (unit->bonus_stack[i].turns == 0) {
            DARR_DEL(unit->bonus_stack, i);
        } else {
            unit->bonus_stack[i].turns--;
            i++;
        }
    }
}

struct Bonus_Stats Aura2Bonus(struct Aura *aura, tnecs_entity unit, u16 item, u16 skill,
                              b32 active) {
    struct Bonus_Stats bonus;
    bonus.unit_stats        = aura->unit_stats;
    bonus.computed_stats    = aura->computed_stats;
    bonus.range             = aura->range;
    bonus.turns             = aura->turns;
    bonus.source_unit       = unit;
    bonus.source_item       = item;
    bonus.source_skill      = skill;
    bonus.active            = active;
    return (bonus);
}

b32 Bonus_Stats_Compare(struct Bonus_Stats bonus1, struct Bonus_Stats bonus2) {
    /* Check if bonus_stats are the same.
        Ostensibly, bonuses don't stack.
    */
    b32 out = true;
    out &= (bonus1.source_unit  == bonus2.source_unit);
    out &= (bonus1.source_item  == bonus2.source_item);
    out &= (bonus1.source_skill == bonus2.source_skill);
    out &= (bonus1.active       == bonus2.active);
    return (out);
}

void Unit_Bonus_Add(struct Unit *unit, struct Bonus_Stats bonus) {
    SDL_assert(unit                 != NULL);
    SDL_assert(unit->bonus_stack    != NULL);
    DARR_PUT(unit->bonus_stack, bonus);
}

void Unit_Bonus_Refresh(struct Unit *unit, struct Bonus_Stats bonus) {
    SDL_assert(unit != NULL);
    /* If bonus with same item_id, unit_ent amd active exists
        - Update turns to bonus turns
        If not, add it to the bonus_stats stack
    */
    int found = -1;
    for (int i = 0; i < DARR_NUM(unit->bonus_stack); i++) {
        if (Bonus_Stats_Compare(unit->bonus_stack[i], bonus)) {
            found = true;
            break;
        }
    }

    if (found >= 0) {
        unit->bonus_stack[found].turns = bonus.turns;
    } else {
        Unit_Bonus_Add(unit, bonus);
    }
}
