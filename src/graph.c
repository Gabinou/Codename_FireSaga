
#include "graph.h"
#include "palette.h"
#include "n9patch.h"
#include "pixelfonts.h"
#include "filesystem.h"

#include "unit/unit.h"
#include "unit/stats.h"
#include "unit/loadout.h"
#include "unit/equipment.h"

#include "stb_sprintf.h"

// TODO: move test grown stat stuff to render tests
Unit_stats test_grown_stats[10] = {
    /*hp str mag agi dex fth luck def res con move prof */
    { 0,  1,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0},
    { 1,  1,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0},
    { 0,  1,  1,  0,  0,  1,  0,  1,  1,  0,  0,  1},
    { 0,  1,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1},
    { 1,  0,  0,  0,  0,  1,  0,  1,  0,  0,  0,  1},
    { 0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1},
    { 0,  1,  0,  0,  0,  1,  0,  1,  0,  0,  0,  0},
    { 1,  0,  1,  0,  0,  1,  0,  1,  0,  0,  0,  0},
    { 0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  1,  0,  0,  0,  1,  0,  0,  1,  0,  0,  1},
};

Unit_stats test_base_stats = {
    /*  hp str mag agi dex fth luck def res con move prof */
    05, 06, 07, 8, 9, 10, 11, 12, 13, 01, 02, 03
};

const GraphStat GraphStat_default = {0};

const Graph Graph_default = {
    .y_lenperpixel = GRAPH_DEFAULT_LENPERPIXEL_HEIGHT, /* 2 or 1 */
    .size = {
        .x = GRAPH_DATA_WIDTH   / GRAPH_DEFAULT_LENPERPIXEL_WIDTH,
        .y = SOTA_MAX_STAT      / GRAPH_DEFAULT_LENPERPIXEL_HEIGHT
    },

    .max = {
        .x    = SOTA_MAX_LEVEL,
        .y    = SOTA_MAX_STAT_PC
    },
    .ticks = {
        .x    = 1,
        .y    = 1
    }
};

Point Graph_Pixel_Tick_Dist(Graph *graph, Point tick_num) {
    /* Distance between minor/major ticks */
    Point out = {
        .x = GRAPH_DATA_WIDTH / GRAPH_TICK_LABELS_DIVISOR / 2,
        .y = GRAPH_DATA_HEIGHT / tick_num.y
    };
    return (out);
}

Point Graph_Pixel_Tick_Num(Graph *graph) {
    /* Compute number of ticks in graph, both
    **  major and minor ticks */

    Point out = {
        /* X: always 5 levels between each tick */
        .x = graph->max.x / GRAPH_TICK_LVL_DIST,
        /* Y: Always fixed number of ticks shown */
        .y = graph->max.y / GRAPH_TICK_Y_NUM,
    };
    return (out);
}

Point Graph_Pixel_Pos(Graph *graph, Point point) {
    /* From data XY space to pixel texture space
    ** [level, stat] -> [px, px]
    /* Note: scale factors:
        1. 2x: 2 pixels per point (faking low res).
        2. 2x: Each point 1 point apart.
     */
    Point out = {
        .x = point.x * GRAPH_SCALE,
        .y = point.y * GRAPH_SCALE
    };
    return (out);
}

void Graph_Stat_Remove(Graph *graph, i32 stat_id) {
    graph->graph_stats[stat_id].stat_id = STAT_ID_NULL;
}

void Graph_Stat_Add(Graph *graph, Unit *unit, i32 stat_id) {
    Unit_stats  bases   = Unit_Stats_Bases(unit);
    Unit_stats *grown   = Unit_Stats_Grown(unit);
    i32 level           = Unit_Level(unit);
    i32 base_level      = Unit_Base_Level(unit);

    _Graph_Stat_Add(graph,      &bases,
                    grown,      level,
                    base_level, stat_id);
}

void _Graph_Stat_Add(Graph       *graph,
                     Unit_stats  *base_stats,
                     Unit_stats  *grown_stats,
                     i32 level,  i32 base_level,
                     i32 stat_id) {

    GraphStat graph_stat    = {
        .stat_id      = stat_id
    };
    GraphStat_Cumul(&graph_stat,    base_stats,
                    grown_stats,   level,
                    base_level);
    graph->graph_stats[stat_id] = graph_stat;
}

void GraphStat_Cumul(   GraphStat   *gstat,
                        Unit_stats  *base_stats,
                        Unit_stats  *grown_stats,
                        i32 level,  i32 base_level) {
    /* Compute cumul_stat for input gstat */
    i32 *base_arr   = Unit_stats_arr(base_stats);
    i32  stat_id    = gstat->stat_id;

    /* Compute cumul_stat using unit grown_stat */
    i32 grown = 0;
    for (i32 l = 0; l <= (level - base_level); l++) {
        /* No growth yet at base level */
        if (l > 0) {
            i32 *grown_arr = Unit_stats_arr(&grown_stats[l - 1]);
            grown += grown_arr[stat_id];
        }
        gstat->cumul_stat[l] = base_arr[stat_id] + grown;
    }
}

