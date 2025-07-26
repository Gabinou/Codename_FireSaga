
#include "menu/which_hand.h"

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


void WhichHandMenu_Draw(struct Menu     *mc,
                        SDL_Texture     *rt,
                        SDL_Renderer    *r) {

}

void WhichHandMenu_Update(struct WhichHandMenu  *whm,
                          struct n9Patch         *n9,
                          SDL_Texture            *rt,
                          SDL_Renderer           *r) {

}
