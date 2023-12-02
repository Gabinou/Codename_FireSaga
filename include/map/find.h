#ifndef MAP_FIND_H
#define MAP_FIND_H

#include "nmath.h"
#include "map/map.h"

/* --- FORWARD DECLARATIONS --- */
struct Map;

/*-- Map Usable -- */
void Map_Find_Usable(struct Map *m, tnecs_world *w, tnecs_entity ent, bool mo, int a);

/* -- Find at tile -- */
tnecs_entity Map_Find_Door_Ent(     struct Map *map, i32 x, i32 y);
tnecs_entity Map_Find_Chest_Ent(    struct Map *map, i32 x, i32 y);
tnecs_entity Map_Find_Breakable_Ent(struct Map *map, i32 x, i32 y);

/* -- Find at other tile  -- */
/* - attackmap - */
tnecs_entity *Map_Find_Patients(struct Map *map, struct dtab *d, i32 *h,
                                tnecs_entity *p, tnecs_entity he, bool q);
tnecs_entity *Map_Find_Breakables(struct Map *map, i32 *al, tnecs_entity *a, bool q);
tnecs_entity *Map_Find_Defendants(struct Map *map, i32 *al, tnecs_entity *a,
                                  tnecs_entity agg, bool q);

/* - neighbours - */
tnecs_entity *Map_Find_Victims(struct Map *map, tnecs_entity *a, i32 x, i32 y,
                               tnecs_entity savior);

tnecs_entity *Map_Find_Traders(   struct Map *map, tnecs_entity *a, i32 x, i32 y);
tnecs_entity *Map_Find_Auditors(  struct Map *map, tnecs_entity *a, i32 x, i32 y);
tnecs_entity *Map_Find_Spectators(struct Map *map, tnecs_entity *a, i32 x, i32 y);

tnecs_entity *Map_Find_Doors( struct Map *map, tnecs_entity *, i32 x, i32 y);
tnecs_entity *Map_Find_Chests(struct Map *map, tnecs_entity *, i32 x, i32 y);

/* - Closest - */
tnecs_entity _Map_Find_Unit_Closest(struct Map *map, u8 army, i32 x, i32 y);
tnecs_entity Map_Find_Enemy_Closest(struct Map *map, i32 x, i32 y);
tnecs_entity Map_Find_Friendly_Closest(struct Map *map, i32 x, i32 y);

#endif /* MAP_FIND_H */
