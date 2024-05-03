
#include "unit/bonus.h"

/* Instant aura: You only get it if you're in range at all times.
 * - Need to be called for all units every unit move
 */
void Unit_Bonus_Instant_Decay(struct Unit *unit) {
    /* Any aura/bonus with value <= AURA_REMOVE_ON_MOVE gets removed */
    SDL_assert(unit                 != NULL);
    SDL_assert(unit->bonus_stack    != NULL);

    size_t i = 0;
    while (i < DARR_NUM(unit->bonus_stack)) {
        if (unit->bonus_stack[i].turns <= AURA_REMOVE_ON_MOVE) {
            DARR_DEL(unit->bonus_stack, i);
            continue;
        }
        i++;
    }
}

/* Persistent aura: Only gets removed at turn end, even if it lasts 0 turns.
 * - Need to be called for all units every end turn
*/
void Unit_Bonus_Persistent_Decay(struct Unit *unit) {
    /*  1. Any bonus with value == AURA_REMOVE_ON_TURN_END gets removed
        2. Any bonus with value  > AURA_REMOVE_ON_TURN_END gets decremented
    */

    SDL_assert(unit                 != NULL);
    SDL_assert(unit->bonus_stack    != NULL);

    size_t i = 0;
    while (i < DARR_NUM(unit->bonus_stack)) {
        if (unit->bonus_stack[i].turns == AURA_REMOVE_ON_TURN_END) {
            DARR_DEL(unit->bonus_stack, i);
            continue;
        }

        if (unit->bonus_stack[i].turns > AURA_REMOVE_ON_TURN_END) {
            unit->bonus_stack[i].turns--;
        }
        i++;
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
    for (int i = 0; i < DARR_NUM(unit->bonus_stack); i++) {
        if (Bonus_Stats_Compare(unit->bonus_stack[i], bonus)) {
            /* Refresh */
            // found = true;
            unit->bonus_stack[i].turns = bonus.turns;
            return;
        }
    }

    Unit_Bonus_Add(unit, bonus);
}

/* --- Supports --- */
struct Bonus_Stats Unit_supportBonus(struct Unit *unit) {
    /* Support bonus given to other unit if in support range (2 tiles) */
    /* TODO Find all bonuses from supports */
    return (Bonus_Stats_default);
}
