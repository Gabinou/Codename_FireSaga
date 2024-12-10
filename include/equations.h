#ifndef EQUATIONS_H
#define EQUATIONS_H

#include <math.h>
#include <stdarg.h>
#include <limits.h>
#include "types.h"
#include "enums.h"

/* --- FORWARD DECLARATIONS --- */
struct Unit;
struct Damage;

/* --- MACROS --- */
/* -- Unit Combat -- */
#define Equation_multiplyDamage(dmg, multiplier) ((dmg) > (i32)(UINT8_MAX / (multiplier)) ? UINT8_MAX : (i32)((dmg) * (multiplier)))

/* --- FUNCTION DEFINITIONS --- */
/* -- Unit Computed Stats -- */
i32 Equation_Unit_Hit(  i32 wpn_hit,   i32 dex,       i32 luck, i32 bonus);
i32 Equation_Unit_Crit( i32 wpn_crit,  i32 dex,       i32 luck, i32 bonus);
i32 Equation_Unit_Speed(i32 wpn_wgt,   i32 agi,       i32 con,  i32 str,
                        i32 bonus);
i32 Equation_Unit_Favor(i32 wpn_favor, i32 faith,     i32 bonus);
i32 Equation_Unit_Dodge(i32 wpn_wgt,   i32 wpn_dodge, i32 luck, i32 faith,
                        i32 agi,       i32 str,       i32 con,  i32 tile_dodge,
                        i32 bonus);

/* -- Staff Healing -- */
i32 Equation_Staff_Healing(i32 item_AP,  i32 user_mag);

/* -- Unit Healing -- */
i32 Equation_Unit_Healshp(i32 total_hp, i32 heal_percent);
i32 Equation_Unit_Healshpvar(size_t argnum, ...);

/* -- Unit Rescue -- */
b32 Equation_canCarry(i32 savior_con, i32 victim_con);

/* -- Unit Regrets -- */
i32 Equation_Regrets(i32 kills, i32 faith);

/* -- Combat -- */
i32    Equation_Combat_Hit(     i32 hit_a,  i32 avoid_d);
i32    Equation_Combat_Crit(    i32 crit_a, i32 favor_d);
i32    Equation_Attack_Damage(  i32 dmg_a,  i32 def_d);
i32    Equation_Combat_Damage(  i32 att_a,  i32 def_d, i32 eff, i32 critp, b32 crit);
float Equation_Expected_Damage(i32 hit,    i32 crit,  i32 critfactor);

/* -- Compute damage total from other damae types in struct -- */
void  Equation_Damage_Total(struct Damage *dmg);

/* -- Weapon -- */
i32 Equation_Weapon_Hit(    i32 Lwpn_hit,       i32 Rwpn_hit);
i32 Equation_Weapon_Wgt(    i32 Lwpn_wgt,       i32 Rwpn_wgt);
i32 Equation_Weapon_Crit(   i32 Lwpn_crit,      i32 Rwpn_crit);
i32 Equation_Weapon_Favor(  i32 Lwpn_favor,     i32 Rwpn_favor);
i32 Equation_Weapon_Dodge(  i32 Lwpn_dodge,     i32 Rwpn_dodge);
i32 Equation_Weapon_Attack( i32 Lwpn_attack,    i32 Rwpn_attack);
i32 Equation_Weapon_Defense(i32 Lwpn_def,       i32 Rwpn_def);

i32 Equation_Weapon_Hitarr(     i32 *hits,      i32 num);
i32 Equation_Weapon_Wgtarr(     i32 *wgts,      i32 num);
i32 Equation_Weapon_Critarr(    i32 *crits,     i32 num);
i32 Equation_Weapon_Dodgearr(   i32 *dodges,    i32 num);
i32 Equation_Weapon_Favorarr(   i32 *favors,    i32 num);

/* - argvar - */
i32 Equation_Weapon_Hitvar(    size_t argnum, ...);
i32 Equation_Weapon_Wgtvar(    size_t argnum, ...);
i32 Equation_Weapon_Critvar(   size_t argnum, ...);
i32 Equation_Weapon_Favorvar(  size_t argnum, ...);
i32 Equation_Weapon_Dodgevar(  size_t argnum, ...);
i32 Equation_Weapon_Attackvar( size_t argnum, ...);
i32 Equation_Weapon_Defensevar(size_t argnum, ...);

/* -- Agony -- */
i32 Equation_Agony_Turns(        i32 str, i32 def, i32 con, i32 bonus);
i32 Equation_Agony_PercentonCrit(i32 luck, i32 con);

/* -- AI equations -- */
i32 Equation_AI_Rating_Stats(   i32   stat);
i32 Equation_AI_Rating_Damage(  i32   dmg);
i32 Equation_AI_Rating_HPprop(  float hp_prop);
i32 Equation_AI_Rating_hitRate( i32   hit_rate);
i32 Equation_AI_Rating_critRate(i32   crit_rate);

i32 Equation_AI_Rating_Equipment(   i32 crit);
i32 Equation_AI_Rating_pushGradient(i32 gradient);
i32 Equation_AI_Rating_pullGradient(i32 gradient);

#endif /* EQUATIONS_H */
