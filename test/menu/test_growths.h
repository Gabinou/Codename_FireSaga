
#include "nourstest.h"
#include "platform.h"
#include "menu/growths.h"
#include "unit/unit.h"
#include "graph.h"
#include "globals.h"
#include "RNG.h"

void test_menu_growths() {
    /* --- Preliminaries --- */
    sota_mkdir("menu_growths");

    /* -- Weapon dtab -- */
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);

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
    jsonio_readJSON(s8_literal(PATH_JOIN("units", "Silou_test.json")), &Silou);
    grown = Unit_Stats_Grown(&Silou);
    SDL_assert(grown != NULL);
    SDL_assert(Silou.equipment.num == 4);

    /* - Unit equip - */
    struct Inventory_item in_wpn = Inventory_item_default;
    in_wpn.id   = ITEM_ID_FLEURET;
    in_wpn.used = 0;
    Weapon_Load(gl_weapons_dtab, in_wpn.id);

    int weakhand    = Unit_Hand_Weak(&Silou);

    Unit_Item_Drop(&Silou,           weakhand);
    Unit_Item_Takeat(&Silou, in_wpn, weakhand);
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
    Graph_Stat_Add(&gm->graph, &Silou.stats.bases, grown_1, 2, 1, stat_toplot);
    /* SDL_free alloced in Unit_readJSON grown_stats*/
    grown = Unit_Stats_Grown(&Silou);

    DARR_FREE(grown);
    grown = &grown_1[0];
    GrowthsMenu_Update(gm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_growths", "GrowthsMenu_Grown_01.png"), renderer,
                            gm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    Graph_Stat_Remove(&gm->graph, stat_toplot);

    /* -- 2 growths -- */
    struct Unit_stats grown_2[2] = {
        {00, 01, 00, 00, 00, 01, 00, 00, 00, 00, 00, 00},
        {01, 01, 00, 00, 00, 00, 01, 01, 01, 00, 00, 00},
    };
    Graph_Stat_Add(&gm->graph, &Silou.stats.bases, grown_2, 13, 11,
                   stat_toplot);
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
    Graph_Stat_Add(&gm->graph, &Silou.stats.bases, grown_10, 30, 20,
                   stat_toplot);
    grown = &grown_10[0];
    // Silou.support_bonus.speed = - 50;

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
    Graph_Stat_Add(&gm->graph, &Silou.stats.bases, grown_40, 40, 1,
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
    GrowthsMenu_Free(gm);
    SDL_FreeSurface(surface);

    if (n9patch.texture != NULL)
        SDL_DestroyTexture(n9patch.texture);

    SDL_DestroyRenderer(renderer);
}