
#include "equations.h"

/* ---EXPECTED DAMAGE COMPUTATION--- */
/* Outputs expected/average damage value when multiplied with (att-def) */
/* Offensive skills should also give expected damage factor values.     */
float Equation_Expected_Damage(int hit, int crit, int critfactor) {
    return (hit / SOTA_100PERCENT * pow((double)critfactor, ((double)crit) / SOTA_100PERCENT));
}

bool Equation_canCarry(int savior_con, int victim_con) {
    return (savior_con > victim_con);
}

i8 Equation_Regrets(int kills, int faith) {
    i8 out = 0;
    int eff_faith = faith > UINT8_MAX ? UINT8_MAX : faith;
    i8 divisor = ((eff_faith < 0 ? 0 : eff_faith) / REGRET_FAITH_FACTOR);
    if (divisor > 0)
        out = kills / divisor;
    return (out);
}

u8 Equation_Agony_PercentonCrit(int luck, int con) {
    int eff_luck    = luck     > UINT8_MAX ? UINT8_MAX : luck;
    u8 agony_prob  = eff_luck <    0      ?     0     : eff_luck;
    agony_prob      = con > SOTA_100PERCENT - agony_prob ? SOTA_100PERCENT : agony_prob + con;
    return (agony_prob);
}

u8 Equation_Agony_Turns(int str, int def, int con) {
    int eff_str         = str     > UINT8_MAX ? UINT8_MAX : str;
    u8 agony_length    = eff_str <     0     ?     0     : eff_str;
    agony_length        = nmath_bplus(agony_length, def, UINT8_MAX);
    agony_length        = nmath_bplus(agony_length, con, UINT8_MAX);
    agony_length       /= AGONY_FACTOR;
    return (agony_length);
}

u8 Equation_Combat_Hit(int att_hit, int dfd_avoid) {

    // int (defender_avoid > UINT8_MAX ? UINT8_MAX : defender_avoid);
    u8 out_hit = nmath_bminus((att_hit > UINT8_MAX ? UINT8_MAX : att_hit), dfd_avoid, 0);
    return (out_hit);
}

u8 Equation_Combat_Crit(int attacker_crit, int defender_favor) {
    u8 eff_crit    = (attacker_crit  > UINT8_MAX ? UINT8_MAX : attacker_crit);
    u8 eff_favor   = (defender_favor > UINT8_MAX ? UINT8_MAX : defender_favor);
    u8 out_crit    = nmath_bminus(eff_crit, eff_favor, 0);
    return (out_crit);
}

u8 Equation_Unit_Hit(int wpn_hit, int dex, int luck, int support) {
    // hit = wpn_hit + dex*2 + luck/2 + support
    int eff_hit = wpn_hit > UINT8_MAX ? UINT8_MAX : wpn_hit;
    u8 out_hit = eff_hit <     0     ?     0     : eff_hit;
    u8 effdex  = nmath_bmultp(dex,    HIT_DEX_FACTOR,          UINT8_MAX);
    out_hit     = nmath_bplus(out_hit, effdex,                  UINT8_MAX);
    out_hit     = nmath_bplus(out_hit, luck / HIT_LUCK_FACTOR,  UINT8_MAX);
    out_hit     = nmath_bplus(out_hit, support,                 UINT8_MAX);
    return (out_hit);
}

u8 Equation_Unit_Crit(int wpn_crit, int dex, int luck, int support) {
    // favor = wpn_crit + dex / 3 + luck / 4 + support
    int eff_crit = wpn_crit > UINT8_MAX ? UINT8_MAX : wpn_crit;
    u8 out_crit = eff_crit <    0      ?    0      : eff_crit;
    out_crit     = nmath_bplus(out_crit, (luck / CRIT_LUCK_FACTOR), UINT8_MAX);
    out_crit     = nmath_bplus(out_crit, (dex  / CRIT_DEX_FACTOR),  UINT8_MAX);
    out_crit     = nmath_bplus(out_crit, support,                   UINT8_MAX);
    return (out_crit);
}