void _Graph_Draw_Spines(Graph           *graph,
                        SDL_Rect         spines[TWO_D],
                        n9Patch         *n9patch,
                        PixelFont       *pixelnours_big,
                        SDL_Renderer    *renderer,
                        SDL_Texture     *render_target) {
    SDL_Rect spine_x = spines[DIM_X];
    SDL_Rect spine_y = spines[DIM_Y];

    /* -- axes spines -- */
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRects(renderer, spines, TWO_D);

}
void _Graph_Draw_Ticks( Graph           *graph,
                        SDL_Rect         spines[TWO_D],
                        n9Patch         *n9patch,
                        PixelFont       *pixelnours_big,
                        SDL_Renderer    *renderer,
                        SDL_Texture     *render_target) {
    SDL_Rect spine_x = spines[DIM_X];
    SDL_Rect spine_y = spines[DIM_Y];

    /* -- axes ticks -- */
    Point tick_num  = Graph_Pixel_Tick_Num(graph);
    Point tick_dist = Graph_Pixel_Tick_Dist(graph, tick_num);

    if (graph->ticks.x) {
        SDL_Rect tick = {0, 0, 1, 1};
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
        for (i32 i = 1; i < tick_num.x - 1; i++) {
            /* TODO: draw x tick util */
            tick.h = GRAPH_TICK_SIZE(i);
            tick.x = spine_x.x + i * tick_dist.x + GRAPH_XAXIS_OFFSET;
            tick.y = spine_x.y - tick.h / 2;
            SDL_RenderFillRect(renderer, &tick);
        }
    }
    if (graph->ticks.y) {
        SDL_Rect tick = {0, 0, 1, 1};
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
        for (i32 i = 1; i < tick_num.y + 2; i++) {
            /* TODO: draw y tick util */
            tick.w = GRAPH_TICK_MAJOR_LEN;
            tick.x = spine_x.x - tick.w / 2;
            tick.y = spine_x.y - i * tick_dist.y - GRAPH_YAXIS_OFFSET;
            SDL_RenderFillRect(renderer, &tick);
        }
    }
}

void _Graph_Draw_Labels(Graph           *graph,
                        SDL_Rect         spines[TWO_D],
                        n9Patch         *n9patch,
                        PixelFont       *pixelnours_big,
                        SDL_Renderer    *renderer,
                        SDL_Texture     *render_target) {
    SDL_Rect spine_x = spines[DIM_X];
    SDL_Rect spine_y = spines[DIM_Y];

    /* -- Writing ticks labels -- */
    SDL_Rect label = {0, 0, 1, 1};
    /* TODO: utility to draw labels */
    /* - X labels - */
    Point tick_num  = Graph_Pixel_Tick_Num(graph);
    Point tick_dist = Graph_Pixel_Tick_Dist(graph, tick_num);

    char numbuff[8];

    for (i32 i = 1; i < tick_num.x; i++) {
        label.h = GRAPH_TICK_SIZE(i);
        label.x = spine_x.x + i * tick_dist.x * 2 + GRAPH_XAXIS_OFFSET - GRAPH_YLABEL_X_OFFSET;
        label.y = spine_x.y + GRAPH_YLABEL_Y_OFFSET;
        stbsp_sprintf(numbuff, "%02d\0\0\0\0", i * tick_num.x);
        PixelFont_Write(pixelnours_big, renderer,
                        numbuff,        strlen(numbuff),
                        label.x,        label.y);
    }
    /* - Y labels - */
    for (i32 i = 1; i < tick_num.y + 2; i++) {
        label.w = GRAPH_TICK_SIZE(i);
        label.x = spine_x.x - GRAPH_XLABEL_X_OFFSET;
        label.y = spine_x.y - i * tick_dist.y - GRAPH_YAXIS_OFFSET - GRAPH_XLABEL_Y_OFFSET;
        stbsp_sprintf(numbuff, "%02d\0\0\0\0", i * GRAPH_TICK_Y_NUM);
        PixelFont_Write(pixelnours_big, renderer,
                        numbuff,        strlen(numbuff),
                        label.x,        label.y);
    }
}

