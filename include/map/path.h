#ifndef MAP_PATH_H
#define MAP_PATH_H

#include "structs.h"
#include "pathfinding.h"
#include "map/map.h"

/* --- FORWARD DECLARATIONS --- */
struct Map;

i32 *Map_Danger_Compute(struct Map *map, tnecs_entity u);

/* --- Dangermap --- */
// Dangermap: Player chooses enemies to add
void Map_Danger_Add(               struct Map *map, i32 *danger);
void Map_Danger_Sub(               struct Map *map, i32 *danger);
void Map_Danger_Reset(             struct Map *map);
void Map_Stacked_Dangermap_Reset(  struct Map *map);
void Map_Stacked_Dangermap_Compute(struct Map *map, i32 *danger);

/* --- Global_Dangermap --- */
// Global_Dangermap: All enemies added to dangermap upon toggle by Player
void Map_Global_Danger_Add(       struct Map *map, i32 *danger);
void Map_Global_Danger_Sub(       struct Map *map, i32 *danger);
void Map_Global_Danger_Reset(     struct Map *map);
void Map_Global_Dangermap_Reset(  struct Map *map);
void Map_Global_Dangermap_Compute(struct Map *map);

/* --- costmap --- */
i32   * Map_Costmap_Movement_Compute(struct Map *map, tnecs_entity u);
i32   *_Map_Costmap_Movement_Compute(struct Map *map, struct Unit *u);

i32   *Map_Costmap_PushPull_Compute( struct Map *map, tnecs_entity u);
float *Map_fCostmap_Movement_Compute(struct Map *map, tnecs_entity u);

/* --- Perimeter --- */
struct Padding *Map_PerimeterM(i32 *map, i32 row_len, i32 col_len);
void Map_Perimeter(struct Padding *edges, i32 *map, i32 row_len, i32 col_len);

void Map_Danger_Perimeter_Compute(struct Map *map, i32 *danger);

/* --- Movemap --- */
// Map_Movemap_Compute also computes REQUIRED costmap
i32   *Map_Movemap_Compute( struct Map *map, tnecs_entity u);
i32  *_Map_Movemap_Compute( struct Map *map, struct Point   s, i32 mv);
float *Map_fMovemap_Compute(struct Map *map, tnecs_entity u);

/* --- Attackmap --- */
i32 *Map_Mapto(struct Map *map, tnecs_entity u, MapTo attackto);

i32 *Map_Attackfromlist_Compute(struct Map *map);
i32 *Map_Attackfrommap_Compute(struct Map *map, tnecs_entity a,
                               tnecs_entity d, b32 m, b32 e);

/* --- global_rangemap --- */
void Map_globalRange(struct Map *map, u8 alignment);

#endif /* MAP_PATH_H */
