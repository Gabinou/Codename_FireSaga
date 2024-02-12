#ifndef DEPLOYMENT_MENU_H
#define DEPLOYMENT_MENU_H

#include "enums.h"
#include "pixelfonts.h"
#include "menu.h"
#include "unit.h"
#include "map/map.h"
#include "graph.h"
#include "n9patch.h"
#include "stb_sprintf.h"
#include "SDL2/SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct Map;
struct Unit;
struct Menu;
struct Game;
struct MenuElemDirections;

enum DM_MENU {

    DM_PATCH_X_SIZE     =  29,
    DM_PATCH_Y_SIZE     =  21,
    DM_N9PATCH_SCALE_X  =   3,
    DM_N9PATCH_SCALE_Y  =   3,

    DM_PAGE_NUM         =   4,
    DM_LINE_CONTENT_H   =  16,
    DM_LINE_SPACE_H     =   2,
    DM_SCROLL_UP_ADD    =   2, /* Number of units to scroll up/down*/
    DM_LINE_H           =  DM_LINE_CONTENT_H + DM_LINE_SPACE_H,
    DM_LINE_NUM         =   8,

    /* --- Menu frame --- */
    DM_NAME_X       =  59,
    DM_NAME_Y       =   5,

    DM_PAGE_NUM_X   = 216,
    DM_PAGE_NUM_Y   =   8,

    /* -- Page 1 -- */
    DM_CLASS_X      = DM_NAME_X + 56,
    DM_CLASS_Y      = DM_NAME_Y,

    DM_LVL_X        = DM_CLASS_X + 45,
    DM_LVL_Y        = DM_CLASS_Y + 2,

    DM_EXP_X        = DM_LVL_X + 18,
    DM_EXP_Y        = DM_LVL_Y,

    DM_HP_X         = DM_EXP_X + 16,
    DM_HP_Y         = DM_LVL_Y,

    DM_MOVE_X       = DM_HP_X + 13,
    DM_MOVE_Y       = DM_LVL_Y,

    /* -- Page 2 -- */
    DM_STR_X        = DM_NAME_X + 52,
    DM_STR_Y        = DM_NAME_Y +  3,

    DM_MAG_X        = DM_STR_X  + 26,
    DM_MAG_Y        = DM_STR_Y,

    DM_DEX_X        = DM_MAG_X  + 26,
    DM_DEX_Y        = DM_STR_Y,

    DM_AGI_X        = DM_DEX_X  + 26,
    DM_AGI_Y        = DM_STR_Y,

    DM_CON_X        = DM_AGI_X  + 26,
    DM_CON_Y        = DM_STR_Y,

    /* -- Page 3 -- */
    DM_DEF_X        = DM_NAME_X + 52,
    DM_DEF_Y        = DM_NAME_Y +  5,

    DM_RES_X        = DM_DEF_X  + 26,
    DM_RES_Y        = DM_DEF_Y,

    DM_FTH_X        = DM_DEF_X  + 26,
    DM_FTH_Y        = DM_DEF_Y,

    DM_LUCK_X       = DM_DEF_X  + 26,
    DM_LUCK_Y       = DM_DEF_Y,

    DM_PROF_X       = DM_DEF_X  + 26,
    DM_PROF_Y       = DM_DEF_Y,

    /* -- Page 4 -- */
    DM_WPN_TYPE_X   = DM_NAME_X + 60,
    DM_WPN_TYPE_Y   = DM_NAME_Y + 5,

    DM_REGRETS_X    = DM_WPN_TYPE_X + 43,
    DM_REGRETS_Y    = DM_WPN_TYPE_Y,

    DM_MOUNT_X      = DM_REGRETS_X  + 38,
    DM_MOUNT_Y      = DM_WPN_TYPE_Y,

    /* --- Unit frame --- */
    /* Relative to menu frame */
    DM_UNIT_FRAME_X     =   8,
    DM_UNIT_FRAME_Y     =  24,
    DM_UNIT_FRAME_W     = 208,
    DM_UNIT_FRAME_H     = DM_LINE_H * DM_LINE_NUM,

    /* --- Page frame --- */
    /* Relative to unit frame */
    DM_PAGE_FRAME_X     =  78,
    DM_PAGE_FRAME_Y     =   0,
    DM_PAGE_FRAME_W     = 128,
    DM_PAGE_FRAME_H     = DM_UNIT_FRAME_H,

};

/* --- DeploymentMenu Design --- */
// 3 Different frames
//  -> Menu Frame
//      - Parent of Units Frame
//      - Headers
//      - Scroll bar
//  -> Units Frame
//      - Parent of Page frame
//      - +X +Y transform
//      - Cutoff content outside bounds
//      - Scrolls up and down
//      - Names
//      - Map Units
//  -> Page Frame
//      - +Y transform
//      - Scrolls Left and right
//      - Stats
//      - Weapon icons
//      - Mount icons

struct DeploymentMenu {
    b32 update;
    struct Point pos; /* [pixels] */

    i32 party_size;
    i32 top_unit;   /* Up   - Down  scrolling [0, party_size] */
    i32 page;       /* Left - Right scrolling [0, DM_PAGE_NUM]*/

    struct Unit         *party;
    struct Map          *map;
    struct PixelFont    *font_wpns; /* wpn icons as font to write them easily */
    struct PixelFont    *pixelnours;
    struct PixelFont    *pixelnours_16;
    struct PixelFont    *pixelnours_big;

    SDL_Texture *texture;
    SDL_Texture *texture_mount;
};
extern struct DeploymentMenu DeploymentMenu_default;

/* --- Constructors/Destructors --- */
struct DeploymentMenu *DeploymentMenu_Alloc(void);
void DeploymentMenu_Free(struct DeploymentMenu *dm);
void DeploymentMenu_Load(struct DeploymentMenu *dm, SDL_Renderer *renderer,
                         struct n9Patch *n9patch);

/* --- Drawing --- */
void DeploymentMenu_Draw(  struct Menu *mc, SDL_Texture *rt, SDL_Renderer *r);
void DeploymentMenu_Update(struct DeploymentMenu *dm, struct n9Patch      *n,
                           SDL_Texture    *t,         SDL_Renderer *r);

#endif /* DEPLOYMENT_MENU_H */
