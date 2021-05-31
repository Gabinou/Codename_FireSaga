#include "equations.h"


/********************** EXPECTED DAMAGE COMPUTATION **************************/
// Multiplier, which gives expected damage  when * (def-att)
// Offensive skills should also give expected damage factor values.
float equation_expectedDamage(uint8_t in_hit_rate, uint8_t in_crit_rate, uint8_t in_critfactor) {
    return (in_hit_rate / 100.0f * pow((double)in_critfactor, ((double)in_crit_rate) / 100));
}


bool equation_canCarry(uint8_t rescuer_con, uint8_t rescuee_con) {
    return (rescuer_con > rescuee_con);
}

int32_t equation_inbounds(int_point_t pos, int_point_t boundmin, int_point_t boundmax) {
    int32_t out = 0;
    out = pos < boundmin ? boundmin : pos;
    out = out > boundmax ? boundmax : out;
    return (out);
}

uint8_t equation_agonyPercentonCrit(uint8_t luck, uint8_t con) {
    uint8_t notdeadyet_prob = luck;
    notdeadyet_prob = con > HYAKU_PERCENT - notdeadyet_prob ? HYAKU_PERCENT : notdeadyet_prob + con;
    return (notdeadyet_prob);
}

uint8_t equation_agonyTurns(uint8_t str, uint8_t def, uint8_t con) {
    uint8_t agony_length = str;
    agony_length = def > UINT8_MAX - agony_length ? UINT8_MAX : agony_length + def;
    agony_length = con > UINT8_MAX - agony_length ? UINT8_MAX : agony_length + con;
    agony_length /= 20;
    return (agony_length);
}

uint8_t equation_rateHit(uint8_t attacker_hit, uint8_t defender_avoid) {
    uint8_t out_hit = (defender_avoid > attacker_hit) ? 0 : attacker_hit - defender_avoid;
    return (out_hit);
}

uint8_t equation_rateCrit(uint8_t attacker_crit, uint8_t defender_favor) {
    uint8_t out_crit = (defender_favor > attacker_crit) ? 0 : attacker_crit - defender_favor;
    return (out_crit);
}

uint8_t equation_attackDamage(uint8_t attacker_dmg, uint8_t defender_def) {
    uint8_t out_dmg = (defender_def > attacker_dmg) ? 0 : attacker_dmg - defender_def;
    return (out_dmg);
}

uint8_t equation_unitHit(uint8_t wpn_hit, uint8_t dex, uint8_t luck, uint8_t support) {
    // hit = wpn_hit + dex*3 + luck + support
    uint8_t out_hit = wpn_hit;
    out_hit = ((dex * 3U) > UINT8_MAX - out_hit) ? UINT8_MAX : out_hit + (dex * 3U);
    out_hit = (luck > UINT8_MAX - out_hit) ? UINT8_MAX : out_hit + luck;
    out_hit = (support > UINT8_MAX - out_hit) ? UINT8_MAX : out_hit + support;
    return (out_hit);
}

uint8_t equation_unitCrit(uint8_t wpn_crit, uint8_t dex, uint8_t luck, uint8_t support) {
    // favor = wpn_crit + luck / 4 + dex / 4 + support
    uint8_t out_crit = wpn_crit;
    out_crit = ((luck / 4) > UINT8_MAX - out_crit) ? UINT8_MAX : out_crit + luck / 4;
    out_crit = ((dex / 4) > UINT8_MAX - out_crit) ? UINT8_MAX : out_crit + dex / 4;
    out_crit = (support > UINT8_MAX - out_crit) ? UINT8_MAX : out_crit + support;
    return (out_crit);
}

int8_t equation_unitSpeed(uint8_t wpn_wgt, uint8_t agi, uint8_t con, uint8_t str) {
    // speed = agi - slowed
    // slowed = max(0, wpn_wgt - con / 2 - str / 4))
    uint8_t slowed = (wpn_wgt > con / 2) ? (wpn_wgt - con / 2) : 0 ;
    slowed = (slowed > str / 4) ? slowed - (str / 4) : 0 ;
    int8_t out_speed = ((slowed - agi) > INT8_MAX) ? INT8_MIN : agi - slowed;
    return (out_speed);
}

