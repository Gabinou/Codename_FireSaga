
#include "graph.h"
#include "filesystem.h"
#include "unit/unit.h"
#include "unit/equipment.h"
#include "unit/loadout.h"
#include "unit/stats.h"
#include "palette.h"
#include "pixelfonts.h"
#include "stb_sprintf.h"

/* --- STATIC FUNCTIONS DECLARATIONS --- */
// static void _Graph_Draw_Axes(struct Graph *g, struct n9Patch *n9patch,
//                              struct PixelFont *pb, SDL_Renderer *r, SDL_Texture *rt);
// static void _Graph_Draw_Stat(struct Graph *g, u8 stat, struct n9Patch *n9patch,
//                              struct PixelFont *pb, SDL_Renderer *r, SDL_Texture *rt);


// TODO: move test grown stat stuff to render tests
struct Unit_stats test_grown_stats[10] = {
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

/*                                  hp str mag agi dex fth luck def res con move prof */
struct Unit_stats test_base_stats = {05, 06, 07, 8, 9, 10, 11, 12, 13, 01, 02, 03};

const struct GraphStat GraphStat_default = {0};

const struct Graph Graph_default = {
    .y_lenperpixel = GRAPH_DEFAULT_LENPERPIXEL_HEIGHT, /* 2 or 1 */
    .plot_max = {GRAPH_DATA_WIDTH / GRAPH_DEFAULT_LENPERPIXEL_WIDTH, SOTA_MAX_STAT / GRAPH_DEFAULT_LENPERPIXEL_HEIGHT}, // [XY units]

    .linestyle  = GRAPH_LINESTYLE_POINT,
    .x_ticks    = true,
    .y_ticks    = true,
};

void Graph_Stat_Remove(struct Graph *graph, u8 stat_id) {
    graph->graph_stats[stat_id].stat_id = STAT_ID_NULL;
}

void Graph_Stat_Add(struct Graph *graph, struct Unit_stats *base_stats,
                    struct Unit_stats *grown_stats, i8 level, i8 base_level, u8 stat_id) {
    i32 *stat_arr, *base_arr, total_grown = 0;

    struct GraphStat graph_stat = GraphStat_default;
    graph_stat.level = level;
    graph_stat.base_level = base_level;
    graph_stat.stat_id = stat_id;

    /* Compute cumul_stat using unit grown_stat */
    for (u8 lvl = 0; lvl <= (level - base_level); lvl++) {
        /* No growth yet at base level */
        if (lvl > 0) {
            stat_arr = Unit_stats_arr(grown_stats);
            /* Use stat_id as offset in stat_arr */
            total_grown += *(stat_arr + stat_id);
        }
        base_arr = Unit_stats_arr(base_stats);
        graph_stat.cumul_stat[lvl] = *(base_arr + stat_id) + total_grown;
    }
    graph->graph_stats[stat_id] = graph_stat;
}

void Graph_Draw(struct Graph *graph, struct n9Patch *n9patch, struct PixelFont *pixelnours_big,
                SDL_Renderer *renderer, SDL_Texture *render_target) {
    SDL_assert(graph != NULL);
    SDL_assert(graph->plot_min.x != graph->plot_max.x);
    SDL_assert(graph->plot_min.y != graph->plot_max.y);
    SDL_assert(n9patch != NULL);
    SDL_assert(pixelnours_big != NULL);

    /* Create texture if it doesn't exist */
    if (graph->texture == NULL) {
        graph->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_TARGET, graph->rect.w, graph->rect.h);
        SDL_assert(graph->texture != NULL);
        SDL_SetTextureBlendMode(graph->texture, SDL_BLENDMODE_BLEND);
    }
    SDL_SetRenderTarget(renderer, graph->texture);
    _Graph_Draw_Axes(graph, n9patch, pixelnours_big, renderer, render_target);
    for (int i = UNIT_STAT_NULL + 1; i <= UNIT_STAT_NUM; i++) {
        if (graph->graph_stats[i].stat_id > STAT_ID_NULL)
            _Graph_Draw_Stat(graph, i, n9patch, pixelnours_big, renderer, render_target);
    }
    SDL_SetRenderTarget(renderer, render_target);
}

