#include "nourstest.h"
#include "platform.h"
#include "menu/deployment.h"
#include "unit/unit.h"
#include "RNG.h"

#define PARTY_FOLDER "units"
#define DEBUG_MAP_FOLDER "units", "debug_map"
struct Game *sota;
struct Unit party[SOTA_MAX_PARTY_SIZE] = {0};
s8 filenames[SOTA_MAX_PARTY_SIZE];
i16 *party_id_stack;
struct Mount mount1;
struct Mount mount2;
struct Mount mount3;
struct Mount mount4;
struct dtab *weapons_dtab;
struct dtab *items_dtab;
struct Party party_struct;

void test_menu_deployment_party(struct DeploymentMenu *dm) {
    /* -- Party -- */
    /* - Preliminaries - */
    Party_Reset(&party_struct);
    Party_Folder(&party_struct, PATH_JOIN(PARTY_FOLDER));
    DARR_NUM(party_struct.json_ids) = 0;
    DARR_PUT(party_struct.json_ids, UNIT_ID_SILOU);
    DARR_PUT(party_struct.json_ids, UNIT_ID_KIARA);
    DARR_PUT(party_struct.json_ids, UNIT_ID_RAYAN);
    DARR_PUT(party_struct.json_ids, UNIT_ID_ERWIN);

    Party_Ids2Filenames(&party_struct);
    Party_Load(&party_struct, sota, weapons_dtab, items_dtab);

    mount1 = Mount_default_horse;
    party[UNIT_ID_SILOU].mount = &mount1;
    mount2 = Mount_default_pegasus;
    party[UNIT_ID_KIARA].mount = &mount2;
    mount3 = Mount_default_salamander;
    party[UNIT_ID_RAYAN].mount = &mount3;
    mount4 = Mount_default_eagle;
    party[UNIT_ID_ERWIN].mount = &mount4;
    SDL_assert(party[0].regrets == 0);
    SDL_assert(party[1].regrets == 0);
    SDL_assert(party[2].regrets == 0);
    SDL_assert(party[3].regrets == 0);

    DeploymentMenu_Party_Set(dm, &party_struct);
    SDL_assert(dm->_party_size > 0);
}

void test_menu_deployment_party_overfull(struct DeploymentMenu *dm) {
    /* -- Party -- */
    /* - Preliminaries - */
    /* -- Adding units to Party -- */
    Party_Reset(&party_struct);

    DARR_NUM(party_struct.json_ids) = 0;
    DARR_PUT(party_struct.json_ids, UNIT_ID_KIARA);
    DARR_PUT(party_struct.json_ids, UNIT_ID_PERIGNON);
    DARR_PUT(party_struct.json_ids, UNIT_ID_KAKWI);
    DARR_PUT(party_struct.json_ids, UNIT_ID_NICOLE);
    DARR_PUT(party_struct.json_ids, UNIT_ID_CHASSE);
    DARR_PUT(party_struct.json_ids, UNIT_ID_SILOU);
    DARR_PUT(party_struct.json_ids, UNIT_ID_LUCRECE);
    DARR_PUT(party_struct.json_ids, UNIT_ID_ERWIN);
    DARR_PUT(party_struct.json_ids, UNIT_ID_RAYAN);
    DARR_PUT(party_struct.json_ids, UNIT_ID_MELLY);
    DARR_PUT(party_struct.json_ids, UNIT_ID_TEHARON);
    SDL_assert(party[0].regrets == 0);
    SDL_assert(party[1].regrets == 0);
    SDL_assert(party[2].regrets == 0);
    SDL_assert(party[3].regrets == 0);
    SDL_assert(party[4].regrets == 0);
    SDL_assert(party[5].regrets == 0);
    SDL_assert(party[6].regrets == 0);
    SDL_assert(party[7].regrets == 0);
    SDL_assert(party[8].regrets == 0);
    SDL_assert(party[9].regrets == 0);
    SDL_assert(party[10].regrets == 0);

    Party_Ids2Filenames(&party_struct);
    Party_Load(&party_struct, sota, weapons_dtab, items_dtab);
    mount1 = Mount_default_horse;
    party[UNIT_ID_SILOU].mount = &mount1;
    mount2 = Mount_default_pegasus;
    party[UNIT_ID_KIARA].mount = &mount2;
    mount3 = Mount_default_salamander;
    party[UNIT_ID_RAYAN].mount = &mount3;
    mount4 = Mount_default_eagle;
    party[UNIT_ID_ERWIN].mount = &mount4;

    DeploymentMenu_Party_Set(dm, &party_struct);
    SDL_assert(dm->_party_size > 0);
    SDL_assert(dm->_party_size == 11);
}