void Graph_Spines(Graph *graph, SDL_Rect spines[TWO_D]) {
    /* Rect of each XY spine forming the axes */
    if (graph == NULL) {
        SDL_assert(false);
        return;
    }

    Point margin_xy = Margin_XY(graph->margin);

    spines[DIM_X].x = graph->margin.left;
    spines[DIM_X].y = graph->rect.h - graph->margin.bottom;
    spines[DIM_X].w = graph->rect.w - margin_xy.x - 10;
    spines[DIM_X].h = 1;

    /* Note: rect fills from top to bottom, like Y */
    spines[DIM_Y].x = graph->margin.left;
    spines[DIM_Y].y = graph->margin.top;
    spines[DIM_Y].w = 1;
    spines[DIM_Y].h = graph->rect.h - margin_xy.y;
}

void _Graph_Draw_Axes_Shadows(
        Graph           *graph,
        SDL_Rect         spines[TWO_D],
        n9Patch         *n9patch,
        PixelFont       *pixelnours_big,
        SDL_Renderer    *renderer,
        SDL_Texture     *render_target
) {
    SDL_Rect spine_x = spines[DIM_X];
    SDL_Rect spine_y = spines[DIM_Y];

    /* axes spine shadows */
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);

    SDL_Rect shadow_x = {
        .x = spine_y.x + 1,
        .y = spine_x.y + 1,
        .w = spine_x.w,
        .h = spine_x.h
    };
    SDL_Rect shadow_0 = {
        .x = spine_y.x + spine_x.w,
        .y = spine_x.y,
        .w = 1,
        .h = 1
    };
    SDL_Rect shadow_y = {
        .x = spine_y.x + 1,
        .y = spine_y.y,
        .w = 1,
        .h = spine_y.h
    };
    SDL_Rect rects2[3] = {shadow_x, shadow_0, shadow_y};
    SDL_RenderFillRects(renderer, rects2, 3);

    /* -- axes ticks shadows -- */
    Point tick_num  = Graph_Pixel_Tick_Num(graph);
    Point tick_dist = Graph_Pixel_Tick_Dist(graph, tick_num);
    if (graph->ticks.x) {
        SDL_Rect tick = {0, 0, 1, 1};
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        for (i32 i = 1; i < tick_num.x - 1; i++) {
            /* TODO: draw x tick util */
            tick.h = GRAPH_TICK_SIZE(i);
            tick.x = spine_x.x + i * tick_dist.x + GRAPH_XAXIS_OFFSET + 1;
            tick.y = spine_x.y - tick.h / 2;
            tick.h += 1;
            SDL_RenderFillRect(renderer, &tick);
            tick.x -= 1;
            tick.y = spine_x.y + tick.h / 2;
            tick.h = 1;
            SDL_RenderFillRect(renderer, &tick);
        }
    }
    if (graph->ticks.y) {
        SDL_Rect tick = {0, 0, 1, 1};
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        for (i32 i = 1; i < tick_num.y + 2; i++) {
            /* TODO: draw y tick util */
            tick.w = GRAPH_TICK_MAJOR_LEN;
            tick.x = spine_x.x - tick.w / 2;
            tick.y = spine_x.y - i * tick_dist.y - GRAPH_YAXIS_OFFSET + 1;
            tick.w += 1;
            SDL_RenderFillRect(renderer, &tick);
            tick.x = spine_x.x + (tick.w - 1) / 2 + 1;
            tick.y -= 1;
            tick.w = 1;
            SDL_RenderFillRect(renderer, &tick);
        }
    }
}

void _Graph_Draw_Axes(  Graph           *graph,
                        n9Patch         *n9patch,
                        PixelFont       *pixelnours_big,
                        SDL_Renderer    *renderer,
                        SDL_Texture     *render_target) {
    /* --- Draw X and Y axes. --- */
    /* -- Clear graph -- */
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_TRANSPARENT);
    SDL_RenderFillRect(renderer, NULL);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);

    /* Computing rectangles of X/Y axes spines */
    SDL_Rect spines[TWO_D] = {0};
    Graph_Spines(graph, spines);
    SDL_Rect spine_x = spines[DIM_X];
    SDL_Rect spine_y = spines[DIM_Y];

    _Graph_Draw_Axes_Shadows(   graph,      spines,
                                n9patch,    pixelnours_big,
                                renderer,   render_target);
    _Graph_Draw_Spines( graph,      spines,
                        n9patch,    pixelnours_big,
                        renderer,   render_target);
    _Graph_Draw_Ticks(  graph,      spines,
                        n9patch,    pixelnours_big,
                        renderer,   render_target);
    _Graph_Draw_Labels( graph,      spines,
                        n9patch,    pixelnours_big,
                        renderer,   render_target);
}

