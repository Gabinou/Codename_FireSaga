
#include "AI.h"
#include "log.h"
#include "fsm.h"
#include "RNG.h"
#include "tile.h"
#include "text.h"
#include "scene.h"
#include "hover.h"
#include "music.h"
#include "names.h"
#include "input.h"
#include "macros.h"
#include "macros.h"
#include "macros.h"
#include "slider.h"
#include "sprite.h"
#include "combat.h"
#include "events.h"
#include "convoy.h"
#include "structs.h"
#include "platform.h"
#include "position.h"
#include "cutscene.h"
#include "platform.h"
#include "platform.h"
#include "nourstest.h"
#include "utilities.h"
#include "filesystem.h"
#include "pixelfonts.h"
#include "noursclock.h"
#include "pathfinding.h"

#include "bars/map_hp.h"

#include "controller/fsm.h"
#include "controller/mouse.h"
#include "controller/gamepad.h"
#include "controller/touchpad.h"
#include "controller/keyboard.h"

#include "map/map.h"
#include "map/find.h"
#include "map/tiles.h"
#include "map/ontile.h"
#include "map/render.h"
#include "map/animation.h"

#include "menu/menu.h"
#include "menu/stats.h"
#include "menu/deployment.h"
#include "menu/loadout_select.h"

#include "popup/popup.h"

#include "systems/slide.h"
#include "systems/render.h"
#include "systems/time_system.h"

#include "unit/boss.h"
#include "unit/anim.h"
#include "unit/unit.h"
#include "unit/party.h"

#define TEST_ROW_LEN 10
#define TEST_COL_LEN 10

#define TEST_SET_EQUIPMENT(world, ID, eq) \
    seteqentity  = IES_E_CREATE_wC(world, Inventory_item_ID);\
    seteqinvitem = IES_GET_C(world, seteqentity, Inventory_item);\
    seteqinvitem->id = ID;\
    silou_eq[eq] = seteqentity;

