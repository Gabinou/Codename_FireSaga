#ifndef ITEM_ACTION_H
#define ITEM_ACTION_H
/*
**  Copyight 2025 Gabriel Taillon
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
** ItemActionMenu (IAM): choose action with item
**
*/

#include "enums.h"
#include "types.h"
#include "structs.h"

#include "menu/action.h"

/* --- FORWARD DECLARATIONS --- */
struct Menu;
struct Game;
struct n9Patch;
struct Loadout;

typedef struct ItemActionMenu {
    struct Point pos; /* MENU_POS_bOFFSET = 0 */

    Menu_Option options[SOTA_MAX_MENU_OPTIONS];
    i32 option_num;

    struct pActionMenu  *platform;

    struct PixelFont    *pixelnours;
    Padding       menu_padding;

    u32 id;
    /* total height is row_height * option_num */
    i32 row_height; /* [pixels] */
    i32 text_width; /* [pixels] */
    i32 icon_width;
    i32 text_alignment;

    b32 update;

    struct Loadout previous_loadout;
} ItemActionMenu;

/* --- Limited options in First Menu --- */
enum ITEM_ACTION_MENU_ENUM {
    IAM_OPTION_NUM      = 10,
    IAM_PATCH_X_SIZE    = 31,
    IAM_PATCH_Y_SIZE    = 23,
    IAM_N9PATCH_SCALE_X =  6,
    IAM_N9PATCH_SCALE_Y =  6,
};
extern const i32 IAM_Options[IAM_OPTION_NUM];

ItemActionMenu *ItemActionMenu_Alloc(void);
void ItemActionMenu_Free(ItemActionMenu *fm, struct Menu *mc);

/* --- Dynamic Options --- */
void ItemActionMenu_Dynamic(ItemActionMenu *uam,
                            struct n9Patch *n9,
                            tnecs_E unit_E,
                            struct Game *game);

/* --- Elem Move --- */
i32 ItemActionMenu_Elem_Move(    struct Menu *mc, i32 direction);

/* -- Elems -- */
void ItemActionMenu_Elem_Pos(    ItemActionMenu *m, struct Menu *mc);
void ItemActionMenu_Elem_Links(  ItemActionMenu *m, struct Menu *mc);
void ItemActionMenu_Elem_Boxes(  ItemActionMenu *m, struct Menu *mc);

i32 ItemActionMenu_Option_Order(ItemActionMenu *m,
                                i32 option);

i32 ItemActionMenu_Options_Num(const ItemActionMenu *uam);

void ItemActionMenu_Load(   ItemActionMenu *am,
                            struct n9Patch *n9);

/* --- Drawing --- */
/* TODO: remove SDL stuff from _Draw functions
** If ever platform/core is separated EVERYWHERE */
void ItemActionMenu_Draw(   struct Menu *mc,
                            SDL_Texture *rt,
                            SDL_Renderer *r);

#endif /* ITEM_ACTION_H */
