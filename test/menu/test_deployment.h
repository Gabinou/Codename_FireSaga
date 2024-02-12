#include "nourstest.h"
#include "platform.h"
#include "menu/deployment.h"
#include "unit/unit.h"
#include "RNG.h"

void test_menu_deployment() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);
    /* --- Preliminaries --- */
    sota_mkdir("menu_deployment");

    /* -- Create n9patch -- */
    struct n9Patch n9patch = n9Patch_default;
    // NULL cause no target!
    SDL_Texture *render_target = NULL;

    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    /* -- Deployment Menu -- */
    struct DeploymentMenu *dm = DeploymentMenu_Alloc();
    DeploymentMenu_Load(dm, renderer, &n9patch);

    /* - loading fonts - */
    char *path = PATH_JOIN("..", "assets", "fonts", "pixelnours.png");

    PixelFont_Load(dm->pixelnours, renderer, path);
    SDL_assert(dm->pixelnours);

    path = PATH_JOIN("..", "assets", "fonts", "pixelnours_Big.png");
    PixelFont_Load(dm->pixelnours_big, renderer, path);
    SDL_assert(dm->pixelnours_big);

    path = PATH_JOIN("..", "assets", "fonts", "pixelnours_16_tight.png");
    PixelFont_Load(dm->pixelnours_16, renderer, path);
    SDL_assert(dm->pixelnours_big);


    /* --- RENDERS --- */
    /* -- Test page 1 -- */
    dm->page = 0;
    DeploymentMenu_Update(dm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_deployment", "DeploymentMenu_P1.png"),
                            renderer, dm->texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);
    /* -- Test page 2 -- */
    dm->page = 1;
    DeploymentMenu_Update(dm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_deployment", "DeploymentMenu_P2.png"),
                            renderer, dm->texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);

    /* -- Test page 3 -- */
    dm->page = 2;
    DeploymentMenu_Update(dm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_deployment", "DeploymentMenu_P3.png"),
                            renderer, dm->texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);

    /* -- Test page 4 -- */
    dm->page = 3;
    DeploymentMenu_Update(dm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_deployment", "DeploymentMenu_P4.png"),
                            renderer, dm->texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);










    /* --- FREE --- */
    DeploymentMenu_Free(dm);
    SDL_FreeSurface(surface);

    if (n9patch.texture != NULL)
        SDL_DestroyTexture(n9patch.texture);

    SDL_DestroyRenderer(renderer);
}