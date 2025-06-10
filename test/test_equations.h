
#include "nourstest.h"
#include <stdint.h>
#include "equations.h"
#include "utilities.h"

void test_Equation_Unit_Hit() {
    u8 wpn_hit = 0;
    u8 dex     = 0;
    u8 luck    = 0;
    u8 support = 0;
    nourstest_true(Equation_Unit_Hit(wpn_hit, dex, luck, support) == 0);
    wpn_hit = 10;
    nourstest_true(Equation_Unit_Hit(wpn_hit, dex, luck, support) == wpn_hit);
    luck    = 10;
    nourstest_true(Equation_Unit_Hit(wpn_hit, dex, luck,
                                     support) == (wpn_hit + luck / HIT_LUCK_FACTOR));
    dex     = 10;
    nourstest_true(Equation_Unit_Hit(wpn_hit, dex, luck,
                                     support) == (wpn_hit + luck / HIT_LUCK_FACTOR + HIT_DEX_FACTOR * dex));
    wpn_hit = 80;
    luck    = 1;
    dex     = 10;
    support = 1;
    nourstest_true(Equation_Unit_Hit(wpn_hit, dex, luck,
                                     support) == (wpn_hit + luck / HIT_LUCK_FACTOR + HIT_DEX_FACTOR * dex + support));
    dex     = SOTA_MAX_DAMAGE;
    nourstest_true(Equation_Unit_Hit(wpn_hit, dex, luck, support) == SOTA_MAX_DAMAGE);
    wpn_hit = SOTA_MAX_DAMAGE;
    dex     = SOTA_MAX_DAMAGE;
    luck    = SOTA_MAX_DAMAGE;
    support = SOTA_MAX_DAMAGE;
    nourstest_true(Equation_Unit_Hit(wpn_hit, dex, luck, support) == SOTA_MAX_DAMAGE);

}