int8_t equation_unitDodge(uint8_t wpn_dodge, uint8_t wpn_wgt, uint8_t luck, uint8_t agi, uint8_t str, uint8_t con, uint8_t tile_dodge, uint8_t support) {
    // dodge =  speed + luck + support
    // Dodge can be negative -> weapon equipped too heavy, harder to dodge
    int8_t out_dodge = (tile_dodge > INT8_MAX) ? INT8_MAX : tile_dodge;
    int8_t speed = equation_unitSpeed(wpn_wgt, agi, con, str);
    if (speed > 0) {
        out_dodge = (out_dodge > INT8_MAX - speed) ? INT8_MAX : out_dodge + speed;
    } else {
        out_dodge = (out_dodge < INT8_MIN - speed) ? INT8_MIN : out_dodge + speed;
    }
    out_dodge = (out_dodge > INT8_MAX - luck) ? INT8_MAX : out_dodge + luck;
    out_dodge = (out_dodge > INT8_MAX - wpn_dodge) ? INT8_MAX : out_dodge + wpn_dodge;
    out_dodge = (out_dodge > INT8_MAX - support) ? INT8_MAX : out_dodge + support;
    return (out_dodge);
}

uint8_t equation_unitFavor(uint8_t wpn_favor, uint8_t luck, uint8_t support) {
    // favor = wpn_favor + luck / 2 + support
    uint8_t out_favor = wpn_favor;
    out_favor = ((luck / 2) > UINT8_MAX - out_favor) ? UINT8_MAX : out_favor + luck / 2;
    out_favor = (support > UINT8_MAX - out_favor) ? UINT8_MAX : out_favor + support;
    return (out_favor);
}

uint8_t equation_defense(uint8_t deforres, uint8_t tile_deforres) {
    uint8_t out_def = (deforres > UINT8_MAX - tile_deforres) ? UINT8_MAX : deforres + tile_deforres;
    return (out_def);
}

uint8_t equation_defensevar(size_t argnum, ...) {
    va_list valist;
    uint8_t unit_defend = 0;
    uint8_t current_defend = 0;
    uint32_t current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, uint32_t);
        current_defend = (current_arg > UINT8_MAX) ? UINT8_MAX : current_arg;
        unit_defend = (unit_defend > UINT8_MAX - current_defend) ? UINT8_MAX : current_defend + unit_defend;
    }
    va_end(valist);
    return (unit_defend);
}

uint8_t equation_Unit_Healshp(uint8_t total_hp, uint8_t heal_percent) {
    uint16_t intermediate = (total_hp * heal_percent) / HYAKU_PERCENT;
    uint8_t to_heal = (uint8_t)intermediate;
    return (to_heal);
}

uint8_t equation_attack(uint8_t Lwpn_PorMmight, uint8_t Rwpn_PorMmight) {
    uint8_t wpn_attack = (Lwpn_PorMmight > UINT8_MAX - Rwpn_PorMmight) ? UINT8_MAX : Lwpn_PorMmight + Rwpn_PorMmight;
    return (wpn_attack);
}

uint8_t equation_attackvar(size_t argnum, ...) {
    va_list valist;
    uint8_t wpn_attack = 0;
    uint8_t current_attack = 0;
    uint32_t current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, uint32_t);
        current_attack = (current_arg > UINT8_MAX) ? UINT8_MAX : current_arg;
        wpn_attack = (wpn_attack > UINT8_MAX - current_attack) ? UINT8_MAX : current_attack + wpn_attack;
    }
    va_end(valist);
    return (wpn_attack);
}

uint8_t equation_wpnHit(uint8_t Lwpn_hit, uint8_t Rwpn_hit) {
    uint8_t wpn_hit = 0;
    if ((Rwpn_hit > 0) & (Lwpn_hit > 0)) {
        wpn_hit = Rwpn_hit / 2 + Lwpn_hit / 2;
    } else {
        wpn_hit = Lwpn_hit + Rwpn_hit; // -> which is 0? no idea so add
    }
    return (wpn_hit);
}

uint8_t equation_wpnHitvar(size_t argnum, ...) {
    va_list valist;
    uint8_t wpn_hit = 0;
    uint8_t current_hit = 0;
    uint32_t current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, uint32_t);
        current_hit = (current_arg > UINT8_MAX) ? UINT8_MAX : current_arg;
        wpn_hit += current_hit / argnum;
    }
    va_end(valist);
    return (wpn_hit);
}

uint8_t equation_wpnDodge(uint8_t Lwpn_dodge, uint8_t Rwpn_dodge) {
    uint8_t wpn_dodge = (Rwpn_dodge > UINT8_MAX - Lwpn_dodge) ? UINT8_MAX : Rwpn_dodge + Lwpn_dodge;
    return (wpn_dodge);
}

uint8_t equation_wpnDodgevar(size_t argnum, ...) {
    va_list valist;
    uint8_t wpn_dodge = 0;
    uint8_t current_dodge = 0;
    uint32_t current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, uint32_t);
        current_dodge = (current_arg > UINT8_MAX) ? UINT8_MAX : current_arg;
        wpn_dodge = (wpn_dodge > UINT8_MAX - current_dodge) ? UINT8_MAX : current_dodge + wpn_dodge;
    }
    va_end(valist);
    return (wpn_dodge);
}

