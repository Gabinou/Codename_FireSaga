#ifndef COOLDOWN_H
#define COOLDOWN_H

#include "struct.h"

/* Note: if it's on cooldown, it can't be used */
b32 isOnCooldown(const struct Cooldown *const cooldown);

void Cooldown_Tick( struct Cooldown *cooldown);
void Cooldown_Start(struct Cooldown *cooldown);

#endif /* COOLDOWN_H */
