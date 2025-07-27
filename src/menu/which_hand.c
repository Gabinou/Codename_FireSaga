
#include "menu/which_hand.h"
#include "unit/unit.h"
#include "item.h"

/* --- ELEMENTS --- */
MenuElemDirections whm_links[WH_ELEM_NUM] = {
    /*right, top, left, bottom */
    /* HAND1 */ {WH_ELEM_NULL,  WH_ELEM_NULL,
                 WH_ELEM_NULL,  WH_ELEM_HAND2},
    /* HAND2 */ {WH_ELEM_NULL,  WH_ELEM_NULL,
                 WH_ELEM_HAND1, WH_ELEM_HAND3},
    /* HAND3 */ {WH_ELEM_NULL,  WH_ELEM_NULL,
                 WH_ELEM_HAND2, WH_ELEM_NULL},
}

Point whm_elem_pos[WH_ELEM_NUM] = {
    /* HAND1 */  {8,  21},
    /* HAND1 */  {8,  39},
    /* HAND1 */  {8,  57},
}

Point whm_elem_box[WH_ELEM_NUM] = {
    /* HAND1 */  {SOTA_TILESIZE, SOTA_TILESIZE},
    /* HAND2 */  {SOTA_TILESIZE, SOTA_TILESIZE},
    /* HAND3 */  {SOTA_TILESIZE, SOTA_TILESIZE},
}

i32 WhichHandMenu_Select(struct WhichHandMenu *whm,
                         i32 elem) {
    /* Player selects hand from list of menu elements */

    SDL_assert(elem < whm->num_handedness);
    /* Ouput Hand from present elems
    ** 1. Some items are one hand only, L or R only...
    ** 2. Some items can't be two handed
    ** 3. Some units have onbe hand missing */
    return (whm->handedness[elem]);
}

void WhichHandMenu_Elements(struct WhichHandMenu   *whm,
                            struct Unit            *unit,
                            struct Item            *item) {
    /* Build list of menu elements from
    **  1. Weapon handedness
    **  2. Unit hands */
    whm->num_handedness = 0;

    /* -- Unit hands -- */
    b32 unitL   = Unit_hasHand(unit, UNIT_HAND_LEFT);
    b32 unitR   = Unit_hasHand(unit, UNIT_HAND_LEFT);

    /* -- Item handedness -- */
    b32 wpnL    = (Item_Handedness(item) == WEAPON_HAND_LEFT);
    b32 wpnR    = (Item_Handedness(item) == WEAPON_HAND_RIGHT);
    b32 wpn1H   = (Item_Handedness(item) == WEAPON_HAND_ONE);
    b32 wpn2H   = (Item_Handedness(item) == WEAPON_HAND_TWO);
    b32 wpnAny  = (Item_Handedness(item) == WEAPON_HAND_ANY);

    /* -- Can item be equipped in Left hand only? -- */
    if (unitL && (wpn1H || wpnL || wpnAny)) {
        whm->handedness[whm->num_handedness++] = UNIT_EQUIP_LEFT;
    }

    /* -- Can item be equipped in Right hand only? -- */
    if (unitR && (wpn1H || wpnR || wpnAny)) {
        whm->handedness[whm->num_handedness++] = UNIT_EQUIP_RIGHT;
    }

    /* -- Can item be equipped in two hands? -- */
    if (unitR && unitL && (wpn2H || wpnAny)) {
        whm->handedness[whm->num_handedness++] = UNIT_EQUIP_TWO_HANDS;
    }
}

void DeploymentMenu_Elem_Links(DeploymentMenu *dm,
                               struct Menu *mc) {
    /* Get number of elements for the menu */

    mc->elem_num    = whm->num_handedness;
    mc->elem_links  = whm_links;

    /* Remove links if elems are disabled. */
    if (mc->elem_num <= 2) {
        memset(mc->elem_links[2], 0, bytesize); 
    } 

    if (mc->elem_num <= 1) {
        memset(mc->elem_links[1], 0, bytesize); 
    } 
}


void WhichHandMenu_Draw(struct Menu     *mc,
                        SDL_Texture     *rt,
                        SDL_Renderer    *r) {

}

void WhichHandMenu_Update(struct WhichHandMenu  *whm,
                          struct n9Patch         *n9,
                          SDL_Texture            *rt,
                          SDL_Renderer           *r) {

}





