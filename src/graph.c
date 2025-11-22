/*
**  Copyright 2025 Gabriel Taillon
**  Licensed under GPLv3
**
**      Éloigne de moi l'esprit d'oisiveté, de
**          découragement, de domination et de
**          vaines paroles.
**      Accorde-moi l'esprit d'intégrité,
**          d'humilité, de patience et de charité.
**      Donne-moi de voir mes fautes.
**
***************************************************
**
** Graph to show grown stats.
**
*/

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

/* Margin for anything outside the axes */
const Margin graph_margin = {
    .top    = GRAPH_MARGIN_TOP,
    .bottom = GRAPH_MARGIN_BOTTOM,
    .left   = GRAPH_MARGIN_LEFT,
    .right  = GRAPH_MARGIN_RIGHT
};

const Point graph_size = {
    .x = GRAPH_DATA_WIDTH +
    GRAPH_DATA_MARGIN_LEFT + GRAPH_MARGIN_LEFT +
    GRAPH_DATA_MARGIN_RIGHT + GRAPH_MARGIN_RIGHT,
    .y = GRAPH_DATA_HEIGHT +
    GRAPH_DATA_MARGIN_BOTTOM + GRAPH_MARGIN_BOTTOM +
    GRAPH_DATA_MARGIN_TOP + GRAPH_MARGIN_TOP,
};

const GraphStat GraphStat_default = {0};

const Graph Graph_default = {
    .ticks = {
        .x    = 1,
        .y    = 1
    }
};

void Graph_Free(Graph *graph) {
    if (graph->texture_1x != NULL) {
        SDL_DestroyTexture(graph->texture_1x);
        graph->texture_1x = NULL;
    }
    if (graph->texture_2x != NULL) {
        SDL_DestroyTexture(graph->texture_2x);
        graph->texture_2x = NULL;
    }
}

SDL_Texture *Graph_Texture(const Graph *graph) {
    return (graph->texture_2x);
}

i32 Graph_Level_Num(const Graph *graph) {
    /* Number of levels to plot */
    int num = graph->level - graph->base_level;
    return (num > 1 ? num : 1);
}

Point Graph_Pixel_Level_Dist(void) {
    /* Distance in 1x pixels between levels */
    Point out = {
        .x = GRAPH_POINT_pxDIST_X,
        .y = GRAPH_POINT_pxDIST_Y
    };
    return (out);
}

Point Graph_Pixel_Tick_Dist(void) {
    /* Distance in 1x pixels between minor/major ticks */
    Point out = Graph_Pixel_Level_Dist();
    out.x *= GRAPH_TICK_X_DIST;
    out.y *= GRAPH_TICK_Y_DIST;
    return (out);
}

Point Graph_Pixel_Tick_Num(void) {
    /* Compute number of ticks in graph, both
    **  major and minor ticks */
    Point out = {
        .x = GRAPH_TICK_X_NUM,
        .y = GRAPH_TICK_Y_NUM
    };
    return (out);
}

void Graph_Stat_Remove(Graph *graph, i32 stat_id) {
    graph->graph_stats[stat_id].stat_id = STAT_ID_NULL;
}

void Graph_Stat_Style(Graph *graph, i32 stat, i32 style) {
    graph->styles[stat] = style;
}

void Graph_Stat_Color(Graph *graph, i32 stat, i32 color) {
    graph->colors[stat] = color;
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
    graph->level        = level;
    graph->base_level   = base_level;

    GraphStat graph_stat    = {
        .stat_id      = stat_id
    };
    GraphStat_Cumul(&graph_stat,   base_stats,
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
        i32 cumul = base_arr[stat_id] + grown;
        gstat->cumul_stat[l] = cumul < SOTA_MAX_STAT_PC ? cumul : SOTA_MAX_STAT_PC;
    }
}