void _Graph_Draw_Axes(struct Graph *graph, struct n9Patch *n9patch,
                      struct PixelFont *pixelnours_big,
                      SDL_Renderer *renderer, SDL_Texture *render_target) {
    /* -- Clear graph -- */
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_TRANSPARENT);
    SDL_RenderFillRect(renderer, NULL);

    /* -- Draw graph -- */
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    char numbuff[8];
    SDL_Rect yrect = {graph->margin_left, graph->header, 1, graph->rect.h - graph->header - graph->footer};
    SDL_Rect xrect = {yrect.x, graph->rect.h - graph->footer, graph->rect.w - graph->margin_left - graph->margin_right - 10, 1};
    i32 data_width = GRAPH_DATA_WIDTH;
    i32 data_height = GRAPH_DATA_HEIGHT;
    SDL_Rect rects[TWO_D] = {xrect, yrect};
    i32 y_lvl_dist; /* distances in levels to next y tick */
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
    if (graph->x_ticks) {
        i32 tick_num = (graph->plot_max.x - graph->plot_min.x) / GRAPH_TICK_LABELS_DIVISOR;
        i32 x_dist = data_width / GRAPH_TICK_LABELS_DIVISOR / 2;
        SDL_Rect tick = {0, 0, 1, 1};
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        for (u8 i = 1; i < tick_num - 1; i++) {
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
    if (graph->y_ticks) {
        i32 tick_num = (graph->plot_max.y - graph->plot_min.y) / y_lvl_dist;
        i32 y_dist = data_height / tick_num;
        SDL_Rect tick = {0, 0, 1, 1};
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        for (u8 i = 1; i < tick_num + 2; i++) {
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
    if (graph->x_ticks) {
        i32 tick_num = (graph->plot_max.x - graph->plot_min.x) / GRAPH_TICK_LABELS_DIVISOR;
        i32 x_dist = data_width / GRAPH_TICK_LABELS_DIVISOR / 2;
        SDL_Rect tick = {0, 0, 1, 1};
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
        for (u8 i = 1; i < tick_num - 1; i++) {
            tick.h = (i % 2) == 0 ? GRAPH_TICK_MAJOR_LEN : GRAPH_TICK_MINOR_LEN;
            tick.x = xrect.x + i * x_dist + GRAPH_XAXIS_OFFSET;
            tick.y = xrect.y - tick.h / 2;
            SDL_RenderFillRect(renderer, &tick);
        }
    }
    if (graph->y_ticks) {
        i32 tick_num = (graph->plot_max.y - graph->plot_min.y) / y_lvl_dist;
        i32 y_dist = data_height / tick_num;
        SDL_Rect tick = {0, 0, 1, 1};
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
        for (u8 i = 1; i < tick_num + 2; i++) {
            tick.w = GRAPH_TICK_MAJOR_LEN;
            tick.x = xrect.x - tick.w / 2;
            tick.y = xrect.y - i * y_dist - GRAPH_YAXIS_OFFSET;
            SDL_RenderFillRect(renderer, &tick);
        }
    }
    SDL_Rect label = {0, 0, 1, 1};
    i32 x_tick_num = (graph->plot_max.x - graph->plot_min.x) / GRAPH_TICK_LABELS_DIVISOR;
    i32 x_dist = data_width / GRAPH_TICK_LABELS_DIVISOR;
    for (u8 i = 1; i < x_tick_num; i++) {
        label.h = (i % 2) == 0 ? GRAPH_TICK_MAJOR_LEN : GRAPH_TICK_MINOR_LEN;
        label.x = xrect.x + i * x_dist + GRAPH_XAXIS_OFFSET - GRAPH_YLABEL_X_OFFSET;
        label.y = xrect.y + GRAPH_YLABEL_Y_OFFSET;
        stbsp_sprintf(numbuff, "%02d\0\0\0\0", i * x_tick_num);
        PixelFont_Write(pixelnours_big, renderer, numbuff, strlen(numbuff), label.x, label.y);
    }
    i32 y_tick_num = (graph->plot_max.y - graph->plot_min.y) / y_lvl_dist;
    i32 y_dist = data_height / y_tick_num;
    for (u8 i = 1; i < y_tick_num + 2; i++) {
        label.w = (i % 2) == 0 ? GRAPH_TICK_MAJOR_LEN : GRAPH_TICK_MINOR_LEN;
        label.x = xrect.x - GRAPH_XLABEL_X_OFFSET;
        label.y = xrect.y - i * y_dist - GRAPH_YAXIS_OFFSET - GRAPH_XLABEL_Y_OFFSET;
        stbsp_sprintf(numbuff, "%02d\0\0\0\0", i * y_lvl_dist);
        PixelFont_Write(pixelnours_big, renderer, numbuff, strlen(numbuff), label.x, label.y);
    }
}

void _Graph_Draw_Stat(struct Graph *graph, u8 stat_id, struct n9Patch *n9patch,
                      struct PixelFont *pixelnours_big,
                      SDL_Renderer *renderer, SDL_Texture *render_target) {
    /* -- Preliminaries -- */
    struct GraphStat graph_stat = graph->graph_stats[stat_id];
    SDL_Rect axes = {
        .x = graph->margin_left + GRAPH_XAXIS_OFFSET,
        .y = graph->rect.h - graph->footer + GRAPH_YAXIS_OFFSET - 2,
        .w = GRAPH_DATA_WIDTH,
        .h = GRAPH_DATA_HEIGHT,
    };
    SDL_Rect point = {0, 0, 1, 1};

    /* -- Drawing level -- */
    SDL_SetRenderDrawColor(renderer, 0xB2, 0x10, 0x30, SDL_ALPHA_OPAQUE);
    SDL_Rect level = {
        .x = axes.x + graph_stat.level * axes.w / (graph->plot_max.x - graph->plot_min.x),
        .y = GRAPH_LVL_Y_OFFSET + PIXELFONT_HEIGHT,
        .w = 1,
        .h = graph->rect.h - graph->header - PIXELFONT_HEIGHT - GRAPH_TICK_MINOR_LEN + 2,
    };
    SDL_RenderFillRect(renderer, &level);
    char numbuff[8];
    stbsp_sprintf(numbuff, "%02d\0\0\0\0", graph_stat.level);
    int height = level.y - PIXELFONT_HEIGHT;
    PixelFont_Write(pixelnours_big, renderer, "Lv", 2, level.x - GRAPH_LVL_X_OFFSET, height);
    PixelFont_Write(pixelnours_big, renderer, numbuff, strlen(numbuff), level.x + 2, height);

    /* -- Drawing stats -- */
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
    for (u8 i = 0; i <= (graph_stat.level - graph_stat.base_level); i++) {
        point.x = axes.x + (i + graph_stat.base_level) * axes.w / (graph->plot_max.x - graph->plot_min.x);
        point.y = axes.y - graph->y_lenperpixel * graph_stat.cumul_stat[i];
        SDL_RenderFillRect(renderer, &point);
    }
}