void test_Equation_Unit_Dodge() {
    u8 wpn_dodge   = 0;
    u8 wpn_wgt     = 0;
    u8 tile_dodge  = 0;
    u8 str         = 0;
    u8 agi         = 0;
    u8 fth         = 0;
    u8 con         = 0;
    u8 luck        = 0;
    u8 support     = 0;

    nourstest_true(Equation_Unit_Dodge(wpn_wgt, wpn_dodge, luck, fth, agi, str, con, tile_dodge,
                                       support) == (wpn_dodge + support + tile_dodge + luck / DODGE_LUCK_FACTOR + fth / DODGE_FTH_FACTOR +
                                                    agi / DODGE_AGI_FACTOR - con / DODGE_CON_FACTOR - (wpn_wgt / DODGE_WPN_WGT_FACTOR) + str /
                                                    DODGE_STR_FACTOR));
    wpn_wgt = 10;

    nourstest_true(Equation_Unit_Dodge(wpn_wgt, wpn_dodge, luck, fth, agi, str, con, tile_dodge,
                                       support) == (wpn_dodge + support + tile_dodge + luck / DODGE_LUCK_FACTOR + fth / DODGE_FTH_FACTOR +
                                                    agi / DODGE_AGI_FACTOR - con / DODGE_CON_FACTOR - wpn_wgt / DODGE_WPN_WGT_FACTOR) + str /
                   DODGE_STR_FACTOR);
    str = 4;
    nourstest_true(Equation_Unit_Dodge(wpn_wgt, wpn_dodge, luck, fth, agi, str, con, tile_dodge,
                                       support) == (wpn_dodge + support + tile_dodge + luck / DODGE_LUCK_FACTOR + fth / DODGE_FTH_FACTOR +
                                                    agi / DODGE_AGI_FACTOR - con / DODGE_CON_FACTOR - wpn_wgt / DODGE_WPN_WGT_FACTOR) + str /
                   DODGE_STR_FACTOR);
    con = 2;
    nourstest_true(Equation_Unit_Dodge(wpn_wgt, wpn_dodge, luck, fth, agi, str, con, tile_dodge,
                                       support) == (wpn_dodge + support + tile_dodge + luck / DODGE_LUCK_FACTOR + fth / DODGE_FTH_FACTOR +
                                                    agi / DODGE_AGI_FACTOR - con / DODGE_CON_FACTOR - wpn_wgt / DODGE_WPN_WGT_FACTOR) + str /
                   DODGE_STR_FACTOR);
    luck = 10;
    nourstest_true(Equation_Unit_Dodge(wpn_wgt, wpn_dodge, luck, fth, agi, str, con, tile_dodge,
                                       support) == (wpn_dodge + support + tile_dodge + luck / DODGE_LUCK_FACTOR + fth / DODGE_FTH_FACTOR +
                                                    agi / DODGE_AGI_FACTOR - con / DODGE_CON_FACTOR - wpn_wgt / DODGE_WPN_WGT_FACTOR) + str /
                   DODGE_STR_FACTOR);
    agi = 4;
    nourstest_true(Equation_Unit_Dodge(wpn_wgt, wpn_dodge, luck, fth, agi, str, con, tile_dodge,
                                       support) == (wpn_dodge + support + tile_dodge + luck / DODGE_LUCK_FACTOR + fth / DODGE_FTH_FACTOR +
                                                    agi / DODGE_AGI_FACTOR - con / DODGE_CON_FACTOR - wpn_wgt / DODGE_WPN_WGT_FACTOR) + str /
                   DODGE_STR_FACTOR);
    tile_dodge = 3;
    nourstest_true(Equation_Unit_Dodge(wpn_wgt, wpn_dodge, luck, fth, agi, str, con, tile_dodge,
                                       support) == (wpn_dodge + support + tile_dodge + luck / DODGE_LUCK_FACTOR + fth / DODGE_FTH_FACTOR +
                                                    agi / DODGE_AGI_FACTOR - con / DODGE_CON_FACTOR - wpn_wgt / DODGE_WPN_WGT_FACTOR) + str /
                   DODGE_STR_FACTOR);
    support = 7;
    nourstest_true(Equation_Unit_Dodge(wpn_wgt, wpn_dodge, luck, fth, agi, str, con, tile_dodge,
                                       support) == (wpn_dodge + support + tile_dodge + luck / DODGE_LUCK_FACTOR + fth / DODGE_FTH_FACTOR +
                                                    agi / DODGE_AGI_FACTOR - con / DODGE_CON_FACTOR - wpn_wgt / DODGE_WPN_WGT_FACTOR) + str /
                   DODGE_STR_FACTOR);
    wpn_dodge = 5;
    nourstest_true(Equation_Unit_Dodge(wpn_wgt, wpn_dodge, luck, fth, agi, str, con, tile_dodge,
                                       support) == (wpn_dodge + support + tile_dodge + luck / DODGE_LUCK_FACTOR + fth / DODGE_FTH_FACTOR +
                                                    agi / DODGE_AGI_FACTOR - con / DODGE_CON_FACTOR - wpn_wgt / DODGE_WPN_WGT_FACTOR) + str /
                   DODGE_STR_FACTOR);
    wpn_wgt = INT8_MAX;
    // i32 slowed = NMATH_MAX(0, wpn_wgt - str / SPEED_STR_FACTOR);
    nourstest_true(Equation_Unit_Dodge(wpn_wgt, wpn_dodge, luck, fth, agi, str, con, tile_dodge,
                                       support) == SOTA_MIN_DODGE);
    wpn_dodge = INT8_MAX;
    nourstest_true(Equation_Unit_Dodge(wpn_wgt, wpn_dodge, luck, fth, agi, str, con, tile_dodge,
                                       support) == (wpn_dodge + support + tile_dodge +
                                                    luck / DODGE_LUCK_FACTOR + fth / DODGE_FTH_FACTOR +
                                                    agi / DODGE_AGI_FACTOR - con / DODGE_CON_FACTOR -
                                                    wpn_wgt / DODGE_WPN_WGT_FACTOR) + str / DODGE_STR_FACTOR);
}