void test_menu_loadout_select_render(void) {
    // Test with arbirary values

    /* -- Preliminaries -- */
    sota_mkdir("menu_loadout_select");

    /* -- Tnecs world -- */
    tnecs_W *world = NULL;
    tnecs_genesis(&world);
    gl_world = world;
    tnecs_E    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;

#include "register/components.h"

    tnecs_E Silou_ent  = IES_E_CREATE_wC(world, Unit_ID, Position_ID);
    struct Unit *Silou      = IES_GET_C(world, Silou_ent, Unit);

    /* -- Map -- */
    NewMap new_map      = NewMap_default;
    new_map.tilesize[0] = 16;
    new_map.tilesize[1] = 16;
    new_map.row_len     = TEST_ROW_LEN;
    new_map.col_len     = TEST_COL_LEN;

    Map *map = Map_New(new_map);

    /* -- Weapon dtab -- */
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab = DTAB_INIT(gl_items_dtab, struct Item);

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
    wsm->_unit   = Silou_ent;

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
    SDL_assert(Silou->equipment.num == 0);
    jsonio_readJSON(s8_literal(PATH_JOIN("units", "Silou_test.json")), Silou);
    SDL_assert(Silou->equipment.num == 4);

    /* - Unit equip - */
    // struct Inventory_item in_wpn = Inventory_item_default;
    // in_wpn.id   = ITEM_ID_FLEURET;
    // in_wpn.used = 0;
    // Item_Load(in_wpn.id);

    Unit_Handedness_set(Silou, UNIT_HAND_LEFTIE);
    i32 stronghand      = Unit_Hand_Strong(Silou);
    i32 weakhand        = Unit_Hand_Weak(Silou);
    Unit_Equippable_set(Silou, Unit_Equippable(Silou) | ITEM_TYPE_ANGELIC);
    SDL_assert(Silou->equipment.num == 4);
    Unit_Equip(Silou, stronghand,   ITEM1);
    Unit_Equip(Silou, weakhand,     ITEM4);
    SDL_assert(Unit_isEquipped(Silou, stronghand));
    nourstest_true(Unit_canAttack(Silou));

    canEquip can_equip  = canEquip_default;
    SDL_assert(Unit_Eq_Equipped(Silou, UNIT_HAND_LEFT)  >= ITEM1);
    SDL_assert(Unit_Eq_Equipped(Silou, UNIT_HAND_LEFT)  <= ITEM6);
    SDL_assert(Unit_Eq_Equipped(Silou, UNIT_HAND_RIGHT) >= ITEM1);
    SDL_assert(Unit_Eq_Equipped(Silou, UNIT_HAND_RIGHT) <= ITEM6);

    wsm->equippable.num = 0;
    canEquip_Loadout(&can_equip, UNIT_HAND_LEFT,
                     Unit_Eq_Equipped(Silou, UNIT_HAND_LEFT));
    canEquip_Loadout(&can_equip, UNIT_HAND_RIGHT,
                     Unit_Eq_Equipped(Silou, UNIT_HAND_RIGHT));

    can_equip.archetype = ITEM_ARCHETYPE_WEAPON;
    wsm->equippable = Unit_canEquip_Equipment(Silou, can_equip);
    SDL_assert(Silou != NULL);
    SDL_assert(wsm->equippable.num == 1);
    i32 *silou_can_equip = wsm->equippable.arr;

    SDL_assert(silou_can_equip[0] == ITEM1);
    SDL_assert(Unit_Id_Equipped(Silou, ITEM1) > ITEM_NULL);

    WeaponSelectMenu_Load(wsm, map, renderer, &n9patch);
    LoadoutSelectMenu_Unit(wsm, Silou_ent);
    // _LoadoutSelectMenu_Load(wsm, Silou_ent, renderer, &n9patch);
    Loadout_Set(&wsm->selected, stronghand, silou_can_equip[0]);

    struct Menu mc;
    mc.elem = 0;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    /* -- Long weapon names -- */
    tnecs_E *silou_eq = Unit_Equipment(Silou);
    TEST_SET_EQUIPMENT(world, ITEM_ID_RETRACTABLE_WRISTBLADE, 0);
    seteqinvitem->used = 1;
    Item_Load(seteqinvitem->id);
    TEST_SET_EQUIPMENT(world, ITEM_ID_REPEATABLE_CROSSBOW, 1);
    Item_Load(seteqinvitem->id);
    TEST_SET_EQUIPMENT(world, ITEM_ID_HONJOU_MASAMUNE, 2);
    Item_Load(seteqinvitem->id);
    TEST_SET_EQUIPMENT(world, ITEM_ID_SILVERLIGHT_SPEAR, 3);
    Item_Load(seteqinvitem->id);

    silou_can_equip[0] = ITEM1;
    silou_can_equip[1] = ITEM2;
    silou_can_equip[2] = ITEM3;
    silou_can_equip[2] = ITEM4;
    silou_can_equip[3] = ITEM5;
    wsm->equippable.num   = 4;

    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_Long.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Short weapon names -- */
    Unit_Handedness_set(Silou, UNIT_HAND_LEFTIE);
    silou_eq = Unit_Equipment(Silou);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, 0);
    seteqinvitem->used = 1;
    Item_Load(seteqinvitem->id);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, 1);
    seteqinvitem->used = 10;
    Item_Load(seteqinvitem->id);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, 2);
    seteqinvitem->used = 20;
    Item_Load(seteqinvitem->id);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, 3);
    seteqinvitem->used = 21;
    Item_Load(seteqinvitem->id);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, 4);
    seteqinvitem->used = 12;
    Item_Load(seteqinvitem->id);

    silou_can_equip[0]     = ITEM1;
    silou_can_equip[1]     = ITEM2;
    silou_can_equip[2]     = ITEM3;
    silou_can_equip[3]     = ITEM4;
    silou_can_equip[4]     = ITEM5;
    wsm->equippable.num       = 5;

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
    wsm->equippable.num = 4;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_LSelected_Usable4.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_Set(&wsm->selected, stronghand, ITEM3);
    mc.elem             = 0;
    wsm->equippable.num = 3;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_LSelected_Usable3.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_Set(&wsm->selected, stronghand, ITEM2);
    mc.elem             = 0;
    wsm->equippable.num = 2;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_LSelected_Usable2.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_Set(&wsm->selected, stronghand, ITEM1);
    mc.elem             = 0;
    wsm->equippable.num = 1;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_LSelected_Usable1.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem             =  3;
    wsm->equippable.num =  4;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_LNotSelected_Usable4.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem             =  2;
    wsm->equippable.num =  3;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_LNotSelected_Usable3.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem             =  1;
    wsm->equippable.num =  2;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_LNotSelected_Usable2.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem                 =  0;
    wsm->equippable.num     =  1;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_LNotSelected_Usable1.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Unit_Handedness_set(Silou, UNIT_HAND_RIGHTIE);
    stronghand  = Unit_Hand_Strong(Silou);
    weakhand    = Unit_Hand_Weak(Silou);

    Loadout_Set(&wsm->selected, stronghand, ITEM4);
    mc.elem                 = 0;
    wsm->equippable.num     = 4;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_RSelected_Usable4.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_Set(&wsm->selected, stronghand, ITEM3);
    mc.elem                 = 0;
    wsm->equippable.num     = 3;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_RSelected_Usable3.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_Set(&wsm->selected, stronghand, ITEM2);
    mc.elem                 = 0;
    wsm->equippable.num     = 2;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_RSelected_Usable2.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_Set(&wsm->selected, stronghand, ITEM1);
    mc.elem                 = 0;
    wsm->equippable.num     = 1;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_RSelected_Usable1.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem                 =  3;
    wsm->equippable.num     =  4;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_RNotSelected_Usable4.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem                     =  2;
    wsm->equippable.num         =  3;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_RNotSelected_Usable3.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem                     =  1;
    wsm->equippable.num     =  2;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_RNotSelected_Usable2.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem                     =  0;
    wsm->equippable.num     =  1;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_RNotSelected_Usable1.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- Testing header drawing --- */
    wsm->equippable.num   = 4;
    LoadoutSelectMenu_Header_Set(wsm, "Drop 1 item for two-handing");
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_Header.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- SDL_free --- */
    PixelFont_Free(wsm->pixelnours,     true);
    PixelFont_Free(wsm->pixelnours_big, true);

    SDL_FreeSurface(surface);
    LoadoutSelectMenu_Free(wsm);

    if (n9patch.texture != NULL)
        SDL_DestroyTexture(n9patch.texture);
    Unit_Free(Silou);

    SDL_DestroyRenderer(renderer);
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
    tnecs_finale(&world);
    gl_world = NULL;
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
    tnecs_W *world = NULL;
    tnecs_genesis(&world);
    tnecs_E    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;
    gl_world = world;

