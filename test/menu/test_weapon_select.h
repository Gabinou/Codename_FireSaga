#include "nourstest.h"
#include "platform.h"
#include "menu/loadout_select.h"
#include "unit/unit.h"
#include "RNG.h"

#define TEST_ROW_LEN 10
#define TEST_COL_LEN 10

void test_menu_loadout_select(void) {
    // Test with arbirary values

    /* -- Preliminaries -- */
    sota_mkdir("menu_loadout_select");

    /* -- Tnecs world -- */
    tnecs_world *world = tnecs_world_genesis();
    TNECS_REGISTER_COMPONENT(world, Unit);
    TNECS_REGISTER_COMPONENT(world, Position);
    tnecs_entity Silou_ent  = TNECS_ENTITY_CREATE_wCOMPONENTS(world, Unit, Position);
    struct Unit *Silou      = TNECS_GET_COMPONENT(world, Silou_ent, Unit);

    /* -- Map -- */
    NewMap new_map      = NewMap_default;
    new_map.tilesize[0] = 16;
    new_map.tilesize[1] = 16;
    new_map.row_len     = TEST_ROW_LEN;
    new_map.col_len     = TEST_COL_LEN;
    new_map.world       = world;

    Map *map = Map_New(new_map);

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
    wsm->world  = world;
    wsm->unit   = Silou_ent;

    Loadout_Set(&wsm->selected, UNIT_HAND_LEFT,   ITEM1);
    Loadout_Set(&wsm->selected, UNIT_HAND_RIGHT,  ITEM2);

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
    *Silou = Unit_default;
    Unit_Init(Silou);
    /* - title - */
    Silou->weapons_dtab = weapons_dtab;
    SDL_assert(Silou->num_equipment == 0);
    jsonio_readJSON(s8_literal(PATH_JOIN("units", "Silou_test.json")), Silou);
    SDL_assert(Silou->num_equipment == 4);

    /* - Unit equip - */
    struct Inventory_item in_wpn = Inventory_item_default;
    // in_wpn.id   = ITEM_ID_FLEURET;
    // in_wpn.used = 0;
    // Weapon_Load(weapons_dtab, in_wpn.id);

    Silou->handedness   = UNIT_HAND_LEFTIE;
    i32 stronghand      = Unit_Hand_Strong(Silou);
    i32 weakhand        = Unit_Hand_Weak(Silou);
    Silou->equippable |= ITEM_TYPE_ANGELIC;
    SDL_assert(Silou->num_equipment == 4);
    Unit_Equip(Silou, stronghand,   ITEM1);
    Unit_Equip(Silou, weakhand,     ITEM4);
    SDL_assert(Unit_isEquipped(Silou, stronghand));
    nourstest_true(Unit_canAttack(Silou));

    canEquip can_equip  = canEquip_default;
    SDL_assert(Unit_Eq_Equipped(Silou, UNIT_HAND_LEFT)  >= ITEM1);
    SDL_assert(Unit_Eq_Equipped(Silou, UNIT_HAND_LEFT)  <= ITEM6);
    SDL_assert(Unit_Eq_Equipped(Silou, UNIT_HAND_RIGHT) >= ITEM1);
    SDL_assert(Unit_Eq_Equipped(Silou, UNIT_HAND_RIGHT) <= ITEM6);

    Silou->num_canEquip = 0;
    canEquip_Loadout(&can_equip, UNIT_HAND_LEFT,  Unit_Eq_Equipped(Silou, UNIT_HAND_LEFT));
    canEquip_Loadout(&can_equip, UNIT_HAND_RIGHT, Unit_Eq_Equipped(Silou, UNIT_HAND_RIGHT));

    can_equip.archetype = ITEM_ARCHETYPE_WEAPON;
    Unit_canEquip_Equipment(Silou, can_equip);
    SDL_assert(Silou->num_canEquip == 1);
    SDL_assert(Silou->eq_canEquip[0] == ITEM1);
    SDL_assert(Unit_Id_Equipped(Silou, ITEM1) > ITEM_NULL);

    WeaponSelectMenu_Load(wsm, map, world, renderer, &n9patch);
    LoadoutSelectMenu_Unit(wsm, Silou_ent);
    // _LoadoutSelectMenu_Load(wsm, Silou_ent, renderer, &n9patch);
    Loadout_Set(&wsm->selected, stronghand, Silou->eq_canEquip[0]);

    struct Menu mc;
    mc.elem = 0;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    /* -- Long weapon names -- */
    Silou->_equipment[0].used = 1;
    Silou->_equipment[1].used = 0;
    Silou->_equipment[2].used = 0;
    Silou->_equipment[0].id = ITEM_ID_RETRACTABLE_WRISTBLADE;
    Silou->_equipment[1].id = ITEM_ID_REPEATABLE_CROSSBOW;
    Silou->_equipment[2].id = ITEM_ID_HONJOU_MASAMUNE;
    Silou->_equipment[3].id = ITEM_ID_SILVERLIGHT_SPEAR;
    Weapon_Load(weapons_dtab, Silou->_equipment[0].id);
    Weapon_Load(weapons_dtab, Silou->_equipment[1].id);
    Weapon_Load(weapons_dtab, Silou->_equipment[2].id);
    Weapon_Load(weapons_dtab, Silou->_equipment[3].id);
    Silou->eq_canEquip[0] = ITEM1;
    Silou->eq_canEquip[1] = ITEM2;
    Silou->eq_canEquip[2] = ITEM3;
    Silou->eq_canEquip[2] = ITEM4;
    Silou->eq_canEquip[3] = ITEM5;
    Silou->num_canEquip   = 4;

    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_Long.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Short weapon names -- */
    Silou->handedness = UNIT_HAND_LEFTIE;
    Silou->_equipment[0].used    = 1;
    Silou->_equipment[1].used    = 10;
    Silou->_equipment[2].used    = 20;
    Silou->_equipment[3].used    = 21;
    Silou->_equipment[4].used    = 12;
    Silou->_equipment[0].id      = ITEM_ID_FLEURET;
    Silou->_equipment[1].id      = ITEM_ID_FLEURET;
    Silou->_equipment[2].id      = ITEM_ID_FLEURET;
    Silou->_equipment[3].id      = ITEM_ID_FLEURET;
    Silou->_equipment[4].id      = ITEM_ID_FLEURET;
    Weapon_Load(weapons_dtab, Silou->_equipment[0].id);
    Weapon_Load(weapons_dtab, Silou->_equipment[1].id);
    Weapon_Load(weapons_dtab, Silou->_equipment[1].id);
    Weapon_Load(weapons_dtab, Silou->_equipment[2].id);
    Weapon_Load(weapons_dtab, Silou->_equipment[3].id);
    Weapon_Load(weapons_dtab, Silou->_equipment[4].id);
    Silou->eq_canEquip[0]     = ITEM1;
    Silou->eq_canEquip[1]     = ITEM2;
    Silou->eq_canEquip[2]     = ITEM3;
    Silou->eq_canEquip[3]     = ITEM4;
    Silou->eq_canEquip[4]     = ITEM5;
    Silou->num_canEquip       = 5;

    stronghand  = Unit_Hand_Strong(Silou);
    weakhand    = Unit_Hand_Weak(Silou);

    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_Short.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- HIGHLIGHT -- */
    /* Testing writing stuff to grey arbitrarily */

    /* Switch black to grey */
    int grey = 2;
    PixelFont_Swap_Palette(wsm->pixelnours,     renderer, -1, grey);
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

    Loadout_Set(&wsm->selected, stronghand, ITEM4);
    mc.elem             = 0;
    Silou->num_canEquip = 4;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_LSelected_Usable4.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_Set(&wsm->selected, stronghand, ITEM3);
    mc.elem             = 0;
    Silou->num_canEquip = 3;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_LSelected_Usable3.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_Set(&wsm->selected, stronghand, ITEM2);
    mc.elem             = 0;
    Silou->num_canEquip = 2;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_LSelected_Usable2.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_Set(&wsm->selected, stronghand, ITEM1);
    mc.elem             = 0;
    Silou->num_canEquip = 1;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_LSelected_Usable1.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem             =  3;
    Silou->num_canEquip =  4;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_LNotSelected_Usable4.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem             =  2;
    Silou->num_canEquip =  3;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_LNotSelected_Usable3.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem             =  1;
    Silou->num_canEquip =  2;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_LNotSelected_Usable2.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem                 =  0;
    Silou->num_canEquip     =  1;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_LNotSelected_Usable1.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Silou->handedness = UNIT_HAND_RIGHTIE;
    stronghand  = Unit_Hand_Strong(Silou);
    weakhand    = Unit_Hand_Weak(Silou);

    Loadout_Set(&wsm->selected, stronghand, ITEM4);
    mc.elem                 = 0;
    Silou->num_canEquip     = 4;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_RSelected_Usable4.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_Set(&wsm->selected, stronghand, ITEM3);
    mc.elem                 = 0;
    Silou->num_canEquip     = 3;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_RSelected_Usable3.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_Set(&wsm->selected, stronghand, ITEM2);
    mc.elem                 = 0;
    Silou->num_canEquip     = 2;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_RSelected_Usable2.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_Set(&wsm->selected, stronghand, ITEM1);
    mc.elem                 = 0;
    Silou->num_canEquip     = 1;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_RSelected_Usable1.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem                 =  3;
    Silou->num_canEquip     =  4;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_RNotSelected_Usable4.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem                     =  2;
    Silou->num_canEquip         =  3;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_RNotSelected_Usable3.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem                     =  1;
    Silou->num_canEquip     =  2;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_RNotSelected_Usable2.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem                     =  0;
    Silou->num_canEquip     =  1;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_RNotSelected_Usable1.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- Testing header drawing --- */
    Silou->num_canEquip   = 4;
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
    Unit_Free(Silou);

    SDL_DestroyRenderer(renderer);
    Game_Weapons_Free(&weapons_dtab);
    tnecs_world_destroy(world);
}

