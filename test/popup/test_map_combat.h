
#include "RNG.h"
#include "combat.h"
#include "globals.h"
#include "platform.h"
#include "nourstest.h"
#include "unit/unit.h"
#include "popup/map_combat.h"

#define TEST_SET_EQUIPMENT(world, unit, ID, eq) \
    seteqentity  = IES_E_CREATE_wC(world, Inventory_item_ID);\
    seteqinvitem = IES_GET_C(world, seteqentity, Inventory_item);\
    seteqinvitem->id = ID;\
    unit##_eq[eq] = seteqentity;

void test_popup_map_combat() {
    /* -- Preliminaries -- */
    sota_mkdir("popup_map_combat");
    Names_Load_All();

    tnecs_E    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;

    /* Tnecs init */
    tnecs_W *world = NULL;
    tnecs_genesis(&world);
    gl_world = world;

#include "register/components.h"

    /* -- Combat -- */
    // struct Combat_Forecast combat_forecast;
    // struct Combat_Phase combat_phases[SOTA_COMBAT_MAX_PHASES];
    // for (int i = 0; i < SOTA_COMBAT_MAX_PHASES; i++) {
    //     combat_phases[i] = Combat_Phase_default;
    // }
    struct Combat_Attack *combat_attacks;
    combat_attacks = DARR_INIT(combat_attacks, struct Combat_Attack, SOTA_COMBAT_MAX_ATTACKS);

    /* -- Weapon dtab -- */
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab   = DTAB_INIT(gl_items_dtab, Item);

    /* -- Create n9patch -- */
    // render_target is NULL cause there is render_target!
    struct n9Patch n9patch;
    // SDL_Texture *render_target = NULL;

    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    /* -- Create combatants -- */
    struct Unit aggressor  = Unit_default;
    struct Unit defendant  = Unit_default;
    Unit_Init(&aggressor);
    Unit_Init(&defendant);

    jsonio_readJSON(s8_literal(PATH_JOIN("units", "Silou_test.json")), &aggressor);
    jsonio_readJSON(s8_literal(PATH_JOIN("units", "Fencer_test.json")), &defendant);

    struct Point agg_tilemap_pos = {0, 0};
    struct Point dft_tilemap_pos = {0, 1};
    int dist = Pathfinding_Manhattan(agg_tilemap_pos, dft_tilemap_pos);

    /* - Combatants equip - */

    // int stronghand  = Unit_Han#d_Strong(&aggressor);
    int weakhand    = Unit_Hand_Weak(&aggressor);
    tnecs_E *aggressor_eq = Unit_Equipment(&aggressor);
    Unit_Item_Drop(&aggressor,           weakhand);
    TEST_SET_EQUIPMENT(world, aggressor, ITEM_ID_FLEURET, 0);
    Unit_Item_Takeat(&aggressor, seteqentity, weakhand);
    Unit_Equip(&aggressor, weakhand, weakhand);

    tnecs_E *defendant_eq = Unit_Equipment(&defendant);
    TEST_SET_EQUIPMENT(world, defendant, ITEM_ID_GALATINE, 0);
    Unit_Item_Drop(&defendant,           weakhand);
    Unit_Item_Takeat(&defendant, seteqentity, weakhand);
    Unit_Equip(&defendant, weakhand, weakhand);

    nourstest_true(Unit_isEquipped(&aggressor, weakhand));
    nourstest_true(Unit_isEquipped(&defendant, weakhand));
    nourstest_true(Unit_canAttack(&aggressor));
    nourstest_true(Unit_canAttack(&defendant));
    Unit_stats ES_A = Unit_effectiveStats(&aggressor);
    Unit_stats ES_D = Unit_effectiveStats(&defendant);
    Unit_computedStats(&aggressor, dist, ES_A);
    Unit_computedStats(&defendant, dist, ES_D);

    /* -- Create Popup_Map_Combat -- */
    struct PopUp_Map_Combat pmc = PopUp_Map_Combat_default;
    PopUp_Map_Combat_Load(&pmc, renderer, &n9patch);
    // TODO: use Es
    // pmc.aggressor = &aggressor;
    // pmc.defendant = &defendant;

    // pmc.forecast = &combat_forecast;
    // pmc.phases   = combat_phases;

    // combat_forecast = Compute_Combat_Forecast(&aggressor, &defendant,
    //                                           (struct Point *)&agg_tilemap_pos,
    //                                           (struct Point *)&dft_tilemap_pos);

    // /* - loading fonts - */
    // pmc.pixelnours_tight = PixelFont_Alloc();
    // char *path = PATH_JOIN("..", "assets", "fonts", "pixelnours_tight.png");
    // PixelFont_Load(pmc.pixelnours_tight, renderer, path);

    // pmc.pixelnours_big = PixelFont_Alloc();
    // path = PATH_JOIN("..", "assets", "fonts", "pixelnours_Big.png");
    // PixelFont_Load(pmc.pixelnours_big, renderer, path);
    // SDL_assert(pmc.pixelnours_big);

    // pmc.forecast->flow.aggressor_phases = 2;
    // pmc.forecast->flow.defendant_phases = 2;
    // /* -- Brave = 2 -- */
    // pmc.phases[0].attack_num    = 2;
    // pmc.phases[0].attacker      = SOTA_AGGRESSOR;
    // /* -- Brave = 4 -- */
    // pmc.phases[1].attack_num    = 4;
    // pmc.phases[1].attacker      = SOTA_DEFENDANT;
    // /* -- Brave = 2, Astra = 4 -- */
    // pmc.phases[2].attack_num    = 8;
    // pmc.phases[2].attacker      = SOTA_AGGRESSOR;
    // /* -- Brave = 2, Astra = 5 -- */
    // pmc.phases[3].attack_num    = 10;
    // pmc.phases[3].attacker      = SOTA_DEFENDANT;

    // /* - attack = 0 - */
    // pmc.current_attack = 0;
    // pmc.dft_pos = (struct Point *)&dft_tilemap_pos;
    // pmc.agg_pos = (struct Point *)&agg_tilemap_pos;

    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave02_att0.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    // /* - attack = 1 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave02_att1.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    // /* - attack = 2 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave02_att2.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    // /* -- Brave = 2, Astra = 2 -- */
    // /* - attack = 1 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave04_att1.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    // /* - attack = 2 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave04_att2.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    // /* - attack = 3 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave04_att3.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    // /* - attack = 4 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave04_att4.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    // /* - attack = 1 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave08_att1.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    // /* - attack = 2 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave08_att2.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    // /* - attack = 3 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave08_att3.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    // /* - attack = 4 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave08_att4.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    // /* - attack = 5 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave08_att5.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    // /* - attack = 6 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave08_att6.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    // /* - attack = 7 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave08_att7.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    // /* - attack = 8 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave08_att8.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    // /* - attack = 1 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave10_att01.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    // /* - attack = 2 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave10_att02.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    // /* - attack = 3 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave10_att03.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    // /* - attack = 4 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave10_att04.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    // /* - attack = 5 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave10_att05.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    // /* - attack = 6 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave10_att06.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    // /* - attack = 7 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave10_att07.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    // /* - attack = 8 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave10_att08.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    // /* - attack = 9 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave10_att09.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    // /* - attack = 10 - */
    // pmc.current_attack++;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Brave10_att10.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_NoHP.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    // // /* -- No HP -- */
    // // // Draw draws to the screen -> no need, use Update instead
    // // /* -- Full HP -- */
    // // aggressor.counters.hp = aggressor.effective_stats.hp;
    // // defendant.counters.hp = defendant.effective_stats.hp;
    // // pmc.agg_current_hp = aggressor.counters.hp;
    // // pmc.dft_current_hp = defendant.counters.hp;
    // // SDL_assert(aggressor.counters.hp > 0);
    // // SDL_assert(defendant.counters.hp > 0);
    // // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_FullHP.png"), renderer,
    // //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    // /* -- 1 HP -- */
    // aggressor.counters.hp = 1;
    // defendant.counters.hp = 1;
    // pmc.agg_current_hp = aggressor.counters.hp;
    // pmc.dft_current_hp = defendant.counters.hp;
    // aggressor.stats.current.hp = SOTA_MAX_HP;
    // defendant.stats.current.hp = SOTA_MAX_HP;
    // SDL_assert(aggressor.counters.hp > 0);
    // SDL_assert(defendant.counters.hp > 0);
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_OneHP.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    // /* -- half HP -- */
    // aggressor.counters.hp = SOTA_MAX_HP / 2;
    // defendant.counters.hp = SOTA_MAX_HP / 2;
    // pmc.agg_current_hp = aggressor.counters.hp;
    // pmc.dft_current_hp = defendant.counters.hp;
    // aggressor.stats.current.hp = SOTA_MAX_HP;
    // defendant.stats.current.hp = SOTA_MAX_HP;
    // SDL_assert(aggressor.counters.hp > 0);
    // SDL_assert(defendant.counters.hp > 0);
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_HalfHP.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    // /* -- Single digit damage -- */
    // pmc.forecast->stats.agg_damage.dmg[DMG_TOTAL] = 1;
    // pmc.forecast->stats.dft_damage.dmg[DMG_TOTAL] = 2;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Damage_SingleDigit.png"),
    //                         renderer, pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    // /* -- Double digit damage -- */
    // pmc.forecast->stats.agg_damage.dmg[DMG_TOTAL] = 20;
    // pmc.forecast->stats.dft_damage.dmg[DMG_TOTAL] = 10;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Damage_DoubleDigit.png"),
    //                         renderer, pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    // /* -- Triple digit HIT -- */
    // /* -- Triple digit CRIT -- */
    // pmc.forecast->stats.agg_rates.hit   = 100;
    // pmc.forecast->stats.agg_rates.crit  =   2;
    // pmc.forecast->stats.dft_rates.hit   = 100;
    // pmc.forecast->stats.dft_rates.crit  =  40;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Digits_3.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    // /* -- Double digit HIT -- */
    // /* -- Double digit CRIT -- */
    // pmc.forecast->stats.agg_rates.hit   =  10;
    // pmc.forecast->stats.agg_rates.crit  = 100;
    // pmc.forecast->stats.dft_rates.hit   =   3;
    // pmc.forecast->stats.dft_rates.crit  = 100;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Digits_2.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    // /* -- Single digit HIT -- */
    // /* -- Single digit CRIT -- */
    // pmc.forecast->stats.agg_rates.hit   =  1;
    // pmc.forecast->stats.agg_rates.crit  = 20;
    // pmc.forecast->stats.dft_rates.hit   = 30;
    // pmc.forecast->stats.dft_rates.crit  =  4;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Digits_1.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    // /* -- No attack -- */
    // pmc.forecast->flow.aggressor_phases = 0;
    // pmc.forecast->flow.defendant_phases = 0;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_No_Attacks.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    // /* -- Singling -- */
    // pmc.forecast->flow.aggressor_phases = 1;
    // pmc.forecast->flow.defendant_phases = 1;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Phases_1.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    // /* -- Doubling -- */
    // pmc.forecast->flow.aggressor_phases = 2;
    // pmc.forecast->flow.defendant_phases = 2;
    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Phases_2.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    // /* -- Tripling -- */
    // pmc.forecast->flow.aggressor_phases = 3;
    // pmc.forecast->flow.defendant_phases = 1;
    // pmc.phases[0].attack_num            = 1;
    // pmc.phases[1].attack_num            = 1;
    // pmc.phases[2].attack_num            = 1;
    // pmc.phases[3].attack_num            = 1;
    // pmc.phases[0].attacker              = SOTA_AGGRESSOR;
    // pmc.phases[1].attacker              = SOTA_DEFENDANT;
    // pmc.phases[2].attacker              = SOTA_AGGRESSOR;
    // pmc.phases[3].attacker              = SOTA_AGGRESSOR;

    // PopUp_Map_Combat_Update(&pmc, &n9patch, render_target, renderer);
    // Filesystem_Texture_Dump(PATH_JOIN("popup_map_combat", "PopupMapCombat_Phases_3.png"), renderer,
    //                         pmc.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SDL_free -- */
    if (pmc.pixelnours_tight != NULL) {
        PixelFont_Free(pmc.pixelnours_tight, true);
    }

    if (pmc.pixelnours_big != NULL) {
        PixelFont_Free(pmc.pixelnours_big,   true);
    }

    PopUp_Map_Combat_Free(&pmc);
    DARR_FREE(combat_attacks);
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
    Unit_Free(&aggressor);
    Unit_Free(&defendant);
    tnecs_finale(&world);
    gl_world = NULL;
    SDL_Quit();
}

#undef TEST_SET_EQUIPMENT