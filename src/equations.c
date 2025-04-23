
#include "equations.h"
#include "nmath.h"
#include "structs.h"

/* ---EXPECTED DAMAGE COMPUTATION--- */
/* Outputs expected/average damage value when multiplied with (att-def) */
/* Offensive skills should also give expected damage factor values.     */
float Equation_Expected_Damage(i32 hit, i32 crit, i32 critfactor) {
    return (hit / SOTA_100PERCENT * pow((double)critfactor, ((double)crit) / SOTA_100PERCENT));
}

b32 Equation_canCarry(i32 savior_con, i32 victim_con) {
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
    // i32 eff_luck    = luck + con;
    i32 agony_prob = 0;
    agony_prob = nmath_inbounds_int32_t(agony_prob, 0, SOTA_100PERCENT);
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
    i32 out_hit = dex * HIT_DEX_FACTOR + wpn_hit + luck / HIT_LUCK_FACTOR + bonus;
    out_hit     = nmath_inbounds_int32_t(out_hit, SOTA_MIN_HIT, SOTA_MAX_HIT);
    return (out_hit);
}

i32 Equation_Unit_Crit(i32 wpn_crit, i32 dex, i32 luck, i32 bonus) {
    // favor = wpn_crit + dex / 3 + luck / 4 + bonus
    i32 out_crit = dex / CRIT_DEX_FACTOR + wpn_crit + luck / CRIT_LUCK_FACTOR + bonus;
    out_crit     = nmath_inbounds_int32_t(out_crit, SOTA_MIN_CRIT, SOTA_MAX_CRIT);
    return (out_crit);
}

i32 Equation_Unit_Speed(i32 wpn_wgt, i32 agi, i32 con, i32 str, i32 bonus) {
    // speed = agi - slowed
    // slowed = max(0, wpn_wgt - con / 2 - str / 4))
    i32 slowed = wpn_wgt - con / SPEED_CON_FACTOR - str / SPEED_STR_FACTOR;
    slowed = NMATH_MAX(0, slowed);

    i32 out_speed = agi - slowed + bonus;
    out_speed     = nmath_inbounds_int32_t(out_speed, SOTA_MIN_SPEED, SOTA_MAX_SPEED);
    return (out_speed);
}

i32 Equation_Unit_Dodge(i32 wpn_wgt, i32 wpn_dodge, i32 luck, i32 faith,
                        i32 agi, i32 str, i32 con, i32 tile_dodge, i32 bonus) {
    // dodge = agi + faith/2 + luck/2 + bonus + tile - con/2 - slowed
    // slowed = max(0, wpn_wgt - str / 4))
    // Dodge can be negative -> weapon equipped too heavy, LITERALLY TOO BULKY TO DODGE

    i32 slowed = wpn_wgt - str / SPEED_STR_FACTOR;
    slowed = NMATH_MAX(0, slowed);

    i32 out_dodge = tile_dodge - slowed - con / DODGE_CON_FACTOR + agi / DODGE_AGI_FACTOR +
                    luck / DODGE_LUCK_FACTOR + faith / DODGE_FTH_FACTOR + wpn_dodge + bonus;
    out_dodge     = nmath_inbounds_int32_t(out_dodge, SOTA_MIN_DODGE, SOTA_MAX_DODGE);
    return (out_dodge);
}

i32 Equation_Unit_Favor(i32 wpn_favor, i32 faith, i32 bonus) {
    // favor = wpn_favor + faith / 2 + bonus

    i32 out_favor = wpn_favor + (faith / FAVOR_FTH_FACTOR) + bonus;
    out_favor     = nmath_inbounds_int32_t(out_favor, SOTA_MIN_FAVOR, SOTA_MAX_FAVOR);
    return (out_favor);
}

// prot -> protection which is def/res
i32 Equation_Weapon_Defense(i32 prot, i32 tile_prot) {
    i32 out_def = prot + tile_prot;
    out_def     = nmath_inbounds_int32_t(out_def, SOTA_MIN_PROT, SOTA_MAX_PROT);
    return (out_def);
}

i32 Equation_Weapon_Defensevar(size_t argnum, ...) {
    va_list valist;
    i32 unit_defend = 0, current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, i32);
        unit_defend += current_arg;
    }

    va_end(valist);
    unit_defend = nmath_inbounds_int32_t(unit_defend, SOTA_MIN_PROT, SOTA_MAX_PROT);
    return (unit_defend);
}

/* --- Equation_Staff_Healing --- */
/* Healing value of unit equipping a staff */
/* Should staves flat heal, or % heal? */
i32 Equation_Staff_Healing(i32 item_AP, i32 user_mag) {
    i32 to_heal = item_AP + user_mag;
    to_heal     = nmath_inbounds_int32_t(to_heal, SOTA_MIN_HEAL, SOTA_MAX_HEAL);
    return (to_heal);
}