void _Graph_Draw_Stats( Graph           *graph,
                        n9Patch         *n9patch,
                        PixelFont       *pixelnours_big,
                        SDL_Renderer    *renderer,
                        SDL_Texture     *render_target) {
    /* --- Draw all valid stat lines. --- */

    for (int i = UNIT_STAT_NULL + 1; i <= UNIT_STAT_NUM; i++) {
        if (graph->graph_stats[i].stat_id == STAT_ID_NULL)
            continue;

        _Graph_Draw_Stat(   graph,      i,
                            n9patch,    pixelnours_big,
                            renderer,   render_target);
    }
}


void _Graph_Draw_Stat(  Graph           *graph,
                        i32              stat_id,
                        n9Patch         *n9patch,
                        PixelFont       *pixelnours_big,
                        SDL_Renderer    *renderer,
                        SDL_Texture     *render_target) {
    /* Drawing a single line, showing stat grown per level. */

    /* -- Preliminaries -- */
    GraphStat graph_stat = graph->graph_stats[stat_id];
    SDL_Rect axes = {
        .x = graph->margin.left + GRAPH_XAXIS_OFFSET,
        .y = graph->rect.h - graph->margin.bottom + GRAPH_YAXIS_OFFSET - 2,
        .w = GRAPH_DATA_WIDTH,
        .h = GRAPH_DATA_HEIGHT,
    };
    SDL_Rect point = {0, 0, 1, 1};

    /* -- Drawing stats -- */
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    for (i32 i = 0; i <= (graph->level - graph->base_level); i++) {
        point.x = axes.x + (i + graph->base_level) * axes.w / graph->size.x;
        point.y = axes.y - graph->y_lenperpixel * graph_stat.cumul_stat[i];
        SDL_RenderFillRect(renderer, &point);
    }
}

void _Graph_Draw_Lvl(  Graph           *graph,
                       n9Patch         *n9patch,
                       PixelFont       *pixelnours_big,
                       SDL_Renderer    *renderer,
                       SDL_Texture     *render_target) {
    /* --- Drawing a vertical line, and level #. --- */
    SDL_Rect axes = {
        .x = graph->margin.left + GRAPH_XAXIS_OFFSET,
        .y = graph->rect.h - graph->margin.bottom + GRAPH_YAXIS_OFFSET - 2,
        .w = GRAPH_DATA_WIDTH,
        .h = GRAPH_DATA_HEIGHT,
    };
    SDL_Rect point = {0, 0, 1, 1};

    /* -- Drawing bar at level -- */
    SDL_SetRenderDrawColor( renderer, 0xB2, 0x10, 0x30,
                            SDL_ALPHA_OPAQUE);
    SDL_Rect level = {
        .x = axes.x + graph->level * axes.w / graph->size.x,
        .y = GRAPH_LVL_Y_OFFSET + PIXELFONT_HEIGHT,
        .w = 1,
        .h = graph->rect.h - graph->margin.top - PIXELFONT_HEIGHT - GRAPH_TICK_MINOR_LEN + 2,
    };
    SDL_RenderFillRect(renderer, &level);

    /* -- Writing "Lv #" on top of bar -- */
    char numbuff[8];
    stbsp_sprintf(numbuff, "%02d\0\0\0\0", graph->level);
    int height = level.y - PIXELFONT_HEIGHT;
    PixelFont_Write(pixelnours_big, renderer, "Lv", 2, level.x - GRAPH_LVL_X_OFFSET, height);
    PixelFont_Write(pixelnours_big, renderer, numbuff, strlen(numbuff), level.x + 2, height);
}

void _Graph_Draw_Point( Graph *g, Point pos, i32 style) {

}

void Graph_Draw(Graph           *graph, n9Patch *n9patch,
                PixelFont       *pixelnours_big,
                SDL_Renderer    *renderer,
                SDL_Texture     *render_target) {
    SDL_assert(graph                != NULL);
    SDL_assert(n9patch              != NULL);
    SDL_assert(pixelnours_big       != NULL);

    /* Create texture if it doesn't exist */
    if (graph->texture == NULL) {
        graph->texture = SDL_CreateTexture(
                                 renderer, SDL_PIXELFORMAT_ARGB8888,
                                 SDL_TEXTUREACCESS_TARGET,
                                 graph->rect.w, graph->rect.h
                         );
        SDL_assert(graph->texture != NULL);
        SDL_SetTextureBlendMode(graph->texture, SDL_BLENDMODE_BLEND);
    }

    SDL_SetRenderTarget(renderer, graph->texture);
    _Graph_Draw_Axes(   graph,          n9patch,
                        pixelnours_big, renderer,
                        render_target);
    _Graph_Draw_Stats(  graph,          n9patch,
                        pixelnours_big, renderer,
                        render_target);
    _Graph_Draw_Lvl(    graph,          n9patch,
                        pixelnours_big, renderer,
                        render_target);
    SDL_SetRenderTarget(renderer, render_target);
}
