#include "nourstest.h"
#include "platform.h"
#include "menu/stats.h"
#include "unit.h"
#include "RNG.h"

void test_menu_stats() {
    SDL_Log("%s " STRINGIZE(__LINE__), __func__);
    /* -- Preliminaries -- */
    sota_mkdir("menu_stats");

    /* -- Weapon dtab -- */
    struct dtab *weapons_dtab = DTAB_INIT(weapons_dtab, struct Weapon);
    struct dtab *items_dtab   = DTAB_INIT(items_dtab,   struct Item);

    /* -- Create n9patch -- */
    struct n9Patch n9patch = n9Patch_default;
    // render_target is NULL cause there is no render_target!
    SDL_Texture *render_target = NULL;

    /* -- Create renderer -- */
    SDL_Surface  *surface  = Filesystem_indexedSurface_Init(1024, 1024);
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(surface);

    /* -- Stats Menu -- */
    struct StatsMenu *sm    = StatsMenu_Alloc();
    sm->update_stats        = false;

    /* - loading fonts - */
    sm->pixelnours = PixelFont_Alloc();
    PixelFont_Load(sm->pixelnours, renderer, PATH_JOIN("..", "assets", "Fonts", "pixelnours.png"));
    SDL_assert(sm->pixelnours);
    sm->pixelnours->y_offset = pixelfont_y_offset;

    sm->pixelnours_big = PixelFont_Alloc();
    char *path = PATH_JOIN("..", "assets", "Fonts", "pixelnours_Big.png");
    PixelFont_Load(sm->pixelnours_big, renderer, path);
    SDL_assert(sm->pixelnours_big);
    sm->pixelnours_big->y_offset = pixelfont_big_y_offset;

    /* -- Create Unit -- */
    struct Unit Silou = Unit_default;
    Unit_Init(&Silou);
    /* - title - */
    memcpy(Silou.title, "Playful Mage", strlen("Playful Mage"));
    Silou.weapons_dtab = weapons_dtab;
    SDL_assert(Silou.num_equipment == 0);
    jsonio_readJSON(s8_literal(PATH_JOIN("units", "Silou_test.json")), &Silou);
    SDL_assert(Silou.num_equipment == 4);

    /* - Unit equip - */
    struct Inventory_item in_wpn = Inventory_item_default;
    in_wpn.id = ITEM_ID_FLEURET;
    in_wpn.used = 0;
    Weapon_Load(weapons_dtab, in_wpn.id);

    Unit_Item_Drop(&Silou,           UNIT_HAND_WEAK);
    Unit_Item_Takeat(&Silou, in_wpn, UNIT_HAND_WEAK);
    SDL_assert(Silou.num_equipment == 4);
    Silou.equipped[UNIT_HAND_WEAK] = true;
    SDL_assert(Silou.equipped[UNIT_HAND_WEAK]);
    nourstest_true(Unit_canAttack(&Silou));
    Unit_computedStats(&Silou, -1);
    Unit_effectiveStats(&Silou);

    StatsMenu_Load(sm, &Silou, renderer, &n9patch);

    /* --- RENDERS --- */
    /* -- test 0 -- */
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    SDL_assert(sm->texture != NULL);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Test.png"), renderer, sm->texture,
                            SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Capped stats -- */
    Unit_Cap_Stats(&Silou);
    Unit_computedStats(&Silou, -1);
    Unit_effectiveStats(&Silou);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Capped.png"), renderer, sm->texture,
                            SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Half stats -- */
    Unit_HalfCap_Stats(&Silou);
    Unit_computedStats(&Silou, -1);
    Unit_effectiveStats(&Silou);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_HalfCapped.png"), renderer, sm->texture,
                            SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- Computed Stats --- */
    /* - Hit single digits - */
    Silou.computed_stats.hit    = 1;
    Silou.computed_stats.dodge  = 8;
    /* - Crit single digits - */
    Silou.computed_stats.crit   = 2;
    Silou.computed_stats.favor  = 9;
    /* - Speed single digits - */
    Silou.computed_stats.speed  = 3;
    /* - Attack single digits - */
    Silou.computed_stats.attack[DAMAGE_TYPE_PHYSICAL]       = 4;
    Silou.computed_stats.attack[DAMAGE_TYPE_MAGICAL]        = 5;
    /* - Prot single digits - */
    Silou.computed_stats.protection[DAMAGE_TYPE_PHYSICAL]   = 6;
    Silou.computed_stats.protection[DAMAGE_TYPE_MAGICAL]    = 7;
    /* - Range single digits - */
    Silou.computed_stats.range_loadout.min = 1;
    Silou.computed_stats.range_loadout.max = 2;
    /* - Regret single digits - */

    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Digits_1.png"), renderer, sm->texture,
                            SDL_PIXELFORMAT_ARGB8888, render_target);
    /* --- Computed Stats --- */
    /* - Hit double digits - */
    Silou.computed_stats.hit    = 10;
    Silou.computed_stats.dodge  = 80;
    /* - Crit double digits - */
    Silou.computed_stats.crit   = 20;
    Silou.computed_stats.favor  = 90;
    /* - Speed double digits - */
    Silou.computed_stats.speed  = 30;
    /* - Attack double digits - */
    Silou.computed_stats.attack[DAMAGE_TYPE_PHYSICAL]       = 40;
    Silou.computed_stats.attack[DAMAGE_TYPE_MAGICAL]        = 50;
    /* - Prot double digits - */
    Silou.computed_stats.protection[DAMAGE_TYPE_PHYSICAL]   = 60;
    Silou.computed_stats.protection[DAMAGE_TYPE_MAGICAL]    = 70;
    /* - Regret double digits - */

    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Digits_2.png"), renderer, sm->texture,
                            SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - Hit triple digits - */
    Silou.computed_stats.hit    = 100;
    Silou.computed_stats.dodge  = 100;
    /* - Crit triple digits - */
    Silou.computed_stats.crit   = 100;
    Silou.computed_stats.favor  = 100;
    /* - Regret triple digits - */
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Digits_3.png"), renderer, sm->texture,
                            SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - Attack double digits - */
    Silou.computed_stats.attack[DAMAGE_TYPE_TRUE] = 9;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Digits_4.png"), renderer, sm->texture,
                            SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- HANDEDNESS --- */
    /* -- Ambidextrous -- */
    Silou.handedness = UNIT_HAND_AMBIDEXTROUS;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Handedness_Ambidex.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Rightie -- */
    Silou.handedness = UNIT_HAND_RIGHTIE;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Handedness_Rightie.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Leftie -- */
    Silou.handedness = UNIT_HAND_LEFTIE;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Handedness_Leftie.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- MOUNT --- */
    /* - Salamander - */
    struct Mount mount = Mount_default_salamander;
    Silou.mount = &mount;
    SDL_assert(Silou.mount->type = MOUNT_TYPE_SALAMANDER);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Mount_Salamander.png"), renderer,
                            sm->texture,
                            SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - Horse - */
    mount = Mount_default_horse;
    SDL_assert(Silou.mount->type = MOUNT_TYPE_HORSE);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Mount_Horse.png"), renderer, sm->texture,
                            SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - Eagle - */
    mount = Mount_default_eagle;
    SDL_assert(Silou.mount->type = MOUNT_TYPE_EAGLE);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Mount_Eagle.png"), renderer, sm->texture,
                            SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - Pegasus - */
    mount = Mount_default_pegasus;
    SDL_assert(Silou.mount->type = MOUNT_TYPE_PEGASUS);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Mount_Pegasus.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Equipped in hands -- */
    /* -- Unequipped in hands -- */

    /* -- Weapon types -- */
    /* - sword + lance + shield + offhand - */
    Silou.equippable = ITEM_TYPE_SWORD + ITEM_TYPE_LANCE + ITEM_TYPE_SHIELD + ITEM_TYPE_OFFHAND;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Weapon_Duke.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - axe + bow - */
    Silou.equippable = ITEM_TYPE_AXE + ITEM_TYPE_BOW;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Weapon_AxBo.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - angelic + claw + Sword + Lance + shield - */
    Silou.equippable = ITEM_TYPE_SWORD + ITEM_TYPE_LANCE + ITEM_TYPE_SHIELD + ITEM_TYPE_CLAW +
                       ITEM_TYPE_ANGELIC;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Weapon_Angel.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - demonic  + staff + trinket - */
    Silou.equippable = ITEM_TYPE_STAFF + ITEM_TYPE_DEMONIC + ITEM_TYPE_TRINKET;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Weapon_DeStTr.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);
    /* - NONE - */
    Silou.equippable = ITEM_TYPE_NULL;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Weapon_None.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Rescuing -- */
    // TODO: RESCUE ICON
    Silou.rescuee = UNIT_ID_ERWIN;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Rescue.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Status -- */
    struct Unit_status status = {.turns = 1};
    /* - poisoned - */
    Unit_Status_Add(&Silou, status);
    Silou.status_queue[0].status = UNIT_STATUS_EXP_POISONED;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Status_Poisoned.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - stunned - */
    Silou.status_queue[0].status = UNIT_STATUS_EXP_STUNNED;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Status_Stunned.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - rooted - */
    Silou.status_queue[0].status = UNIT_STATUS_EXP_ROOTED;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Status_Rooted.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - seduced - */
    Silou.status_queue[0].status = UNIT_STATUS_EXP_SEDUCED;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Status_Seduced.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - berserk - */
    Silou.status_queue[0].status = UNIT_STATUS_EXP_BERSERK;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Status_Berserk.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - terror - */
    Silou.status_queue[0].status = UNIT_STATUS_EXP_TERROR;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Status_Terror.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - stone - */
    Silou.status_queue[0].status = UNIT_STATUS_EXP_STONE;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Status_Stone.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - slowed - */
    Silou.status_queue[0].status = UNIT_STATUS_EXP_SLOWED;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Status_Slowed.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - stasis - */
    Silou.status_queue[0].status = UNIT_STATUS_EXP_STASIS;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Status_Stasis.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* - silence - */
    Silou.status_queue[0].status = UNIT_STATUS_EXP_SILENCE;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Status_Silence.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- REGRETS -- */
    /* -- 0 -- */
    Silou.regrets = 0;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Regrets_0.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Single digits -- */
    Silou.regrets = 2;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Regrets_Digits_1.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Double digits -- */
    Silou.regrets = 25;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Regrets_Digits_2.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Triple digits -- */
    Silou.regrets = 137;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Regrets_Digits_3.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Skills -- */
    // TODO: Skill Icons

    /* -- Face -- */
    // TODO: Faces Icons

    /* -- Empty inventory -- */
    Silou.num_equipment = 0;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Equipment_0.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Full inventory -- */
    Silou.num_equipment = 4;
    in_wpn.id   = ITEM_ID_IRON_SWORD;
    in_wpn.used = 0;
    Weapon_Load(weapons_dtab, in_wpn.id);
    Unit_Item_Take(&Silou, in_wpn);
    SDL_assert(Silou.num_equipment == 5);

    in_wpn.id   = ITEM_ID_IRON_LANCE;
    in_wpn.used = 10;
    Weapon_Load(weapons_dtab, in_wpn.id);
    Unit_Item_Take(&Silou, in_wpn);
    SDL_assert(Silou.num_equipment == 6);

    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Equipment_Full.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Low Durability -- */
    Silou._equipment[0].used = 20;
    Silou._equipment[1].used = 21;
    Silou._equipment[2].used = 12;
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Durability_Digits_1.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* -- Long weapon names -- */
    Silou._equipment[0].used = 0;
    Silou._equipment[1].used = 0;
    Silou._equipment[2].used = 0;
    Silou._equipment[0].id   = ITEM_ID_RETRACTABLE_WRISTBLADE;
    Silou._equipment[1].id   = ITEM_ID_REPEATABLE_CROSSBOW;
    Silou._equipment[2].id   = ITEM_ID_HONJOU_MASAMUNE;
    Silou._equipment[3].id   = ITEM_ID_SILVERLIGHT_SPEAR;
    Weapon_Load(weapons_dtab, Silou._equipment[0].id);
    Weapon_Load(weapons_dtab, Silou._equipment[1].id);
    Weapon_Load(weapons_dtab, Silou._equipment[2].id);
    Weapon_Load(weapons_dtab, Silou._equipment[3].id);
    // TODO: when stats menu supports items
    // Silou._equipment[4].id   = ITEM_ID_DOUBLE_SIDED_WHETSTONE;
    // Item_Load(items_dtab, Silou._equipment[4].id);
    StatsMenu_Update(sm, &n9patch, render_target, renderer);
    Filesystem_Texture_Dump(PATH_JOIN("menu_stats", "StatsMenu_Names_Long.png"), renderer,
                            sm->texture, SDL_PIXELFORMAT_ARGB8888, render_target);

    /* --- FREE --- */
    PixelFont_SDL_free(sm->pixelnours, true);
    PixelFont_SDL_free(sm->pixelnours_big, true);

    Game_Weapons_SDL_free(&weapons_dtab);
    Game_Items_SDL_free(&items_dtab);
    SDL_FreeSurface(surface);
    StatsMenu_SDL_free(sm);

    if (n9patch.texture != NULL)
        SDL_DestroyTexture(n9patch.texture);

    SDL_DestroyRenderer(renderer);
    Unit_Free(&Silou);
}