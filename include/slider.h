#ifndef SLIDER_H
#define SLIDER_H

#include "enums.h"
#include "types.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Settings;

enum SLIDER {
    SLIDER_MIN_DIST = 3, /* Above this distance, don't slide */
};

/* --- COMPONENTS --- */
typedef struct Slider {
    /* Ultimate target of Slider movmement */
    struct Point target;
    /* point meaning depends on SLIDETYPE:                              */
    /* SLIDETYPE_LINEAR: distance, SLIDETYPE_EASYINEASYOUT: midpoint    */
    struct Point point;

    // Factor that divides distance between target and end -> next point.
    float   slidefactors[TWO_D]; /* for SLIDETYPE_GEOMETRIC */
    // Number of points slide will take
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

/* --- Slider --- */
void Slider_Start(           struct Slider *s, struct Point *p, struct Point *t);
void Slider_Compute_Next(    struct Slider *s, struct Point *p, struct Point *t, b32 g);

/* --- SliderOffscreen --- */
void Slider_Target_Offscreen(struct Slider *s, struct SliderOffscreen *o, struct Point *p);
void SliderOffscreen_Compute_Next(struct Slider *s, struct SliderOffscreen *o,
                                  struct Point *p);

#endif /* SLIDER_H */
