
#include "cooldown.h"

b32 isOnCooldown(const struct Cooldown *const cooldown) {
    return (cooldown->left > 0);
}

void Cooldown_Tick(struct Cooldown *cooldown) {
    if (cooldown->left > 0) {
        cooldown->left--;
    }
}

void Cooldown_Start(struct Cooldown *cooldown) {
    cooldown->left = cooldown->ticks;
}
