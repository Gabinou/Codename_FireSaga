#ifndef POSITION_H
#define POSITION_H

#include "types.h"
#include "enums.h"
#include "structs.h"

/* --- 2D POSITION ON A SQUARE GRID --- */
typedef struct Position {
    struct Point pixel_pos;
    struct Point tilemap_pos;
    struct Point boundsmin; /* inclusive */
    struct Point boundsmax; /* inclusive */
    float scale[TWO_D];
    /* If not on the Tilemap, position is absolute in window */
    b32 onTilemap  : 1; /* position transformed by scale   */
} Position;
extern struct Position Position_default;

b32 Point_isIn(struct Point pos, struct Point *arr, size_t len);

/* --- API --- */
void Position_Pos_Set(   struct Position *pos, i32 in_x,   i32 in_y);
void Position_Pos_Add(   struct Position *pos, i32 move_x, i32 move_y);
void Position_Bounds_Set(struct Position *pos, i32 xmin, i32 xmax, i32 ymin, i32 ymax);
void Position_replaceInbounds(struct Position *pos);

#endif /* POSITION_H */
