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

    DM_PAGE_NUM         =   4,
    DM_LINE_CONTENT_H   =  16,
    DM_LINE_SPACE_H     =   2,
    DM_SCROLL_UP_ADD    =   2, /* Number of units to scroll up/down*/
    DM_LINE_H           =  DM_LINE_CONTENT_H + DM_LINE_SPACE_H,
    DM_LINE_NUM         =   8,

    /* --- Unit frame ---  */
    /* Relative to menu frame */
    DM_UNIT_FRAME_X     =   8,
    DM_UNIT_FRAME_Y     =  24,
    DM_UNIT_FRAME_W     = 208,
    DM_UNIT_FRAME_H     = DM_LINE_H * DM_LINE_NUM,

    /* --- Page frame ---  */
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
void DeploymentMenu_Free(struct DeploymentMenu *gm);

/* --- Drawing --- */
void DeploymentMenu_Draw(  struct Menu *mc, SDL_Texture *rt, SDL_Renderer *r);
void DeploymentMenu_Update(struct DeploymentMenu *g, struct n9Patch      *n,
                           SDL_Texture    *t,        SDL_Renderer *r);

#endif /* DEPLOYMENT_MENU_H */
