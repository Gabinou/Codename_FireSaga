
#ifndef WHICH_HAND_H
#define WHICH_HAND_H

#include "enums.h"
#include "structs.h"
#include "tnecs.h"

/* --- FORWARD DECLARATIONS --- */
struct n9Patch;
struct Menu;
struct Unit;
struct Item;

/* --- Which hand menu---
** Items Unit action Equip submenu:
** choose hand in which to equip item.
**  Flow:
**  1. Choose *Items* on unit action menu
**  2. Pick item in equipment
**  3. Choose what to do with item
**      1. *Equip* or *Use*
**      ->  - Which hand menu (L, R or 2H)  <- HERE
*/

enum WH_MENU_ELEMENTS {
    WHM_ELEM_NULL  = -1,
    WHM_ELEM_HAND1 =  0,
    WHM_ELEM_HAND2 =  1,
    WHM_ELEM_HAND3 =  2,
    WHM_ELEM_NUM   =  3
};

enum WH_MENU {
    WHM_WIDTH           =  0,
    WHM_PATCH_X_SIZE    =  5,
    WHM_PATCH_Y_SIZE    =  8,
    WHM_N9PATCH_SCALE_X =  3,
    WHM_N9PATCH_SCALE_Y =  3,


};

/* --- ELEMENTS --- */
extern MenuElemDirections whm_links[WHM_ELEM_NUM];
extern Point whm_elem_pos[WHM_ELEM_NUM];
extern Point whm_elem_box[WHM_ELEM_NUM];

typedef struct WhichHandMenu {
    SDL_Texture *texture;

    /* With which hand is item equippable: L, R, or 2H */
    i32 handedness[UNIT_EQUIP_END];
    i32 num_handedness;

    SDL_Texture *Lhand;
    SDL_Texture *Rhand;
} WhichHandMenu;

i32 WhichHandMenu_Select(struct WhichHandMenu   *whm,
                         i32 elem);

void WhichHandMenu_Elements(struct WhichHandMenu   *whm,
                            struct Unit            *unit,
                            struct Item            *item);

/* --- Links --- */
void WhichHandMenu_Elem_Links(struct WhichHandMenu *dm,
                              struct Menu *mc);

/* --- Drawing --- */
void WhichHandMenu_Draw(struct Menu     *mc,
                        SDL_Texture     *rt,
                        SDL_Renderer    *r);

void WhichHandMenu_Update(struct WhichHandMenu  *whm,
                          struct n9Patch         *n9,
                          SDL_Texture            *rt,
                          SDL_Renderer           *r);

#endif /* WHICH_HAND_H */
