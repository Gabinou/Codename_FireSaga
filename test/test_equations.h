#include "minctest.h"
#include <stdint.h>
#include "equations.h"


void test_equation_unitHit() {
    SDL_Log("test_equation_unitHit");
    uint8_t wpn_hit = 0;
    uint8_t dex = 0;
    uint8_t luck = 0;
    uint8_t support = 0;
    lok(equation_unitHit(wpn_hit, dex, luck, support) == 0);
    wpn_hit = 10;
    lok(equation_unitHit(wpn_hit, dex, luck, support) == wpn_hit);
    luck = 10;
    lok(equation_unitHit(wpn_hit, dex, luck, support) == (wpn_hit + luck));
    dex = 10;
    lok(equation_unitHit(wpn_hit, dex, luck, support) == (wpn_hit + luck + 3 * dex));
    wpn_hit = 80;
    luck = 1;
    dex = 10;
    support = 1;
    lok(equation_unitHit(wpn_hit, dex, luck, support) == (wpn_hit + luck + 3 * dex + support));
    dex = UINT8_MAX;
    lok(equation_unitHit(wpn_hit, dex, luck, support) == UINT8_MAX);
    wpn_hit = UINT8_MAX;
    dex = UINT8_MAX;
    luck = UINT8_MAX;
    support = UINT8_MAX;
    lok(equation_unitHit(wpn_hit, dex, luck, support) == UINT8_MAX);

}

void test_equation_unitDodge() {
    SDL_Log("test_equation_unitDodge");
    uint8_t wpn_dodge = 0;
    uint8_t wpn_wgt = 0;
    uint8_t tile_dodge = 0;
    uint8_t str = 0;
    uint8_t agi = 0;
    uint8_t con = 0;
    uint8_t luck = 0;
    uint8_t support = 0;
    lok(equation_unitDodge(wpn_dodge, wpn_wgt, luck, agi, str, con, tile_dodge, support) == 0);
    wpn_wgt = 10;
    lok(equation_unitDodge(wpn_dodge, wpn_wgt, luck, agi, str, con, tile_dodge, support) == (-10));
    str = 4;
    lok(equation_unitDodge(wpn_dodge, wpn_wgt, luck, agi, str, con, tile_dodge, support) == (-9));
    con = 2;
    lok(equation_unitDodge(wpn_dodge, wpn_wgt, luck, agi, str, con, tile_dodge, support) == (-8));
    luck = 10;
    lok(equation_unitDodge(wpn_dodge, wpn_wgt, luck, agi, str, con, tile_dodge, support) == (2));
    agi = 4;
    lok(equation_unitDodge(wpn_dodge, wpn_wgt, luck, agi, str, con, tile_dodge, support) == (6));
    tile_dodge = 3;
    lok(equation_unitDodge(wpn_dodge, wpn_wgt, luck, agi, str, con, tile_dodge, support) == (9));
    support = 7;
    lok(equation_unitDodge(wpn_dodge, wpn_wgt, luck, agi, str, con, tile_dodge, support) == (16));
    wpn_dodge = 5;
    lok(equation_unitDodge(wpn_dodge, wpn_wgt, luck, agi, str, con, tile_dodge, support) == (21));
    wpn_wgt = UINT8_MAX;
    lok(equation_unitDodge(wpn_dodge, wpn_wgt, luck, agi, str, con, tile_dodge, support) == (INT8_MIN + wpn_dodge + luck + tile_dodge + support));
    wpn_dodge = UINT8_MAX;
    lok(equation_unitDodge(wpn_dodge, wpn_wgt, luck, agi, str, con, tile_dodge, support) == INT8_MAX);
}

