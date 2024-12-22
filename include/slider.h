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
//  - Define Slider velocity bounds
//  - Introduce velocity slidetype (px/s)
//  - Update geometric type
//    - Velocity propto distance to target

/* --- FORWARD DECLARATIONS --- */
struct Settings;

enum SLIDER {
    /* Above this distance, don't slide */
    SLIDER_MIN_DIST         = 3,
    // By default, ratio makes it geometric at 60FPS:
    // move half distance every frame
    SLIDER_DEFAULT_RATIO    = FPS_DEFAULT_CAP / 2, 
};
// XP -> X positive
#define SLIDER_PERIODIC_XN_LIMIT 0.2f
#define SLIDER_PERIODIC_YN_LIMIT 0.2f
#define SLIDER_PERIODIC_XP 0.6f
#define SLIDER_PERIODIC_XN 1.5f
#define SLIDER_PERIODIC_YP 0.8f
#define SLIDER_PERIODIC_YN 1.5f

union Slider_uFactors {
    // Velocity in pixel per second for each axis
    // Ex: Res is 1600 x 800 px^2
    i32   velocity[TWO_D]; /* [px/s] SLIDETYPE_VELOCITY */

    // ratio = fps / rate  
    //  - with rate the Geometric per frame rate 
    // - e.g. if you want the slider to move half distance 
    //   EVERY FRAME, ratio should be half of fps    
    // - To keep same speed at different FPS: same ratio
    i32   ratio[TWO_D]; /* [spf] SLIDETYPE_GEOMETRIC */
};

/* --- COMPONENTS --- */
typedef struct Slider {
    u8      slidetype;
    /* Ultimate target of Slider movement */
    struct Point target;
    struct Point midpoint; /* SLIDETYPE_EASYINEASYOUT */
    union Slider_uFactors ufactors;

    i32     fps;
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
void Slider_Rate_Set(Slider *s, float rate0, float rate1);

/* --- Slider --- */
void Slider_Start(           struct Slider *s, struct Point *p, struct Point *t);
void Slider_Compute_Next(    struct Slider *s, struct Point *p, struct Point *t, b32 g);

/* --- SliderOffscreen --- */
void Slider_Target_Offscreen(struct Slider *s, struct SliderOffscreen *o, struct Point *p);
void SliderOffscreen_Compute_Next(struct Slider *s, struct SliderOffscreen *o,
                                  struct Point *p);

#endif /* SLIDER_H */
