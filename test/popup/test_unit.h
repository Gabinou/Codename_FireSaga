#include "nourstest.h"
#include "platform.h"
#include "popup/unit.h"
#include "unit/unit.h"

void test_popup_unit() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);
    /* -- Preliminaries -- */
    sota_mkdir("popup_unit");
    Names_Load_All();

    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    /* -- Create n9patch -- */
    // render_target is NULL cause there is render_target!
    struct n9Patch n9patch = n9Patch_default;
    SDL_Texture *render_target = NULL;

    /* -- Create PopUp_Loadout_Stats -- */
    struct PopUp_Unit pu = PopUp_Unit_default;

    /* -- Create Unit -- */
    struct Unit Silou = Unit_default;
    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);
    Unit_InitWweapons(&Silou, weapons_dtab);
    /* - title - */
    Silou.title = s8_mut("Playful Mage");
    Silou.weapons_dtab = weapons_dtab;
    SDL_assert(Silou.num_equipment == 0);
    jsonio_readJSON(s8_literal(PATH_JOIN("units", "Silou_test.json")), &Silou);
    SDL_assert(Silou.num_equipment == 4);
    SDL_assert(Silou.name.data != NULL);

    _PopUp_Unit_Set(&pu, &Silou);

    /* - loading fonts - */
    pu.pixelnours = PixelFont_Alloc();
    PixelFont_Load(pu.pixelnours, renderer, PATH_JOIN("..", "assets", "fonts", "pixelnours.png"));

    pu.pixelnours_big = PixelFont_Alloc();
    PixelFont_Load(pu.pixelnours_big, renderer, PATH_JOIN("..", "assets", "fonts",
                                                          "pixelnours_Big.png"));
    SDL_assert(pu.pixelnours_big);

    PopUp_Unit_Load(&pu, renderer, &n9patch);

    PopUp_Unit_Update(&pu, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_unit", "PopupUnit_Test.png"),
                            renderer, pu.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- FREE -- */
    Unit_Free(&Silou);
    n9Patch_Free(&n9patch);
    PixelFont_Free(pu.pixelnours, true);
    PixelFont_Free(pu.pixelnours_big, true);
    PopUp_Unit_Free(&pu);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
    Weapons_All_Free(weapons_dtab);
    DTAB_FREE(weapons_dtab);
    SDL_Quit();
}