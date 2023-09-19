#ifndef WSM_H
#define WSM_H

#include "enums.h"
#include "globals.h"
#include "pixelfonts.h"
#include "n9patch.h"
#include "events.h"
#include "enums.h"
#include "structs.h"
#include "menu.h"
#include "nstr.h"
#include "stb_sprintf.h"
#include "SDL2/SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct MenuComponent;
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

enum WSM_MENU {
    WSM_PATCH_X_PIXELS          =  8,
    WSM_PATCH_Y_PIXELS          =  8,
    ISM_PATCH_X_SIZE            = 23,
    WSM_PATCH_X_SIZE            = 23,
    WSM_PATCH_Y_SIZE            = 12,
    ISM_PATCH_Y_SIZE            = 12,
    WSM_N9PATCH_SCALE_X         =  4,
    WSM_N9PATCH_SCALE_Y         =  4,
    ISM_N9PATCH_SCALE_X         =  4,
    ISM_N9PATCH_SCALE_Y         =  4,

    WSM_ROW_HEIGHT              = ASCII_GLYPH_HEIGHT + 6,

    WSM_ITEM_MIN_WIDTH          = 60,
    WSM_HEADER_LEFT             = 12,
    WSM_HEADER_RIGHT            = 12,
    WSM_LEFT_OF_TEXT            = 38,
    WSM_RIGHT_OF_TEXT           = 48,
    WSM_BOTTOM_OF_TEXT          =  8,
    WSM_TOP_OF_TEXT             =  8,
    WSM_HAND_RIGHT_OFFSET       = 42,
    WSM_HAND_RIGHT_OFFSET_SMALL = 38,

    WSM_ICONTOTEXT_W            = 15,
    WSM_HIGHLIGHT_EXTRA_W       =  3,

    WSM_ICON_W                  = 12,
    WSM_ICON_H                  = 12,
    WSM1_X_OFFSET               =  9,
    WSM1_Y_OFFSET               =  7,

    WSM1_DURA_X_OFFSET          = WSM1_X_OFFSET + WSM_ICON_W + 7,
    WSM1_DURA_Y_OFFSET          = WSM1_Y_OFFSET + 2,

    WSM1_NAME_X_OFFSET          = WSM1_DURA_X_OFFSET + 8,
    WSM1_NAME_Y_OFFSET          = WSM1_Y_OFFSET + 3,

    WSM2_X_OFFSET               = WSM1_X_OFFSET,
    WSM3_X_OFFSET               = WSM1_X_OFFSET,
    WSM4_X_OFFSET               = WSM1_X_OFFSET,
    WSM5_X_OFFSET               = WSM1_X_OFFSET,
    WSM6_X_OFFSET               = WSM1_X_OFFSET,
    WSM2_Y_OFFSET               = WSM1_Y_OFFSET + WSM_ICON_H + 2,
    WSM3_Y_OFFSET               = WSM2_Y_OFFSET + WSM_ICON_H + 2,
    WSM4_Y_OFFSET               = WSM3_Y_OFFSET + WSM_ICON_H + 2,
    WSM5_Y_OFFSET               = WSM4_Y_OFFSET + WSM_ICON_H + 2,
    WSM6_Y_OFFSET               = WSM5_Y_OFFSET + WSM_ICON_H + 2,
};

enum WSM_HANDS {
    WSM_HANDS_TILESIZE          =  16,
    WSM_HANDS_NONE              =  -1,
    WSM_HANDS_BIG_L             =   0,
    WSM_HANDS_SMALL_L           =   1,
    WSM_HANDS_SMALL_R           =   2,
    WSM_HANDS_BIG_R             =   3,
    WSM_HANDL_X                 =  10,
    WSM_HANDL_Y                 =   6,
    WSM_HANDR_X                 = 100,
    WSM_HANDR_Y                 =   6,
    WSM_HAND_SMALLX_OFFSET      =   2,
    WSM_HAND_SMALLY_OFFSET      =   3,
    WSM_WEAKHAND_Y_OFFSET       =  14,
    WSM_TWOHAND_Y_OFFSET        =   2,
};

enum WSM_ELEMS {
    WSM_ELEM_NULL   = -1,
    WSM_ELEM_ITEM1  =  0,
    WSM_ELEM_ITEM2,
    WSM_ELEM_ITEM3,
    WSM_ELEM_ITEM4,
    WSM_ELEM_ITEM5,
    WSM_ELEM_ITEM6,
    WSM_ELEMS_NUM,
};

#define WSM_BUFFER_LEN 128
extern char WSM_menu_description[WSM_ELEMS_NUM][WSM_BUFFER_LEN];
// TODO: reduce memory usage.

