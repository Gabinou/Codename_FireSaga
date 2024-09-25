#include "nourstest.h"
#include "platform.h"
#include "menu/loadout_select.h"
#include "unit/unit.h"
#include "RNG.h"

void test_menu_loadout_select(void) {
    // Test with arbirary values

    /* -- Preliminaries -- */
    sota_mkdir("menu_loadout_select");

    /* -- Weapon dtab -- */
    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);

    /* -- Create n9patch -- */
    struct n9Patch n9patch = n9Patch_default;
    SDL_Texture *render_target = NULL;

    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    char *path = PATH_JOIN("..", "assets", "GUI", "n9Patch", "menu8px.png");
    n9patch.texture = Filesystem_Texture_Load(renderer, path, SDL_PIXELFORMAT_INDEX8);

    /* -- Create LoadoutSelectMenu -- */
    struct LoadoutSelectMenu *wsm = LoadoutSelectMenu_Alloc();
    wsm->selected[UNIT_HAND_LEFT]       = 0;
    wsm->selected[UNIT_HAND_RIGHT]      = 1;
    /* - loading fonts - */
    wsm->pixelnours     = PixelFont_Alloc();
    wsm->pixelnours_big = PixelFont_Alloc();
    wsm->pixelnours_big->y_offset = pixelfont_big_y_offset;
    PixelFont_Load(wsm->pixelnours,     renderer, PATH_JOIN("..", "assets", "fonts", "pixelnours.png"));
    PixelFont_Load(wsm->pixelnours_big, renderer, PATH_JOIN("..", "assets", "fonts",
                                                            "pixelnours_Big.png"));
    SDL_assert(wsm->pixelnours);
    SDL_assert(wsm->pixelnours_big);

    WeaponSelectMenu_Load_n9Patch(wsm, renderer, &n9patch);

    /* -- Create Unit -- */
    struct Unit Silou = Unit_default;
    Unit_Init(&Silou);
    /* - title - */
    Silou.weapons_dtab = weapons_dtab;
    SDL_assert(Silou.num_equipment == 0);
    jsonio_readJSON(s8_literal(PATH_JOIN("units", "Silou_test.json")), &Silou);
    SDL_assert(Silou.num_equipment == 4);

    /* - Unit equip - */
    struct Inventory_item in_wpn = Inventory_item_default;
    // in_wpn.id   = ITEM_ID_FLEURET;
    // in_wpn.used = 0;
    // Weapon_Load(weapons_dtab, in_wpn.id);

    Silou.handedness    = UNIT_HAND_LEFTIE;
    i32 stronghand      = Unit_Hand_Strong(&Silou);
    i32 weakhand        = Unit_Hand_Weak(&Silou);
    Silou.equippable |= ITEM_TYPE_ANGELIC;
    SDL_assert(Silou.num_equipment == 4);
    Unit_Equip(&Silou, stronghand, 0);
    SDL_assert(Unit_isEquipped(&Silou, stronghand));
    nourstest_true(Unit_canAttack(&Silou));

    wsm->unit = &Silou;
    canEquip can_equip  = canEquip_default;
    canEquip_Loadout(&can_equip, UNIT_HAND_LEFT,  Unit_Eq_Equipped(wsm->unit, UNIT_HAND_LEFT));
    canEquip_Loadout(&can_equip, UNIT_HAND_RIGHT, Unit_Eq_Equipped(wsm->unit, UNIT_HAND_RIGHT));

    can_equip.archetype = ITEM_ARCHETYPE_WEAPON;
    Unit_canEquip_Equipment(wsm->unit, can_equip);
    SDL_assert(Silou.num_canEquip > 0);
    SDL_assert(Silou.eq_canEquip[0] == 0);

    _LoadoutSelectMenu_Load(wsm, &Silou, renderer, &n9patch);
    wsm->selected[stronghand] = Silou.eq_canEquip[0];
    struct Menu mc;
    mc.elem = 0;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    /* -- Long weapon names -- */
    Silou._equipment[0].used = 1;
    Silou._equipment[1].used = 0;
    Silou._equipment[2].used = 0;
    Silou._equipment[0].id = ITEM_ID_RETRACTABLE_WRISTBLADE;
    Silou._equipment[1].id = ITEM_ID_REPEATABLE_CROSSBOW;
    Silou._equipment[2].id = ITEM_ID_HONJOU_MASAMUNE;
    Silou._equipment[3].id = ITEM_ID_SILVERLIGHT_SPEAR;
    Weapon_Load(weapons_dtab, Silou._equipment[0].id);
    Weapon_Load(weapons_dtab, Silou._equipment[1].id);
    Weapon_Load(weapons_dtab, Silou._equipment[2].id);
    Weapon_Load(weapons_dtab, Silou._equipment[3].id);
    wsm->unit->eq_canEquip[0] = 0;
    wsm->unit->eq_canEquip[1] = 1;
    wsm->unit->eq_canEquip[2] = 2;
    wsm->unit->eq_canEquip[2] = 2;
    wsm->unit->eq_canEquip[3] = 3;
    wsm->unit->num_canEquip   = 4;

    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_Long.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Short weapon names -- */
    Silou.handedness = UNIT_HAND_LEFTIE;
    Silou._equipment[0].used    = 1;
    Silou._equipment[1].used    = 10;
    Silou._equipment[2].used    = 20;
    Silou._equipment[3].used    = 21;
    Silou._equipment[4].used    = 12;
    Silou._equipment[0].id      = ITEM_ID_FLEURET;
    Silou._equipment[1].id      = ITEM_ID_FLEURET;
    Silou._equipment[2].id      = ITEM_ID_FLEURET;
    Silou._equipment[3].id      = ITEM_ID_FLEURET;
    Silou._equipment[4].id      = ITEM_ID_FLEURET;
    Weapon_Load(weapons_dtab, Silou._equipment[0].id);
    Weapon_Load(weapons_dtab, Silou._equipment[1].id);
    Weapon_Load(weapons_dtab, Silou._equipment[1].id);
    Weapon_Load(weapons_dtab, Silou._equipment[2].id);
    Weapon_Load(weapons_dtab, Silou._equipment[3].id);
    Weapon_Load(weapons_dtab, Silou._equipment[4].id);
    wsm->unit->eq_canEquip[0]     = 0;
    wsm->unit->eq_canEquip[1]     = 1;
    wsm->unit->eq_canEquip[2]     = 2;
    wsm->unit->eq_canEquip[3]     = 3;
    wsm->unit->eq_canEquip[4]     = 4;
    wsm->unit->num_canEquip       = 5;

    stronghand  = Unit_Hand_Strong(&Silou);
    weakhand    = Unit_Hand_Weak(&Silou);

    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_Short.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- HIGHLIGHT -- */
    /* Testing writing stuff to grey arbitrarily */

    /* Switch black to grey */
    int grey = 2;
    PixelFont_Swap_Palette(wsm->pixelnours, renderer, -1, grey);
    PixelFont_Swap_Palette(wsm->pixelnours_big, renderer, -1, grey);

    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_Grey1.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* Switch grey back to black */
    PixelFont_Swap_Palette(wsm->pixelnours, renderer, SOTA_WHITE, SOTA_BLACK);
    PixelFont_Swap_Palette(wsm->pixelnours_big, renderer, SOTA_WHITE, SOTA_BLACK);

    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_Grey2.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    LoadoutSelectMenu_Deselect(wsm);
    LoadoutSelectMenu_Deselect(wsm);

    wsm->selected[stronghand]   = 3;
    mc.elem                     = 3;
    wsm->unit->num_canEquip     = 4;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_LSelected_Usable4.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->selected[stronghand]   = 2;
    mc.elem                     = 2;
    wsm->unit->num_canEquip     = 3;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_LSelected_Usable3.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->selected[stronghand]   = 1;
    mc.elem                     = 1;
    wsm->unit->num_canEquip     = 2;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_LSelected_Usable2.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->selected[stronghand]   = 0;
    mc.elem                     = 0;
    wsm->unit->num_canEquip     = 1;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_LSelected_Usable1.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    wsm->selected[stronghand]   = -1;
    mc.elem                     =  3;
    wsm->unit->num_canEquip     =  4;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_LNotSelected_Usable4.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->selected[stronghand]   = -1;
    mc.elem                     =  2;
    wsm->unit->num_canEquip     =  3;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_LNotSelected_Usable3.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    wsm->selected[stronghand]   = -1;
    mc.elem                     =  1;
    wsm->unit->num_canEquip     =  2;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_LNotSelected_Usable2.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->selected[stronghand]   = -1;
    mc.elem                     =  0;
    wsm->unit->num_canEquip     =  1;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_LNotSelected_Usable1.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Silou.handedness = UNIT_HAND_RIGHTIE;
    stronghand  = Unit_Hand_Strong(&Silou);
    weakhand    = Unit_Hand_Weak(&Silou);

    wsm->selected[stronghand]   = 3;
    mc.elem                     = 3;
    wsm->unit->num_canEquip     = 4;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_RSelected_Usable4.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->selected[stronghand]   = 2;
    mc.elem                     = 2;
    wsm->unit->num_canEquip     = 3;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_RSelected_Usable3.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->selected[stronghand]   = 1;
    mc.elem                     = 1;
    wsm->unit->num_canEquip     = 2;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_RSelected_Usable2.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->selected[stronghand]   = 0;
    mc.elem                     = 0;
    wsm->unit->num_canEquip     = 1;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_RSelected_Usable1.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    wsm->selected[stronghand]   = -1;
    mc.elem                     =  3;
    wsm->unit->num_canEquip     =  4;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_RNotSelected_Usable4.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->selected[stronghand]   = -1;
    mc.elem                     =  2;
    wsm->unit->num_canEquip     =  3;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_RNotSelected_Usable3.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    wsm->selected[stronghand]   = -1;
    mc.elem                     =  1;
    wsm->unit->num_canEquip     =  2;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_RNotSelected_Usable2.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->selected[stronghand]   = -1;
    mc.elem                     =  0;
    wsm->unit->num_canEquip     =  1;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_RNotSelected_Usable1.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- Testing header drawing --- */
    wsm->unit->num_canEquip   = 4;
    LoadoutSelectMenu_Header_Set(wsm, "Drop 1 item for two-handing");
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_Header.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- FREE --- */
    PixelFont_Free(wsm->pixelnours,     true);
    PixelFont_Free(wsm->pixelnours_big, true);

    Game_Weapons_Free(&weapons_dtab);
    SDL_FreeSurface(surface);
    LoadoutSelectMenu_Free(wsm);

    if (n9patch.texture != NULL)
        SDL_DestroyTexture(n9patch.texture);
    Unit_Free(&Silou);

    SDL_DestroyRenderer(renderer);
    Game_Weapons_Free(&weapons_dtab);
}