i8 Equation_Unit_Speed(int wpn_wgt, int agi, int con, int str) {
    // speed = agi - slowed
    // slowed = max(0, wpn_wgt - con / 2 - str / 4))
    int eff_wgt   = wpn_wgt > UINT8_MAX ? UINT8_MAX : wpn_wgt;
    u8 slowed    = eff_wgt <     0     ?     0     : eff_wgt;
    slowed        = nmath_bminus(slowed, (con / SPEED_CON_FACTOR), 0);
    slowed        = nmath_bminus(slowed, (str / SPEED_STR_FACTOR), 0);
    i8 out_speed  = nmath_bminus(agi,    slowed,                   INT8_MIN);
    return (out_speed);
}

i8 Equation_Unit_Dodge(int wpn_wgt, int wpn_dodge, int luck, int faith,
                       int agi, int str, int con, int tile_dodge, int support) {
    // dodge = agi + faith/2 + luck/2 + support + tile - con/2 - slowed
    // slowed = max(0, wpn_wgt - str / 4))
    // Dodge can be negative -> weapon equipped too heavy, LITERALLY TOO BULKY TO DODGE

    int eff_wgt = wpn_wgt > UINT8_MAX ? UINT8_MAX : wpn_wgt;
    u8 slowed  = eff_wgt <     0     ?     0     : eff_wgt;
    slowed      = nmath_bminus(slowed, (str / DODGE_STR_FACTOR), 0);

    i8 out_dodge = (tile_dodge > INT8_MAX) ? INT8_MAX : tile_dodge;
    out_dodge = nmath_bminus(out_dodge, slowed,                      INT8_MIN);
    out_dodge = nmath_bminus(out_dodge, (con   / DODGE_CON_FACTOR),  INT8_MIN);

    out_dodge = nmath_bplus(out_dodge, (agi   / DODGE_AGI_FACTOR),  INT8_MAX);
    out_dodge = nmath_bplus(out_dodge, (luck  / DODGE_LUCK_FACTOR), INT8_MAX);
    out_dodge = nmath_bplus(out_dodge, (faith / DODGE_FTH_FACTOR),  INT8_MAX);
    out_dodge = nmath_bplus(out_dodge,  wpn_dodge,                   INT8_MAX);
    out_dodge = nmath_bplus(out_dodge,  support,                     INT8_MAX);
    return (out_dodge);
}

u8 Equation_Unit_Favor(int wpn_favor, int faith, int support) {
    // favor = wpn_favor + faith / 2 + support
    int eff_favor = wpn_favor > UINT8_MAX ? UINT8_MAX : wpn_favor;
    u8 out_favor = eff_favor <     0     ?     0     : eff_favor;
    out_favor = nmath_bplus(out_favor, (faith / FAVOR_FTH_FACTOR), UINT8_MAX);
    out_favor = nmath_bplus(out_favor, support,                    UINT8_MAX);
    return (out_favor);
}

// prot -> protection which is def/res
u8 Equation_Weapon_defense(int prot, int tile_prot) {
    u8 out_def = nmath_bplus(prot, tile_prot, UINT8_MAX);
    return (out_def);
}

u8 Equation_Weapon_Defensevar(size_t argnum, ...) {
    va_list valist;
    u8 unit_defend = 0;
    int current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, int);
        unit_defend = nmath_bplus(unit_defend, current_arg, UINT8_MAX);
    }
    va_end(valist);
    return (unit_defend);
}

/* --- Equation_Staff_Healing --- */
/* Healing value of unit equipping a staff */
/* Should staves flat heal, or % heal? */
u8 Equation_Staff_Healing(int item_AP, int user_mag) {
    int intermediate = item_AP + user_mag < 0 ? 0 : item_AP + user_mag;
    if (intermediate > UINT8_MAX)
        intermediate = UINT8_MAX;
    u8 to_heal = (u8)intermediate;
    return (to_heal);
}

