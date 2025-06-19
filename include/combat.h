#ifndef COMBAT_H
#define COMBAT_H

#include <math.h>
#include "types.h"
#include "enums.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Game;
struct RNG_Sequence;
struct Unit;
struct Point;

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

/* -- isCan -- */
b32 Combat_canDouble(Computed_Stats cs_att, Computed_Stats cs_dfd);
b32 Combat_canAttack_Equipped(Unit  *agg,
                              Point *_ag,
                              Point *_df);

/* -- Combat Death -- */
Combat_Death Compute_Combat_Death(Unit *agg,
                                  Unit *dft,
                                  Combat_Stats s,
                                  Combat_Flow f);

/* -- Combat Attacks -- */
void Compute_Combat_Phase( Combat_Phase *, Combat_Attack *,
                           Combat_Damage, Unit *, u8, u8, u8);
void Compute_Combat_Attack(Combat_Phase *, Combat_Attack *,
                           Combat_Damage, Unit *, u8, u8);

int Combat_Attack_Total_Num(Combat_Phase *phases, int b, int num);
int Combat_Phase_Attack_Num(Combat_Phase *phases, int brave_factor);

/* -- Combat Forecast -- */
Combat_Damage          Compute_Combat_Damage(Unit *att, Unit *dfd,
                                             Computed_Stats cs_agg,
                                             Computed_Stats cs_dft);
Combat_Flow     Compute_Combat_Flow(    Unit *agg, Unit *dft,
                                        Computed_Stats cs_agg, Computed_Stats cs_dft,
                                        Point *_a, Point *_d);
Combat_Rates    Compute_Combat_Rates(   Computed_Stats cs_agg, Computed_Stats cs_dft);
Combat_Forecast Compute_Combat_Forecast(Unit *agg, Unit *dft,
                                        Point *ap, Point *dp);

/* -- Combat Outcome -- */
/* RNG check happens here. */
void Compute_Combat_Outcome(Combat_Outcome *, Combat_Forecast *,
                            Unit *,           Unit *);

/* -- Combat Resolution -- */
/* Dealing damage happens here. */
void Combat_Resolve(Combat_Attack *, u8 a, Unit *, Unit *);
void Combat_Resolve_Attack(Combat_Attack,  Unit *, Unit *);

#endif /* COMBAT_H */
