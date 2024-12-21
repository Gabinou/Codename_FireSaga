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

union Slider_uPoint {
    struct Point distance; /* SLIDETYPE_LINEAR */
    struct Point midpoint; /* SLIDETYPE_EASYINEASYOUT */
};

union Slider_uFactors {
    // Divides distance between target and end -> next point.
    float   rate[TWO_D]; /* SLIDETYPE_GEOMETRIC */
    // Number of points slide will take
    i32     slide_num;           /* SLIDETYPE_LINEAR, SLIDETYPE_GEOMETRIC */
};

/* --- COMPONENTS --- */
typedef struct Slider {
    u8      slidetype;
    /* Ultimate target of Slider movement */
    struct Point target;
    union Slider_uPoint upoint;
    union Slider_uFactors ufactors;

    i32     update_wait_ns;
    i32     timer_ns;
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

float* Slider_Rate(Slider *s);
i32 Slider_Slide_Num(Slider *s);
void Slider_Rate_Set(Slider *s, float rate0, float rate1);
void Slider_Slide_Num_Set(Slider *s, i32 num);

/* --- SliderOffscreen --- */
void Slider_Target_Offscreen(struct Slider *s, struct SliderOffscreen *o, struct Point *p);
void SliderOffscreen_Compute_Next(struct Slider *s, struct SliderOffscreen *o,
                                  struct Point *p);

#endif /* SLIDER_H */
