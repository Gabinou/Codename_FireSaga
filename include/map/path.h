#ifndef MAP_PATH_H
#define MAP_PATH_H

#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Map;


/* --- Dangermap --- */
// Dangermap: Player chooses enemies to add
void Map_Danger_Add(                struct Map *map, i32 *danger);
void Map_Danger_Sub(                struct Map *map, i32 *danger);
void Map_Danger_Reset(              struct Map *map);
i32 *Map_Danger_Compute(            struct Map *map, tnecs_E u);
void Map_Stacked_Dangermap_Reset(   struct Map *map);
void Map_Stacked_Dangermap_Compute( struct Map *map, i32 *danger);

/* --- Global_Dangermap --- */
// Global_Dangermap: All enemies added to dangermap upon toggle by Player
void Map_Global_Danger_Add(       struct Map *map, i32 *danger);
void Map_Global_Danger_Sub(       struct Map *map, i32 *danger);
void Map_Global_Danger_Reset(     struct Map *map);
void Map_Global_Dangermap_Reset(  struct Map *map);
void Map_Global_Dangermap_Compute(struct Map *map);

/* --- costmap --- */
// Should costmap computation be skipped?
b32  Map_Costmap_Skip(struct Map *map, tnecs_E u);
// Makes the costmap be recomputed always next Map_Costmap_Movement_Compute call
void Map_Costmap_Wipe(struct Map *map);
i32   * Map_Costmap_Movement_Compute(   struct Map *map, tnecs_E u);
i32   * Map_Costmap_PushPull_Compute(   struct Map *map, tnecs_E u);
i32   *_Map_Costmap_Movement_Compute(   struct Map *map, struct Unit *u);

/* --- Perimeter --- */
Padding *Map_PerimeterM(i32 *map, i32 row_len, i32 col_len);
void Map_Perimeter(Padding *edges, i32 *map, i32 row_len, i32 col_len);

void Map_Danger_Perimeter_Compute(struct Map *map, i32 *danger);

/* --- Movemap --- */
// Map_Movemap_Compute also computes REQUIRED costmap
i32   * Map_Movemap_Compute( struct Map *map, tnecs_E u);
i32   *_Map_Movemap_Compute( struct Map *map, struct Point s, i32 mv);

/* --- Map_Act_To --- */
// Tiles that unit can perform action TO
// - Attack unit on tile -> attacktomap
// - Heal   unit on tile -> healtomap
i32 *Map_Act_To(struct Map *map, MapAct map_to);

/* --- Map_Act_From --- */
// Tiles the unit can perform action FROM
// - Attack unit from tile -> attackfrommap
// - Heal   unit from tile -> healfrommap
i32 *Map_Act_From(struct Map *map, MapAct map_from);

/* --- global_rangemap --- */
void Map_globalRange(struct Map *map, u8 alignment);

#endif /* MAP_PATH_H */
