
#include "position.h"

struct Position Position_default = {
    .updatable = true,
    .onTilemap = true,
    .periodic = false,

    .pixel_pos = {1, 1},
    .tilemap_pos = {1, 1},
    .offset_px = {1, 1},
    .pixel_boundsmin = {0, 0},
    .pixel_boundsmax = {100, 100},
    .tilemap_boundsmin = {0, 0},
    .tilemap_boundsmax = {100, 100},
    .tilemap_moveable = NULL, // pointer to 2D dynamic array.
    .tilemap_row = 0,
    .tilemap_col = 0,

    .scale = {1.0f, 1.0f},
};

void PositionModuleImport(ecs_world_t * in_world) {
    ECS_MODULE(in_world, PositionModule);
    ECS_COMPONENT(in_world, Position);
    ECS_SET_COMPONENT(Position);
}

void Position_replaceInbounds(struct Position * in_pos) {
    struct Point * pos;
    if (in_pos->onTilemap) {
        pos = &(in_pos->tilemap_pos);
    } else {
        pos = &(in_pos->pixel_pos);
    }
    SDL_assert(pos != NULL);
    if (pos->x < in_pos->boundsmin.x) {
        pos->x = in_pos->boundsmin.x;
    }
    if (pos->x > in_pos->boundsmax.x) {
        pos->x = in_pos->boundsmax.x;
    }
    if (pos->y < in_pos->boundsmin.y) {
        pos->y = in_pos->boundsmin.y;
    }
    if (pos->y > in_pos->boundsmax.y) {
        pos->y = in_pos->boundsmax.y;
    }
}

void Position_Bounds_Set(struct Position * in_pos, int_point_t xmin, int_point_t xmax, int_point_t ymin, int_point_t ymax) {
    in_pos->boundsmin.x = xmin;
    in_pos->boundsmax.x = xmax;
    in_pos->boundsmin.y = ymin;
    in_pos->boundsmax.y = ymax;
    Position_replaceInbounds(in_pos);
}

struct Point Position_tilemap2pixel(struct Position * in_pos, int_point_t tilemap_x, int_point_t tilemap_y) {
    struct Point pixel_pos;
    pixel_pos.x = in_pos->scale[0] * tilemap_x + in_pos->offset_px.x;
    pixel_pos.y = in_pos->scale[1] * tilemap_y + in_pos->offset_px.y;
    return (pixel_pos);
}

struct Point Position_pixel2tilemap(struct Position * in_pos, int_point_t pixel_x, int_point_t pixel_y) {
    struct Point tile_pos;
    tile_pos.x = (int)((pixel_x - in_pos->offset_px.x) / in_pos->scale[0]);
    tile_pos.y = (int)((pixel_y - in_pos->offset_px.y) / in_pos->scale[1]);
    return (tile_pos);
}

bool Position_PosTilemap_Set(struct Position * in_pos, int_point_t in_x, int_point_t in_y) {
    in_pos->onTilemap = false;
    return (Position_Pos_Set(in_pos, in_x, in_y));
}

bool Position_PosPixel_Set(struct Position * in_pos, int_point_t in_x, int_point_t in_y) {
    in_pos->onTilemap = true;
    return (Position_Pos_Set(in_pos, in_x, in_y));
}

bool Position_Pos_Add(struct Position * in_pos, int_point_t move_x, int_point_t move_y) {
    // SDL_Log("%d %d", move_x, move_y);
    int_point_t new_x, new_y;
    if (in_pos->onTilemap) {
        new_x = move_x + in_pos->tilemap_pos.x;
        new_y = move_y + in_pos->tilemap_pos.y;
    } else {
        new_x = move_x + in_pos->pixel_pos.x;
        new_y = move_y + in_pos->pixel_pos.y;
    }
    return (Position_Pos_Set(in_pos, new_x, new_y));
}

struct Point Position_Pos_Get(struct Position * in_pos) {
    struct Point out;
    if (in_pos->onTilemap) {
        out = in_pos->tilemap_pos;
    } else {
        out = in_pos->pixel_pos;
    }
    return (out);
}

bool Position_Pos_Set(struct Position * in_pos, int_point_t new_x, int_point_t new_y) {
    bool moved = false;
    struct Point * pos;
    if (in_pos->onTilemap) {
        pos = &(in_pos->tilemap_pos);
    } else {
        pos = &(in_pos->pixel_pos);
    }

    if (in_pos->updatable) {

        if (in_pos->periodic) {
            if (new_x < in_pos->boundsmin.x) {
                new_x = in_pos->boundsmax.x;
            } else if (new_x > in_pos->boundsmax.x) {
                new_x = in_pos->boundsmin.x;
            }
            if (new_y < in_pos->boundsmin.y) {
                new_y = in_pos->boundsmax.y;
            } else if (new_y > in_pos->boundsmax.y) {
                new_y = in_pos->boundsmin.y;
            }
        }
        if (in_pos->tilemap_moveable != NULL) {
            if (in_pos->tilemap_moveable[in_pos->tilemap_col * new_y + new_x] > 0) {
                pos->x = new_x;
                pos->y = new_y;
                moved = true;
            }
        } else {
            if ((new_x >= in_pos->boundsmin.x) && (new_x <= in_pos->boundsmax.x) && (pos->x != new_x)) {
                pos->x = new_x;
                moved = true;
            } else {
                if (new_x < in_pos->boundsmin.x) {
                    pos->x = in_pos->boundsmin.x;
                } else if (new_x > in_pos->boundsmax.x) {
                    pos->x = in_pos->boundsmax.x;
                }
            }
            if ((new_y >= in_pos->boundsmin.y) && (new_y <= in_pos->boundsmax.y) && (pos->y != new_y)) {
                pos->y = new_y;
                moved = true;
            } else {
                if (new_y < in_pos->boundsmin.y) {
                    pos->y = in_pos->boundsmin.y;
                } else if (new_y > in_pos->boundsmax.y) {
                    pos->y = in_pos->boundsmax.y;
                }
            }
        }
    }
    return (moved);
}

