
#include "nourstest.h"
#include "platform.h"
#include "menu/growths.h"
#include "unit/unit.h"
#include "graph.h"
#include "globals.h"
#include "RNG.h"

#define TEST_SET_EQUIPMENT(world, ID, eq) \
    seteqentity  = IES_E_CREATE_wC(world, Inventory_item_ID);\
    seteqinvitem = IES_GET_C(world, seteqentity, Inventory_item);\
    seteqinvitem->id = ID;\
    silou_eq[eq] = seteqentity;

void test_menu_growths() {
    tnecs_E    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;
    // tnecs_E    seteqentity     = TNECS_NULL;
    // Inventory_item *seteqinvitem    = NULL;
    /* --- Preliminaries --- */
    sota_mkdir("menu_growths");

    tnecs_W *world = NULL;
    tnecs_genesis(&world);
    gl_world = world;

#include "register/components.h"

    /* -- Weapon dtab -- */
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab = DTAB_INIT(gl_items_dtab, struct Item);

    /* -- Create n9patch -- */
    struct n9Patch n9patch = n9Patch_default;
    // NULL cause no target!
    SDL_Texture *render_target = NULL;

    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    /* -- Growths Menu -- */
    struct GrowthsMenu *gm = GrowthsMenu_Alloc();
    GrowthsMenu_Load(gm, renderer, &n9patch);
    gm->update_stats = false;
    i16 stat_toplot = 1;

    /* - loading fonts - */
    char *path = PATH_JOIN("..", "assets", "fonts", "pixelnours.png");

    PixelFont_Load(gm->pixelnours, renderer, path);
    SDL_assert(gm->pixelnours);

    path = PATH_JOIN("..", "assets", "fonts", "pixelnours_Big.png");
    PixelFont_Load(gm->pixelnours_big, renderer, path);
    SDL_assert(gm->pixelnours_big);

    /* -- Create Unit -- */
    struct Unit Silou = Unit_default;
    struct Unit_stats *grown = Unit_Stats_Grown(&Silou);
    SDL_assert(grown == NULL);
    Unit_Init(&Silou);
    SDL_assert(Silou.equipment.num == 0);
    SDL_assert(world != NULL);
    jsonio_readJSON(s8_literal(PATH_JOIN("units", "Silou_test.json")), &Silou);
    grown = Unit_Stats_Grown(&Silou);
    SDL_assert(grown != NULL);
    SDL_assert(Silou.equipment.num == 4);

    /* - Unit equip - */
    tnecs_E *silou_eq = Unit_Equipment(&Silou);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, 0);
    Inventory_item *invitem = Unit_InvItem(&Silou, ITEM1);
    SDL_assert(invitem != NULL);
    Weapon_Load(gl_weapons_dtab, invitem->id);

    int weakhand    = Unit_Hand_Weak(&Silou);

    Unit_Item_Drop(&Silou,           weakhand);
    Unit_Item_Takeat(&Silou, seteqentity, weakhand);
    SDL_assert(Silou.equipment.num == 4);
    Unit_Equip(&Silou, weakhand, ITEM1);
    // Silou._equipped[weakhand] = true;
    SDL_assert(Unit_isEquipped(&Silou, weakhand));
    nourstest_true(Unit_canAttack(&Silou));
    Unit_stats ES_S = Unit_effectiveStats(&Silou);
    Unit_computedStats(&Silou, -1, ES_S);
    Unit_effectiveGrowths(&Silou);
    Unit_supportBonus(&Silou);
    gm->unit = &Silou;

    /* --- RENDERS --- */
    /* -- test no growths -- */
    GrowthsMenu_Update(gm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_growths", "GrowthsMenu_Test.png"), renderer, gm->texture,
                            SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- 1 growth -- */
    struct Unit_stats grown_1[1] = {
        {00, 01, 00, 00, 00, 01, 00, 00, 00, 00, 00, 00},
    };
    _Graph_Stat_Add(&gm->graph, &Silou.stats.bases, grown_1, 2, 1, stat_toplot);
    /* SDL_free alloced in Unit_readJSON grown_stats*/
    grown = Unit_Stats_Grown(&Silou);

    DARR_FREE(grown);
    Silou.stats.grown = &grown_1[0];
    grown = Silou.stats.grown;
    GrowthsMenu_Update(gm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_growths", "GrowthsMenu_Grown_01.png"), renderer,
                            gm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    Graph_Stat_Remove(&gm->graph, stat_toplot);
    Silou.stats.grown = NULL;

    /* -- 2 growths -- */
    struct Unit_stats grown_2[2] = {
        {00, 01, 00, 00, 00, 01, 00, 00, 00, 00, 00, 00},
        {01, 01, 00, 00, 00, 00, 01, 01, 01, 00, 00, 00},
    };
    _Graph_Stat_Add(&gm->graph, &Silou.stats.bases,
                    grown_2, 13, 11, stat_toplot);
    grown = &grown_2[0];
    // Silou.support_bonus.speed = 50;

    GrowthsMenu_Update(gm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_growths", "GrowthsMenu_Grown_02.png"), renderer,
                            gm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    Graph_Stat_Remove(&gm->graph, stat_toplot);

    /* -- 10 growths -- */
    struct Unit_stats grown_10[10] = {
        {00, 01, 00, 00, 00, 01, 00, 00, 00, 00, 00, 00},
        {01, 01, 00, 00, 00, 00, 01, 01, 01, 00, 00, 00},
        {00, 01, 01, 00, 00, 01, 00, 01, 01, 00, 00, 01},
        {00, 01, 00, 00, 00, 00, 00, 01, 00, 00, 00, 01},
        {01, 00, 00, 00, 00, 01, 00, 01, 00, 00, 00, 01},
        {00, 00, 00, 00, 00, 00, 00, 01, 00, 00, 00, 01},
        {00, 01, 00, 00, 00, 01, 00, 01, 00, 00, 00, 00},
        {01, 00, 01, 00, 00, 01, 00, 01, 00, 00, 00, 00},
        {00, 01, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
        {00, 01, 00, 00, 00, 01, 00, 00, 01, 00, 00, 01},
    };
    _Graph_Stat_Add(&gm->graph, &Silou.stats.bases,
                    grown_10, 30, 20, stat_toplot);
    grown = &grown_10[0];

    GrowthsMenu_Update(gm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_growths", "GrowthsMenu_Grown_10.png"), renderer,
                            gm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    Graph_Stat_Remove(&gm->graph, stat_toplot);

    /* -- 40 growths -- */
    struct Unit_stats grown_40[39] = {
        {00, 01, 00, 00, 00, 01, 00, 00, 00, 00, 00, 00},
        {01, 01, 00, 00, 00, 00, 01, 01, 01, 00, 00, 00},
        {00, 01, 01, 00, 00, 01, 00, 01, 01, 00, 00, 01},
        {00, 01, 00, 00, 00, 00, 00, 01, 00, 00, 00, 01},
        {01, 00, 00, 00, 00, 01, 00, 01, 00, 00, 00, 01},
        {00, 00, 00, 00, 00, 00, 00, 01, 00, 00, 00, 01},
        {00, 01, 00, 00, 00, 01, 00, 01, 00, 00, 00, 00},
        {01, 00, 01, 00, 00, 01, 00, 01, 00, 00, 00, 00},
        {00, 01, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
        {00, 01, 00, 00, 00, 01, 00, 00, 01, 00, 00, 01},
        {00, 01, 00, 00, 00, 01, 00, 00, 00, 00, 00, 00},
        {01, 01, 00, 00, 00, 00, 01, 01, 01, 00, 00, 00},
        {00, 01, 01, 00, 00, 01, 00, 01, 01, 00, 00, 01},
        {00, 01, 00, 00, 00, 00, 00, 01, 00, 00, 00, 01},
        {01, 00, 00, 00, 00, 01, 00, 01, 00, 00, 00, 01},
        {00, 00, 00, 00, 00, 00, 00, 01, 00, 00, 00, 01},
        {00, 01, 00, 00, 00, 01, 00, 01, 00, 00, 00, 00},
        {01, 00, 01, 00, 00, 01, 00, 01, 00, 00, 00, 00},
        {00, 01, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
        {00, 01, 00, 00, 00, 01, 00, 00, 01, 00, 00, 01},
        {00, 01, 00, 00, 00, 01, 00, 00, 00, 00, 00, 00},
        {01, 01, 00, 00, 00, 00, 01, 01, 01, 00, 00, 00},
        {00, 01, 01, 00, 00, 01, 00, 01, 01, 00, 00, 01},
        {00, 01, 00, 00, 00, 00, 00, 01, 00, 00, 00, 01},
        {01, 00, 00, 00, 00, 01, 00, 01, 00, 00, 00, 01},
        {00, 00, 00, 00, 00, 00, 00, 01, 00, 00, 00, 01},
        {00, 01, 00, 00, 00, 01, 00, 01, 00, 00, 00, 00},
        {01, 00, 01, 00, 00, 01, 00, 01, 00, 00, 00, 00},
        {00, 01, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
        {00, 01, 00, 00, 00, 01, 00, 00, 01, 00, 00, 01},
        {00, 01, 00, 00, 00, 01, 00, 00, 00, 00, 00, 00},
        {01, 01, 00, 00, 00, 00, 01, 01, 01, 00, 00, 00},
        {00, 01, 01, 00, 00, 01, 00, 01, 01, 00, 00, 01},
        {00, 01, 00, 00, 00, 00, 00, 01, 00, 00, 00, 01},
        {01, 00, 00, 00, 00, 01, 00, 01, 00, 00, 00, 01},
        {00, 00, 00, 00, 00, 00, 00, 01, 00, 00, 00, 01},
        {00, 01, 00, 00, 00, 01, 00, 01, 00, 00, 00, 00},
        {01, 00, 01, 00, 00, 01, 00, 01, 00, 00, 00, 00},
        {00, 01, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
    };
    _Graph_Stat_Add(&gm->graph, &Silou.stats.bases, grown_40, 40, 1,
                    stat_toplot);
    // Silou.support_bonus.speed = - 5;

    grown = &grown_40[0];
    GrowthsMenu_Update(gm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_growths", "GrowthsMenu_Grown_40.png"), renderer,
                            gm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    Graph_Stat_Remove(&gm->graph, stat_toplot);

    grown = NULL;

    /* -- Supports -- */
    /* - 1 digit - */
    // /* - Hit - */
    // Silou.support_bonus.hit = 1;
    // Silou.support_bonus.dodge = 2;
    // /* - Crit - */
    // Silou.support_bonus.crit = 3;
    // Silou.support_bonus.favor = 4;
    // /* - Speed - */
    // Silou.support_bonus.speed = 5;
    // /* - Attack - */
    // Silou.support_bonus.attack.physical = 6;
    // Silou.support_bonus.attack.magical = 7;
    // /* - Prot - */
    // Silou.support_bonus.protection.physical = 8;
    // Silou.support_bonus.protection.magical = 9;
    /* - Move - */
    // Silou.growth.effective.move = 1;

    GrowthsMenu_Update(gm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_growths", "GrowthsMenu_Digits_1.png"), renderer,
                            gm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - 2 digits - */
    // /* - Hit - */
    // Silou.support_bonus.hit = 10;
    // Silou.support_bonus.dodge = 20;
    // /* - Crit - */
    // Silou.support_bonus.crit = 30;
    // Silou.support_bonus.favor = 40;
    // /* - Speed - */
    // Silou.support_bonus.speed = -50;
    // /* - Attack - */
    // Silou.support_bonus.attack.physical = 60;
    // Silou.support_bonus.attack.magical = 70;
    // /* - Prot - */
    // Silou.support_bonus.protection.physical = 80;
    // Silou.support_bonus.protection.magical = 90;
    /* - Move - */
    // Silou.growth.effective.move = 23;
    GrowthsMenu_Update(gm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_growths", "GrowthsMenu_Digits_2.png"), renderer,
                            gm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- SDL_free --- */
    Unit_Free(&Silou);
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
    GrowthsMenu_Free(gm);
    SDL_FreeSurface(surface);

    if (n9patch.texture != NULL)
        SDL_DestroyTexture(n9patch.texture);

    tnecs_finale(&world);
    gl_world = NULL;
    SDL_DestroyRenderer(renderer);
}
#undef TEST_SET_EQUIPMENT