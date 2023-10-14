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

enum PATHFINDING_COSTMAP {
    COSTMAP_BLOCKED     = 0,
    COSTMAP_MOVEABLEMIN = 1,
};
#define COSTMAP_fBLOCKED 0.0f

enum PATHFINDING_ATTACKMAP {
    ATTACKMAP_BLOCKED       = 0,
    ATTACKMAP_MOVEABLEMIN   = 1,
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

/* -- Visible -- */
extern i32 *Pathfinding_Visible_noM(i32 *sightmap, i32 *blockmap, size_t row_len,
                                    size_t col_len, struct Point start, i32 sight);
extern i32 *Pathfinding_Visible(i32 *blockmap, size_t row_len, size_t col_len,
                                struct Point start, i32 sight, int mode_output);

/* -- Attackto -- */
extern void Pathfinding_Attackto_noM(i32 *attackto_mat, i32 *move_matrix,
                                     size_t row_len, size_t col_len, i32 move,
                                     u8 range[2], int mode_movetile);
extern i32 *Pathfinding_Attackto(i32 *move_matrix, size_t row_len, size_t col_len,
                                 i32 move, u8 range[2], int mode_output,
                                 int mode_movetile);

/* -- Attackfrom -- */
extern i32 *Pathfinding_Attackfrom_noM(i32 *attackfrom, i32 *movemap,
                                       size_t row_len, size_t col_len,
                                       struct Point target, u8 range[2]);
extern i32 *Pathfinding_Attackfrom(i32 *movemap, size_t row_len, size_t col_len,
                                   struct Point target, u8 range[2], int mode_output);

/* -- Utils -- */
extern void Pathfinding_Neighbors(struct Node *open, struct Node *closed,
                                  struct Node current, i32 *cost_matrix,
                                  size_t row_len, size_t col_len, i32 move);
extern i32 *Pathfinding_CameFrom_List(i32 *path, i32 *came_from, size_t col_len,
                                      struct Point start, struct Point end);

/* --- Pushing and pulling --- */
extern i32 *Pathfinding_Pushto(i32 *movemap, size_t row_len, size_t col_len,
                               struct Point target, int mode_output);
extern i32 *Pathfinding_Pullto(i32 *movemap, size_t row_len, size_t col_len,
                               struct Point target, int mode_output);

/* -- Unit Gradient -- */
extern i32 *Pathfinding_unitGradient_noM(i32 *gradmap, i32 *costmap,
                                         size_t row_len, size_t col_len,
                                         struct Point *targets, size_t unit_num);
extern i32 *Pathfinding_unitGradient(i32 *costmap, size_t row_len, size_t col_len,
                                     struct Point  *targets, size_t unit_num);

/* -- Hex -- */
extern i32 *Pathfinding_Moveto_Hex(i32 *costmap, size_t row_len, size_t depth_len,
                                   struct Pointf start, i32 move, int mode_output);
extern i32 *Pathfinding_Visible_Hex(i32 *blockmap, size_t depth_len, size_t col_len,
                                    struct Pointf  start, i32 sight, int mode);

/* TODO: Convert from nmath to SOTA */

#endif /* MAP_PATH_H */
