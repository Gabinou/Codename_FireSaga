
#include "macros.h"
#include "platform.h"
#include "position.h"

const Position Position_default = {
    .onTilemap      = true,
    .pixel_pos      = {1, 1},
    .tilemap_pos    = {1, 1},
    .scale          = {1.0f, 1.0f},
};

b32 Point_isIn(Point pos, Point *arr, size_t len) {
    IES_check_ret(arr != NULL, 0);
    for (int i = 0; i < len; i++) {
        if ((pos.x == arr[i].x) && (pos.y == arr[i].y))
            return (true);
    }
    return (false);
}

b32 Point_Valid(Point p1) {
    return (
                   p1.x > DISTANCE_INVALID &&
                   p1.x > DISTANCE_INVALID &&
                   p1.x < DISTANCE_MAX &&
                   p1.y < DISTANCE_MAX
           );
}

i32 Point_Distance(Point p1, Point p2) {
    i32 dx = abs(p1.x - p2.x);
    i32 dy = abs(p1.y - p2.y);
    return (dx + dy);
}

/* --- Position Component --- */
void Position_Init(Position *spos) {
    *spos = Position_default;
}

void Position_Init_tnecs(void *voidpos) {
    Position_Init(voidpos);
}

void Position_replaceInbounds(Position *spos) {
    IES_check(spos != NULL);
    Point *pos = spos->onTilemap ? &(spos->tilemap_pos) :
                 &(spos->pixel_pos);
    Point min = spos->boundsmin;
    Point max = spos->boundsmax;
    pos->x = (pos->x < min.x) ? min.x : pos->x;
    pos->x = (pos->x > max.x) ? max.x : pos->x;
    pos->y = (pos->y < min.y) ? min.y : pos->y;
    pos->y = (pos->y > max.y) ? max.y : pos->y;
}

void Position_Bounds_Set(Position *spos, Point min, Point max) {
    IES_check(spos != NULL);
    _Position_Bounds_Set(spos, min.x, min.y, max.x, max.y);
}

void _Position_Bounds_Set(Position *spos,   i32 xmin, i32 xmax,
                          i32 ymin, i32 ymax) {
    IES_check(spos != NULL);
    spos->boundsmin.x = xmin;
    spos->boundsmax.x = xmax;
    spos->boundsmin.y = ymin;
    spos->boundsmax.y = ymax;
    Position_replaceInbounds(spos);
}

void Position_Pos_Add(Position *spos, Point mv) {
    IES_check(spos != NULL);
    _Position_Pos_Add(spos, mv.x, mv.y);
}

void _Position_Pos_Add(Position *spos, i32 mv_x, i32 mv_y) {
    IES_check(spos != NULL);
    Point *pos =    spos->onTilemap ? &(spos->tilemap_pos) :
                    &(spos->pixel_pos);
    _Position_Pos_Set(spos, mv_x + pos->x, mv_y + pos->y);
}

void Position_Pos_Set(Position *spos, Point in) {
    IES_check(spos != NULL);
    _Position_Pos_Set(spos, in.x, in.y);
}

void _Position_Pos_Set(Position *spos, i32 x, i32 y) {
    IES_check(spos != NULL);
    Point *pos =    spos->onTilemap ? &(spos->tilemap_pos) :
                    &(spos->pixel_pos);
    pos->x = x;
    pos->y = y;
    Position_replaceInbounds(spos);
}


