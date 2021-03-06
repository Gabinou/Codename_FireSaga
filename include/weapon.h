#ifndef WEAPON_H
#define WEAPON_H

#include "item.h"
#include "utilities.h"
#include "filesystem.h"
#include <stdint.h>

extern SDL_Renderer * Game_renderer;

typedef struct Weapon {
    uint8_t json_element;
    struct Item * item; // Should be a pointer to an item?
    struct Weapon_stats stats;
    int_unitType_t effective;
    struct Infusion infused;
    bool infuseable;
    bool attackable;
} Weapon;
extern struct Weapon Weapon_default;

struct Weapons_hash {
    int16_t key;
    struct Weapon value;
};

extern bool Weapon_canInfuse(struct Weapon * in_weapon);
extern void Weapon_Infuse(struct Weapon * in_weapon, uint8_t in_mag, uint16_t in_type);
extern void Weapon_attackablefromType(struct Weapon * in_weapon);
extern void Weapon_readJSON(struct Weapon * in_weapon, cJSON * in_json);
extern void Weapon_writeJSON(struct Weapon * in_weapon, cJSON * in_json);

#endif /* WEAPON_H */
