
#include "nourstest.h"
#include "platform.h"
#include "menu/deployment.h"
#include "unit/unit.h"
#include "unit/mount.h"
#include "unit/flags.h"
#include "log.h"
#include "globals.h"
#include "RNG.h"
#include "macros.h"

#define PARTY_FOLDER "units"
#define DEBUG_MAP_FOLDER "units", "debug_map"
struct Game *sota;
s8 filenames[SOTA_MAX_PARTY_SIZE];
struct Mount mount1;
struct Mount mount2;
struct Mount mount3;
struct Mount mount4;

void test_menu_deployment_party(struct DeploymentMenu *dm) {
    /* -- Party -- */
    /* - Preliminaries - */
    Party_Reset(&sota->party);
    Party_Folder(&sota->party, PATH_JOIN(PARTY_FOLDER));
    DARR_NUM(sota->party.json_ids) = 0;
    DARR_PUT(sota->party.json_ids, UNIT_ID_SILOU);
    DARR_PUT(sota->party.json_ids, UNIT_ID_KIARA);
    DARR_PUT(sota->party.json_ids, UNIT_ID_RAYAN);
    DARR_PUT(sota->party.json_ids, UNIT_ID_ERWIN);

    Party_Ids2Filenames(&sota->party);
    SDL_assert(DARR_NUM(sota->party.json_filenames) > 0);
    Game_Party_Load(sota);

    SDL_assert(sota->party.Es[UNIT_ID_SILOU] > TNECS_NULL);
    SDL_assert(sota->party.Es[UNIT_ID_ERWIN] > TNECS_NULL);
    SDL_assert(sota->party.Es[UNIT_ID_KIARA] > TNECS_NULL);
    SDL_assert(sota->party.Es[UNIT_ID_RAYAN] > TNECS_NULL);
    SDL_assert(Party_Size(&sota->party) > 0);

    Unit *silou = IES_GET_C(gl_world, sota->party.Es[UNIT_ID_SILOU], Unit);
    Unit *erwin = IES_GET_C(gl_world, sota->party.Es[UNIT_ID_ERWIN], Unit);
    Unit *kiara = IES_GET_C(gl_world, sota->party.Es[UNIT_ID_KIARA], Unit);
    Unit *rayan = IES_GET_C(gl_world, sota->party.Es[UNIT_ID_RAYAN], Unit);
    SDL_assert(silou != NULL);
    SDL_assert(erwin != NULL);
    SDL_assert(kiara != NULL);
    SDL_assert(rayan != NULL);

    Mounts_Load();
    mount1 = gl_mounts[MOUNT_HORSIE];
    mount2 = gl_mounts[MOUNT_GOITEIA];
    mount3 = gl_mounts[MOUNT_NIBAL];
    mount4 = gl_mounts[MOUNT_MANWE];

    SDL_assert(Unit_Current_Regrets(silou) == 0);
    SDL_assert(Unit_Current_Regrets(erwin) == 0);
    SDL_assert(Unit_Current_Regrets(kiara) == 0);
    SDL_assert(Unit_Current_Regrets(rayan) == 0);

    SDL_assert(Party_Size(&sota->party) > 0);
    DeploymentMenu_Party_Set(dm, &sota->party);
    SDL_assert(dm->_party_size > 0);
}

