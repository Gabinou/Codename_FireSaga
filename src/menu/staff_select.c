
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
    return (Unit_canStaff_oneHand(ssm->unit));
}

void StaffSelectMenu_Switch_Items(struct  LoadoutSelectMenu *ssm) {
    ssm->update     = true;
}

void StaffSelectMenu_Switch_Staves(struct LoadoutSelectMenu *ssm) {
    ssm->update     = true;
}

void StaffSelectMenu_Select(struct LoadoutSelectMenu *ssm, i32 select) {
    /* Player just selected loadout. */

    /* Note: select is in strong space: stronghandd first hand */
    /* - Equip weapons according to player choice - */
    i32 eq          = ssm->unit->eq_usable[select];
    i32 stronghand  = Unit_Hand_Strong(ssm->unit);
    i32 weakhand    = 1 - stronghand;

    // If stronghand is unselected, there should be usable weapons
    if (ssm->selected[stronghand] < 0) {
        SDL_assert(select < ssm->unit->num_usable);
    }

    // TODO: selection if one hand skill exists
    if (ssm->selected[stronghand] < 0) {
        // Equipping staff in both hands
        ssm->selected[stronghand]   = eq;
        ssm->selected[weakhand]     = eq;
        Unit_Equip(ssm->unit, stronghand, eq);
        Unit_Equip(ssm->unit, weakhand, eq);
    } else {
        /* - Both Hands already selected - */
        SDL_Log("Both weapons already selected, but select sent to LoadoutSelectMenu");
        SDL_assert(false);      /* For  debug   */
        exit(ERROR_Generic);    /* For release  */
    }
    ssm->update = true;
}
