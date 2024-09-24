
#include "unit/loadout.h"

/* --- Transformation --- */
// ITEM_EQUIPPED_DIFF should be used only HERE.
i32 eq2equipped(i32 eq) {
    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);
    return (eq + ITEM_EQUIPPED_DIFF);
}

i32 equipped2eq(i32 equipped) {
    SDL_assert(equipped >= ITEM1_EQUIPPED);
    SDL_assert(equipped <= ITEM1_EQUIPPED);
    return (equipped - ITEM_EQUIPPED_DIFF);
}

/* --- Loadout --- */
void Loadout_Set(Loadout *loadout, i32 hand, i32 eq) {
    SDL_assert(hand >= 0);
    SDL_assert(hand < MAX_ARMS_NUM);
    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);
    loadout->_loadout[hand] = eq2equipped(eq);
}

i32 Loadout_Eq(Loadout *loadout, i32 hand) {
    SDL_assert(hand >= 0);
    SDL_assert(hand < MAX_ARMS_NUM);
    return (equipped2eq(loadout->_loadout[hand]));
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

/* --- canEquips --- */
void canEquip_Eq(canEquip *can_equip, i32 eq) {
    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);
    can_equip->_eq = eq2equipped(eq);
}

void canEquip_Loadout(canEquip *can_equip, i32 hand, i32 eq) {
    SDL_assert(hand >= 0);
    SDL_assert(hand < MAX_ARMS_NUM);
    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);
    can_equip->_loadout[hand] = eq2equipped(eq);
}

void canEquip_Loadout_None(canEquip *can_equip, i32 hand) {
    SDL_assert(hand >= 0);
    SDL_assert(hand < MAX_ARMS_NUM);
    can_equip->_loadout[hand] = ITEM_UNEQUIPPED;
}
