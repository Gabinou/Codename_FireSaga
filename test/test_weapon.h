
#include "nourstest.h"
#include "weapon.h"
#include "platform.h"
#include "scene.h"

void test_weapon1() {
    struct Weapon wpn1  = Weapon_default;
    struct Weapon wpn2  = Weapon_default;
    struct Weapon wpn3  = Weapon_default;
    struct Item item1   = Item_default;
    struct Item item2   = Item_default;
    struct Item item3   = Item_default;
    wpn1.item   = SDL_malloc(sizeof(struct Item));
    wpn2.item   = SDL_malloc(sizeof(struct Item));
    // wpn3.item   = SDL_malloc(sizeof(struct Item));
    *wpn1.item  = Item_default;
    *wpn2.item  = Item_default;
    // *wpn3.item  = Item_default;
    struct Weapon_stats in_wpn_stats = {
        .attack.physical   = 3,
        .attack.magical    = 0,
        .hit    = 80,
        .crit   =  0,
        .dodge  =  0,
        .favor  =  0,
        .wgt    =  3,
        .range  = {1, 2},
    };
    struct Weapon_stats out_wpn_stats;
    struct Unit_stats in_stats = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    struct Unit_stats out_stats;
    u64 in_effect;
    b32 in_canSell = false;
    u16 in_type = ITEM_TYPE_SWORD + ITEM_TYPE_LANCE;

    in_effect = ITEM_EFFECT_KILL1P + ITEM_EFFECT_BRAVE2X + ITEM_EFFECT_BREAK_SHIELD;
    wpn1.item->type = in_type;
    wpn1.stats = in_wpn_stats;
    wpn1.item->id = ITEM_ID_WOODEN_SWORD;
    wpn1.item->name = s8_mut("Wooden Sword");
    char *in_description = "Practice sword, made of wood. It's crushing blows are still deadly.";
    char *out_description;
    memcpy(wpn1.item->description, in_description, strlen(in_description));
    wpn1.item->passive          = in_effect;
    wpn1.item->aura.unit_stats  = in_stats;
    wpn1.item->canSell          = in_canSell;
    out_wpn_stats               = wpn1.stats;
    nourstest_true(in_wpn_stats.attack.physical ==
                   out_wpn_stats.attack.physical);
    nourstest_true(in_wpn_stats.attack.magical ==
                   out_wpn_stats.attack.magical);
    nourstest_true(in_wpn_stats.hit         == out_wpn_stats.hit);
    nourstest_true(in_wpn_stats.crit        == out_wpn_stats.crit);
    nourstest_true(in_wpn_stats.dodge       == out_wpn_stats.dodge);
    nourstest_true(in_wpn_stats.favor       == out_wpn_stats.favor);
    nourstest_true(in_wpn_stats.wgt         == out_wpn_stats.wgt);
    nourstest_true(in_wpn_stats.prof        == out_wpn_stats.prof);
    nourstest_true(in_wpn_stats.range.min   == out_wpn_stats.range.min);
    nourstest_true(in_wpn_stats.range.max   == out_wpn_stats.range.max);
    out_description = wpn1.item->description;
    nourstest_true(s8equal(s8_var(in_description), s8_var(out_description)));
    nourstest_true(s8equal(wpn1.item->name, s8_literal("Wooden Sword")));
    nourstest_true(wpn1.item->passive   == in_effect);
    nourstest_true(wpn1.item->type      == in_type);
    nourstest_true(wpn1.item->canSell   == in_canSell);
    out_stats = wpn1.item->aura.unit_stats;
    nourstest_true(in_stats.hp      == out_stats.hp);
    nourstest_true(in_stats.str     == out_stats.str);
    nourstest_true(in_stats.mag     == out_stats.mag);
    nourstest_true(in_stats.agi     == out_stats.agi);
    nourstest_true(in_stats.dex     == out_stats.dex);
    nourstest_true(in_stats.luck    == out_stats.luck);
    nourstest_true(in_stats.def     == out_stats.def);
    nourstest_true(in_stats.res     == out_stats.res);
    nourstest_true(in_stats.con     == out_stats.con);
    nourstest_true(in_stats.move    == out_stats.move);
    nourstest_true(in_stats.prof    == out_stats.prof);
    if (PHYSFS_stat("saves", NULL) == 0) {
        PHYSFS_mkdir("saves");
    }

    PHYSFS_mount("saves", NULL, 1);

    jsonio_writeJSON(s8_literal(PATH_JOIN("saves", "weapon_test.json")), &wpn1, false);
    jsonio_readJSON(s8_literal(PATH_JOIN("saves", "weapon_test.json")), &wpn3);
    out_wpn_stats = wpn3.stats;
    nourstest_true(in_wpn_stats.attack.physical ==
                   out_wpn_stats.attack.physical);
    nourstest_true(in_wpn_stats.attack.magical ==
                   out_wpn_stats.attack.magical);
    nourstest_true(in_wpn_stats.hit       == out_wpn_stats.hit);
    nourstest_true(in_wpn_stats.crit      == out_wpn_stats.crit);
    nourstest_true(in_wpn_stats.dodge     == out_wpn_stats.dodge);
    nourstest_true(in_wpn_stats.favor     == out_wpn_stats.favor);
    nourstest_true(in_wpn_stats.wgt       == out_wpn_stats.wgt);
    nourstest_true(in_wpn_stats.prof      == out_wpn_stats.prof);
    nourstest_true(in_wpn_stats.range.min == out_wpn_stats.range.min);
    nourstest_true(in_wpn_stats.range.max == out_wpn_stats.range.max);
    out_description = wpn3.item->description;
    nourstest_true(s8equal(s8_var(in_description), s8_var(out_description)));
    nourstest_true(s8equal(wpn3.item->name, s8_literal("Wooden Sword")));
    nourstest_true(wpn3.item->passive   == in_effect);
    nourstest_true(wpn3.item->type      == in_type);
    nourstest_true(wpn3.item->canSell   == in_canSell);
    out_stats = wpn3.item->aura.unit_stats;
    nourstest_true(in_stats.hp      == out_stats.hp);
    nourstest_true(in_stats.str     == out_stats.str);
    nourstest_true(in_stats.mag     == out_stats.mag);
    nourstest_true(in_stats.agi     == out_stats.agi);
    nourstest_true(in_stats.dex     == out_stats.dex);
    nourstest_true(in_stats.luck    == out_stats.luck);
    nourstest_true(in_stats.def     == out_stats.def);
    nourstest_true(in_stats.res     == out_stats.res);
    nourstest_true(in_stats.con     == out_stats.con);
    nourstest_true(in_stats.move    == out_stats.move);
    nourstest_true(in_stats.prof    == out_stats.prof);
    jsonio_writeJSON(s8_literal(PATH_JOIN("saves", "weapon_rewrite.json")), &wpn3, false);
    nourstest_true(Filesystem_fequal(PATH_JOIN("saves", "weapon_test.json"),
                                     PATH_JOIN("saves", "weapon_rewrite.json")));
    out_wpn_stats = wpn3.stats;
    nourstest_true(in_wpn_stats.attack.physical ==
                   out_wpn_stats.attack.physical);
    nourstest_true(in_wpn_stats.attack.magical ==
                   out_wpn_stats.attack.magical);
    nourstest_true(in_wpn_stats.hit         == out_wpn_stats.hit);
    nourstest_true(in_wpn_stats.crit        == out_wpn_stats.crit);
    nourstest_true(in_wpn_stats.dodge       == out_wpn_stats.dodge);
    nourstest_true(in_wpn_stats.favor       == out_wpn_stats.favor);
    nourstest_true(in_wpn_stats.wgt         == out_wpn_stats.wgt);
    nourstest_true(in_wpn_stats.prof        == out_wpn_stats.prof);
    nourstest_true(in_wpn_stats.range.min   == out_wpn_stats.range.min);
    nourstest_true(in_wpn_stats.range.max   == out_wpn_stats.range.max);
    out_description = wpn3.item->description;
    nourstest_true(s8equal(s8_var(in_description), s8_var(out_description)));
    nourstest_true(s8equal(wpn3.item->name, s8_literal("Wooden Sword")));
    nourstest_true(wpn3.item->passive    == in_effect);
    nourstest_true(wpn3.item->type       == in_type);
    nourstest_true(wpn3.item->canSell    == in_canSell);
    out_stats = wpn3.item->aura.unit_stats;
    nourstest_true(in_stats.hp      == out_stats.hp);
    nourstest_true(in_stats.str     == out_stats.str);
    nourstest_true(in_stats.mag     == out_stats.mag);
    nourstest_true(in_stats.agi     == out_stats.agi);
    nourstest_true(in_stats.dex     == out_stats.dex);
    nourstest_true(in_stats.luck    == out_stats.luck);
    nourstest_true(in_stats.def     == out_stats.def);
    nourstest_true(in_stats.res     == out_stats.res);
    nourstest_true(in_stats.con     == out_stats.con);
    nourstest_true(in_stats.move    == out_stats.move);
    nourstest_true(in_stats.prof    == out_stats.prof);
    jsonio_writeJSON(s8_literal(PATH_JOIN("saves", "weapon_rewrite.json")), &wpn3, false);
    nourstest_true(Filesystem_fequal(PATH_JOIN("saves", "weapon_test.json"),
                                     PATH_JOIN("saves", "weapon_rewrite.json")));

    Weapon_Free(&wpn1);
    SDL_free(wpn1.item);
    Weapon_Free(&wpn2);
    SDL_free(wpn2.item);
    Weapon_Free(&wpn3);
    SDL_free(wpn3.item);
}

void test_weapon_stats() {
    struct Weapon_stats wpn_stats_struct = {
        .attack.physical = 3,
        .hit    = 80,
        .crit   =  0,
        .dodge  = -4,
        .favor  =  0,
        .wgt    =  3,
        .range  = {2, 1},
    };
    nourstest_true(wpn_stats_struct.dodge == -4);
    i32 *wpn_stats_arr = (i32 *)&wpn_stats_struct;
    nourstest_true(wpn_stats_arr[WEAPON_STAT_HIT - WEAPON_STAT_START - 1] == 80);
    nourstest_true(wpn_stats_arr[WEAPON_STAT_DODGE - WEAPON_STAT_START - 1] == -4);
}

void test_weapon() {
    test_weapon1();
    test_weapon_stats();
}
