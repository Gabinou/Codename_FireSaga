
#include "nourstest.h"
#include <stdint.h>
#include "equations.h"
#include "utilities.h"

void test_Eq_Unit_Hit() {
    i32 support = 0;
    Weapon_stats    wpn_stats   = Weapon_stats_default;
    Unit_stats      unit_stats  = Unit_stats_default;
    nourstest_true(Eq_Unit_Hit(wpn_stats, unit_stats, support) == 0);
    wpn_stats.hit       = 10;
    nourstest_true(Eq_Unit_Hit(wpn_stats, unit_stats, support) == wpn_stats.hit);
    unit_stats.luck     = 10;
    nourstest_true(Eq_Unit_Hit(wpn_stats, unit_stats,
                               support) == (wpn_stats.hit + unit_stats.luck / HIT_LUCK_FACTOR));
    unit_stats.dex      = 10;
    nourstest_true(Eq_Unit_Hit(wpn_stats, unit_stats,
                               support) == (wpn_stats.hit + unit_stats.luck / HIT_LUCK_FACTOR + HIT_DEX_FACTOR * unit_stats.dex));
    wpn_stats.hit   = 80;
    unit_stats.luck =  1;
    unit_stats.dex  = 10;
    support         =  1;
    nourstest_true(Eq_Unit_Hit(wpn_stats, unit_stats,
                               support) == (wpn_stats.hit + unit_stats.luck / HIT_LUCK_FACTOR + HIT_DEX_FACTOR * unit_stats.dex +
                                            support));
    unit_stats.dex     = SOTA_MAX_DAMAGE;
    nourstest_true(Eq_Unit_Hit(wpn_stats, unit_stats, support) == SOTA_MAX_DAMAGE);
    wpn_stats.hit       = SOTA_MAX_DAMAGE;
    unit_stats.dex      = SOTA_MAX_DAMAGE;
    unit_stats.luck     = SOTA_MAX_DAMAGE;
    support             = SOTA_MAX_DAMAGE;
    nourstest_true(Eq_Unit_Hit(wpn_stats, unit_stats, support) == SOTA_MAX_DAMAGE);
}

