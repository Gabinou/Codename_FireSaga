
#include "arrow.h"
#include "pathfinding.h"
#include "octant.h"
#include "macros.h"
#include "map/map.h"
#include "nmath.h"
#include "filesystem.h"
#include "utilities.h"

/* --- STATIC FUNCTIONS DECLARATIONS --- */
static struct Rendered _Arrow_Decider_Startend(i32 x_0, i32 y_0, i32 x_1, i32 y_1);
static struct Rendered _Arrow_Decider_Start(i32 x_0, i32 y_0, i32 x_1, i32 y_1);
static struct Rendered _Arrow_Decider_Middle(i32 x_0, i32 y_0, i32 x_1, i32 y_1, i32 x_2, i32 y_2);
static struct Rendered _Arrow_Decider_End(i32 x_0, i32 y_0, i32 x_1, i32 y_1, i32 x_2, i32 y_2);
static void _Arrow_Decider(struct Arrow *arrow, i32 point);
static void _Arrow_Path_Trace(struct Arrow *arrow, struct Point end_in, struct Map_Size size);

const struct Arrow Arrow_default = {0};

/* --- STATIC FUNCTIONS --- */
/* Short arrow, start and end point in one tile. */
static struct Rendered _Arrow_Decider_Startend(i32 x_0, i32 y_0, i32 x_1, i32 y_1) {
    struct Rendered out = Rendered_default;
    struct Point move   = {x_1 - x_0, y_1 - y_0};
    int direction_01    = Ternary_Direction(move);

    switch (direction_01) {
        case SOTA_DIRECTION_LEFT:
            out.graphics.index = ARROW_STARTEND_HORIZONTAL;
            out.flip = SDL_FLIP_HORIZONTAL;
            break;
        case SOTA_DIRECTION_RIGHT:
            out.graphics.index = ARROW_STARTEND_HORIZONTAL;
            break;
        case SOTA_DIRECTION_TOP:
            out.graphics.index = ARROW_STARTEND_DOWN;
            break;
        case SOTA_DIRECTION_BOTTOM:
            out.graphics.index = ARROW_STARTEND_UP;
            break;
    }
    return (out);
}

/* Start tile of arrow, if longer than one tile. */
static struct Rendered _Arrow_Decider_Start(i32 x_0, i32 y_0, i32 x_1, i32 y_1) {
    struct Rendered out = Rendered_default;
    struct Point move   = {x_1 - x_0, y_1 - y_0};
    int direction_01    = Ternary_Direction(move);

    switch (direction_01) {
        case SOTA_DIRECTION_LEFT:
            out.graphics.index = ARROW_START_HORIZONTAL;
            out.flip = SDL_FLIP_HORIZONTAL;
            break;
        case SOTA_DIRECTION_RIGHT:
            out.graphics.index = ARROW_START_HORIZONTAL;
            break;
        case SOTA_DIRECTION_TOP:
            out.graphics.index = ARROW_START_DOWN;
            break;
        case SOTA_DIRECTION_BOTTOM:
            out.graphics.index = ARROW_START_UP;
            break;
    }
    return (out);
}

/* Middle tile of arrow, if longer than one tile. */
static struct Rendered _Arrow_Decider_Middle(i32 x_0, i32 y_0, i32 x_1, i32 y_1, i32 x_2, i32 y_2) {
    SDL_assert(((x_0 != x_2) || (y_0 != y_2)));
    struct Rendered out  = Rendered_default;
    struct Point move_01 = {x_1 - x_0, y_1 - y_0};
    int direction_01     = Ternary_Direction(move_01);
    struct Point move_12 = {x_2 - x_1, y_2 - y_1};
    int direction_12     = Ternary_Direction(move_12);

