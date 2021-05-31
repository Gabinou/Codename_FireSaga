#include "minctest.h"
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
    struct Unit_stats in_stats = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    struct Unit_stats out_stats;
    uint64_t in_effect;
    bool in_sellable = false;
    uint32_t in_type = ITEM_TYPE_SWORD + ITEM_TYPE_LANCE;
    in_effect = ITEM_EFFECT_KILL1P + ITEM_EFFECT_BRAVE2X + ITEM_EFFECT_BREAK_SHIELD;
    item1.type = in_type;
    item1.id = ITEM_NAME_CROSS;
    char in_description[DEFAULT_BUFFER_SIZE] = {"Naturally full of angelic energy. Protects against demons."};
    char * out_description;
    item1.description = in_description;
    item1.name = "Cross";
    item1.passive_effect = in_effect;
    item1.bonus_stats = in_stats;
    item1.malus_stats = in_stats;
    item1.sellable = in_sellable;
    out_description = item1.description;
    lok(strcmp(in_description, out_description) == 0);
    lok(strcmp(in_description, "Naturally full of angelic energy. Protects against demons.") == 0);
    lok(strcmp(out_description, "Naturally full of angelic energy. Protects against demons.") == 0);
    lok(strcmp(item1.name, "Cross") == 0);
    lok(item1.passive_effect == in_effect);
    lok(item1.type == in_type);
    lok(item1.sellable == in_sellable);
    out_stats = item1.bonus_stats;
    lok(in_stats.hp == out_stats.hp);
    lok(in_stats.str == out_stats.str);
    lok(in_stats.mag == out_stats.mag);
    lok(in_stats.agi == out_stats.agi);
    lok(in_stats.dex == out_stats.dex);
    lok(in_stats.luck == out_stats.luck);
    lok(in_stats.def == out_stats.def);
    lok(in_stats.res == out_stats.res);
    lok(in_stats.con == out_stats.con);
    lok(in_stats.move == out_stats.move);
    lok(in_stats.prof == out_stats.prof);
    out_stats = item1.malus_stats;
    lok(in_stats.hp == out_stats.hp);
    lok(in_stats.str == out_stats.str);
    lok(in_stats.mag == out_stats.mag);
    lok(in_stats.agi == out_stats.agi);
    lok(in_stats.dex == out_stats.dex);
    lok(in_stats.luck == out_stats.luck);
    lok(in_stats.def == out_stats.def);
    lok(in_stats.res == out_stats.res);
    lok(in_stats.con == out_stats.con);
    lok(in_stats.move == out_stats.move);
    lok(in_stats.prof == out_stats.prof);

    writeJSON("saves//item_test.json", &item1, false);
    SDL_Log("Saved item_test");
    readJSON("saves//item_test.json", &item2);
    out_description = item2.description;
    lok(strcmp(in_description, out_description) == 0);
    SDL_Log("CURRENT ITEM %s", item2.name);
    lok(strcmp(item2.name, "Cross") == 0);
    lok(item2.passive_effect == in_effect);
    lok(item2.type == in_type);
    lok(item2.sellable == in_sellable);
    out_stats = item2.bonus_stats;
    lok(in_stats.hp == out_stats.hp);
    lok(in_stats.str == out_stats.str);
    lok(in_stats.mag == out_stats.mag);
    lok(in_stats.agi == out_stats.agi);
    lok(in_stats.dex == out_stats.dex);
    lok(in_stats.luck == out_stats.luck);
    lok(in_stats.def == out_stats.def);
    lok(in_stats.res == out_stats.res);
    lok(in_stats.con == out_stats.con);
    lok(in_stats.move == out_stats.move);
    lok(in_stats.prof == out_stats.prof);
    out_stats = item2.malus_stats;
    lok(in_stats.hp == out_stats.hp);
    lok(in_stats.str == out_stats.str);
    lok(in_stats.mag == out_stats.mag);
    lok(in_stats.agi == out_stats.agi);
    lok(in_stats.dex == out_stats.dex);
    lok(in_stats.luck == out_stats.luck);
    lok(in_stats.def == out_stats.def);
    lok(in_stats.res == out_stats.res);
    lok(in_stats.con == out_stats.con);
    lok(in_stats.move == out_stats.move);
    lok(in_stats.prof == out_stats.prof);
    writeJSON("saves//item_rewrite.json", &item2, false);
    lok(fequal("saves//item_test.json", "saves//item_rewrite.json"));
    lok(PHYSFS_exists("saves//item_rewrite.json"));
    lok(PHYSFS_exists("saves//item_test.json"));
}