void test_Eq_Unit_Dodge() {
    i32 support     = 0;
    i32 tile_dodge  = 0;
    Weapon_stats    wpn_stats   = Weapon_stats_default;
    Unit_stats      unit_stats  = Unit_stats_default;

    nourstest_true(Eq_Unit_Dodge(wpn_stats, unit_stats, tile_dodge, support) ==
                   (wpn_stats.dodge + support + tile_dodge + unit_stats.luck / DODGE_LUCK_FACTOR + unit_stats.fth /
                    DODGE_FTH_FACTOR +
                    unit_stats.agi / DODGE_AGI_FACTOR - unit_stats.con / DODGE_CON_FACTOR  + unit_stats.str /
                    DODGE_STR_FACTOR));
    wpn_stats.wgt = 10;

    nourstest_true(Eq_Unit_Dodge(wpn_stats, unit_stats, tile_dodge,
                                 support) == (wpn_stats.dodge + support + tile_dodge + unit_stats.luck / DODGE_LUCK_FACTOR +
                                              unit_stats.fth / DODGE_FTH_FACTOR +
                                              unit_stats.agi / DODGE_AGI_FACTOR - unit_stats.con / DODGE_CON_FACTOR ) + unit_stats.str /
                   DODGE_STR_FACTOR);
    unit_stats.str = 4;
    nourstest_true(Eq_Unit_Dodge(wpn_stats, unit_stats, tile_dodge,
                                 support) == (wpn_stats.dodge + support + tile_dodge + unit_stats.luck / DODGE_LUCK_FACTOR +
                                              unit_stats.fth / DODGE_FTH_FACTOR +
                                              unit_stats.agi / DODGE_AGI_FACTOR - unit_stats.con / DODGE_CON_FACTOR ) + unit_stats.str /
                   DODGE_STR_FACTOR);
    unit_stats.con = 2;
    nourstest_true(Eq_Unit_Dodge(wpn_stats, unit_stats, tile_dodge,
                                 support) == (wpn_stats.dodge + support + tile_dodge + unit_stats.luck / DODGE_LUCK_FACTOR +
                                              unit_stats.fth / DODGE_FTH_FACTOR +
                                              unit_stats.agi / DODGE_AGI_FACTOR - unit_stats.con / DODGE_CON_FACTOR ) + unit_stats.str /
                   DODGE_STR_FACTOR);
    unit_stats.luck = 10;
    nourstest_true(Eq_Unit_Dodge(wpn_stats, unit_stats, tile_dodge,
                                 support) == (wpn_stats.dodge + support + tile_dodge + unit_stats.luck / DODGE_LUCK_FACTOR +
                                              unit_stats.fth / DODGE_FTH_FACTOR +
                                              unit_stats.agi / DODGE_AGI_FACTOR - unit_stats.con / DODGE_CON_FACTOR ) + unit_stats.str /
                   DODGE_STR_FACTOR);
    unit_stats.agi = 4;
    nourstest_true(Eq_Unit_Dodge(wpn_stats, unit_stats, tile_dodge,
                                 support) == (wpn_stats.dodge + support + tile_dodge + unit_stats.luck / DODGE_LUCK_FACTOR +
                                              unit_stats.fth / DODGE_FTH_FACTOR +
                                              unit_stats.agi / DODGE_AGI_FACTOR - unit_stats.con / DODGE_CON_FACTOR ) + unit_stats.str /
                   DODGE_STR_FACTOR);
    tile_dodge = 3;
    nourstest_true(Eq_Unit_Dodge(wpn_stats, unit_stats, tile_dodge,
                                 support) == (wpn_stats.dodge + support + tile_dodge + unit_stats.luck / DODGE_LUCK_FACTOR +
                                              unit_stats.fth / DODGE_FTH_FACTOR +
                                              unit_stats.agi / DODGE_AGI_FACTOR - unit_stats.con / DODGE_CON_FACTOR ) + unit_stats.str /
                   DODGE_STR_FACTOR);
    support = 7;
    nourstest_true(Eq_Unit_Dodge(wpn_stats, unit_stats, tile_dodge,
                                 support) == (wpn_stats.dodge + support + tile_dodge + unit_stats.luck / DODGE_LUCK_FACTOR +
                                              unit_stats.fth / DODGE_FTH_FACTOR +
                                              unit_stats.agi / DODGE_AGI_FACTOR - unit_stats.con / DODGE_CON_FACTOR ) + unit_stats.str /
                   DODGE_STR_FACTOR);
    wpn_stats.dodge = 5;
    nourstest_true(Eq_Unit_Dodge(wpn_stats, unit_stats, tile_dodge,
                                 support) == (wpn_stats.dodge + support + tile_dodge + unit_stats.luck / DODGE_LUCK_FACTOR +
                                              unit_stats.fth / DODGE_FTH_FACTOR +
                                              unit_stats.agi / DODGE_AGI_FACTOR - unit_stats.con / DODGE_CON_FACTOR ) + unit_stats.str /
                   DODGE_STR_FACTOR);
    wpn_stats.wgt = INT8_MAX;
    // i32 slowed = NMATH_MAX(0, wpn_stats.wgt - unit_stats.str / SPEED_STR_FACTOR);
    nourstest_true(Eq_Unit_Dodge(wpn_stats, unit_stats, tile_dodge, support) == SOTA_MIN_DODGE);
    wpn_stats.dodge = INT8_MAX;
    nourstest_true(Eq_Unit_Dodge(wpn_stats, unit_stats, tile_dodge,
                                 support) == (wpn_stats.dodge + support + tile_dodge +
                                              unit_stats.luck / DODGE_LUCK_FACTOR + unit_stats.fth / DODGE_FTH_FACTOR +
                                              unit_stats.agi / DODGE_AGI_FACTOR - unit_stats.con / DODGE_CON_FACTOR + unit_stats.str /
                                              DODGE_STR_FACTOR));
}

