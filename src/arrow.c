
#include "arrow.h"

struct Arrow Arrow_default = {
    .costmap        = NULL,
    .col_len        = 21,
    .row_len        = 25,
    .move           =  0,
    .pathlist       = NULL,
    .tilesize       = {ARROW_TILESIZE, ARROW_TILESIZE},
    .map_tilesize   = {SOTA_TILESIZE, SOTA_TILESIZE},
    .textures       = NULL, // [patch_id]
    .rendereds      = {{0}}, // [patch_id]
    .show           = false,
};

/* --- Constructors/Deconstructors --- */
void Arrow_Init(struct Arrow *arrow, i32 tilesize[TWO_D]) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    arrow->map_tilesize[0] = tilesize[0];
    arrow->map_tilesize[1] = tilesize[1];
    arrow->pathlist = DARR_INIT(arrow->pathlist, i32, 16);
    DARR_NUM(arrow->pathlist) = 0;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

void Arrow_Free(struct Arrow *arrow) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    if (arrow->pathlist != NULL) {
        DARR_FREE(arrow->pathlist);
        arrow->pathlist = NULL;
    }
    if (arrow->textures != NULL) {
        SDL_DestroyTexture(arrow->textures);
        arrow->textures = NULL;
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- I/O --- */
void Arrow_Textures_Load(struct Arrow *arrow, const char *filename, SDL_Renderer *renderer) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(renderer != NULL);
    arrow->textures = Filesystem_Texture_Load(renderer, filename, SDL_PIXELFORMAT_INDEX8);
    SDL_assert(arrow->textures != NULL);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* --- Internals --- */
void Arrow_Path_Init(struct Arrow *arrow, i32 *costmap, i32 move,
                     struct Point start) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(costmap != NULL);
    arrow->move    = move;
    arrow->costmap = costmap;
    DARR_NUM(arrow->pathlist) = 0;
    DARR_PUT(arrow->pathlist, start.x);
    DARR_PUT(arrow->pathlist, start.y);
    arrow->show = true;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* - Adding next point to path or not decision function - */
void Arrow_Path_Add(struct Arrow *arrow, i32 x_next, i32 y_next) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Preliminaries -- */
    SDL_assert(arrow->textures != NULL);
    SDL_assert(arrow->costmap != NULL);
    size_t num_current   = DARR_NUM(arrow->pathlist) / TWO_D;
    size_t point_current = num_current - 1;
    i32 x_previous = arrow->pathlist[(point_current  - 1) * TWO_D];
    i32 y_previous = arrow->pathlist[((point_current - 1) * TWO_D) + 1];

    /* -- Point characterstics -- */
    bool isin_list  = linalg_list_isIn_2D_int32_t(arrow->pathlist, point_current, x_next, y_next);
    bool isprevious = ((x_next == x_previous) && (y_next == y_previous));

    /* - Arrow can be infinitely long or not - */
    #ifdef INFINITE_MOVE_ALL
    bool istoolong = (num_current > ARROW_MAX_LEN);
    #else
    bool istoolong = (num_current > arrow->move);
    #endif /* INFINITE_MOVE_ALL */

    /* - Point decider - */
    if (isprevious) {
        /* Next point is the previous point */
        /* Reduce arrow size by one */
        DARR_NUM(arrow->pathlist) -= TWO_D;
        if (point_current >= 2)
            Arrow_Decider(arrow, point_current - 2);
    } else if (istoolong || isin_list) {
        /* Next point is in list OR list is too long */
        /* Trace arrow */
        struct Point end = {x_next, y_next};
        Arrow_Path_Trace(arrow, end);
    } else {
        /* Next point is: */
        /*    not previous one, not in pathlist, pathlist isn't too long. */
        /* -> add it to pathlist */
        DARR_PUT(arrow->pathlist, x_next);
        DARR_PUT(arrow->pathlist, y_next);
        Arrow_Decider(arrow, point_current);
        if (point_current > 0)
            Arrow_Decider(arrow, point_current - 1);
    }
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* - Retracing path using A* (A_star) algorithm - */
void Arrow_Path_Trace(struct Arrow *arrow, struct Point end_in) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(arrow->textures != NULL);
    SDL_assert(arrow->costmap  != NULL);
    struct Point end   = {end_in.x, end_in.y};
    struct Point start = {arrow->pathlist[0], arrow->pathlist[1]};

    DARR_NUM(arrow->pathlist) = 0;
    if ((start.x != end.x) || (start.y != end.y)) {
        /* A* implemented here. Goes backwards for some reason. */
        /* IMPORTANT NOTE: Switching start and end CRASHES MY COMPUTER. */
        arrow->pathlist = Pathfinding_Astar(arrow->pathlist, arrow->costmap,
                                            arrow->row_len, arrow->col_len, start, end);
        /* Flipping path. */
        i32 pointnum    = DARR_NUM(arrow->pathlist) / TWO_D;
        for (i32 i = 0; i < (pointnum - 1); i++)
            Arrow_Decider(arrow, i);
    } else
        DARR_NUM(arrow->pathlist) = TWO_D;
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* - Texture decider: for current point - */
void Arrow_Decider(struct Arrow *arrow, i32 point) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    /* -- Preliminaries -- */
    SDL_assert(arrow->textures != NULL);
    SDL_assert(point >= 0);
    uf8 pointnum = DARR_NUM(arrow->pathlist) / TWO_D;
    i32 endpoint = (pointnum - 2);
    SDL_assert(point <= endpoint);
    #ifndef INFINITE_MOVE_ALL
    pointnum = pointnum < (arrow->move + 1) ? pointnum : (arrow->move + 1) ;
    #endif /* INFINITE_MOVE_ALL */

    /* - tiles around point - */
    /* 0,1,2 tiles -> 0 = behind, 1 = current, 2 = next */
    i32 x_0 = 0, x_1 = 0, x_2 = 0, y_0 = 0, y_1 = 0, y_2 = 0;
    if (point > 0) {
        x_0 = arrow->pathlist[(point - 1) * TWO_D];
        y_0 = arrow->pathlist[(point - 1) * TWO_D + 1];
    }
    x_1 = arrow->pathlist[   point    * TWO_D];
    y_1 = arrow->pathlist[   point    * TWO_D + 1];
    x_2 = arrow->pathlist[(point + 1) * TWO_D];
    y_2 = arrow->pathlist[(point + 1) * TWO_D + 1];

    /* -- Deciding texture -- */
    if (point == 0) {
        if (pointnum == 2)
            arrow->rendereds[point] = Arrow_Decider_Startend(x_1, y_1, x_2, y_2);
        else
            arrow->rendereds[point] = Arrow_Decider_Start(x_1, y_1, x_2, y_2);
    } else if (point == endpoint) {
        arrow->rendereds[point] = Arrow_Decider_End(x_0, y_0, x_1, y_1, x_2, y_2);
    } else {
        arrow->rendereds[point] = Arrow_Decider_Middle(x_0, y_0, x_1, y_1, x_2, y_2);
    }
    SDL_assert(arrow->textures != NULL);
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}