void _Graph_Draw_Spines(Graph           *graph,
                        SDL_Rect         spines[TWO_D],
                        n9Patch         *n9patch,
                        PixelFont       *pixelnours_big,
                        SDL_Renderer    *renderer) {
    SDL_Rect spine_x = spines[DIM_X];
    SDL_Rect spine_y = spines[DIM_Y];

    /* -- axes spines -- */
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF,
                            SDL_ALPHA_OPAQUE);
    SDL_RenderFillRects(renderer, spines, TWO_D);

}
void _Graph_Draw_Ticks( Graph           *graph,
                        SDL_Rect         spines[TWO_D],
                        n9Patch         *n9patch,
                        PixelFont       *pixelnours_big,
                        SDL_Renderer    *renderer) {
    SDL_Rect spine_x = spines[DIM_X];
    SDL_Rect spine_y = spines[DIM_Y];

    /* -- axes ticks -- */
    Point tick_num  = Graph_Pixel_Tick_Num();
    Point tick_dist = Graph_Pixel_Tick_Dist();

    if (graph->ticks.x) {
        SDL_Rect tick = {0, 0, 1, 1};
        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF,
                                SDL_ALPHA_OPAQUE);
        for (i32 i = 1; i < tick_num.x + 1; i++) {
            Point stat = {.x = i * GRAPH_TICK_X_DIST};
            Point point = Graph_Point(graph, stat, spines);
            tick.h = GRAPH_TICK_SIZE(i);
            tick.x = point.x;
            tick.y = spine_x.y - tick.h / 2;
            SDL_RenderFillRect(renderer, &tick);
        }
    }
    if (graph->ticks.y) {
        SDL_Rect tick = {0, 0, 1, 1};
        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF,
                                SDL_ALPHA_OPAQUE);
        for (i32 i = 1; i < tick_num.y + 1; i++) {
            Point stat = {.y = i * GRAPH_TICK_Y_DIST};
            Point point = Graph_Point(graph, stat, spines);
            tick.w = GRAPH_TICK_SIZE(i);
            tick.x = spine_x.x - tick.w / 2;
            tick.y = point.y;
            SDL_RenderFillRect(renderer, &tick);
        }
    }
}

void _Graph_Draw_Labels(Graph           *graph,
                        SDL_Rect         spines[TWO_D],
                        n9Patch         *n9patch,
                        PixelFont       *pixelnours_big,
                        SDL_Renderer    *renderer) {
    SDL_Rect spine_x = spines[DIM_X];
    SDL_Rect spine_y = spines[DIM_Y];

    /* -- Writing ticks labels -- */
    SDL_Rect label = {0, 0, 1, 1};
    /* - X labels - */
    Point tick_num  = Graph_Pixel_Tick_Num();
    Point tick_dist = Graph_Pixel_Tick_Dist();

    char numbuff[8];

    for (i32 i = 1; i < tick_num.x + 1; i++) {
        label.h = GRAPH_TICK_SIZE(i);
        /* Skip minor ticks for X labels */
        if (label.h == GRAPH_TICK_MINOR_LEN) {
            continue;
        }
        label.x = spine_x.x + i * tick_dist.x +
                  GRAPH_DATA_MARGIN_LEFT -
                  GRAPH_YLABEL_X_OFFSET;
        label.y = spine_x.y + GRAPH_YLABEL_Y_OFFSET;
        stbsp_sprintf(numbuff, "%02d\0\0\0\0", i * GRAPH_TICK_X_DIST);
        PixelFont_In pxin = {
            .renderer = renderer,
            .text   = numbuff,
            .pos =  {
                .x = label.x,
                .y = label.y,
            }
        };
        PixelFont_Write(pixelnours_big, pxin);
    }
    /* - Y labels - */
    for (i32 i = 1; i < tick_num.y + 1; i++) {
        label.x = spine_x.x - GRAPH_XLABEL_X_OFFSET;
        Point stat = {.y = i * GRAPH_TICK_Y_DIST};
        /* Centering y label around tick */
        Point point = Graph_Point(graph, stat, spines);
        point.y -= PIXELFONT_HEIGHT / 2 - 1;
        stbsp_sprintf(numbuff, "%02d\0\0\0\0", i * GRAPH_MAX_Y / tick_num.y);
        PixelFont_In pxin = {
            .renderer = renderer,
            .text   = numbuff,
            .pos =  {
                .x = label.x,
                .y = point.y,
            }
        };
        PixelFont_Write(pixelnours_big, pxin);
    }
}

