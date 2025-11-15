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
    b32 onTilemap; /* position transformed by scale   */
} Position;
extern const Position Position_default;

/* --- Utils --- */
b32 Point_isIn(Point     pos, Point    *arr,
               size_t    len);

b32 Point_Valid(Point p1);
i32 Point_Distance(Point p1, Point p2);

/* --- Position Component --- */
/* -- tnecs -- */
void Position_Init(Position *spos);
void Position_Init_tnecs(void *voidpos);

void Position_replaceInbounds(Position *pos);

void _Position_Pos_Set(   Position *pos, i32 in_x,   i32 in_y);
void _Position_Pos_Add(   Position *pos, i32 move_x, i32 move_y);
void _Position_Bounds_Set(Position *pos,
                          i32 xmin, i32 xmax,
                          i32 ymin, i32 ymax);

void Position_Pos_Set(   Position *pos, Point in);
void Position_Pos_Add(   Position *pos, Point mv);
void Position_Bounds_Set(Position *pos, Point min, Point max);


#endif /* POSITION_H */
