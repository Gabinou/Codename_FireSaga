
#include "equations.h"

/* ---EXPECTED DAMAGE COMPUTATION--- */
/* Outputs expected/average damage value when multiplied with (att-def) */
/* Offensive skills should also give expected damage factor values.     */
float Equation_Expected_Damage(i32 hit, i32 crit, i32 critfactor) {
    return (hit / SOTA_100PERCENT * pow((double)critfactor, ((double)crit) / SOTA_100PERCENT));
}

bool Equation_canCarry(i32 savior_con, i32 victim_con) {
    return (savior_con > victim_con);
}

/* TODO: change to SOTA values*/
i32 Equation_Regrets(i32 kills, i32 faith) {
    i32 out = 0;
    i32 eff_faith = faith > UINT8_MAX ? UINT8_MAX : faith;
    i32 divisor = ((eff_faith < 0 ? 0 : eff_faith) / REGRET_FAITH_FACTOR);
    if (divisor > 0)
        out = kills / divisor;

    out = nmath_inbounds_int32_t(out, REGRET_MIN, REGRET_MAX);
    return (out);
}

i32 Equation_Agony_PercentonCrit(i32 luck, i32 con) {
    i32 eff_luck    = luck + con;
    i32 agony_prob  = nmath_inbounds_int32_t(agony_prob, 0, SOTA_100PERCENT);
    return (agony_prob);
}

i32 Equation_Agony_Turns(i32 str, i32 def, i32 con, i32 bonus) {
    i32 agony_length     = str + def + con;
    agony_length        /= AGONY_FACTOR + bonus;
    agony_length         = nmath_inbounds_int32_t(agony_length, SOTA_MIN_AGONY, SOTA_MAX_AGONY);
    return (agony_length);
}

i32 Equation_Combat_Hit(i32 att_hit, i32 dfd_avoid) {
    i32 out_hit = att_hit - dfd_avoid;
    out_hit     = nmath_inbounds_int32_t(out_hit, 0, SOTA_100PERCENT);
    return (out_hit);
}

i32 Equation_Combat_Crit(i32 attacker_crit, i32 defender_favor) {
    i32 out_crit    = attacker_crit - defender_favor;
    out_crit        = nmath_inbounds_int32_t(out_crit, 0, SOTA_100PERCENT);
    return (out_crit);
}

i32 Equation_Unit_Hit(i32 wpn_hit, i32 dex, i32 luck, i32 bonus) {
    // hit = wpn_hit + dex*2 + luck/2 + bonus
    i32 eff_hit = wpn_hit > UINT8_MAX ? UINT8_MAX : wpn_hit;
    i32 out_hit = eff_hit <     0     ?     0     : eff_hit;
    i32 effdex  = nmath_bmultp(dex,    HIT_DEX_FACTOR,          UINT8_MAX);
    out_hit     = nmath_bplus(out_hit, effdex,                  UINT8_MAX);
    out_hit     = nmath_bplus(out_hit, luck / HIT_LUCK_FACTOR,  UINT8_MAX);
    out_hit     = nmath_bplus(out_hit, bonus,                 UINT8_MAX);
    return (out_hit);
}

i32 Equation_Unit_Crit(i32 wpn_crit, i32 dex, i32 luck, i32 bonus) {
    // favor = wpn_crit + dex / 3 + luck / 4 + bonus
    i32 eff_crit = wpn_crit > UINT8_MAX ? UINT8_MAX : wpn_crit;
    i32 out_crit = eff_crit <    0      ?    0      : eff_crit;
    out_crit     = nmath_bplus(out_crit, (luck / CRIT_LUCK_FACTOR), UINT8_MAX);
    out_crit     = nmath_bplus(out_crit, (dex  / CRIT_DEX_FACTOR),  UINT8_MAX);
    out_crit     = nmath_bplus(out_crit, bonus,                   UINT8_MAX);
    return (out_crit);
}

