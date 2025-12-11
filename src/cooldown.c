
#include "cooldown.h"
#include "SDL.h"

b32 isOnCooldown(const Cooldown *const cooldown) {
    return (cooldown->left > 0);
}

void Cooldown_Tick(Cooldown *cooldown) {
    if (cooldown->left > 0) {
        cooldown->left--;
    }
}

void Cooldown_Set(  Cooldown *cooldown, i32 ticks) {
    cooldown->ticks = ticks;
}

void Cooldown_Start(Cooldown *cooldown) {
    cooldown->left = cooldown->ticks;
}
