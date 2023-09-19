#ifndef COMBAT_HP_BAR_H
#define COMBAT_HP_BAR_H

#include "structs.h"
#include "SDL2/SDL.h"

/* --- COMBAT_HPBAR --- */
// Bar drawn using repeated textures, representing different types of health.
// Each texture is one independent HP.
// - Filled
// - Empty
// - Overfilled
// - Healed
// - Physical Damaged
// - Magical Damaged
// - True Damaged

struct BarDamage {
    uint32_t dmg[DAMAGE_TYPES];
    uint32_t previoushp;
};

struct Combat_HPBar {
    SDL_Texture *texture;
    SDL_Texture *textures;
    struct HP hp;
    struct BarDamage damage;
};
extern struct Combat_HPBar Combat_HPBar_default;

#endif /* BARS_H */
