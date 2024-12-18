#ifndef SLIDER_H
#define SLIDER_H

#include "enums.h"
#include "types.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Settings;

/* --- COMPONENTS --- */
/* Type-specific members: void * are 8 bytes, so not worth using. */
typedef struct Slider {
    /* point meaning depends on SLIDETYPE:                              */
    /* SLIDETYPE_LINEAR: distance, SLIDETYPE_EASYINEASYOUT: midpoint    */
    struct Point point;
    struct Point target;

    float   slidefactors[TWO_D]; /* for SLIDETYPE_GEOMETRIC */
    i32     slide_num;           /* for SLIDETYPE_LINEAR, SLIDETYPE_GEOMETRIC */
    i32     update_wait_ns;
    i32     timer_ns;
    u8      slidetype;
} Slider;
extern const struct Slider Slider_default;

typedef struct SliderOffscreen {
    struct Settings             *settings;
    struct Point  target;
    b32   go_offscreen;
} SliderOffscreen;
extern const struct SliderOffscreen SliderOffscreen_default;

/* --- FUNCTIONS --- */
void Slider_Start(           struct Slider *s, struct Point *p, struct Point *t);
void Slider_Compute_Next(    struct Slider *s, struct Point *p, struct Point *t, b32 g);
void Slider_Target_Offscreen(struct Slider *s, struct SliderOffscreen *o, struct Point *p);
void SliderOffscreen_Compute_Next(struct Slider *s, struct SliderOffscreen *o,
                                  struct Point *p);

#endif /* SLIDER_H */
