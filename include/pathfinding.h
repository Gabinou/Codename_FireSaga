#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <math.h>
#include <stdint.h>
#include "SDL2/SDL.h"
#include "types.h"
#include "enums.h"
#include "unit.h"
#include "linalg.h"
#include "q_math.h"
#include "narrative.h"
#include "hexposition.h"
#include "equations.h"

extern struct Square_Neighbors Pathfinding_Direction_Block(int_tile_t * costmap_pushpull, size_t row_len, size_t col_len, struct Point start);
extern struct Square_Neighbors Pathfinding_Direction_Pushable(int_tile_t * assailablemap, size_t row_len, size_t col_len, int_tile_t range[2], struct Point target);
extern struct Square_Neighbors Pathfinding_Direction_Pullable(int_tile_t * assailablemap, size_t row_len, size_t col_len, int_tile_t range[2], struct Point target);

extern int_tile_t * Pathfinding_Map_PushPullable(struct Square_Neighbors direction_block, struct Square_Neighbors pushpullable, size_t row_len, size_t col_len, struct Point start, uint8_t mode_output);
extern int_tile_t * Pathfinding_Map_Move(int_tile_t * costmap, size_t row_len, size_t col_len, struct Point start, int_tile_t move, uint8_t mode_output);
extern int_tile_t * Pathfinding_Map_Sight(int_tile_t * blockmap, size_t row_len, size_t col_len, struct Point start, int_tile_t sight, uint8_t mode_output);
extern int_tile_t * Pathfinding_Map_Attack(int_tile_t * move_matrix, size_t row_len, size_t col_len, int_tile_t move, int_tile_t range[2], uint8_t mode_output, uint8_t mode_movetile);
extern int_tile_t * Pathfinding_Map_Move_Hex(int_tile_t * costmap, size_t row_len, size_t depth_len, struct HexPoint start, int_tile_t move, uint8_t mode_output);
extern int_tile_t * Pathfinding_Map_Sight_Hex(int_tile_t * blockmap, size_t depth_len, size_t col_len, struct HexPoint start, int_tile_t sight, uint8_t mode);
extern int_tile_t * Pathfinding_Map_Assailable(int_tile_t * in_movemap, size_t row_len, size_t col_len, struct Point in_target, int_tile_t range[2], uint8_t mode_output);
extern int_tile_t * Pathfinding_Map_Pushable(int_tile_t * in_movemap, size_t row_len, size_t col_len, struct Point in_target, uint8_t mode_output);
extern int_tile_t * Pathfinding_Map_Pullable(int_tile_t * in_movemap, size_t row_len, size_t col_len, struct Point in_target, uint8_t mode_output);
extern int_tile_t * Pathfinding_Map_unitGradient(int_tile_t * in_costmap, size_t row_len, size_t col_len, struct Point * in_targets, size_t unit_num);
extern int_tile_t * Pathfinding_Map_Path(int_tile_t * move_matrix, size_t row_len, size_t col_len, struct Point start, struct Point end, uint8_t mode_path);

extern int_tile_t * Pathfinding_Path_step2position(int_tile_t * step_list, size_t list_len, struct Point start);
extern int_tile_t * Pathfinding_Path_position2step(int_tile_t * position_list, size_t list_len);

extern int_tile_t distance_manhattan(struct Point start, struct Point end);
extern int_tile_t distance_euclidian(struct Point start, struct Point end);

#define Point_inList_1D list_isIn_1D_int16_t
#define Point_inList_2D list_isIn_2D_int16_t
#define Point_inList_3D list_isIn_3D_int16_t

#endif /* PATHFINDING_H */