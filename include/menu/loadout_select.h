#ifndef LSM_H
#define LSM_H

#include "enums.h"
#include "pixelfonts.h"
#include "n9patch.h"
#include "events.h"
#include "enums.h"
#include "structs.h"
#include "types.h"
#include "menu.h"
#include "nstr.h"
#include "stb_sprintf.h"
#include "SDL2/SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct Menu;
struct MenuElemDirections;

/* LoadoutSelectMenu */
// Menu to select items in equipment, usable or not. For:
//  -   WeaponSelectmenu
//  -   StaffSelectmenu
//  -   ItemSelectmenu
//  -   TradeMenu
// TODO: Empty option for weakhand
// TODO: Weakhand selection is always ALL ITEMS + empty if there is room
// TODO: TWO HAND WEAPONS
//      - Can't use two handed weapon if equipment full
//      - Menu to ask to send item in hand to convoy?
//      - How to two hand weapons?
//         - Selecting same weapon in first hand

// Design rules:
// Two hand weapons: click twice on same weapon, once for each hand
// Two hand weapons && full equipment : click twice + send an item to convoy menu
// Stronghand choose first, so it is always on top.

/* lsm Selection flow:
*   0. Cursor starts on strong hand
*   1. Move, Click to select strong Hand weapon
*   2. Strong hand weapon switches to selected item
*   3. Highlight stronghand weapon, can still select it if twohanding.
*   4. Cursor Resets to Weak hand
*   5. Move, Click to select Weak Hand weapon
*/

/* SSM Selection flow:
*   1. Only staves for first hand
*   2. Any other weapons for second hand
*       - INCLUDING wielding staff with two hands.
*       - Make second hand show TWO HANDING or something.
*/

enum LSM_HANDS {
    LSM_HANDS_TILESIZE          =  16,
    LSM_HANDS_NONE              =  -1,
    LSM_HANDS_BIG_L             =   0,
    LSM_HANDS_SMALL_L           =   1,
    LSM_HANDS_SMALL_R           =   2,
    LSM_HANDS_BIG_R             =   3,
    LSM_HANDL_X                 =  10,
    LSM_HANDL_Y                 =   6,
    LSM_HANDR_X                 = 100,
    LSM_HANDR_Y                 =   6,
    LSM_HAND_SMALLX_OFFSET      =   2,
    LSM_HAND_SMALLY_OFFSET      =   3,
    LSM_STRONGHAND_Y_OFFSET     =   7,
    LSM_WEAKHAND_Y_OFFSET       =   6,
    LSM_TWOHAND_Y_OFFSET        =   2,
};



enum LSM_MENU {
    LSM_PATCH_X_PIXELS          =  8,
    LSM_PATCH_Y_PIXELS          =  8,
    ISM_PATCH_X_SIZE            = 23,
    LSM_PATCH_X_SIZE            = 23,
    LSM_PATCH_Y_SIZE            = 12,
    ISM_PATCH_Y_SIZE            = 12,
    LSM_N9PATCH_SCALE_X         =  4,
    LSM_N9PATCH_SCALE_Y         =  4,
    ISM_N9PATCH_SCALE_X         =  4,
    ISM_N9PATCH_SCALE_Y         =  4,

    LSM_ROW_HEIGHT              = ASCII_GLYPH_HEIGHT + 6,

    LSM_ITEM_MIN_WIDTH          = 60,
    LSM_HEADER_LEFT             = 12,
    LSM_HEADER_RIGHT            = 12,
    LSM_LEFT_OF_TEXT            = 38,
    LSM_RIGHT_OF_TEXT           = 48,
    LSM_BOTTOM_OF_TEXT          =  8,
    LSM_TOP_OF_TEXT             =  8,
    LSM_HAND_RIGHT_OFFSET       = 42,
    LSM_HAND_RIGHT_OFFSET_SMALL = 38,

    LSM_ICONTOTEXT_W            = 15,
    LSM_HIGHLIGHT_EXTRA_W       =  3,

    LSM_ICON_W                  = 12,
    LSM_ICON_H                  = 12,
    LSM1_X_OFFSET               =  9 + LSM_HANDS_TILESIZE,
    LSM1_Y_OFFSET               =  7,

    LSM1_DURA_X_OFFSET          = LSM1_X_OFFSET + LSM_ICON_W + 7,
    LSM1_DURA_Y_OFFSET          = LSM1_Y_OFFSET + 2,

    LSM1_NAME_X_OFFSET          = LSM1_DURA_X_OFFSET + 8,
    LSM1_NAME_Y_OFFSET          = LSM1_Y_OFFSET + 2,

    LSM2_X_OFFSET               = LSM1_X_OFFSET,
    LSM3_X_OFFSET               = LSM1_X_OFFSET,
    LSM4_X_OFFSET               = LSM1_X_OFFSET,
    LSM5_X_OFFSET               = LSM1_X_OFFSET,
    LSM6_X_OFFSET               = LSM1_X_OFFSET,
    LSM2_Y_OFFSET               = LSM1_Y_OFFSET + LSM_ICON_H + 2,
    LSM3_Y_OFFSET               = LSM2_Y_OFFSET + LSM_ICON_H + 2,
    LSM4_Y_OFFSET               = LSM3_Y_OFFSET + LSM_ICON_H + 2,
    LSM5_Y_OFFSET               = LSM4_Y_OFFSET + LSM_ICON_H + 2,
    LSM6_Y_OFFSET               = LSM5_Y_OFFSET + LSM_ICON_H + 2,
};

