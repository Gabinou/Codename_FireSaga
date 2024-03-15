#ifndef GRAPH_H
#define GRAPH_H

#include "debug.h"
#include "tnecs.h"
#include "nmath.h"
#include "n9patch.h"
#include "pixelfonts.h"
#include "stb_sprintf.h"
#include "SDL2/SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Unit;
struct Game;

#define GRAPH_POS(xory, offset) (offset * n9patch->scale.xory)

enum GRAPH { /* palette_NES, offset by 1 for NULL == 0 */
    GRAPH_MAX_PLOT_STATS             =  6,
    GRAPH_TICK_LABELS_DIVISOR        =  4,
    GRAPH_AXIS_TICK_DIVISOR          =  5,
    GRAPH_XAXIS_OFFSET               =  1,
    GRAPH_YAXIS_OFFSET               =  1,
    GRAPH_TICK_MAJOR_LEN             =  3,
    GRAPH_TICK_MINOR_LEN             =  5,
    GRAPH_DATA_WIDTH                 = 80,
    GRAPH_DEFAULT_LENPERPIXEL_WIDTH  =  2,
    GRAPH_DATA_HEIGHT                = SOTA_MAX_STAT,
    GRAPH_DEFAULT_LENPERPIXEL_HEIGHT =  2,  /* 2 or 1 */
    GRAPH_LVL_X_OFFSET               = 10,
    GRAPH_LVL_Y_OFFSET               =  0,
    GRAPH_XLABEL_X_OFFSET            = 14,
    GRAPH_XLABEL_Y_OFFSET            =  3,
    GRAPH_YLABEL_X_OFFSET            =  4,
    GRAPH_YLABEL_Y_OFFSET            =  4,
};

enum GRAPH_COLORS { /* palette_NES, offset by 1 for NULL == 0 */
    GRAPH_COLOR1 = 30, // Bright orange
    GRAPH_COLOR2 = 14, // Bright green
    GRAPH_COLOR3 = 55, // White
    GRAPH_COLOR4 = 52, // Dark Grey
    GRAPH_COLOR5 =  1, //  Black
    GRAPH_COLOR6 = 38, // Pink
};

enum GRAPH_LINESTYLE { /* palette_NES, offset by 1 for NULL == 0 */
    GRAPH_LINESTYLE_NONE  = 0,
    GRAPH_LINESTYLE_POINT = 1,
    GRAPH_LINESTYLE_LINE  = 2,
};

struct GraphStat {
    i16 level;
    i16 base_level;
    i8 cumul_stat[SOTA_MAX_LEVEL];
    i8 stat_id;
};
extern struct GraphStat GraphStat_default;

struct Graph {
    SDL_Rect rect; // x,y,w,h
    SDL_Texture *texture;

    struct GraphStat graph_stats[UNIT_STAT_NUM];

    struct Point plot_min; /* [XY units] */
    struct Point plot_max; /* [XY units] */

    i32 header; /* [pixels] */
    i32 footer; /* [pixels] */
    i32 margin_left;  /* [pixels] */
    i32 margin_right; /* [pixels] */

    /* length until writing another pixel, including pixel */
    /* ant for x */
    u8 y_lenperpixel;

    u8 stat_num;
    u8 linestyle;
    bool x_ticks : 1;
    bool y_ticks : 1;
};
extern struct Graph Graph_default;

extern struct Unit_stats test_grown_stats[10];
extern struct Unit_stats test_base_stats;

/* --- Stats --- */
void Graph_Stat_Remove(struct Graph *g, u8 stat);
// TODO: call Graph_Stat_Add when growths menu elem is selected.
void Graph_Stat_Add(struct Graph *g, struct Unit_stats *base_stats,
                    struct Unit_stats *grown_stats,
                    i8 level, i8 base_level, u8 stat);

/* --- Drawing --- */
void Graph_Draw(struct Graph *g, struct n9Patch *n9, struct PixelFont *pb,
                SDL_Renderer *r, SDL_Texture *rt);

void _Graph_Draw_Axes(struct Graph *g, struct n9Patch *n9patch,
                      struct PixelFont *pb, SDL_Renderer *r, SDL_Texture *rt);
void _Graph_Draw_Stat(struct Graph *g, u8 stat, struct n9Patch *n9patch,
                      struct PixelFont *pb, SDL_Renderer *r, SDL_Texture *rt);

#endif /* GRAPH_H */
