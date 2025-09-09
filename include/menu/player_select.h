#ifndef PLAYER_SELECT_MENU_H
#define PLAYER_SELECT_MENU_H
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
** Menu for player to choose dynamic number of options
**
*/

/* --- Menu FSMs --- */
// NOTE: my menu naming convention is BAD
// - player_select_menu -> player is SELECTING
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
**      - event -> state_fsm -> menu_fsm -> psm_fsm -> mo
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

enum PLAYER_SELECT_ENUM {
    PSM_N9PATCH_SCALE_X =  6,
    PSM_N9PATCH_SCALE_Y =  6,
    PSM_PADDING_RIGHT   =  7,
    PSM_PADDING_TOP     =  5,
    PSM_PADDING_LEFT    =  7,
    PSM_PADDING_BOTTOM  =  3,
};

typedef void (*psm_maker_t)(struct Game *in_game,
                            void *d1, void *d2);
extern const psm_maker_t menuContentMakers[MENU_PLAYER_SELECT_END];

typedef struct PSM_Option {
    s8  name;       /* [option_O] */
    u32 id;         /* [option_O] */
    b32 enabled;    /* [option_O] */
} PSM_Option;

typedef struct PlayerSelectMenu {
    struct Point pos; /* MENU_POS_bOFFSET = 0 */

    PSM_Option   *options;

    SDL_Texture  *texture;
    struct PixelFont    *pixelnours;
    Padding       menu_padding;

    u32 id;
    i32 row_height; /* [pixels] total height is row_height * option_num */
    i32 text_width; /* [pixels] */
    i32 icon_width;
    i32 text_alignment;
    b32 update;
} PlayerSelectMenu;
extern const PlayerSelectMenu PlayerSelectMenu_default;

/* --- Constructors/Destructors --- */
PlayerSelectMenu *PlayerSelectMenu_Alloc(void);
void FirstMenu_Load(    PlayerSelectMenu *m,
                        SDL_Renderer *r, struct n9Patch *n9);
void PlayerSelectMenu_Free( PlayerSelectMenu *m,
                            struct Menu *mc);
void PlayerSelectMenu_Load( PlayerSelectMenu *m,
                            SDL_Renderer *r, struct n9Patch *n9);

/* --- Menu Elem properties --- */
i32 PSM_Options_Num(const PlayerSelectMenu *m);

/* --- Elem Move --- */
i32 PlayerSelectMenu_Elem_Move(struct Menu *mc, i32 direction);

/* -- Options -- */
void PlayerSelectMenu_Option_Add(   PlayerSelectMenu *m,
                                    u32 op, b32 enabled);
int  PlayerSelectMenu_Option_Index( PlayerSelectMenu *m, u32 op);
void PlayerSelectMenu_Compute_Size( PlayerSelectMenu *m,
                                    struct n9Patch *n9);
void PlayerSelectMenu_Options_Reset(PlayerSelectMenu *m);

/* -- Elems -- */
// PlayerSelectMenu_Elem_Links SHOULD NOT NEED LINKS
void PlayerSelectMenu_Elem_Pos(     PlayerSelectMenu *m,
                                    struct Menu *mc);
void PlayerSelectMenu_Elem_Links(   PlayerSelectMenu *m,
                                    struct Menu *mc);
void PlayerSelectMenu_Elem_Boxes(   PlayerSelectMenu *m,
                                    struct Menu *mc);

/* -- Cursor -- */
void PlayerSelectMenu_Cursor_Pos(  PlayerSelectMenu *m,
                                   struct Menu *mc);
void PlayerSelectMenu_Cursor_Boxes(PlayerSelectMenu *m,
                                   struct Menu *mc);

/* --- Drawing --- */
void PlayerSelectMenu_Draw( struct Menu *mc,
                            SDL_Texture *rt, SDL_Renderer *r);
void PlayerSelectMenu_Update(PlayerSelectMenu *m,
                             struct n9Patch *n9,
                             SDL_Texture *rt,
                             SDL_Renderer *r);

/* --- DECLARE PLAYER_SELECT MENU CONTENT MAKERS --- */
#define REGISTER_ENUM(x) void makeContent_PSM_##x(struct Game * in_game, void * data_1, void * data_2);
#include "names/menu/player_select.h"
#undef REGISTER_ENUM
void makeContent_FirstMenu(struct Game *g);

#endif /* PLAYER_SELECT_MENU_H */
