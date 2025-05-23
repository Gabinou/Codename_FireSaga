#ifndef ARROW_H
#define ARROW_H

#include <math.h>
#include "types.h"
#include "enums.h"
#include "debug.h"
#include "structs.h"
#include "SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct Map_Size;

typedef struct Arrow {
    i32 *costmap;
    i32 *path;  /* start pos -> cursor pos */

    /* Note: texture contains all arrow 17 patches */
    struct SDL_Texture *texture;
    struct Rendered rendereds[SOTA_MAX_MOVEMENT]; /* [patch_id] */
    i32 move;
    b32 show;
} Arrow;
extern const struct Arrow Arrow_default;

/* NOTE: Arrow breaks if INFINITE_MOVE_ALL is defined. DON'T CARE. */

enum ARROW_17PATCHES {
    /* Arrows cast shadows that cannot be rotated */
    /* Screen "UP" is reversed */
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

/* --- PUBLIC --- */
/* -- Constructors/Destructors -- */
Arrow   *Arrow_Init();
void     Arrow_Free(struct Arrow *a);

/* -- Path -- */
void Arrow_Path_Add(struct Arrow    *a,
                    struct Map_Size  s,
                    i32  x, i32 y);
void Arrow_Path_Init(struct Arrow *a, i32 *c, i32 m, struct Point s);

/* -- I/O -- */
void Arrow_Textures_Load(struct Arrow *a,  char *f, SDL_Renderer *r);

/* -- Draw -- */
void Arrow_Draw(struct Arrow    *arrow,
                struct Map_Size  size,
                struct Camera   *camera,
                SDL_Renderer    *renderer);

#endif /* ARROW_H */
