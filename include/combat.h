#ifndef COMBAT_H
#define COMBAT_H

#include "SDL2/SDL.h"
#include "types.h"
#include "enums.h"
#include "structs.h"
#include "RNG.h"
#include "nmath.h"
#include "unit/unit.h"

/* --- FORWARD DECLARATIONS --- */
struct Game;
struct Unit;

/* --- GLOSSARY ---
* - Combatants: Either aggressor or defendant
* - Aggressor: I initiate combat against an enemy       (abrev: agg)
* - Defendant: Enemy initiates combat against me        (abrev: dft)
*   -> aggressor and defendant DON'T CHANGE during combat
* - Attacker: I am attacking during combat              (abrev: att)
* - Defender: I am getting attacked during combat       (abrev: dfd)
*   -> attacker and defender CHANGES EVERY ATTACK during combat
* - Phase: attacker starts attacking defender n times
*   -> after phase is done, ATTACKER and DEFENDER switch
* - Attack: inflict damage upon enemy with current loadout
*   -> Each combat phase can have n attack, depending on astra, brave, skills
*
* --- ATTACKING PRIORITY/HIERARCHY ---
* 0. Combat phase (Doubles or not?)
* 1. Brave effect (multiplies number of attacks)
*      -> APPLIES TO ALL PREVIOUS ATTACKS
* 2. Astra effect (multiplies number of attacks)
*      -> MAY APPLY TO EACH PREVIOUS ATTACK
* 3. Corsair Viscious skill. Doubles attack of first phase.
*
* -- total number of attacks --
* 1. Do I double/triple? -> n_phase
* 2. Do I brave? -> brave_factor * n_phase
* 3. For every (brave * n): Do I astra? -> (brave_factor * n) * (astra_factor * n_astra)
* 4. For every (brave * n): Do I have an attack multiplier skill?
* What other skills may multiply attacks?
*   a. Fencer skill Swift thrust basically brave effect for first phase.
*   b. Corsair skill Viscious. Double attacks if dual wielding axes.
*/

/* --- STRUCTS --- */
/* -- Combat_Phase -- */
// Total attack num in phase = for i < brave_factor -> SUM(skillp_multipliers[i]) * skill_multiplier
// skill_multiplier and skillp_multipliers stack, BUT -> no skills should use both.
struct Combat_Phase {
    // skillp_multipliers: Different multiplier for every brave applies to every attack in phase
    u8     skillp_multipliers[SOTA_BRAVE_MAX];
    // skill_multiplier: Applies to every higher priority attack in phase
    u8     skill_multiplier;
    u8     attack_num;
    b32    attacker;
};
extern struct Combat_Phase Combat_Phase_default;

/* -- Combat_Attack -- */
struct Combat_Attack {
    u8     total_damage; // total damage taken, depending on hit/crit
    b32    hit;
    b32    crit;
    b32    attacker;
};
extern struct Combat_Attack Combat_Attack_default;

/* -- Combat_Flow -- */
// Number of combat phases initiated by each combatants
// (aggressor_phases > 1) -> (defendant_phases <= 1) and vice versa
// Brave: attack multiplier combat for all phases
struct Combat_Flow {
    b32    defendant_retaliates;
    u8     defendant_phases;
    u8     aggressor_phases;
    u8     aggressor_brave;
    u8     defendant_brave;
};
extern struct Combat_Flow Combat_Flow_default;

/* -- Combat_Death -- */
// Can combatants die?
// WILL combatants die?
struct Combat_Death {
    b32 aggressor_certain;
    b32 defendant_certain;
    b32 aggressor_possible;
    b32 defendant_possible;
};
extern struct Combat_Death Combat_Death_default;

/* -- Combat_Rates -- */
struct Combat_Rates {
    u8 hit;
    u8 crit;
} ;
extern struct Combat_Rates Combat_Rates_default;

/* -- Combat_Stats -- */
// All combatant stats related to combats
struct Combat_Stats {
    struct Combat_Rates     agg_rates;
    struct Combat_Rates     dft_rates;
    struct Damage           agg_damage;
    struct Damage           dft_damage;
    struct Computed_Stats   agg_stats;
    struct Computed_Stats   dft_stats;
    i8                      agg_equipment[UNIT_HANDS_NUM];
    i8                      dft_equipment[UNIT_HANDS_NUM];
};
extern struct Combat_Stats Combat_Stats_default;

/* -- Combat_Forecast -- */
// All stats required to predict how combat will go,
// before actually doing the RNG check.
struct Combat_Forecast {
    struct Combat_Flow      flow;
    struct Combat_Death     death;
    struct Combat_Stats     stats;
    u8                      phase_num;
    u8                      attack_num;
};
extern struct Combat_Forecast Combat_Forecast_default;

/* -- Combat_Outcome -- */
// Actual phases and attacks that happen during combat
/* RNG CHECK HAPPENS HERE. */
struct Combat_Outcome {
    struct Combat_Phase   phases[SOTA_COMBAT_MAX_PHASES];
    struct Combat_Attack *attacks;
    b32                   ended; /* death before all attacks */
};
extern struct Combat_Outcome Combat_Outcome_default;

/* -- isCan -- */
b32 Combat_canDouble(struct Unit *_a, struct Unit *_d);
b32 Combat_canAttack_Equipped(struct Unit  *agg, struct Unit  *dft,
                              struct Point *_ag, struct Point *_df);

/* -- Combat Death -- */
struct Combat_Death Compute_Combat_Death(struct Unit *agg, struct Unit *dft,
                                         struct Combat_Stats, struct Combat_Flow);

/* -- Combat Attacks -- */
void Compute_Combat_Phase( struct Combat_Phase *, struct Combat_Attack *,
                           struct Damage, struct Unit *, u8, u8, u8);
void Compute_Combat_Attack(struct Combat_Phase *, struct Combat_Attack *,
                           struct Damage, struct Unit *, u8, u8);

int Combat_Attack_Total_Num(struct Combat_Phase *phases, int b, int num);
int Combat_Phase_Attack_Num(struct Combat_Phase *phases, int brave_factor);

/* -- Combat Forecast -- */
struct Damage          Compute_Combat_Damage(  struct Unit *att, struct Unit *dfd);
struct Combat_Flow     Compute_Combat_Flow(    struct Unit *agg, struct Unit *dft,
                                               struct Point *_a, struct Point *_d);
struct Combat_Rates    Compute_Combat_Rates(   struct Unit *att, struct Unit *dfd,
                                               struct Point *_a, struct Point *_d);
struct Combat_Forecast Compute_Combat_Forecast(struct Unit *agg, struct Unit *dft,
                                               struct Point *ap, struct Point *dp);

/* -- Combat Outcome -- */
/* RNG check happens here. */
void Compute_Combat_Outcome(struct Combat_Outcome *, struct Combat_Forecast *,
                            struct Unit *,           struct Unit *);

/* -- Combat Resolution -- */
/* Dealing damage happens here. */
void Combat_Resolve(struct Combat_Attack *, u8 a, struct Unit *, struct Unit *);
void Combat_Resolve_Attack(struct Combat_Attack,  struct Unit *, struct Unit *);

#endif /* COMBAT_H */
