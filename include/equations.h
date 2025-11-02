#ifndef EQUATIONS_H
#define EQUATIONS_H
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

#include <math.h>
#include <stdarg.h>
#include <limits.h>
#include "types.h"
#include "enums.h"
#include "structs.h"

// Input for equations
// 1- raw values
//  + Clarity about used values
//  - Annoying to update
// 2- unique struct per equation
//  --- VERY annoying
// 3- Common structs (Unit_stats, Weapon_stats)
//  - Unclear which stats are used
//  + input does not need update
//  - NOT easy to create inputs for weapon stats
//      - e.g. need HANDedness to output stats
//      - e.g. input need infusion
//      + Make Effective_Weapon_stats


/* --- FORWARD DECLARATIONS --- */
struct Unit;
struct InvItem;
struct Combat_Damage;

/* --- MACROS --- */
/* -- Unit Combat -- */
#define Eq_multiplyDamage(dmg, multiplier) ((dmg) > (i32)(UINT8_MAX / (multiplier)) ? UINT8_MAX : (i32)((dmg) * (multiplier)))

/* --- FUNCTION DEFINITIONS --- */
/* -- Unit Computed Stats -- */
i32 Eq_Unit_Hit(    Weapon_stats    wpn_stats,
                    Unit_stats      unit_stats,
                    i32 bonus);
i32 Eq_Unit_Crit(   Weapon_stats    wpn_stats,
                    Unit_stats      unit_stats,
                    i32 bonus);
i32 Eq_Unit_Speed(  Weapon_stats    wpn_stats,
                    Unit_stats      unit_stats,
                    i32 bonus);
i32 Eq_Unit_mSpeed( Weapon_stats    wpn_stats,
                    Unit_stats      unit_stats,
                    i32 bonus);
i32 Eq_Unit_Favor(  Weapon_stats    wpn_stats,
                    Unit_stats      unit_stats,
                    i32 bonus);
i32 Eq_Unit_Dodge(  Weapon_stats    wpn_stats,
                    Unit_stats      unit_stats,
                    i32 tile_dodge, i32 bonus);

/* -- Staff Healing -- */
i32 Eq_Staff_Healing(   i32 item_AP,  i32 user_mag);
i32 Eq_Item_Healing(    i32 item_AP);

/* -- Unit Healing -- */
i32 Eq_Unit_Healshp(i32 total_hp, i32 heal_percent);

/* -- Unit Rescue -- */
b32 Eq_canCarry(i32 savior_con, i32 victim_con);

/* -- Unit Regrets -- */
i32 Eq_Regrets(i32 kills, i32 faith);

/* -- Combat -- */
i32     Eq_Combat_Hit(     i32 hit_a,  i32 avoid_d);
i32     Eq_Combat_Crit(    i32 crit_a, i32 favor_d);
i32     Eq_Combat_Damage(   i32 att_a,  i32 def_d,
                            i32 eff,    i32 critp,
                            b32 crit);
void    Eq_Combat_Damage_Dealt(struct Combat_Damage *dmg);

/* -- Weapon -- */
i32 Eq_Wpn_Hit(    i32 Lwpn_hit,       i32 Rwpn_hit);
i32 Eq_Wpn_Wgt(    i32 Lwpn_wgt,       i32 Rwpn_wgt);
i32 Eq_Wpn_Crit(   i32 Lwpn_crit,      i32 Rwpn_crit);
i32 Eq_Wpn_Favor(  i32 Lwpn_favor,     i32 Rwpn_favor);
i32 Eq_Wpn_Dodge(  i32 Lwpn_dodge,     i32 Rwpn_dodge);
i32 Eq_Wpn_Attack( i32 Lwpn_attack,    i32 Rwpn_attack);
i32 Eq_Wpn_Infuse( i32 s, i32 i);
i32 Eq_Wpn_Defense(i32 Lwpn_def,       i32 Rwpn_def);
i32 Eq_Wpn_Repair_Hard(Weapon_stats wpn_stats);
i32 Eq_Wpn_Repair(i32 hardness, i32 AP);

i32 Eq_Wpn_Hitarr(      i32 *hits,      i32 num);
i32 Eq_Wpn_Wgtarr(      i32 *wgts,      i32 num);
i32 Eq_Wpn_Critarr(     i32 *crits,     i32 num);
i32 Eq_Wpn_Dodgearr(    i32 *dodges,    i32 num);
i32 Eq_Wpn_Favorarr(    i32 *favors,    i32 num);
i32 Eq_Wpn_Attackarr(   i32 *att,       i32 num);
i32 Eq_Wpn_Defensearr(  i32 *def,       i32 num);
i32 Eq_Wpn_Profarr(     i32 *prof,      i32 num);
i32 Eq_Wpn_Masteryarr(  i32 *mst,       i32 num);

/* -- Agony -- */
i32 Eq_Agony_Turns(           i32 str,    i32 def,
                              i32 con,    i32 bonus);
i32 Eq_Agony_PercentonCrit(   i32 luck,   i32 con);

i32  Eq_Price_Used(struct InvItem *item);
i32 _Eq_Price_Used(i32 uses, i32 used, i32 price);

/* -- AI equations -- */
i32 Eq_AI_Rating_Stats(   i32   stat);
i32 Eq_AI_Rating_Damage(  i32   dmg);
i32 Eq_AI_Rating_HPprop(  float hp_prop);
i32 Eq_AI_Rating_hitRate( i32   hit_rate);
i32 Eq_AI_Rating_critRate(i32   crit_rate);

i32 Eq_AI_Rating_Equipment(   i32 crit);
i32 Eq_AI_Rating_pushGradient(i32 gradient);
i32 Eq_AI_Rating_pullGradient(i32 gradient);

i32 Eq_Wpn_Two_Handing_Wgt(i32 wgt);

#endif /* EQUATIONS_H */
