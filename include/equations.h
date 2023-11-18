#ifndef EQUATIONS_H
#define EQUATIONS_H

#include <stdarg.h>
#include <limits.h>
#include "enums.h"
#include "nmath.h"
#include "macros.h"
#include "debug.h"
#include "types.h"
#include "combat.h"
#include "structs.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Damage;

/* --- MACROS --- */
/* -- Unit Combat -- */
#define Equation_multiplyDamage(dmg, multiplier) ((dmg) > (u8)(UINT8_MAX / (multiplier)) ? UINT8_MAX : (u8)((dmg) * (multiplier)))

/* --- FUNCTION DEFINITIONS --- */
/* -- Unit Computed Stats -- */
u8 Equation_Unit_Hit(  int wpn_hit,   int dex,       int luck, int support);
u8 Equation_Unit_Crit( int wpn_crit,  int dex,       int luck, int support);
i8 Equation_Unit_Speed(int wpn_wgt,   int agi,       int con,  int str);
u8 Equation_Unit_Favor(int wpn_favor, int faith,     int support);
i8 Equation_Unit_Dodge(int wpn_wgt,   int wpn_dodge, int luck, int faith,
                       int agi,       int str,       int con,  int tile_dodge,
                       int support);

/* -- Stat total with bonus and malus -- */
u8 Stat_Total(int current, int bonus, int malus, int cap);

/* -- Staff Healing -- */
u8 Equation_Staff_Healing(int item_AP,  int user_mag);

/* -- Unit Healing -- */
u8 Equation_Unit_Healshp(int total_hp, int heal_percent);
u8 Equation_Unit_Healshpvar(size_t argnum, ...);

/* -- Unit Rescue -- */
bool Equation_canCarry(int savior_con, int victim_con);

/* -- Unit Regrets -- */
i8 Equation_Regrets(int kills, int faith);

/* -- Combat -- */
u8    Equation_Combat_Hit(     int hit_a,  int avoid_d);
u8    Equation_Combat_Crit(    int crit_a, int favor_d);
u8    Equation_Attack_Damage(  int dmg_a,  int def_d);
u8    Equation_Combat_Damage(  int att_a,  int def_d, int eff, int critp, bool crit);
float Equation_Expected_Damage(int hit,    int crit,  int critfactor);

/* -- Compute damage total from other damae types in struct -- */
void  Equation_Damage_Total(struct Damage *dmg);

/* -- Weapon -- */
u8 Equation_Weapon_Hit(    int Lwpn_hit,    int Rwpn_hit);
u8 Equation_Weapon_Wgt(    int Lwpn_wgt,    int Rwpn_wgt);
u8 Equation_Weapon_Crit(   int Lwpn_crit,   int Rwpn_crit);
u8 Equation_Weapon_Favor(  int Lwpn_favor,  int Rwpn_favor);
u8 Equation_Weapon_Dodge(  int Lwpn_dodge,  int Rwpn_dodge);
u8 Equation_Weapon_Attack( int Lwpn_attack, int Rwpn_attack);
u8 Equation_Weapon_defense(int Lwpn_def,    int Rwpn_def);

/* - argvar - */
u8 Equation_Weapon_Hitvar(    size_t argnum, ...);
u8 Equation_Weapon_Wgtvar(    size_t argnum, ...);
u8 Equation_Weapon_Critvar(   size_t argnum, ...);
u8 Equation_Weapon_Favorvar(  size_t argnum, ...);
u8 Equation_Weapon_Dodgevar(  size_t argnum, ...);
u8 Equation_Weapon_Attackvar( size_t argnum, ...);
u8 Equation_Weapon_Defensevar(size_t argnum, ...);

/* -- Agony -- */
u8 Equation_Agony_Turns(        int str, int def, int con);
u8 Equation_Agony_PercentonCrit(int luck, int con);

/* -- AI equations -- */
i8 Equation_AI_Rating_Stats(   int   stat);
i8 Equation_AI_Rating_Damage(  int   dmg);
i8 Equation_AI_Rating_HPprop(  float hp_prop);
i8 Equation_AI_Rating_hitRate( int   hit_rate);
i8 Equation_AI_Rating_critRate(int   crit_rate);

i8 Equation_AI_Rating_Equipment(   int crit);
i8 Equation_AI_Rating_pushGradient(i32 gradient);
i8 Equation_AI_Rating_pullGradient(i32 gradient);

#endif /* EQUATIONS_H */