enum LSM_ELEMS {
    LSM_ELEM_NULL   = -1,
    LSM_ELEM_ITEM1  =  0,
    LSM_ELEM_ITEM2,
    LSM_ELEM_ITEM3,
    LSM_ELEM_ITEM4,
    LSM_ELEM_ITEM5,
    LSM_ELEM_ITEM6,
    LSM_ELEMS_NUM,
};

#define LSM_BUFFER_LEN 128
extern char LSM_menu_description[LSM_ELEMS_NUM][LSM_BUFFER_LEN];
// TODO: reduce memory usage.

extern struct MenuElemDirections    wsm_links[LSM_ELEMS_NUM];
extern struct MenuElemDirections    wsm_links_start[LSM_ELEMS_NUM];
extern struct Point   wsm_elem_pos[LSM_ELEMS_NUM];
extern struct Point   wsm_elem_box[LSM_ELEMS_NUM];
extern struct Point   wsm_cursor_pos[LSM_ELEMS_NUM];
extern struct Point   wsm_cursor_box[LSM_ELEMS_NUM];

struct LoadoutSelectMenu {
    b32 update;
    struct Point pos; /* [pixels] */

    SDL_Texture *texture;
    SDL_Texture *texture_hands;

    int menu_w;
    int menu_h;

    struct Unit *unit;
    // selected is -1 if NULL

    struct PixelFont *pixelnours;
    struct PixelFont *pixelnours_big;
    i8 selected[UNIT_HANDS_NUM]; /* side space */
    s8 item_name;
    s8 header;

    int archetype_stronghand;
    int archetype_weakhand;
    u8 black;
    u8 white;
};

extern struct LoadoutSelectMenu LoadoutSelectMenu_default;

/* --- Constructors/Destructors --- */
struct LoadoutSelectMenu *LoadoutSelectMenu_Alloc(void);
void LoadoutSelectMenu_Free(struct LoadoutSelectMenu *lsm);
void LoadoutSelectMenu_Load(struct LoadoutSelectMenu *sm, struct Map *map,
                            tnecs_world *w, tnecs_entity ent, SDL_Renderer *r,
                            struct n9Patch *n9, u64 a);

void LoadoutSelectMenu_Unit(struct LoadoutSelectMenu *sm, struct Unit *u);

void _LoadoutSelectMenu_Load(struct LoadoutSelectMenu *sm, struct Unit *u,
                             SDL_Renderer *r, struct n9Patch *n9);

void WeaponSelectMenu_Load_n9Patch(struct LoadoutSelectMenu *sm, SDL_Renderer *r,
                                   struct n9Patch *n9);
void WeaponSelectMenu_Load(struct LoadoutSelectMenu *sm, struct Map *map,
                           tnecs_world *w, tnecs_entity ent,
                           SDL_Renderer *r, struct n9Patch *n9);

void StaffSelectMenu_Load(struct LoadoutSelectMenu *sm, struct Map *map,
                          tnecs_world *w, tnecs_entity ent, SDL_Renderer *r,
                          struct n9Patch *n9);

void LoadoutSelectMenu_Header_Set(struct LoadoutSelectMenu *lsm,  char *header);

/* --- Item placement --- */
void LoadoutSelectMenu_Select(      struct LoadoutSelectMenu *lsm, int s);
void LoadoutSelectMenu_Deselect(    struct LoadoutSelectMenu *lsm);
void LoadoutSelectMenu_Select_Reset(struct LoadoutSelectMenu *lsm);
b32 WeaponSelectMenu_Usable_Remains(struct LoadoutSelectMenu *lsm);

void ItemSelectMenu_Select(          struct LoadoutSelectMenu *lsm, int s);

/* --- Elem Move --- */
i32 ItemSelectMenu_Elem_Move(  struct Menu *mc, i32 direction);
i32 WeaponSelectMenu_Elem_Move(struct Menu *mc, i32 direction);

/* --- Elements --- */
void LoadoutSelectMenu_Elem_Pos(  struct LoadoutSelectMenu *lsm, struct Menu *mc);
void LoadoutSelectMenu_Elem_Reset(struct LoadoutSelectMenu *lsm, struct Menu *mc);
void LoadoutSelectMenu_Elem_Pos_Revert(struct LoadoutSelectMenu *lsm,
                                       struct Menu *mc);

/* --- Drawing --- */
void LoadoutSelectMenu_Size(struct LoadoutSelectMenu *lsm, struct n9Patch *n9);
void LoadoutSelectMenu_Draw(  struct  Menu *mc, SDL_Texture *rt, SDL_Renderer *r);
void LoadoutSelectMenu_Update(struct  Menu *mc, struct LoadoutSelectMenu  *lsm, struct n9Patch *n9,
                              SDL_Texture *rt, SDL_Renderer *r);

#endif /* LSM_H */