void Graph_Spines(Graph *graph, SDL_Rect spines[TWO_D]) {
    /* Rect of each XY spine forming the axes */
    if (graph == NULL) {
        SDL_assert(false);
        return;
    }

    Point margin_xy = Margin_XY(graph_margin);

    spines[DIM_X].x = graph_margin.left;
    spines[DIM_X].y = graph_size.y - graph_margin.bottom;
    spines[DIM_X].w = graph_size.x - margin_xy.x;
    spines[DIM_X].h = 1;

    /* Minimum pixel distance to fit 40 levels */
    i32 mindist =   SOTA_MAX_LEVEL * 2 -
                    GRAPH_TICK_MAJOR_LEN / 2 -
                    GRAPH_TICK_MINOR_LEN / 2;
    SDL_assert(spines[DIM_X].w > mindist);

    /* Note: rect fills from top to bottom, like Y */
    spines[DIM_Y].x = graph_margin.left;
    spines[DIM_Y].y = graph_margin.top;
    spines[DIM_Y].w = 1;
    spines[DIM_Y].h = graph_size.y - margin_xy.y;
}

void _Graph_Draw_Axes_Shadows(
        Graph           *graph,
        SDL_Rect         spines[TWO_D],
        n9Patch         *n9patch,
        PixelFont       *pixelnours_big,
        SDL_Renderer    *renderer
) {
    SDL_Rect spine_x = spines[DIM_X];
    SDL_Rect spine_y = spines[DIM_Y];


    /* axes spine shadows */
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);

    SDL_Rect shadow_x = {
        .x = spine_y.x + 1,         .y = spine_x.y + 1,
        .w = spine_x.w,             .h = spine_x.h
    };
    SDL_Rect shadow_0 = {
        .x = spine_y.x + spine_x.w, .y = spine_x.y,
        .w = 1,                     .h = 1
    };
    SDL_Rect shadow_y = {
        .x = spine_y.x + 1,         .y = spine_y.y,
        .w = 1,                     .h = spine_y.h
    };
    SDL_Rect rects2[3] = {shadow_x, shadow_0, shadow_y};
    SDL_RenderFillRects(renderer, rects2, 3);

    /* -- axes ticks shadows -- */
    Point tick_num  = Graph_Pixel_Tick_Num();
    Point tick_dist = Graph_Pixel_Tick_Dist();
    if (graph->ticks.x) {
        SDL_Rect tick = {0, 0, 1, 1};
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        for (i32 i = 1; i < tick_num.x + 1; i++) {
            Point stat = {.x = i * GRAPH_TICK_X_DIST};
            Point point = Graph_Point(graph, stat, spines);
            tick.h = GRAPH_TICK_SIZE(i);
            tick.x = point.x;
            tick.y = spine_x.y - tick.h / 2;
            tick.h += 1;
            SDL_RenderFillRect(renderer, &tick);
            tick.x += 1;
            tick.y = spine_x.y + tick.h / 2 - 1;
            tick.h = 2;
            SDL_RenderFillRect(renderer, &tick);
        }
    }
    if (graph->ticks.y) {
        SDL_Rect tick = {0, 0, 1, 1};
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        for (i32 i = 1; i < tick_num.y + 1; i++) {
            Point stat = {.y = i * GRAPH_TICK_Y_DIST};
            Point point = Graph_Point(graph, stat, spines);
            tick.w = GRAPH_TICK_SIZE(i);
            tick.x = spine_x.x - tick.w / 2;
            tick.y = point.y;
            tick.w += 1;
            SDL_RenderFillRect(renderer, &tick);
            tick.x = spine_x.x + (tick.w - 1) / 2;
            tick.y += 1;
            tick.w = 2;
            SDL_RenderFillRect(renderer, &tick);
        }
    }
}

