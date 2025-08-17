
#include "macros.h"
#include "globals.h"
#include "nourstest.h"
#include "platform.h"
#include "unit/unit.h"
#include "popup/loadout_stats.h"

#define TEST_SET_EQUIPMENT(world, ID, eq) \
    seteqentity  = IES_E_CREATE_wC(world, Inventory_item_ID);\
    seteqinvitem = IES_GET_C(world, seteqentity, Inventory_item);\
    seteqinvitem->id = ID;\
    silou_eq[eq - ITEM1] = seteqentity;

void test_popup_loadout_stats() {
    /* -- Preliminaries -- */
    sota_mkdir("popup_loadout_stats");
    Names_Load_All();

    tnecs_E    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;

    /* Tnecs init */
    tnecs_W *world = NULL;
    tnecs_genesis(&world);
    gl_world = world;

#include "register/components.h"

    TNECS_REGISTER_COMPONENT(world, Unit, NULL, NULL);
    TNECS_REGISTER_COMPONENT(world, Position, NULL, NULL);
    tnecs_E Silou  = IES_E_CREATE_wC(world, Unit_ID, Position_ID);
    // tnecs_E Erwin  = IES_E_CREATE_wC(world, Unit_ID, Position_ID);
    // tnecs_E Enemy  = IES_E_CREATE_wC(world, Unit_ID, Position_ID);

    Unit *silou         = IES_GET_C(world, Silou, Unit);
    // Unit *erwin         = IES_GET_C(world, Erwin, Unit);
    // Unit *enemy         = IES_GET_C(world, Enemy, Unit);

    // Position *silou_pos = IES_GET_C(world, Silou, Position);
    // Position *erwin_pos = IES_GET_C(world, Erwin, Position);
    // Position *enemy_pos = IES_GET_C(world, Enemy, Position);

    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    /* -- Create n9patch -- */
    // render_target is NULL cause there is render_target!
    struct n9Patch n9patch = n9Patch_default;
    SDL_Texture *render_target = NULL;

    /* -- Create Computed stats -- */
    struct Computed_Stats low_cs = {0};
    low_cs.attack.physical        =   1;
    low_cs.attack.magical         =   2;
    low_cs.protection.physical    =   3;
    low_cs.protection.magical     =   4;
    low_cs.hit                              =   5;
    low_cs.dodge                            =   6;
    low_cs.crit                             =   7;
    low_cs.favor                            =   8;
    low_cs.speed                            =   9;
    low_cs.range_loadout.min                =   2;
    low_cs.range_loadout.max                =   2;

    struct Computed_Stats high_cs           = {0};
    high_cs.attack.physical       =  10;
    high_cs.attack.magical        =  20;
    high_cs.protection.physical   =  30;
    high_cs.protection.magical    =  40;
    high_cs.hit                             = 100;
    high_cs.dodge                           =  60;
    high_cs.crit                            =  70;
    high_cs.favor                           =  80;
    high_cs.speed                           =  90;
    high_cs.range_loadout.min               =   1;
    high_cs.range_loadout.max               =   3;

    /* -- Create PopUp_Loadout_Stats -- */
    struct PopUp_Loadout_Stats pls = PopUp_Loadout_Stats_default;
    pls.type_left   = ITEM_TYPE_EXP_SHIELD;
    pls.type_right  = ITEM_TYPE_EXP_SWORD;
    PopUp_Loadout_Stats_Load(&pls, renderer, &n9patch);

    /* - loading fonts - */
    pls.pixelnours = PixelFont_Alloc();
    PixelFont_Load(pls.pixelnours, renderer, PATH_JOIN("..", "assets", "fonts", "pixelnours.png"));

    pls.pixelnours_big = PixelFont_Alloc();
    PixelFont_Load(pls.pixelnours_big, renderer, PATH_JOIN("..", "assets", "fonts",
                                                           "pixelnours_Big.png"));
    SDL_assert(pls.pixelnours_big);

    /* -- Two handing weapon -- */
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, Weapon);
    gl_items_dtab   = DTAB_INIT(gl_items_dtab, Item);
    Unit_Init(silou);

    pls.unit_ent = Silou;
    tnecs_E *silou_eq = Unit_Equipment(silou);
    TEST_SET_EQUIPMENT(world, ITEM_ID_GLAIVE, ITEM1);
    Weapon_Load(gl_weapons_dtab, seteqinvitem->id);

    TEST_SET_EQUIPMENT(world, ITEM_ID_LEATHER_SHIELD, ITEM2);
    Weapon_Load(gl_weapons_dtab, seteqinvitem->id);
    pls.type_left   = ITEM_TYPE_EXP_SWORD;
    Loadout_Set(&pls.loadout_initial, UNIT_HAND_LEFT,   ITEM1);
    Loadout_Set(&pls.loadout_initial, UNIT_HAND_RIGHT,  ITEM2);

    pls.l_equip_override    = false;
    pls.r_equip_override    = true;

    PopUp_Loadout_Stats_Update(&pls, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_loadout_stats", "PopupLoadoutStats_TwoHanding.png"),
                            renderer, pls.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    pls.type_left = ITEM_TYPE_EXP_ANGELIC;
    PopUp_Loadout_Stats_Update(&pls, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_loadout_stats",
                                      "PopupLoadoutStats_TwoHanding_Angelic.png"),
                            renderer, pls.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    pls.l_equip_override   = true;
    pls.r_equip_override  = false;

    pls.type_left = ITEM_TYPE_EXP_LANCE;
    PopUp_Loadout_Stats_Update(&pls, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_loadout_stats", "PopupLoadoutStats_TwoHanding_Lance.png"),
                            renderer, pls.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Unit_Handedness_set(silou, UNIT_HAND_LEFTIE);
    PopUp_Loadout_Stats_Update(&pls, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_loadout_stats", "PopupLoadoutStats_TwoHanding_LEFTIE.png"),
                            renderer, pls.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    Unit_Handedness_set(silou, UNIT_HAND_AMBIDEXTROUS);
    PopUp_Loadout_Stats_Update(&pls, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_loadout_stats",
                                      "PopupLoadoutStats_TwoHanding_AMBIDEXTROUS.png"),
                            renderer, pls.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- All higher -- */
    pls.initial_cs      = low_cs;
    pls.selected_cs     = high_cs;
    Unit_Handedness_set(silou, UNIT_HAND_RIGHTIE);
    pls.type_left       = ITEM_TYPE_EXP_SHIELD;
    pls.type_right      = ITEM_TYPE_EXP_SWORD;

    PopUp_Loadout_Stats_Update(&pls, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_loadout_stats", "PopupLoadoutStats_Lower.png"), renderer,
                            pls.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- All equal -- */
    pls.initial_cs      = low_cs;
    pls.selected_cs     = low_cs;
    Unit_Handedness_set(silou, UNIT_HAND_RIGHTIE);
    pls.type_left       = ITEM_TYPE_EXP_SHIELD;
    pls.type_right      = ITEM_TYPE_EXP_AXE;

    PopUp_Loadout_Stats_Update(&pls, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_loadout_stats", "PopupLoadoutStats.png"), renderer,
                            pls.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- All Lower -- */
    pls.initial_cs      = high_cs;
    pls.selected_cs     = low_cs;
    Unit_Handedness_set(silou, UNIT_HAND_LEFTIE);
    pls.type_right      = ITEM_TYPE_EXP_LANCE;
    pls.type_left       = ITEM_TYPE_EXP_STAFF;

    PopUp_Loadout_Stats_Update(&pls, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_loadout_stats", "PopupLoadoutStats_Higher.png"), renderer,
                            pls.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- 1 digit/ 2digits -- */
    low_cs.attack.physical        =  1;
    low_cs.attack.magical         = 20;
    low_cs.protection.physical    =  3;
    low_cs.protection.magical     = 40;
    low_cs.hit                              =  5;
    low_cs.dodge                            = 60;
    low_cs.crit                             =  7;
    low_cs.favor                            = 80;
    low_cs.speed                            =  9;
    Unit_Handedness_set(silou, UNIT_HAND_RIGHTIE);
    pls.type_right                          = ITEM_TYPE_EXP_CLAW;
    pls.type_left                           = ITEM_TYPE_EXP_OFFHAND;

    pls.initial_cs  = low_cs;
    pls.selected_cs = low_cs;
    PopUp_Loadout_Stats_Update(&pls, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_loadout_stats", "PopupLoadoutStats_Digits_1_2.png"),
                            renderer, pls.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- 2 digit/ 1 digits -- */
    low_cs.attack.physical = 10;
    low_cs.attack.magical = 2;
    low_cs.protection.physical = 30;
    low_cs.protection.magical = 4;
    low_cs.hit = 50;
    low_cs.dodge = 6;
    low_cs.crit = 70;
    low_cs.favor = 8;
    low_cs.speed = 9;
    Unit_Handedness_set(silou, UNIT_HAND_LEFTIE);
    pls.type_right  = ITEM_TYPE_EXP_DEMONIC;
    pls.type_left   = ITEM_TYPE_EXP_ELEMENTAL;

    pls.initial_cs = low_cs;
    pls.selected_cs = low_cs;
    /* -- 2 digit/ 1 digits -- */

    PopUp_Loadout_Stats_Update(&pls, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_loadout_stats", "PopupLoadoutStats_Digits_2_1.png"),
                            renderer, pls.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- True damage -- */
    low_cs.attack.True = 1;
    pls.initial_cs = low_cs;
    pls.selected_cs = low_cs;
    Unit_Handedness_set(silou, UNIT_HAND_RIGHTIE);
    pls.l_equip_override   = true;
    pls.r_equip_override  = true;

    PopUp_Loadout_Stats_Update(&pls, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_loadout_stats", "PopupLoadoutStats_True.png"),
                            renderer, pls.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- True damage 2 digits -- */
    low_cs.attack.magical     = 21;
    low_cs.attack.True        = 34;
    pls.initial_cs                      = low_cs;
    pls.selected_cs                     = low_cs;
    Unit_Handedness_set(silou, UNIT_HAND_LEFTIE);
    low_cs.hit                          = 100;
    low_cs.dodge                        = 14;
    pls.initial_cs                      = low_cs;
    pls.selected_cs                     = low_cs;
    pls.l_equip_override                = false;
    pls.r_equip_override                = false;

    PopUp_Loadout_Stats_Update(&pls, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_loadout_stats", "PopupLoadoutStats_True2.png"),
                            renderer, pls.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- SDL_free -- */
    Unit_Free(silou);
    n9Patch_Free(&n9patch);
    PixelFont_Free(pls.pixelnours, true);
    PixelFont_Free(pls.pixelnours_big, true);
    PopUp_Loadout_Stats_Free(&pls);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);

    SDL_Quit();
    tnecs_finale(&world);
    gl_world = NULL;
}
#undef TEST_SET_EQUIPMENT