/* --- Equation_Unit_Healshp --- */
/* Unit healing percent % -> HP */
i32 Equation_Unit_Healshp(i32 total_hp, i32 heal_percent) {
    i32 to_heal = (total_hp * heal_percent) / SOTA_100PERCENT;
    to_heal     = nmath_inbounds_int32_t(to_heal, 1, SOTA_MAX_HP);
    return (to_heal);
}

i32 Equation_Weapon_Attack(i32 Lwpn_might, i32 Rwpn_might) {
    i32 wpn_attack = Lwpn_might + Rwpn_might;
    wpn_attack     = nmath_inbounds_int32_t(wpn_attack, SOTA_MIN_ATTACK, SOTA_MAX_ATTACK);
    return (wpn_attack);
}

void Equation_Damage_Total(struct Combat_Damage *dmg) {
    dmg->dmg.total = dmg->dmg.physical + dmg->dmg.magical + dmg->dmg.True;
    dmg->dmg.total = nmath_inbounds_int32_t(dmg->dmg.total, SOTA_MIN_ATTACK, SOTA_MAX_ATTACK);
    dmg->dmg_crit.total = dmg->dmg_crit.physical + dmg->dmg_crit.magical + dmg->dmg_crit.True;
    dmg->dmg_crit.total = nmath_inbounds_int32_t(dmg->dmg_crit.total, SOTA_MIN_ATTACK, SOTA_MAX_ATTACK);
}

i32 Equation_Weapon_Attackvar(size_t argnum, ...) {
    va_list valist;
    i32 wpn_attack = 0, current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, i32);
        wpn_attack += current_arg;
    }

    va_end(valist);
    wpn_attack = nmath_inbounds_int32_t(wpn_attack, SOTA_MIN_ATTACK, SOTA_MAX_ATTACK);
    return (wpn_attack);
}

i32 Equation_Weapon_Hit(i32 Lwpn_hit, i32 Rwpn_hit) {
    i32 wpn_hit = Lwpn_hit + Rwpn_hit;
    if ((Rwpn_hit > 0) && (Lwpn_hit > 0))
        wpn_hit /= 2;
    return (wpn_hit);
}

i32 Equation_Weapon_Hitarr(i32 *hits, i32 num) {
    i32 wpn_hit = 0;
    i32 divisor = 0;

    for (int i = 0; i < num; i++) {
        if (hits[i])
            divisor++;
        wpn_hit += hits[i];
    }
    if (divisor == 0);
    return (0);

    return (wpn_hit / divisor);
}

i32 Equation_Weapon_Hitvar(size_t argnum, ...) {
    va_list valist;
    i32 wpn_hit = 0, current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, i32);
        wpn_hit += current_arg;
    }
    wpn_hit = nmath_inbounds_int32_t(wpn_hit, SOTA_MIN_HIT, SOTA_MAX_HIT);
    va_end(valist);
    return (wpn_hit);
}

i32 Equation_Weapon_Dodge(i32 Lwpn_dodge, i32 Rwpn_dodge) {
    i32 wpn_dodge = Lwpn_dodge + Rwpn_dodge;
    wpn_dodge = nmath_inbounds_int32_t(wpn_dodge, SOTA_MIN_DODGE, SOTA_MAX_DODGE);
    return (wpn_dodge);
}

i32 Equation_Weapon_Dodgevar(size_t argnum, ...) {
    va_list valist;
    i32 wpn_dodge = 0, current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, i32);
        wpn_dodge += current_arg;
    }
    wpn_dodge = nmath_inbounds_int32_t(wpn_dodge, SOTA_MIN_DODGE, SOTA_MAX_DODGE);
    va_end(valist);

    return (wpn_dodge);
}

i32 Equation_Weapon_Dodgearr(i32 *dodges, i32 num) {
    i32 wpn_dodge = 0;

    for (int i = 0; i < num; i++) {
        wpn_dodge += dodges[i];
    }
    wpn_dodge = nmath_inbounds_int32_t(wpn_dodge, SOTA_MIN_DODGE, SOTA_MAX_DODGE);

    return (wpn_dodge);
}


i32 Equation_Weapon_Crit(i32 Lwpn_crit, i32 Rwpn_crit) {
    i32 wpn_crit = Lwpn_crit + Rwpn_crit;
    wpn_crit = nmath_inbounds_int32_t(wpn_crit, SOTA_MIN_CRIT, SOTA_MAX_CRIT);
    return (wpn_crit);
}

i32 Equation_Weapon_Critarr(i32 *crits, i32 num) {
    i32 wpn_crit = 0;

    for (int i = 0; i < num; i++) {
        wpn_crit += crits[i];
    }
    wpn_crit = nmath_inbounds_int32_t(wpn_crit, SOTA_MIN_CRIT, SOTA_MAX_CRIT);

    return (wpn_crit);
}

i32 Equation_Weapon_Critvar(size_t argnum, ...) {
    va_list valist;
    i32 wpn_crit = 0, current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, i32);
        wpn_crit += current_arg;
    }
    wpn_crit = nmath_inbounds_int32_t(wpn_crit, SOTA_MIN_CRIT, SOTA_MAX_CRIT);
    va_end(valist);
    return (wpn_crit);
}

