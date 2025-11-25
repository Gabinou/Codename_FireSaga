
#include "globals.h"
#include "nourstest.h"
#include "platform.h"
#include "popup/unit.h"
#include "unit/unit.h"

void test_popup_unit() {
    /* -- Preliminaries -- */
    sota_mkdir("popup_unit");
    Names_Load_All();

    /* Tnecs init */
    tnecs_W *world = NULL;
    tnecs_genesis(&world);
    gl_world = world;

#include "register/components.h"


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
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab   = DTAB_INIT(gl_items_dtab, Item);
    Unit_Init(&Silou);

    /* - title - */
    SDL_assert(Silou.equipment.num == 0);
    jsonio_readJSON(s8_literal(PATH_JOIN("units", "Silou_test.json")), &Silou);
    SDL_assert(Silou.equipment.num == 4);

    _PopUp_Unit_Set(&pu, &Silou);

    /* - loading fonts - */
    pu.pixelnours = PixelFont_New();
    PixelFont_Load(pu.pixelnours, renderer, PATH_JOIN("assets", "fonts", "pixelnours.png"));

    pu.pixelnours_big = PixelFont_New();
    PixelFont_Load(pu.pixelnours_big, renderer, PATH_JOIN("assets", "fonts",
                                                          "pixelnours_Big.png"));
    SDL_assert(pu.pixelnours_big);

    PopUp_Unit_Load(&pu, renderer, &n9patch);

    PopUp_Unit_Update(&pu, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_unit", "PopupUnit_Test.png"),
                            renderer, pu.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SDL_free -- */
    Unit_Free(&Silou);
    n9Patch_Free(&n9patch);
    PixelFont_Delete(pu.pixelnours);
    PixelFont_Delete(pu.pixelnours_big);
    PopUp_Unit_Free(&pu);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
    SDL_Quit();
    tnecs_finale(&world);
    gl_world = NULL;

}