void test_equation_unitFavor() {
    SDL_Log("test_equation_unitFavor");
    uint8_t wpn_favor = 0;
    uint8_t luck = 0;
    uint8_t support = 0;
    lok(equation_unitFavor(wpn_favor, luck, support) == (wpn_favor + luck / 2 + support));
    support = 10;
    lok(equation_unitFavor(wpn_favor, luck, support) == (wpn_favor + luck / 2 + support));
    luck = 10;
    support = 0;
    lok(equation_unitFavor(wpn_favor, luck, support) == (wpn_favor + luck / 2 + support));
    wpn_favor = 10;
    lok(equation_unitFavor(wpn_favor, luck, support) == (wpn_favor + luck / 2 + support));

    wpn_favor = UINT8_MAX;
    lok(equation_unitFavor(wpn_favor, luck, support) == UINT8_MAX);
    wpn_favor = 0;
    support = UINT8_MAX;
    lok(equation_unitFavor(wpn_favor, luck, support) == UINT8_MAX);
    support = 0;
    luck = UINT8_MAX;
    lok(equation_unitFavor(wpn_favor, luck, support) == 127);
    support = UINT8_MAX;
    wpn_favor = UINT8_MAX;
    lok(equation_unitFavor(wpn_favor, luck, support) == UINT8_MAX);
}

void test_equation_unitCrit() {
    SDL_Log("test_equation_unitCrit");
    uint8_t wpn_crit = 0;
    uint8_t dex = 0;
    uint8_t luck = 0;
    uint8_t support = 0;
    lok(equation_unitCrit(wpn_crit, dex, luck, support) == 0);
    wpn_crit = 10;
    lok(equation_unitCrit(wpn_crit, dex, luck, support) == (wpn_crit + dex / 4 + luck / 4 + support));
    support = 10;
    lok(equation_unitCrit(wpn_crit, dex, luck, support) == (wpn_crit + dex / 4 + luck / 4 + support));
    dex = 10;
    lok(equation_unitCrit(wpn_crit, dex, luck, support) == (wpn_crit + dex / 4 + luck / 4 + support));
    luck = 10;
    lok(equation_unitCrit(wpn_crit, dex, luck, support) == (wpn_crit + dex / 4 + luck / 4 + support));
    support = UINT8_MAX;
    lok(equation_unitCrit(wpn_crit, dex, luck, support) == UINT8_MAX);
    luck = UINT8_MAX;
    lok(equation_unitCrit(wpn_crit, dex, luck, support) == UINT8_MAX);
    luck = 0;
    dex = UINT8_MAX;
    lok(equation_unitCrit(wpn_crit, dex, luck, support) == UINT8_MAX);
    dex = 100;
    wpn_crit = 100;
    luck = 100;
    support = 100;
    lok(equation_unitCrit(wpn_crit, dex, luck, support) == (wpn_crit + dex / 4 + luck / 4 + support));
    dex = 200;
    luck = 200;
    lok(equation_unitCrit(wpn_crit, dex, luck, support) == UINT8_MAX);
}

void test_equation_unitSpeed() {
    SDL_Log("test_equation_unitSpeed");
    uint8_t wpn_wgt = 0;
    uint8_t agi = 0;
    uint8_t con = 0;
    uint8_t str = 0;
    lok(equation_unitSpeed(wpn_wgt, agi, con, str) == 0);
    wpn_wgt = UINT8_MAX;
    lok(equation_unitSpeed(wpn_wgt, agi, con, str) == INT8_MIN);
    agi = 10;
    lok(equation_unitSpeed(wpn_wgt, agi, con, str) == INT8_MIN);
    agi = 128;
    lok(equation_unitSpeed(wpn_wgt, agi, con, str) == INT8_MIN + 1);
    wpn_wgt = 0;
    agi = 10;
    lok(equation_unitSpeed(wpn_wgt, agi, con, str) == (agi));
    wpn_wgt = 10;
    lok(equation_unitSpeed(wpn_wgt, agi, con, str) == (agi - wpn_wgt));
    agi = 20;
    lok(equation_unitSpeed(wpn_wgt, agi, con, str) == (agi - wpn_wgt));
    str = 4;
    lok(equation_unitSpeed(wpn_wgt, agi, con, str) == (agi - wpn_wgt + str / 4));
    con = 2;
    lok(equation_unitSpeed(wpn_wgt, agi, con, str) == (agi - wpn_wgt + str / 4 + con / 2));
    con = 20;
    lok(equation_unitSpeed(wpn_wgt, agi, con, str) == agi);
    con = 2;
    str = 40;
    lok(equation_unitSpeed(wpn_wgt, agi, con, str) == agi);
    str = UINT8_MAX;
    lok(equation_unitSpeed(wpn_wgt, agi, con, str) == agi);
    str = 0;
    con = UINT8_MAX;
    lok(equation_unitSpeed(wpn_wgt, agi, con, str) == agi);
}

