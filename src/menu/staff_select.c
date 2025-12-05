
#include "menu/staff_select.h"
#include "unit/unit.h"
#include "globals.h"
#include "unit/equipment.h"
#include "unit/loadout.h"

const struct Point ssm_cursor_pos[LSM_ELEMS_NUM] = {
    /* SSM_ELEM_ITEM1 */ {LSM1_X_OFFSET, LSM1_Y_OFFSET},
    /* SSM_ELEM_ITEM2 */ {LSM2_X_OFFSET, LSM2_Y_OFFSET},
    /* SSM_ELEM_ITEM3 */ {LSM3_X_OFFSET, LSM3_Y_OFFSET},
    /* SSM_ELEM_ITEM4 */ {LSM4_X_OFFSET, LSM4_Y_OFFSET},
    /* SSM_ELEM_ITEM5 */ {LSM5_X_OFFSET, LSM5_Y_OFFSET},
    /* SSM_ELEM_ITEM6 */ {LSM6_X_OFFSET, LSM6_Y_OFFSET},
};

struct Point ssm_elem_box[LSM_ELEMS_NUM] = {
    /* SSM_ELEM_ITEM1 */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SSM_ELEM_ITEM2 */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SSM_ELEM_ITEM3 */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SSM_ELEM_ITEM4 */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SSM_ELEM_ITEM5 */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SSM_ELEM_ITEM6 */ {SOTA_TILESIZE, SOTA_TILESIZE},
};

struct Point ssm_elem_pos[LSM_ELEMS_NUM] = {
    /* SSM_ELEM_ITEM1 */ {LSM1_X_OFFSET, LSM1_Y_OFFSET},
    /* SSM_ELEM_ITEM2 */ {LSM2_X_OFFSET, LSM2_Y_OFFSET},
    /* SSM_ELEM_ITEM3 */ {LSM3_X_OFFSET, LSM3_Y_OFFSET},
    /* SSM_ELEM_ITEM4 */ {LSM4_X_OFFSET, LSM4_Y_OFFSET},
    /* SSM_ELEM_ITEM5 */ {LSM5_X_OFFSET, LSM5_Y_OFFSET},
    /* SSM_ELEM_ITEM6 */ {LSM6_X_OFFSET, LSM6_Y_OFFSET},
};

struct n4Directions ssm_links[LSM_ELEMS_NUM] = {
    /*right, top, left, bottom */
    /* SSM_ELEM_ITEM1 */ {LSM_ELEM_NULL, LSM_ELEM_NULL,  LSM_ELEM_NULL, LSM_ELEM_ITEM2},
    /* SSM_ELEM_ITEM2 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM1, LSM_ELEM_NULL, LSM_ELEM_ITEM3},
    /* SSM_ELEM_ITEM3 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM2, LSM_ELEM_NULL, LSM_ELEM_ITEM4},
    /* SSM_ELEM_ITEM4 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM3, LSM_ELEM_NULL, LSM_ELEM_ITEM5},
    /* SSM_ELEM_ITEM5 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM4, LSM_ELEM_NULL, LSM_ELEM_ITEM6},
    /* SSM_ELEM_ITEM6 */ {LSM_ELEM_NULL, LSM_ELEM_ITEM5, LSM_ELEM_NULL, LSM_ELEM_NULL},
};

b32 StaffSelectMenu_canEqItem(struct LoadoutSelectMenu *ssm) {
    SDL_assert(ssm != NULL);
    SDL_assert(ssm->_unit > TNECS_NULL);

    struct Unit *unit = IES_GET_C(gl_world, ssm->_unit, Unit);
    return (Unit_canStaff_oneHand(unit));
}

void StaffSelectMenu_Switch_Items(struct  LoadoutSelectMenu *ssm) {
    ssm->update     = true;
}

void StaffSelectMenu_Switch_Staves(struct LoadoutSelectMenu *ssm) {
    ssm->update     = true;
}

void StaffSelectMenu_Unit(  struct LoadoutSelectMenu *ssm,
                            tnecs_E ent) {
    LoadoutSelectMenu_Unit(ssm, ent);
    /* TODO: just find all staves and put in equippable */
    /* ssm->equippable */
}

void StaffSelectMenu_Select(struct LoadoutSelectMenu *ssm, i32 select) {
    SDL_assert(ssm          != NULL);
    SDL_assert(ssm->_unit    > TNECS_NULL);
    SDL_assert(gl_world     != NULL);
    SDL_assert(select       >= ITEM_NULL);
    SDL_assert(select       < EQM_SIZE);
    /* Player just selected loadout. */

    /* Note: select is in strong space: stronghandd first hand */
    /* - Equip staff according to player choice - */
    Unit *unit      = IES_GET_C(gl_world, ssm->_unit, Unit);
    i32 eq          = ssm->equippable.arr[select];
    i32 stronghand  = Unit_Hand_Strong(unit);
    i32 weakhand    = Unit_Hand_Weak(unit);
    SDL_assert(eq >= ITEM1);
    SDL_assert(eq <= ITEM6);

    // TODO: selection if one hand skill exists
    Loadout_Set(&ssm->selected, weakhand, eq);
    Loadout_Set(&ssm->selected, stronghand, eq);
    Unit_Equip(unit, stronghand,    eq);
    Unit_Equip(unit, weakhand,      eq);
    SDL_assert(Unit_isEquipped(unit, stronghand));
    SDL_assert(Unit_isEquipped(unit, weakhand));

    ssm->update = true;
}
