
#include "nourstest.h"
#include "platform.h"
#include "popup/objective.h"
#include "unit/unit.h"

void test_popup_objective() {

    /* -- Preliminaries -- */
    sota_mkdir("popup_objective");

    /* -- Tinymt -- */
    struct TINYMT32_T tinymt32;
    RNG_Init_tinymt(&tinymt32);

    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    /* -- Create n9patch -- */
    // render_target is NULL cause there is render_target!
    struct n9Patch n9patch;
    SDL_Texture *render_target = NULL;
    struct PopUp_Objective *po = PopUp_Objective_Alloc();
    PopUp_Objective_Load(po, renderer, &n9patch);

    /* - loading fonts - */
    po->pixelnours = PixelFont_Alloc();
    PixelFont_Load(po->pixelnours, renderer, PATH_JOIN("..", "assets", "fonts", "pixelnours.png"));

    /* - Objective - */
    PopUp_Objective_Set_Obj(po, "Rout");
    PopUp_Objective_Update(po, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_objective", "PopupObjective_Lower.png"), renderer,
                            po->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - Sub-Objective - */
    // PopUp_Objective_Set_Obj(po, "Rout");

    /* -- SDL_free -- */
    PixelFont_Free(po->pixelnours, true);
    PopUp_Objective_Free(po);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
    SDL_Quit();
}