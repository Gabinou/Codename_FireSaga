#ifndef COOLDOWN_H
#define COOLDOWN_H

#include "types.h"

typedef struct Cooldown {
    /* Ex: Number of turns to use weapon again  */

    i32 ticks;  /* at start, left set to ticks  */
    i32 left;   /* if > 0, on cooldown          */
} Cooldown;

/* Note: if it's on cooldown, it can't be used */
b32 isOnCooldown(const Cooldown *const cooldown);

void Cooldown_Tick( Cooldown *cooldown);
void Cooldown_Set(  Cooldown *cooldown, i32 ticks);
void Cooldown_Start(Cooldown *cooldown);

#endif /* COOLDOWN_H */