#include "register/components.h"

    tnecs_E Silou_ent  = IES_E_CREATE_wC(world, Unit_ID, Position_ID);
    struct Unit *Silou      = IES_GET_C(world, Silou_ent, Unit);

    /* -- Weapon dtab -- */
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab = DTAB_INIT(gl_items_dtab, struct Weapon);

    /* -- Create Unit -- */
    Unit_Init(Silou);
    SDL_assert(Silou->equipment.num == 0);
    tnecs_E *silou_eq = Unit_Equipment(Silou);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, 0);
    Item_Load(seteqinvitem->id);
    TEST_SET_EQUIPMENT(world, ITEM_ID_RAPIERE, 1);
    Item_Load(seteqinvitem->id);
    TEST_SET_EQUIPMENT(world, ITEM_ID_IRON_SWORD, 2);
    Item_Load(seteqinvitem->id);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, 3);
    Item_Load(seteqinvitem->id);
    TEST_SET_EQUIPMENT(world, ITEM_ID_WOODEN_SHIELD, 4);
    Item_Load(seteqinvitem->id);
    TEST_SET_EQUIPMENT(world, ITEM_ID_SALVE, 5);
    Item_Load(seteqinvitem->id);
    Unit_Handedness_set(Silou, UNIT_HAND_LEFTIE);
    Unit_Equippable_set(Silou, ITEM_TYPE_SWORD);

    struct Weapon *weapons[SOTA_EQUIPMENT_SIZE] = {0};
    weapons[0] = DTAB_GET(gl_weapons_dtab, Unit_InvItem(Silou, ITEM1)->id);
    weapons[1] = DTAB_GET(gl_weapons_dtab, Unit_InvItem(Silou, ITEM2)->id);
    weapons[2] = DTAB_GET(gl_weapons_dtab, Unit_InvItem(Silou, ITEM3)->id);
    weapons[3] = DTAB_GET(gl_weapons_dtab, Unit_InvItem(Silou, ITEM4)->id);
    weapons[4] = DTAB_GET(gl_weapons_dtab, Unit_InvItem(Silou, ITEM5)->id);

    /* -- Create LoadoutSelectMenu -- */
    struct LoadoutSelectMenu *wsm = LoadoutSelectMenu_Alloc();
    wsm->_unit   = Silou_ent;

    /* --- TESTS --- */

    /* -- Equipping a two-hand only weapon -- */
    Weapon_Handedness_Set(weapons[0], WEAPON_HAND_TWO);
    Weapon_Handedness_Set(weapons[1], WEAPON_HAND_ONE);
    Weapon_Handedness_Set(weapons[2], WEAPON_HAND_ANY);
    Weapon_Handedness_Set(weapons[3], WEAPON_HAND_LEFT);
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

    /* --- SDL_free --- */
    Unit_Free(Silou);
    LoadoutSelectMenu_Free(wsm);
    Game_Items_Free(&gl_items_dtab);
    Game_Weapons_Free(&gl_weapons_dtab);
    tnecs_finale(&world);
    gl_world = NULL;
}


void test_menu_loadout_select(void) {
    test_menu_loadout_select_two_hands();
}

#undef TEST_SET_EQUIPMENT