uint8_t equation_wpnCrit(uint8_t Lwpn_crit, uint8_t Rwpn_crit) {
    uint8_t wpn_crit = (Rwpn_crit > UINT8_MAX - Lwpn_crit) ? UINT8_MAX : Rwpn_crit + Lwpn_crit;
    return (wpn_crit);
}

uint8_t equation_wpnCritvar(size_t argnum, ...) {
    va_list valist;
    uint8_t wpn_crit = 0;
    uint8_t current_crit = 0;
    uint32_t current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, uint32_t);
        current_crit = (current_arg > UINT8_MAX) ? UINT8_MAX : current_arg;
        wpn_crit = (wpn_crit > UINT8_MAX - current_crit) ? UINT8_MAX : current_crit + wpn_crit;
    }
    va_end(valist);
    return (wpn_crit);
}

uint8_t equation_wpnFavor(uint8_t Lwpn_favor, uint8_t Rwpn_favor) {
    uint8_t wpn_favor = (Rwpn_favor > UINT8_MAX - Lwpn_favor) ? UINT8_MAX : Rwpn_favor + Lwpn_favor;
    return (wpn_favor);
}

uint8_t equation_wpnFavorvar(size_t argnum, ...) {
    va_list valist;
    uint8_t wpn_favor = 0;
    uint8_t current_favor = 0;
    uint32_t current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, uint32_t);
        current_favor = (current_arg > UINT8_MAX) ? UINT8_MAX : current_arg;
        wpn_favor = (wpn_favor > UINT8_MAX - current_favor) ? UINT8_MAX : current_favor + wpn_favor;
    }
    va_end(valist);
    return (wpn_favor);
}

uint8_t equation_wpnWgt(uint8_t Lwpn_wgt, uint8_t Rwpn_wgt) {
    uint8_t wpn_wgt = (Rwpn_wgt > UINT8_MAX - Lwpn_wgt) ? UINT8_MAX : Rwpn_wgt + Lwpn_wgt;
    return (wpn_wgt);
}

uint8_t equation_wpnWgtvar(size_t argnum, ...) {
    va_list valist;
    uint8_t wpn_wgt = 0;
    uint8_t current_wgt = 0;
    uint32_t current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, uint32_t);
        current_wgt = (current_arg > UINT8_MAX) ? UINT8_MAX : current_arg;
        wpn_wgt = (wpn_wgt > UINT8_MAX - current_wgt) ? UINT8_MAX : current_wgt + wpn_wgt;
    }
    va_end(valist);
    return (wpn_wgt);
}

uint8_t equation_Unit_Healshpvar(size_t argnum, ...) {
    // Heal values are in PERCENT.
    va_list valist;
    uint8_t wpn_heal = 0;
    uint8_t current_heal = 0;
    uint32_t current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, uint32_t);
        current_heal = (current_arg > UINT8_MAX) ? UINT8_MAX : current_arg;
        if (wpn_heal < UINT8_MAX - current_heal) {
            if (wpn_heal + current_heal > HYAKU_PERCENT) {
                // Heal portions above 100 get divided by 4
                wpn_heal = (wpn_heal > HYAKU_PERCENT) ? wpn_heal + current_heal / 4 : (wpn_heal + current_heal + 300) / 4;
            } else {
                wpn_heal += current_heal;
            }
        } else {
            wpn_heal = UINT8_MAX;
        }
    }
    va_end(valist);
    return (wpn_heal);
}

uint8_t equation_combatDamage(uint8_t attackerPorMAttack, uint8_t defenderDeforRes, uint8_t effective_multiplier, uint8_t critp_multiplier, bool crit) {
    // critp_multiplier is in percent
    // damage = Attack*crit_factor^crit - defense
    // How should effective damage works?
    uint8_t crit_factor = crit ? critp_multiplier : HYAKU_PERCENT;
    uint16_t attack = (attackerPorMAttack > UINT8_MAX / effective_multiplier * HYAKU_PERCENT) ? UINT8_MAX : effective_multiplier * attackerPorMAttack / HYAKU_PERCENT;
    attack = (attack > UINT8_MAX / crit_factor * HYAKU_PERCENT) ? UINT8_MAX : crit_factor * attack / HYAKU_PERCENT;
    uint8_t damage = (attack < defenderDeforRes) ? 0 : (uint8_t)attack - defenderDeforRes;
    return (damage);
}