    out.flip = (direction_01 == SOTA_DIRECTION_LEFT)
               || (direction_12 == SOTA_DIRECTION_RIGHT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    switch (direction_01) {
        case SOTA_DIRECTION_LEFT:
        case SOTA_DIRECTION_RIGHT:
            switch (direction_12) {
                case SOTA_DIRECTION_LEFT:
                case SOTA_DIRECTION_RIGHT:
                    out.graphics.index = ARROW_STRAIGHT_HORIZONTAL;
                    break;
                case SOTA_DIRECTION_BOTTOM:
                    out.graphics.index = ARROW_CORNER_UP;
                    break;
                case SOTA_DIRECTION_TOP:
                    out.graphics.index = ARROW_CORNER_DOWN;
                    break;
            }
            break;
        case SOTA_DIRECTION_TOP:
            switch (direction_12) {
                case SOTA_DIRECTION_TOP:
                    out.graphics.index = ARROW_STRAIGHT_VERTICAL;
                    break;
                case SOTA_DIRECTION_RIGHT:
                case SOTA_DIRECTION_LEFT:
                    out.graphics.index = ARROW_CORNER_UP;
                    break;
            }
            break;
        case SOTA_DIRECTION_BOTTOM:
            switch (direction_12) {
                case SOTA_DIRECTION_BOTTOM:
                    out.graphics.index = ARROW_STRAIGHT_VERTICAL;
                    break;
                case SOTA_DIRECTION_RIGHT:
                case SOTA_DIRECTION_LEFT:
                    out.graphics.index = ARROW_CORNER_DOWN;
                    break;
            }
            break;
    }
    return (out);
}

/* End tile of arrow, if longer than one tile. */
static struct Rendered _Arrow_Decider_End(i32 x_0, i32 y_0, i32 x_1, i32 y_1, i32 x_2, i32 y_2) {
    struct Rendered out  = Rendered_default;

    struct Point move_01 = {x_1 - x_0, y_1 - y_0};
    int direction_01     = Ternary_Direction(move_01);
    struct Point move_12 = {x_2 - x_1, y_2 - y_1};
    int direction_12     = Ternary_Direction(move_12);

    b32 left_01 = (direction_01 == SOTA_DIRECTION_LEFT);
    b32 left_12 = (direction_12 == SOTA_DIRECTION_LEFT);