void test_menu_deployment_party_overfull(struct DeploymentMenu *dm) {
    /* -- Party -- */
    /* - Preliminaries - */
    /* -- Adding units to Party -- */
    Party_Reset(&sota->party);

    DARR_NUM(sota->party.json_ids) = 0;
    DARR_PUT(sota->party.json_ids, UNIT_ID_KIARA);
    DARR_PUT(sota->party.json_ids, UNIT_ID_PERIGNON);
    DARR_PUT(sota->party.json_ids, UNIT_ID_KAKWI);
    DARR_PUT(sota->party.json_ids, UNIT_ID_NICOLE);
    DARR_PUT(sota->party.json_ids, UNIT_ID_CHASSE);
    DARR_PUT(sota->party.json_ids, UNIT_ID_SILOU);
    DARR_PUT(sota->party.json_ids, UNIT_ID_LUCRECE);
    DARR_PUT(sota->party.json_ids, UNIT_ID_ERWIN);
    DARR_PUT(sota->party.json_ids, UNIT_ID_RAYAN);
    DARR_PUT(sota->party.json_ids, UNIT_ID_MELLY);
    DARR_PUT(sota->party.json_ids, UNIT_ID_TEHARON);

    Party_Ids2Filenames(&sota->party);
    Game_Party_Load(sota);
    Unit *silou = IES_GET_C(gl_world, sota->party.Es[UNIT_ID_SILOU], Unit);
    Unit *erwin = IES_GET_C(gl_world, sota->party.Es[UNIT_ID_ERWIN], Unit);
    Unit *kiara = IES_GET_C(gl_world, sota->party.Es[UNIT_ID_KIARA], Unit);
    Unit *rayan = IES_GET_C(gl_world, sota->party.Es[UNIT_ID_RAYAN], Unit);
    SDL_assert(silou != NULL);
    SDL_assert(erwin != NULL);
    SDL_assert(kiara != NULL);
    SDL_assert(rayan != NULL);

    Mounts_Load();
    mount1 = gl_mounts[MOUNT_HORSIE];
    mount2 = gl_mounts[MOUNT_GOITEIA];
    mount3 = gl_mounts[MOUNT_NIBAL];
    mount4 = gl_mounts[MOUNT_MANWE];

    DeploymentMenu_Party_Set(dm, &sota->party);
    SDL_assert(dm->_party_size > 0);
    SDL_assert(dm->_party_size      == 11);
    SDL_assert(DARR_NUM(dm->party->id_stack)  == dm->_party_size);

    SDL_assert(dm->party->id_stack[0]  == UNIT_ID_ERWIN);
    SDL_assert(dm->party->id_stack[1]  == UNIT_ID_KIARA);
    SDL_assert(dm->party->id_stack[2]  == UNIT_ID_SILOU);
    SDL_assert(dm->party->id_stack[3]  == UNIT_ID_PERIGNON);
    SDL_assert(dm->party->id_stack[4]  == UNIT_ID_NICOLE);
    SDL_assert(dm->party->id_stack[5]  == UNIT_ID_CHASSE);
    SDL_assert(dm->party->id_stack[6]  == UNIT_ID_MELLY);
    SDL_assert(dm->party->id_stack[7]  == UNIT_ID_RAYAN);
    SDL_assert(dm->party->id_stack[8]  == UNIT_ID_TEHARON);
    SDL_assert(dm->party->id_stack[9]  == UNIT_ID_KAKWI);
    SDL_assert(dm->party->id_stack[10] == UNIT_ID_LUCRECE);
}

#define TEST_ROW_LEN 10
#define TEST_COL_LEN 10

void test_menu_deployment() {
    /* --- Preliminaries --- */
    sota_mkdir("menu_deployment");

    SDL_LogInfo(SOTA_LOG_SYSTEM, "Creating game object\n");
    struct Settings settings    = Settings_default;
    settings.window             = SDL_WINDOW_HIDDEN;
    sota                        = Game_New(settings);
    SDL_assert(gl_world != NULL);

    sota->party = Party_default;
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab,   struct Weapon);
    gl_items_dtab   = DTAB_INIT(gl_items_dtab,     struct Item);
    Party_Init(&sota->party);

    /* -- Create n9patch -- */
    struct n9Patch n9patch = n9Patch_default;
    /* NULL cause no target! */
    SDL_Texture *render_target = NULL;

    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    /* -- Deployment Menu -- */
    struct DeploymentMenu *dm = DeploymentMenu_Alloc();
    DeploymentMenu_Load(dm, renderer, &n9patch);
    dm->select_max = 2;
    dm->world = gl_world;
    dm->party = &sota->party;

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

    /* --- SDL_free --- */
    s8_free(&sota->party.folder);
    Game_Party_Free(sota);
    Party_Reset(&sota->party);
    Party_Free(&sota->party);
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
    DeploymentMenu_Free(dm);
    SDL_FreeSurface(surface);

    if (n9patch.texture != NULL)
        SDL_DestroyTexture(n9patch.texture);

    SDL_DestroyRenderer(renderer);
    Game_Free(sota);
}

#undef PARTY_FOLDER
#undef DEBUG_MAP_FOLDER
#undef TEST_ROW_LEN
#undef TEST_COL_LEN
