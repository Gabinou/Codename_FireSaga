
#include "position.h"

struct Position Position_default = {
    .onTilemap      = true,
    .absolute       = false,
    .pixel_pos      = {1, 1},
    .tilemap_pos    = {1, 1},
    .scale          = {1.0f, 1.0f},
};

void Position_replaceInbounds(struct Position *spos) {
    struct Point *pos = spos->onTilemap ? &(spos->tilemap_pos) : & (spos->pixel_pos);
    SDL_assert(pos != NULL);
    if (pos->x < spos->boundsmin.x)
        pos->x = spos->boundsmin.x;
    if (pos->x > spos->boundsmax.x)
        pos->x = spos->boundsmax.x;
    if (pos->y < spos->boundsmin.y)
        pos->y = spos->boundsmin.y;
    if (pos->y > spos->boundsmax.y)
        pos->y = spos->boundsmax.y;
}

void Position_Bounds_Set(struct Position *spos, i32 xmin, i32 xmax, i32 ymin, i32 ymax) {
    spos->boundsmin.x = xmin;
    spos->boundsmax.x = xmax;
    spos->boundsmin.y = ymin;
    spos->boundsmax.y = ymax;
    Position_replaceInbounds(spos);
}

bool Position_Pos_Add(struct Position *spos, i32 move_x, i32 move_y) {
    SOTA_Log_FPS("%d %d", move_x, move_y);
    // used by cursor only, and cursor is basically always on the tilemap...
    i32 new_x, new_y;
    if (spos->onTilemap) {
        new_x = move_x + spos->tilemap_pos.x;
        new_y = move_y + spos->tilemap_pos.y;
    } else {
        new_x = move_x + spos->pixel_pos.x;
        new_y = move_y + spos->pixel_pos.y;
    }
    return (Position_Pos_Set(spos, new_x, new_y));
}

bool Position_Pos_Set(struct Position *spos, i32 new_x, i32 new_y) {
    bool moved = false;
    struct Point *pos = spos->onTilemap ? &(spos->tilemap_pos) : &(spos->pixel_pos);
    SDL_assert(pos != NULL);
    if ((new_x >= spos->boundsmin.x) && (new_x <= spos->boundsmax.x) && (pos->x != new_x)) {
        pos->x = new_x;
        moved = true;
    } else {
        if (new_x < spos->boundsmin.x)
            pos->x = spos->boundsmin.x;
        else if (new_x > spos->boundsmax.x)
            pos->x = spos->boundsmax.x;
    }
    if ((new_y >= spos->boundsmin.y) && (new_y <= spos->boundsmax.y) && (pos->y != new_y)) {
        pos->y = new_y;
        moved = true;
    } else {
        if (new_y < spos->boundsmin.y)
            pos->y = spos->boundsmin.y;
        else if (new_y > spos->boundsmax.y)
            pos->y = spos->boundsmax.y;
    }
    return (moved);
}