void test_Eq_Unit_Favor() {
    Weapon_stats    wpn_stats   = Weapon_stats_default;
    Unit_stats      unit_stats  = Unit_stats_default;
    i32 support          = 0;
    nourstest_true(Eq_Unit_Favor(wpn_stats, unit_stats,
                                 support) == (wpn_stats.favor + unit_stats.luck / 2 + support));
    support = 10;
    nourstest_true(Eq_Unit_Favor(wpn_stats, unit_stats,
                                 support) == (wpn_stats.favor + unit_stats.luck / 2 + support));
    unit_stats.luck = 10;
    support = 0;
    nourstest_true(Eq_Unit_Favor(wpn_stats, unit_stats,
                                 support) == (wpn_stats.favor + unit_stats.luck / 2 + support));
    wpn_stats.favor = 10;
    nourstest_true(Eq_Unit_Favor(wpn_stats, unit_stats,
                                 support) == (wpn_stats.favor + unit_stats.luck / 2 + support));

    wpn_stats.favor = SOTA_MAX_DAMAGE;
    nourstest_true(Eq_Unit_Favor(wpn_stats, unit_stats, support) == SOTA_MAX_DAMAGE);
    wpn_stats.favor = 0;
    support = SOTA_MAX_DAMAGE;
    nourstest_true(Eq_Unit_Favor(wpn_stats, unit_stats, support) == SOTA_MAX_DAMAGE);
    support = 0;
    unit_stats.luck = SOTA_MAX_DAMAGE;
    nourstest_true(Eq_Unit_Favor(wpn_stats, unit_stats, support) == 127);
    support = SOTA_MAX_DAMAGE;
    wpn_stats.favor = SOTA_MAX_DAMAGE;
    nourstest_true(Eq_Unit_Favor(wpn_stats, unit_stats, support) == SOTA_MAX_DAMAGE);
}

void test_Eq_Unit_Crit() {
    Weapon_stats    wpn_stats   = Weapon_stats_default;
    Unit_stats      unit_stats  = Unit_stats_default;
    i32 support = 0;
    nourstest_true(Eq_Unit_Crit(wpn_stats, unit_stats, support) == 0);
    wpn_stats.crit = 10;
    nourstest_true(Eq_Unit_Crit(wpn_stats, unit_stats,
                                support) == (wpn_stats.crit + unit_stats.dex / CRIT_DEX_FACTOR + unit_stats.luck / CRIT_LUCK_FACTOR
                                             + support));
    support = 10;
    nourstest_true(Eq_Unit_Crit(wpn_stats, unit_stats,
                                support) == (wpn_stats.crit + unit_stats.dex / CRIT_DEX_FACTOR + unit_stats.luck / CRIT_LUCK_FACTOR
                                             + support));
    unit_stats.dex = 10;
    nourstest_true(Eq_Unit_Crit(wpn_stats, unit_stats,
                                support) == (wpn_stats.crit + unit_stats.dex / CRIT_DEX_FACTOR + unit_stats.luck / CRIT_LUCK_FACTOR
                                             + support));
    unit_stats.luck = 10;
    nourstest_true(Eq_Unit_Crit(wpn_stats, unit_stats,
                                support) == (wpn_stats.crit + unit_stats.dex / CRIT_DEX_FACTOR + unit_stats.luck / CRIT_LUCK_FACTOR
                                             + support));
    support = SOTA_MAX_DAMAGE;
    nourstest_true(Eq_Unit_Crit(wpn_stats, unit_stats, support) == SOTA_MAX_DAMAGE);
    unit_stats.luck = SOTA_MAX_DAMAGE;
    nourstest_true(Eq_Unit_Crit(wpn_stats, unit_stats, support) == SOTA_MAX_DAMAGE);
    unit_stats.luck = 0;
    unit_stats.dex = SOTA_MAX_DAMAGE;
    nourstest_true(Eq_Unit_Crit(wpn_stats, unit_stats, support) == SOTA_MAX_DAMAGE);
    unit_stats.dex  = 100;
    wpn_stats.crit  = 100;
    unit_stats.luck = 100;
    support         = 100;
    nourstest_true(Eq_Unit_Crit(wpn_stats, unit_stats, support) == SOTA_MAX_DAMAGE);
    unit_stats.dex  = 200;
    unit_stats.luck = 200;
    nourstest_true(Eq_Unit_Crit(wpn_stats, unit_stats, support) == SOTA_MAX_DAMAGE);
}

