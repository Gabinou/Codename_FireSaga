#include "minctest.h"
#include "item.hpp"
#include "structs.hpp"

void test_item() {
    SDL_Log("test_item");

    if (PHYSFS_stat("saves", nullptr) == 0) {
        PHYSFS_mkdir("saves");
    }

    Item item1, item2, item3;
    Unit_stats in_stats = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    Unit_stats out_stats;
    long unsigned int in_effect;
    bool in_sellable = false;
    short unsigned int in_type = ITEM::TYPE::SWORD + ITEM::TYPE::LANCE;
    in_effect = ITEM::EFFECT::KILL1P + ITEM::EFFECT::BRAVE2X + ITEM::EFFECT::BREAK_SHIELD;
    item1 = Item(in_type, ITEM::NAME::CROSS);
    std::string in_description = "Naturally full of angelic energy. Protects against demons.";
    std::string out_description;
    item1.setDescription(in_description);
    item1.setEffect(in_effect);
    item1.setBonus(in_stats);
    item1.setMalus(in_stats);
    item1.setSellable(in_sellable);

    out_description = item1.getDescription();
    lok(in_description == out_description);
    lok(item1.getName() == "Cross");
    lok(item1.getEffect() == in_effect);
    lok(item1.getType() == in_type);
    lok(item1.isSellable() == in_sellable);
    out_stats = item1.getBonus();
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
    out_stats = item1.getMalus();
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

    // item1.writeXML("item_test.xml");
    item1.writeJSON("saves//item_test.json");

    // item2 = item();
    // item2.readXML("saves//item_test.xml");
    item2 = Item();
    item2.readJSON("saves//item_test.json");

    out_description = item2.getDescription();
    lok(in_description == out_description);
    lok(item2.getName() == "Cross");
    lok(item2.getEffect() == in_effect);
    lok(item2.getType() == in_type);
    lok(item2.isSellable() == in_sellable);
    out_stats = item2.getBonus();
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
    out_stats = item2.getMalus();
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

    item2.writeJSON("saves//item_rewrite.json");
    lok(fequal("saves//item_test.json", "saves//item_rewrite.json"));
}
