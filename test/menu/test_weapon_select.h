
#include "nourstest.h"
#include "platform.h"
#include "menu/loadout_select.h"
#include "unit/unit.h"
#include "RNG.h"
#include "macros.h"
#include "nourstest.h"
#include "platform.h"
#include "map/tiles.h"
#include "map/ontile.h"
#include "map/find.h"
#include "menu/menu.h"
#include "pathfinding.h"
#include "macros.h"
#include "structs.h"
#include "map/map.h"
#include "bars/map_hp.h"
#include "systems/render.h"
#include "filesystem.h"
#include "noursclock.h"
#include "log.h"
#include "macros.h"
#include "RNG.h"
#include "platform.h"
#include "position.h"
#include "popup/popup.h"
#include "menu/menu.h"
#include "unit/boss.h"
#include "unit/anim.h"
#include "menu/loadout_select.h"
#include "menu/deployment.h"
#include "menu/stats.h"
#include "map/animation.h"
#include "unit/party.h"
#include "map/render.h"
#include "scene.h"
#include "cutscene.h"
#include "slider.h"
#include "pixelfonts.h"
#include "sprite.h"
#include "input.h"
#include "tile.h"
#include "hover.h"
#include "combat.h"
#include "music.h"
#include "controller/mouse.h"
#include "controller/touchpad.h"
#include "controller/keyboard.h"
#include "controller/gamepad.h"
#include "controller/fsm.h"
#include "systems/time_system.h"
#include "systems/slide.h"
#include "AI.h"
#include "events.h"
#include "fps_fsm.h"
#include "fsm.h"
#include "names.h"
#include "text.h"
#include "convoy.h"
#include "utilities.h"

#define TEST_ROW_LEN 10
#define TEST_COL_LEN 10