void _Graph_Draw_Axes(  Graph           *graph,
                        SDL_Rect         spines[TWO_D],
                        n9Patch         *n9patch,
                        PixelFont       *pixelnours_big,
                        SDL_Renderer    *renderer) {
    /* --- Draw X and Y axes. --- */
    /* -- Clear graph -- */
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_TRANSPARENT);
    SDL_RenderFillRect(renderer, NULL);

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);

    _Graph_Draw_Axes_Shadows(   graph,      spines,
                                n9patch,    pixelnours_big,
                                renderer);
    _Graph_Draw_Spines( graph,      spines,
                        n9patch,    pixelnours_big,
                        renderer);
    _Graph_Draw_Ticks(  graph,      spines,
                        n9patch,    pixelnours_big,
                        renderer);
    _Graph_Draw_Labels( graph,      spines,
                        n9patch,    pixelnours_big,
                        renderer);
}

void _Graph_Draw_Stats( Graph           *graph,
                        SDL_Rect         spines[TWO_D],
                        n9Patch         *n9patch,
                        PixelFont       *pixelnours_big,
                        SDL_Renderer    *renderer) {
    /* --- Draw all valid stat lines. --- */

    for (int i = UNIT_STAT_NULL + 1; i <= UNIT_STAT_NUM; i++) {
        if (graph->graph_stats[i].stat_id == STAT_ID_NULL)
            continue;

        _Graph_Draw_Stat(   graph,          spines,
                            i,              n9patch,
                            pixelnours_big, renderer);
    }
}

Point Graph_Point(  const Graph *graph,
                    Point       stat,
                    SDL_Rect     spines[TWO_D]) {
    /* Position of data point on texture_1x,
    **  according to input [lvl, stat] pair
    **  Note: origin is top left */
    SDL_Rect spine_x = spines[DIM_X];
    SDL_Rect spine_y = spines[DIM_Y];

    Point pos = {
        .x = spine_x.x + stat.x * GRAPH_POINT_pxDIST_X +
        GRAPH_DATA_MARGIN_LEFT,
        .y = spine_y.y +
        (SOTA_MAX_STAT_PC - stat.y) * GRAPH_POINT_pxDIST_Y +
        GRAPH_DATA_MARGIN_TOP
    };
    return (pos);
}

void _Graph_Draw_Stat(  Graph           *graph,
                        SDL_Rect         spines[TWO_D],
                        i32              stat_id,
                        n9Patch         *n9patch,
                        PixelFont       *pixelnours_big,
                        SDL_Renderer    *renderer) {
    /* Drawing a single line, showing stat grown per level. */

    /* -- Preliminaries -- */
    GraphStat graph_stat = graph->graph_stats[stat_id];

    /* -- Drawing stats -- */

    if (graph->colors[stat_id] != 0) {
        SDL_Color color = palette_SOTA->colors[graph->colors[stat_id]];
        SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b,
                                SDL_ALPHA_OPAQUE);
    } else  {
        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF,
                                SDL_ALPHA_OPAQUE);
    }

    i32 lvlnum  = Graph_Level_Num(graph);
    i32 style   = graph->styles[stat_id];
    for (i32 i = 0; i <= lvlnum; i++) {
        Point stat = {
            .x = i + graph->base_level,
            .y = graph_stat.cumul_stat[i]
        };
        Point pos = Graph_Point(graph, stat, spines);
        _Graph_Draw_Point(graph, pos, style, n9patch,
                          pixelnours_big, renderer);
    }
}

void _Graph_Draw_Max_Level( Graph          *graph,
                            SDL_Rect        spines[TWO_D],
                            n9Patch        *n9patch,
                            PixelFont      *pixelnours_big,
                            SDL_Renderer   *renderer) {
    /* --- Drawing a vertical line, and level #. --- */

    /* -- Drawing bar at level -- */
    SDL_SetRenderDrawColor( renderer, 0xB2, 0x10, 0x30,
                            SDL_ALPHA_OPAQUE);
    Point stat = {
        .x = graph->max_level,
        .y = SOTA_MAX_STAT_PC,
    };
    Point point = Graph_Point(graph, stat, spines);

    SDL_Rect level = {
        .x = point.x,
        .y = point.y,
        .w = 1,
        .h = GRAPH_DATA_HEIGHT,
    };
    SDL_RenderFillRect(renderer, &level);

    /* -- Writing "Lv #" on top of bar -- */
    char numbuff[8];
    char *lvlbuff = "Lv";
    stbsp_sprintf(numbuff, "%02d\0\0\0\0", graph->max_level);
    int height = level.y - PIXELFONT_HEIGHT - 1;

    PixelFont_In pxin = {
        .renderer = renderer,
        .text = lvlbuff,
        .pos =  {
            .x = level.x - GRAPH_LVL_X_OFFSET,
            .y = height,
        }
    };
    PixelFont_Write(pixelnours_big, pxin);

    pxin.text = numbuff;
    pxin.pos.x = level.x + 2;
    PixelFont_Write(pixelnours_big, pxin);
}

