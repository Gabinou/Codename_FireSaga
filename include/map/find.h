#ifndef MAP_FIND_H
#define MAP_FIND_H

#include "nmath.h"
#include "map/map.h"

/* --- FORWARD DECLARATIONS --- */
struct Map;

/*-- Map Usable -- */
void Map_Find_Usable(struct Map *m, tnecs_world_t *w, tnecs_entity_t ent, bool mo, int a);

/* -- Find at tile -- */
tnecs_entity_t Map_Find_Door_Ent(     struct Map *map, i32 x, i32 y);
tnecs_entity_t Map_Find_Chest_Ent(    struct Map *map, i32 x, i32 y);
tnecs_entity_t Map_Find_Breakable_Ent(struct Map *map, i32 x, i32 y);

/* -- Find at other tile  -- */
/* - attackmap - */
tnecs_entity_t *Map_Find_Patients(struct Map *map, struct dtab *d, i32 *h,
                                  tnecs_entity_t *p, tnecs_entity_t he, bool q);
tnecs_entity_t *Map_Find_Breakables(struct Map *map, i32 *al, tnecs_entity_t *a, bool q);
tnecs_entity_t *Map_Find_Defendants(struct Map *map, i32 *al, tnecs_entity_t *a,
                                    tnecs_entity_t agg, bool q);

/* - neighbours - */
tnecs_entity_t *Map_Find_Victims(struct Map *map, tnecs_entity_t *a, i32 x, i32 y,
                                 tnecs_entity_t savior);

tnecs_entity_t *Map_Find_Traders(   struct Map *map, tnecs_entity_t *a, i32 x, i32 y);
tnecs_entity_t *Map_Find_Auditors(  struct Map *map, tnecs_entity_t *a, i32 x, i32 y);
tnecs_entity_t *Map_Find_Spectators(struct Map *map, tnecs_entity_t *a, i32 x, i32 y);

tnecs_entity_t *Map_Find_Doors( struct Map *map, tnecs_entity_t *, i32 x, i32 y);
tnecs_entity_t *Map_Find_Chests(struct Map *map, tnecs_entity_t *, i32 x, i32 y);

#endif /* MAP_FIND_H */
