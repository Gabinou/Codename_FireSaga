
#ifndef MAP_ONTILE_H
#define MAP_ONTILE_H

#include "map/map.h"

/* --- FORWARD DECLARATIONS --- */
struct Map;

// How to check for events, objects?
// list: Every move, check all list elem. need loop.
// 2D array: Every move, check if thing on map. need memory
// DTAB: Every move, check if elem is NULL. need loop
// -> List I guess
// -> Split lists into different "trigger Time"

// INTERACTIONs
// Chests,
// Door
//  ->through menus (specific commands Open)
// Breakable
//  ->through menus (Attack Command)
//
// Map events occur when :
//      unit moved
//      unit died
//      unit recruited

//
// After unit moved:
// Check attack map for
//      - Attackable Breakable -> UnitAction menu
//      - Attackable Units -> UnitAction menu
// Check neighbour tiles for
//      - Openable Doors -> UnitAction menu
//      - Openable Chests -> UnitAction menu
// Check Current Tiles for
//      - Openable Chest -> UnitAction menu
//      - Event -> Play now.

// Openable-> list of 2D positions
// Attackable-> list of 2D positions

/* --- Entity finders --- */
bool entity_isIn(    u64 *array, u64 to_find, size_t arr_len);
size_t *entity_where(u64 *array, u64 to_find, size_t arr_len);

/* --- Units --- */
extern void Map_startingPos_Add(       struct Map *map, i32 col, i32 row);
extern void Map_addArmy(struct Map *map, const uf8 army);

extern void Map_Unit_Put(      struct Map *map, tnecs_world_t *w, uf8 c, uf8 r, tnecs_entity_t u);
extern void Map_Unit_Move(             struct Map *map, uf8 x, uf8 y, uf8 new_x, uf8 new_y);
extern void Map_Unit_Remove(           struct Map *map, tnecs_entity_t entity);
extern void Map_Unit_Remove_fromPos(   struct Map *map, uf8 col, uf8 row);
extern void Map_Unit_Remove_fromEntity(struct Map *map, tnecs_world_t *w, tnecs_entity_t entity);

extern tnecs_entity_t  Map_Unit_Get(   struct Map *map, uf8 col, uf8 row);
extern tnecs_entity_t *Map_Unit_Gets(  struct Map *map, tnecs_world_t *w, uf8 army);

/* --- Map Objects --- */
extern void Map_Door_onOpen(       struct Map *map, tnecs_world_t *w, tnecs_entity_t d);
extern void Map_Chest_onOpen(      struct Map *map, tnecs_world_t *w, tnecs_entity_t c);
extern void Map_Breakable_onBroken(struct Map *map, tnecs_world_t *w, tnecs_entity_t b);

#endif /* MAP_ONTILE_H */
