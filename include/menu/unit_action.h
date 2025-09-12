#ifndef UNIT_ACTION_H
#define UNIT_ACTION_H
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
** UnitActionMenu (UAM): choose unit action after mvt
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

typedef struct ActionMenu UnitActionMenu;

/* --- Limited options in First Menu --- */
enum UNIT_ACTION_MENU_ENUM {
    UAM_OPTION_NUM      = 10,
    UAM_PATCH_X_SIZE    = 31,
    UAM_PATCH_Y_SIZE    = 23,
    UAM_N9PATCH_SCALE_X =  6,
    UAM_N9PATCH_SCALE_Y =  6,


};
extern const i32 UAM_Options[UAM_OPTION_NUM];

UnitActionMenu *UnitActionMenu_Alloc(void);
void UnitActionMenu_Free(UnitActionMenu *fm, Menu *mc);

/* --- Dynamic Options --- */
void UnitActionMenu_Dynamic(UnitActionMenu *uam, n9Patch *n9,
                            tnecs_E unit_E,     Game *game);

/* --- Elem Move --- */
i32 UnitActionMenu_Elem_Move(    struct Menu *mc, i32 direction);

/* -- Elems -- */
void UnitActionMenu_Elem_Pos(    UnitActionMenu *m, struct Menu *mc);
void UnitActionMenu_Elem_Links(  UnitActionMenu *m, struct Menu *mc);
void UnitActionMenu_Elem_Boxes(  UnitActionMenu *m, struct Menu *mc);

i32 UnitActionMenu_Options_Num(const UnitActionMenu *uam);

void UnitActionMenu_Load(UnitActionMenu *am, n9Patch *n9);

/* --- Drawing --- */
/* TODO: remove SDL stuff from _Draw functions
** If ever platform/core is separated EVERYWHERE */
void UnitActionMenu_Draw(struct Menu *mc, SDL_Texture *rt, SDL_Renderer *r);

#endif /* UNIT_ACTION_H */
