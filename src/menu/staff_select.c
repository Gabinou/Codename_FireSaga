
#include "menu/staff_select.h"

struct Point ssm_cursor_pos[WSM_ELEMS_NUM] = {
    /* SSM_ELEM_ITEM1 */ {WSM1_X_OFFSET, WSM1_Y_OFFSET},
    /* SSM_ELEM_ITEM2 */ {WSM2_X_OFFSET, WSM2_Y_OFFSET},
    /* SSM_ELEM_ITEM3 */ {WSM3_X_OFFSET, WSM3_Y_OFFSET},
    /* SSM_ELEM_ITEM4 */ {WSM4_X_OFFSET, WSM4_Y_OFFSET},
    /* SSM_ELEM_ITEM5 */ {WSM5_X_OFFSET, WSM5_Y_OFFSET},
    /* SSM_ELEM_ITEM6 */ {WSM6_X_OFFSET, WSM6_Y_OFFSET},
};

struct Point ssm_elem_box[WSM_ELEMS_NUM] = {
    /* SSM_ELEM_ITEM1 */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SSM_ELEM_ITEM2 */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SSM_ELEM_ITEM3 */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SSM_ELEM_ITEM4 */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SSM_ELEM_ITEM5 */ {SOTA_TILESIZE, SOTA_TILESIZE},
    /* SSM_ELEM_ITEM6 */ {SOTA_TILESIZE, SOTA_TILESIZE},
};

struct Point ssm_elem_pos[WSM_ELEMS_NUM] = {
    /* SSM_ELEM_ITEM1 */ {WSM1_X_OFFSET, WSM1_Y_OFFSET},
    /* SSM_ELEM_ITEM2 */ {WSM2_X_OFFSET, WSM2_Y_OFFSET},
    /* SSM_ELEM_ITEM3 */ {WSM3_X_OFFSET, WSM3_Y_OFFSET},
    /* SSM_ELEM_ITEM4 */ {WSM4_X_OFFSET, WSM4_Y_OFFSET},
    /* SSM_ELEM_ITEM5 */ {WSM5_X_OFFSET, WSM5_Y_OFFSET},
    /* SSM_ELEM_ITEM6 */ {WSM6_X_OFFSET, WSM6_Y_OFFSET},
};

struct MenuElemDirections ssm_links[WSM_ELEMS_NUM] = {
    /*right, top, left, bottom */
    /* SSM_ELEM_ITEM1 */ {WSM_ELEM_NULL, WSM_ELEM_NULL,  WSM_ELEM_NULL, WSM_ELEM_ITEM2},
    /* SSM_ELEM_ITEM2 */ {WSM_ELEM_NULL, WSM_ELEM_ITEM1, WSM_ELEM_NULL, WSM_ELEM_ITEM3},
    /* SSM_ELEM_ITEM3 */ {WSM_ELEM_NULL, WSM_ELEM_ITEM2, WSM_ELEM_NULL, WSM_ELEM_ITEM4},
    /* SSM_ELEM_ITEM4 */ {WSM_ELEM_NULL, WSM_ELEM_ITEM3, WSM_ELEM_NULL, WSM_ELEM_ITEM5},
    /* SSM_ELEM_ITEM5 */ {WSM_ELEM_NULL, WSM_ELEM_ITEM4, WSM_ELEM_NULL, WSM_ELEM_ITEM6},
    /* SSM_ELEM_ITEM6 */ {WSM_ELEM_NULL, WSM_ELEM_ITEM5, WSM_ELEM_NULL, WSM_ELEM_NULL},
};

bool StaffSelectMenu_canEqItem(struct LoadoutSelectMenu *ssm) {
    return (Unit_canStaff_oneHand(ssm->unit));
}

void StaffSelectMenu_Switch_Items(struct  LoadoutSelectMenu *ssm) {
    ssm->update     = true;
}

void StaffSelectMenu_Switch_Staves(struct LoadoutSelectMenu *ssm) {
    ssm->update     = true;
}
