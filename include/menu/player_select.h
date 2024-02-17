#ifndef PLAYER_SELECT_MENU_H
#define PLAYER_SELECT_MENU_H

#include "enums.h"
#include "pixelfonts.h"
#include "n9patch.h"
#include "events.h"
#include "nstr.h"
#include "types.h"
#include "stb_sprintf.h"
#include "SDL2/SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct Menu;

enum PLAYER_SELECT_ENUM {
    PSM_N9PATCH_SCALE_X =  6,
    PSM_N9PATCH_SCALE_Y =  6,
    PSM_PADDING_RIGHT   =  7,
    PSM_PADDING_TOP     =  5,
    PSM_PADDING_LEFT    =  7,
    PSM_PADDING_BOTTOM  =  3,
};

typedef void (*psm_maker_t)(struct Game *in_game, void *dat1, void *dat2);
extern psm_maker_t menuContentMakers[MENU_PLAYER_SELECT_END];

struct PlayerSelectMenu {
    b32 update;

    struct Point pos; /* MENU_POS_bOFFSET = 0 */

    char **option_names;        /* [option_i] */
    u32  *options;             /* [option_i] */
    SDL_Texture *texture;
    struct PixelFont *pixelnours;
    struct Padding menu_padding;

    u32 option_num;
    u32 id;
    i32 row_height; /* [pixels] total height is row_height * option_num */
    i32 text_width; /* [pixels] */
    i32 icon_width;
    i32 text_alignment;
};
extern struct PlayerSelectMenu PlayerSelectMenu_default;

/* --- Constructors/Destructors --- */
struct PlayerSelectMenu *PlayerSelectMenu_Alloc(void);
void FirstMenu_Load(       struct PlayerSelectMenu *m, SDL_Renderer *r, struct n9Patch *n9);
void PlayerSelectMenu_Free(struct PlayerSelectMenu *m, struct Menu *mc);
void PlayerSelectMenu_Load(struct PlayerSelectMenu *m, SDL_Renderer *r, struct n9Patch *n9);

/* --- Menu Elem properties --- */

/* --- Elem Move --- */
i32 PlayerSelectMenu_Elem_Move(struct Menu *mc, i32 direction);

/* -- Options -- */
void PlayerSelectMenu_Option_Add(   struct PlayerSelectMenu *m, u32 op);
int  PlayerSelectMenu_Option_Index( struct PlayerSelectMenu *m, u32 op);
void PlayerSelectMenu_Compute_Size( struct PlayerSelectMenu *m, struct n9Patch *n9);
void PlayerSelectMenu_Options_Reset(struct PlayerSelectMenu *m);

/* -- Elems -- */
// PlayerSelectMenu_Elem_Links SHOULD NOT NEED LINKS
void PlayerSelectMenu_Elem_Pos(  struct PlayerSelectMenu *m, struct Menu *mc);
void PlayerSelectMenu_Elem_Links(struct PlayerSelectMenu *m, struct Menu *mc);
void PlayerSelectMenu_Elem_Boxes(struct PlayerSelectMenu *m, struct Menu *mc);

/* -- Cursor -- */
void PlayerSelectMenu_Cursor_Pos(  struct PlayerSelectMenu *m, struct Menu *mc);
void PlayerSelectMenu_Cursor_Boxes(struct PlayerSelectMenu *m, struct Menu *mc);

/* --- Drawing --- */
void PlayerSelectMenu_Draw(  struct Menu *mc, SDL_Texture *rt, SDL_Renderer *r);
void PlayerSelectMenu_Update(struct PlayerSelectMenu *m, struct n9Patch *n9,
                             SDL_Texture *rt, SDL_Renderer *r);

/* --- DECLARE PLAYER_SELECT MENU CONTENT MAKERS --- */
#define REGISTER_ENUM(x) void makeContent_PSM_##x(struct Game * in_game, void * data_1, void * data_2);
#include "names/menu/player_select.h"
#undef REGISTER_ENUM
void makeContent_FirstMenu(struct Game *g);

#endif /* PLAYER_SELECT_MENU_H */
