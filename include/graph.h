#ifndef GRAPH_H
#define GRAPH_H
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
** Graph to show grown stats. Requirements
**  1. Fixed number of levels   [0, SOTA_MAX_LEVEL]
**  2. Fixed number of stats    [0, SOTA_MAX_STAT_PC]
**  3. Minimum distance between each point: 2 points
**      - Ex: YES: (- - - -) NO (----)
**
**  Issues:
**      - Items that increase max level break graph
**
*/
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

/* TODO:
**  1. 2x resolution for more points style/better resolution
**  2. 1 point style for every stat
**  3. 1 point color for every stat
**  Note: excluding move stat */

#define GRAPH_TICK_SIZE(i) (i % 2) == 0 ? GRAPH_TICK_MAJOR_LEN : GRAPH_TICK_MINOR_LEN

enum GRAPH {
    /* Length in pixels of each tick */
    GRAPH_TICK_MAJOR_LEN            =  5,
    GRAPH_TICK_MINOR_LEN            =  3,

    /* Margins for the data i.e. point drawing area.
    **  Make sure points don't touch axes, ticks */
    GRAPH_DATA_MARGIN_TOP       = 2,
    GRAPH_DATA_MARGIN_BOTTOM    = GRAPH_TICK_MAJOR_LEN / 2 + 1,
    GRAPH_DATA_MARGIN_LEFT      = GRAPH_TICK_MAJOR_LEN / 2 + 2,
    GRAPH_DATA_MARGIN_RIGHT     = 1,

    /* Distance in *levels* between each X ticks */
    GRAPH_TICK_X_DIST               =  5,
    /* Distance in *stat* between each Y ticks */
    GRAPH_TICK_Y_DIST               =  5,

    /* Maximum number of *ticks* in stat */
    GRAPH_TICK_X_NUM                =  SOTA_MAX_LEVEL / GRAPH_TICK_X_DIST,
    GRAPH_TICK_Y_NUM                =  SOTA_MAX_STAT_PC / GRAPH_TICK_Y_DIST,

    /* Distance in *pixels* between each point on graph */
    GRAPH_XAXIS_OFFSET              =  1,
    GRAPH_YAXIS_OFFSET              =  1,

    /* Requirement 3: minimum distance between each point */
    GRAPH_POINT_pxDIST              =  2,
    /* Size of point drawing area, including margins */
    GRAPH_DATA_HEIGHT   = GRAPH_POINT_pxDIST * SOTA_MAX_STAT_PC +
                          GRAPH_DATA_MARGIN_BOTTOM +
                          GRAPH_DATA_MARGIN_TOP,
    GRAPH_DATA_WIDTH    = GRAPH_POINT_pxDIST * SOTA_MAX_LEVEL +
                          GRAPH_DATA_MARGIN_LEFT +
                          GRAPH_DATA_MARGIN_RIGHT,

    GRAPH_DEFAULT_LENPERPIXEL_WIDTH  =  2,
    GRAPH_SCALE                      =  2,
    GRAPH_DEFAULT_LENPERPIXEL_HEIGHT =  2,
    GRAPH_LVL_X_OFFSET               = 10,
    GRAPH_LVL_Y_OFFSET               =  0,
    GRAPH_XLABEL_X_OFFSET            = 14,
    GRAPH_XLABEL_Y_OFFSET            =  3,
    GRAPH_YLABEL_X_OFFSET            =  4,
    GRAPH_YLABEL_Y_OFFSET            =  4,
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

/* --- Set/Get --- */
SDL_Texture *Graph_Texture(const Graph *g);
void Graph_Textures_Create(Graph *g, SDL_Renderer *r);
void Graph_Textures_Clear(Graph *g, SDL_Renderer *r);

void Graph_Size_Set(Graph *g, Point size);
void Graph_Free(Graph *g);

Point Graph_Point(  const Graph *g, Point p,
                    SDL_Rect spines[TWO_D]);

/* --- GraphStat --- */
void GraphStat_Cumul(   GraphStat   *gstat,
                        Unit_stats  *base_stats,
                        Unit_stats  *grown_stats,
                        i32 level, i32 base_level);

/* --- Stats --- */
void Graph_Stat_Remove(Graph *g, i32 stat);

void  Graph_Stat_Add(Graph *g, Unit *unit, i32 stat);
void _Graph_Stat_Add(Graph *g, Unit_stats *bs,
                     Unit_stats *gs, i32 level,
                     i32 base_level, i32 stat);

i32     Graph_Level_Num(const Graph *g);
Point   Graph_Pixel_Pos(Graph *g, Point p);

Point Graph_Pixel_Tick_Num(     void);
Point Graph_Pixel_Tick_Dist(    void);
Point Graph_Pixel_Level_Dist(   void);

/* --- Drawing --- */
/* No Graph_Update, because graph is a menu sub-element. */
void Graph_Draw(Graph       *g,     n9Patch         *n9,
                PixelFont   *pb,    SDL_Renderer    *r,
                SDL_Texture *rt);

void _Graph_Draw_Point( Graph *g, Point pos, i32 style,
                        n9Patch *n9patch, PixelFont *pb,
                        SDL_Renderer *r);

void _Graph_Draw_Axes(  Graph *g, SDL_Rect spines[TWO_D],
                        n9Patch *n9patch, PixelFont *pb,
                        SDL_Renderer *r);
void _Graph_Draw_Stat(  Graph *g, SDL_Rect spines[TWO_D],
                        i32 stat, n9Patch *n9patch,
                        PixelFont *pb, SDL_Renderer *r);
void _Graph_Draw_Stats( Graph *g, SDL_Rect spines[TWO_D],
                        n9Patch *n9patch, PixelFont *pb,
                        SDL_Renderer *r);
void _Graph_Draw_Level( Graph *g, SDL_Rect spines[TWO_D],
                        n9Patch *n9patch, PixelFont *pb,
                        SDL_Renderer *r);

void _Graph_Draw_Axes_Shadows(  Graph *g,
                                SDL_Rect spines[TWO_D],
                                n9Patch *n9patch,
                                PixelFont *pb,
                                SDL_Renderer *r);

void _Graph_Draw_Spines(Graph *g, SDL_Rect spines[TWO_D],
                        n9Patch *n9patch, PixelFont *pb,
                        SDL_Renderer *r);
void _Graph_Draw_Ticks( Graph *g, SDL_Rect spines[TWO_D],
                        n9Patch *n9patch, PixelFont *pb,
                        SDL_Renderer *r);

void _Graph_Draw_Labels(Graph *g, SDL_Rect spines[TWO_D],
                        n9Patch *n9patch, PixelFont *pb,
                        SDL_Renderer *r);


void Graph_Spines(Graph *g, SDL_Rect spines[TWO_D]);

#endif /* GRAPH_H */