#undef TEST_ROW_LEN
#undef TEST_COL_LEN

void test_menu_loadout_select_two_hands(void) {
    // Test that the menu correctly shows
    //  - Twohand only weapons at first
    //  - If first selected weapon is only twohand,
    //      only show it possible in other hand.

    /* --- PREPARATION --- */

    /* -- Tnecs world -- */
    tnecs_world *world = tnecs_world_genesis();
    TNECS_REGISTER_COMPONENT(world, Unit);
    TNECS_REGISTER_COMPONENT(world, Position);
    tnecs_entity Silou_ent  = TNECS_ENTITY_CREATE_wCOMPONENTS(world, Unit, Position);
    struct Unit *Silou      = TNECS_GET_COMPONENT(world, Silou_ent, Unit);

    /* -- Weapon dtab -- */
    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);

    /* -- Create Unit -- */
    Unit_Init(Silou);
    Silou->weapons_dtab = weapons_dtab;
    SDL_assert(Silou->num_equipment == 0);
    Silou->_equipment[0].id      = ITEM_ID_FLEURET;
    Silou->_equipment[1].id      = ITEM_ID_RAPIERE;
    Silou->_equipment[2].id      = ITEM_ID_IRON_SWORD;
    Silou->_equipment[3].id      = ITEM_ID_FLEURET;
    Silou->_equipment[4].id      = ITEM_ID_WOODEN_SHIELD;
    Silou->_equipment[5].id      = ITEM_ID_SALVE;
    Silou->handedness            = UNIT_HAND_LEFTIE;
    Silou->equippable            = ITEM_TYPE_SWORD;

    Weapon_Load(weapons_dtab, Silou->_equipment[0].id);
    Weapon_Load(weapons_dtab, Silou->_equipment[1].id);
    Weapon_Load(weapons_dtab, Silou->_equipment[2].id);
    Weapon_Load(weapons_dtab, Silou->_equipment[3].id);
    struct Weapon *weapons[6] = {0};
    weapons[0] = DTAB_GET(weapons_dtab, Silou->_equipment[0].id);
    weapons[1] = DTAB_GET(weapons_dtab, Silou->_equipment[1].id);
    weapons[2] = DTAB_GET(weapons_dtab, Silou->_equipment[2].id);
    weapons[3] = DTAB_GET(weapons_dtab, Silou->_equipment[3].id);
    weapons[4] = DTAB_GET(weapons_dtab, Silou->_equipment[4].id);

    /* -- Create LoadoutSelectMenu -- */
    struct LoadoutSelectMenu *wsm = LoadoutSelectMenu_Alloc();
    wsm->world  = world;
    wsm->unit   = Silou_ent;

    /* --- TESTS --- */

    /* -- Equipping a two-hand only weapon -- */
    weapons[0]->handedness  = WEAPON_HAND_TWO;
    weapons[1]->handedness  = WEAPON_HAND_ONE;
    weapons[2]->handedness  = WEAPON_HAND_ANY;
    weapons[3]->handedness  = WEAPON_HAND_LEFT;
    /* - Can be selected by stronghand                  - */
    LoadoutSelectMenu_Select_Reset(wsm);

    /* - No other weapon can be selected by weakhand    - */
    // LoadoutSelectMenu_Select(wsm, 0);

    /* -- Equipping a one-hand only weapon -- */
    /* - Can be selected by stronghand - */
    /* - Two-hand only weapons can't be selected  - */
    /* - Option to equip nothing in weakhand  - */

    /* -- Equipping a any hand weapon -- */

    /* -- Equipping a any left-hand only weapon -- */

    /* -- Equipping a any right-hand only weapon -- */

    /* --- FREE --- */
    Unit_Free(Silou);
    LoadoutSelectMenu_Free(wsm);
    Game_Weapons_Free(&weapons_dtab);
}


void unit_test_menu_loadout_select(void) {
    test_menu_loadout_select_two_hands();
}

void render_test_menu_loadout_select(void) {
    test_menu_loadout_select();
}
