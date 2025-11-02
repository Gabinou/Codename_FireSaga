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
#include "macros.h"
#include "structs.h"
#include "equations.h"

b32 Eq_canCarry(i32 savior_con, i32 victim_con) {
    return (savior_con > victim_con);
}

i32 Eq_Regrets(i32 kills, i32 faith) {
    i32 regrets = 0;
    i32 eff_fth = nmath_inbounds_int32_t(   faith,
                                            0, UINT8_MAX);
    i32 divisor = eff_fth / REGRET_FAITH_FACTOR;

    if (divisor > 0) {
        regrets = kills / divisor;
    }

    regrets = nmath_inbounds_int32_t(   regrets,
                                        REGRET_MIN,
                                        REGRET_MAX);
    return (regrets);
}

i32 Eq_Agony_PercentonCrit(i32 luck, i32 con) {
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
    out_hit     = nmath_inbounds_int32_t(out_hit,
                                         0, SOTA_100PERCENT);
    return (out_hit);
}

i32 Eq_Combat_Crit(i32 att_crit, i32 dfd_favor) {
    i32 out = att_crit - dfd_favor;
    out = nmath_inbounds_int32_t(out, 0, SOTA_100PERCENT);
    return (out);
}

i32 Eq_Unit_Hit(Weapon_stats    wpn_stats,
                Unit_stats      unit_stats,
                i32 bonus) {
    // hit = wpn_hit + dex*2 + luck/2 + bonus
    i32 out = wpn_stats.hit
              + unit_stats.dex * HIT_DEX_FACTOR
              + unit_stats.luck / HIT_LUCK_FACTOR
              + bonus;
    out = nmath_inbounds_int32_t(   out, SOTA_MIN_HIT,
                                    SOTA_MAX_HIT);
    return (out);
}

i32 Eq_Unit_Crit(   Weapon_stats    wpn_stats,
                    Unit_stats      unit_stats,
                    i32 bonus) {
    // favor = wpn_crit + dex / 3 + luck / 4 + bonus
    i32 out = wpn_stats.crit
              + unit_stats.dex / CRIT_DEX_FACTOR
              + unit_stats.luck / CRIT_LUCK_FACTOR
              + bonus;
    out = nmath_inbounds_int32_t(   out,
                                    SOTA_MIN_CRIT,
                                    SOTA_MAX_CRIT);
    return (out);
}
/* Speed for Magical weapons */
i32 Eq_Unit_mSpeed( Weapon_stats    wpn_stats,
                    Unit_stats      unit_stats,
                    i32 bonus) {
    /* speed = agi
    **          - max(0, wpn_cmp - mag / 2))
    **          - min(wpn_mst, prof - (wpn_prof + wpn_mst))
    */
    i32 slowed  = wpn_stats.wgt - unit_stats.mag / SPEED_MAG_FACTOR;
    slowed      = NMATH_MAX(0, slowed);

    i32 prof_bonus  = unit_stats.prof - (wpn_stats.prof + wpn_stats.mastery);
    prof_bonus      = NMATH_MIN(prof_bonus, wpn_stats.mastery);

    i32 out = unit_stats.agi - slowed + bonus + prof_bonus;
    out = nmath_inbounds_int32_t(   out,
                                    SOTA_MIN_SPEED,
                                    SOTA_MAX_SPEED);
    return (out);
}

/* Speed for Physical weapons */
i32 Eq_Unit_Speed(  Weapon_stats    wpn_stats,
                    Unit_stats      unit_stats,
                    i32 bonus) {
    /* speed = agi
    **          - max(0, wpn_wgt - con / 3 - str / 4))
    **          - min(wpn_mst, prof - (wpn_prof + wpn_mst))
    */

    i32 slowed  = wpn_stats.wgt
                  - unit_stats.con / SPEED_CON_FACTOR
                  - unit_stats.str / SPEED_STR_FACTOR;
    slowed      = NMATH_MAX(0, slowed);

    i32 prof_bonus  = unit_stats.prof - (wpn_stats.prof + wpn_stats.mastery);
    prof_bonus      = NMATH_MIN(wpn_stats.mastery, prof_bonus);

    i32 out = unit_stats.agi - slowed + bonus + prof_bonus;
    out = nmath_inbounds_int32_t(   out,
                                    SOTA_MIN_SPEED,
                                    SOTA_MAX_SPEED);
    return (out);
}

