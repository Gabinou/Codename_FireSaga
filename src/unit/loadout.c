
#include "unit/loadout.h"

void Loadout_Set(Loadout *loadout, i32 hand, i32 eq) {
    SDL_assert(hand >= 0);
    SDL_assert(hand < MAX_ARMS_NUM);
    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);
    loadout->_loadout[hand] = eq + ITEM_EQUIPPED_DIFF;
}

i32 Loadout_Eq(Loadout *loadout, i32 hand) {
    SDL_assert(hand >= 0);
    SDL_assert(hand < MAX_ARMS_NUM);
    return (loadout->_loadout[hand] - ITEM_EQUIPPED_DIFF);
}

i32 Loadout_isEquipped(Loadout *loadout, i32 hand) {
    SDL_assert(hand >= 0);
    SDL_assert(hand < MAX_ARMS_NUM);
    return (loadout->_loadout[hand] > ITEM_UNEQUIPPED);
}

void Loadout_None(Loadout *loadout, i32 hand) {
    SDL_assert(hand >= 0);
    SDL_assert(hand < MAX_ARMS_NUM);
    loadout->_loadout[hand] = ITEM_UNEQUIPPED;
}
