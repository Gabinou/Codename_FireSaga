#ifndef COMBAT_HP_BAR_H
#define COMBAT_HP_BAR_H

#include "structs.h"
#include "SDL.h"

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
    u32 dmg[DMG_TYPES];
    u32 previoushp;
};

struct Combat_HPBar {
    SDL_Texture *texture;
    SDL_Texture *textures;
    struct HP hp;
    struct BarDamage damage;
};
extern const struct Combat_HPBar Combat_HPBar_default;

#endif /* BARS_H */
