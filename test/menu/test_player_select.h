
#include "RNG.h"
#include "platform.h"
#include "nourstest.h"

#include "game/game.h"

#include "menu/action.h"

#include "unit/unit.h"


void test_menu_action() {
    /* -- Preliminaries -- */
    sota_mkdir("menu_action");

    // render_target is NULL cause there is render_target!
    SDL_Texture *render_target = NULL;

    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    ActionMenu *psm = ActionMenu_Alloc();
    psm->row_height = ASCII_GLYPH_HEIGHT + 1; /* pixel fonts have python8 pixels*/

    /* -- Menu -- */
    struct Menu *mc = SDL_malloc(sizeof(struct Menu));
    *mc = Menu_default;
    mc->elem_box    = NULL;
    mc->elem_pos    = NULL;
    mc->elem_links  = NULL;
    mc->type        = MENU_TYPE_ACTION;
    mc->draw        = &ActionMenu_Draw;
    mc->visible     = true;
    mc->n9patch     = n9Patch_default;

    ActionMenu_Load(psm, renderer, &mc->n9patch);

    /* -- Player Select Menu Menu -- */
    /* - loading fonts - */
    psm->pixelnours = PixelFont_Alloc();

    PixelFont_Load(psm->pixelnours, renderer, PATH_JOIN("..", "assets", "fonts", "pixelnours.png"));
    SDL_assert(psm->pixelnours);

    /* --- TEST OPTIONS RENDERING --- */

    /* -- Option 1 -- */
    ActionMenu_Option_Add(psm, MENU_OPTION_TRADE, 1);
    mc->elem_num = PSM_Options_Num(psm);
    ActionMenu_Compute_Size(psm, &mc->n9patch);

    ActionMenu_Elem_Links(psm, mc);
    ActionMenu_Elem_Boxes(psm, mc);
    ActionMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    ActionMenu_Update(psm, &mc->n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_action", "ActionMenu_Option_1.png"), renderer,
                            psm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 2 -- */
    ActionMenu_Option_Add(psm, MENU_OPTION_SEIZE, 1);
    mc->elem_num = PSM_Options_Num(psm);
    ActionMenu_Compute_Size(psm, &mc->n9patch);

    ActionMenu_Elem_Links(psm, mc);
    ActionMenu_Elem_Boxes(psm, mc);
    ActionMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    ActionMenu_Update(psm, &mc->n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_action", "ActionMenu_Option_2.png"), renderer,
                            psm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 3 -- */
    ActionMenu_Option_Add(psm, MENU_OPTION_TALK, 1);
    mc->elem_num = PSM_Options_Num(psm);
    ActionMenu_Compute_Size(psm, &mc->n9patch);

    ActionMenu_Elem_Links(psm, mc);
    ActionMenu_Elem_Boxes(psm, mc);
    ActionMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    ActionMenu_Update(psm, &mc->n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_action", "ActionMenu_Option_3.png"), renderer,
                            psm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 4 -- */
    ActionMenu_Option_Add(psm, MENU_OPTION_ATTACK, 1);
    mc->elem_num = PSM_Options_Num(psm);
    ActionMenu_Compute_Size(psm, &mc->n9patch);

    ActionMenu_Elem_Links(psm, mc);
    ActionMenu_Elem_Boxes(psm, mc);
    ActionMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    ActionMenu_Update(psm, &mc->n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_action", "ActionMenu_Option_4.png"), renderer,
                            psm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 5 -- */
    ActionMenu_Option_Add(psm, MENU_OPTION_STAFF, 1);
    mc->elem_num = PSM_Options_Num(psm);
    ActionMenu_Compute_Size(psm, &mc->n9patch);

    ActionMenu_Elem_Links(psm, mc);
    ActionMenu_Elem_Boxes(psm, mc);
    ActionMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    ActionMenu_Update(psm, &mc->n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_action", "ActionMenu_Option_5.png"), renderer,
                            psm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 6 -- */
    ActionMenu_Option_Add(psm, MENU_OPTION_DANCE, 1);
    mc->elem_num = PSM_Options_Num(psm);
    ActionMenu_Compute_Size(psm, &mc->n9patch);

    ActionMenu_Elem_Links(psm, mc);
    ActionMenu_Elem_Boxes(psm, mc);
    ActionMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    ActionMenu_Update(psm, &mc->n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_action", "ActionMenu_Option_6.png"), renderer,
                            psm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 7 -- */
    ActionMenu_Option_Add(psm, MENU_OPTION_RESCUE, 1);
    mc->elem_num = PSM_Options_Num(psm);
    ActionMenu_Compute_Size(psm, &mc->n9patch);

    ActionMenu_Elem_Links(psm, mc);
    ActionMenu_Elem_Boxes(psm, mc);
    ActionMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    ActionMenu_Update(psm, &mc->n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_action", "ActionMenu_Option_7.png"), renderer,
                            psm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 8 -- */
    ActionMenu_Option_Add(psm, MENU_OPTION_OPEN, 1);
    mc->elem_num = PSM_Options_Num(psm);
    ActionMenu_Compute_Size(psm, &mc->n9patch);

    ActionMenu_Elem_Links(psm, mc);
    ActionMenu_Elem_Boxes(psm, mc);
    ActionMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    ActionMenu_Update(psm, &mc->n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_action", "ActionMenu_Option_8.png"), renderer,
                            psm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 9 -- */
    ActionMenu_Option_Add(psm, MENU_OPTION_WAIT, 1);
    mc->elem_num = PSM_Options_Num(psm);
    ActionMenu_Compute_Size(psm, &mc->n9patch);

    ActionMenu_Elem_Links(psm, mc);
    ActionMenu_Elem_Boxes(psm, mc);
    ActionMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    ActionMenu_Update(psm, &mc->n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_action", "ActionMenu_Test.png"), renderer,
                            psm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* Map action menu*/
    ActionMenu_Options_Reset(psm);

    SDL_assert(DARR_NUM(psm->options) == 0);
    ActionMenu_Option_Add(psm,    MENU_OPTION_UNITS, 1);
    ActionMenu_Option_Add(psm,    MENU_OPTION_CONVOY, 1);
    ActionMenu_Option_Add(psm,    MENU_OPTION_QUIT, 1);
    ActionMenu_Option_Add(psm,    MENU_OPTION_END_TURN, 1);
    mc->elem_num = PSM_Options_Num(psm);
    ActionMenu_Compute_Size(psm, &mc->n9patch);
    SDL_assert(DARR_NUM(psm->options) == 4);
    SDL_assert(mc->elem_num == 4);

    ActionMenu_Update(psm, &mc->n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_action", "ActionMenu_Map_Action.png"),
                            renderer, psm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- TEST OPTIONS AS A FUNCTION OF GAME STATE --- */

    /* --- SDL_free --- */
    PixelFont_Free(psm->pixelnours, true);
    SDL_FreeSurface(surface);
    ActionMenu_Free(psm, mc);
    SDL_free(mc);
    SDL_DestroyRenderer(renderer);
}
