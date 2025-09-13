#ifndef MAP_ACTION_H
#define MAP_ACTION_H
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
** MapActionMenu (MAM): choose action on map
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

typedef struct ActionMenu MapActionMenu;

/* --- Limited options in First Menu --- */
enum MAP_ACTION_MENU_ENUM {
    MAM_OPTION_NUM      =  5,
    MAM_PATCH_X_SIZE    = 31,
    MAM_PATCH_Y_SIZE    = 23,
    MAM_N9PATCH_SCALE_X =  6,
    MAM_N9PATCH_SCALE_Y =  6,
};
extern const i32 MAM_Options[MAM_OPTION_NUM];

MapActionMenu *MapActionMenu_Alloc(void);
void MapActionMenu_Free(MapActionMenu *fm, struct Menu *mc);

/* --- Dynamic Options --- */
void MapActionMenu_Dynamic(MapActionMenu *mam, n9Patch *n9);

/* --- Elem Move --- */
i32 MapActionMenu_Elem_Move(    struct Menu *mc, i32 direction);

/* -- Elems -- */
void MapActionMenu_Elem_Pos(    MapActionMenu *m, struct Menu *mc);
void MapActionMenu_Elem_Links(  MapActionMenu *m, struct Menu *mc);
void MapActionMenu_Elem_Boxes(  MapActionMenu *m, struct Menu *mc);

i32 MapActionMenu_Option_Order(MapActionMenu *m, i32 option);

i32 MapActionMenu_Options_Num(const MapActionMenu *uam);

void MapActionMenu_Load(MapActionMenu *am, struct n9Patch *n9);

/* --- Drawing --- */
/* TODO: remove SDL stuff from _Draw functions
** If ever platform/core is separated EVERYWHERE */
void MapActionMenu_Draw(struct Menu *mc, SDL_Texture *rt,
                        SDL_Renderer *r);

#endif /* MAP_ACTION_H */
