#ifndef SSM_H
#define SSM_H

#include "menu/loadout_select.h"

/* DESIGN RULES:
* One click for staff unless SKILL
*   - Two clicks if have Skills
*   - Staff has to be in strong hand to be used
*/

/* Staff selection flow: 2 clicks max
*   1. Choose Staff (only staves in menu)
*      a. One handing staves: Staff goes into strong hands
*      b. Two handing staves: Staff goes into two    hands
*   2. Choose second hand item, any item
*/

enum SSM_ELEMS {
    SSM_ELEM_NULL   = -1,
    SSM_ELEM_ITEM1,
    SSM_ELEM_ITEM2,
    SSM_ELEM_ITEM3,
    SSM_ELEM_ITEM4,
    SSM_ELEM_ITEM5,
    SSM_ELEM_ITEM6,
    SSM_ELEMS_NUM,
};

/* --- FORWARD DECLARATIONS --- */
struct LoadoutSelectMenu;

/* --- Item placement --- */
/* After selecting staff, if unit can one hand staves, menu mode staves->items */
void StaffSelectMenu_Select(        struct LoadoutSelectMenu *ssm, i32 select);
void StaffSelectMenu_Deselect(      struct LoadoutSelectMenu *ssm);
b32 StaffSelectMenu_canEqItem(      struct LoadoutSelectMenu *ssm);
void StaffSelectMenu_Switch_Items(  struct LoadoutSelectMenu *ssm);
void StaffSelectMenu_Switch_Staves( struct LoadoutSelectMenu *ssm);

void StaffSelectMenu_Unit(  struct LoadoutSelectMenu *sm,
                            tnecs_E ent);


extern struct n4Directions    ssm_links[SSM_ELEMS_NUM];
extern struct Point   ssm_elem_pos[SSM_ELEMS_NUM];
extern struct Point   ssm_elem_box[SSM_ELEMS_NUM];
extern const struct Point   ssm_cursor_pos[SSM_ELEMS_NUM];
extern const struct Point   ssm_cursor_box[SSM_ELEMS_NUM];

#endif /* SSM_H */
