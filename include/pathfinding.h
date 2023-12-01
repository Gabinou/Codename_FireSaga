#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "structs.h"
#include "map/map.h"

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

/* --- About blocking --- */
// 1. Everything BLOCKED    is == 0
// 2. Everything DOABLE     is  > 0 ex: move to, visible, attack to
//     - A costmap for movement can ALSO be used as a blockmap for vision
//          -> Only if units cannot block vision.
enum PATHFINDING_costmap {
    costmap_BLOCKED     = 0,
    costmap_MIN         = 1,
};

// NOTE: unitmap is occupymap
enum PATHFINDING_OCCUPYMAP {
    OCCUPYMAP_UNBLOCKED   = TNECS_NULL,
};

#define costmap_fBLOCKED 0.0f

enum PATHFINDING_ATTACKMAP {
    ATTACKMAP_BLOCKED       = 0,
    ATTACKMAP_MOVEABLEMIN   = 1,
};

/* --- About pushpull --- */
// unit gradient map SHOULD BE a Pushpullmap also
enum PATHFINDING_PUSHPULLMAP {
    PUSHPULLMAP_UNIT    =  0,
    PUSHPULLMAP_BLOCKED = -1,
    PUSHPULLMAP_MINDIST =  1, // minimal moveable distance
};

enum PATHFINDING_BLOCKMAP {
    BLOCKMAP_INIT       = 0,
    BLOCKMAP_BLOCKED    = 0,
    BLOCKMAP_MIN        = 1,
};

enum SIGHTMAP_CODES {
    SIGHTMAP_NOT_VISIBLE    = 0,
    SIGHTMAP_VISIBLE        = 1,
    SIGHTMAP_OBSERVER       = 2,
    SIGHTMAP_ENEMY          = 3,
    SIGHTMAP_FRIENDLY       = 4,
    SIGHTMAP_BLOCKED        = 5,
};

/* --- Manhattan distance --- */
i32 _Pathfinding_Manhattan(i32 x_0, i32 y_0, i32 x_1, i32 y_1);
i32  Pathfinding_Manhattan(struct Point start, struct Point end);

/* --- Taxicab Geometry --- */
/* Taxicabs can't move diagonal so manhattan distance: abs(x1-x2) + abs(y1-ys2)
 * Manhattan (distance) used to trace 'circles' on square tilemap
 * Returns: Find points at distance [range_min, range_max] dist from [x, y]
 * Returns: Find points at distance [range_min, range_max] dist from [x, y]
 */
void Taxicab_Circle(             i32 *m, i32 v, i32 x, i32 y, size_t r, size_t c, struct Range *R);
i32 *Taxicab_Circle_List(i32 *d, i32 *m, i32 v, i32 x, i32 y, size_t r, size_t c, struct Range *R);

/* --- Pathfinding --- */
void Pathfinding_Neighbour(struct Node *o, struct Node *c, struct Node ne);

/* -- Astar -- */
i32 *Pathfinding_Astar(i32 *path, i32 *cost, size_t rowl, size_t coll,
                       struct Point start, struct Point end, b32 forward);

i32 *Pathfinding_Astar_plus(i32 *path_list, i32 *costmap, tnecs_entity *occupymap,
                            size_t row_len, size_t col_len, int move,
                            struct Point start, struct Point end, b32 forward);

i32 Pathfinding_Manhattan(struct Point start, struct Point end);

/* -- Moveto -- */
// NOMENCLATURE: moveto alternatives
// - traversable, accessible, travelable (long...)
// - crossable  (Maybe)
// - passable (used more to mean "Okay")
i32 *Pathfinding_Moveto(i32 *costmap, size_t row_len, size_t col_len,
                        struct Point start, i32 move);
void Pathfinding_Moveto_noM(i32 *move_matrix, i32 *cost_matrix,
                            size_t row_len, size_t col_len,
                            struct Point start, i32 move);
void Pathfinding_Moveto_Neighbours(struct Node *open, struct Node *closed,
                                   struct Node current, i32 *cost_matrix,
                                   size_t row_len, size_t col_len, i32 move);

/* -- Visible -- */
bool Pathfinding_Tile_Visible(i32 *sightmap, i32 *block_matrix, struct Point start,
                              struct Point delta, size_t col_len);
i32 *Pathfinding_Visible(i32 *blockmap, size_t row_len, size_t col_len,
                         struct Point start, i32 sight);
void Pathfinding_Visible_noM(i32 *sightmap, i32 *blockmap, size_t row_len,
                             size_t col_len, struct Point start, i32 sight);

/* -- Attackto -- */
// NOMENCLATURE: attackto alternatives
// - assailable
i32 *Pathfinding_Attackto(i32 *move_matrix, size_t row_len, size_t col_len,
                          u8 range[2], i32 mode_movetile);
void Pathfinding_Attackto_noM(i32 *attackto_mat, i32 *move_matrix,
                              size_t row_len, size_t col_len,
                              u8 range[2], i32 mode_movetile);
void Pathfinding_Attackto_Neighbours(i32 x, i32 y, i32 *attackmap, i32 *movemat,
                                     size_t row_len, size_t col_len,
                                     u8 range[2], i32 mode_movetile);

/* -- Attackfrom -- */
i32 *Pathfinding_Attackfrom(i32 *movemap, size_t row_len, size_t col_len,
                            struct Point target, u8 range[2], int mode_output);
void Pathfinding_Attackfrom_noM(i32 *attackfrom, i32 *movemap,
                                size_t row_len, size_t col_len,
                                struct Point target, u8 range[2]);

/* -- Utils -- */
typedef i32 *(*PathList_f)(i32 *, i32 *, size_t, struct Point, struct Point );

i32 *Pathfinding_PathList_Forward(i32 *path, i32 *came_from, size_t col_len,
                                  struct Point start, struct Point end);
i32 *Pathfinding_PathList_Backward(i32 *path, i32 *came_from, size_t col_len,
                                   struct Point start, struct Point end);

/* --- Pushing and pulling --- */
i32 *Pathfinding_Pushto(i32 *movemap, size_t row_len, size_t col_len,
                        struct Point target, int mode_output);
i32 *Pathfinding_Pullto(i32 *movemap, size_t row_len, size_t col_len,
                        struct Point target, int mode_output);

/* -- Unit Gradient -- */
i32 *Pathfinding_unitGradient(i32 *costmap, size_t row_len, size_t col_len,
                              struct Point  *targets, size_t unit_num);
void Pathfinding_unitGradient_noM(i32 *gradmap, i32 *costmap,
                                  size_t row_len, size_t col_len,
                                  struct Point *targets, size_t unit_num);

/* -- Hex -- */
i32 *Pathfinding_Moveto_Hex(i32 *costmap, size_t row_len, size_t depth_len,
                            struct Pointf start, i32 move, int mode_output);
i32 *Pathfinding_Visible_Hex(i32 *blockmap, size_t depth_len, size_t col_len,
                             struct Pointf start, i32 sight, int mode);

#endif /* PATHFINDING_H */