void test_Eq_Unit_Speed() {
    Weapon_stats    wpn_stats   = Weapon_stats_default;
    Unit_stats      unit_stats  = Unit_stats_default;
    i32 support = 0;
    nourstest_true(Eq_Unit_Speed(wpn_stats, unit_stats,
                                 0) == 0);
    wpn_stats.wgt = SOTA_MAX_WGT;
    nourstest_true(Eq_Unit_Speed(wpn_stats, unit_stats,
                                 0) == SOTA_MIN_SPEED);
    unit_stats.agi = 10;
    nourstest_true(Eq_Unit_Speed(wpn_stats, unit_stats,
                                 0) == SOTA_MIN_SPEED);
    unit_stats.agi = SOTA_MAX_AGI;
    nourstest_true(Eq_Unit_Speed(wpn_stats, unit_stats,
                                 0) == 0);
    wpn_stats.wgt = 0;
    unit_stats.agi = 10;
    nourstest_true(Eq_Unit_Speed(wpn_stats, unit_stats,
                                 0) == (unit_stats.agi));
    wpn_stats.wgt = 10;
    nourstest_true(Eq_Unit_Speed(wpn_stats, unit_stats,
                                 0) == (unit_stats.agi - wpn_stats.wgt));
    unit_stats.agi = 20;
    nourstest_true(Eq_Unit_Speed(wpn_stats, unit_stats,
                                 0) == (unit_stats.agi - wpn_stats.wgt));
    unit_stats.str = 4;
    nourstest_true(Eq_Unit_Speed(wpn_stats, unit_stats,
                                 0) == (unit_stats.agi - wpn_stats.wgt + unit_stats.str / 4));
    unit_stats.con = 2;
    nourstest_true(Eq_Unit_Speed(wpn_stats, unit_stats,
                                 0) == (unit_stats.agi - wpn_stats.wgt + unit_stats.str / 4 + unit_stats.con / 2));
    unit_stats.con = 20;
    nourstest_true(Eq_Unit_Speed(wpn_stats, unit_stats,
                                 0) == unit_stats.agi);
    unit_stats.con = 2;
    unit_stats.str = 40;
    nourstest_true(Eq_Unit_Speed(wpn_stats, unit_stats,
                                 0) == unit_stats.agi);
    unit_stats.str = SOTA_MAX_DAMAGE;
    nourstest_true(Eq_Unit_Speed(wpn_stats, unit_stats,
                                 0) == unit_stats.agi);
    unit_stats.str = 0;
    unit_stats.con = SOTA_MAX_DAMAGE;
    nourstest_true(Eq_Unit_Speed(wpn_stats, unit_stats,
                                 0) == unit_stats.agi);
}

