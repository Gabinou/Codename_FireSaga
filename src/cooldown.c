
#include "cooldown.h"
#include "SDL.h"

b32 isOnCooldown(const struct Cooldown *const cooldown) {
    return (cooldown->left > 0);
}

void Cooldown_Tick(struct Cooldown *cooldown) {
    SDL_Log(__func__);
    if (cooldown->left > 0) {
        cooldown->left--;
    }
}

void Cooldown_Set(  Cooldown *cooldown, i32 ticks) {
    cooldown->ticks = ticks;
}

void Cooldown_Start(struct Cooldown *cooldown) {
    cooldown->left = cooldown->ticks;
}