/* --- Equation_Unit_Healshp --- */
/* Unit healing percent % -> HP */
u8 Equation_Unit_Healshp(int total_hp, int heal_percent) {
    u16 intermediate = (total_hp * heal_percent) / SOTA_100PERCENT;
    u8 to_heal = (u8)intermediate;
    return (to_heal);
}

u8 Equation_Weapon_Attack(int Lwpn_might, int Rwpn_might) {
    u8 wpn_attack = nmath_bplus(Lwpn_might, Rwpn_might, UINT8_MAX);
    return (wpn_attack);
}

void Equation_Damage_Total(struct Damage *dmg) {
    dmg->dmg[3]      = nmath_bplus(dmg->dmg[0],      dmg->dmg[1],      UINT8_MAX);
    dmg->dmg[3]      = nmath_bplus(dmg->dmg[3],      dmg->dmg[2],      UINT8_MAX);
    dmg->dmg_crit[3] = nmath_bplus(dmg->dmg_crit[0], dmg->dmg_crit[1], UINT8_MAX);
    dmg->dmg_crit[3] = nmath_bplus(dmg->dmg_crit[3], dmg->dmg_crit[2], UINT8_MAX);
}

u8 Equation_Weapon_Attackvar(size_t argnum, ...) {
    va_list valist;
    u8 wpn_attack = 0;
    int current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, int);
        wpn_attack = nmath_bplus(wpn_attack, current_arg, UINT8_MAX);
    }
    va_end(valist);
    return (wpn_attack);
}

u8 Equation_Weapon_Hit(int Lwpn_hit, int Rwpn_hit) {
    u16 wpn_hit = Lwpn_hit + Rwpn_hit;
    if ((Rwpn_hit > 0) && (Lwpn_hit > 0))
        wpn_hit /= 2;
    return (wpn_hit);
}

u8 Equation_Weapon_Hitvar(size_t argnum, ...) {
    va_list valist;
    u8 wpn_hit = 0;
    int current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, int);
        wpn_hit = nmath_bplus(wpn_hit, current_arg, UINT8_MAX);
    }
    va_end(valist);
    return (wpn_hit);
}

u8 Equation_Weapon_Dodge(int Lwpn_dodge, int Rwpn_dodge) {
    u8 wpn_dodge = nmath_bplus(Lwpn_dodge, Rwpn_dodge, UINT8_MAX);
    return (wpn_dodge);
}

u8 Equation_Weapon_Dodgevar(size_t argnum, ...) {
    va_list valist;
    u8 wpn_dodge = 0;
    int current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, int);
        wpn_dodge = nmath_bplus(wpn_dodge, (u8)current_arg, UINT8_MAX);
    }
    va_end(valist);
    return (wpn_dodge);
}

u8 Equation_Weapon_Crit(int Lwpn_crit, int Rwpn_crit) {
    u8 wpn_crit = nmath_bplus(Lwpn_crit, Rwpn_crit, UINT8_MAX);
    return (wpn_crit);
}

u8 Equation_Weapon_Critvar(size_t argnum, ...) {
    va_list valist;
    u8 wpn_crit = 0;
    int current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, int);
        wpn_crit = nmath_bplus(wpn_crit, (u8)current_arg, UINT8_MAX);
    }
    va_end(valist);
    return (wpn_crit);
}

u8 Equation_Weapon_Favor(int Lwpn_favor, int Rwpn_favor) {
    u8 wpn_favor = nmath_bplus(Lwpn_favor, Rwpn_favor, UINT8_MAX);
    return (wpn_favor);
}

u8 Equation_Weapon_Favorvar(size_t argnum, ...) {
    va_list valist;
    u8 wpn_favor = 0;
    int current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, int);
        wpn_favor = nmath_bplus(wpn_favor, (u8)current_arg, UINT8_MAX);
    }
    va_end(valist);
    return (wpn_favor);
}

u8 Equation_Weapon_Wgt(int Lwpn_wgt, int Rwpn_wgt) {
    u8 wpn_wgt = nmath_bplus(Lwpn_wgt, Rwpn_wgt, UINT8_MAX);
    return (wpn_wgt);
}

