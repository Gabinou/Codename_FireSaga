#ifndef GRAPH_H
#define GRAPH_H

#include "enums.h"
#include "types.h"
#include "debug.h"
#include "n9patch.h"
#include "structs.h"
#include "pixelfonts.h"

/* --- FORWARD DECLARATIONS --- */
struct Item;
struct Unit;
struct Game;

#define GRAPH_POS(xory, offset) (offset * n9patch->scale.xory)

/* TODO:
**  1. 2x resolution for more points style/better resolution
**  2. 1 point style for every stat
**  3. 1 point color for every stat
**  Note: excluding move stat */

enum GRAPH {
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

enum GRAPH_COLORS {
    GRAPH_COLOR1 = 30, /* Bright orange */
    GRAPH_COLOR2 = 14, /* Bright green */
    GRAPH_COLOR3 = 55, /* White */
    GRAPH_COLOR4 = 52, /* Dark Grey */
    GRAPH_COLOR5 =  1, /* Black */
    GRAPH_COLOR6 = 38, /* Pink */
};

/* Todo: Implement the points. */
enum GRAPH_POINTS {
    GRAPH_POINT_NULL    = 0,
    GRAPH_POINT_8PX_1, /* Many pixels */
    GRAPH_POINT_8PX_2,
    GRAPH_POINT_8PX_3,
    GRAPH_POINT_8PX_4,
    GRAPH_POINT_8PX_5,
    GRAPH_POINT_8PX_6, /* Few pixels */
    GRAPH_POINT_4PX_1, /* Many pixels */
    GRAPH_POINT_4PX_2,
    GRAPH_POINT_4PX_3,
    GRAPH_POINT_4PX_4,
    GRAPH_POINT_4PX_5, /* Few pixels */
};

/* TODO: get rid of this */
extern struct Unit_stats test_grown_stats[10];
extern struct Unit_stats test_base_stats;

/* --- GraphStat --- */
void GraphStat_Cumul(   GraphStat   *gstat,
                        Unit_stats  *base_stats,
                        Unit_stats  *grown_stats,
                        i32 level, i32 base_level);

/* --- Stats --- */
void Graph_Stat_Remove(Graph *g, i32 stat);

void  Graph_Stat_Add(Graph *g, Unit *unit, i32 stat);
void _Graph_Stat_Add(Graph *g, Unit_stats *bs, Unit_stats *gs,
                     i32 level, i32 base_level, i32 stat);

/* --- Drawing --- */
void Graph_Draw(Graph *g, n9Patch *n9, PixelFont *pb,
                SDL_Renderer *r, SDL_Texture *rt);
/* TODO: _Graph_Draw_Point */
void _Graph_Draw_Point( Graph *g, Point pos, i32 style);

void _Graph_Draw_Axes(  Graph *g, n9Patch *n9patch,
                        PixelFont *pb, SDL_Renderer *r,
                        SDL_Texture *rt);
void _Graph_Draw_Stat(  Graph *g, i32 stat,
                        n9Patch *n9patch, PixelFont *pb,
                        SDL_Renderer *r, SDL_Texture *rt);
void _Graph_Draw_Stats( Graph *g,
                        n9Patch *n9patch, PixelFont *pb,
                        SDL_Renderer *r, SDL_Texture *rt);
void _Graph_Draw_Lvl(   Graph *g,
                        n9Patch *n9patch, PixelFont *pb,
                        SDL_Renderer *r, SDL_Texture *rt);

#endif /* GRAPH_H */
