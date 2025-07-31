
#include "nourstest.h"
#include "platform.h"
#include "menu/stats.h"
#include "unit/unit.h"
#include "unit/flags.h"
#include "unit/status.h"
#include "RNG.h"
#include "globals.h"

#define TEST_SET_EQUIPMENT(world, ID, eq) \
    seteqentity  = TNECS_ENTITY_CREATE_wCOMPONENTS(world, Inventory_item_ID);\
    seteqinvitem = IES_GET_COMPONENT(world, seteqentity, Inventory_item);\
    seteqinvitem->id = ID;\
    silou_eq[eq] = seteqentity;

void test_menu_stats() {
    /* -- Preliminaries -- */
    tnecs_world *world = NULL;
    tnecs_world_genesis(&world);
    gl_world = world;

#include "register/components.h"

    sota_mkdir("menu_stats");
    Names_Load_All();

    /* -- Weapon dtab -- */
    gl_weapons_dtab = DTAB_INIT(gl_weapons_dtab, struct Weapon);
    gl_items_dtab   = DTAB_INIT(gl_items_dtab,   struct Item);

    /* -- Create n9patch -- */
    struct n9Patch n9patch = n9Patch_default;
    // render_target is NULL cause there is no render_target!
    SDL_Texture *render_target = NULL;

    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    /* -- Stats Menu -- */
    struct StatsMenu *sm    = StatsMenu_Alloc();

    /* - loading fonts - */
    sm->pixelnours = PixelFont_Alloc();
    PixelFont_Load(sm->pixelnours, renderer, PATH_JOIN("..", "assets", "fonts", "pixelnours.png"));
    SDL_assert(sm->pixelnours);
    sm->pixelnours->y_offset = pixelfont_y_offset;

    sm->pixelnours_big = PixelFont_Alloc();
    char *path = PATH_JOIN("..", "assets", "fonts", "pixelnours_Big.png");
    PixelFont_Load(sm->pixelnours_big, renderer, path);
    SDL_assert(sm->pixelnours_big);
    sm->pixelnours_big->y_offset = pixelfont_big_y_offset;

    /* -- Create Unit -- */
    struct Unit Silou = Unit_default;
    Silou.id.self = UNIT_ID_SILOU;
    Unit_Init(&Silou);
    /* - title - */
    Silou.id.title = 0;
    global_unitTitlesId[Silou.id.self] = Silou.id.title;
    global_unitTitles[Silou.id.title] = s8_mut("Playful Mage");
    SDL_assert(Silou.equipment.num == 0);
    jsonio_readJSON(s8_literal(PATH_JOIN("units", "Silou_test.json")), &Silou);
    SDL_assert(Silou.equipment.num == 4);

    /* - Unit equip - */
    tnecs_entity    seteqentity     = TNECS_NULL;
    Inventory_item *seteqinvitem    = NULL;
    tnecs_entity *silou_eq = Unit_Equipment(&Silou);
    TEST_SET_EQUIPMENT(world, ITEM_ID_FLEURET, 0);

    Weapon_Load(gl_weapons_dtab, Unit_InvItem(&Silou, ITEM1)->id);

    // int stronghand  = Unit_Hand_Strong(&Silou);
    int weakhand    = Unit_Hand_Weak(&Silou);

    Unit_Item_Drop(&Silou,           weakhand);
    Unit_Item_Takeat(&Silou, seteqentity, weakhand);
    SDL_assert(Silou.equipment.num == 4);

    Unit_Equip(&Silou, weakhand, weakhand);
    SDL_assert(Unit_isEquipped(&Silou, weakhand));
    nourstest_true(Unit_canAttack(&Silou));
    // Unit_stats ES_S = Unit_effectiveStats(&Silou);
    // Computed_Stats cs_S = Unit_computedStats(&Silou, -1, ES_S);

    StatsMenu_Load(sm, &Silou, renderer, &n9patch);

    /* --- RENDERS --- */
    /* -- test 0 -- */
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    SDL_assert(sm->texture != NULL);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Test.png"), renderer, sm->texture,
                            SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Capped stats -- */
    Unit_Cap_Stats(&Silou);
    // ES_S = Unit_effectiveStats(&Silou);
    // cs_S = Unit_computedStats(&Silou, -1, ES_S);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Capped.png"), renderer, sm->texture,
                            SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Half stats -- */
    Unit_HalfCap_Stats(&Silou);
    // ES_S = Unit_effectiveStats(&Silou);
    // cs_S = Unit_computedStats(&Silou, -1, ES_S);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_HalfCapped.png"), renderer, sm->texture,
                            SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- Computed Stats --- */
    /* - Hit single digits - */
    // cs_S.hit    = 1;
    // cs_S.dodge  = 8;
    // /* - Crit single digits - */
    // cs_S.crit   = 2;
    // cs_S.favor  = 9;
    // /* - Speed single digits - */
    // cs_S.speed  = 3;
    // /* - Attack single digits - */
    // cs_S.attack.physical       = 4;
    // cs_S.attack.magical        = 5;
    // /* - Prot single digits - */
    // cs_S.protection.physical   = 6;
    // cs_S.protection.magical    = 7;
    // /* - Range single digits - */
    // cs_S.range_loadout.min = 1;
    // cs_S.range_loadout.max = 2;
    /* - Regret single digits - */

    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Digits_1.png"), renderer, sm->texture,
                            SDL_PIXELFORMAT_ARGB8888, render_target);
    /* --- Computed Stats --- */
    /* - Hit double digits - */
    // cs_S.hit    = 10;
    // cs_S.dodge  = 80;
    // /* - Crit double digits - */
    // cs_S.crit   = 20;
    // cs_S.favor  = 90;
    // /* - Speed double digits - */
    // cs_S.speed  = 30;
    // /* - Attack double digits - */
    // cs_S.attack.physical       = 40;
    // cs_S.attack.magical        = 50;
    // /* - Prot double digits - */
    // cs_S.protection.physical   = 60;
    // cs_S.protection.magical    = 70;
    // /* - Regret double digits - */

    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Digits_2.png"), renderer, sm->texture,
                            SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - Hit triple digits - */
    // cs_S.hit    = 100;
    // cs_S.dodge  = 100;
    // /* - Crit triple digits - */
    // cs_S.crit   = 100;
    // cs_S.favor  = 100;
    /* - Regret triple digits - */
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Digits_3.png"), renderer, sm->texture,
                            SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - Attack double digits - */
    // cs_S.attack.True = 9;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Digits_4.png"), renderer, sm->texture,
                            SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- HANDEDNESS --- */
    /* -- Ambidextrous -- */
    Silou.flags.handedness = UNIT_HAND_AMBIDEXTROUS;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Handedness_Ambidex.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Rightie -- */
    Silou.flags.handedness = UNIT_HAND_RIGHTIE;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Handedness_Rightie.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Leftie -- */
    Silou.flags.handedness = UNIT_HAND_LEFTIE;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Handedness_Leftie.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- MOUNT --- */
    /* - Salamander - */
    // struct Mount mount = Mount_default_salamander;
    // Silou.mount.ptr = &mount;
    // SDL_assert(Silou.mount.ptr->type = MOUNT_SALAMANDER);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Mount_Salamander.png"), renderer,
                            sm->texture,
                            SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - Horse - */
    // mount = Mount_default_horse;
    // SDL_assert(Silou.mount.ptr->type = MOUNT_HORSE);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Mount_Horse.png"), renderer, sm->texture,
                            SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - Eagle - */
    // mount = Mount_default_eagle;
    // SDL_assert(Silou.mount.ptr->type = MOUNT_EAGLE);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Mount_Eagle.png"), renderer, sm->texture,
                            SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - Pegasus - */
    // mount = Mount_default_pegasus;
    // SDL_assert(Silou.mount.ptr->type = MOUNT_PEGASUS);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Mount_Pegasus.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Equipped in hands -- */
    /* -- Unequipped in hands -- */

    /* -- Weapon types -- */
    /* - sword + lance + shield + offhand - */
    Silou.flags.equippable = ITEM_TYPE_SWORD + ITEM_TYPE_LANCE + ITEM_TYPE_SHIELD + ITEM_TYPE_OFFHAND;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Weapon_Duke.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - axe + bow - */
    Silou.flags.equippable = ITEM_TYPE_AXE + ITEM_TYPE_BOW;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Weapon_AxBo.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - angelic + claw + Sword + Lance + shield - */
    Silou.flags.equippable = ITEM_TYPE_SWORD + ITEM_TYPE_LANCE + ITEM_TYPE_SHIELD + ITEM_TYPE_CLAW +
                             ITEM_TYPE_ANGELIC;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Weapon_Angel.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - demonic  + staff + trinket - */
    Silou.flags.equippable = ITEM_TYPE_STAFF + ITEM_TYPE_DEMONIC + ITEM_TYPE_TRINKET;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Weapon_DeStTr.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    /* - NONE - */
    Silou.flags.equippable = ITEM_TYPE_NULL;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Weapon_None.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Rescuing -- */
    // TODO: RESCUE ICON
    Silou.rescue.id = UNIT_ID_ERWIN;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Rescue.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Status -- */
    Unit_Status status = {0};
    i32 turns = 1;
    /* - poisoned - */
    Unit_Status_Add(&status, UNIT_STATUS_POISONED, turns);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Status_Poisoned.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - stunned - */
    Unit_Status_Add(&status, UNIT_STATUS_STUNNED, turns);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Status_Stunned.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - rooted - */
    Unit_Status_Add(&status, UNIT_STATUS_ROOTED, turns);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Status_Rooted.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - seduced - */
    Unit_Status_Add(&status, UNIT_STATUS_SEDUCED, turns);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Status_Seduced.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - berserk - */
    Unit_Status_Add(&status, UNIT_STATUS_BERSERK, turns);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Status_Berserk.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - terror - */
    Unit_Status_Add(&status, UNIT_STATUS_TERROR, turns);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Status_Terror.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - stone - */
    Unit_Status_Add(&status, UNIT_STATUS_STONE, turns);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Status_Stone.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - slowed - */
    Unit_Status_Add(&status, UNIT_STATUS_SLOWED, turns);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Status_Slowed.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - stasis - */
    Unit_Status_Add(&status, UNIT_STATUS_STASIS, turns);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Status_Stasis.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - silence - */
    Unit_Status_Add(&status, UNIT_STATUS_SILENCE, turns);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Status_Silence.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- REGRETS -- */
    /* -- 0 -- */
    Silou.counters.regrets = 0;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Regrets_0.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Single digits -- */
    Silou.counters.regrets = 2;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Regrets_Digits_1.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Double digits -- */
    Silou.counters.regrets = 25;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Regrets_Digits_2.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Triple digits -- */
    Silou.counters.regrets = 137;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Regrets_Digits_3.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Skills -- */
    // TODO: Skill Icons

    /* -- Face -- */
    // TODO: Faces Icons

    /* -- Empty inventory -- */
    Silou.equipment.num = 0;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Equipment_0.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Full inventory -- */
    Silou.equipment.num = 4;
    TEST_SET_EQUIPMENT(world, ITEM_ID_IRON_SWORD, 4);
    Weapon_Load(gl_weapons_dtab, Unit_InvItem(&Silou, ITEM5)->id);
    Unit_Item_Take(&Silou, seteqentity);
    SDL_assert(Silou.equipment.num == 5);

    TEST_SET_EQUIPMENT(world, ITEM_ID_IRON_LANCE, 5);
    seteqinvitem->used = 10;
    Weapon_Load(gl_weapons_dtab, Unit_InvItem(&Silou, ITEM6)->id);
    Unit_Item_Take(&Silou, seteqentity);
    SDL_assert(Silou.equipment.num == 5);

    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Equipment_Full.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Low Durability -- */
    silou_eq = Unit_Equipment(&Silou);
    Unit_InvItem(&Silou, ITEM1)->used = 20;
    Unit_InvItem(&Silou, ITEM2)->used = 21;
    Unit_InvItem(&Silou, ITEM3)->used = 12;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Durability_Digits_1.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Long weapon names -- */
    Unit_InvItem(&Silou, ITEM1)->used = 0;
    Unit_InvItem(&Silou, ITEM2)->used = 0;
    Unit_InvItem(&Silou, ITEM3)->used = 0;

    Unit_InvItem(&Silou, ITEM1)->id   = ITEM_ID_RETRACTABLE_WRISTBLADE;
    Unit_InvItem(&Silou, ITEM2)->id   = ITEM_ID_REPEATABLE_CROSSBOW;
    Unit_InvItem(&Silou, ITEM3)->id   = ITEM_ID_HONJOU_MASAMUNE;
    Unit_InvItem(&Silou, ITEM4)->id   = ITEM_ID_SILVERLIGHT_SPEAR;
    Weapon_Load(gl_weapons_dtab, Unit_InvItem(&Silou, ITEM1)->id);
    Weapon_Load(gl_weapons_dtab, Unit_InvItem(&Silou, ITEM2)->id);
    Weapon_Load(gl_weapons_dtab, Unit_InvItem(&Silou, ITEM3)->id);
    Weapon_Load(gl_weapons_dtab, Unit_InvItem(&Silou, ITEM4)->id);
    // TODO: when stats menu supports items
    // Silou._equipment[4].id   = ITEM_ID_DOUBLE_SIDED_WHETSTONE;
    // Item_Load(gl_items_dtab, Silou._equipment[4].id);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Names_Long.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- SDL_free --- */
    PixelFont_Free(sm->pixelnours, true);
    PixelFont_Free(sm->pixelnours_big, true);

    Game_Weapons_Free(&gl_weapons_dtab);
    Game_Items_Free(&gl_items_dtab);
    SDL_FreeSurface(surface);
    StatsMenu_Free(sm);

    if (n9patch.texture != NULL)
        SDL_DestroyTexture(n9patch.texture);

    SDL_DestroyRenderer(renderer);
    Unit_Free(&Silou);
    tnecs_world_destroy(&world);
    gl_world = NULL;
}
#undef TEST_SET_EQUIPMENT
