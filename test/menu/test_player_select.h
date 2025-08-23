
#include "nourstest.h"
#include "platform.h"
#include "game/game.h"
#include "menu/player_select.h"
#include "unit/unit.h"
#include "RNG.h"

void test_menu_player_select() {
    /* -- Preliminaries -- */
    sota_mkdir("menu_player_select");

    // render_target is NULL cause there is render_target!
    SDL_Texture *render_target = NULL;

    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    struct PlayerSelectMenu *psm = PlayerSelectMenu_Alloc();
    psm->row_height = ASCII_GLYPH_HEIGHT + 1; /* pixel fonts have python8 pixels*/

    /* -- Menu -- */
    struct Menu *mc = SDL_malloc(sizeof(struct Menu));
    *mc = Menu_default;
    mc->elem_box = NULL;
    mc->elem_pos = NULL;
    mc->elem_links = NULL;
    mc->type = MENU_TYPE_PLAYER_SELECT;
    mc->draw = &PlayerSelectMenu_Draw;
    mc->visible = true;
    mc->n9patch = n9Patch_default;

    PlayerSelectMenu_Load(psm, renderer, &mc->n9patch);

    /* -- Player Select Menu Menu -- */
    /* - loading fonts - */
    psm->pixelnours = PixelFont_Alloc();

    PixelFont_Load(psm->pixelnours, renderer, PATH_JOIN("..", "assets", "fonts", "pixelnours.png"));
    SDL_assert(psm->pixelnours);

    /* --- TEST OPTIONS RENDERING --- */

    /* -- Option 1 -- */
    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_TRADE, 1);
    mc->elem_num = PSM_Options_Num(psm);
    PlayerSelectMenu_Compute_Size(psm, &mc->n9patch);

    PlayerSelectMenu_Elem_Links(psm, mc);
    PlayerSelectMenu_Elem_Boxes(psm, mc);
    PlayerSelectMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    PlayerSelectMenu_Update(psm, &mc->n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_player_select", "PlayerSelectMenu_Option_1.png"), renderer,
                            psm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 2 -- */
    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_SEIZE, 1);
    mc->elem_num = PSM_Options_Num(psm);
    PlayerSelectMenu_Compute_Size(psm, &mc->n9patch);

    PlayerSelectMenu_Elem_Links(psm, mc);
    PlayerSelectMenu_Elem_Boxes(psm, mc);
    PlayerSelectMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    PlayerSelectMenu_Update(psm, &mc->n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_player_select", "PlayerSelectMenu_Option_2.png"), renderer,
                            psm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 3 -- */
    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_TALK, 1);
    mc->elem_num = PSM_Options_Num(psm);
    PlayerSelectMenu_Compute_Size(psm, &mc->n9patch);

    PlayerSelectMenu_Elem_Links(psm, mc);
    PlayerSelectMenu_Elem_Boxes(psm, mc);
    PlayerSelectMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    PlayerSelectMenu_Update(psm, &mc->n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_player_select", "PlayerSelectMenu_Option_3.png"), renderer,
                            psm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 4 -- */
    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_ATTACK, 1);
    mc->elem_num = PSM_Options_Num(psm);
    PlayerSelectMenu_Compute_Size(psm, &mc->n9patch);

    PlayerSelectMenu_Elem_Links(psm, mc);
    PlayerSelectMenu_Elem_Boxes(psm, mc);
    PlayerSelectMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    PlayerSelectMenu_Update(psm, &mc->n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_player_select", "PlayerSelectMenu_Option_4.png"), renderer,
                            psm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 5 -- */
    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_STAFF, 1);
    mc->elem_num = PSM_Options_Num(psm);
    PlayerSelectMenu_Compute_Size(psm, &mc->n9patch);

    PlayerSelectMenu_Elem_Links(psm, mc);
    PlayerSelectMenu_Elem_Boxes(psm, mc);
    PlayerSelectMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    PlayerSelectMenu_Update(psm, &mc->n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_player_select", "PlayerSelectMenu_Option_5.png"), renderer,
                            psm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 6 -- */
    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_DANCE, 1);
    mc->elem_num = PSM_Options_Num(psm);
    PlayerSelectMenu_Compute_Size(psm, &mc->n9patch);

    PlayerSelectMenu_Elem_Links(psm, mc);
    PlayerSelectMenu_Elem_Boxes(psm, mc);
    PlayerSelectMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    PlayerSelectMenu_Update(psm, &mc->n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_player_select", "PlayerSelectMenu_Option_6.png"), renderer,
                            psm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 7 -- */
    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_RESCUE, 1);
    mc->elem_num = PSM_Options_Num(psm);
    PlayerSelectMenu_Compute_Size(psm, &mc->n9patch);

    PlayerSelectMenu_Elem_Links(psm, mc);
    PlayerSelectMenu_Elem_Boxes(psm, mc);
    PlayerSelectMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    PlayerSelectMenu_Update(psm, &mc->n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_player_select", "PlayerSelectMenu_Option_7.png"), renderer,
                            psm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 8 -- */
    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_OPEN, 1);
    mc->elem_num = PSM_Options_Num(psm);
    PlayerSelectMenu_Compute_Size(psm, &mc->n9patch);

    PlayerSelectMenu_Elem_Links(psm, mc);
    PlayerSelectMenu_Elem_Boxes(psm, mc);
    PlayerSelectMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    PlayerSelectMenu_Update(psm, &mc->n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_player_select", "PlayerSelectMenu_Option_8.png"), renderer,
                            psm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Option 9 -- */
    PlayerSelectMenu_Option_Add(psm, MENU_OPTION_WAIT, 1);
    mc->elem_num = PSM_Options_Num(psm);
    PlayerSelectMenu_Compute_Size(psm, &mc->n9patch);

    PlayerSelectMenu_Elem_Links(psm, mc);
    PlayerSelectMenu_Elem_Boxes(psm, mc);
    PlayerSelectMenu_Elem_Pos(psm, mc);
    Menu_Elem_Boxes_Check(mc);

    PlayerSelectMenu_Update(psm, &mc->n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_player_select", "PlayerSelectMenu_Test.png"), renderer,
                            psm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* Map action menu*/
    PlayerSelectMenu_Options_Reset(psm);

    SDL_assert(DARR_NUM(psm->options) == 0);
    PlayerSelectMenu_Option_Add(psm,    MENU_OPTION_UNITS, 1);
    PlayerSelectMenu_Option_Add(psm,    MENU_OPTION_CONVOY, 1);
    PlayerSelectMenu_Option_Add(psm,    MENU_OPTION_QUIT, 1);
    PlayerSelectMenu_Option_Add(psm,    MENU_OPTION_END_TURN, 1);
    mc->elem_num = PSM_Options_Num(psm);
    PlayerSelectMenu_Compute_Size(psm, &mc->n9patch);
    SDL_assert(DARR_NUM(psm->options) == 4);
    SDL_assert(mc->elem_num == 4);

    PlayerSelectMenu_Update(psm, &mc->n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_player_select", "PlayerSelectMenu_Map_Action.png"),
                            renderer, psm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- TEST OPTIONS AS A FUNCTION OF GAME STATE --- */

    /* --- SDL_free --- */
    PixelFont_Free(psm->pixelnours, true);
    SDL_FreeSurface(surface);
    PlayerSelectMenu_Free(psm, mc);
    SDL_free(mc);
    SDL_DestroyRenderer(renderer);
}
