
#include "RNG.h"
#include "platform.h"
#include "nourstest.h"

#include "game/game.h"

#include "menu/action.h"
#include "menu/action_platform.h"

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
    Menu *mc = IES_malloc(sizeof(struct Menu));
    *mc = Menu_default;
    mc->elem_box    = NULL;
    mc->elem_pos    = NULL;
    mc->elem_links  = NULL;
    mc->type        = MENU_TYPE_ACTION;
    mc->draw        = &ActionMenu_Draw;
    mc->visible     = true;
    mc->n9patch     = n9Patch_default;

    pActionMenu_Set(psm->platform, render_target, renderer);
    ActionMenu_Load(psm, &mc->n9patch);
    pActionMenu_Load(psm->platform, &mc->n9patch);

    /* -- Player Select Menu Menu -- */
    /* - loading fonts - */
    psm->pixelnours = PixelFont_New();

    PixelFont_Load(psm->pixelnours, renderer, PATH_JOIN("..", "assets", "fonts", "pixelnours.png"));
    SDL_assert(psm->pixelnours);

    /* --- TEST OPTIONS RENDERING --- */

    /* -- Option 1 -- */
    Menu_Option option = {MENU_OPTION_TRADE, 1};
    ActionMenu_Option_Add(psm, option);
    mc->elem_num = ActionMenu_Options_Num(psm);
    ActionMenu_Compute_Size(psm, &mc->n9patch);

    ActionMenu_Elem_Links(psm, mc);
    ActionMenu_Elem_Boxes(psm, mc);
    ActionMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    pActionMenu_Update(psm, &mc->n9patch);
    Filesystem_Texture_Dump(PATH_JOIN("menu_action", "ActionMenu_Option_1.png"), renderer,
                            pActionMenu_Texture(psm->platform), SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 2 -- */
    option.id = MENU_OPTION_SEIZE;
    ActionMenu_Option_Add(psm, option);
    mc->elem_num = ActionMenu_Options_Num(psm);
    ActionMenu_Compute_Size(psm, &mc->n9patch);

    ActionMenu_Elem_Links(psm, mc);
    ActionMenu_Elem_Boxes(psm, mc);
    ActionMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    pActionMenu_Update(psm, &mc->n9patch);
    Filesystem_Texture_Dump(PATH_JOIN("menu_action", "ActionMenu_Option_2.png"), renderer,
                            pActionMenu_Texture(psm->platform), SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 3 -- */
    option.id = MENU_OPTION_TALK;
    ActionMenu_Option_Add(psm, option);
    mc->elem_num = ActionMenu_Options_Num(psm);
    ActionMenu_Compute_Size(psm, &mc->n9patch);

    ActionMenu_Elem_Links(psm, mc);
    ActionMenu_Elem_Boxes(psm, mc);
    ActionMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    pActionMenu_Update(psm, &mc->n9patch);
    Filesystem_Texture_Dump(PATH_JOIN("menu_action", "ActionMenu_Option_3.png"), renderer,
                            pActionMenu_Texture(psm->platform), SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 4 -- */
    option.id = MENU_OPTION_ATTACK;
    ActionMenu_Option_Add(psm, option);
    mc->elem_num = ActionMenu_Options_Num(psm);
    ActionMenu_Compute_Size(psm, &mc->n9patch);

    ActionMenu_Elem_Links(psm, mc);
    ActionMenu_Elem_Boxes(psm, mc);
    ActionMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    pActionMenu_Update(psm, &mc->n9patch);
    Filesystem_Texture_Dump(PATH_JOIN("menu_action", "ActionMenu_Option_4.png"), renderer,
                            pActionMenu_Texture(psm->platform), SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 5 -- */
    option.id = MENU_OPTION_SEIZE;
    ActionMenu_Option_Add(psm, option);
    mc->elem_num = ActionMenu_Options_Num(psm);
    ActionMenu_Compute_Size(psm, &mc->n9patch);

    ActionMenu_Elem_Links(psm, mc);
    ActionMenu_Elem_Boxes(psm, mc);
    ActionMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    pActionMenu_Update(psm, &mc->n9patch);
    Filesystem_Texture_Dump(PATH_JOIN("menu_action", "ActionMenu_Option_5.png"), renderer,
                            pActionMenu_Texture(psm->platform), SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 6 -- */
    option.id = MENU_OPTION_DANCE;
    ActionMenu_Option_Add(psm, option);
    mc->elem_num = ActionMenu_Options_Num(psm);
    ActionMenu_Compute_Size(psm, &mc->n9patch);

    ActionMenu_Elem_Links(psm, mc);
    ActionMenu_Elem_Boxes(psm, mc);
    ActionMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    pActionMenu_Update(psm, &mc->n9patch);
    Filesystem_Texture_Dump(PATH_JOIN("menu_action", "ActionMenu_Option_6.png"), renderer,
                            pActionMenu_Texture(psm->platform), SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 7 -- */
    option.id = MENU_OPTION_RESCUE;
    ActionMenu_Option_Add(psm, option);
    mc->elem_num = ActionMenu_Options_Num(psm);
    ActionMenu_Compute_Size(psm, &mc->n9patch);

    ActionMenu_Elem_Links(psm, mc);
    ActionMenu_Elem_Boxes(psm, mc);
    ActionMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    pActionMenu_Update(psm, &mc->n9patch);
    Filesystem_Texture_Dump(PATH_JOIN("menu_action", "ActionMenu_Option_7.png"), renderer,
                            pActionMenu_Texture(psm->platform), SDL_PIXELFORMAT_ARGB8888, render_target);

    ActionMenu_Options_Reset(psm);
    /* -- Option 8 -- */
    option.id = MENU_OPTION_OPEN;
    ActionMenu_Option_Add(psm, option);
    mc->elem_num = ActionMenu_Options_Num(psm);
    ActionMenu_Compute_Size(psm, &mc->n9patch);

    ActionMenu_Elem_Links(psm, mc);
    ActionMenu_Elem_Boxes(psm, mc);
    ActionMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    pActionMenu_Update(psm, &mc->n9patch);
    Filesystem_Texture_Dump(PATH_JOIN("menu_action", "ActionMenu_Option_8.png"), renderer,
                            pActionMenu_Texture(psm->platform), SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 9 -- */
    option.id = MENU_OPTION_WAIT;
    ActionMenu_Option_Add(psm, option);
    mc->elem_num = ActionMenu_Options_Num(psm);
    ActionMenu_Compute_Size(psm, &mc->n9patch);

    ActionMenu_Elem_Links(psm, mc);
    ActionMenu_Elem_Boxes(psm, mc);
    ActionMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    pActionMenu_Update(psm, &mc->n9patch);
    Filesystem_Texture_Dump(PATH_JOIN("menu_action", "ActionMenu_Test.png"), renderer,
                            pActionMenu_Texture(psm->platform), SDL_PIXELFORMAT_ARGB8888, render_target);

    /* Map action menu*/
    ActionMenu_Options_Reset(psm);

    SDL_assert(DARR_NUM(psm->options) == 0);
    option.id = MENU_OPTION_UNITS;
    ActionMenu_Option_Add(psm, option);
    option.id = MENU_OPTION_CONVOY;
    ActionMenu_Option_Add(psm, option);
    option.id = MENU_OPTION_QUIT;
    ActionMenu_Option_Add(psm, option);
    option.id = MENU_OPTION_END_TURN;
    ActionMenu_Option_Add(psm, option);
    mc->elem_num = ActionMenu_Options_Num(psm);
    ActionMenu_Compute_Size(psm, &mc->n9patch);
    SDL_assert(mc->elem_num == 4);

    pActionMenu_Update(psm, &mc->n9patch);
    Filesystem_Texture_Dump(PATH_JOIN("menu_action", "ActionMenu_Map_Action.png"),
                            renderer, pActionMenu_Texture(psm->platform), SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- TEST OPTIONS AS A FUNCTION OF GAME STATE --- */

    /* --- SDL_free --- */
    PixelFont_Free(psm->pixelnours, true);
    SDL_FreeSurface(surface);
    ActionMenu_Free(psm, mc);
    SDL_free(mc);
    SDL_DestroyRenderer(renderer);
}