void test_equation_simple() {
    SDL_Log("test_equation_simple");

    lok(equation_inbounds(1, 0, 23) == 1);
    lok(equation_inbounds(-1, 0, 23) == 0);
    lok(equation_inbounds(24, 0, 23) == 23);
    lok(equation_inbounds(1, 0, 23) == 1);
    lok(equation_inbounds(2, 0, 23) == 2);
    lok(equation_inbounds(3, 0, 23) == 3);
    lok(equation_inbounds(4, 0, 23) == 4);
    lok(equation_inbounds(5, 0, 23) == 5);
    lok(equation_inbounds(6, 0, 23) == 6);
    lok(equation_inbounds(7, 0, 23) == 7);
    lok(equation_inbounds(8, 0, 23) == 8);
    lok(equation_inbounds(9, 0, 23) == 9);
    lok(equation_inbounds(0, 0, 23) == 0);
    lok(equation_inbounds(23, 0, 23) == 23);

    lok(equation_attack(1, 2) == 3);
    lok(equation_attack(UINT8_MAX, 2) == UINT8_MAX);
    lok(equation_attack(1, UINT8_MAX) == UINT8_MAX);
    lok(equation_attack(0, 0) == 0);
    lok(equation_attackvar(2, 1, 2) == 3);
    lok(equation_attackvar(2, 1, UINT8_MAX) == UINT8_MAX);
    lok(equation_attackvar(2, UINT8_MAX, 2) == UINT8_MAX);
    lok(equation_attackvar(3, 3, 4, 5) == 12);

    lok(equation_defense(1, 2) == 3);
    lok(equation_defense(UINT8_MAX, 2) == UINT8_MAX);
    lok(equation_defense(1, UINT8_MAX) == UINT8_MAX);
    lok(equation_defense(0, 0) == 0);
    lok(equation_defensevar(2, 1, 2) == 3);
    lok(equation_defensevar(2, 1, UINT8_MAX) == UINT8_MAX);
    lok(equation_defensevar(2, UINT8_MAX, 2) == UINT8_MAX);
    lok(equation_defensevar(3, 3, 4, 5) == 12);

    lok(equation_combatDamage(1, 2, NOTEFFECTIVE_FACTOR, CRIT_FACTOR, false) == 0);
    lok(equation_combatDamage(2, 2, NOTEFFECTIVE_FACTOR, CRIT_FACTOR, false) == 0);
    lok(equation_combatDamage(3, 2, NOTEFFECTIVE_FACTOR, CRIT_FACTOR, false) == 1);
    lok(equation_combatDamage(3, 2, NOTEFFECTIVE_FACTOR, CRIT_FACTOR, true) == 2);
    lok(equation_combatDamage(3, 2, NOTEFFECTIVE_FACTOR, CRIT_FACTOR_PROMOTED, true) == 4);
    lok(equation_combatDamage(3, UINT8_MAX, NOTEFFECTIVE_FACTOR, CRIT_FACTOR, true) == 0);
    lok(equation_combatDamage(UINT8_MAX, UINT8_MAX, NOTEFFECTIVE_FACTOR, CRIT_FACTOR, true) == 0);
    lok(equation_combatDamage(UINT8_MAX, 1, NOTEFFECTIVE_FACTOR, CRIT_FACTOR, true) == 254);

    lok(equation_combatDamage(1, 2, EFFECTIVE_FACTOR, CRIT_FACTOR, false) == 0);
    lok(equation_combatDamage(2, 2, EFFECTIVE_FACTOR, CRIT_FACTOR, false) == 2);
    lok(equation_combatDamage(3, 2, EFFECTIVE_FACTOR, CRIT_FACTOR, false) == 4);
    lok(equation_combatDamage(3, 2, EFFECTIVE_FACTOR, CRIT_FACTOR, true) == 7);
    lok(equation_combatDamage(3, 2, EFFECTIVE_FACTOR, CRIT_FACTOR_PROMOTED, true) == 10);
    lok(equation_combatDamage(3, UINT8_MAX, EFFECTIVE_FACTOR, CRIT_FACTOR, true) == 0);
    lok(equation_combatDamage(UINT8_MAX, UINT8_MAX, EFFECTIVE_FACTOR, CRIT_FACTOR, true) == 0);
    lok(equation_combatDamage(UINT8_MAX, 1, EFFECTIVE_FACTOR, CRIT_FACTOR, true) == 254);

    lok(equation_wpnDodge(1, 2) == 3);
    lok(equation_wpnDodge(UINT8_MAX, 2) == UINT8_MAX);
    lok(equation_wpnDodge(1, UINT8_MAX) == UINT8_MAX);
    lok(equation_wpnDodge(0, 0) == 0);
    lok(equation_wpnDodgevar(2, 1, 2) == 3);
    lok(equation_wpnDodgevar(2, 1, UINT8_MAX) == UINT8_MAX);
    lok(equation_wpnDodgevar(2, UINT8_MAX, 2) == UINT8_MAX);
    lok(equation_wpnDodgevar(3, 3, 4, 5) == 12);

    lok(equation_wpnFavor(1, 2) == 3);
    lok(equation_wpnFavor(UINT8_MAX, 2) == UINT8_MAX);
    lok(equation_wpnFavor(1, UINT8_MAX) == UINT8_MAX);
    lok(equation_wpnFavor(0, 0) == 0);
    lok(equation_wpnFavorvar(2, 1, 2) == 3);
    lok(equation_wpnFavorvar(2, 1, UINT8_MAX) == UINT8_MAX);
    lok(equation_wpnFavorvar(2, UINT8_MAX, 2) == UINT8_MAX);
    lok(equation_wpnFavorvar(3, 3, 4, 5) == 12);

    lok(equation_wpnHit(1, 2) == 1);
    lok(equation_wpnHit(UINT8_MAX, 2) == 128);
    lok(equation_wpnHit(1, UINT8_MAX) == 127);
    lok(equation_wpnHit(0, UINT8_MAX) == UINT8_MAX);
    lok(equation_wpnHit(UINT8_MAX, 0) == UINT8_MAX);
    lok(equation_wpnHit(0, 10) == 10);
    lok(equation_wpnHit(10, 0) == 10);
    lok(equation_wpnHit(UINT8_MAX, UINT8_MAX) == 254);
    lok(equation_wpnHit(0, 0) == 0);
    lok(equation_wpnHitvar(2, 1, 2) == 1);
    lok(equation_wpnHitvar(2, UINT8_MAX, UINT8_MAX) == 254);
    lok(equation_wpnHitvar(3, UINT8_MAX, UINT8_MAX, UINT8_MAX) == UINT8_MAX);
    lok(equation_wpnHitvar(3, 1000, 1000, 1000) == UINT8_MAX);

    lok(equation_Unit_Healshp(27, 15) == 4);
    lok(equation_Unit_Healshp(27, 25) == 6);
    lok(equation_Unit_Healshp(27, 50) == 13);
    lok(equation_Unit_Healshp(27, 100) == 27);

    lok(equation_Unit_Healshpvar(2, 75, 35) == 102);
    lok(equation_Unit_Healshpvar(2, 75, 45) == 105);
    lok(equation_Unit_Healshpvar(3, 30, 30, 30) == 90);
    lok(equation_Unit_Healshpvar(4, 30, 30, 30, 30) == 105);

    lok(equation_rateHit(30, 20) == 10);
    lok(equation_rateHit(10, 20) == 0);
    lok(equation_rateHit(200, 200) == 0);

    lok(equation_rateCrit(30, 20) == 10);
    lok(equation_rateCrit(10, 20) == 0);
    lok(equation_rateCrit(200, 200) == 0);

    lok(equation_attackDamage(30, 20) == 10);
    lok(equation_attackDamage(10, 20) == 0);
    lok(equation_attackDamage(200, 200) == 0);
}

void test_equations() {
    SDL_Log("test_equations");
    test_equation_unitHit();
    test_equation_unitDodge();
    test_equation_unitSpeed();
    test_equation_unitCrit();
    test_equation_unitFavor();
    test_equation_simple();
}