/* Short arrow, start and end point in one tile. */
struct Rendered Arrow_Decider_Startend(i32 x_0, i32 y_0, i32 x_1, i32 y_1) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

/* Start tile of arrow, if longer than one tile. */
struct Rendered Arrow_Decider_Start(i32 x_0, i32 y_0, i32 x_1, i32 y_1) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

/* Middle tile of arrow, if longer than one tile. */
struct Rendered Arrow_Decider_Middle(i32 x_0, i32 y_0, i32 x_1, i32 y_1, i32 x_2, i32 y_2) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
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
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

/* End tile of arrow, if longer than one tile. */
struct Rendered Arrow_Decider_End(i32 x_0, i32 y_0, i32 x_1, i32 y_1, i32 x_2, i32 y_2) {
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    struct Rendered out  = Rendered_default;

    struct Point move_01 = {x_1 - x_0, y_1 - y_0};
    int direction_01     = Ternary_Direction(move_01);
    struct Point move_12 = {x_2 - x_1, y_2 - y_1};
    int direction_12     = Ternary_Direction(move_12);

    bool left_01 = (direction_01 == SOTA_DIRECTION_LEFT);
    bool left_12 = (direction_12 == SOTA_DIRECTION_LEFT);

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
    SOTA_Log_Func("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
    return (out);
}

void Arrow_Draw(struct Arrow *arrow, SDL_Renderer *renderer, struct Camera *camera) {
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), call_stack_depth++, __func__);
    SDL_assert(arrow->textures != NULL);
    SDL_assert(arrow->pathlist != NULL);
    i32 x, y, num = DARR_NUM(arrow->pathlist) / TWO_D;
    SDL_Rect srcrect = {.w = arrow->tilesize[0], .h = arrow->tilesize[1]}, dstrect = {0};
    dstrect.h = SOTA_ZOOM(arrow->map_tilesize[1], camera->zoom);
    dstrect.w = SOTA_ZOOM(arrow->map_tilesize[0], camera->zoom);
    for (i32 i = 0; i < (num - 1); i++) {
        x = arrow->pathlist[TWO_D * i];
        y = arrow->pathlist[(TWO_D * i) + 1];
        struct Rendered rend = arrow->rendereds[i];
        dstrect.x = SOTA_ZOOM(x * arrow->map_tilesize[0], camera->zoom) + camera->offset.x;
        dstrect.y = SOTA_ZOOM(y * arrow->map_tilesize[1], camera->zoom) + camera->offset.y;
        srcrect.x = ((rend.graphics.index - 1) % TILESET_COL_LEN) * arrow->tilesize[0];
        srcrect.y = ((rend.graphics.index - 1) / TILESET_COL_LEN) * arrow->tilesize[1];
        SDL_RenderCopyEx(renderer, arrow->textures, &srcrect, &dstrect, 0, NULL, rend.flip);
    }
    SOTA_Log_FPS("%d\t%s\t" STRINGIZE(__LINE__), --call_stack_depth, __func__);
}
