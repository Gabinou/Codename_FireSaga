#ifndef POSITION_H
#define POSITION_H

#include "types.h"
#include "narrative.h"

typedef struct Position {
    bool updatable;
    bool onTilemap;
    bool periodic;

    struct Point pixel_pos;
    struct Point tilemap_pos;
    struct Point offset_px;
    struct Point pixel_boundsmin;
    struct Point pixel_boundsmax;
    struct Point tilemap_boundsmin;
    struct Point tilemap_boundsmax;

    int_tile_t * tilemap_moveable; // pointer to 2D dynamic array.
    int_tile_t tilemap_row;
    int_tile_t tilemap_col;

    struct Point boundsmin; // why is that a pointer????
    struct Point boundsmax;
    float scale[2];
} Position;

extern struct Position Position_default;

extern void Position_replaceInbounds(struct Position * in_pos);
// extern void Position_whichSpace(struct Position * in_pos);

extern void Position_Bounds_Set(struct Position * in_pos, int_point_t xmin, int_point_t xmax, int_point_t ymin, int_point_t ymax);

extern bool Position_PosTilemap_Set(struct Position * in_pos, int_point_t in_x, int_point_t in_y);
extern bool Position_PosPixel_Set(struct Position * in_pos, int_point_t in_x, int_point_t in_y);
extern bool Position_Pos_Set(struct Position * in_pos, int_point_t in_x, int_point_t in_y);
extern bool Position_Pos_Add(struct Position * in_pos, int_point_t move_x, int_point_t move_y);
extern struct Point Position_Pos_Get(struct Position * in_pos);

extern struct Point Position_tilemap2pixel(struct Position * in_pos, int_point_t tilemap_x, int_point_t tilemap_y);
extern struct Point Position_pixel2tilemap(struct Position * in_pos, int_point_t pixel_x, int_point_t pixel_y);

#endif /* POSITION_H */
