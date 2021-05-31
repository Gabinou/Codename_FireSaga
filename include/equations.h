#ifndef EQUATIONS_H
#define EQUATIONS_H

#include "enums.h"
#include "narrative.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <limits.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#define equation_multiplyDamage(in_dmg, multiplier) ((in_dmg) > (uint8_t)(UINT8_MAX / (multiplier)) ? UINT8_MAX : (uint8_t)((in_dmg) * (multiplier)))

extern float equation_expectedDamage(uint8_t in_hit_rate, uint8_t in_crit_rate, uint8_t in_critfactor);
extern int_point_t equation_inbounds(int_point_t pos, int_point_t boundmin, int_point_t boundmax);
extern bool equation_canCarry(uint8_t rescuer_con, uint8_t rescuee_con);

extern uint8_t equation_rateHit(uint8_t attacker_hit, uint8_t defender_avoid);
extern uint8_t equation_rateCrit(uint8_t attacker_crit, uint8_t defender_favor);
extern uint8_t equation_attackDamage(uint8_t attacker_dmg, uint8_t defender_def);

extern uint8_t equation_unitHit(uint8_t wpn_hit, uint8_t dex, uint8_t luck, uint8_t support);
extern int8_t equation_unitSpeed(uint8_t wpn_wgt, uint8_t agi, uint8_t con, uint8_t str);
extern uint8_t equation_unitFavor(uint8_t wpn_favor, uint8_t luck, uint8_t support);
extern uint8_t equation_unitCrit(uint8_t wpn_crit, uint8_t dex, uint8_t luck, uint8_t support);
extern int8_t equation_unitDodge(uint8_t wpn_dodge, uint8_t wpn_wgt, uint8_t luck, uint8_t agi, uint8_t str, uint8_t con, uint8_t tile_dodge, uint8_t support);
extern uint8_t equation_Unit_Healshp(uint8_t total_hp, uint8_t heal_percent);
extern uint8_t equation_Unit_Healshpvar(size_t argnum, ...);

extern uint8_t equation_combatDamage(uint8_t attackerPorMAttack, uint8_t defenderDeforRes, uint8_t effective_multiplier, uint8_t critp_multiplier, bool crit);
extern uint8_t equation_attack(uint8_t Lwpn_PorMmight, uint8_t Rwpn_PorMmight);
extern uint8_t equation_attackvar(size_t argnum, ...);
extern uint8_t equation_defense(uint8_t Lwpn_deforres, uint8_t Rwpn_deforres);
extern uint8_t equation_defensevar(size_t argnum, ...);
extern uint8_t equation_wpnHit(uint8_t Lwpn_hit, uint8_t Rwpn_hit);
extern uint8_t equation_wpnHitvar(size_t argnum, ...);
extern uint8_t equation_wpnDodge(uint8_t Lwpn_dodge, uint8_t Rwpn_dodge);
extern uint8_t equation_wpnDodgevar(size_t argnum, ...);
extern uint8_t equation_wpnCrit(uint8_t Lwpn_crit, uint8_t Rwpn_crit);
extern uint8_t equation_wpnCritvar(size_t argnum, ...);
extern uint8_t equation_wpnFavor(uint8_t Lwpn_favor, uint8_t Rwpn_favor);
extern uint8_t equation_wpnFavorvar(size_t argnum, ...);
extern uint8_t equation_wpnWgt(uint8_t Lwpn_wgt, uint8_t Rwpn_wgt);
extern uint8_t equation_wpnWgtvar(size_t argnum, ...);

extern uint8_t equation_agonyTurns(uint8_t str, uint8_t def, uint8_t con);
extern uint8_t equation_agonyPercentonCrit(uint8_t luck, uint8_t con);

extern int8_t equation_AI_Rating_hitRate(uint8_t in_hit_rate);
extern int8_t equation_AI_Rating_critRate(uint8_t in_crit_rate);
extern int8_t equation_AI_Rating_Damage(uint8_t in_dmg);
extern int8_t equation_AI_Rating_Stats(uint8_t in_stat);
extern int8_t equation_AI_Rating_HPprop(float in_hp_prop);
extern int8_t equation_AI_Rating_pushGradient(int_tile_t gradient);
extern int8_t equation_AI_Rating_pullGradient(int_tile_t gradient);
// extern int8_t equation_AI_Rating_Equipment(uint8_t in_crit);

extern struct Unit_stats Unit_stats_plus(struct Unit_stats in_stats1, struct Unit_stats in_stats2);
extern struct Unit_stats Unit_stats_minus(struct Unit_stats in_stats1, struct Unit_stats in_stats2);

#ifdef __cplusplus
}
#endif

#endif /* EQUATIONS_H */
