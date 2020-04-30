#include "minctest.h"
#include "unit.hpp"

void test_unit() {
    Unit unit1, unit2, unit3;
    Unit_stats in_stats, out_stats;
    Inventory_item in_stats_wpn, out_stats_wpn;

    in_stats = {17,  6,  2,  7,  7,   7,  4,  5,  6, 5};
    unit1 = Unit(UNIT::NAME::ERWIN, UNIT::CLASS::MERCENARY, in_stats, UNIT::SEX::M);
    in_stats = {48, 14, 25, 32, 34,  28, 19, 40, 15, 0};
    unit1.setCaps(in_stats);
    in_stats = {60, 50, 20, 60, 70,  40, 30, 20,  10, 0};
    unit1.setGrowths(in_stats);
    unit1.setBaseExp(0);
    in_stats_wpn.id = ITEM::NAME::FLEURET;
    unit1.addEquipment(in_stats_wpn);
    in_stats_wpn.id = ITEM::NAME::KITCHEN_KNIFE;
    unit1.addEquipment(in_stats_wpn);
    in_stats_wpn.id = ITEM::NAME::POT_LID;
    unit1.addEquipment(in_stats_wpn);





    
    unit1.levelUp();
    unit1.levelUp();
    unit1.writeXML("unit_test.xml");
    unit1.writeXML("unit_test.binou");
    unit2 = Unit();
    unit2.readXML("unit_test.xml");
    unit2.writeXML("unit_rewrite.xml");
    unit3 = Unit();
    unit3.readXML("unit_test.xml;");
    unit3.writeXML("unit_rewrite.binou");

    lok(fequal("unit_test.xml", "unit_rewrite.xml"));
    lok(fequal("unit_test.xml", "unit_test.binou"));

}