i32 Equation_Unit_Speed(i32 wpn_wgt, i32 agi, i32 con, i32 str, i32 bonus) {
    // speed = agi - slowed
    // slowed = max(0, wpn_wgt - con / 2 - str / 4))
    i32 eff_wgt     = wpn_wgt > UINT8_MAX ? UINT8_MAX : wpn_wgt;
    i32 slowed      = eff_wgt <     0     ?     0     : eff_wgt;
    slowed          = nmath_bminus(slowed, (con / SPEED_CON_FACTOR), 0);
    slowed          = nmath_bminus(slowed, (str / SPEED_STR_FACTOR), 0);
    i32 out_speed   = nmath_bminus(agi,    slowed, INT8_MIN);
    + bonus;
    return (out_speed);
}

i32 Equation_Unit_Dodge(i32 wpn_wgt, i32 wpn_dodge, i32 luck, i32 faith,
                        i32 agi, i32 str, i32 con, i32 tile_dodge, i32 bonus) {
    // dodge = agi + faith/2 + luck/2 + bonus + tile - con/2 - slowed
    // slowed = max(0, wpn_wgt - str / 4))
    // Dodge can be negative -> weapon equipped too heavy, LITERALLY TOO BULKY TO DODGE

    i32 eff_wgt = wpn_wgt > UINT8_MAX ? UINT8_MAX : wpn_wgt;
    i32 slowed  = eff_wgt <     0     ?     0     : eff_wgt;
    slowed      = nmath_bminus(slowed, (str / DODGE_STR_FACTOR), 0);

    i32 out_dodge = (tile_dodge > INT8_MAX) ? INT8_MAX : tile_dodge;
    out_dodge = nmath_bminus(out_dodge, slowed,                      INT8_MIN);
    out_dodge = nmath_bminus(out_dodge, (con   / DODGE_CON_FACTOR),  INT8_MIN);

    out_dodge = nmath_bplus(out_dodge, (agi   / DODGE_AGI_FACTOR),  INT8_MAX);
    out_dodge = nmath_bplus(out_dodge, (luck  / DODGE_LUCK_FACTOR), INT8_MAX);
    out_dodge = nmath_bplus(out_dodge, (faith / DODGE_FTH_FACTOR),  INT8_MAX);
    out_dodge = nmath_bplus(out_dodge,  wpn_dodge,                   INT8_MAX);
    out_dodge = nmath_bplus(out_dodge,  bonus,                     INT8_MAX);
    return (out_dodge);
}

i32 Equation_Unit_Favor(i32 wpn_favor, i32 faith, i32 bonus) {
    // favor = wpn_favor + faith / 2 + bonus
    i32 eff_favor = wpn_favor > UINT8_MAX ? UINT8_MAX : wpn_favor;
    i32 out_favor = eff_favor <     0     ?     0     : eff_favor;
    out_favor = nmath_bplus(out_favor, (faith / FAVOR_FTH_FACTOR), UINT8_MAX);
    out_favor = nmath_bplus(out_favor, bonus,                    UINT8_MAX);
    return (out_favor);
}

// prot -> protection which is def/res
i32 Equation_Weapon_Defense(i32 prot, i32 tile_prot) {
    i32 out_def = nmath_bplus(prot, tile_prot, UINT8_MAX);
    return (out_def);
}

i32 Equation_Weapon_Defensevar(size_t argnum, ...) {
    va_list valist;
    i32 unit_defend = 0;
    i32 current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, i32);
        unit_defend = nmath_bplus(unit_defend, current_arg, UINT8_MAX);
    }
    va_end(valist);
    return (unit_defend);
}

/* --- Equation_Staff_Healing --- */
/* Healing value of unit equipping a staff */
/* Should staves flat heal, or % heal? */
i32 Equation_Staff_Healing(i32 item_AP, i32 user_mag) {
    i32 intermediate = item_AP + user_mag < 0 ? 0 : item_AP + user_mag;
    if (intermediate > UINT8_MAX)
        intermediate = UINT8_MAX;
    i32 to_heal = (i32)intermediate;
    return (to_heal);
}

