#ifndef ITEM_DROP_MENU_H
#define ITEM_DROP_MENU_H

#include "enums.h"
#include "globals.h"
#include "game/game.h"
#include "pixelfonts.h"
#include "menu.h"
#include "unit.h"
#include "item.h"
#include "n9patch.h"
#include "nstr.h"


/* Dropping backpack full when twohanding */
// 1- Player gets choice of what to equip
//      a- Player chooses to twohand a weapon
//      b- ItemSelectMenu (Equipment Full, Drop item to twohand) appears
// 2- Player gets choice of what to drop/send to convoy
//      a- Player chooses a weapon
// 3- Player gets choice of drop or not
//      a- Player chooses yes/no

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
    IDM_ELEM_YES    =  0, /* Yes is on top */
    IDM_ELEM_NO     =  1,
    IDM_ELEM_NUM    =  2,
};

extern       struct MenuElemDirections  idm_links         [IDM_ELEM_NUM];
extern       struct Point               idm_elem_pos      [IDM_ELEM_NUM];
extern       struct Point               idm_elem_box      [IDM_ELEM_NUM];
extern       struct Point               idm_cursor_pos    [IDM_ELEM_NUM];
extern       struct Point               idm_cursor_box    [IDM_ELEM_NUM];


struct ItemDropMenu {
    struct Point pos; /* [pixels], MENU_POS_bOFFSET = 0 */

    SDL_Texture *texture;

    int item_todrop;   /* side space */
    int item_width;    /* [pixels]   */

    struct Unit *unit;

    struct PixelFont *pixelnours;
    struct PixelFont *pixelnours_big;

    bool update : 1;
};
extern struct ItemDropMenu ItemDropMenu_default;

/* --- Constructors/Destructors --- */
extern struct ItemDropMenu *ItemDropMenu_Alloc();
extern void ItemDropMenu_Free(struct ItemDropMenu *idm);
extern void ItemDropMenu_Load(struct ItemDropMenu *idm, SDL_Renderer *r, struct n9Patch *n9);

/* --- Elements --- */
extern void ItemDropMenu_Elem_Pos(struct ItemDropMenu *idm, struct MenuComponent *mc);

/* --- Selection --- */
extern void ItemDropMenu_Drop(  struct ItemDropMenu *idm);
extern void ItemDropMenu_Select(struct ItemDropMenu *idm, if8 elem);

/* --- Drawing --- */
extern void ItemDropMenu_Draw(  struct MenuComponent *mc,  SDL_Texture *rt, SDL_Renderer *r);
extern void ItemDropMenu_Update(struct ItemDropMenu  *idm, struct n9Patch *n9, SDL_Texture *rt,
                                SDL_Renderer *r);

#endif /* ITEM_DROP_MENU_H */
