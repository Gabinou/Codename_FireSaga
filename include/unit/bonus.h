#ifndef UNIT_BONUS_H
#define UNIT_BONUS_H

#include "enums.h"
#include "tnecs.h"
#include "structs.h"

/* Unit Bonus stats
*   - Aura, items, supports, etc. apply bonus stats to units
*   - Each bonus stat is added through this API.
*   - Each bonus stat includes conditions for decay/removal
*/

/* --- Adding --- */
// Add bonus, without checking      e.g. stacks
void Unit_Bonus_Add(Unit *unit, Bonus_Stats bonus);
// Adds bonus or refresh turn count e.g. doesn't stack
void Unit_Bonus_Refresh(Unit *unit, Bonus_Stats bonus);

Bonus_Stats Unit_Bonus_Total(Unit *unit);

/* --- Decay/Removal --- */
/* Instant aura: You only get it if you're in range at all times.
 * - Need to be called for all units every unit move
 */
void Unit_Bonus_Instant_Decay(Unit *unit);

/* Persistent aura: Only gets removed at turn end, even if it lasts 0 turns.
 * - Need to be called for all units every end turn
*/
void Unit_Bonus_Persistent_Decay(Unit *unit);

/* --- Utilities --- */
Bonus_Stats Aura2Bonus(Aura *a, tnecs_E u, u16 i, u16 s, b32 ac, b32 in);

b32 Bonus_Stats_isEqual(Bonus_Stats bonus1, Bonus_Stats bonus2);

/* --- Supports --- */
struct Bonus_Stats Unit_supportBonus(struct Unit *unit);

#endif /* UNIT_BONUS_H */