void test_Equation_Unit_Favor() {
    u8 wpn_favor = 0;
    u8 luck = 0;
    u8 support = 0;
    nourstest_true(Equation_Unit_Favor(wpn_favor, luck, support) == (wpn_favor + luck / 2 + support));
    support = 10;
    nourstest_true(Equation_Unit_Favor(wpn_favor, luck, support) == (wpn_favor + luck / 2 + support));
    luck = 10;
    support = 0;
    nourstest_true(Equation_Unit_Favor(wpn_favor, luck, support) == (wpn_favor + luck / 2 + support));
    wpn_favor = 10;
    nourstest_true(Equation_Unit_Favor(wpn_favor, luck, support) == (wpn_favor + luck / 2 + support));

    wpn_favor = SOTA_MAX_DAMAGE;
    nourstest_true(Equation_Unit_Favor(wpn_favor, luck, support) == SOTA_MAX_DAMAGE);
    wpn_favor = 0;
    support = SOTA_MAX_DAMAGE;
    nourstest_true(Equation_Unit_Favor(wpn_favor, luck, support) == SOTA_MAX_DAMAGE);
    support = 0;
    luck = SOTA_MAX_DAMAGE;
    nourstest_true(Equation_Unit_Favor(wpn_favor, luck, support) == 127);
    support = SOTA_MAX_DAMAGE;
    wpn_favor = SOTA_MAX_DAMAGE;
    nourstest_true(Equation_Unit_Favor(wpn_favor, luck, support) == SOTA_MAX_DAMAGE);
}

void test_Equation_Unit_Crit() {
    u8 wpn_crit  = 0;
    u8 dex       = 0;
    u8 luck      = 0;
    u8 support   = 0;
    nourstest_true(Equation_Unit_Crit(wpn_crit, dex, luck, support) == 0);
    wpn_crit = 10;
    nourstest_true(Equation_Unit_Crit(wpn_crit, dex, luck,
                                      support) == (wpn_crit + dex / CRIT_DEX_FACTOR + luck / CRIT_LUCK_FACTOR + support));
    support = 10;
    nourstest_true(Equation_Unit_Crit(wpn_crit, dex, luck,
                                      support) == (wpn_crit + dex / CRIT_DEX_FACTOR + luck / CRIT_LUCK_FACTOR + support));
    dex = 10;
    nourstest_true(Equation_Unit_Crit(wpn_crit, dex, luck,
                                      support) == (wpn_crit + dex / CRIT_DEX_FACTOR + luck / CRIT_LUCK_FACTOR + support));
    luck = 10;
    nourstest_true(Equation_Unit_Crit(wpn_crit, dex, luck,
                                      support) == (wpn_crit + dex / CRIT_DEX_FACTOR + luck / CRIT_LUCK_FACTOR + support));
    support = SOTA_MAX_DAMAGE;
    nourstest_true(Equation_Unit_Crit(wpn_crit, dex, luck, support) == SOTA_MAX_DAMAGE);
    luck = SOTA_MAX_DAMAGE;
    nourstest_true(Equation_Unit_Crit(wpn_crit, dex, luck, support) == SOTA_MAX_DAMAGE);
    luck = 0;
    dex = SOTA_MAX_DAMAGE;
    nourstest_true(Equation_Unit_Crit(wpn_crit, dex, luck, support) == SOTA_MAX_DAMAGE);
    dex       = 100;
    wpn_crit  = 100;
    luck      = 100;
    support   = 100;
    nourstest_true(Equation_Unit_Crit(wpn_crit, dex, luck, support) == SOTA_MAX_DAMAGE);
    dex   = 200;
    luck  = 200;
    nourstest_true(Equation_Unit_Crit(wpn_crit, dex, luck, support) == SOTA_MAX_DAMAGE);
}

