#include "nourstest.h"
#include "item.h"
#include "jsonio.h"
#include "narrative.h"

void test_item() {
    SDL_Log("test_item");
    if (PHYSFS_stat("saves", NULL) == 0) {
        PHYSFS_mkdir("saves");
    }
    struct Item item1 = Item_default;
    struct Item item2 = Item_default;
    struct Item item3 = Item_default;
    //                               hp, str, mag, agi, dex, fth, luck, def, res, con, move
    struct Unit_stats in_stats = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    struct Unit_stats out_stats;
    uint64_t in_effect;
    bool in_canSell   = false;
    uint32_t in_type  = ITEM_TYPE_SWORD + ITEM_TYPE_LANCE;
    in_effect   = ITEM_EFFECT_KILL1P + ITEM_EFFECT_BRAVE2X + ITEM_EFFECT_BREAK_SHIELD;
    item1.type  = in_type;
    item1.id    = ITEM_ID_CROSS;
    char in_description[DEFAULT_BUFFER_SIZE] = {"Naturally full of angelic energy. Protects against demons."};
    char *out_description;
    strncpy(item1.description, in_description, strlen(in_description));
    strncpy(item1.name, "Cross", strlen("Cross"));
    item1.passive     = in_effect;
    item1.bonus_stats = in_stats;
    item1.malus_stats = in_stats;
    item1.canSell     = in_canSell;
    out_description   = item1.description;
    nourstest_true(strcmp(in_description, out_description) == 0);
    nourstest_true(strcmp(in_description,
                          "Naturally full of angelic energy. Protects against demons.") == 0);
    nourstest_true(strcmp(out_description,
                          "Naturally full of angelic energy. Protects against demons.") == 0);
    nourstest_true(strcmp(item1.name, "Cross") == 0);
    nourstest_true(item1.passive  == in_effect);
    nourstest_true(item1.type     == in_type);
    nourstest_true(item1.canSell  == in_canSell);
    out_stats = item1.bonus_stats;
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
    out_stats = item1.malus_stats;
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
    item1.write_stats = true;
    jsonio_writeJSON("saves/item_test.json", &item1, false);

    SDL_Log("Saved item_test");
    jsonio_readJSON("saves/item_test.json", &item2);
    out_description = item2.description;
    nourstest_true(strcmp(in_description, out_description) == 0);
    nourstest_true(strcmp(item2.name, "Cross") == 0);
    nourstest_true(item2.passive  == in_effect);
    nourstest_true(item2.type     == in_type);
    nourstest_true(item2.canSell  == in_canSell);
    out_stats = item2.bonus_stats;
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
    out_stats = item2.malus_stats;
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
    item2.write_stats = true;
    jsonio_writeJSON("saves/item_rewrite.json", &item2, false);
    nourstest_true(Filesystem_fequal("saves/item_test.json", "saves/item_rewrite.json"));
    nourstest_true(PHYSFS_exists("saves/item_rewrite.json"));
    nourstest_true(PHYSFS_exists("saves/item_test.json"));
    Item_Free(&item2);
}
