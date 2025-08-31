
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

    .ticks.x    = 1,
    .ticks.y    = 1,
};

Point Graph_Pixel_Pos(Graph *graph, Point point) {
    /* From data XY space to pixel texture space
    ** [level, stat] -> [px, px] */
    /* Note: scale factors:
        1. 2x: 2 pixels per point (faking low res). 
        2. 2x: Each point 1 point apart. 
     */
    Point out = {
        .x = point.x * GRAPH_SCALE,
        .y = point.y * GRAPH_SCALE
    }
    return(out);
}

void Graph_Stat_Remove(Graph *graph, i32 stat_id) {
    graph->graph_stats[stat_id].stat_id = STAT_ID_NULL;
}

void Graph_Stat_Add(Graph *graph, Unit *unit, i32 stat_id) {
    struct Unit_stats  bases = Unit_Stats_Bases(unit);
    struct Unit_stats *grown = Unit_Stats_Grown(unit);
    i32 level       = Unit_Level(unit);
    i32 base_level  = Unit_Base_Level(unit);

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
    GraphStat_Cumul(&graph_stat, base_stats, grown_stats,
                    level, base_level);
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

void _Graph_Draw_Axes(  Graph           *graph,
                        n9Patch         *n9patch,
                        PixelFont       *pixelnours_big,
                        SDL_Renderer    *renderer,
                        SDL_Texture     *render_target) {
    /* --- Draw X and Y axes. --- */
    /* -- Clear graph -- */
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_TRANSPARENT);
    SDL_RenderFillRect(renderer, NULL);

    /* -- Draw graph -- */
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    char numbuff[8];
    SDL_Rect yrect = {
        .x = graph->margin_left,
        .y = graph->header,
        .w = 1,
        .h = graph->rect.h - graph->header - graph->footer
    };
    SDL_Rect xrect = {
        .x = yrect.x,
        .y = graph->rect.h - graph->footer,
        .w = graph->rect.w - graph->margin_left - graph->margin_right - 10,
        .h = 1
    };
    i32 data_width  = GRAPH_DATA_WIDTH;
    i32 data_height = GRAPH_DATA_HEIGHT;
    SDL_Rect rects[TWO_D] = {xrect, yrect};
    i32 y_lvl_dist; /* distances in levels to next y tick */
    /* TODO enum y_lvl_dist/array, or remove this? */
    switch (graph->y_lenperpixel) {
        case 1:
            y_lvl_dist = 10; /* [levels] */
            break;
        case 2:
            y_lvl_dist = 5;  /* [levels] */
            break;
    }
    /* --- DRAWING SHADOWS --- */
    /* axes ticks shadows */
    if (graph->ticks.x) {
        i32 tick_num = graph->size.x / GRAPH_TICK_LABELS_DIVISOR;
        i32 x_dist = data_width / GRAPH_TICK_LABELS_DIVISOR / 2;
        SDL_Rect tick = {0, 0, 1, 1};
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        for (i32 i = 1; i < tick_num - 1; i++) {
            tick.h = (i % 2) == 0 ? GRAPH_TICK_MAJOR_LEN : GRAPH_TICK_MINOR_LEN;
            tick.x = xrect.x + i * x_dist + GRAPH_XAXIS_OFFSET + 1;
            tick.y = xrect.y - tick.h / 2;
            tick.h += 1;
            SDL_RenderFillRect(renderer, &tick);
            tick.x -= 1;
            tick.y = xrect.y + tick.h / 2;
            tick.h = 1;
            SDL_RenderFillRect(renderer, &tick);
        }
    }
    if (graph->ticks.y) {
        i32 tick_num = graph->size.y / y_lvl_dist;
        i32 y_dist = data_height / tick_num;
        SDL_Rect tick = {0, 0, 1, 1};
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        for (i32 i = 1; i < tick_num + 2; i++) {
            tick.w = GRAPH_TICK_MAJOR_LEN;
            tick.x = xrect.x - tick.w / 2;
            tick.y = xrect.y - i * y_dist - GRAPH_YAXIS_OFFSET + 1;
            tick.w += 1;
            SDL_RenderFillRect(renderer, &tick);
            tick.x = xrect.x + (tick.w - 1) / 2 + 1;
            tick.y -= 1;
            tick.w = 1;
            SDL_RenderFillRect(renderer, &tick);
        }
    }
    /* axes spine shadows */
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_Rect shadow1 = {yrect.x + 1, xrect.y + 1, xrect.w, xrect.h};
    SDL_Rect shadow2 = {yrect.x + xrect.w, xrect.y, 1, 1};
    SDL_Rect shadow3 = {yrect.x + 1, yrect.y, 1, yrect.h};
    SDL_Rect rects2[3] = {shadow1, shadow2, shadow3};
    SDL_RenderFillRects(renderer, rects2, 3);
    /* --- DRAWING GRAPH --- */
    /* axes spines */
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRects(renderer, rects, TWO_D);
    /* axes ticks*/
    if (graph->ticks.x) {
        i32 tick_num = graph->size.x / GRAPH_TICK_LABELS_DIVISOR;
        i32 x_dist = data_width / GRAPH_TICK_LABELS_DIVISOR / 2;
        SDL_Rect tick = {0, 0, 1, 1};
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
        for (i32 i = 1; i < tick_num - 1; i++) {
            tick.h = (i % 2) == 0 ? GRAPH_TICK_MAJOR_LEN : GRAPH_TICK_MINOR_LEN;
            tick.x = xrect.x + i * x_dist + GRAPH_XAXIS_OFFSET;
            tick.y = xrect.y - tick.h / 2;
            SDL_RenderFillRect(renderer, &tick);
        }
    }
    if (graph->ticks.y) {
        i32 tick_num = graph->size.y / y_lvl_dist;
        i32 y_dist = data_height / tick_num;
        SDL_Rect tick = {0, 0, 1, 1};
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
        for (i32 i = 1; i < tick_num + 2; i++) {
            tick.w = GRAPH_TICK_MAJOR_LEN;
            tick.x = xrect.x - tick.w / 2;
            tick.y = xrect.y - i * y_dist - GRAPH_YAXIS_OFFSET;
            SDL_RenderFillRect(renderer, &tick);
        }
    }
    SDL_Rect label = {0, 0, 1, 1};
    i32 x_tick_num = graph->size.x / GRAPH_TICK_LABELS_DIVISOR;
    i32 x_dist = data_width / GRAPH_TICK_LABELS_DIVISOR;
    for (i32 i = 1; i < x_tick_num; i++) {
        label.h = (i % 2) == 0 ? GRAPH_TICK_MAJOR_LEN : GRAPH_TICK_MINOR_LEN;
        label.x = xrect.x + i * x_dist + GRAPH_XAXIS_OFFSET - GRAPH_YLABEL_X_OFFSET;
        label.y = xrect.y + GRAPH_YLABEL_Y_OFFSET;
        stbsp_sprintf(numbuff, "%02d\0\0\0\0", i * x_tick_num);
        PixelFont_Write(pixelnours_big, renderer, numbuff, strlen(numbuff), label.x, label.y);
    }
    i32 y_tick_num = graph->size.y / y_lvl_dist;
    i32 y_dist = data_height / y_tick_num;
    for (i32 i = 1; i < y_tick_num + 2; i++) {
        label.w = (i % 2) == 0 ? GRAPH_TICK_MAJOR_LEN : GRAPH_TICK_MINOR_LEN;
        label.x = xrect.x - GRAPH_XLABEL_X_OFFSET;
        label.y = xrect.y - i * y_dist - GRAPH_YAXIS_OFFSET - GRAPH_XLABEL_Y_OFFSET;
        stbsp_sprintf(numbuff, "%02d\0\0\0\0", i * y_lvl_dist);
        PixelFont_Write(pixelnours_big, renderer, numbuff, strlen(numbuff), label.x, label.y);
    }
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
        .x = graph->margin_left + GRAPH_XAXIS_OFFSET,
        .y = graph->rect.h - graph->footer + GRAPH_YAXIS_OFFSET - 2,
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
        .x = graph->margin_left + GRAPH_XAXIS_OFFSET,
        .y = graph->rect.h - graph->footer + GRAPH_YAXIS_OFFSET - 2,
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
        .h = graph->rect.h - graph->header - PIXELFONT_HEIGHT - GRAPH_TICK_MINOR_LEN + 2,
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