void test_Equation_Unit_Speed() {
    u8 wpn_wgt = 0;
    u8 agi = 0;
    u8 con = 0;
    u8 str = 0;
    nourstest_true(Equation_Unit_Speed(wpn_wgt, agi, con, str, 0) == 0);
    wpn_wgt = SOTA_MAX_WGT;
    nourstest_true(Equation_Unit_Speed(wpn_wgt, agi, con, str, 0) == SOTA_MIN_SPEED);
    agi = 10;
    nourstest_true(Equation_Unit_Speed(wpn_wgt, agi, con, str, 0) == SOTA_MIN_SPEED);
    agi = SOTA_MAX_AGI;
    nourstest_true(Equation_Unit_Speed(wpn_wgt, agi, con, str, 0) == 0);
    wpn_wgt = 0;
    agi = 10;
    nourstest_true(Equation_Unit_Speed(wpn_wgt, agi, con, str, 0) == (agi));
    wpn_wgt = 10;
    nourstest_true(Equation_Unit_Speed(wpn_wgt, agi, con, str, 0) == (agi - wpn_wgt));
    agi = 20;
    nourstest_true(Equation_Unit_Speed(wpn_wgt, agi, con, str, 0) == (agi - wpn_wgt));
    str = 4;
    nourstest_true(Equation_Unit_Speed(wpn_wgt, agi, con, str, 0) == (agi - wpn_wgt + str / 4));
    con = 2;
    nourstest_true(Equation_Unit_Speed(wpn_wgt, agi, con, str,
                                       0) == (agi - wpn_wgt + str / 4 + con / 2));
    con = 20;
    nourstest_true(Equation_Unit_Speed(wpn_wgt, agi, con, str, 0) == agi);
    con = 2;
    str = 40;
    nourstest_true(Equation_Unit_Speed(wpn_wgt, agi, con, str, 0) == agi);
    str = SOTA_MAX_DAMAGE;
    nourstest_true(Equation_Unit_Speed(wpn_wgt, agi, con, str, 0) == agi);
    str = 0;
    con = SOTA_MAX_DAMAGE;
    nourstest_true(Equation_Unit_Speed(wpn_wgt, agi, con, str, 0) == agi);
}

