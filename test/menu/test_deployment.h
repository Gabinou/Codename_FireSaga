#include "nourstest.h"
#include "platform.h"
#include "menu/deployment.h"
#include "unit/unit.h"
#include "RNG.h"

struct Unit party[SOTA_MAX_PARTY_SIZE];
i16 party_stack[SOTA_MAX_PARTY_SIZE];

void test_menu_deployment_party(struct DeploymentMenu *dm) {
    /* -- Party -- */
    /* - Preliminaries - */
    struct Unit party[SOTA_MAX_PARTY_SIZE];
    struct dtab *weapons_dtab   = DTAB_INIT(weapons_dtab,   struct Weapon);
    struct dtab *items_dtab     = DTAB_INIT(items_dtab,     struct Item);
    /* -- Adding units to Party -- */
    dm->party_stack = party_stack;
    dm->party = party;

    dm->party_size = 0;
    dm->party_stack[dm->party_size++] = UNIT_ID_SILOU;
    dm->party_stack[dm->party_size++] = UNIT_ID_KIARA;
    dm->party_stack[dm->party_size++] = UNIT_ID_RAYAN;

    Party_Load(dm->party, weapons_dtab, items_dtab, dm->party_stack, dm->party_size);

    /* -- Putting party on map -- */
}

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

    /* - loading party - */
    test_menu_deployment_party(dm);

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