    out.flip = (left_01 || left_12) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    switch (direction_01) {
        case SOTA_DIRECTION_LEFT:
        case SOTA_DIRECTION_RIGHT:
            switch (direction_12) {
                case SOTA_DIRECTION_LEFT:
                case SOTA_DIRECTION_RIGHT:
                    out.graphics.index = ARROW_STOP_HORIZONTAL;
                    break;
                case SOTA_DIRECTION_BOTTOM:
                    out.graphics.index = ARROW_STOP_CORNER_RUP;
                    break;
                case SOTA_DIRECTION_TOP:
                    out.graphics.index = ARROW_STOP_CORNER_RDOWN;
                    break;
            }
            break;
        case SOTA_DIRECTION_TOP:
            switch (direction_12) {
                case SOTA_DIRECTION_TOP:
                    out.graphics.index = ARROW_STOP_DOWN;
                    break;
                case SOTA_DIRECTION_RIGHT:
                case SOTA_DIRECTION_LEFT:
                    out.graphics.index = ARROW_STOP_CORNER_DRIGHT;
                    break;
            }
            break;
        case SOTA_DIRECTION_BOTTOM:
            switch (direction_12) {
                case SOTA_DIRECTION_BOTTOM:
                    out.graphics.index = ARROW_STOP_UP;
                    break;
                case SOTA_DIRECTION_RIGHT:
                case SOTA_DIRECTION_LEFT:
                    out.graphics.index = ARROW_STOP_CORNER_URIGHT;
                    break;
            }
            break;
    }
    return (out);
}

/* - Texture decider: for current point - */
static void _Arrow_Decider(struct Arrow *arrow, i32 point) {
    /* -- Preliminaries -- */
    SDL_assert(arrow->texture != NULL);
    SDL_assert(point >= 0);
    u8 pointnum = DARR_NUM(arrow->path) / TWO_D;
    i32 endpoint = (pointnum - 2);
    SDL_assert(point <= endpoint);

#ifndef INFINITE_MOVE_ALL
    pointnum = pointnum < (arrow->move + 1) ? pointnum : (arrow->move + 1) ;
#endif /* INFINITE_MOVE_ALL */

    /* - tiles around point - */
    /* 0,1,2 tiles -> 0 = behind, 1 = current, 2 = next */
    i32 x_0 = 0, x_1 = 0, x_2 = 0, y_0 = 0, y_1 = 0, y_2 = 0;
    if (point > 0) {
        x_0 = arrow->path[(point - 1) * TWO_D];
        y_0 = arrow->path[(point - 1) * TWO_D + 1];
    }
    x_1     = arrow->path[   point    * TWO_D];
    y_1     = arrow->path[   point    * TWO_D + 1];
    x_2     = arrow->path[(point + 1) * TWO_D];
    y_2     = arrow->path[(point + 1) * TWO_D + 1];

    /* -- Deciding texture -- */
    if (point == 0) {
        if (pointnum == 2)
            arrow->rendereds[point] = _Arrow_Decider_Startend(x_1, y_1, x_2, y_2);
        else
            arrow->rendereds[point] = _Arrow_Decider_Start(x_1, y_1, x_2, y_2);
    } else if (point == endpoint) {
        arrow->rendereds[point] = _Arrow_Decider_End(x_0, y_0, x_1, y_1, x_2, y_2);
    } else {
        arrow->rendereds[point] = _Arrow_Decider_Middle(x_0, y_0, x_1, y_1, x_2, y_2);
    }
    SDL_assert(arrow->texture != NULL);
}

/* - Retracing path using A* (A_star) algorithm - */
static void _Arrow_Path_Trace(struct Arrow *arrow, struct Point end_in, struct Map_Size size) {
    SDL_assert(arrow->texture      != NULL);
    SDL_assert(arrow->costmap       != NULL);
    struct Point end   = {end_in.x, end_in.y};
    struct Point start = {arrow->path[0], arrow->path[1]};
    DARR_NUM(arrow->path) = 0;

    if ((start.x != end.x) || (start.y != end.y)) {
        i32 row_len = Map_Size_row_len(&size);
        i32 col_len = Map_Size_col_len(&size);
        /* A* goes backwards. */
        /* NOTE: Switching start and end CRASHES MY COMPUTER. */
        arrow->path = Pathfinding_Astar(arrow->path,
                                        arrow->costmap,
                                        row_len, col_len,
                                        start, end, true);
        /* Endpoint not included in Astar */
        DARR_PUT(arrow->path, end.x);
        DARR_PUT(arrow->path, end.y);

        /* Deciding path. */
        i32 pointnum    = DARR_NUM(arrow->path) / TWO_D;
        for (i32 i = 0; i < (pointnum - 1); i++)
            _Arrow_Decider(arrow, i);
    } else
        DARR_NUM(arrow->path) = TWO_D;
}

/* --- GLOBAL FUNCTIONS --- */
/* --- Constructor/Destructors --- */
struct Arrow *Arrow_Init(void) {
    struct Arrow *arrow = SDL_malloc(sizeof(*arrow));
    *arrow = Arrow_default;
    arrow->path             = DARR_INIT(arrow->path, i32, 16);
    DARR_NUM(arrow->path)   = 0;
    return (arrow);
}

void Arrow_Free(struct Arrow *arrow) {
    if (arrow == NULL)
        return;

    if (arrow->path != NULL) {
        DARR_FREE(arrow->path);
        arrow->path = NULL;
    }
    if (arrow->texture != NULL
       ) {
        SDL_DestroyTexture(arrow->texture);
        arrow->texture = NULL;
    }
    SDL_free(arrow);
}

/* --- I/O --- */
void Arrow_Textures_Load(struct Arrow *arrow,  char *filename, SDL_Renderer *renderer) {
    SDL_assert(renderer != NULL);

    if (arrow->texture != NULL) {
        SDL_DestroyTexture(arrow->texture);
        arrow->texture = NULL;
    }

    arrow->texture = Filesystem_Texture_Load(renderer, filename, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(arrow->texture != NULL);
}

/* --- Internals --- */
void Arrow_Path_Init(struct Arrow *arrow, i32 *costmap, i32 move,
                     struct Point start) {
    SDL_assert(costmap != NULL);
    arrow->move    = move;
    arrow->costmap      = costmap;
    DARR_NUM(arrow->path)   = 0;
    DARR_PUT(arrow->path, start.x);
    DARR_PUT(arrow->path, start.y);
    arrow->show = true;
}

/* - Adding next point to path or not decision function - */
void Arrow_Path_Add(struct Arrow *arrow, struct Map_Size size, i32 x_next, i32 y_next) {
    /* -- Preliminaries -- */
    SDL_assert(arrow->texture  != NULL);
    SDL_assert(arrow->costmap   != NULL);
    size_t num_current      = DARR_NUM(arrow->path) / TWO_D;
    size_t point_current    = num_current - 1;
    size_t index            = (point_current  - 1) * TWO_D;

    /* -- Point characteristics -- */
    i32 x_previous  = arrow->path[index];
    i32 y_previous  = arrow->path[index + 1];
    b32 isprevious = ((x_next == x_previous) && (y_next == y_previous));
    b32 isin_list  = list_isIn_2D(arrow->path, point_current, x_next, y_next);

    /* - Arrow can be infinitely long or not - */
#ifdef INFINITE_MOVE_ALL
    b32 istoolong = (num_current > ARROW_MAX_LEN);
#else
    b32 istoolong = (num_current > arrow->move);
#endif /* INFINITE_MOVE_ALL */

    /* - Point decider - */
    if (isprevious) {
        /* Next point is the previous point */
        /* Reduce arrow size by one */
        DARR_NUM(arrow->path) -= TWO_D;
        if (point_current >= 2)
            _Arrow_Decider(arrow, point_current - 2);
    } else if (istoolong || isin_list) {
        /* Next point is in list OR list is too long */
        /* Trace arrow */
        struct Point end = {x_next, y_next};
        _Arrow_Path_Trace(arrow, end, size);
    } else {
        /* Next point is: */
        /*    not previous one, not in pathlist, pathlist isn't too long. */
        /* -> add it to pathlist */
        DARR_PUT(arrow->path, x_next);
        DARR_PUT(arrow->path, y_next);
        _Arrow_Decider(arrow, point_current);
        if (point_current > 0)
            _Arrow_Decider(arrow, point_current - 1);
    }
}

void Arrow_Draw(struct Arrow    *arrow,
                struct Map_Size  size,
                struct Camera   *camera,
                SDL_Renderer    *renderer) {
    SDL_assert(arrow->texture != NULL);
    SDL_assert(arrow->path != NULL);

    /* Define ants */
    i32 num             = DARR_NUM(arrow->path) / TWO_D;

    SDL_Rect srcrect    = {0};
    srcrect.w           = size.tile.x;
    srcrect.h           = size.tile.y;

    SDL_Rect dstrect    = {0};
    dstrect.w           = SOTA_ZOOM(size.tile.x, camera->zoom);
    dstrect.h           = SOTA_ZOOM(size.tile.y, camera->zoom);

    /* Loop over all tiles that make up the arrow */
    for (i32 i = 0; i < (num - 1); i++) {
        /* - srcrect - */
        struct Rendered rend = arrow->rendereds[i];
        i32 x_texture   = sota_ss_x((rend.graphics.index - 1), TILESET_COL_LEN);
        i32 y_texture   = sota_ss_y((rend.graphics.index - 1), TILESET_COL_LEN);
        srcrect.x       = x_texture * size.tile.x;
        srcrect.y       = y_texture * size.tile.y;

        /* - dstrect - */
        i32 x           = arrow->path[TWO_D * i];
        i32 y           = arrow->path[(TWO_D * i) + 1];
        i32 x_zoom      = SOTA_ZOOM(x * size.tile.x, camera->zoom);
        i32 y_zoom      = SOTA_ZOOM(y * size.tile.y, camera->zoom);
        dstrect.x       = x_zoom + camera->offset.x;
        dstrect.y       = y_zoom + camera->offset.y;

        /* -- Rendering -- */
        SDL_RenderCopyEx(renderer, arrow->texture, &srcrect, &dstrect, 0, NULL, rend.flip);
    }
}
