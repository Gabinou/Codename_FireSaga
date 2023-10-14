#ifndef MAP_PATH_H
#define MAP_PATH_H

#include "structs.h"
#include "map/map.h"

/* --- FORWARD DECLARATIONS --- */
struct Map;

/* --- ENUMS --- */
enum MOVEMAP {
    MOVEMAP_BLOCKED     = 0,
    MOVEMAP_MOVEABLEMIN = 1,
};

enum PATHFINDING_POINTS_MODE {
    POINTS_MODE_MATRIX  = 0,
    POINTS_MODE_LIST    = 1,
};

enum PATHFINDING_MODE_MOVETILE {
    MOVETILE_EXCLUDE = 0,
    MOVETILE_INCLUDE = 1,
};


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
extern i32 *Map_Healtomap_Compute(  struct Map *map, tnecs_world_t *w, tnecs_entity_t u, bool m,
                                    bool e);
extern i32 *Map_Attacktomap_Compute(struct Map *map, tnecs_world_t *w, tnecs_entity_t u, bool m,
                                    bool e);

extern i32 *Map_Healtomap_Compute_wLoadout(  struct Map *map, tnecs_world_t *w, tnecs_entity_t u,
                                             bool m, int lh, int rh);
extern i32 *Map_Attacktomap_Compute_wLoadout(struct Map *map, tnecs_world_t *w, tnecs_entity_t u,
                                             bool m, int lh, int rh);

extern i32 *_Map_tomap_Compute(i32 *tomap, i32 *mvtmat, uf8 row_len, uf8 col_len,
                               i32 mv, struct Range *r, uf8 mode);

extern void Map_globalRange(struct Map *map, tnecs_world_t *w, uf8 alignment);


/* --- Taxicab Geometry --- */
/* Taxicabs can't move diagonal so manhattan distance: abs(x1-x2) + abs(y1-ys2)
 * Manhattan (distance) used to trace 'circles' on square tilemap
 * Returns: Find points at distance [range_min, range_max] dist from [x, y]
 * Returns: Find points at distance [range_min, range_max] dist from [x, y]
 */
extern i32 *Taxicab_Circle(             i32 *m, i32 x, i32 y, size_t r, size_t c, struct Range *R);
extern i32 *Taxicab_Circle_List(i32 *d, i32 *m, i32 x, i32 y, size_t r, size_t c, struct Range *R);

/* --- Pathfinding --- */
/* -- Astar -- */
extern i32 *Pathfinding_Astar(i32 *path, i32 *cost, size_t rowl, size_t coll,
                              struct Point start, struct Point end);
extern i32 Pathfinding_Manhattan(struct Point start, struct Point end);


/* -- Moveto -- */
extern i32 *Pathfinding_Moveto(i32 *costmap, size_t row_len, size_t col_len,
                               struct Point start, i32 move, int mode_output);
extern void Pathfinding_Moveto_noM(i32 *move_matrix, i32 *cost_matrix,
                                   size_t row_len, size_t col_len,
                                   struct Point start, i32 move);

/* -- Utils -- */
extern void Pathfinding_Neighbors(struct Node *open, struct Node *closed,
                                  struct Node current, i32 *cost_matrix,
                                  size_t row_len, size_t col_len, i32 move);
extern i32 *Pathfinding_CameFrom_List(i32 *path, i32 *came_from, size_t col_len,
                                      struct Point start, struct Point end);




/* --- Pushing and pulling --- */

/* TODO: Convert from nmath to SOTA */
// extern int * came_from2path_list(int * path_list, int * came_from, size_t row_len, size_t col_len, int x_start, int y_start, int x_end, int y_end);

// extern int * came_from2path_map(int * path_map, int * came_from, size_t row_len, size_t col_len, int x_start, int y_start, int x_end, int y_end);

// extern struct nmath_sq_neighbors pathfinding_Direction_Block(type * costmap_pushpull, size_t row_len, size_t col_len, struct Point start);

// extern struct nmath_sq_neighbors pathfinding_Direction_Pushto(type * assailablemap, size_t row_len, size_t col_len, uint8_t range[2], struct Point target);

// extern struct nmath_sq_neighbors pathfinding_Direction_Pullto(type * assailablemap, size_t row_len, size_t col_len, uint8_t range[2], struct Point target);

// extern type * pathfinding_Map_PushPullto_noM(type * pushpulltomap, struct nmath_sq_neighbors direction_block, struct nmath_sq_neighbors pushpullto, size_t row_len, size_t col_len, struct Point start);

// extern type * pathfinding_Map_PushPullto(struct nmath_sq_neighbors direction_block, struct nmath_sq_neighbors pushpullto, size_t row_len, size_t col_len, struct Point start, uint8_t mode_output);


#endif /* MAP_PATH_H */
