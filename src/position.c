
#include "position.h"

const struct Position Position_default = {
    .onTilemap      = true,
    .pixel_pos      = {1, 1},
    .tilemap_pos    = {1, 1},
    .scale          = {1.0f, 1.0f},
};

void Position_Init_tnecs(void *voidpos) {
    Position_Init(voidpos);
}

void Position_Init(Position *pos) {
    *pos = Position_default;
}

b32 Point_isIn(struct Point pos, struct Point *arr, size_t len) {
    for (int i = 0; i < len; i++) {
        if ((pos.x == arr[i].x) && (pos.y == arr[i].y))
            return (true);
    }
    return (false);
}

u32 Point_Distance(Point p1, Point p2) {
    u32 dx = abs(p1->x - p2->x);
    u32 dy = abs(p1->y - p2->y);
    return (dx + dy);
}

void Position_replaceInbounds(struct Position *spos) {
    SDL_assert(spos != NULL);
    struct Point *pos = spos->onTilemap ? &(spos->tilemap_pos) : &(spos->pixel_pos);
    pos->x = (pos->x < spos->boundsmin.x) ? spos->boundsmin.x : pos->x;
    pos->x = (pos->x > spos->boundsmax.x) ? spos->boundsmax.x : pos->x;
    pos->y = (pos->y < spos->boundsmin.y) ? spos->boundsmin.y : pos->y;
    pos->y = (pos->y > spos->boundsmax.y) ? spos->boundsmax.y : pos->y;
}

void Position_Bounds_Set(struct Position *spos, i32 xmin, i32 xmax, i32 ymin, i32 ymax) {
    SDL_assert(spos != NULL);
    spos->boundsmin.x = xmin;
    spos->boundsmax.x = xmax;
    spos->boundsmin.y = ymin;
    spos->boundsmax.y = ymax;
    Position_replaceInbounds(spos);
}

void Position_Pos_Add(struct Position *spos, i32 move_x, i32 move_y) {
    SDL_assert(spos != NULL);
    struct Point *pos = spos->onTilemap ? &(spos->tilemap_pos) : &(spos->pixel_pos);
    Position_Pos_Set(spos, move_x + pos->x, move_y + pos->y);
}

void Position_Pos_Set(struct Position *spos, i32 new_x, i32 new_y) {
    SDL_assert(spos != NULL);
    struct Point *pos = spos->onTilemap ? &(spos->tilemap_pos) : &(spos->pixel_pos);
    pos->y = new_y;
    pos->x = new_x;
    Position_replaceInbounds(spos);
}
