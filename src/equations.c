/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
** equations: centralized game design computations
**
*/

#include "nmath.h"
#include "structs.h"
#include "equations.h"

/* ---EXPECTED DAMAGE COMPUTATION--- */
/* Outputs expected/average damage value when multiplied with (att-def) */
/* Offensive skills should also give expected damage factor values.     */
float Eq_Expected_Damage(i32 hit, i32 crit, i32 critfactor) {
    return (hit / SOTA_100PERCENT * pow((double)critfactor, ((double)crit) / SOTA_100PERCENT));
}

b32 Eq_canCarry(i32 savior_con, i32 victim_con) {
    return (savior_con > victim_con);
}

/* TODO: change to SOTA values*/
i32 Eq_Regrets(i32 kills, i32 faith) {
    i32 out = 0;
    i32 eff_faith = faith > UINT8_MAX ? UINT8_MAX : faith;
    i32 divisor = ((eff_faith < 0 ? 0 : eff_faith) / REGRET_FAITH_FACTOR);
    if (divisor > 0)
        out = kills / divisor;

    out = nmath_inbounds_int32_t(out, REGRET_MIN, REGRET_MAX);
    return (out);
}

i32 Eq_Agony_PercentonCrit(i32 luck, i32 con) {
    // i32 eff_luck    = luck + con;
    i32 agony_prob = 0;
    agony_prob = nmath_inbounds_int32_t(agony_prob, 0, SOTA_100PERCENT);
    return (agony_prob);
}

i32 Eq_Agony_Turns(i32 str, i32 def, i32 con, i32 bonus) {
    i32 agony_length     = str + def + con;
    agony_length        /= AGONY_FACTOR + bonus;
    agony_length         = nmath_inbounds_int32_t(agony_length, SOTA_MIN_AGONY, SOTA_MAX_AGONY);
    return (agony_length);
}

i32 Eq_Combat_Hit(i32 att_hit, i32 dfd_avoid) {
    i32 out_hit = att_hit - dfd_avoid;
    out_hit     = nmath_inbounds_int32_t(out_hit, 0, SOTA_100PERCENT);
    return (out_hit);
}

i32 Eq_Combat_Crit(i32 attacker_crit, i32 defender_favor) {
    i32 out = attacker_crit - defender_favor;
    out     = nmath_inbounds_int32_t(   out,
                                        0, SOTA_100PERCENT);
    return (out);
}

i32 Eq_Unit_Hit(i32 wpn_hit,    i32 dex,
                i32 luck,       i32 bonus) {
    // hit = wpn_hit + dex*2 + luck/2 + bonus
    i32 out = wpn_hit
              + dex * HIT_DEX_FACTOR
              + luck / HIT_LUCK_FACTOR
              + bonus;
    out = nmath_inbounds_int32_t(   out,
                                    SOTA_MIN_HIT,
                                    SOTA_MAX_HIT);
    return (out);
}

i32 Eq_Unit_Crit(   i32 wpn_crit,   i32 dex,
                    i32 luck,       i32 bonus) {
    // favor = wpn_crit + dex / 3 + luck / 4 + bonus
    i32 out = wpn_crit
              + dex / CRIT_DEX_FACTOR
              + luck / CRIT_LUCK_FACTOR
              + bonus;
    out = nmath_inbounds_int32_t(   out,
                                    SOTA_MIN_CRIT,
                                    SOTA_MAX_CRIT);
    return (out);
}
/* Speed for Magical weapons */
i32 Eq_Unit_mSpeed( i32 wpn_cmp,   i32 wpn_mst,
                    i32 wpn_prof,   i32 prof,
                    i32 agi,        i32 mag,
                    i32 bonus) {
    /* speed = agi
    **          - max(0, wpn_cmp - mag / 2))
    **          - min(wpn_mst, prof - (wpn_prof + wpn_mst))
    */
    i32 slowed  = wpn_cmp - mag / SPEED_MAG_FACTOR;
    slowed      = NMATH_MAX(0, slowed);

    i32 prof_bonus  = prof - (wpn_prof + wpn_mst);
    prof_bonus      = NMATH_MIN(wpn_mst, prof);

    i32 out = agi - slowed + bonus + prof_bonus;
    out = nmath_inbounds_int32_t(   out,
                                    SOTA_MIN_SPEED,
                                    SOTA_MAX_SPEED);
    return (out);
}

