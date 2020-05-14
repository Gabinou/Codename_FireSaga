#include "minctest.h"
#include "unit.hpp"

void test_unit() {
    Unit unit1, unit2, unit3, unit4, unit5;
    Unit_stats in_stats, out_stats;
    Unit_stats in_caps, out_caps;
    Unit_stats in_growths, out_growths;
    Inventory_item in_stats_wpn, out_stats_wpn;

    in_stats = {17,  6,  2,  7,  7,   7,  4,  5,  6, 5};
    unit1 = Unit(UNIT::NAME::SILOU, UNIT::CLASS::MERCENARY, in_stats, UNIT::SEX::M);
    in_caps = {48, 14, 25, 32, 34,  28, 19, 40, 15, 0};
    unit1.setCaps(in_stats);
    in_growths = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    unit1.setGrowths(in_growths);
    unit1.setBaseExp(0);
    in_stats_wpn.id = ITEM::NAME::FLEURET;
    unit1.addEquipment(in_stats_wpn);
    in_stats_wpn.id = ITEM::NAME::KITCHEN_KNIFE;
    unit1.addEquipment(in_stats_wpn);
    in_stats_wpn.id = ITEM::NAME::POT_LID;
    unit1.addEquipment(in_stats_wpn);
    out_stats = unit1.getStats();
    lok(unit1.getName() == "Erwin");
    lok(unit1.getSex() == 1);
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
    out_growths = unit1.getGrowths();
    lok(in_growths.hp == out_growths.hp);
    lok(in_growths.str == out_growths.str);
    lok(in_growths.mag == out_growths.mag);
    lok(in_growths.agi == out_growths.agi);
    lok(in_growths.dex == out_growths.dex);
    lok(in_growths.luck == out_growths.luck);
    lok(in_growths.def == out_growths.def);
    lok(in_growths.res == out_growths.res);
    lok(in_growths.con == out_growths.con);
    lok(in_growths.move == out_growths.move);
    lok(in_growths.prof == out_growths.prof);

    unit1.levelUp();
    unit1.levelUp();
    out_stats = unit1.getBases();
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
    out_stats = unit1.getStats();
    lok(in_stats.hp >= out_stats.hp);
    lok(in_stats.str >= out_stats.str);
    lok(in_stats.mag >= out_stats.mag);
    lok(in_stats.agi >= out_stats.agi);
    lok(in_stats.dex >= out_stats.dex);
    lok(in_stats.luck >= out_stats.luck);
    lok(in_stats.def >= out_stats.def);
    lok(in_stats.res >= out_stats.res);
    lok(in_stats.con >= out_stats.con);
    lok(in_stats.move >= out_stats.move);
    lok(in_stats.prof >= out_stats.prof);

    unit1.writeXML("unit_test.xml");
    unit1.writeXML("unit_test.binou");
    unit2 = Unit();
    unit2.readXML("unit_test.xml");
    unit2.writeXML("unit_rewrite.xml");
    unit3 = Unit();
    unit3.readXML("unit_test.binou");
    unit3.writeXML("unit_rewrite.binou");

    lok(fequal("unit_test.xml", "unit_test.binou"));
    lok(fequal("unit_test.xml", "unit_rewrite.xml"));
    lok(fequal("unit_test.binou", "unit_rewrite.binou"));
    SDL_Log("FROM HERE");
    unit1.writeJSON("unit_test.json");
    unit1.writeJSON("unit_testj.binou");
    // unit4 = Unit();
    // unit4.readJSON("unit_test.json");
    // unit4.writeJSON("unit_rewrite.json");
    // unit5 = Unit();
    // unit5.readJSON("unit_testj.binou");
    // unit5.writeJSON("unit_rewritej.binou");

    // lok(fequal("unit_test.json", "unit_testj.binou"));
    // lok(fequal("unit_test.json", "unit_rewrite.json"));
    // lok(fequal("unit_testj.binou", "unit_rewritej.binou"));

}
