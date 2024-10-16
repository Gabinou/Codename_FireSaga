
#include "unit/loadout.h"

/* --- Transformation --- */
b32 equipped_valid(i32 equipped) {
    return ((equipped >= ITEM1) && (equipped <= ITEM6));
}

b32 eq_valid(i32 eq) {
    return ((eq >= ITEM1) && (eq <= ITEM6));
}

/* --- Loadout --- */
void Loadout_Set(Loadout *loadout, i32 hand, i32 eq) {
    SDL_assert(hand >= UNIT_HAND_LEFT);
    SDL_assert(hand <= MAX_ARMS_NUM);
    SDL_assert(eq   >= ITEM_UNEQUIPPED);
    SDL_assert(eq   <= ITEM6);
    loadout->_loadout[hand - UNIT_HAND_LEFT] = eq;
}

i32 Loadout_Eq(Loadout *loadout, i32 hand) {
    SDL_assert(hand >= UNIT_HAND_LEFT);
    SDL_assert(hand <= MAX_ARMS_NUM);
    return (loadout->_loadout[hand - UNIT_HAND_LEFT]);
}

b32 Loadout_isEquipped(Loadout *loadout, i32 hand) {
    SDL_assert(hand >= UNIT_HAND_LEFT);
    SDL_assert(hand <= MAX_ARMS_NUM);
    return (loadout->_loadout[hand - UNIT_HAND_LEFT] > ITEM_UNEQUIPPED);
}

void Loadout_None(Loadout *loadout, i32 hand) {
    SDL_assert(hand >= UNIT_HAND_LEFT);
    SDL_assert(hand <= MAX_ARMS_NUM);
    loadout->_loadout[hand - UNIT_HAND_LEFT] = ITEM_UNEQUIPPED;
}

/* --- canEquip --- */
void canEquip_Eq(canEquip *can_equip, i32 eq) {
    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);
    can_equip->_eq = eq;
}

void canEquip_Loadout(canEquip *can_equip, i32 hand, i32 eq) {
    SDL_assert(hand >= UNIT_HAND_LEFT);
    SDL_assert(hand <= MAX_ARMS_NUM);
    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);
    can_equip->_loadout[hand - UNIT_HAND_LEFT] = eq;
}

void canEquip_Loadout_None(canEquip *can_equip, i32 hand) {
    SDL_assert(hand >= UNIT_HAND_LEFT);
    SDL_assert(hand <= MAX_ARMS_NUM);
    can_equip->_loadout[hand - UNIT_HAND_LEFT] = ITEM_UNEQUIPPED;
}

/* --- Importing/Exporting --- */
/* Importing and exporting equipped for wloadout functions */

void Unit_Equipped_Import(Unit *unit, i32 *_loadout) {
    size_t bytesize = unit->arms_num * sizeof(*_loadout);
    memcpy(unit->_equipped, _loadout, bytesize);
}

void Unit_Equipped_Export(Unit *unit, i32 *_loadout) {
    size_t bytesize = unit->arms_num * sizeof(*_loadout);
    memcpy(_loadout, unit->_equipped, bytesize);
}

/* Unit <- Loadout */
void Unit_Loadout_Import(Unit *unit, Loadout *loadout) {
    Unit_Equipped_Import(unit, loadout->_loadout);
}

/* Unit -> Loadout */
void Unit_Loadout_Export(Unit *unit, Loadout *loadout) {
    Unit_Equipped_Export(unit, loadout->_loadout);
}

/* Importing and exporting equipment */
void Unit_Equipment_Import(struct Unit *unit, struct Inventory_item *equipment) {
    Equipment_Copy(unit->_equipment, equipment, SOTA_EQUIPMENT_SIZE);
}

void Unit_Equipment_Export(struct Unit *unit, struct Inventory_item *equipment) {
    Equipment_Copy(equipment, unit->_equipment, SOTA_EQUIPMENT_SIZE);
}