/* --- Equation_Unit_Healshp --- */
/* Unit healing percent % -> HP */
i32 Equation_Unit_Healshp(i32 total_hp, i32 heal_percent) {
    u16 intermediate = (total_hp * heal_percent) / SOTA_100PERCENT;
    i32 to_heal = (i32)intermediate;
    return (to_heal);
}

i32 Equation_Weapon_Attack(i32 Lwpn_might, i32 Rwpn_might) {
    i32 wpn_attack = nmath_bplus(Lwpn_might, Rwpn_might, UINT8_MAX);
    return (wpn_attack);
}

void Equation_Damage_Total(struct Damage *dmg) {
    dmg->dmg[3]      = nmath_bplus(dmg->dmg[0],      dmg->dmg[1],      UINT8_MAX);
    dmg->dmg[3]      = nmath_bplus(dmg->dmg[3],      dmg->dmg[2],      UINT8_MAX);
    dmg->dmg_crit[3] = nmath_bplus(dmg->dmg_crit[0], dmg->dmg_crit[1], UINT8_MAX);
    dmg->dmg_crit[3] = nmath_bplus(dmg->dmg_crit[3], dmg->dmg_crit[2], UINT8_MAX);
}

i32 Equation_Weapon_Attackvar(size_t argnum, ...) {
    va_list valist;
    i32 wpn_attack = 0;
    i32 current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, i32);
        wpn_attack = nmath_bplus(wpn_attack, current_arg, UINT8_MAX);
    }
    va_end(valist);
    return (wpn_attack);
}

i32 Equation_Weapon_Hit(i32 Lwpn_hit, i32 Rwpn_hit) {
    u16 wpn_hit = Lwpn_hit + Rwpn_hit;
    if ((Rwpn_hit > 0) && (Lwpn_hit > 0))
        wpn_hit /= 2;
    return (wpn_hit);
}

i32 Equation_Weapon_Hitvar(size_t argnum, ...) {
    va_list valist;
    i32 wpn_hit = 0;
    i32 current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, i32);
        wpn_hit = nmath_bplus(wpn_hit, current_arg, UINT8_MAX);
    }
    va_end(valist);
    return (wpn_hit);
}

i32 Equation_Weapon_Dodge(i32 Lwpn_dodge, i32 Rwpn_dodge) {
    i32 wpn_dodge = nmath_bplus(Lwpn_dodge, Rwpn_dodge, UINT8_MAX);
    return (wpn_dodge);
}

i32 Equation_Weapon_Dodgevar(size_t argnum, ...) {
    va_list valist;
    i32 wpn_dodge = 0;
    i32 current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, i32);
        wpn_dodge = nmath_bplus(wpn_dodge, (i32)current_arg, UINT8_MAX);
    }
    va_end(valist);
    return (wpn_dodge);
}

i32 Equation_Weapon_Crit(i32 Lwpn_crit, i32 Rwpn_crit) {
    i32 wpn_crit = nmath_bplus(Lwpn_crit, Rwpn_crit, UINT8_MAX);
    return (wpn_crit);
}

i32 Equation_Weapon_Critvar(size_t argnum, ...) {
    va_list valist;
    i32 wpn_crit = 0;
    i32 current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, i32);
        wpn_crit = nmath_bplus(wpn_crit, (i32)current_arg, UINT8_MAX);
    }
    va_end(valist);
    return (wpn_crit);
}

i32 Equation_Weapon_Favor(i32 Lwpn_favor, i32 Rwpn_favor) {
    i32 wpn_favor = nmath_bplus(Lwpn_favor, Rwpn_favor, UINT8_MAX);
    return (wpn_favor);
}

