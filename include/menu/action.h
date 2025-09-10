#ifndef ACTION_MENU_H
#define ACTION_MENU_H
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
** ActionMenu (AM): Base type for dynamic test menu.
**
**  Requirements:
**  - Dynamic number of TEXT-ONLY actions
**  - Actions might be GREYED OUT, or ABSENT
**      - GREYED OUT:   action possible in other context,
**                      e.g. using item only when HP not full
**                      gives access to hint
**      - ABSENT:       action not possible,
**                      e.g. item has NO use action
**
*/

#include "enums.h"
#include "types.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Menu;
struct Game;
struct n9Patch;
struct pActionMenu;

enum ACTION_MENU_ENUM {
    AM_N9PATCH_SCALE_X =  6,
    AM_N9PATCH_SCALE_Y =  6,
    AM_PADDING_RIGHT   =  7,
    AM_PADDING_TOP     =  5,
    AM_PADDING_LEFT    =  7,
    AM_PADDING_BOTTOM  =  3,
};

typedef struct ActionMenu {
    struct Point pos; /* MENU_POS_bOFFSET = 0 */

    Menu_Option options[SOTA_MAX_MENU_OPTIONS];
    i32 option_num;

    struct pActionMenu  *platform;

    struct PixelFont    *pixelnours;
    Padding       menu_padding;

    u32 id;
    /* total height is row_height * option_num */
    i32 row_height; /* [pixels] */
    i32 text_width; /* [pixels] */
    i32 icon_width;
    i32 text_alignment;

    b32 update;
} ActionMenu;
extern const ActionMenu ActionMenu_default;

/* --- Constructors/Destructors --- */
ActionMenu *ActionMenu_Alloc(void);
struct pActionMenu *pActionMenu_Alloc(void);
void ActionMenu_Load(   ActionMenu *m,  struct n9Patch    *n9);
void ActionMenu_Free(   ActionMenu *m,  struct Menu       *mc);
void pActionMenu_Free_Texture(struct pActionMenu *pam);
void pActionMenu_Free(struct pActionMenu *pam);
void pActionMenu_Set(struct pActionMenu *pam, SDL_Texture *rt, SDL_Renderer *r);

void pActionMenu_Load(const struct pActionMenu  *pAM,
                      struct n9Patch      *n9);

/* --- Menu Elem properties --- */
i32 ActionMenu_Options_Num( const ActionMenu *m);

/* --- Elem Move --- */
i32 ActionMenu_Elem_Move(   struct Menu *mc, i32 direction);

/* -- Options -- */
void ActionMenu_Option_Add(     ActionMenu *m, Menu_Option opt);
void ActionMenu_Compute_Size(   ActionMenu *m, struct n9Patch *n9);
void ActionMenu_Options_Reset(  ActionMenu *m);

/* -- Elems -- */
void ActionMenu_Elem_Pos(   ActionMenu *m, struct Menu *mc);
void ActionMenu_Elem_Links( ActionMenu *m, struct Menu *mc);
void ActionMenu_Elem_Boxes( ActionMenu *m, struct Menu *mc);

/* -- Cursor -- */
void ActionMenu_Cursor_Pos(     ActionMenu *m, struct Menu *mc);
void ActionMenu_Cursor_Boxes(   ActionMenu *m, struct Menu *mc);

int ActionMenu_Option_Index(ActionMenu *am, i32 option);

/* --- Drawing --- */
/* TODO: remove SDL stuff from _Draw functions
** If ever platform/core is separated EVERYWHERE */
void ActionMenu_Draw(struct Menu *mc, SDL_Texture *rt, SDL_Renderer *r);

void pActionMenu_Draw(      ActionMenu *am, n9Patch *n9);
void pActionMenu_Update(    ActionMenu   *fm, struct n9Patch  *n9);

/* TODO Menu option source? */
s8 Menu_Option_Name(i32 id);

#endif /* ACTION_MENU_H */