void test_menu_loadout_select(void) {
    // Test with arbirary values

    /* -- Preliminaries -- */
    sota_mkdir("menu_loadout_select");

    /* -- Tnecs world -- */
    tnecs_world *world = NULL;
    tnecs_world_genesis(&world);

#include "register_components.h"

    tnecs_entity Silou_ent  = TNECS_ENTITY_CREATE_wCOMPONENTS(world, Unit, Position);
    struct Unit *Silou      = IES_GET_COMPONENT(world, Silou_ent, Unit);

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
    Silou->equipment.weapons_dtab = weapons_dtab;
    SDL_assert(Silou->equipment.num == 0);
    jsonio_readJSON(s8_literal(PATH_JOIN("units", "Silou_test.json")), Silou);
    SDL_assert(Silou->equipment.num == 4);

    /* - Unit equip - */
    struct Inventory_item in_wpn = Inventory_item_default;
    // in_wpn.id   = ITEM_ID_FLEURET;
    // in_wpn.used = 0;
    // Weapon_Load(weapons_dtab, in_wpn.id);

    Silou->flags.handedness   = UNIT_HAND_LEFTIE;
    i32 stronghand      = Unit_Hand_Strong(Silou);
    i32 weakhand        = Unit_Hand_Weak(Silou);
    Silou->flags.equippable |= ITEM_TYPE_ANGELIC;
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

    Silou->can_equip.num = 0;
    canEquip_Loadout(&can_equip, UNIT_HAND_LEFT,  Unit_Eq_Equipped(Silou, UNIT_HAND_LEFT));
    canEquip_Loadout(&can_equip, UNIT_HAND_RIGHT, Unit_Eq_Equipped(Silou, UNIT_HAND_RIGHT));

    can_equip.archetype = ITEM_ARCHETYPE_WEAPON;
    Unit_canEquip_Equipment(Silou, can_equip);
    SDL_assert(Silou->can_equip.num == 1);
    i32 *silou_can_equip = Unit_canEquip_Arr(Silou);

    SDL_assert(silou_can_equip[0] == ITEM1);
    SDL_assert(Unit_Id_Equipped(Silou, ITEM1) > ITEM_NULL);

    WeaponSelectMenu_Load(wsm, map, world, renderer, &n9patch);
    LoadoutSelectMenu_Unit(wsm, Silou_ent);
    // _LoadoutSelectMenu_Load(wsm, Silou_ent, renderer, &n9patch);
    Loadout_Set(&wsm->selected, stronghand, silou_can_equip[0]);

    struct Menu mc;
    mc.elem = 0;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    /* -- Long weapon names -- */
    Inventory_item *silou_eq = Unit_Equipment(Silou);
    silou_eq[0].used = 1;
    silou_eq[1].used = 0;
    silou_eq[2].used = 0;
    silou_eq[0].id = ITEM_ID_RETRACTABLE_WRISTBLADE;
    silou_eq[1].id = ITEM_ID_REPEATABLE_CROSSBOW;
    silou_eq[2].id = ITEM_ID_HONJOU_MASAMUNE;
    silou_eq[3].id = ITEM_ID_SILVERLIGHT_SPEAR;
    Weapon_Load(weapons_dtab, silou_eq[0].id);
    Weapon_Load(weapons_dtab, silou_eq[1].id);
    Weapon_Load(weapons_dtab, silou_eq[2].id);
    Weapon_Load(weapons_dtab, silou_eq[3].id);
    silou_can_equip[0] = ITEM1;
    silou_can_equip[1] = ITEM2;
    silou_can_equip[2] = ITEM3;
    silou_can_equip[2] = ITEM4;
    silou_can_equip[3] = ITEM5;
    Silou->can_equip.num   = 4;

    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_Long.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Short weapon names -- */
    Silou->flags.handedness = UNIT_HAND_LEFTIE;
    silou_eq = Unit_Equipment(Silou);
    silou_eq[0].used    = 1;
    silou_eq[1].used    = 10;
    silou_eq[2].used    = 20;
    silou_eq[3].used    = 21;
    silou_eq[4].used    = 12;
    silou_eq[0].id      = ITEM_ID_FLEURET;
    silou_eq[1].id      = ITEM_ID_FLEURET;
    silou_eq[2].id      = ITEM_ID_FLEURET;
    silou_eq[3].id      = ITEM_ID_FLEURET;
    silou_eq[4].id      = ITEM_ID_FLEURET;
    Weapon_Load(weapons_dtab, silou_eq[0].id);
    Weapon_Load(weapons_dtab, silou_eq[1].id);
    Weapon_Load(weapons_dtab, silou_eq[1].id);
    Weapon_Load(weapons_dtab, silou_eq[2].id);
    Weapon_Load(weapons_dtab, silou_eq[3].id);
    Weapon_Load(weapons_dtab, silou_eq[4].id);
    silou_can_equip[0]     = ITEM1;
    silou_can_equip[1]     = ITEM2;
    silou_can_equip[2]     = ITEM3;
    silou_can_equip[3]     = ITEM4;
    silou_can_equip[4]     = ITEM5;
    Silou->can_equip.num       = 5;

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
    Silou->can_equip.num = 4;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_LSelected_Usable4.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_Set(&wsm->selected, stronghand, ITEM3);
    mc.elem             = 0;
    Silou->can_equip.num = 3;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_LSelected_Usable3.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_Set(&wsm->selected, stronghand, ITEM2);
    mc.elem             = 0;
    Silou->can_equip.num = 2;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_LSelected_Usable2.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_Set(&wsm->selected, stronghand, ITEM1);
    mc.elem             = 0;
    Silou->can_equip.num = 1;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_LSelected_Usable1.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem             =  3;
    Silou->can_equip.num =  4;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_LNotSelected_Usable4.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem             =  2;
    Silou->can_equip.num =  3;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_LNotSelected_Usable3.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem             =  1;
    Silou->can_equip.num =  2;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_LNotSelected_Usable2.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem                 =  0;
    Silou->can_equip.num     =  1;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_LNotSelected_Usable1.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Silou->flags.handedness = UNIT_HAND_RIGHTIE;
    stronghand  = Unit_Hand_Strong(Silou);
    weakhand    = Unit_Hand_Weak(Silou);

    Loadout_Set(&wsm->selected, stronghand, ITEM4);
    mc.elem                 = 0;
    Silou->can_equip.num     = 4;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_RSelected_Usable4.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_Set(&wsm->selected, stronghand, ITEM3);
    mc.elem                 = 0;
    Silou->can_equip.num     = 3;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_RSelected_Usable3.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_Set(&wsm->selected, stronghand, ITEM2);
    mc.elem                 = 0;
    Silou->can_equip.num     = 2;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_RSelected_Usable2.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_Set(&wsm->selected, stronghand, ITEM1);
    mc.elem                 = 0;
    Silou->can_equip.num     = 1;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_RSelected_Usable1.png"),
                            renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem                 =  3;
    Silou->can_equip.num     =  4;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_RNotSelected_Usable4.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem                     =  2;
    Silou->can_equip.num         =  3;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_RNotSelected_Usable3.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem                     =  1;
    Silou->can_equip.num     =  2;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_RNotSelected_Usable2.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Loadout_None(&wsm->selected, stronghand);
    mc.elem                     =  0;
    Silou->can_equip.num     =  1;
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select",
                                      "WeaponSelectMenu_RNotSelected_Usable1.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- Testing header drawing --- */
    Silou->can_equip.num   = 4;
    LoadoutSelectMenu_Header_Set(wsm, "Drop 1 item for two-handing");
    LoadoutSelectMenu_Update(&mc, wsm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_loadout_select", "WeaponSelectMenu_Header.png"), renderer,
                            wsm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- SDL_free --- */
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
    tnecs_world_destroy(&world);
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
    tnecs_world *world = NULL;
    tnecs_world_genesis(&world);

#include "register_components.h"

    tnecs_entity Silou_ent  = TNECS_ENTITY_CREATE_wCOMPONENTS(world, Unit_ID, Position_ID);
    struct Unit *Silou      = IES_GET_COMPONENT(world, Silou_ent, Unit);

    /* -- Weapon dtab -- */
    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);

    /* -- Create Unit -- */
    Unit_Init(Silou);
    Silou->equipment.weapons_dtab = weapons_dtab;
    SDL_assert(Silou->equipment.num == 0);
    Inventory_item *silou_eq = Unit_Equipment(Silou);
    silou_eq[0].id      = ITEM_ID_FLEURET;
    silou_eq[1].id      = ITEM_ID_RAPIERE;
    silou_eq[2].id      = ITEM_ID_IRON_SWORD;
    silou_eq[3].id      = ITEM_ID_FLEURET;
    silou_eq[4].id      = ITEM_ID_WOODEN_SHIELD;
    silou_eq[5].id      = ITEM_ID_SALVE;
    Silou->flags.handedness            = UNIT_HAND_LEFTIE;
    Silou->flags.equippable            = ITEM_TYPE_SWORD;

    Weapon_Load(weapons_dtab, silou_eq[0].id);
    Weapon_Load(weapons_dtab, silou_eq[1].id);
    Weapon_Load(weapons_dtab, silou_eq[2].id);
    Weapon_Load(weapons_dtab, silou_eq[3].id);
    struct Weapon *weapons[6] = {0};
    weapons[0] = DTAB_GET(weapons_dtab, silou_eq[0].id);
    weapons[1] = DTAB_GET(weapons_dtab, silou_eq[1].id);
    weapons[2] = DTAB_GET(weapons_dtab, silou_eq[2].id);
    weapons[3] = DTAB_GET(weapons_dtab, silou_eq[3].id);
    weapons[4] = DTAB_GET(weapons_dtab, silou_eq[4].id);

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

    /* --- SDL_free --- */
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