i32 Eq_Unit_Dodge(  Weapon_stats    wpn_stats,
                    Unit_stats      unit_stats,
                    i32 tile_dodge, i32 bonus) {
    /*    dodge =
    **        - max(0, (Wpn.Wgt - STR/a))
    **        + AGI/d + FAITH/a + LUCK/b - CON/c
    */
    i32 slowed = wpn_stats.wgt - unit_stats.str / DODGE_STR_FACTOR;
    slowed = NMATH_MAX(0, slowed);

    i32 out = tile_dodge - slowed
              - unit_stats.con    / DODGE_CON_FACTOR
              + unit_stats.luck   / DODGE_LUCK_FACTOR
              + unit_stats.fth    / DODGE_FTH_FACTOR
              + unit_stats.agi    / DODGE_AGI_FACTOR
              + unit_stats.dex    / DODGE_DEX_FACTOR
              + wpn_stats.dodge + bonus;
    out = nmath_inbounds_int32_t(out,   SOTA_MIN_DODGE,
                                 SOTA_MAX_DODGE);
    return (out);
}

i32 Eq_Unit_Favor(  Weapon_stats    wpn_stats,
                    Unit_stats      unit_stats,
                    i32 bonus) {
    // favor = wpn_favor + faith / 2 + bonus

    i32 out = wpn_stats.favor
              + unit_stats.fth / FAVOR_FTH_FACTOR
              + bonus;
    out = nmath_inbounds_int32_t(out,   SOTA_MIN_FAVOR,
                                 SOTA_MAX_FAVOR);
    return (out);
}

i32 Eq_Wpn_Infuse( i32 stat, i32 infusion) {
    return (stat + infusion);
}

i32 Eq_Wpn_Profarr(i32 *prof, i32 num) {
    /* prof for multiple weapons get added */
    //      1- NOT used for Unit_canEquip
    //      2- used for prof/mastery AS bonus/malus
    if ((prof == NULL) || (num <= 0)) {
        return (0);
    }
    i32 wpn_prof = 0;

    for (int i = 0; i < num; i++) {
        wpn_prof += prof[i];
    }
    wpn_prof = nmath_inbounds_int32_t(wpn_prof,
                                      SOTA_MIN_PROF,
                                      SOTA_MAX_PROF);
    return (wpn_prof);
}

i32 Eq_Wpn_Masteryarr(i32 *mst, i32 num) {
    /* mastery for multiple weapons get added */
    if ((mst == NULL) || (num <= 0)) {
        return (0);
    }
    i32 wpn_mst = 0;

    for (int i = 0; i < num; i++) {
        wpn_mst += mst[i];
    }
    wpn_mst = nmath_inbounds_int32_t(wpn_mst,
                                     SOTA_MIN_MST,
                                     SOTA_MAX_MST);
    return (wpn_mst);
}

// prot -> protection which is def/res
i32 Eq_Wpn_Defense(i32 prot, i32 tile_prot) {
    i32 out_def = prot + tile_prot;
    out_def     = nmath_inbounds_int32_t(out_def, SOTA_MIN_PROT, SOTA_MAX_PROT);
    return (out_def);
}

