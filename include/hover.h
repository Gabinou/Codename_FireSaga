#ifndef HOVER_H
#define HOVER_H

#include "enums.h"
#include "globals.h"
#include "macros.h"
#include "debug.h"
#include "structs.h"
#include "types.h"
#include "nmath.h"
#include "SDL2/SDL.h"

/* NOTE: HOVER BREAKS SLIDER */

#define HOVER_WAIT 100e6 /* [ns] */
enum SOTA_HOVER {
    SOTA_HOVER_NULL          = -1,
    SOTA_HOVER_WIDTH_DEFAULT =  6,
    SOTA_HOVER_STEP_DEFAULT  =  3,
    SOTA_HOVER_WAIT_FACTOR   =  2, /* between 2 and 4 is good */
};

/* --- FORWARD DECLARATIONS --- */
typedef struct Hover {
    if32  width;
    if32  step;            /* every step, move step [pixels] < width */
    uf32  current;         /* every step, move step [pixels] < width */
    if8   wait_factor;
    if32  update_wait_ns;
    if32  timer_ns;
} Hover;
extern struct Hover Hover_default;

extern void Hover_Compute_Next(struct Hover *hover, struct Point *pos);

#endif /* HOVER_H */