u8 Equation_Weapon_Wgtvar(size_t argnum, ...) {
    va_list valist;
    u8 wpn_wgt = 0;
    int current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, int);
        wpn_wgt = nmath_bplus(wpn_wgt, (u8)current_arg, UINT8_MAX);
    }
    va_end(valist);
    return (wpn_wgt);
}

u8 Equation_Unit_Healshpvar(size_t argnum, ...) {
    va_list valist;
    u8 wpn_heal = 0;
    int current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, int);
        wpn_heal = nmath_bplus(wpn_heal, (u8)current_arg, UINT8_MAX);
    }
    // Heal portions above 100 get divided by 4
    if (wpn_heal > SOTA_100PERCENT)
        wpn_heal = (wpn_heal - SOTA_100PERCENT) / OVERHEAL_FACTOR  + SOTA_100PERCENT;
    va_end(valist);
    return (wpn_heal);
}

/* --  Equation_Combat_Damage -- */
//  Arguments:
//   1. att is attacker attack:       unit str + wpn might
//   2. def is defender defense:      unit def + wpn prot
//   3. effective_multiplier  is in percent e.g. 200 for double damage
//   4. critp_multiplier      is in percent e.g. 200 for double damage
u8 Equation_Combat_Damage(int att, int defender_block,
                          int effective_multiplier, int critp_multiplier, bool crit) {
    /* damage = Attack*crit_factor^crit - defense */
    // DESIGN QUESTION: Should effective triple weapon might? or attack damage?
    // I'm thinking about doubling ATTACK DAMAGE, before removing defense?
    SDL_assert(att >= 0);
    u8 crit_factor = crit ? critp_multiplier : SOTA_100PERCENT;
    u16 attack = (att * effective_multiplier * crit_factor) /
                 (SOTA_100PERCENT * SOTA_100PERCENT);
    attack = attack > UINT8_MAX ? UINT8_MAX : attack;
    u8 damage = nmath_bminus(attack, defender_block, 0);
    return (damage);
}

/* --  Equation_Attack_Damage -- */
/* Basic attack damage equation with no effective, no crit */
u8 Equation_Attack_Damage(int attacker_dmg, int defender_def) {
    return (Equation_Combat_Damage(attacker_dmg, defender_def, 100, 100, false));
}

u8 Stat_Total(int current, int bonus, int malus, int cap) {
    u8 total = nmath_bplus(current, bonus, UINT8_MAX);
    total = nmath_bminus(current, malus, 0);
    total = (total > cap) ? cap : total;
    return (total);
}

i8 Equation_AI_Rating_hitRate(int in_hit_rate) {
    i8 out_rating = (in_hit_rate / 5);
    return (out_rating); // MAX 20
}

i8 Equation_AI_Rating_critRate(int in_crit_rate) {
    i8 out_rating = (in_crit_rate / 3);
    return (out_rating); // MAX 33
}

i8 Equation_AI_Rating_Damage(int in_dmg) {
    i8 out_rating = (in_dmg / 3);
    return (out_rating); /* MAX 85 but rarely approaches that... */
}

i8 Equation_AI_Rating_Stats(int in_stat) {
    i8 out_rating = (in_stat / 3);

    return (out_rating);  /* MAX 85 but rarely approaches that... */
}

i8 Equation_AI_Rating_HPprop(float in_hp_prop) {
    i8 out_rating = (i8)((in_hp_prop * 100) / 4);
    return (out_rating); /* MAX 85 but rarely approaches that... */
}

i8 Equation_AI_Rating_pushGradient(i32 gradient) {
    i8 out_rating = (i8)gradient;
    return (out_rating); // MAX ?? but rarely approaches that...
}

i8 Equation_AI_Rating_pullGradient(i32 gradient) {
    i8 out_rating = (i8)gradient;
    return (out_rating); // MAX ?? but rarely approaches that...
}
