#ifndef DEPLOYMENT_MENU_H
#define DEPLOYMENT_MENU_H

#include "enums.h"
#include "pixelfonts.h"
#include "menu.h"
#include "map.h"
#include "graph.h"
#include "n9patch.h"
#include "stb_sprintf.h"

/* --- FORWARD DECLARATIONS --- */
struct Map;
struct Unit;
struct Menu;
struct Game;
struct MenuElemDirections;

enum DM_MENU {
    DM_ELEM_START       = -1,

};

struct DeploymentMenu {
    b32 update;
    u8  party_size;

    struct Unit *party;
    struct Map  *map;
    struct PixelFont *font_wpns; /* wpn icons as font to write them easily */
    struct PixelFont *pixelnours;
    struct PixelFont *pixelnours_16;
    struct PixelFont *pixelnours_big;

    SDL_Texture *texture_mount;
};
extern struct DeploymentMenu DeploymentMenu_default;

/* --- ConstConstructors/Destructors --- */
struct DeploymentMenu *DeploymentMenu_Alloc(void);
void DeploymentMenu_Free(struct DeploymentMenu *gm);

/* --- Drawing --- */
void DeploymentMenu_Draw(  struct Menu *mc, SDL_Texture *rt, SDL_Renderer *r);
void DeploymentMenu_Update(struct DeploymentMenu *g, struct n9Patch *n9, SDL_Texture *rt,
                        SDL_Renderer *r);


#endif /* DEPLOYMENT_MENU_H */
