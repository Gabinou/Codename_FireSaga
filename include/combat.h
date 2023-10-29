#ifndef COMBAT_H
#define COMBAT_H

#include "types.h"
#include "enums.h"
#include "structs.h"
#include "RNG.h"
#include "nmath.h"
#include "SDL.h"
#include "SDL2/SDL_render.h"
#include "unit.h"

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
extern struct Combat_Phase {
    // skillp_multipliers: Different multiplier for every brave applies to every attack in phase
    u8     skillp_multipliers[SOTA_BRAVE_MAX];
    // skill_multiplier: Applies to every higher priority attack in phase
    u8     skill_multiplier;
    u8     attack_num;
    bool    attacker    : 1;
} Combat_Phase_default;

/* -- Combat_Attack -- */
extern struct Combat_Attack {
    u8     total_damage; // total damage taken, depending on hit/crit
    bool    hit         : 1;
    bool    crit        : 1;
    bool    attacker    : 1;
} Combat_Attack_default;

/* -- Combat_Flow -- */
// Number of combat phases initiated by each combatants
// (aggressor_phases > 1) -> (defendant_phases <= 1) and vice versa
// Brave: attack multiplier combat for all phases
extern struct Combat_Flow {
    bool    defendant_retaliates;
    u8     defendant_phases;
    u8     aggressor_phases;
    u8     aggressor_brave;
    u8     defendant_brave;
} Combat_Flow_default;

/* -- Combat_Death -- */
// Can combatants die?
// WILL combatants die?
extern struct Combat_Death {
    bool aggressor_certain;
    bool defendant_certain;
    bool aggressor_possible;
    bool defendant_possible;
} Combat_Death_default;

/* -- Combat_Rates -- */
extern struct Combat_Rates {
    u8 hit;
    u8 crit;
} Combat_Rates_default;

extern struct Combat_Stats {
    struct Combat_Rates     agg_rates;
    struct Combat_Rates     dft_rates;
    struct Damage           agg_damage;
    struct Damage           dft_damage;
    struct Computed_Stats   agg_stats;
    struct Computed_Stats   dft_stats;
    i8                     agg_equipment[UNIT_HANDS_NUM];
    i8                     dft_equipment[UNIT_HANDS_NUM];
} Combat_Stats_default;

struct Combat_Forecast {
    struct Combat_Flow      flow;
    struct Combat_Death     death;
    struct Combat_Stats     stats;
    u8                     phase_num;
    u8                     attack_num;
};

struct canAttack {
    bool iscan;
    bool with[DEFAULT_EQUIPMENT_SIZE]; // which weapon can unit attack with
};

/* --- API --- */

/* -- Combat Forecast -- */
extern struct Combat_Forecast Compute_Combat_Forecast(struct Unit *agg, struct Unit *dft,
                                                      const struct Point *ap, const struct Point *dp);

/* -- Combat Outcome -- */
extern void Compute_Combat_Outcome(struct Combat_Phase *,    struct Combat_Attack *,
                                   struct Combat_Forecast *, struct TINYMT32_T *,
                                   struct Unit *,            struct Unit *);

/* -- Combat Resolution -- */
/* - Execute all combat attacks - */
extern u8 Combat_Next_HP( struct Combat_Attack, u8 hp);
extern void Combat_Resolve(struct Combat_Attack *, u8 a, struct Unit *, struct Unit *);

/* --- INTERNALS --- */
/* -- isCan -- */
extern bool Combat_canDouble(const struct Unit *a, const struct Unit *d);
extern bool Combat_canAttack_Equipped(      struct Unit  *agg,           struct Unit  *dft,
                                            const struct Point *agg_pos, const struct Point *dft_pos);
extern void Combat_Death_isPossible(struct Combat_Flow, u8 *out);

/* -- Combat Forecast -- */
extern struct Damage Compute_Combat_Damage(struct      Unit *att, struct Unit *dfd);
extern struct Combat_Rates Compute_Combat_Rates(struct Unit  *att, struct Unit  *dfd,
                                                const struct Point *agg_pos, const struct Point *dft_pos);

extern struct Combat_Death Compute_Combat_Death(struct Unit *agg, struct Unit *dft,
                                                struct Combat_Stats, struct Combat_Flow);

extern struct Combat_Flow Compute_Combat_Flow(struct Unit *agg, struct Unit *dft,
                                              const struct Point *agg_pos, const struct Point *dft_pos);

/* -- Combat Attacks -- */
extern void
Compute_Combat_Phase(struct TINYMT32_T *, struct Combat_Phase *, struct Combat_Attack *,
                     struct Damage, struct Unit *, u8, u8, u8);
extern void Compute_Combat_Attack(struct TINYMT32_T *, struct Combat_Phase *,
                                  struct Combat_Attack *, struct Damage, struct Unit *, u8, u8);

extern int Combat_TotalAttack_Num( struct Combat_Phase *phases, int b, int num);
extern int Combat_Phase_Attack_Num(struct Combat_Phase *phase,  int brave_factor);

/* -- Combat Resolution -- */
/* - Execute one attack during combat - */
extern void Resolve_Attack(struct Combat_Attack, struct Unit *, struct Unit *);

#endif /* COMBAT_H */