void _Graph_Draw_Point( Graph           *graph,
                        Point            pos,
                        i32              style,
                        n9Patch         *n9patch,
                        PixelFont       *pixelnours_big,
                        SDL_Renderer    *renderer) {

    int two = 2;
    SDL_Rect dstrect = {
        .x = pos.x * two,
        .y = pos.y * two
    };

    if (style == GRAPH_POINT_1) {
        SDL_RenderDrawPoint(renderer, dstrect.x, dstrect.y);
    }

    if (style == GRAPH_POINT_2) {
        SDL_RenderDrawPoint(renderer, dstrect.x,        dstrect.y);
        SDL_RenderDrawPoint(renderer, dstrect.x + 1,    dstrect.y);
    }

    if (style == GRAPH_POINT_3) {
        SDL_RenderDrawPoint(renderer, dstrect.x,        dstrect.y);
        SDL_RenderDrawPoint(renderer, dstrect.x + 1,    dstrect.y);
        SDL_RenderDrawPoint(renderer, dstrect.x + 2,    dstrect.y);
    }

    if (style == GRAPH_POINT_4) {
        SDL_RenderDrawPoint(renderer, dstrect.x,        dstrect.y);
        SDL_RenderDrawPoint(renderer, dstrect.x + 1,    dstrect.y);
        SDL_RenderDrawPoint(renderer, dstrect.x + 1,    dstrect.y + 1);
    }

    if (style == GRAPH_POINT_5) {
        dstrect.w = 2;
        dstrect.h = 2;
        SDL_RenderFillRect(renderer, &dstrect);
    }

    if (style == GRAPH_POINT_6) {
        SDL_RenderDrawPoint(renderer, dstrect.x,        dstrect.y - 1);
        SDL_RenderDrawPoint(renderer, dstrect.x - 1,    dstrect.y);
        SDL_RenderDrawPoint(renderer, dstrect.x + 1,    dstrect.y);
        SDL_RenderDrawPoint(renderer, dstrect.x,        dstrect.y + 1);
    }
    if (style == GRAPH_POINT_7) {
        SDL_RenderDrawPoint(renderer, dstrect.x,        dstrect.y - 1);
        SDL_RenderDrawPoint(renderer, dstrect.x - 1,    dstrect.y);
        SDL_RenderDrawPoint(renderer, dstrect.x,        dstrect.y);
        SDL_RenderDrawPoint(renderer, dstrect.x + 1,    dstrect.y);
        SDL_RenderDrawPoint(renderer, dstrect.x,        dstrect.y + 1);
    }
    if (style == GRAPH_POINT_8) {
        SDL_RenderDrawPoint(renderer, dstrect.x - 1,    dstrect.y - 1);
        SDL_RenderDrawPoint(renderer, dstrect.x + 1,    dstrect.y - 1);
        SDL_RenderDrawPoint(renderer, dstrect.x,        dstrect.y);
        SDL_RenderDrawPoint(renderer, dstrect.x - 1,    dstrect.y + 1);
        SDL_RenderDrawPoint(renderer, dstrect.x + 1,    dstrect.y + 1);
    }
    if (style == GRAPH_POINT_9) {
        SDL_RenderDrawPoint(renderer, dstrect.x,        dstrect.y);
        SDL_RenderDrawPoint(renderer, dstrect.x + 1,    dstrect.y);
        SDL_RenderDrawPoint(renderer, dstrect.x,        dstrect.y + 1);
        SDL_RenderDrawPoint(renderer, dstrect.x - 1,    dstrect.y + 1);
        SDL_RenderDrawPoint(renderer, dstrect.x + 1,    dstrect.y + 1);
        SDL_RenderDrawPoint(renderer, dstrect.x,        dstrect.y + 2);
    }
    if (style == GRAPH_POINT_10) {
        SDL_RenderDrawPoint(renderer, dstrect.x,        dstrect.y);
        SDL_RenderDrawPoint(renderer, dstrect.x + 1,    dstrect.y);
        SDL_RenderDrawPoint(renderer, dstrect.x - 1,    dstrect.y + 1);
        SDL_RenderDrawPoint(renderer, dstrect.x + 1,    dstrect.y + 1);
        SDL_RenderDrawPoint(renderer, dstrect.x - 1,    dstrect.y + 2);
        SDL_RenderDrawPoint(renderer, dstrect.x,        dstrect.y + 2);
    }
    if (style == GRAPH_POINT_11) {
        dstrect.w = 3;
        dstrect.h = 3;
        SDL_RenderFillRect(renderer, &dstrect);
    }
}