extern struct MenuElemDirections    wsm_links[WSM_ELEMS_NUM];
extern struct Point   wsm_elem_pos[WSM_ELEMS_NUM];
extern struct Point   wsm_elem_box[WSM_ELEMS_NUM];
extern struct Point   wsm_cursor_pos[WSM_ELEMS_NUM];
extern struct Point   wsm_cursor_box[WSM_ELEMS_NUM];
extern const struct Point   wsm_elem_pos_const[WSM_ELEMS_NUM];

struct LoadoutSelectMenu {
    struct Point pos; /* [pixels], MENU_POS_bOFFSET = 0 */

    SDL_Texture *texture;
    SDL_Texture *texture_hands;

    int menu_w;
    int menu_h;

    struct Unit *unit;
    // selected is -1 if NULL

    struct PixelFont *pixelnours;
    struct PixelFont *pixelnours_big;
    if8 selected[UNIT_HANDS_NUM]; /* side space */
    char *item_name;
    size_t item_name_len;
    char *header;

    int archetype_stronghand;
    int archetype_weakhand;
    bool update:             1;
};
extern struct LoadoutSelectMenu LoadoutSelectMenu_default;

/* --- Constructors/Destructors --- */
extern struct LoadoutSelectMenu *LoadoutSelectMenu_Alloc();
extern void LoadoutSelectMenu_Free(struct LoadoutSelectMenu *lsm);
extern void LoadoutSelectMenu_Load(struct LoadoutSelectMenu *sm, struct Map *map,
                                   tnecs_world_t *w, tnecs_entity_t ent, SDL_Renderer *r, struct n9Patch *n9, int a);

extern void _LoadoutSelectMenu_Load(struct LoadoutSelectMenu *sm, struct Unit *u, SDL_Renderer *r,
                                    struct n9Patch *n9);

extern void WeaponSelectMenu_Load_n9Patch(struct LoadoutSelectMenu *sm, SDL_Renderer *r,
                                          struct n9Patch *n9);
extern void WeaponSelectMenu_Load(struct LoadoutSelectMenu *sm, struct Map *map, tnecs_world_t *w,
                                  tnecs_entity_t ent, SDL_Renderer *r, struct n9Patch *n9);

extern void StaffSelectMenu_Load(struct LoadoutSelectMenu *sm, struct Map *map,
                                 tnecs_world_t *w, tnecs_entity_t ent, SDL_Renderer *r, struct n9Patch *n9);

/* --- Hand --- */
extern int LoadoutSelectMenu_isLeftHand( struct LoadoutSelectMenu *lsm, int s);
extern int LoadoutSelectMenu_isRightHand(struct LoadoutSelectMenu *lsm, int s);

extern void LoadoutSelectMenu_Header_Set(struct LoadoutSelectMenu *lsm, const char *header);

/* --- Item placement --- */
extern void LoadoutSelectMenu_Select(       struct LoadoutSelectMenu *lsm, int s);
extern void LoadoutSelectMenu_Deselect(     struct LoadoutSelectMenu *lsm);
extern int  LoadoutSelectMenu_num_items(    struct LoadoutSelectMenu *lsm);
extern void LoadoutSelectMenu_Name_Alloc(   struct LoadoutSelectMenu *lsm, char *n);
extern void LoadoutSelectMenu_Select_Reset( struct LoadoutSelectMenu *lsm);
extern bool WeaponSelectMenu_Usable_Remains(struct LoadoutSelectMenu *lsm);
extern void ItemSelectMenu_Select(          struct LoadoutSelectMenu *lsm, int s);


/* --- Elements --- */
extern void LoadoutSelectMenu_Elem_Pos(  struct LoadoutSelectMenu *lsm, struct MenuComponent *mc);
extern void LoadoutSelectMenu_Elem_Reset(struct LoadoutSelectMenu *lsm, struct MenuComponent *mc);

/* --- Drawing --- */
extern void LoadoutSelectMenu_Size(struct LoadoutSelectMenu  *lsm, struct n9Patch *n9);

extern void LoadoutSelectMenu_Draw_Hands(    struct LoadoutSelectMenu  *lsm, SDL_Renderer *r);
extern void LoadoutSelectMenu_Draw_Items(    struct LoadoutSelectMenu  *lsm, SDL_Renderer *r);
extern void LoadoutSelectMenu_Draw_Header(   struct LoadoutSelectMenu  *lsm, SDL_Renderer *r);
extern void LoadoutSelectMenu_Draw_Patches(  struct LoadoutSelectMenu  *lsm, SDL_Renderer *r);
extern void LoadoutSelectMenu_Draw_Highlight(struct LoadoutSelectMenu  *lsm, SDL_Renderer *r);

extern void LoadoutSelectMenu_Draw(  struct  MenuComponent *mc, SDL_Texture *rt, SDL_Renderer *r);
extern void LoadoutSelectMenu_Update(struct  LoadoutSelectMenu  *lsm, struct n9Patch *n9,
                                     SDL_Texture *rt, SDL_Renderer *r);

#endif /* WSM_H */
