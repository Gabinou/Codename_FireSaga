
#include "nourstest.h"
#include "item.h"
#include "jsonio.h"
#include "scene.h"

void test_item() {
    // test_item
    if (PHYSFS_stat("saves", NULL) == 0) {
        PHYSFS_mkdir("saves");
    }
    struct Item item1 = Item_default;
    struct Item item2 = Item_default;
    struct Item item3 = Item_default;
    SDL_assert(item1.jsonio_header.json_filename.data == NULL);
    SDL_assert(item2.jsonio_header.json_filename.data == NULL);
    SDL_assert(item3.jsonio_header.json_filename.data == NULL);
    //                          hp,str,mag,agi,dex,fth,luck,def,res,con,mv
    struct Unit_stats in_stats = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    struct Unit_stats out_stats;
    u64 in_effect;
    b32 in_canSell   = false;
    uint32_t in_type  = ITEM_TYPE_SWORD + ITEM_TYPE_LANCE;
    in_effect   = ITEM_EFFECT_KILL1P + ITEM_EFFECT_BRAVE2X + ITEM_EFFECT_BREAK_SHIELD;
    item1.ids.id    = ITEM_ID_CROSS;
    char in_description[DEFAULT_BUFFER_SIZE] = {"Naturally full of angelic energy. Protects against demons."};
    char *out_description;
    memcpy(item1.description, in_description, strlen(in_description));
    item1.effect.passive           = in_effect;
    item1.aura.unit_stats   = in_stats;
    item1.flags.canSell     = in_canSell;
    out_description         = item1.description;
    nourstest_true(s8equal(s8_var(in_description), s8_var(out_description)));
    nourstest_true(s8equal(s8_var(in_description),
                           s8_literal("Naturally full of angelic energy. Protects against demons.")));
    nourstest_true(s8equal(s8_var(out_description),
                           s8_literal("Naturally full of angelic energy. Protects against demons.")));
    nourstest_true(item1.effect.passive     == in_effect);
    nourstest_true(item1.flags.canSell      == in_canSell);
    out_stats = item1.aura.unit_stats;
    nourstest_true(in_stats.hp    == out_stats.hp);
    nourstest_true(in_stats.str   == out_stats.str);
    nourstest_true(in_stats.mag   == out_stats.mag);
    nourstest_true(in_stats.agi   == out_stats.agi);
    nourstest_true(in_stats.dex   == out_stats.dex);
    nourstest_true(in_stats.luck  == out_stats.luck);
    nourstest_true(in_stats.def   == out_stats.def);
    nourstest_true(in_stats.res   == out_stats.res);
    nourstest_true(in_stats.con   == out_stats.con);
    nourstest_true(in_stats.move  == out_stats.move);
    nourstest_true(in_stats.prof  == out_stats.prof);
    jsonio_writeJSON(s8_literal(PATH_JOIN("saves", "item_test.json")), &item1, false);

    /* Saved item_test */
    SDL_assert(item2.jsonio_header.json_filename.data == NULL);
    s8_free(&item2.jsonio_header.json_filename);
    jsonio_readJSON(s8_literal(PATH_JOIN("saves", "item_test.json")), &item2);
    out_description = item2.description;
    nourstest_true(s8equal(s8_var(in_description), s8_var(out_description)));
    nourstest_true(item2.effect.passive == in_effect);
    nourstest_true(item2.flags.canSell  == in_canSell);
    nourstest_true(item2.cooldown.ticks == 0);
    out_stats = item2.aura.unit_stats;
    nourstest_true(in_stats.hp    == out_stats.hp);
    nourstest_true(in_stats.str   == out_stats.str);
    nourstest_true(in_stats.mag   == out_stats.mag);
    nourstest_true(in_stats.agi   == out_stats.agi);
    nourstest_true(in_stats.dex   == out_stats.dex);
    nourstest_true(in_stats.luck  == out_stats.luck);
    nourstest_true(in_stats.def   == out_stats.def);
    nourstest_true(in_stats.res   == out_stats.res);
    nourstest_true(in_stats.con   == out_stats.con);
    nourstest_true(in_stats.move  == out_stats.move);
    nourstest_true(in_stats.prof  == out_stats.prof);
    jsonio_writeJSON(s8_literal(PATH_JOIN("saves", "item_rewrite.json")), &item2, false);
    nourstest_true(Filesystem_fequal(PATH_JOIN("saves", "item_test.json"), PATH_JOIN("saves",
                                     "item_rewrite.json")));
    nourstest_true(PHYSFS_exists(PATH_JOIN("saves", "item_rewrite.json")));
    nourstest_true(PHYSFS_exists(PATH_JOIN("saves", "item_test.json")));
    Item_Free(&item1);
    Item_Free(&item2);
}