struct Unit_stats Unit_stats_plus(struct Unit_stats in_stats1, struct Unit_stats in_stats2) {
    struct Unit_stats out_stats = Unit_stats_default;
    out_stats.hp = (in_stats1.hp > UINT8_MAX - in_stats2.hp) ? UINT8_MAX : in_stats1.hp + in_stats2.hp;
    out_stats.str = (in_stats1.str > UINT8_MAX - in_stats2.str) ? UINT8_MAX : in_stats1.str + in_stats2.str;
    out_stats.mag = (in_stats1.mag > UINT8_MAX - in_stats2.mag) ? UINT8_MAX : in_stats1.mag + in_stats2.mag;
    out_stats.agi = (in_stats1.agi > UINT8_MAX - in_stats2.agi) ? UINT8_MAX : in_stats1.agi + in_stats2.agi;
    out_stats.dex = (in_stats1.dex > UINT8_MAX - in_stats2.dex) ? UINT8_MAX : in_stats1.dex + in_stats2.dex;
    out_stats.luck = (in_stats1.luck > UINT8_MAX - in_stats2.luck) ? UINT8_MAX : in_stats1.luck + in_stats2.luck;
    out_stats.def = (in_stats1.def > UINT8_MAX - in_stats2.def) ? UINT8_MAX : in_stats1.def + in_stats2.def;
    out_stats.res = (in_stats1.res > UINT8_MAX - in_stats2.res) ? UINT8_MAX : in_stats1.res + in_stats2.res;
    out_stats.con = (in_stats1.con > UINT8_MAX - in_stats2.con) ? UINT8_MAX : in_stats1.con + in_stats2.con;
    out_stats.move = (in_stats1.move > UINT8_MAX - in_stats2.move) ? UINT8_MAX : in_stats1.move + in_stats2.move;
    out_stats.prof = (in_stats1.prof > UINT8_MAX - in_stats2.prof) ? UINT8_MAX : in_stats1.prof + in_stats2.prof;
    return (out_stats);
}

struct Unit_stats Unit_stats_minus(struct Unit_stats in_stats1, struct Unit_stats in_stats2) {
    struct Unit_stats out_stats = Unit_stats_default;
    out_stats.hp = (in_stats1.hp < in_stats2.hp) ? 0 : in_stats1.hp - in_stats2.hp;
    out_stats.str = (in_stats1.str < in_stats2.str) ? 0 : in_stats1.str - in_stats2.str;
    out_stats.mag = (in_stats1.mag < in_stats2.mag) ? 0 : in_stats1.mag - in_stats2.mag;
    out_stats.agi = (in_stats1.agi < in_stats2.agi) ? 0 : in_stats1.agi - in_stats2.agi;
    out_stats.dex = (in_stats1.dex < in_stats2.dex) ? 0 : in_stats1.dex - in_stats2.dex;
    out_stats.luck = (in_stats1.luck < in_stats2.luck) ? 0 : in_stats1.luck - in_stats2.luck;
    out_stats.def = (in_stats1.def < in_stats2.def) ? 0 : in_stats1.def - in_stats2.def;
    out_stats.res = (in_stats1.res < in_stats2.res) ? 0 : in_stats1.res - in_stats2.res;
    out_stats.con = (in_stats1.con < in_stats2.con) ? 0 : in_stats1.con - in_stats2.con;
    out_stats.move = (in_stats1.move < in_stats2.move) ? 0 : in_stats1.move - in_stats2.con;
    out_stats.prof = (in_stats1.prof < in_stats2.prof) ? 0 : in_stats1.prof - in_stats2.prof;
    return (out_stats);
}

int8_t equation_AI_Rating_hitRate(uint8_t in_hit_rate) {
    int8_t out_rating = (in_hit_rate / 5);
    return (out_rating); // MAX 20
}

int8_t equation_AI_Rating_critRate(uint8_t in_crit_rate) {
    int8_t out_rating = (in_crit_rate / 3);
    return (out_rating); // MAX 33
}

int8_t equation_AI_Rating_Damage(uint8_t in_dmg) {
    int8_t out_rating = (in_dmg / 3);
    return (out_rating); // MAX 85 but rarely approaches that...
}

int8_t equation_AI_Rating_Stats(uint8_t in_stat) {
    int8_t out_rating = (in_stat / 3);
    return (out_rating); // MAX 85 but rarely approaches that...
}

int8_t equation_AI_Rating_HPprop(float in_hp_prop) {
    int8_t out_rating = (int8_t)((in_hp_prop * 100) / 4);
    return (out_rating); // MAX 85 but rarely approaches that...
}

int8_t equation_AI_Rating_pushGradient(int_tile_t gradient) {
    int8_t out_rating = (int8_t)gradient;
    return (out_rating); // MAX ?? but rarely approaches that...

}

int8_t equation_AI_Rating_pullGradient(int_tile_t gradient) {
    int8_t out_rating = (int8_t)gradient;
    return (out_rating); // MAX ?? but rarely approaches that...
}
