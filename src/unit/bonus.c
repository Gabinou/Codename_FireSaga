
#include "unit/bonus.h"

// TODO: Same functions for maluses

/* Increment turn and remove expired bonuses from stack */
void Unit_Bonus_Turn(struct Unit *unit) {

}

/* Compute support bonus from supporter at order, add to bonus_stack */
/* Range check should be done previously. */
void Unit_Bonus_Support_Add(struct Unit *unit, i16 _id) {

}

/* Check if Any bonus comes from weapon.
If so, check if still in hand.
If not in hand, remove bonus from bonus_Stack */
void Unit_Bonus_Weapon_Refresh(struct Unit *unit, i16 _id) {

}

/* Iterate over every friendly:
    - Check if supporting friendly in range
    - If so refresh support in bonus_stack
    - If not remove from bonus_stack
    */
void Game_Bonus_Supports_Refresh(struct Game *sota) {


}

/* Check if any bonus comes from a passive aura (standard)
If so, check conditions.
If not satisfied, remove from bonus_stack  */
void Game_Bonus_Aura_Standard_Refresh(struct Game *sota) {

}