void test_Eq_simple() {

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

    nourstest_true(Eq_Wpn_Attack(1, 2) == 3);
    nourstest_true(Eq_Wpn_Attack(SOTA_MAX_DAMAGE, 2) == SOTA_MAX_DAMAGE);
    nourstest_true(Eq_Wpn_Attack(1, SOTA_MAX_DAMAGE) == SOTA_MAX_DAMAGE);
    nourstest_true(Eq_Wpn_Attack(0, 0) == 0);
    nourstest_true(Eq_Wpn_Attackarr((i32[]){1, 2}, 2) == 3);
    nourstest_true(Eq_Wpn_Attackarr((i32[]){1, SOTA_MAX_DAMAGE}, 2) == SOTA_MAX_DAMAGE);
    nourstest_true(Eq_Wpn_Attackarr((i32[]){SOTA_MAX_DAMAGE, 2}, 2) == SOTA_MAX_DAMAGE);
    nourstest_true(Eq_Wpn_Attackarr((i32[]){3, 4, 5}, 3) == 12);

    nourstest_true(Eq_Wpn_Defense(1, 2) == 3);
    nourstest_true(Eq_Wpn_Defense(SOTA_MAX_DAMAGE, 2) == SOTA_MAX_DAMAGE);
    nourstest_true(Eq_Wpn_Defense(1, SOTA_MAX_DAMAGE) == SOTA_MAX_DAMAGE);
    nourstest_true(Eq_Wpn_Defense(0, 0) == 0);
    nourstest_true(Eq_Wpn_Defensearr((i32[]){1, 2}, 2) == 3);
    nourstest_true(Eq_Wpn_Defensearr((i32[]){1, SOTA_MAX_DAMAGE}, 2) == SOTA_MAX_DAMAGE);
    nourstest_true(Eq_Wpn_Defensearr((i32[]){SOTA_MAX_DAMAGE, 2}, 2) == SOTA_MAX_DAMAGE);
    nourstest_true(Eq_Wpn_Defensearr((i32[]){3, 4, 5}, 3) == 12);

    nourstest_true(Eq_Combat_Damage(1, 2, NOTEFFECTIVE_FACTOR, CRIT_FACTOR, false) == 0);
    nourstest_true(Eq_Combat_Damage(2, 2, NOTEFFECTIVE_FACTOR, CRIT_FACTOR, false) == 0);
    nourstest_true(Eq_Combat_Damage(3, 2, NOTEFFECTIVE_FACTOR, CRIT_FACTOR, false) == 1);
    nourstest_true(Eq_Combat_Damage(3, 2, NOTEFFECTIVE_FACTOR, CRIT_FACTOR, true)  == 1);
    nourstest_true(Eq_Combat_Damage(3, 2, NOTEFFECTIVE_FACTOR, CRIT_FACTOR_PROMOTED, true) == 2);
    nourstest_true(Eq_Combat_Damage(3, SOTA_MAX_DAMAGE, NOTEFFECTIVE_FACTOR, CRIT_FACTOR,
                                    true)  == 0);
    nourstest_true(Eq_Combat_Damage(SOTA_MAX_DAMAGE, SOTA_MAX_DAMAGE, NOTEFFECTIVE_FACTOR,
                                    CRIT_FACTOR,
                                    true) == 0);
    nourstest_true(Eq_Combat_Damage(SOTA_MAX_DAMAGE, 1, NOTEFFECTIVE_FACTOR, CRIT_FACTOR,
                                    true) == SOTA_MAX_DAMAGE);

    nourstest_true(Eq_Combat_Damage(1, 2, EFFECTIVE_FACTOR, CRIT_FACTOR, false) == 0);
    nourstest_true(Eq_Combat_Damage(2, 2, EFFECTIVE_FACTOR, CRIT_FACTOR, false) == 0);
    nourstest_true(Eq_Combat_Damage(3, 2, EFFECTIVE_FACTOR, CRIT_FACTOR, false) == 3);
    nourstest_true(Eq_Combat_Damage(3, 2, EFFECTIVE_FACTOR, CRIT_FACTOR, true)  == 4);
    nourstest_true(Eq_Combat_Damage(3, 2, EFFECTIVE_FACTOR, CRIT_FACTOR_PROMOTED, true) == 6);
    nourstest_true(Eq_Combat_Damage(3, SOTA_MAX_DAMAGE, EFFECTIVE_FACTOR, CRIT_FACTOR,
                                    true)  ==  0);
    nourstest_true(Eq_Combat_Damage(SOTA_MAX_DAMAGE, SOTA_MAX_DAMAGE, EFFECTIVE_FACTOR,
                                    CRIT_FACTOR,
                                    true) == 0);
    nourstest_true(Eq_Combat_Damage(SOTA_MAX_DAMAGE, 1, EFFECTIVE_FACTOR, CRIT_FACTOR,
                                    true) == SOTA_MAX_DAMAGE);

    nourstest_true(Eq_Wpn_Dodge(1, 2)                == 3);
    nourstest_true(Eq_Wpn_Dodge(SOTA_MAX_DAMAGE, 2)        == SOTA_MAX_DODGE);
    nourstest_true(Eq_Wpn_Dodge(1, SOTA_MAX_DAMAGE)        == SOTA_MAX_DODGE);
    nourstest_true(Eq_Wpn_Dodge(0, 0)                == 0);
    nourstest_true(Eq_Wpn_Dodgearr((i32[]){1, 2}, 2)          == 3);
    nourstest_true(Eq_Wpn_Dodgearr((i32[]){1, SOTA_MAX_DAMAGE},3)  == SOTA_MAX_DODGE);
    nourstest_true(Eq_Wpn_Dodgearr((i32[]){SOTA_MAX_DAMAGE, 2},2)  == SOTA_MAX_DODGE);
    nourstest_true(Eq_Wpn_Dodgearr((i32[]){3, 4, 5},3)       == 12);

    nourstest_true(Eq_Wpn_Favor(1, 2) == 3);
    nourstest_true(Eq_Wpn_Favor(SOTA_MAX_DAMAGE, 2) == SOTA_MAX_FAVOR);
    nourstest_true(Eq_Wpn_Favor(1, SOTA_MAX_DAMAGE) == SOTA_MAX_FAVOR);
    nourstest_true(Eq_Wpn_Favor(0, 0) == 0);
    nourstest_true(Eq_Wpn_Favorarr((i32[]){1, 2}, 2) == 3);
    nourstest_true(Eq_Wpn_Favorarr((i32[]){1, SOTA_MAX_DAMAGE}, 2) == SOTA_MAX_FAVOR);
    nourstest_true(Eq_Wpn_Favorarr((i32[]){SOTA_MAX_DAMAGE, 2}, 2) == SOTA_MAX_FAVOR);
    nourstest_true(Eq_Wpn_Favorarr((i32[]){3, 4, 5}, 3) == 12);

    nourstest_true(Eq_Wpn_Hit(1, 2) == 1);
    nourstest_true(Eq_Wpn_Hit(SOTA_MAX_DAMAGE, 2) == 128);
    nourstest_true(Eq_Wpn_Hit(1, SOTA_MAX_DAMAGE) == 128);
    nourstest_true(Eq_Wpn_Hit(0, SOTA_MAX_DAMAGE) == SOTA_MAX_HIT);
    nourstest_true(Eq_Wpn_Hit(SOTA_MAX_DAMAGE, 0) == SOTA_MAX_HIT);
    nourstest_true(Eq_Wpn_Hit(0, 10) == 10);
    nourstest_true(Eq_Wpn_Hit(10, 0) == 10);
    nourstest_true(Eq_Wpn_Hit(SOTA_MAX_DAMAGE, SOTA_MAX_DAMAGE) == SOTA_MAX_DAMAGE);
    nourstest_true(Eq_Wpn_Hit(0, 0) == 0);
    nourstest_true(Eq_Wpn_Hitarr((i32[]){1, 2}, 2) == 3);
    nourstest_true(Eq_Wpn_Hitarr((i32[]){SOTA_MAX_DAMAGE, SOTA_MAX_DAMAGE}, 2) == SOTA_MAX_DAMAGE);
    nourstest_true(Eq_Wpn_Hitarr((i32[]){SOTA_MAX_DAMAGE, SOTA_MAX_DAMAGE,
                                       SOTA_MAX_DAMAGE}, 3) == SOTA_MAX_DAMAGE);
    nourstest_true(Eq_Wpn_Hitarr((i32[]){1000, 1000, 1000}, 3) == SOTA_MAX_DAMAGE);

    nourstest_true(Eq_Unit_Healshp(27,  15) ==  4);
    nourstest_true(Eq_Unit_Healshp(27,  25) ==  6);
    nourstest_true(Eq_Unit_Healshp(27,  50) == 13);
    nourstest_true(Eq_Unit_Healshp(27, 100) == 27);

    nourstest_true(Eq_Combat_Hit( 30,  20) == 10);
    nourstest_true(Eq_Combat_Hit( 10,  20) ==  0);
    nourstest_true(Eq_Combat_Hit(200, 200) ==  0);

    nourstest_true(Eq_Combat_Crit( 30,  20) == 10);
    nourstest_true(Eq_Combat_Crit( 10,  20) ==  0);
    nourstest_true(Eq_Combat_Crit(200, 200) ==  0);

    nourstest_true(Eq_Combat_Damage( 30,  20, 100, 100, 100) == 10);
    nourstest_true(Eq_Combat_Damage( 10,  20, 100, 100, 100) ==  0);
    nourstest_true(Eq_Combat_Damage(200, 200, 100, 100, 100) ==  0);
}

void test_equations() {
    test_Eq_Unit_Hit();
    test_Eq_Unit_Dodge();
    test_Eq_Unit_Speed();
    test_Eq_Unit_Crit();
    test_Eq_Unit_Favor();
    test_Eq_simple();
}