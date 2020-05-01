#include "minctest.h"
#include "weapon.hpp"

void test_weapon() {
    Weapon temp_wpn;
    Weapon_stats temp_wpn_stats;
    // Pmight, Mmight, hit, dodge, crit, favor, wgt, uses, wpnlvl, range, hand, dmg_type, price
    temp_wpn_stats = {3, 0, 80, 0, 0, 0, 3, 30, 2, {1, 1}, {1, 2}, 0, 1000, 0};
    temp_wpn = Weapon(ITEM::TYPE::SWORD, temp_wpn_stats, ITEM::NAME::WOODEN_SWORD);
    temp_wpn.setDescription("Practice sword, made of wood. It's crushing blows are still deadly.");
    temp_wpn.writeXML("weapon_test.xml");

    temp_wpn = Weapon();
    temp_wpn.readXML("weapon_test.xml");
    temp_wpn.writeXML("weapon_rewrite.xml");

    // lok(fequal("test_results.txt", "test_results.txt"));
}
