
#include "menu/staff_select.h"

struct Point ssm_cursor_pos[LSM_ELEMS_NUM] = {
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

struct MenuElemDirections ssm_links[LSM_ELEMS_NUM] = {
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
    SDL_assert(ssm->unit > TNECS_NULL);

    struct Unit *unit = TNECS_GET_COMPONENT(ssm->world, ssm->unit, Unit);
    return (Unit_canStaff_oneHand(unit));
}

void StaffSelectMenu_Switch_Items(struct  LoadoutSelectMenu *ssm) {
    ssm->update     = true;
}

void StaffSelectMenu_Switch_Staves(struct LoadoutSelectMenu *ssm) {
    ssm->update     = true;
}

void StaffSelectMenu_Select(struct LoadoutSelectMenu *ssm, i32 select) {
    SDL_assert(ssm          != NULL);
    SDL_assert(ssm->unit    > TNECS_NULL);
    SDL_assert(ssm->world   != NULL);
    /* Player just selected loadout. */

    /* Note: select is in strong space: stronghandd first hand */
    /* - Equip weapons according to player choice - */
    Unit *unit      = TNECS_GET_COMPONENT(ssm->world, ssm->unit, Unit);
    i32 eq          = unit->eq_canEquip[select];
    i32 stronghand  = Unit_Hand_Strong(unit);
    i32 weakhand    = Unit_Hand_Strong(unit);

    // If stronghand is unselected, there should be usable weapons
    if (!Loadout_isEquipped(&ssm->selected, stronghand)) {
        SDL_assert(select < unit->num_canEquip);
    }

    // TODO: selection if one hand skill exists
    if (!Loadout_isEquipped(&ssm->selected, stronghand)) {
        // Equipping staff in both hands
        Loadout_Set(&ssm->selected, stronghand,  eq);
        Loadout_Set(&ssm->selected, weakhand,    eq);
        Unit_Equip(unit, stronghand, eq);
        Unit_Equip(unit, weakhand, eq);
    } else {
        /* - Both Hands already selected - */
        SDL_Log("Both weapons already selected, but select sent to LoadoutSelectMenu");
        SDL_assert(false);      /* For  debug   */
        exit(ERROR_Generic);    /* For release  */
    }
    ssm->update = true;
}