/* Speed for Physical weapons */
i32 Eq_Unit_Speed(  i32 wpn_wgt,    i32 wpn_mst,
                    i32 wpn_prof,   i32 prof,
                    i32 agi,        i32 con,
                    i32 str,        i32 bonus) {
    /* speed = agi
    **          - max(0, wpn_wgt - con / 3 - str / 4))
    **          - min(wpn_mst, prof - (wpn_prof + wpn_mst))
    */
    i32 slowed  = wpn_wgt
                  - con / SPEED_CON_FACTOR
                  - str / SPEED_STR_FACTOR;
    slowed      = NMATH_MAX(0, slowed);

    i32 prof_bonus  = prof - (wpn_prof + wpn_mst);
    prof_bonus      = NMATH_MIN(wpn_mst, prof);

    i32 out = agi - slowed + bonus + prof_bonus;
    out = nmath_inbounds_int32_t(   out,
                                    SOTA_MIN_SPEED,
                                    SOTA_MAX_SPEED);
    return (out);
}

i32 Eq_Unit_Dodge(  i32 wpn_wgt,    i32 wpn_dodge,
                    i32 luck,       i32 faith,
                    i32 agi,        i32 str,
                    i32 con,        i32 dex,
                    i32 tile_dodge, i32 bonus) {
    /*    dodge =
    **        - max(0, (Wpn.Wgt - STR/a))
    **        + AGI/d + FAITH/a + LUCK/b - CON/c
    */
    i32 slowed = wpn_wgt - str / DODGE_STR_FACTOR;
    slowed = NMATH_MAX(0, slowed);

    i32 out_dodge = tile_dodge - slowed
                    - con   / DODGE_CON_FACTOR
                    + luck  / DODGE_LUCK_FACTOR
                    + faith / DODGE_FTH_FACTOR
                    + agi   / DODGE_AGI_FACTOR
                    + dex   / DODGE_DEX_FACTOR
                    + wpn_dodge + bonus;
    out_dodge     = nmath_inbounds_int32_t(out_dodge, SOTA_MIN_DODGE, SOTA_MAX_DODGE);
    return (out_dodge);
}

i32 Eq_Unit_Favor(i32 wpn_favor, i32 faith, i32 bonus) {
    // favor = wpn_favor + faith / 2 + bonus

    i32 out_favor = wpn_favor + (faith / FAVOR_FTH_FACTOR) + bonus;
    out_favor     = nmath_inbounds_int32_t(out_favor, SOTA_MIN_FAVOR, SOTA_MAX_FAVOR);
    return (out_favor);
}

i32 Eq_Wpn_Infuse( i32 stat, i32 infusion) {
    return (stat + infusion);
}

// prot -> protection which is def/res
i32 Eq_Wpn_Defense(i32 prot, i32 tile_prot) {
    i32 out_def = prot + tile_prot;
    out_def     = nmath_inbounds_int32_t(out_def, SOTA_MIN_PROT, SOTA_MAX_PROT);
    return (out_def);
}