i32 Equation_Weapon_Favorvar(size_t argnum, ...) {
    va_list valist;
    i32 wpn_favor = 0;
    i32 current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, i32);
        wpn_favor = nmath_bplus(wpn_favor, (i32)current_arg, UINT8_MAX);
    }
    va_end(valist);
    return (wpn_favor);
}

i32 Equation_Weapon_Wgt(i32 Lwpn_wgt, i32 Rwpn_wgt) {
    i32 wpn_wgt = nmath_bplus(Lwpn_wgt, Rwpn_wgt, UINT8_MAX);
    return (wpn_wgt);
}

i32 Equation_Weapon_Wgtvar(size_t argnum, ...) {
    va_list valist;
    i32 wpn_wgt = 0;
    i32 current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, i32);
        wpn_wgt = nmath_bplus(wpn_wgt, (i32)current_arg, UINT8_MAX);
    }
    va_end(valist);
    return (wpn_wgt);
}

i32 Equation_Unit_Healshpvar(size_t argnum, ...) {
    va_list valist;
    i32 wpn_heal = 0;
    i32 current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, i32);
        wpn_heal = nmath_bplus(wpn_heal, (i32)current_arg, UINT8_MAX);
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
i32 Equation_Combat_Damage(i32 att, i32 defender_block,
                           i32 effective_multiplier, i32 critp_multiplier, bool crit) {
    /* damage = Attack*crit_factor^crit - defense */
    // DESIGN QUESTION: Should effective triple weapon might? or attack damage?
    // I'm thinking about doubling ATTACK DAMAGE, before removing defense?
    SDL_assert(att >= 0);
    i32 crit_factor = crit ? critp_multiplier : SOTA_100PERCENT;
    u16 attack = (att * effective_multiplier * crit_factor) /
                 (SOTA_100PERCENT * SOTA_100PERCENT);
    attack = attack > UINT8_MAX ? UINT8_MAX : attack;
    i32 damage = nmath_bminus(attack, defender_block, 0);
    return (damage);
}

/* --  Equation_Attack_Damage -- */
/* Basic attack damage equation with no effective, no crit */
i32 Equation_Attack_Damage(i32 attacker_dmg, i32 defender_def) {
    return (Equation_Combat_Damage(attacker_dmg, defender_def, 100, 100, false));
}

i32 Stat_Total(i32 current, i32 bonus, i32 malus, i32 cap) {
    i32 total = nmath_bplus(current, bonus, UINT8_MAX);
    total = nmath_bminus(current, malus, 0);
    total = (total > cap) ? cap : total;
    return (total);
}

i32 Equation_AI_Rating_hitRate(i32 in_hit_rate) {
    i32 out_rating = (in_hit_rate / 5);
    return (out_rating); // MAX 20
}

i32 Equation_AI_Rating_critRate(i32 in_crit_rate) {
    i32 out_rating = (in_crit_rate / 3);
    return (out_rating); // MAX 33
}

i32 Equation_AI_Rating_Damage(i32 in_dmg) {
    i32 out_rating = (in_dmg / 3);
    return (out_rating); /* MAX 85 but rarely approaches that... */
}

i32 Equation_AI_Rating_Stats(i32 in_stat) {
    i32 out_rating = (in_stat / 3);

    return (out_rating);  /* MAX 85 but rarely approaches that... */
}

i32 Equation_AI_Rating_HPprop(float in_hp_prop) {
    i32 out_rating = (i32)((in_hp_prop * 100) / 4);
    return (out_rating); /* MAX 85 but rarely approaches that... */
}

i32 Equation_AI_Rating_pushGradient(i32 gradient) {
    i32 out_rating = (i32)gradient;
    return (out_rating); // MAX ?? but rarely approaches that...
}

i32 Equation_AI_Rating_pullGradient(i32 gradient) {
    i32 out_rating = (i32)gradient;
    return (out_rating); // MAX ?? but rarely approaches that...
}
