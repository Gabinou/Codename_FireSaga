#ifndef POSITION_H
#define POSITION_H

#include <stdbool.h>
#include "types.h"
#include "enums.h"
#include "log.h"
#include "debug.h"
#include "structs.h"
#include "macros.h"
#include "nmath.h"
#include "SDL.h"

/* --- 2D POSITION ON A SQUARE GRID --- */
/* NOTE. no UPDATABLE bool:
 *      1- unclear the reason why/who/where
 *      2- superfluous check every time
 * updating position or not is outside responsibility */

typedef struct Position {
    struct Point pixel_pos;
    struct Point tilemap_pos;
    struct Point boundsmin;
    struct Point boundsmax;
    float scale[TWO_D];
    /* If not on the Tilemap, position is absolute in window */
    bool onTilemap  : 1; /* position transformed by scale   */
} Position;
extern struct Position Position_default;

/* --- API --- */
bool Position_Pos_Set(        struct Position *pos, i32 in_x,   i32 in_y);
bool Position_Pos_Add(        struct Position *pos, i32 move_x, i32 move_y);
void Position_Bounds_Set(     struct Position *pos, i32 xmin, i32 xmax, i32 ymin, i32 ymax);
void Position_replaceInbounds(struct Position *pos);

#endif /* POSITION_H */