void test_menu_deployment() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);
    /* --- Preliminaries --- */
    sota_mkdir("menu_deployment");

    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    sota = SDL_malloc(sizeof(struct Game));
    *sota = Game_default;
    sota->settings = Settings_default;
    sota->settings.window = false;
    char argv[1] = {0};
    Game_Init(sota, 0, &argv);

    party_struct = Party_default;
    weapons_dtab                    = DTAB_INIT(weapons_dtab,   struct Weapon);
    items_dtab                      = DTAB_INIT(items_dtab,     struct Item);
    party_struct.json_ids           = DARR_INIT(party_struct.json_ids, i16, 8);
    party_struct.json_filenames     = DARR_INIT(party_struct.json_filenames, s8, 8);

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
    dm->select_max = 2;

    /* - Underfull party - */
    test_menu_deployment_party(dm);
    dm->_selected[1] = true;

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

    /* - Overfull party - */
    test_menu_deployment_party_overfull(dm);
    dm->_selected[3] = true;

    /* -- Top unit 0 -- */
    dm->top_unit = 0;
    SDL_assert(_DeploymentMenu_Num(dm) == DM_UNIT_SHOWN_NUM);

    /* -- Test page 1 -- */
    dm->page = 0;
    DeploymentMenu_Update(dm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_deployment", "DeploymentMenu_P1_Overfull1.png"),
                            renderer, dm->texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);
    /* -- Test page 2 -- */
    dm->page = 1;
    DeploymentMenu_Update(dm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_deployment", "DeploymentMenu_P2_Overfull1.png"),
                            renderer, dm->texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);

    /* -- Test page 3 -- */
    dm->page = 2;
    DeploymentMenu_Update(dm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_deployment", "DeploymentMenu_P3_Overfull1.png"),
                            renderer, dm->texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);

    /* -- Test page 4 -- */
    dm->page = 3;
    DeploymentMenu_Update(dm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_deployment", "DeploymentMenu_P4_Overfull1.png"),
                            renderer, dm->texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);

    /* -- Top unit 0 -- */
    dm->top_unit = 1;
    SDL_assert(_DeploymentMenu_Num(dm) == DM_UNIT_SHOWN_NUM);

    /* -- Test page 1 -- */
    dm->page = 0;
    DeploymentMenu_Update(dm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_deployment", "DeploymentMenu_P1_Overfull2.png"),
                            renderer, dm->texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);
    /* -- Test page 2 -- */
    dm->page = 1;
    DeploymentMenu_Update(dm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_deployment", "DeploymentMenu_P2_Overfull2.png"),
                            renderer, dm->texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);

    /* -- Test page 3 -- */
    dm->page = 2;
    DeploymentMenu_Update(dm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_deployment", "DeploymentMenu_P3_Overfull2.png"),
                            renderer, dm->texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);

    /* -- Test page 4 -- */
    dm->page = 3;
    DeploymentMenu_Update(dm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_deployment", "DeploymentMenu_P4_Overfull2.png"),
                            renderer, dm->texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);

    /* -- Top unit 0 -- */
    dm->top_unit = 4;
    SDL_assert(_DeploymentMenu_Num(dm) == (dm->_party_size - dm->top_unit));
    SDL_assert(_DeploymentMenu_Num(dm) == 7);

    /* -- Test page 1 -- */
    dm->page = 0;
    DeploymentMenu_Update(dm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_deployment", "DeploymentMenu_P1_Overfull3.png"),
                            renderer, dm->texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);
    /* -- Test page 2 -- */
    dm->page = 1;
    DeploymentMenu_Update(dm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_deployment", "DeploymentMenu_P2_Overfull3.png"),
                            renderer, dm->texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);

    /* -- Test page 3 -- */
    dm->page = 2;
    DeploymentMenu_Update(dm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_deployment", "DeploymentMenu_P3_Overfull3.png"),
                            renderer, dm->texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);

    /* -- Test page 4 -- */
    dm->page = 3;
    DeploymentMenu_Update(dm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_deployment", "DeploymentMenu_P4_Overfull3.png"),
                            renderer, dm->texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);

    /* -- Top unit 0 -- */
    dm->top_unit = 7;
    SDL_assert(_DeploymentMenu_Num(dm) == (dm->_party_size - dm->top_unit));
    SDL_assert(_DeploymentMenu_Num(dm) == 4);

    /* -- Test page 1 -- */
    dm->page = 0;
    DeploymentMenu_Update(dm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_deployment", "DeploymentMenu_P1_Overfull4.png"),
                            renderer, dm->texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);
    /* -- Test page 2 -- */
    dm->page = 1;
    DeploymentMenu_Update(dm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_deployment", "DeploymentMenu_P2_Overfull4.png"),
                            renderer, dm->texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);

    /* -- Test page 3 -- */
    dm->page = 2;
    DeploymentMenu_Update(dm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_deployment", "DeploymentMenu_P3_Overfull4.png"),
                            renderer, dm->texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);

    /* -- Test page 4 -- */
    dm->page = 3;
    DeploymentMenu_Update(dm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_deployment", "DeploymentMenu_P4_Overfull4.png"),
                            renderer, dm->texture, SDL_PIXELFORMAT_ARGB8888,
                            render_target);

    /* --- FREE --- */
    s8_free(&party_struct.folder);
    Game_Party_Free(sota);
    Party_Reset(&party_struct);
    Game_Weapons_Free(&weapons_dtab);
    Game_Items_Free(&items_dtab);
    DeploymentMenu_Free(dm);
    SDL_FreeSurface(surface);

    if (n9patch.texture != NULL)
        SDL_DestroyTexture(n9patch.texture);

    SDL_DestroyRenderer(renderer);
    DARR_FREE(party_struct.json_ids);
    DARR_FREE(party_struct.json_filenames);
    Game_Free(sota);
}

#undef PARTY_FOLDER
#undef DEBUG_MAP_FOLDER
