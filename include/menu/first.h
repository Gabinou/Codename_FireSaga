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
#include "tnecs.h"
#include "structs.h"
/* --- Menu FSMs --- */
// NOTE: my menu naming convention is BAD
// - ACTION_menu -> player is SELECTING
//      Get rid of common PSM sub-menu type BS.
// - staff_select_menu -> staff is BEING SELECTED
// -> MAKE COHERENT by getting rid of PSM.

/* Design: PSM
**  + Reuse code
**      - Other ways to reuse code:
**          typedefs, common member struct...
**      - Common utils list:
**          1. dynamic size, # options, text
**  - Confusion
**  - PSM needs an fsm.
**      - event -> state_fsm -> menu_fsm -> AM_fsm -> mo
**          + event -> state_fsm -> menu_fsm -> mo_fsm
**  * Do menu options change depending on state?
**      - some options NEED sGmpMap
**  - Hard to customize PSM
**  - Irrelevant menu options for all PSMs
**      + Keep menu options in one list
**      + Each new menu gets a list of possible options.
**  * Menu options decide behavior, not menu
**      * Not really in favor of any design?
**  - fsm_eAcpt_sGmpMap_ssMapCndt_mo

*/

#include "enums.h"
#include "types.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Menu;
struct Game;
struct n9Patch;

enum ACTION_ENUM {
    AM_N9PATCH_SCALE_X =  6,
    AM_N9PATCH_SCALE_Y =  6,
    AM_PADDING_RIGHT   =  7,
    AM_PADDING_TOP     =  5,
    AM_PADDING_LEFT    =  7,
    AM_PADDING_BOTTOM  =  3,
};
/* ActionMenu:
**  - Dynamic number of TEXT-ONLY actions
**  - Actions might be GREYED OUT, or ABSENT 
**      - GREYED OUT:   action possible in other context,
**                      e.g. using item only when HP not full 
**      - ABSENT:       action not possible,
**                      e.g. item has NO use action 
*/
typedef struct ActionMenu {
    struct Point pos; /* MENU_POS_bOFFSET = 0 */

    Menu_Option options[SOTA_MAX_MENU_OPTIONS];
    i32 option_num;

    SDL_Texture  *texture;
    struct PixelFont    *pixelnours;
    struct Padding       menu_padding;

    u32 id;
    i32 row_height; /* [pixels] total height is row_height * option_num */
    i32 text_width; /* [pixels] */
    i32 icon_width;
    i32 text_alignment;

    b32 update;
} FirstMenu;
typedef struct ActionMenu FirstMenu;

extern const FirstMenu ActionMenu_default;

/* --- Constructors/Destructors --- */
ActionMenu *ActionMenu_Alloc(void);
void ActionMenu_Load(FirstMenu *m, SDL_Renderer *r, struct n9Patch *n9);
void ActionMenu_Free( FirstMenu *m, struct Menu *mc);
void ActionMenu_Load( FirstMenu *m,
                            SDL_Renderer *r, struct n9Patch *n9);

/* --- Menu Elem properties --- */
i32 AM_Options_Num(const FirstMenu *m);

/* --- Elem Move --- */
i32 ActionMenu_Elem_Move(struct Menu *mc, i32 direction);

/* -- Options -- */
void ActionMenu_Option_Add(   FirstMenu *m,
                                    u32 op, b32 enabled);
int  ActionMenu_Option_Index( FirstMenu *m, u32 op);
void ActionMenu_Compute_Size( FirstMenu *m,
                                    struct n9Patch *n9);
void ActionMenu_Options_Reset(FirstMenu *m);

/* -- Elems -- */
// ActionMenu_Elem_Links SHOULD NOT NEED LINKS
void ActionMenu_Elem_Pos(     FirstMenu *m,
                                    struct Menu *mc);
void ActionMenu_Elem_Links(   FirstMenu *m,
                                    struct Menu *mc);
void ActionMenu_Elem_Boxes(   FirstMenu *m,
                                    struct Menu *mc);

/* -- Cursor -- */
void ActionMenu_Cursor_Pos(  FirstMenu *m,
                                   struct Menu *mc);
void ActionMenu_Cursor_Boxes(FirstMenu *m,
                                   struct Menu *mc);

/* --- Drawing --- */
void ActionMenu_Draw( struct Menu *mc,
                            SDL_Texture *rt, SDL_Renderer *r);
void ActionMenu_Update(FirstMenu *m,
                             struct n9Patch *n9,
                             SDL_Texture *rt,
                             SDL_Renderer *r);


#endif /* FIRST_MENU_H */
