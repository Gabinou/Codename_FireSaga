#include "nourstest.h"
#include "platform.h"
#include "menu/pre_combat.h"
#include "unit.h"
#include "RNG.h"

void test_menu_pre_combat() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);
    /* -- Preliminaries -- */
    sota_mkdir("menu_pre_combat");

    /* -- Weapon dtab -- */
    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);

    /* -- Create n9patch -- */
    struct n9Patch n9patch;
    // render_target is NULL cause there is render_target!
    SDL_Texture *render_target = NULL;

    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    n9patch                 = n9Patch_default;
    n9patch.patch_pixels.x  = PCM_PATCH_PIXELS;
    n9patch.patch_pixels.y  = PCM_PATCH_PIXELS;
    n9patch.scale.x         = PCM_N9PATCH_SCALE_X;
    n9patch.scale.y         = PCM_N9PATCH_SCALE_Y;
    n9patch.size_pixels.x   = (PCM_PATCH_PIXELS * PCM_PATCH_X_SIZE);
    n9patch.size_pixels.y   = (PCM_PATCH_PIXELS * PCM_PATCH_Y_SIZE);
    n9patch.size_patches.x  = PCM_PATCH_X_SIZE;
    n9patch.size_patches.y  = PCM_PATCH_Y_SIZE;
    n9patch.pos.x           = 0;
    n9patch.pos.y           = 0;
    char *path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png");
    n9patch.texture         = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);
    /* -- Stats Menu -- */
    struct PreCombatMenu *pcm = PreCombatMenu_Alloc();

    /* - loading fonts - */
    pcm->pixelnours = PixelFont_Alloc();
    path = PATH_JOIN("..", "assets", "Fonts", "pixelnours.png");
    PixelFont_Load(pcm->pixelnours, renderer, path);
    nourstest_true(pcm->pixelnours);

    pcm->pixelnours_big = PixelFont_Alloc();
    path = PATH_JOIN("..", "assets", "Fonts", "pixelnours_Big.png");
    PixelFont_Load(pcm->pixelnours_big, renderer, path);
    nourstest_true(pcm->pixelnours_big);

    /* -- Create Combatants -- */
    struct Unit Silou = Unit_default;
    struct Unit Hamilcar = Unit_default;
    Silou.weapons_dtab = weapons_dtab;
    Hamilcar.weapons_dtab = weapons_dtab;
    nourstest_true(Silou.num_equipment    == 0);
    nourstest_true(Hamilcar.num_equipment == 0);
    jsonio_readJSON(PATH_JOIN("units", "Silou_test.json"), &Silou);
    nourstest_true(Silou.num_equipment == 4);
    jsonio_readJSON(PATH_JOIN("units", "Hamilcar_test.json"), &Hamilcar);

    /* - Combatants equip - */
    struct Inventory_item in_wpn = Inventory_item_default;
    in_wpn.id = ITEM_ID_FLEURET;
    in_wpn.used = 0;
    Weapon_Load(weapons_dtab, in_wpn.id);
    // Unit_Item_Drop(&Silou,           UNIT_HAND_WEAK);
    // Unit_Item_Takeat(&Silou, in_wpn, UNIT_HAND_WEAK);
    nourstest_true(Silou.num_equipment == 4);
    Silou.equipped[UNIT_HAND_WEAK] = true;
    Silou.equipped[UNIT_HAND_STRONG] = true;
    // Silou has BALL LIGHTNING and WOODEN SHIELD
    // Hamilcar has GLAIVE and WOODEN SHIELD

    Hamilcar.equipped[UNIT_HAND_WEAK] = true;
    Hamilcar.equipped[UNIT_HAND_STRONG] = true;
    SDL_assert(Silou.equipped[UNIT_HAND_WEAK]);
    SDL_assert(Hamilcar.equipped[UNIT_HAND_WEAK]);
    SDL_assert(Hamilcar.equipped[UNIT_HAND_STRONG]);
    nourstest_true(Unit_canAttack(&Silou));
    nourstest_true(Unit_canAttack(&Hamilcar));

    Unit_effectiveStats(&Silou);

    struct Position silou_pos = Position_default;
    silou_pos.tilemap_pos.x = 0;
    silou_pos.tilemap_pos.y = 0;
    struct Position hamilcar_pos = Position_default;
    hamilcar_pos.tilemap_pos.x = 1;
    hamilcar_pos.tilemap_pos.y = 0;
    int dist = 1;
    Unit_computedStats(&Silou, dist);

    Unit_computedStats(&Hamilcar, dist);
    Unit_effectiveStats(&Hamilcar);
    struct Combat_Forecast combat_forecast;
    combat_forecast = Compute_Combat_Forecast(&Silou, &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    pcm->forecast  = &combat_forecast;
    _PreCombatMenu_Load(pcm, &Silou, &Hamilcar, &silou_pos, &hamilcar_pos, renderer);

    /* --- RENDERS --- */
    Silou.update_stats      = false;
    Hamilcar.update_stats   = false;

    /* -- Single Digits -- */
    Silou.computed_stats.hit        = 1;
    Silou.computed_stats.dodge      = 0;
    Hamilcar.computed_stats.hit     = 2;
    Hamilcar.computed_stats.dodge   = 0;

    Silou.computed_stats.crit       = 3;
    Silou.computed_stats.favor      = 0;
    Hamilcar.computed_stats.crit    = 4;
    Hamilcar.computed_stats.favor   = 0;

    Silou.computed_stats.crit       = 3;
    Silou.computed_stats.favor      = 0;
    Hamilcar.computed_stats.crit    = 4;
    Hamilcar.computed_stats.favor   = 0;

    Silou.computed_stats.attack[DMG_TYPE_PHYSICAL]          = 5;
    Silou.computed_stats.attack[DMG_TYPE_MAGICAL]           = 0;
    Hamilcar.computed_stats.attack[DMG_TYPE_PHYSICAL]       = 6;
    Hamilcar.computed_stats.attack[DMG_TYPE_MAGICAL]        = 0;

    Silou.computed_stats.protection[DMG_TYPE_PHYSICAL]      = 0;
    Silou.computed_stats.protection[DMG_TYPE_MAGICAL]       = 0;
    Hamilcar.computed_stats.protection[DMG_TYPE_PHYSICAL]   = 0;
    Hamilcar.computed_stats.protection[DMG_TYPE_MAGICAL]    = 0;

    /* -- Doubling Agg -- */
    Silou.computed_stats.speed = SOTA_DOUBLING_SPEED + 2;
    Hamilcar.computed_stats.speed = 0;

    /* -- SIMPLE TOTAL MODE -- */
    pcm->mode = PCM_MODE_TOTAL;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_Doubling_Agg_Digit1.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SIMPLE SPLIT MODE -- */
    pcm->mode = PCM_MODE_SIMPLE;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_Split_Doubling_Agg_Digit1.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- MATH MODE -- */
    pcm->mode = PCM_MODE_MATH;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_Math_Doubling_Agg_Digit1.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Doubling Dft -- */
    Hamilcar.computed_stats.speed = SOTA_DOUBLING_SPEED + 2;
    Silou.computed_stats.speed = 0;

    /* -- SIMPLE TOTAL MODE -- */
    pcm->mode = PCM_MODE_TOTAL;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_Doubling_Dft_Digit1.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SIMPLE SPLIT MODE -- */
    pcm->mode = PCM_MODE_SIMPLE;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_Split_Doubling_Dft_Digit1.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- MATH MODE -- */
    pcm->mode = PCM_MODE_MATH;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_Math_Doubling_Dft_Digit1.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Double Digits -- */
    Silou.computed_stats.hit = 10;
    Silou.computed_stats.dodge = 0;
    Hamilcar.computed_stats.hit = 20;
    Hamilcar.computed_stats.dodge = 0;

    Silou.computed_stats.crit = 30;
    Silou.computed_stats.favor = 0;
    Hamilcar.computed_stats.crit = 40;
    Hamilcar.computed_stats.favor = 0;

    Silou.computed_stats.crit = 30;
    Silou.computed_stats.favor = 0;
    Hamilcar.computed_stats.crit = 40;
    Hamilcar.computed_stats.favor = 0;

    Silou.computed_stats.attack[DMG_TYPE_PHYSICAL] = 50;
    Silou.computed_stats.attack[DMG_TYPE_MAGICAL] = 10;
    Hamilcar.computed_stats.attack[DMG_TYPE_PHYSICAL] = 60;
    Hamilcar.computed_stats.attack[DMG_TYPE_MAGICAL] = 20;

    Silou.computed_stats.protection[DMG_TYPE_PHYSICAL] = 0;
    Silou.computed_stats.protection[DMG_TYPE_MAGICAL] = 0;
    Hamilcar.computed_stats.protection[DMG_TYPE_PHYSICAL] = 0;
    Hamilcar.computed_stats.protection[DMG_TYPE_MAGICAL] = 0;

    /* -- Doubling Agg -- */
    Silou.computed_stats.speed = SOTA_DOUBLING_SPEED + 21;
    Hamilcar.computed_stats.speed = 0;

    /* -- SIMPLE TOTAL MODE -- */
    pcm->mode = PCM_MODE_TOTAL;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_Doubling_Agg_Digit2.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SIMPLE SPLIT MODE -- */
    pcm->mode = PCM_MODE_SIMPLE;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_Split_Doubling_Agg_Digit2.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- MATH MODE -- */
    pcm->mode = PCM_MODE_MATH;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_Math_Doubling_Agg_Digit2.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Doubling Dft -- */
    Hamilcar.computed_stats.speed = SOTA_DOUBLING_SPEED + 12;
    Silou.computed_stats.speed = 0;

    /* -- SIMPLE TOTAL MODE -- */
    pcm->mode = PCM_MODE_TOTAL;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_Doubling_Dft_Digit2.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SIMPLE SPLIT MODE -- */
    pcm->mode = PCM_MODE_SIMPLE;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_Split_Doubling_Dft_Digit2.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- MATH MODE -- */
    pcm->mode = PCM_MODE_MATH;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_Math_Doubling_Dft_Digit2.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Triple Digits -- */
    Silou.computed_stats.hit = 100;
    Silou.computed_stats.dodge = 0;
    Hamilcar.computed_stats.hit = 100;
    Hamilcar.computed_stats.dodge = 0;

    Silou.computed_stats.crit = 100;
    Silou.computed_stats.favor = 0;
    Hamilcar.computed_stats.crit = 100;
    Hamilcar.computed_stats.favor = 0;

    Silou.computed_stats.crit = 100;
    Silou.computed_stats.favor = 0;
    Hamilcar.computed_stats.crit = 100;
    Hamilcar.computed_stats.favor = 0;

    /* -- Doubling Agg -- */
    Silou.computed_stats.speed = SOTA_DOUBLING_SPEED + 12;
    Hamilcar.computed_stats.speed = 0;

    /* -- SIMPLE TOTAL MODE -- */
    pcm->mode = PCM_MODE_TOTAL;

    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_Doubling_Agg_Digit3.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SIMPLE SPLIT MODE -- */
    pcm->mode = PCM_MODE_SIMPLE;

    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_Split_Doubling_Agg_Digit3.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- MATH MODE -- */
    pcm->mode = PCM_MODE_MATH;

    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_Math_Doubling_Agg_Digit3.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Doubling Dft -- */
    Hamilcar.computed_stats.speed = SOTA_DOUBLING_SPEED + 12;
    Silou.computed_stats.speed = 0;

    /* -- SIMPLE TOTAL MODE -- */
    pcm->mode = PCM_MODE_TOTAL;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_Doubling_Dft_Digit3.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SIMPLE SPLIT MODE -- */
    pcm->mode = PCM_MODE_SIMPLE;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_Split_Doubling_Dft_Digit3.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- MATH MODE -- */
    pcm->mode = PCM_MODE_MATH;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_Math_Doubling_Dft_Digit3.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- TRUE DAMAGE -- */

    Silou.computed_stats.attack[DMG_TYPE_PHYSICAL] = 50;
    Silou.computed_stats.attack[DMG_TYPE_MAGICAL] = 10;
    Silou.computed_stats.attack[DMG_TYPE_TRUE] = 10;
    Hamilcar.computed_stats.attack[DMG_TYPE_PHYSICAL] = 60;
    Hamilcar.computed_stats.attack[DMG_TYPE_MAGICAL] = 20;
    Hamilcar.computed_stats.attack[DMG_TYPE_TRUE] = 10;

    /* -- Doubling Agg -- */
    Silou.computed_stats.speed = SOTA_DOUBLING_SPEED + 12;
    Hamilcar.computed_stats.speed = 0;

    /* -- SIMPLE TOTAL MODE -- */
    pcm->mode = PCM_MODE_TOTAL;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_True_Damage_Agg_Digit2.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SIMPLE SPLIT MODE -- */
    pcm->mode = PCM_MODE_SIMPLE;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatMenu_Split_True_Damage_Agg_Digit2.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- MATH MODE -- */
    pcm->mode = PCM_MODE_MATH;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatMenu_Math_True_Damage_Agg_Digit2.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Doubling Dft -- */
    Hamilcar.computed_stats.speed = SOTA_DOUBLING_SPEED + 12;
    Silou.computed_stats.speed = 0;

    /* -- SIMPLE TOTAL MODE -- */
    pcm->mode = PCM_MODE_TOTAL;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_True_Damage_Dft_Digit2.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SIMPLE SPLIT MODE -- */
    pcm->mode = PCM_MODE_SIMPLE;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatMenu_Split_True_Dft_Damage_Digit2.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- MATH MODE -- */
    pcm->mode = PCM_MODE_MATH;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatMenu_Math_True_Dft_Damage_Digit2.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Silou.computed_stats.attack[DMG_TYPE_PHYSICAL]      = 50;
    Silou.computed_stats.attack[DMG_TYPE_MAGICAL]       = 10;
    Silou.computed_stats.attack[DMG_TYPE_TRUE]          = 1;
    Hamilcar.computed_stats.attack[DMG_TYPE_PHYSICAL]   = 60;
    Hamilcar.computed_stats.attack[DMG_TYPE_MAGICAL]    = 20;
    Hamilcar.computed_stats.attack[DMG_TYPE_TRUE]       = 1;

    /* -- Doubling Agg -- */
    Silou.computed_stats.speed      = SOTA_DOUBLING_SPEED + 12;
    Hamilcar.computed_stats.speed   = 0;

    /* -- SIMPLE TOTAL MODE -- */
    pcm->mode = PCM_MODE_TOTAL;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_True_Damage_Agg_Digit1.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SIMPLE SPLIT MODE -- */
    pcm->mode = PCM_MODE_SIMPLE;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatMenu_Split_True_Damage_Agg_Digit1.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- MATH MODE -- */
    pcm->mode = PCM_MODE_MATH;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatMenu_Math_True_Damage_Agg_Digit1.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Doubling Dft -- */
    Hamilcar.computed_stats.speed = SOTA_DOUBLING_SPEED + 12;
    Silou.computed_stats.speed = 0;

    /* -- SIMPLE TOTAL MODE -- */
    pcm->mode = PCM_MODE_TOTAL;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_True_Damage_Dft_Digit1.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SIMPLE SPLIT MODE -- */
    pcm->mode = PCM_MODE_SIMPLE;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatMenu_Split_True_Damage_Dft_Digit1.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- MATH MODE -- */
    pcm->mode = PCM_MODE_MATH;
    combat_forecast = Compute_Combat_Forecast(&Silou,  &Hamilcar, &silou_pos.tilemap_pos,
                                              &hamilcar_pos.tilemap_pos);
    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatMenu_Math_True_Damage_Dft_Digit1.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Weapon icons -- */

    /* -- Unequip left -- */
    nourstest_true(Silou.equipped[UNIT_HAND_STRONG]);
    Unit_Unequip(&Silou, UNIT_HAND_STRONG);
    nourstest_true(!Silou.equipped[UNIT_HAND_STRONG]);
    nourstest_true(Hamilcar.equipped[UNIT_HAND_STRONG]);
    Unit_Unequip(&Hamilcar, UNIT_HAND_STRONG);
    nourstest_true(!Hamilcar.equipped[UNIT_HAND_STRONG]);

    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat",
                                      "PreCombatMenu_Unequip_Left_Hand.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Unequip Right -- */
    Unit_Equip_inHand(&Silou, UNIT_HAND_STRONG);
    Unit_Unequip(&Silou, UNIT_HAND_WEAK);
    Unit_Equip_inHand(&Hamilcar, UNIT_HAND_STRONG);
    Unit_Unequip(&Hamilcar, UNIT_HAND_WEAK);

    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_Unequip_Right_Hand.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Two handing -- */
    nourstest_true(!Silou.isTwoHanding);
    nourstest_true(!Hamilcar.isTwoHanding);
    Unit_Item_Drop(&Hamilcar, UNIT_HAND_WEAK);
    Unit_Item_Drop(&Silou, UNIT_HAND_WEAK);
    Silou._equipment[0].id    = ITEM_ID_BROADSWORD;
    Silou._equipment[1].id    = ITEM_ID_BROADSWORD;
    Hamilcar._equipment[0].id = ITEM_ID_BROADSWORD;
    Hamilcar._equipment[1].id = ITEM_ID_BROADSWORD;
    Unit_Equip_TwoHanding(&Hamilcar);
    Unit_Equip_TwoHanding(&Silou);
    nourstest_true(Silou.isTwoHanding);
    nourstest_true(Hamilcar.isTwoHanding);

    PreCombatMenu_Update(pcm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_pre_combat", "PreCombatMenu_Two_Handing.png"),
                            renderer, pcm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- FREE --- */
    Unit_Free(&Silou);
    Unit_Free(&Hamilcar);
    PixelFont_Free(pcm->pixelnours, true);
    PixelFont_Free(pcm->pixelnours_big, true);

    Game_Weapons_Free(weapons_dtab);
    SDL_FreeSurface(surface);
    PreCombatMenu_Free(pcm);

    if (n9patch.texture != NULL)
        SDL_DestroyTexture(n9patch.texture);

    SDL_DestroyRenderer(renderer);
    Unit_Free(&Silou);
}