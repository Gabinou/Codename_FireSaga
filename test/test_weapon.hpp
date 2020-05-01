#include "minctest.h"
#include "weapon.hpp"

void test_weapon() {
    Weapon wpn1, wpn2;
    Weapon_stats in_wpn_stats, out_wpn_stats;
    long unsigned int in_effect;
    short unsigned int in_type = ITEM::TYPE::SWORD + ITEM::TYPE::LANCE;
    in_effect = ITEM::EFFECT::KILL1P + ITEM::EFFECT::BRAVE2X + ITEM::EFFECT::BREAK_SHIELD;
    // Pmight, Mmight, hit, dodge, crit, favor, wgt, uses, wpnlvl, range, hand, dmg_type, price
    in_wpn_stats = {3, 0, 80, 0, 0, 0, 3, 30, 2, {1, 1}, {1, 2}, 0, 1000, 0};
    wpn1 = Weapon(in_type, in_wpn_stats, ITEM::NAME::WOODEN_SWORD);
    std::string in_description = "Practice sword, made of wood. It's crushing blows are still deadly.";
    std::string out_description;
    wpn1.setDescription(in_description);
    wpn1.setEffect(in_effect);
    
    out_wpn_stats = wpn1.getStats();                              
    lok(in_wpn_stats.Pmight == out_wpn_stats.Pmight);
    lok(in_wpn_stats.Mmight == out_wpn_stats.Mmight);
    lok(in_wpn_stats.combat.hit == out_wpn_stats.combat.hit);
    lok(in_wpn_stats.combat.crit == out_wpn_stats.combat.crit);
    lok(in_wpn_stats.combat.dodge == out_wpn_stats.combat.dodge);
    lok(in_wpn_stats.combat.favor == out_wpn_stats.combat.favor);
    lok(in_wpn_stats.wgt == out_wpn_stats.wgt);
    lok(in_wpn_stats.uses == out_wpn_stats.uses);
    lok(in_wpn_stats.prof == out_wpn_stats.prof);
    lok(in_wpn_stats.range[0] == out_wpn_stats.range[0]);
    lok(in_wpn_stats.range[1] == out_wpn_stats.range[1]);
    lok(in_wpn_stats.hand[0] == out_wpn_stats.hand[0]);
    lok(in_wpn_stats.hand[1] == out_wpn_stats.hand[1]);
    lok(in_wpn_stats.dmg_type == out_wpn_stats.dmg_type);
    lok(in_wpn_stats.price == out_wpn_stats.price);
    lok(in_wpn_stats.heal == out_wpn_stats.heal);

    out_description = wpn1.getDescription();
    lok(in_description == out_description);
    lok(wpn1.getName() == "Wooden sword");
    lok(wpn1.getEffect() == in_effect);
    lok(wpn1.getType() == in_type);

    wpn1.writeXML("weapon_test.xml");

    wpn2 = Weapon();
    wpn2.readXML("weapon_test.xml");

    out_wpn_stats = wpn2.getStats();
    lok(in_wpn_stats.Pmight == out_wpn_stats.Pmight);
    lok(in_wpn_stats.Mmight == out_wpn_stats.Mmight);
    lok(in_wpn_stats.combat.hit == out_wpn_stats.combat.hit);
    lok(in_wpn_stats.combat.crit == out_wpn_stats.combat.crit);
    lok(in_wpn_stats.combat.dodge == out_wpn_stats.combat.dodge);
    lok(in_wpn_stats.combat.favor == out_wpn_stats.combat.favor);
    lok(in_wpn_stats.wgt == out_wpn_stats.wgt);
    lok(in_wpn_stats.uses == out_wpn_stats.uses);
    lok(in_wpn_stats.prof == out_wpn_stats.prof);
    lok(in_wpn_stats.range[0] == out_wpn_stats.range[0]);
    lok(in_wpn_stats.range[1] == out_wpn_stats.range[1]);
    lok(in_wpn_stats.hand[0] == out_wpn_stats.hand[0]);
    lok(in_wpn_stats.hand[1] == out_wpn_stats.hand[1]);
    lok(in_wpn_stats.dmg_type == out_wpn_stats.dmg_type);
    lok(in_wpn_stats.price == out_wpn_stats.price);
    lok(in_wpn_stats.heal == out_wpn_stats.heal);

    out_description = wpn2.getDescription();
    lok(in_description == out_description);
    lok(wpn2.getName() == "Wooden sword");
    lok(wpn2.getEffect() == in_effect);
    lok(wpn2.getType() == in_type);

    wpn2.writeXML("weapon_rewrite.xml");
    lok(fequal("weapon_test.xml", "weapon_rewrite.xml"));
}
