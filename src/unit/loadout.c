
#include "macros.h"
#include "platform.h"
#include "inventory.h"

#include "unit/unit.h"
#include "unit/loadout.h"
#include "unit/equipment.h"

/* --- Transformation --- */
b32 eq_valid(i32 eq) {
    return ((eq >= ITEM1) && (eq <= ITEM6));
}

/* --- Loadout --- */
void Loadout_Set(Loadout *loadout, i32 hand, i32 eq) {
    IES_check(loadout != NULL);
    IES_check(hand >= UNIT_HAND_LEFT);
    IES_check(hand <= MAX_ARMS_NUM);
    if (!eq_valid(eq))
        return;

    loadout->_loadout[hand] = eq;
}

i32 Loadout_Eq(Loadout *loadout, i32 hand) {
    IES_check_ret(loadout != NULL, ITEM1);
    IES_check_ret(hand >= UNIT_HAND_LEFT,   ITEM1);
    IES_check_ret(hand <= MAX_ARMS_NUM,     ITEM1);
    return (loadout->_loadout[hand]);
}

b32 Loadout_isEquipped(Loadout *loadout, i32 hand) {
    IES_check_ret(loadout != NULL,          0);
    IES_check_ret(hand >= UNIT_HAND_LEFT,   0);
    IES_check_ret(hand <= MAX_ARMS_NUM,     0);
    return (eq_valid(loadout->_loadout[hand]));
}

void Loadout_None(Loadout *loadout, i32 hand) {
    IES_check(loadout != NULL);
    IES_check(hand >= UNIT_HAND_LEFT);
    IES_check(hand <= MAX_ARMS_NUM);
    loadout->_loadout[hand] = ITEM_NUM;
}

b32 Loadout_istwoHanding(Loadout *loadout) {
    IES_check_ret(loadout != NULL, 0);
    i32 eq_left     = loadout->_loadout[UNIT_HAND_LEFT];
    i32 eq_right    = loadout->_loadout[UNIT_HAND_RIGHT];
    return (_istwoHanding(eq_left, eq_right));
}

b32 _istwoHanding(i32 eqd_left, i32 eqd_right) {
    if (!eq_valid(eqd_left) || !eq_valid(eqd_right)) {
        return (false);
    }
    return (eqd_left == eqd_right);
}

/* --- canEquip --- */
void canEquip_Eq(canEquip *can_equip, i32 eq) {
    IES_check(can_equip != NULL);
    if (!eq_valid(eq))
        return;
    can_equip->_eq = eq;
}

void canEquip_Loadout(canEquip *can_equip, i32 hand, i32 eq) {
    IES_check(can_equip != NULL);
    IES_check(hand >= UNIT_HAND_LEFT);
    IES_check(hand <= MAX_ARMS_NUM);
    if (!eq_valid(eq))
        return;
    can_equip->_loadout[hand] = eq;
}

void canEquip_Loadout_None(canEquip *can_equip, i32 hand) {
    IES_check(can_equip != NULL);
    IES_check(hand >= UNIT_HAND_LEFT);
    IES_check(hand <= MAX_ARMS_NUM);
    can_equip->_loadout[hand] = ITEM_NUM;
}

/* --- Importing/Exporting --- */
/* Importing and exporting equipped for wloadout functions */
void Loadout_Copy(Loadout *dest, const Loadout *const src) {
    IES_check(dest  != NULL);
    IES_check(src   != NULL);
    Equipped_Copy(dest->_loadout, src->_loadout);
}

void Equipped_Copy(i32 *_dest, const i32 *const _src) {
    IES_check(_dest != NULL);
    IES_check(_src  != NULL);
    size_t bytesize = MAX_ARMS_NUM * sizeof(*_dest);
    memcpy(_dest, _src, bytesize);
}

void Unit_Equipped_Import(Unit *unit, i32 *_loadout) {
    IES_check(unit      != NULL);
    IES_check(_loadout  != NULL);
    size_t bytesize = unit->arms.num * sizeof(*_loadout);
    i32 *equipped = Unit_Equipped_Array(unit);
    memcpy(equipped, _loadout, bytesize);
}

void Unit_Equipped_Export(Unit *unit, i32 *_loadout) {
    IES_check(unit      != NULL);
    IES_check(_loadout  != NULL);
    size_t bytesize = unit->arms.num * sizeof(*_loadout);
    i32 *equipped = Unit_Equipped_Array(unit);
    memcpy(_loadout, equipped, bytesize);
}

/* Unit <- Loadout */
void Unit_Loadout_Import(Unit *unit, Loadout *loadout) {
    IES_check(unit      != NULL);
    IES_check(loadout   != NULL);
    Unit_Equipped_Import(unit, loadout->_loadout);
}

/* Unit -> Loadout */
void Unit_Loadout_Export(Unit *unit, Loadout *loadout) {
    IES_check(unit      != NULL);
    IES_check(loadout   != NULL);
    Unit_Equipped_Export(unit, loadout->_loadout);
}

/* Importing and exporting equipment */
void Unit_Equipment_Import(Unit     *unit,
                           tnecs_E  *equipment) {
    IES_check(unit      != NULL);
    IES_check(equipment != NULL);
    Equipment_Copy( unit->equipment._arr, equipment,
                    EQM_SIZE);
}

void Unit_Equipment_Export(Unit     *unit,
                           tnecs_E  *equipment) {
    IES_check(unit      != NULL);
    IES_check(equipment != NULL);
    Equipment_Copy( equipment, unit->equipment._arr,
                    EQM_SIZE);
}