i32 Eq_Wpn_Defensearr(i32 *def, i32 num) {
    /* Attack for multiple weapons get added */
    if ((def == NULL) || (num <= 0)) {
        return (0);
    }
    i32 wpn_def = 0;

    for (int i = 0; i < num; i++) {
        wpn_def += def[i];
    }
    wpn_def = nmath_inbounds_int32_t(wpn_def,
                                     SOTA_MIN_PROT,
                                     SOTA_MAX_PROT);
    return (wpn_def);
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
//   3. eff:                        [%] e.g. 200 for double damage
//      - effectiveness
//   4. crit:                       [%] e.g. 200 for double damage
//   5. iscrit:                     is attack critting
i32 Eq_Combat_Damage(   i32 att, i32 block,
                        i32 eff, i32 crit, b32 iscrit) {
    /* Compute damage to be done by attacker, on defender. */
    /* Equation: damage = (attack - defense)*crit_factor*effective */
    // - ibid physical/magical damage

    /* Fully blocked, no damage */
    if (block >= att) {
        return(0);
    }
    IES_assert(crit >= SOTA_100PERCENT);
    IES_assert(eff  >= SOTA_100PERCENT);
    IES_assert(eff  >= CRIT_FACTOR_MAX);
    IES_assert(crit >= CRIT_FACTOR_MAX);

    i32 unblocked   = att - block;
    i32 crit_factor = iscrit ? crit : SOTA_100PERCENT;

    i32 attack  = (unblocked * eff * crit_factor) /
                  (SOTA_100PERCENT * SOTA_100PERCENT);

    i32 damage  = nmath_inbounds_int32_t(   attack, SOTA_MIN_DAMAGE,
                                            SOTA_MAX_DAMAGE);
    return (damage);
}

void Eq_Combat_Damage_Dealt(struct Combat_Damage *dmg) {
    /* Compute Damage dealt to enemy
    ** Note: physical/magical include effect of def/res
    **      - Can add damages to get total damage dealt
    */
    dmg->dmg.dealt =    dmg->dmg.physical + dmg->dmg.magical +
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

i32 Eq_Wpn_Attackarr(i32 *att, i32 num) {
    /* Attack for multiple weapons get added */
    if ((att == NULL) || (num <= 0)) {
        return (0);
    }

    i32 wpn_att = 0;
    for (int i = 0; i < num; i++) {
        wpn_att += att[i];
    }
    wpn_att = nmath_inbounds_int32_t(wpn_att,
                                     SOTA_MIN_ATTACK,
                                     SOTA_MAX_ATTACK);
    return (wpn_att);
}

i32 Eq_Wpn_Hit(i32 Lwpn_hit, i32 Rwpn_hit) {
    /* Hit for multiple weapons get AVERAGED */
    i32 wpn_hit = Lwpn_hit + Rwpn_hit;
    if ((Rwpn_hit > 0) && (Lwpn_hit > 0))
        wpn_hit /= 2;
    return (wpn_hit);
}

// TODO: RM one of the two next
i32 Eq_Wpn_Hitarr(i32 *hits, i32 num) {
    /* Averaging Hit for multiple weapons
    ** Adding would make offhands unusable stand-alone. */
    if ((hits == NULL) || (num <= 0)) {
        return (0);
    }
    i32 wpn_hit = 0;
    i32 divisor = 0;

    for (int i = 0; i < num; i++) {
        /* skip */
        if (hits[i] == ITEM_STAT_INVALID)
            continue;

        divisor++;
        wpn_hit += hits[i];
    }
    if (divisor == 0) {
        return (0);
    }

    wpn_hit = nmath_inbounds_int32_t(   wpn_hit / divisor, 
                                        SOTA_MIN_HIT, SOTA_MAX_HIT);
    return (wpn_hit);
}

i32 Eq_Wpn_Dodge(i32 Lwpn_dodge, i32 Rwpn_dodge) {
    /* Dodge for multiple weapons get added */
    i32 wpn_dodge = Lwpn_dodge + Rwpn_dodge;
    wpn_dodge = nmath_inbounds_int32_t( wpn_dodge, SOTA_MIN_DODGE, 
                                        SOTA_MAX_DODGE);
    return (wpn_dodge);
}

i32 Eq_Wpn_Dodgearr(i32 *dodges, i32 num) {
    /* Dodge for multiple weapons get added */
    if ((dodges == NULL) || (num <= 0)) {
        return (0);
    }

    i32 wpn_dodge = 0;
    for (int i = 0; i < num; i++) {
        wpn_dodge += dodges[i];
    }
    wpn_dodge = nmath_inbounds_int32_t( wpn_dodge,
                                        SOTA_MIN_DODGE,
                                        SOTA_MAX_DODGE);

    return (wpn_dodge);
}

i32 Eq_Wpn_Crit(i32 Lwpn_crit, i32 Rwpn_crit) {
    /* Crit for multiple weapons get added */
    i32 wpn_crit = Lwpn_crit + Rwpn_crit;
    wpn_crit = nmath_inbounds_int32_t(  wpn_crit, SOTA_MIN_CRIT,
                                        SOTA_MAX_CRIT);
    return (wpn_crit);
}

i32 Eq_Wpn_Critarr(i32 *crits, i32 num) {
    /* Crit for multiple weapons get added */
    if ((crits == NULL) || (num <= 0)) {
        return (0);
    }

    i32 wpn_crit = 0;

    for (int i = 0; i < num; i++) {
        wpn_crit += crits[i];
    }
    wpn_crit = nmath_inbounds_int32_t(  wpn_crit, SOTA_MIN_CRIT,
                                        SOTA_MAX_CRIT);

    return (wpn_crit);
}

i32 Eq_Wpn_Favor(i32 Lwpn_favor, i32 Rwpn_favor) {
    /* Favor for multiple weapons get added */
    i32 wpn_favor = Lwpn_favor + Rwpn_favor;
    wpn_favor = nmath_inbounds_int32_t( wpn_favor, SOTA_MIN_FAVOR,
                                        SOTA_MAX_FAVOR);
    return (wpn_favor);
}

i32 Eq_Wpn_Favorarr(i32 *favors, i32 num) {
    /* Favor for multiple weapons get added */
    if ((favors == NULL) || (num <= 0)) {
        return (0);
    }

    i32 wpn_favor = 0;
    for (int i = 0; i < num; i++) {
        wpn_favor += favors[i];
    }
    wpn_favor = nmath_inbounds_int32_t( wpn_favor, SOTA_MIN_FAVOR,
                                        SOTA_MAX_FAVOR);

    return (wpn_favor);
}

i32 Eq_Wpn_Wgt(i32 Lwpn_wgt, i32 Rwpn_wgt) {
    /* Weights for multiple weapons get added */
    i32 wpn_wgt = Lwpn_wgt + Rwpn_wgt;
    wpn_wgt = nmath_inbounds_int32_t(   wpn_wgt, SOTA_MIN_WGT,
                                        SOTA_MIN_WGT);
    return (wpn_wgt);
}

i32 Eq_Wpn_Wgtarr(i32 *wgts, i32 num) {
    /* Weights for multiple weapons get added */
    if ((wgts == NULL) || (num <= 0)) {
        return (0);
    }

    i32 wpn_wgt = 0;
    for (int i = 0; i < num; i++) {
        wpn_wgt += wgts[i];
    }

    return (wpn_wgt);
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
    return (wgt / TWO_HANDING_WEIGHT_FACTOR);
}

i32 Eq_Wpn_Repair_Hard(Weapon_stats stats) {
    /* Repair scaled by item STRENGTH.*/

    i32 hardness =  stats.attack.physical       +
                    stats.attack.magical        +
                    stats.attack.True           +
                    stats.protection.physical   +
                    stats.protection.magical;
    return (hardness);
}

i32 Eq_Wpn_Repair(i32 hardness, i32 AP) {
    return (AP / hardness);
}

i32  Eq_Price_Used(struct InvItem *invitem) {
    IES_nullcheck_ret(invitem, 0);
    Item *item = Item_Get(invitem);
    IES_nullcheck_ret(item, 0);
    return(_Eq_Price_Used(  item->stats.uses, invitem->used,
                            item->stats.price));
}

i32 _Eq_Price_Used(i32 uses, i32 used, i32 price) {
    /* -- Price lower propto uses. -- */
    if ((used > uses)   || (uses <= 0) ||
        (used <= 0)     || (price <= 0)) {
        return(0);
    }    
    i32 low_price = price * (uses - used) / uses;
    return(low_price);
}