void Graph_Textures_Clear(  Graph           *graph,
                            SDL_Renderer    *renderer) {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_TRANSPARENT);
    SDL_SetRenderTarget(renderer, graph->texture_2x);
    SDL_RenderFillRect(renderer, NULL);
    SDL_SetRenderTarget(renderer, graph->texture_1x);
    SDL_RenderFillRect(renderer, NULL);
}

void Graph_Textures_Create( Graph           *graph,
                            SDL_Renderer    *renderer) {
    /* Create textures if don't exist */
    if (graph->texture_1x == NULL) {
        graph->texture_1x = SDL_CreateTexture(
                                    renderer, SDL_PIXELFORMAT_ARGB8888,
                                    SDL_TEXTUREACCESS_TARGET,
                                    graph_size.x, graph_size.y
                            );
        SDL_assert(graph->texture_1x != NULL);
        SDL_SetTextureBlendMode(graph->texture_1x,
                                SDL_BLENDMODE_BLEND);
    }

    if (graph->texture_2x == NULL) {
        graph->texture_2x = SDL_CreateTexture(
                                    renderer, SDL_PIXELFORMAT_ARGB8888,
                                    SDL_TEXTUREACCESS_TARGET,
                                    graph_size.x * 2, graph_size.y * 2
                            );
        SDL_assert(graph->texture_2x != NULL);
        SDL_SetTextureBlendMode(graph->texture_2x,
                                SDL_BLENDMODE_BLEND);
    }
}

void Graph_Draw(Graph           *graph,
                n9Patch         *n9patch,
                PixelFont       *pixelnours_big,
                SDL_Renderer    *renderer,
                SDL_Texture     *render_target) {
    /* Draw graph, clearing textures everytime */
    SDL_assert(graph                != NULL);
    SDL_assert(n9patch              != NULL);
    SDL_assert(pixelnours_big       != NULL);

    Graph_Textures_Create(  graph, renderer);
    Graph_Textures_Clear(   graph, renderer);

    /* Computing rectangles of X/Y axes spines */
    SDL_Rect spines[TWO_D] = {0};
    Graph_Spines(graph, spines);
    SDL_Rect spine_x = spines[DIM_X];
    SDL_Rect spine_y = spines[DIM_Y];

    /* Draw axes on 1x */
    SDL_SetRenderTarget(renderer, graph->texture_1x);
    _Graph_Draw_Axes(   graph,          spines,
                        n9patch,        pixelnours_big,
                        renderer);
    _Graph_Draw_Max_Level(  graph,          spines,
                            n9patch,        pixelnours_big,
                            renderer);

    /* Copy all 1x elements to 2x textures */
    SDL_SetRenderTarget(renderer, graph->texture_2x);
    SDL_RenderCopy( renderer, graph->texture_1x,
                    NULL, NULL);

    /* Draw points on 2x to fake lowres */
    _Graph_Draw_Stats(  graph,          spines,
                        n9patch,        pixelnours_big,
                        renderer);

    SDL_SetRenderTarget(renderer, render_target);
}
