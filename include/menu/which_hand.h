#ifndef WHICH_HAND_H
#define WHICH_HAND_H
/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
** WhichHandMenu (WHM): choose how to equip item
*/

#include "enums.h"
#include "tnecs.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Menu;
struct Unit;
struct Item;
struct n9Patch;

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
    WHM_PATCH_PIXELS        =   8,
    WHM_WIDTH               =   0,
    WHM_PATCH_X_SIZE        =   5,
    WHM_PATCH_Y_SIZE        =   7,
    WHM_N9PATCH_SCALE_X     =   3,
    WHM_N9PATCH_SCALE_Y     =   3,

    WHM_ELEM_X              =  10,
    WHM_ELEM_Y_0            =   6,
    WHM_ELEM_Y_LINE_SPACING =  16,

    WHM_RH_X_OFFSET         =  12,
    WHM_HAND_SMALLX_OFFSET  =   2,
    WHM_HAND_SMALLY_OFFSET  =   2,
};

/* --- ELEMENTS --- */
extern n4Directions whm_links[WHM_ELEM_NUM];
extern Point whm_elem_box[WHM_ELEM_NUM];

typedef struct WhichHandMenu {
    Point pos;        /* [pixels] */

    SDL_Texture *texture;
    SDL_Texture *texture_hands;

    struct Unit *unit;

    /* With which hand is item equippable: L, R, or 2H */
    i32 handedness[UNIT_EQUIP_END];
    i32 num_handedness;
    i32 selected;

    b32 update;
} WhichHandMenu;
extern struct WhichHandMenu WhichHandMenu_default;

void WhichHandMenu_Load(struct WhichHandMenu    *whm,
                        SDL_Renderer            *renderer,
                        struct n9Patch          *n9patch);
void  WhichHandMenu_Free(struct WhichHandMenu *whm, struct Menu *mc);
void _WhichHandMenu_Free(struct WhichHandMenu *whm);
WhichHandMenu *WhichHandMenu_Alloc(void);

i32 WhichHandMenu_Select(struct WhichHandMenu   *whm,
                         i32 elem);
i32 WhichHandMenu_Selected_Hand(const struct WhichHandMenu *whm);

void WhichHandMenu_Elements(struct Menu *mc,
                            struct Unit *unit,
                            struct Item *item);

void _WhichHandMenu_Elements(WhichHandMenu  *whm,
                             struct n9Patch *n9patch,
                             struct Item    *item);

/* --- Links --- */
void WhichHandMenu_Elem_Links(struct Menu *mc);
void WhichHandMenu_Elem_Pos(WhichHandMenu *whm,
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
