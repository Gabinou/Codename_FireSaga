#include "nourstest.h"
#include "platform.h"
#include "menu/loadout_select.h"
#include "unit/unit.h"
#include "RNG.h"

void test_menu_loadout_select() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);
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
    wsm->selected[UNIT_HAND_LEFT]       = false;
    wsm->selected[UNIT_HAND_RIGHT]      = true;
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

    i32 stronghand  = Unit_Hand_Strong(&Silou);
    // i32 weakhand    = 1 - stronghand;
    // Unit_Item_Drop(&Silou,           weakhand);
    // Unit_Item_Takeat(&Silou, in_wpn, weakhand);
    SDL_assert(Silou.num_equipment == 4);
    Unit_Equip(&Silou, stronghand, 0);
    SDL_assert(Unit_isEquipped(&Silou, stronghand));
    nourstest_true(Unit_canAttack(&Silou));

    canEquip can_equip  = canEquip_default;
    can_equip.lh        = Unit_Eq_Equipped(wsm->unit, UNIT_HAND_LEFT);
    can_equip.rh        = Unit_Eq_Equipped(wsm->unit, UNIT_HAND_RIGHT);
    can_equip.archetype = ITEM_ARCHETYPE_WEAPON;
    Unit_canEquip_Equipment(wsm->unit, can_equip);
    SDL_assert(Silou.num_canEquip > 0);
    SDL_assert(Silou.eq_canEquip[0] == 0);

    _LoadoutSelectMenu_Load(wsm, &Silou, renderer, &n9patch);
    wsm->selected[stronghand] = Silou.eq_canEquip[0];
    struct  Menu mc;
    mc.elem = 0;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    /* -- Long weapon names -- */
    Silou.handedness = UNIT_HAND_LEFTIE;
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

    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_Short.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- HIGHLIGHT -- */
    PixelFont_Swap_Palette(wsm->pixelnours, renderer, 1, 2);
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_Grey1.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    PixelFont_Swap_Palette(wsm->pixelnours, renderer, SOTA_WHITE, SOTA_BLACK);

    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_Grey2.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    LoadoutSelectMenu_Deselect(wsm);
    LoadoutSelectMenu_Deselect(wsm);

    wsm->unit->num_canEquip   = 4;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_LTopStrong_Usable4.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->unit->num_canEquip   = 3;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_LTopStrong_Usable3.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->unit->num_canEquip   = 2;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_LTopStrong_Usable2.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->unit->num_canEquip   = 1;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_LTopStrong_Usable1.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    wsm->unit->num_canEquip   = 4;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_LTopStrongNot_Usable4.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->unit->num_canEquip   = 3;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_LTopStrongNot_Usable3.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->unit->num_canEquip   = 2;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_LTopStrongNot_Usable2.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->unit->num_canEquip   = 1;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_LTopStrongNot_Usable1.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Silou.handedness = UNIT_HAND_RIGHTIE;
    wsm->unit->num_canEquip   = 4;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_RTopStrong_Usable4.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->unit->num_canEquip   = 3;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_RTopStrong_Usable3.png"),
                            renderer, wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->unit->num_canEquip   = 2;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_RTopStrong_Usable2.png"),
                            renderer, wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->unit->num_canEquip   = 1;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_RTopStrong_Usable1.png"),
                            renderer, wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    wsm->unit->num_canEquip   = 4;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_RTopStrongNot_Usable4.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->unit->num_canEquip   = 3;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_RTopStrongNot_Usable3.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->unit->num_canEquip   = 2;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_RTopStrongNot_Usable2.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    wsm->unit->num_canEquip   = 1;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_RTopStrongNot_Usable1.png"), renderer,
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
}