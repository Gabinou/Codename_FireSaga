#ifndef MAP_PATH_H
#define MAP_PATH_H

#include "structs.h"
#include "pathfinding.h"
#include "map/map.h"

/* --- FORWARD DECLARATIONS --- */
struct Map;

/* --- Bounds --- */
extern void Map_Bounds_Compute(struct Map *map);

extern i32 *Map_Danger_Compute(struct Map *map, tnecs_world_t *w, tnecs_entity_t u);

/* --- Dangermap --- */
extern void Map_Danger_Add(               struct Map *map, i32 *danger);
extern void Map_Danger_Sub(               struct Map *map, i32 *danger);
extern void Map_Danger_Reset(             struct Map *map);
extern void Map_Stacked_Dangermap_Reset(  struct Map *map);
extern void Map_Stacked_Dangermap_Compute(struct Map *map);

/* --- Global_Dangermap --- */
extern void Map_Global_Danger_Add(       struct Map *map, i32 *danger);
extern void Map_Global_Danger_Sub(       struct Map *map, i32 *danger);
extern void Map_Global_Danger_Reset(     struct Map *map);
extern void Map_Global_Dangermap_Reset(  struct Map *map);
extern void Map_Global_Dangermap_Compute(struct Map *map);

/* --- Costmap --- */
extern i32   *Map_Costmap_Movement_Compute( struct Map *map, tnecs_world_t *w, tnecs_entity_t u);
extern i32   *Map_Costmap_PushPull_Compute( struct Map *map, tnecs_world_t *w, tnecs_entity_t u);
extern float *Map_fCostmap_Movement_Compute(struct Map *map, tnecs_world_t *w, tnecs_entity_t u);

/* --- Movemap --- */
// Map_Movemap_Compute also computes REQUIRED costmap
extern i32   *Map_Movemap_Compute( struct Map *map, tnecs_world_t *w, tnecs_entity_t u);
extern i32   *_Map_Movemap_Compute(struct Map *map, struct Point   s, i32 mv);
extern float *Map_fMovemap_Compute(struct Map *map, tnecs_world_t *w, tnecs_entity_t u);

/* --- Attackmap --- */
/* Map_Attacktomap_Compute also computes REQUIRED costmap, movemap */
extern i32 *Map_Healtolist_Compute(  struct Map *map);
extern i32 *Map_Attacktolist_Compute(struct Map *map)
;
extern i32 *Map_Healtomap_Compute(  struct Map *map, tnecs_world_t *w,
                                    tnecs_entity_t u, bool m, bool e);
extern i32 *Map_Attacktomap_Compute(struct Map *map, tnecs_world_t *w,
                                    tnecs_entity_t u, bool m, bool e);

extern i32 *Map_Healtomap_Compute_wLoadout(  struct Map *map, tnecs_world_t *w,
                                             tnecs_entity_t u, bool m, int lh, int rh);
extern i32 *Map_Attacktomap_Compute_wLoadout(struct Map *map, tnecs_world_t *w,
                                             tnecs_entity_t u, bool m, int lh, int rh);

extern i32 *_Map_tomap_Compute(i32 *tomap, i32 *mvtmat, uf8 row_len, uf8 col_len,
                               i32 mv, struct Range *r, uf8 mode);

extern void Map_globalRange(struct Map *map, tnecs_world_t *w, uf8 alignment);

#endif /* MAP_PATH_H */
