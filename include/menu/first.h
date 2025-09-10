#ifndef FIRST_MENU_H
#define FIRST_MENU_H
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
** FirstMenu (FM): On Title screen, first choice
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

typedef struct ActionMenu FirstMenu;

/* --- Limited options in First Menu --- */
enum FIRST_MENU_ENUM {
    FM_OPTION_NUM  =  3,
};
extern const i32 FM_Options[FM_OPTION_NUM];

/* --- Constructors/Destructors --- */
FirstMenu *FirstMenu_Alloc(void);
void FirstMenu_Free(    FirstMenu *m,   struct Menu       *mc);
void FirstMenu_Load(    FirstMenu *m,   struct n9Patch    *n9);

/* --- Menu Elem properties --- */
i32 FirstMenu_Options_Num(  const FirstMenu *m);

/* --- Elem Move --- */
i32 FirstMenu_Elem_Move(    struct Menu *mc, i32 direction);

/* -- Elems -- */
void FirstMenu_Elem_Pos(    FirstMenu *m, struct Menu *mc);
void FirstMenu_Elem_Links(  FirstMenu *m, struct Menu *mc);
void FirstMenu_Elem_Boxes(  FirstMenu *m, struct Menu *mc);

/* --- Drawing --- */
/* TODO: remove SDL stuff from _Draw functions
** If ever platform/core is separated EVERYWHERE */
void FirstMenu_Draw(struct Menu *mc, SDL_Texture *rt, SDL_Renderer *r);

void FirstMenu_Dynamic(FirstMenu *fm, struct n9Patch  *n9);

#endif /* FIRST_MENU_H */