void test_Equation_simple() {

    nourstest_true(int_inbounds( 1, 0, 23) == 1);
    nourstest_true(int_inbounds(-1, 0, 23) == 0);
    nourstest_true(int_inbounds(24, 0, 23) == 23);
    nourstest_true(int_inbounds( 1, 0, 23) == 1);
    nourstest_true(int_inbounds( 2, 0, 23) == 2);
    nourstest_true(int_inbounds( 3, 0, 23) == 3);
    nourstest_true(int_inbounds( 4, 0, 23) == 4);
    nourstest_true(int_inbounds( 5, 0, 23) == 5);
    nourstest_true(int_inbounds( 6, 0, 23) == 6);
    nourstest_true(int_inbounds( 7, 0, 23) == 7);
    nourstest_true(int_inbounds( 8, 0, 23) == 8);
    nourstest_true(int_inbounds( 9, 0, 23) == 9);
    nourstest_true(int_inbounds( 0, 0, 23) == 0);
    nourstest_true(int_inbounds(23, 0, 23) == 23);

    nourstest_true(Equation_Weapon_Attack(1, 2) == 3);
    nourstest_true(Equation_Weapon_Attack(SOTA_MAX_DAMAGE, 2) == SOTA_MAX_DAMAGE);
    nourstest_true(Equation_Weapon_Attack(1, SOTA_MAX_DAMAGE) == SOTA_MAX_DAMAGE);
    nourstest_true(Equation_Weapon_Attack(0, 0) == 0);
    nourstest_true(Equation_Weapon_Attackvar(2, 1, 2) == 3);
    nourstest_true(Equation_Weapon_Attackvar(2, 1, SOTA_MAX_DAMAGE) == SOTA_MAX_DAMAGE);
    nourstest_true(Equation_Weapon_Attackvar(2, SOTA_MAX_DAMAGE, 2) == SOTA_MAX_DAMAGE);
    nourstest_true(Equation_Weapon_Attackvar(3, 3, 4, 5) == 12);

    nourstest_true(Equation_Weapon_Defense(1, 2) == 3);
    nourstest_true(Equation_Weapon_Defense(SOTA_MAX_DAMAGE, 2) == SOTA_MAX_DAMAGE);
    nourstest_true(Equation_Weapon_Defense(1, SOTA_MAX_DAMAGE) == SOTA_MAX_DAMAGE);
    nourstest_true(Equation_Weapon_Defense(0, 0) == 0);
    nourstest_true(Equation_Weapon_Defensevar(2, 1, 2) == 3);
    nourstest_true(Equation_Weapon_Defensevar(2, 1, SOTA_MAX_DAMAGE) == SOTA_MAX_DAMAGE);
    nourstest_true(Equation_Weapon_Defensevar(2, SOTA_MAX_DAMAGE, 2) == SOTA_MAX_DAMAGE);
    nourstest_true(Equation_Weapon_Defensevar(3, 3, 4, 5) == 12);

    nourstest_true(Equation_Combat_Damage(1, 2, NOTEFFECTIVE_FACTOR, CRIT_FACTOR, false) == 0);
    nourstest_true(Equation_Combat_Damage(2, 2, NOTEFFECTIVE_FACTOR, CRIT_FACTOR, false) == 0);
    nourstest_true(Equation_Combat_Damage(3, 2, NOTEFFECTIVE_FACTOR, CRIT_FACTOR, false) == 1);
    nourstest_true(Equation_Combat_Damage(3, 2, NOTEFFECTIVE_FACTOR, CRIT_FACTOR, true)  == 1);
    nourstest_true(Equation_Combat_Damage(3, 2, NOTEFFECTIVE_FACTOR, CRIT_FACTOR_PROMOTED, true) == 2);
    nourstest_true(Equation_Combat_Damage(3, SOTA_MAX_DAMAGE, NOTEFFECTIVE_FACTOR, CRIT_FACTOR,
                                          true)  == 0);
    nourstest_true(Equation_Combat_Damage(SOTA_MAX_DAMAGE, SOTA_MAX_DAMAGE, NOTEFFECTIVE_FACTOR,
                                          CRIT_FACTOR,
                                          true) == 0);
    nourstest_true(Equation_Combat_Damage(SOTA_MAX_DAMAGE, 1, NOTEFFECTIVE_FACTOR, CRIT_FACTOR,
                                          true) == SOTA_MAX_DAMAGE);

    nourstest_true(Equation_Combat_Damage(1, 2, EFFECTIVE_FACTOR, CRIT_FACTOR, false) == 0);
    nourstest_true(Equation_Combat_Damage(2, 2, EFFECTIVE_FACTOR, CRIT_FACTOR, false) == 0);
    nourstest_true(Equation_Combat_Damage(3, 2, EFFECTIVE_FACTOR, CRIT_FACTOR, false) == 3);
    nourstest_true(Equation_Combat_Damage(3, 2, EFFECTIVE_FACTOR, CRIT_FACTOR, true)  == 4);
    nourstest_true(Equation_Combat_Damage(3, 2, EFFECTIVE_FACTOR, CRIT_FACTOR_PROMOTED, true) == 6);
    nourstest_true(Equation_Combat_Damage(3, SOTA_MAX_DAMAGE, EFFECTIVE_FACTOR, CRIT_FACTOR,
                                          true)  ==  0);
    nourstest_true(Equation_Combat_Damage(SOTA_MAX_DAMAGE, SOTA_MAX_DAMAGE, EFFECTIVE_FACTOR,
                                          CRIT_FACTOR,
                                          true) == 0);
    nourstest_true(Equation_Combat_Damage(SOTA_MAX_DAMAGE, 1, EFFECTIVE_FACTOR, CRIT_FACTOR,
                                          true) == SOTA_MAX_DAMAGE);

    nourstest_true(Equation_Weapon_Dodge(1, 2)                == 3);
    nourstest_true(Equation_Weapon_Dodge(SOTA_MAX_DAMAGE, 2)        == SOTA_MAX_DODGE);
    nourstest_true(Equation_Weapon_Dodge(1, SOTA_MAX_DAMAGE)        == SOTA_MAX_DODGE);
    nourstest_true(Equation_Weapon_Dodge(0, 0)                == 0);
    nourstest_true(Equation_Weapon_Dodgevar(2, 1, 2)          == 3);
    nourstest_true(Equation_Weapon_Dodgevar(2, 1, SOTA_MAX_DAMAGE)  == SOTA_MAX_DODGE);
    nourstest_true(Equation_Weapon_Dodgevar(2, SOTA_MAX_DAMAGE, 2)  == SOTA_MAX_DODGE);
    nourstest_true(Equation_Weapon_Dodgevar(3, 3, 4, 5)       == 12);

    nourstest_true(Equation_Weapon_Favor(1, 2) == 3);
    nourstest_true(Equation_Weapon_Favor(SOTA_MAX_DAMAGE, 2) == SOTA_MAX_FAVOR);
    nourstest_true(Equation_Weapon_Favor(1, SOTA_MAX_DAMAGE) == SOTA_MAX_FAVOR);
    nourstest_true(Equation_Weapon_Favor(0, 0) == 0);
    nourstest_true(Equation_Weapon_Favorvar(2, 1, 2) == 3);
    nourstest_true(Equation_Weapon_Favorvar(2, 1, SOTA_MAX_DAMAGE) == SOTA_MAX_FAVOR);
    nourstest_true(Equation_Weapon_Favorvar(2, SOTA_MAX_DAMAGE, 2) == SOTA_MAX_FAVOR);
    nourstest_true(Equation_Weapon_Favorvar(3, 3, 4, 5) == 12);

    nourstest_true(Equation_Weapon_Hit(1, 2) == 1);
    nourstest_true(Equation_Weapon_Hit(SOTA_MAX_DAMAGE, 2) == 128);
    nourstest_true(Equation_Weapon_Hit(1, SOTA_MAX_DAMAGE) == 128);
    nourstest_true(Equation_Weapon_Hit(0, SOTA_MAX_DAMAGE) == SOTA_MAX_HIT);
    nourstest_true(Equation_Weapon_Hit(SOTA_MAX_DAMAGE, 0) == SOTA_MAX_HIT);
    nourstest_true(Equation_Weapon_Hit(0, 10) == 10);
    nourstest_true(Equation_Weapon_Hit(10, 0) == 10);
    nourstest_true(Equation_Weapon_Hit(SOTA_MAX_DAMAGE, SOTA_MAX_DAMAGE) == SOTA_MAX_DAMAGE);
    nourstest_true(Equation_Weapon_Hit(0, 0) == 0);
    nourstest_true(Equation_Weapon_Hitvar(2, 1, 2) == 3);
    nourstest_true(Equation_Weapon_Hitvar(2, SOTA_MAX_DAMAGE, SOTA_MAX_DAMAGE) == SOTA_MAX_DAMAGE);
    nourstest_true(Equation_Weapon_Hitvar(3, SOTA_MAX_DAMAGE, SOTA_MAX_DAMAGE,
                                          SOTA_MAX_DAMAGE) == SOTA_MAX_DAMAGE);
    nourstest_true(Equation_Weapon_Hitvar(3, 1000, 1000, 1000) == SOTA_MAX_DAMAGE);

    nourstest_true(Equation_Unit_Healshp(27,  15) ==  4);
    nourstest_true(Equation_Unit_Healshp(27,  25) ==  6);
    nourstest_true(Equation_Unit_Healshp(27,  50) == 13);
    nourstest_true(Equation_Unit_Healshp(27, 100) == 27);

    nourstest_true(Equation_Unit_Healshpvar(2, 75, 35) == 110);
    nourstest_true(Equation_Unit_Healshpvar(2, 75, 45) == 120);
    nourstest_true(Equation_Unit_Healshpvar(3, 30, 30, 30) == 90);
    nourstest_true(Equation_Unit_Healshpvar(4, 30, 30, 30, 30) == 120);

    nourstest_true(Equation_Combat_Hit( 30,  20) == 10);
    nourstest_true(Equation_Combat_Hit( 10,  20) ==  0);
    nourstest_true(Equation_Combat_Hit(200, 200) ==  0);

    nourstest_true(Equation_Combat_Crit( 30,  20) == 10);
    nourstest_true(Equation_Combat_Crit( 10,  20) ==  0);
    nourstest_true(Equation_Combat_Crit(200, 200) ==  0);

    nourstest_true(Equation_Attack_Damage( 30,  20) == 10);
    nourstest_true(Equation_Attack_Damage( 10,  20) ==  0);
    nourstest_true(Equation_Attack_Damage(200, 200) ==  0);
}

void test_equations() {
    test_Equation_Unit_Hit();
    test_Equation_Unit_Dodge();
    test_Equation_Unit_Speed();
    test_Equation_Unit_Crit();
    test_Equation_Unit_Favor();
    test_Equation_simple();
}