
#include "unit/aura.h"

void Unit_Bonus_Decay(struct Unit *unit) {
    SDL_assert(unit                 != NULL);
    SDL_assert(unit->bonus_stack    != NULL);

    size_t i = 0;
    while (i < DARR_NUM(unit->bonus_stack)) {
        unit->bonus_stack[i].turns--;
        if (unit->bonus_stack[i].turns == 0)
            DARR_DEL(unit->bonus_stack, i);
        else
            i++;
    }
}

