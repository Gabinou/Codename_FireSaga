#ifndef COOLDOWN_H
#define COOLDOWN_H

#include "types.h"
// How to actually make all items properly decrement cooldowns at end turn?
//  - Cooldown is a component
//      - Inventory_Item MUST be an entity
//      - Run in separate tnecs_pipeline
//      + Easier to extend
//      - Hard to change inventory implementation
//  - Cooldown is in Inventory_Item
//      - Iterate over every item in equipment of every unit in army
//      - Run in end_turn event.
//      + Easier to implement now
//      - Hard to extend

// What other things need to happen on turn end/start that can benefit from tnecs pipelines?
// - Unit refresh.
// - Status effects.
//      - Units are already entities
//      - Status effects -> component

typedef struct Cooldown {
    /* Ex: Number of turns to use weapon again  */

    i32 ticks;  /* at start, left set to ticks  */
    i32 left;   /* if > 0, on cooldown          */
} Cooldown;

/* Note: if it's on cooldown, it can't be used */
b32 isOnCooldown(const struct Cooldown *const cooldown);

void Cooldown_Tick( struct Cooldown *cooldown);
void Cooldown_Start(struct Cooldown *cooldown);

#endif /* COOLDOWN_H */
