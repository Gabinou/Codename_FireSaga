#ifndef ITEM_DROP_MENU_H
#define ITEM_DROP_MENU_H
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
** ItemDropMenu: Give player 1 chance to cancel item drop
**
*/

#include "enums.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Menu;
struct n9Patch;

/* A:Aggressor */
/* D:Defendant */
enum IDM_MENU {
    IDM_PATCH_X_PIXELS  =  8,
    IDM_PATCH_Y_PIXELS  =  8,
    IDM_PATCH_X_SIZE    =  4,
    IDM_PATCH_Y_SIZE    =  6,

    IDM_N9PATCH_SCALE_X =  1,
    IDM_N9PATCH_SCALE_Y =  1,

    IDM_ROW_HEIGHT      = ASCII_GLYPH_HEIGHT + 5,
    IDM_ITEM_MIN_WIDTH  = 60,
    IDM_LEFT_OF_TEXT    = 12,
    IDM_RIGHT_OF_TEXT   = 12,
    IDM_BOTTOM_OF_TEXT  =  8,
    IDM_TOP_OF_TEXT     =  7,

    IDM_ITEM_NAME_X     =  IDM_LEFT_OF_TEXT,
    IDM_ITEM_NAME_Y     =  IDM_TOP_OF_TEXT,

    IDM_ITEM_ICON_X     =  IDM_LEFT_OF_TEXT,
    IDM_ITEM_ICON_Y     =  IDM_TOP_OF_TEXT,

    IDM_ELEM_YES_X      =  IDM_ITEM_NAME_X, /* temporary, actual position is dynamic */
    IDM_ELEM_YES_Y      =  IDM_ITEM_NAME_Y + IDM_ROW_HEIGHT,

    IDM_ELEM_NO_X       =  IDM_ITEM_NAME_X, /* temporary, actual position is dynamic */
    IDM_ELEM_NO_Y       =  IDM_ELEM_YES_Y  + IDM_ROW_HEIGHT,
};

enum IDM_ELEMS {
    IDM_ELEM_NULL   = -1,
    IDM_ELEM_0      =  0,
    IDM_ELEM_1      =  1,
    IDM_OPTION_NUM  =  2,
};

extern const i32 IDM_Options[IDM_OPTION_NUM];

extern const struct n4Directions    idm_links         [IDM_OPTION_NUM];
extern const struct Point           idm_elem_pos      [IDM_OPTION_NUM];
extern const struct Point           idm_elem_box      [IDM_OPTION_NUM];
extern const struct Point           idm_cursor_pos    [IDM_OPTION_NUM];
extern const struct Point           idm_cursor_box    [IDM_OPTION_NUM];

typedef struct ItemDropMenu {
    b32 update;
    struct Point pos; /* [pixels] */

    Menu_Option options[SOTA_MAX_MENU_OPTIONS];
    i32 option_num;

    i32 eq_todrop;
    tnecs_E unit_E;

    i32 item_width;     /* [pixels]   */

    SDL_Texture *texture;
    struct PixelFont *pixelnours;
    struct PixelFont *pixelnours_big;
} ItemDropMenu;
extern const ItemDropMenu ItemDropMenu_default;

/* --- Constructors/Destructors --- */
ItemDropMenu *ItemDropMenu_Alloc(void);
void ItemDropMenu_Free( ItemDropMenu *idm);
void ItemDropMenu_Load( ItemDropMenu *idm,
                        SDL_Renderer *r, struct n9Patch *n9);

/* --- Elements --- */
void ItemDropMenu_Elem_Pos(ItemDropMenu *idm, struct Menu *mc);

/* --- Drawing --- */
void ItemDropMenu_Draw( struct Menu *mc,
                        SDL_Texture *rt, SDL_Renderer *r);

void ItemDropMenu_Update(   ItemDropMenu *idm,
                            struct n9Patch *n9,
                            SDL_Texture *rt,
                            SDL_Renderer *r);

#endif /* ITEM_DROP_MENU_H */