void test_menu_loadout_select_two_hands(void) {
    // Test that the menu correctly shows
    //  - Twohand only weapons at first
    //  - If first selected weapon is only twohand,
    //      only show it possible in other hand.

    /* -- Weapon dtab -- */
    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);

    /* -- Create Unit -- */
    struct Unit Silou = Unit_default;
    Unit_Init(&Silou);
    Silou.weapons_dtab = weapons_dtab;
    SDL_assert(Silou.num_equipment == 0);
    Silou._equipment[0].id      = ITEM_ID_FLEURET;
    Silou._equipment[1].id      = ITEM_ID_RAPIERE;
    Silou._equipment[2].id      = ITEM_ID_IRON_SWORD;
    Silou._equipment[3].id      = ITEM_ID_FLEURET;
    Silou.handedness            = UNIT_HAND_LEFTIE;
    Silou.equippable            = ITEM_TYPE_SWORD;

    /* -- Setup two hamded weapon -- */
    Weapon_Load(weapons_dtab, Silou._equipment[0].id);
    struct Weapon *weapon = DTAB_GET(weapons_dtab, Silou._equipment[0].id);
    weapon->handedness  = WEAPON_HAND_TWO;

    /* -- Create LoadoutSelectMenu -- */
    struct LoadoutSelectMenu *wsm = LoadoutSelectMenu_Alloc();
    wsm->unit = &Silou;

    Unit_Free(&Silou);
    LoadoutSelectMenu_Free(wsm);
    Game_Weapons_Free(&weapons_dtab);
}

void unit_test_menu_loadout_select(void) {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);
    test_menu_loadout_select_two_hands();
}

void render_test_menu_loadout_select(void) {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);
    test_menu_loadout_select();
}
