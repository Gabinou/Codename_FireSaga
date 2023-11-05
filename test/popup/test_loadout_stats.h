#include "nourstest.h"
#include "platform.h"
#include "popup/loadout_stats.h"
#include "unit.h"

void test_popup_loadout_stats() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);
    /* -- Preliminaries -- */
    sota_mkdir("popup_loadout_stats");

    /* -- Tinymt -- */
    struct TINYMT32_T tinymt32;
    RNG_Init_tinymt(&tinymt32);

    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    /* -- Create n9patch -- */
    // render_target is NULL cause there is render_target!
    struct n9Patch n9patch = n9Patch_default;
    SDL_Texture *render_target = NULL;

    /* -- Create Computed stats -- */
    struct Computed_Stats low_cs = {0};
    low_cs.attack[DMG_TYPE_PHYSICAL]        = 1;
    low_cs.attack[DMG_TYPE_MAGICAL]         = 2;
    low_cs.protection[DMG_TYPE_PHYSICAL]    = 3;
    low_cs.protection[DMG_TYPE_MAGICAL]     = 4;
    low_cs.hit                              = 5;
    low_cs.dodge                            = 6;
    low_cs.crit                             = 7;
    low_cs.favor                            = 8;
    low_cs.speed                            = 9;
    low_cs.range_loadout.min                = 2;
    low_cs.range_loadout.max                = 2;

    struct Computed_Stats high_cs           = {0};
    high_cs.attack[DMG_TYPE_PHYSICAL]       =  10;
    high_cs.attack[DMG_TYPE_MAGICAL]        =  20;
    high_cs.protection[DMG_TYPE_PHYSICAL]   =  30;
    high_cs.protection[DMG_TYPE_MAGICAL]    =  40;
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
    PixelFont_Load(pls.pixelnours, renderer, PATH_JOIN("..", "assets", "Fonts", "pixelnours.png"));

    pls.pixelnours_big = PixelFont_Alloc();
    PixelFont_Load(pls.pixelnours_big, renderer, PATH_JOIN("..", "assets", "Fonts",
                                                           "pixelnours_Big.png"));
    SDL_assert(pls.pixelnours_big);

    /* -- Two handing weapon -- */
    struct Unit unit = Unit_default;
    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);
    Unit_InitWweapons(&unit, weapons_dtab);

    pls.unit = &unit;

    unit.isTwoHanding = true;
    struct Inventory_item item = Inventory_item_default;

    item.id = ITEM_ID_GLAIVE;
    Weapon_Load(weapons_dtab, item.id);
    Unit_Item_Takeat(&unit, item, 0);
    item.id = ITEM_ID_LEATHER_SHIELD;
    Weapon_Load(weapons_dtab, item.id);
    Unit_Item_Takeat(&unit, item, 1);
    pls.type_left = ITEM_TYPE_EXP_SWORD;
    pls.item_left   = 0;
    pls.item_right  = 1;

    pls.l_equip_override   = false;
    pls.r_equip_override  = true;

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

    unit.handedness = UNIT_HAND_LEFTIE;
    PopUp_Loadout_Stats_Update(&pls, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_loadout_stats", "PopupLoadoutStats_TwoHanding_LEFTIE.png"),
                            renderer, pls.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    unit.handedness = UNIT_HAND_AMBIDEXTROUS;
    PopUp_Loadout_Stats_Update(&pls, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_loadout_stats",
                                      "PopupLoadoutStats_TwoHanding_AMBIDEXTROUS.png"),
                            renderer, pls.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    unit.isTwoHanding = false;

    /* -- All higher -- */
    pls.previous_cs = low_cs;
    pls.new_cs      = high_cs;
    unit.handedness = UNIT_HAND_RIGHTIE;
    pls.type_left   = ITEM_TYPE_EXP_SHIELD;
    pls.type_right  = ITEM_TYPE_EXP_SWORD;

    PopUp_Loadout_Stats_Update(&pls, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_loadout_stats", "PopupLoadoutStats_Lower.png"), renderer,
                            pls.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- All equal -- */
    pls.previous_cs = low_cs;
    pls.new_cs      = low_cs;
    unit.handedness = UNIT_HAND_RIGHTIE;
    pls.type_left   = ITEM_TYPE_EXP_SHIELD;
    pls.type_right  = ITEM_TYPE_EXP_AXE;

    PopUp_Loadout_Stats_Update(&pls, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_loadout_stats", "PopupLoadoutStats.png"), renderer,
                            pls.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- All Lower -- */
    pls.previous_cs = high_cs;;

    pls.new_cs      = low_cs;
    unit.handedness = UNIT_HAND_LEFTIE;
    pls.type_right  = ITEM_TYPE_EXP_LANCE;
    pls.type_left   = ITEM_TYPE_EXP_STAFF;

    PopUp_Loadout_Stats_Update(&pls, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_loadout_stats", "PopupLoadoutStats_Higher.png"), renderer,
                            pls.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- 1 digit/ 2digits -- */
    low_cs.attack[DMG_TYPE_PHYSICAL] = 1;
    low_cs.attack[DMG_TYPE_MAGICAL] = 20;
    low_cs.protection[DMG_TYPE_PHYSICAL] = 3;
    low_cs.protection[DMG_TYPE_MAGICAL] = 40;
    low_cs.hit = 5;
    low_cs.dodge = 60;
    low_cs.crit = 7;
    low_cs.favor = 80;
    low_cs.speed = 9;
    unit.handedness = UNIT_HAND_RIGHTIE;
    pls.type_right  = ITEM_TYPE_EXP_CLAW;
    pls.type_left   = ITEM_TYPE_EXP_OFFHAND;


    pls.previous_cs = low_cs;
    pls.new_cs = low_cs;
    PopUp_Loadout_Stats_Update(&pls, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_loadout_stats", "PopupLoadoutStats_Digits_1_2.png"),
                            renderer, pls.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- 2 digit/ 1 digits -- */
    low_cs.attack[DMG_TYPE_PHYSICAL] = 10;
    low_cs.attack[DMG_TYPE_MAGICAL] = 2;
    low_cs.protection[DMG_TYPE_PHYSICAL] = 30;
    low_cs.protection[DMG_TYPE_MAGICAL] = 4;
    low_cs.hit = 50;
    low_cs.dodge = 6;
    low_cs.crit = 70;
    low_cs.favor = 8;
    low_cs.speed = 9;
    unit.handedness = UNIT_HAND_LEFTIE;
    pls.type_right  = ITEM_TYPE_EXP_DEMONIC;
    pls.type_left   = ITEM_TYPE_EXP_ELEMENTAL;

    pls.previous_cs = low_cs;
    pls.new_cs = low_cs;
    /* -- 2 digit/ 1 digits -- */

    PopUp_Loadout_Stats_Update(&pls, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_loadout_stats", "PopupLoadoutStats_Digits_2_1.png"),
                            renderer, pls.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- True damage -- */
    low_cs.attack[DMG_TYPE_TRUE] = 1;
    pls.previous_cs = low_cs;
    pls.new_cs = low_cs;
    unit.handedness = UNIT_HAND_RIGHTIE;
    pls.l_equip_override   = true;
    pls.r_equip_override  = true;

    PopUp_Loadout_Stats_Update(&pls, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_loadout_stats", "PopupLoadoutStats_True.png"),
                            renderer, pls.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- True damage 2 digits -- */
    low_cs.attack[DMG_TYPE_MAGICAL] = 21;
    low_cs.attack[DMG_TYPE_TRUE] = 34;
    pls.previous_cs = low_cs;
    pls.new_cs = low_cs;
    unit.handedness = UNIT_HAND_LEFTIE;
    low_cs.hit = 100;
    low_cs.dodge = 14;
    pls.previous_cs = low_cs;
    pls.new_cs = low_cs;
    pls.l_equip_override   = false;
    pls.r_equip_override  = false;

    PopUp_Loadout_Stats_Update(&pls, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("popup_loadout_stats", "PopupLoadoutStats_True2.png"),
                            renderer, pls.texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- FREE -- */
    Unit_SDL_free(&unit);
    n9Patch_SDL_free(&n9patch);
    PixelFont_SDL_free(pls.pixelnours, true);
    PixelFont_SDL_free(pls.pixelnours_big, true);
    PopUp_Loadout_Stats_SDL_free(&pls);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
    Weapons_All_SDL_free(weapons_dtab);
    DTAB_FREE(weapons_dtab);
    SDL_Quit();
}