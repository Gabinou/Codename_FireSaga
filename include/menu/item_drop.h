
#ifndef ITEM_DROP_MENU_H
#define ITEM_DROP_MENU_H

#include "enums.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct n9Patch;
struct Menu;

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

extern const struct MenuElemDirections  idm_links         [IDM_ELEM_NUM];
extern const struct Point               idm_elem_pos      [IDM_ELEM_NUM];
extern const struct Point               idm_elem_box      [IDM_ELEM_NUM];
extern const struct Point               idm_cursor_pos    [IDM_ELEM_NUM];
extern const struct Point               idm_cursor_box    [IDM_ELEM_NUM];

struct ItemDropMenu {
    b32 update;
    struct Point pos; /* [pixels] */

    SDL_Texture *texture;

    i32 item_todrop;   /* side space */
    i32 item_width;    /* [pixels]   */

    struct Unit *unit; /* TODO: use entity */

    struct PixelFont *pixelnours;
    struct PixelFont *pixelnours_big;
};
extern const struct ItemDropMenu ItemDropMenu_default;

/* --- Constructors/Destructors --- */
struct ItemDropMenu *ItemDropMenu_Alloc(void);
void ItemDropMenu_Free(struct ItemDropMenu *idm);
void ItemDropMenu_Load(struct ItemDropMenu *idm, SDL_Renderer *r, struct n9Patch *n9);

/* --- Elements --- */
void ItemDropMenu_Elem_Pos(struct ItemDropMenu *idm, struct Menu *mc);

/* --- Selection --- */
void ItemDropMenu_Drop(  struct ItemDropMenu *idm);
void ItemDropMenu_Select(struct ItemDropMenu *idm, i8 elem);

/* --- Drawing --- */
void ItemDropMenu_Draw(  struct Menu *mc,  SDL_Texture *rt, SDL_Renderer *r);

void ItemDropMenu_Update(struct ItemDropMenu  *idm, struct n9Patch *n9,
                         SDL_Texture *rt, SDL_Renderer *r);

#endif /* ITEM_DROP_MENU_H */
