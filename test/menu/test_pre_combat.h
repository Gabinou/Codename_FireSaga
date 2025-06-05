
#include "nourstest.h"
#include "platform.h"
#include "popup/pre_combat.h"
#include "globals.h"
#include "unit/unit.h"
#include "RNG.h"
#define TEST_SET_EQUIPMENT_S(world, ID, eq) \
    seteqentity  = TNECS_ENTITY_CREATE_wCOMPONENTS(world, Inventory_item_ID);\
    seteqinvitem = IES_GET_COMPONENT(world, seteqentity, Inventory_item);\
    seteqinvitem->id = ID;\
    silou_eq[eq] = seteqentity;

#define TEST_SET_EQUIPMENT_H(world, ID, eq) \
    seteqentity  = TNECS_ENTITY_CREATE_wCOMPONENTS(world, Inventory_item_ID);\
    seteqinvitem = IES_GET_COMPONENT(world, seteqentity, Inventory_item);\
    seteqinvitem->id = ID;\
    hamilcar_eq[eq] = seteqentity;


void test_menu_pre_combat() {
    /* -- Preliminaries -- */
    tnecs_world *world = NULL;
    tnecs_world_genesis(&world);
    sota_mkdir("menu_pre_combat");
    tnecs_entity    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;

    /* -- Weapon dtab -- */
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);

    /* -- Create n9patch -- */
    struct n9Patch n9patch;
    // render_target is NULL cause there is render_target!
    SDL_Texture *render_target = NULL;

    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    n9patch                 = n9Patch_default;
    n9patch.patch_pixels.x  = PCP_PATCH_PIXELS;
    n9patch.patch_pixels.y  = PCP_PATCH_PIXELS;
    n9patch.scale.x         = PCP_N9PATCH_SCALE_X;
    n9patch.scale.y         = PCP_N9PATCH_SCALE_Y;
    n9patch.size_pixels.x   = (PCP_PATCH_PIXELS * PCP_PATCH_X_SIZE);
    n9patch.size_pixels.y   = (PCP_PATCH_PIXELS * PCP_PATCH_Y_SIZE);
    n9patch.size_patches.x  = PCP_PATCH_X_SIZE;
    n9patch.size_patches.y  = PCP_PATCH_Y_SIZE;
    n9patch.pos.x           = 0;
    n9patch.pos.y           = 0;
    char *path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png");
    n9patch.texture         = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    /* -- Stats Menu -- */
    struct PreCombatPopup *pcp = PreCombatPopup_Alloc();

    /* - loading fonts - */
    pcp->pixelnours = PixelFont_Alloc();
    path = PATH_JOIN("..", "assets", "fonts", "pixelnours.png");
    PixelFont_Load(pcp->pixelnours, renderer, path);
    nourstest_true(pcp->pixelnours);

    pcp->pixelnours_big = PixelFont_Alloc();
    path = PATH_JOIN("..", "assets", "fonts", "pixelnours_Big.png");
    PixelFont_Load(pcp->pixelnours_big, renderer, path);
    nourstest_true(pcp->pixelnours_big);

    /* -- Create Combatants -- */
    struct Unit Silou = Unit_default;
    Unit_Init(&Silou);
    struct Unit Hamilcar = Unit_default;
    Unit_Init(&Hamilcar);
    nourstest_true(Silou.equipment.num    == 0);
    nourstest_true(Hamilcar.equipment.num == 0);
    jsonio_readJSON(s8_literal(PATH_JOIN("units", "Silou_test.json")), &Silou);
    nourstest_true(Silou.equipment.num == 4);
    jsonio_readJSON(s8_literal(PATH_JOIN("units", "Hamilcar_test.json")), &Hamilcar);

    int stronghand  = Unit_Hand_Strong(&Silou);
    int weakhand    = Unit_Hand_Weak(&Silou);

    /* - Combatants equip - */
    struct Inventory_item in_wpn = Inventory_item_default;
    in_wpn.id = ITEM_ID_FLEURET;
    in_wpn.used = 0;
    Weapon_Load(gl_weapons_dtab, in_wpn.id);
    // Unit_Item_Drop(&Silou,           weakhand);
    // Unit_Item_Takeat(&Silou, in_wpn, weakhand);
    nourstest_true(Silou.equipment.num == 4);
    Unit_Equip(&Silou, weakhand, weakhand);
    Unit_Equip(&Silou, stronghand, stronghand);
    // Silou has BALL LIGHTNING and WOODEN SHIELD
    // Hamilcar has GLAIVE and WOODEN SHIELD

    Unit_Equip(&Hamilcar, weakhand, weakhand);
    Unit_Equip(&Hamilcar, stronghand, stronghand);
    SDL_assert(Unit_isEquipped(&Silou, weakhand));
    SDL_assert(Unit_isEquipped(&Hamilcar, weakhand));
    SDL_assert(Unit_isEquipped(&Hamilcar, stronghand));
    nourstest_true(Unit_canAttack(&Silou));
    nourstest_true(Unit_canAttack(&Hamilcar));

    // Unit_stats ES_S = Unit_effectiveStats(&Silou);

    struct Position silou_pos = Position_default;
    silou_pos.tilemap_pos.x = 0;
    silou_pos.tilemap_pos.y = 0;
    struct Position hamilcar_pos = Position_default;
    hamilcar_pos.tilemap_pos.x = 1;
    hamilcar_pos.tilemap_pos.y = 0;
    // int dist = 1;
    // Computed_Stats cs_S = Unit_computedStats(&Silou, dist, ES_S);

    // Unit_stats ES_H     = Unit_effectiveStats(&Hamilcar);
    // Computed_Stats cs_H = Unit_computedStats(&Hamilcar, dist, ES_H);
    struct Combat_Forecast combat_forecast;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    pcp->forecast  = &combat_forecast;
    _PreCombatPopup_Load(pcp, &Silou, &Hamilcar, &silou_pos, &hamilcar_pos, renderer);

    /* -- Single Digits -- */
    // cs_S.hit    = 1;
    // cs_S.dodge  = 0;
    // cs_H.hit    = 2;
    // cs_H.dodge  = 0;

    // cs_S.crit   = 3;
    // cs_S.favor  = 0;
    // // cs_H.crit   = 4;
    // cs_H.favor  = 0;

    // cs_S.crit   = 3;
    // cs_S.favor  = 0;
    // cs_H.crit   = 4;
    // cs_H.favor  = 0;

    // cs_S.attack.physical  = 5;
    // cs_S.attack.magical   = 0;
    // cs_H.attack.physical  = 6;
    // cs_H.attack.magical   = 0;

    // cs_S.protection.physical  = 0;
    // cs_S.protection.magical   = 0;
    // cs_H.protection.physical  = 0;
    // cs_H.protection.magical   = 0;

    /* -- Doubling Agg -- */
    // cs_S.speed = SOTA_DOUBLING_SPEED + 2;
    // cs_H.speed = 0;

    /* -- SIMPLE TOTAL MODE -- */
    pcp->mode = PCP_MODE_TOTAL;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatPopup_Doubling_Agg_Digit1.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SIMPLE SPLIT MODE -- */
    pcp->mode = PCP_MODE_SIMPLE;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatPopup_Split_Doubling_Agg_Digit1.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- MATH MODE -- */
    pcp->mode = PCP_MODE_MATH;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatPopup_Math_Doubling_Agg_Digit1.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Doubling Dft -- */
    // cs_H.speed = SOTA_DOUBLING_SPEED + 2;
    // cs_S.speed = 0;

    /* -- SIMPLE TOTAL MODE -- */
    pcp->mode = PCP_MODE_TOTAL;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatPopup_Doubling_Dft_Digit1.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SIMPLE SPLIT MODE -- */
    pcp->mode = PCP_MODE_SIMPLE;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatPopup_Split_Doubling_Dft_Digit1.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- MATH MODE -- */
    pcp->mode = PCP_MODE_MATH;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatPopup_Math_Doubling_Dft_Digit1.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Double Digits -- */
    // cs_S.hit = 10;
    // cs_S.dodge = 0;
    // cs_H.hit = 20;
    // cs_H.dodge = 0;

    // cs_S.crit = 30;
    // cs_S.favor = 0;
    // cs_H.crit = 40;
    // cs_H.favor = 0;

    // cs_S.crit = 30;
    // cs_S.favor = 0;
    // cs_H.crit = 40;
    // cs_H.favor = 0;

    // cs_S.attack.physical = 50;
    // cs_S.attack.magical = 10;
    // cs_H.attack.physical = 60;
    // cs_H.attack.magical = 20;

    // cs_S.protection.physical = 0;
    // cs_S.protection.magical = 0;
    // cs_H.protection.physical = 0;
    // cs_H.protection.magical = 0;

    /* -- Doubling Agg -- */
    // cs_S.speed = SOTA_DOUBLING_SPEED + 21;
    // cs_H.speed = 0;

    /* -- SIMPLE TOTAL MODE -- */
    pcp->mode = PCP_MODE_TOTAL;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatPopup_Doubling_Agg_Digit2.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SIMPLE SPLIT MODE -- */
    pcp->mode = PCP_MODE_SIMPLE;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatPopup_Split_Doubling_Agg_Digit2.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- MATH MODE -- */
    pcp->mode = PCP_MODE_MATH;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatPopup_Math_Doubling_Agg_Digit2.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Doubling Dft -- */
    // cs_H.speed = SOTA_DOUBLING_SPEED + 12;
    // cs_S.speed = 0;

    /* -- SIMPLE TOTAL MODE -- */
    pcp->mode = PCP_MODE_TOTAL;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatPopup_Doubling_Dft_Digit2.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SIMPLE SPLIT MODE -- */
    pcp->mode = PCP_MODE_SIMPLE;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatPopup_Split_Doubling_Dft_Digit2.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- MATH MODE -- */
    pcp->mode = PCP_MODE_MATH;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatPopup_Math_Doubling_Dft_Digit2.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Triple Digits -- */
    // cs_S.hit = 100;
    // cs_S.dodge = 0;
    // cs_H.hit = 100;
    // cs_H.dodge = 0;

    // cs_S.crit = 100;
    // cs_S.favor = 0;
    // cs_H.crit = 100;
    // cs_H.favor = 0;

    // cs_S.crit = 100;
    // cs_S.favor = 0;
    // cs_H.crit = 100;
    // cs_H.favor = 0;

    /* -- Doubling Agg -- */
    // cs_S.speed = SOTA_DOUBLING_SPEED + 12;
    // cs_H.speed = 0;

    /* -- SIMPLE TOTAL MODE -- */
    pcp->mode = PCP_MODE_TOTAL;

    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatPopup_Doubling_Agg_Digit3.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SIMPLE SPLIT MODE -- */
    pcp->mode = PCP_MODE_SIMPLE;

    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatPopup_Split_Doubling_Agg_Digit3.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- MATH MODE -- */
    pcp->mode = PCP_MODE_MATH;

    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatPopup_Math_Doubling_Agg_Digit3.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Doubling Dft -- */
    // cs_H.speed = SOTA_DOUBLING_SPEED + 12;
    // cs_S.speed = 0;

    /* -- SIMPLE TOTAL MODE -- */
    pcp->mode = PCP_MODE_TOTAL;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatPopup_Doubling_Dft_Digit3.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SIMPLE SPLIT MODE -- */
    pcp->mode = PCP_MODE_SIMPLE;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatPopup_Split_Doubling_Dft_Digit3.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- MATH MODE -- */
    pcp->mode = PCP_MODE_MATH;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatPopup_Math_Doubling_Dft_Digit3.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- TRUE DAMAGE -- */

    // cs_S.attack.physical = 50;
    // cs_S.attack.magical = 10;
    // cs_S.attack.True = 10;
    // cs_H.attack.physical = 60;
    // cs_H.attack.magical = 20;
    // cs_H.attack.True = 10;

    /* -- Doubling Agg -- */
    // cs_S.speed = SOTA_DOUBLING_SPEED + 12;
    // cs_H.speed = 0;

    /* -- SIMPLE TOTAL MODE -- */
    pcp->mode = PCP_MODE_TOTAL;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatPopup_True_Damage_Agg_Digit2.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SIMPLE SPLIT MODE -- */
    pcp->mode = PCP_MODE_SIMPLE;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatPopup_Split_True_Damage_Agg_Digit2.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- MATH MODE -- */
    pcp->mode = PCP_MODE_MATH;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatPopup_Math_True_Damage_Agg_Digit2.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Doubling Dft -- */
    // cs_H.speed = SOTA_DOUBLING_SPEED + 12;
    // cs_S.speed = 0;

    /* -- SIMPLE TOTAL MODE -- */
    pcp->mode = PCP_MODE_TOTAL;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatPopup_True_Damage_Dft_Digit2.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SIMPLE SPLIT MODE -- */
    pcp->mode = PCP_MODE_SIMPLE;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatPopup_Split_True_Dft_Damage_Digit2.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- MATH MODE -- */
    pcp->mode = PCP_MODE_MATH;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatPopup_Math_True_Dft_Damage_Digit2.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    // cs_S.attack.physical      = 50;
    // cs_S.attack.magical       = 10;
    // cs_S.attack.True          = 1;
    // cs_H.attack.physical   = 60;
    // cs_H.attack.magical    = 20;
    // cs_H.attack.True       = 1;

    /* -- Doubling Agg -- */
    // cs_S.speed      = SOTA_DOUBLING_SPEED + 12;
    // cs_H.speed   = 0;

    /* -- SIMPLE TOTAL MODE -- */
    pcp->mode = PCP_MODE_TOTAL;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatPopup_True_Damage_Agg_Digit1.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SIMPLE SPLIT MODE -- */
    pcp->mode = PCP_MODE_SIMPLE;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatPopup_Split_True_Damage_Agg_Digit1.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- MATH MODE -- */
    pcp->mode = PCP_MODE_MATH;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatPopup_Math_True_Damage_Agg_Digit1.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Doubling Dft -- */
    // cs_H.speed = SOTA_DOUBLING_SPEED + 12;
    // cs_S.speed = 0;

    /* -- SIMPLE TOTAL MODE -- */
    pcp->mode = PCP_MODE_TOTAL;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatPopup_True_Damage_Dft_Digit1.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SIMPLE SPLIT MODE -- */
    pcp->mode = PCP_MODE_SIMPLE;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatPopup_Split_True_Damage_Dft_Digit1.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- MATH MODE -- */
    pcp->mode = PCP_MODE_MATH;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatPopup_Math_True_Damage_Dft_Digit1.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Weapon icons -- */

    /* -- Unequip left -- */
    nourstest_true(Unit_isEquipped(&Silou, stronghand));
    Unit_Unequip(&Silou, stronghand);
    nourstest_true(!Unit_isEquipped(&Silou, stronghand));
    nourstest_true(Unit_isEquipped(&Hamilcar, stronghand));
    Unit_Unequip(&Hamilcar, stronghand);
    nourstest_true(!Unit_isEquipped(&Hamilcar, stronghand));

    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatPopup_Unequip_Left_Hand.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Unequip Right -- */
    Unit_Equip(&Silou, stronghand, stronghand);
    Unit_Unequip(&Silou, weakhand);
    Unit_Equip(&Hamilcar, stronghand, stronghand);
    Unit_Unequip(&Hamilcar, weakhand);

    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatPopup_Unequip_Right_Hand.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Two handing -- */
    nourstest_true(!Unit_istwoHanding(&Silou));
    nourstest_true(!Unit_istwoHanding(&Hamilcar));
    Unit_Item_Drop(&Hamilcar, weakhand);
    Unit_Item_Drop(&Silou, weakhand);

    tnecs_entity *silou_eq = Unit_Equipment(&Silou);
    TEST_SET_EQUIPMENT_S(world, ITEM_ID_BROADSWORD, 0);
    TEST_SET_EQUIPMENT_S(world, ITEM_ID_BROADSWORD, 1);
    tnecs_entity *hamilcar_eq = Unit_Equipment(&Hamilcar);
    TEST_SET_EQUIPMENT_H(world, ITEM_ID_BROADSWORD, 0);
    TEST_SET_EQUIPMENT_H(world, ITEM_ID_BROADSWORD, 1);
    Unit_Equip(&Silou,    weakhand, stronghand);
    Unit_Equip(&Hamilcar, weakhand, stronghand);
    nourstest_true(Unit_istwoHanding(&Silou));
    nourstest_true(Unit_istwoHanding(&Hamilcar));

    Weapon_Load(gl_weapons_dtab, ITEM_ID_BROADSWORD);

    PreCombatPopup_Update(pcp, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatPopup_Two_Handing.png"),
                            renderer, pcp->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- SDL_free --- */
    Unit_Free(&Silou);
    Unit_Free(&Hamilcar);
    PixelFont_Free(pcp->pixelnours, true);
    PixelFont_Free(pcp->pixelnours_big, true);

    Game_Weapons_Free(&gl_weapons_dtab);
    SDL_FreeSurface(surface);
    PreCombatPopup_Free(pcp);

    if (n9patch.texture != NULL)
        SDL_DestroyTexture(n9patch.texture);

    SDL_DestroyRenderer(renderer);
    Unit_Free(&Silou);
    tnecs_world_destroy(&world);
}