i32 Equation_Weapon_Favor(i32 Lwpn_favor, i32 Rwpn_favor) {
    i32 wpn_favor = Lwpn_favor + Rwpn_favor;
    wpn_favor = nmath_inbounds_int32_t(wpn_favor, SOTA_MIN_FAVOR, SOTA_MAX_FAVOR);
    return (wpn_favor);
}

i32 Equation_Weapon_Favorarr(i32 *favors, i32 num) {
    i32 wpn_favor = 0;

    for (int i = 0; i < num; i++) {
        wpn_favor += favors[i];
    }
    wpn_favor = nmath_inbounds_int32_t(wpn_favor, SOTA_MIN_FAVOR, SOTA_MAX_FAVOR);

    return (wpn_favor);
}


i32 Equation_Weapon_Favorvar(size_t argnum, ...) {
    va_list valist;
    i32 wpn_favor = 0, current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, i32);
        wpn_favor += current_arg;
    }
    wpn_favor = nmath_inbounds_int32_t(wpn_favor, SOTA_MIN_FAVOR, SOTA_MAX_FAVOR);
    va_end(valist);
    return (wpn_favor);
}

i32 Equation_Weapon_Wgt(i32 Lwpn_wgt, i32 Rwpn_wgt) {
    i32 wpn_wgt = Lwpn_wgt + Rwpn_wgt;
    wpn_wgt = nmath_inbounds_int32_t(wpn_wgt, SOTA_MIN_WGT, SOTA_MIN_WGT);
    return (wpn_wgt);
}

i32 Equation_Weapon_Wgtvar(size_t argnum, ...) {
    va_list valist;
    i32 wpn_wgt = 0, current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, i32);
        wpn_wgt += current_arg;
    }
    wpn_wgt = nmath_inbounds_int32_t(wpn_wgt, SOTA_MIN_WGT, SOTA_MIN_WGT);
    va_end(valist);
    return (wpn_wgt);
}

i32 Equation_Weapon_Wgtarr(i32 *wgts, i32 num) {
    i32 wpn_wgt = 0;

    for (int i = 0; i < num; i++) {
        wpn_wgt += wgts[i];
    }

    return (wpn_wgt);
}

i32 Equation_Unit_Healshpvar(size_t argnum, ...) {
    va_list valist;
    i32 wpn_heal = 0, current_arg = 0;
    va_start(valist, argnum);
    for (size_t i = 0; i < argnum; i++) {
        current_arg = va_arg(valist, i32);
        /* Heal portions above 100 get divided by 4 ? */
        wpn_heal += current_arg;
    }
    wpn_heal = nmath_inbounds_int32_t(wpn_heal, SOTA_MIN_HEAL_PERCENT, SOTA_MAX_HEAL_PERCENT);
    va_end(valist);
    return (wpn_heal);
}

/* --  Equation_Combat_Damage --     */
//  Arguments:
//   1. att is attacker attack:       unit str + wpn might
//   2. def is defender defense:      unit def + wpn prot
//   3. effective_multiplier  is in [%] e.g. 200 for double damage
//   4. critp_multiplier      is in [%] e.g. 200 for double damage
i32 Equation_Combat_Damage(i32 att, i32 defender_block, i32 effective_multiplier,
                           i32 critp_multiplier, b32 crit) {
    /* damage = Attack*crit_factor^crit - defense */
    // DESIGN QUESTION: Should effective triple weapon might? or attack damage?
    // I'm thinking about doubling ATTACK DAMAGE, before removing defense?

    i32 crit_factor = crit ? critp_multiplier : SOTA_100PERCENT;
    /* POST DEFENSE CRIT DAMAGE */
    // i32 attack      = ((att - defender_block) * effective_multiplier * crit_factor) /
    // (SOTA_100PERCENT * SOTA_100PERCENT);
    /* PRE DEFENSE CRIT DAMAGE */
    i32 attack      = (att * effective_multiplier * crit_factor) /
                      (SOTA_100PERCENT * SOTA_100PERCENT) - defender_block;


    i32 damage      = nmath_inbounds_int32_t(attack, SOTA_MIN_DAMAGE, SOTA_MAX_DAMAGE);
    return (damage);
}

/* --  Equation_Attack_Damage -- */
/* Basic attack damage equation with no effective, no crit */
// UNUSED.
i32 Equation_Attack_Damage(i32 attacker_dmg, i32 defender_def) {
    return (Equation_Combat_Damage(attacker_dmg, defender_def, 100, 100, false));
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

i32 Equation_AI_Rating_HPprop(float hp_prop) {
    i32 out_rating = ((hp_prop * 100) / 4);
    return (out_rating); /* MAX 85 but rarely approaches that... */
}

i32 Equation_AI_Rating_pushGradient(i32 gradient) {
    i32 out_rating = gradient;
    return (out_rating); // MAX ?? but rarely approaches that...
}

i32 Equation_AI_Rating_pullGradient(i32 gradient) {
    i32 out_rating = gradient;
    return (out_rating); // MAX ?? but rarely approaches that...
}
