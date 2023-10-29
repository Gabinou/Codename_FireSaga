#ifndef HOVER_H
#define HOVER_H

#include "enums.h"
#include "globals.h"
#include "macros.h"
#include "debug.h"
#include "structs.h"
#include "types.h"
#include "nmath.h"
#include "SDL.h"

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
    i32  width;
    i32  step;            /* every step, move step [pixels] < width */
    u32  current;         /* every step, move step [pixels] < width */
    i8   wait_factor;
    i32  update_wait_ns;
    i32  timer_ns;
} Hover;
extern struct Hover Hover_default;

extern void Hover_Compute_Next(struct Hover *hover, struct Point *pos);

#endif /* HOVER_H */
