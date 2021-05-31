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
    uint16_t effective; // unit type.
    struct Infusion infused;
    bool infuseable;
    bool attackable;
} Weapon;
extern struct Weapon Weapon_default;

struct Weapons_hash {
    int16_t key;
    struct Weapon value;
};

typedef struct WeaponModule {
    ECS_DECLARE_COMPONENT(Weapon);
} WeaponModule;

void WeaponModuleImport(ecs_world_t * in_world);
#define WeaponModuleImportHandles(handles)\
    ECS_IMPORT_COMPONENT(handles, Weapon);

extern bool Weapon_canInfuse(struct Weapon * in_weapon);
extern void Weapon_Infuse(struct Weapon * in_weapon, uint8_t in_mag, uint16_t in_type);
extern void Weapon_attackablefromType(struct Weapon * in_weapon);
extern void Weapon_readJSON(struct Weapon * in_weapon, cJSON * in_json);
extern void Weapon_writeJSON(struct Weapon * in_weapon, cJSON * in_json);

#endif /* WEAPON_H */
