#ifndef MAP_FIND_H
#define MAP_FIND_H

#include "structs.h"
#include "tnecs.h"

/* --- FORWARD DECLARATIONS --- */
struct Map;
struct Unit;

/*-- Map Usable -- */
// move: find canEquip including on moveable tiles or not
// Note: need Es/world, to find defendants
struct Unit_Equippable Map_canEquip(struct Map    *map,
                                    tnecs_E   ent,
                                    canEquip       can_equip);
b32 Map_canEquip_Range(struct Map       *map,
                       tnecs_E      unit_ent,
                       tnecs_E     *defendants,
                       struct canEquip   can_equip);

/* -- Find at tile -- */
tnecs_E Map_Find_Door_Ent(     struct Map *map, i32 x, i32 y);
tnecs_E Map_Find_Chest_Ent(    struct Map *map, i32 x, i32 y);
tnecs_E Map_Find_Breakable_Ent(struct Map *map, i32 x, i32 y);

/* -- Find at other tile  -- */
/* - attackmap - */
// TODO: need to find defendants/patients with WHOLE EQUIPMENT, or ONE ITEM
// TODO: Map_Find(map, map_find) -> Same as Map_Act_To

tnecs_E *Map_Find_Patients(struct Map *map, MapFind mapfind);
tnecs_E *Map_Find_Breakables(struct Map *map, i32 *al, tnecs_E *a, b32 q);
tnecs_E *Map_Find_Defendants(struct Map *map, MapFind mapfind);

/* - neighbours - */
tnecs_E *Map_Find_Victims(struct Map *map, tnecs_E *a, i32 x, i32 y,
                          tnecs_E savior);

tnecs_E *Map_Find_Traders(   struct Map *map, tnecs_E *a, i32 x, i32 y);
tnecs_E *Map_Find_Auditors(  struct Map *map, tnecs_E *a, i32 x, i32 y);
tnecs_E *Map_Find_Spectators(struct Map *map, tnecs_E *a, i32 x, i32 y);

tnecs_E *Map_Find_Doors( struct Map *map, tnecs_E *, i32 x, i32 y);
tnecs_E *Map_Find_Chests(struct Map *map, tnecs_E *, i32 x, i32 y);

/* - Closest - */
tnecs_E _Map_Find_Unit_Closest(struct Map *map, u8 army, i32 x, i32 y);
tnecs_E Map_Find_Enemy_Closest(struct Map *map, i32 x, i32 y);
tnecs_E Map_Find_Friendly_Closest(struct Map *map, i32 x, i32 y);

#endif /* MAP_FIND_H */