i32 Eq_Wpn_Defensevar(size_t argnum, ...) {
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

/* --- Eq_Staff_Healing --- */
/* Healing value of unit equipping a staff */
/* Output: flat HP */
/* DESIGN QUESTION: Should staves flat heal, or % heal? */

i32 Eq_Staff_Healing(i32 item_AP, i32 user_mag) {
    i32 to_heal = item_AP + user_mag;
    to_heal     = nmath_inbounds_int32_t(to_heal, SOTA_MIN_HEAL, SOTA_MAX_HEAL);
    return (to_heal);
}

/* Output: percent% HP */
i32 Eq_Item_Healing(i32 item_AP) {
    i32 to_heal = item_AP;
    to_heal     = nmath_inbounds_int32_t(to_heal, SOTA_MIN_HEAL, SOTA_MAX_HEAL);
    return (to_heal);
}

/* --- Eq_Unit_Healshp --- */
/* Unit healing percent % -> HP */
i32 Eq_Unit_Healshp(i32 total_hp, i32 heal_percent) {
    i32 to_heal = (total_hp * heal_percent) / SOTA_100PERCENT;
    to_heal     = nmath_inbounds_int32_t(to_heal, 1, SOTA_MAX_HP);
    return (to_heal);
}

i32 Eq_Wpn_Attack(i32 Lwpn_might, i32 Rwpn_might) {
    i32 wpn_attack = Lwpn_might + Rwpn_might;
    wpn_attack     = nmath_inbounds_int32_t(wpn_attack, SOTA_MIN_ATTACK, SOTA_MAX_ATTACK);
    return (wpn_attack);
}

/* --  Eq_Combat_Damage -- */
//  Arguments:
//   1. att is attacker attack:     unit str + wpn might
//   2. def is defender defense:    unit def + wpn prot
//   3. effective_multiplier:       is in [%] e.g. 200 for double damage
//   4. critp_multiplier:           is in [%] e.g. 200 for double damage
//   5. crit:                       is attack critting
i32 Eq_Combat_Damage(i32 att, i32 block, i32 effective_multiplier,
                     i32 critp_multiplier, b32 crit) {
    /* Compute damage to be done by attacker, on defender. */
    /* Equation: damage = (attack - defense)*crit_factor*effective */
    // - Same for physical/magical damage

    i32 crit_factor = crit ? critp_multiplier : SOTA_100PERCENT;
    i32 eff_att = (block >= att) ? 0 : (att - block);

    i32 attack      = (eff_att * effective_multiplier * crit_factor) /
                      (SOTA_100PERCENT * SOTA_100PERCENT);

    i32 damage      = nmath_inbounds_int32_t(attack, SOTA_MIN_DAMAGE, SOTA_MAX_DAMAGE);
    return (damage);
}

void Eq_Combat_Damage_Dealt(struct Combat_Damage *dmg) {
    /* Compute Damage dealt to enemy
    ** Note: physical/magical include effect of def/res
    **      - Can add damages to get total damage dealt
    */
    dmg->dmg.dealt =    dmg->dmg.physical +
                        dmg->dmg.magical +
                        dmg->dmg.True;
    dmg->dmg.dealt = nmath_inbounds_int32_t(dmg->dmg.dealt,
                                            SOTA_MIN_ATTACK,
                                            SOTA_MAX_ATTACK);
    dmg->dmg_crit.dealt =   dmg->dmg_crit.physical +
                            dmg->dmg_crit.magical +
                            dmg->dmg_crit.True;
    dmg->dmg_crit.dealt = nmath_inbounds_int32_t(dmg->dmg_crit.dealt,
                                                 SOTA_MIN_ATTACK,
                                                 SOTA_MAX_ATTACK);
}

i32 Eq_Wpn_Attvar(size_t argnum, ...) {
    /* Attack for multiple weapons get AVERAGED */
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
i32 Eq_Wpn_Hit(i32 Lwpn_hit, i32 Rwpn_hit) {
    // TODO: RM
    /* Hit for multiple weapons get AVERAGED */
    i32 wpn_hit = Lwpn_hit + Rwpn_hit;
    if ((Rwpn_hit > 0) && (Lwpn_hit > 0))
        wpn_hit /= 2;
    return (wpn_hit);
}

// TODO: RM one of the two next
i32 Eq_Wpn_Hitarr(i32 *hits, i32 num) {
    /* Hit for multiple weapons get AVERAGED */
    i32 wpn_hit = 0;
    i32 divisor = 0;

    for (int i = 0; i < num; i++) {
        if (hits[i])
            divisor++;
        wpn_hit += hits[i];
    }
    if (divisor == 0) {
        return (0);
    }

    return (wpn_hit / divisor);
}

i32 Eq_Wpn_Hitvar(size_t argnum, ...) {
    /* Hit for multiple weapons get AVERAGED */
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

i32 Eq_Wpn_Dodge(i32 Lwpn_dodge, i32 Rwpn_dodge) {
    /* Dodge for multiple weapons get added */
    i32 wpn_dodge = Lwpn_dodge + Rwpn_dodge;
    wpn_dodge = nmath_inbounds_int32_t(wpn_dodge, SOTA_MIN_DODGE, SOTA_MAX_DODGE);
    return (wpn_dodge);
}

i32 Eq_Wpn_Dodgevar(size_t argnum, ...) {
    /* Dodge for multiple weapons get added */
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

i32 Eq_Wpn_Dodgearr(i32 *dodges, i32 num) {
    /* Dodge for multiple weapons get added */
    i32 wpn_dodge = 0;

    for (int i = 0; i < num; i++) {
        wpn_dodge += dodges[i];
    }
    wpn_dodge = nmath_inbounds_int32_t(wpn_dodge, SOTA_MIN_DODGE, SOTA_MAX_DODGE);

    return (wpn_dodge);
}

i32 Eq_Wpn_Crit(i32 Lwpn_crit, i32 Rwpn_crit) {
    /* Crit for multiple weapons get added */
    i32 wpn_crit = Lwpn_crit + Rwpn_crit;
    wpn_crit = nmath_inbounds_int32_t(wpn_crit, SOTA_MIN_CRIT, SOTA_MAX_CRIT);
    return (wpn_crit);
}

i32 Eq_Wpn_Critarr(i32 *crits, i32 num) {
    /* Crit for multiple weapons get added */
    i32 wpn_crit = 0;

    for (int i = 0; i < num; i++) {
        wpn_crit += crits[i];
    }
    wpn_crit = nmath_inbounds_int32_t(wpn_crit, SOTA_MIN_CRIT, SOTA_MAX_CRIT);

    return (wpn_crit);
}

i32 Eq_Wpn_Critvar(size_t argnum, ...) {
    /* Crit for multiple weapons get added */
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

i32 Eq_Wpn_Favor(i32 Lwpn_favor, i32 Rwpn_favor) {
    /* Favor for multiple weapons get added */
    i32 wpn_favor = Lwpn_favor + Rwpn_favor;
    wpn_favor = nmath_inbounds_int32_t(wpn_favor, SOTA_MIN_FAVOR, SOTA_MAX_FAVOR);
    return (wpn_favor);
}

i32 Eq_Wpn_Favorarr(i32 *favors, i32 num) {
    /* Favor for multiple weapons get added */
    i32 wpn_favor = 0;

    for (int i = 0; i < num; i++) {
        wpn_favor += favors[i];
    }
    wpn_favor = nmath_inbounds_int32_t(wpn_favor, SOTA_MIN_FAVOR, SOTA_MAX_FAVOR);

    return (wpn_favor);
}


i32 Eq_Wpn_Favorvar(size_t argnum, ...) {
    /* Favor for multiple weapons get added */
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

i32 Eq_Wpn_Wgt(i32 Lwpn_wgt, i32 Rwpn_wgt) {
    /* Weights for multiple weapons get added */
    i32 wpn_wgt = Lwpn_wgt + Rwpn_wgt;
    wpn_wgt = nmath_inbounds_int32_t(wpn_wgt, SOTA_MIN_WGT, SOTA_MIN_WGT);
    return (wpn_wgt);
}

i32 Eq_Wpn_Wgtvar(size_t argnum, ...) {
    /* Weights for multiple weapons get added */
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

i32 Eq_Wpn_Wgtarr(i32 *wgts, i32 num) {
    /* Weights for multiple weapons get added */
    i32 wpn_wgt = 0;

    for (int i = 0; i < num; i++) {
        wpn_wgt += wgts[i];
    }

    return (wpn_wgt);
}

i32 Eq_Unit_Healshpvar(size_t argnum, ...) {
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


/* --  Eq_Attack_Damage -- */
/* Basic attack damage equation with no effective, no crit */
// UNUSED.
i32 Eq_Attack_Damage(i32 attacker_dmg, i32 defender_def) {
    return (Eq_Combat_Damage(attacker_dmg, defender_def, 100, 100, false));
}

i32 Eq_AI_Rating_hitRate(i32 in_hit_rate) {
    i32 out_rating = (in_hit_rate / 5);
    return (out_rating); // MAX 20
}

i32 Eq_AI_Rating_critRate(i32 in_crit_rate) {
    i32 out_rating = (in_crit_rate / 3);
    return (out_rating); // MAX 33
}

i32 Eq_AI_Rating_Damage(i32 in_dmg) {
    i32 out_rating = (in_dmg / 3);
    return (out_rating); /* MAX 85 but rarely approaches that... */
}

i32 Eq_AI_Rating_Stats(i32 in_stat) {
    i32 out_rating = (in_stat / 3);

    return (out_rating);  /* MAX 85 but rarely approaches that... */
}

i32 Eq_AI_Rating_HPprop(float hp_prop) {
    i32 out_rating = ((hp_prop * 100) / 4);
    return (out_rating); /* MAX 85 but rarely approaches that... */
}

i32 Eq_AI_Rating_pushGradient(i32 gradient) {
    i32 out_rating = gradient;
    return (out_rating); // MAX ?? but rarely approaches that...
}

i32 Eq_AI_Rating_pullGradient(i32 gradient) {
    i32 out_rating = gradient;
    return (out_rating); // MAX ?? but rarely approaches that...
}

i32 Eq_Wpn_Two_Handing_Wgt(i32 wgt) {
    return(wgt / TWO_HANDING_WEIGHT_FACTOR);
}
