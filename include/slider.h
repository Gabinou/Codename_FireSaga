#ifndef SLIDER_H
#define SLIDER_H

#include "enums.h"
#include "types.h"
#include "structs.h"

/* --- SLIDER --- */
// Makes things slide (animation)
// MAJOR ISSUE: ALL SLIDETYPES SPEED DEPEND ON FRAMERATE
// Solution:
//  - Slider needs FPS to compute px velocity
//     - Lag spike: same px move on frame
//     - 60 fps, 20px/s: 3 px/f
//     - res : 1600 x 800 px^2
//     - 9000 px/s pretty fast
//     - Fast forward: No need to change anything. same px/f called way many more times.
//  - Get rid of linear slidetype
//  - Define Slider velocity bounds
//  - Introduce velocity slidetype (px/s)
//  - Update geometric type
//    - Velocity propto distance to target


/* --- FORWARD DECLARATIONS --- */
struct Settings;

enum SLIDER {
    SLIDER_MIN_DIST = 3, /* Above this distance, don't slide */
};
// XP -> X positive
#define SLIDER_PERIODIC_XN_LIMIT 0.2f
#define SLIDER_PERIODIC_YN_LIMIT 0.2f
#define SLIDER_PERIODIC_XP 0.6f
#define SLIDER_PERIODIC_XN 1.5f
#define SLIDER_PERIODIC_YP 0.8f
#define SLIDER_PERIODIC_YN 1.5f

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
    /* Is SliderOffscreen currently going offscreen */
    b32   go_offscreen;
} SliderOffscreen;
extern const struct SliderOffscreen SliderOffscreen_default;

/* --- Setters and Getters --- */
float* Slider_Rate(Slider *s);
i32 Slider_Slide_Num(Slider *s);
void Slider_Rate_Set(Slider *s, float rate0, float rate1);
void Slider_Slide_Num_Set(Slider *s, i32 num);

/* --- Slider --- */
void Slider_Start(           struct Slider *s, struct Point *p, struct Point *t);
void Slider_Compute_Next(    struct Slider *s, struct Point *p, struct Point *t, b32 g);

/* --- SliderOffscreen --- */
void Slider_Target_Offscreen(struct Slider *s, struct SliderOffscreen *o, struct Point *p);
void SliderOffscreen_Compute_Next(struct Slider *s, struct SliderOffscreen *o,
                                  struct Point *p);

#endif /* SLIDER_H */
