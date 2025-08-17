
#ifndef MAP_ONTILE_H
#define MAP_ONTILE_H

#include "map/map.h"
#include "tnecs.h"

/* --- FORWARD DECLARATIONS --- */
struct Game;
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
int entity_isIn(u64 *array, u64 to_find, size_t arr_len);

/* --- Units --- */
void Map_startingPos_Add(       struct Map *map, i32 col, i32 row);
void Map_addArmy(struct Map *map,  u8 army);

void  Map_Unit_Put(        struct Map *map, u8 c, u8 r, tnecs_E u);
void _Map_Unit_Put(        struct Map *map, u8 c, u8 r, tnecs_E u);
void  Map_Unit_Move(       struct Map *map, u8 x, u8 y, u8 new_x, u8 new_y);
void  Map_Unit_Swap(       struct Map *map, u8 x, u8 y, u8 new_x, u8 new_y);
void  Map_Unit_Remove(     struct Map *map, tnecs_E entity);

tnecs_E _Map_Unit_Remove_List(struct Map *map, tnecs_E entity);
tnecs_E _Map_Unit_Remove_Map( struct Map *map, u8 col, u8 row);

tnecs_E  Map_Unit_Get(     struct Map *map, u8 col, u8 row);
tnecs_E *Map_Unit_Gets(    struct Map *map, u8 army);
tnecs_E  Map_Unit_Get_Boss(struct Map *map, u8 army);

/* --- Map Objects --- */
void Map_Door_onOpen(       struct Map *map, tnecs_E d);
void Map_Chest_onOpen(      struct Map *map, tnecs_E c);
void Map_Breakable_onBroken(struct Map *map, tnecs_E b);

#endif /* MAP_ONTILE_H */
