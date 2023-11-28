#ifndef ARROW_H
#define ARROW_H

#include "SDL.h"
#include "nmath.h"
#include "pathfinding.h"
#include "enums.h"
#include "debug.h"
#include "types.h"
#include "structs.h"
#include "utilities.h"
#include "filesystem.h"

/* NOTE: Arrow breaks if INFINITE_MOVE_ALL is defined. DON'T CARE. */

enum ARROW_17PATCHES {
    /* lots of patches cause arrows cast shadows that cannot be rotated */
    /* screen "UP" is reversed */
    ARROW_NULL                =   0,
    ARROW_START_UP            =   1,
    ARROW_START_HORIZONTAL    =   2,
    ARROW_START_DOWN          =   3,
    ARROW_STRAIGHT_HORIZONTAL =   4,
    ARROW_STRAIGHT_VERTICAL   =   5,
    ARROW_STOP_UP             =   6,
    ARROW_STOP_HORIZONTAL     =   7,
    ARROW_STOP_DOWN           =   8,
    ARROW_CORNER_DOWN         =   9,
    ARROW_CORNER_UP           =  10,
    ARROW_STOP_CORNER_RDOWN   =  11,
    ARROW_STOP_CORNER_RUP     =  12,
    ARROW_STOP_CORNER_URIGHT  =  13,
    ARROW_STOP_CORNER_DRIGHT  =  14,
    ARROW_STARTEND_HORIZONTAL =  15,
    ARROW_STARTEND_UP         =  16,
    ARROW_STARTEND_DOWN       =  17,
    ARROW_NUM                 =  18,
    ARROW_TILESIZE            =  32,
    ARROW_MAXLEN              = 128,
};

typedef struct Arrow {
    i32 *traversemap;
    i32 *movemap;
    i32 move;
    i32 col_len;
    i32 row_len;
    i32 *pathlist;  /* from initial unit position to current cursor position */
    i32 tilesize[TWO_D];
    i32 map_tilesize[TWO_D];
    struct Point start;
    struct SDL_Texture *textures;  /* [patch_id] */
    struct Rendered rendereds[SOTA_MAX_MOVEMENT]; /* [patch_id] */
    bool show: 1;
} Arrow;
extern struct Arrow Arrow_default;

/* --- PUBLIC --- */
/* -- ructors/Deructors -- */
struct Arrow *Arrow_Init(i32 tilesize[TWO_D]);
void          Arrow_Free(struct Arrow *a);

/* -- Path -- */
void Arrow_Path_Add( struct Arrow *a, i32  x, i32 y);
void Arrow_Path_Init(struct Arrow *a, i32 *c, i32 m, struct Point s);

/* -- I/O -- */
void Arrow_Textures_Load(struct Arrow *a,  char *f, SDL_Renderer *r);

/* -- Draw -- */
void Arrow_Draw(struct Arrow *a, SDL_Renderer *r, struct Camera *c);

/* --- PRIVATE --- */
/* -- Path -- */
static void _Arrow_Path_Trace(struct Arrow *a, struct Point end);

/* -- Decider -- */
static void _Arrow_Decider(struct Arrow *a, i32 point);

static struct Rendered _Arrow_Decider_End(     i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2);
static struct Rendered _Arrow_Decider_Start(   i32 x0, i32 y0, i32 x1, i32 y1);
static struct Rendered _Arrow_Decider_Middle(  i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2);
static struct Rendered _Arrow_Decider_Startend(i32 x0, i32 y0, i32 x1, i32 y1);

#endif /* ARROW_H */