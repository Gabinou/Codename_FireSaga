#ifndef SLIDER_H
#define SLIDER_H

#include "enums.h"
#include "types.h"
#include "structs.h"

/* --- SLIDER --- */
// Makes things slide (animation)
// MAJOR ISSUE: ALL SLIDETYPES SPEED DEPEND ON FRAMERATE
// Solution:
//  - Slider needs target FPS to compute px velocity
//     - 60 fps, 20px/s: 3 px/f
//     - res : 1600 x 800 px^2
//     - 9000 px/s pretty fast
//     - Fast forward: No need to change anything. same px/f called way many more times.
//  - Define Slider velocity bounds
//  - Introduce velocity slidetype (px/s)
//  - Update geometric type
//    - Velocity propto distance to target

// ISSUE: Lag spike: same px move on frame
// Rejected: measure time
//  - I'm not including a timer in a Slider
// Solution:
//  - Get current_FPS
//  - SLOW: (current_fps < target_fps)
//      - Game is lagging. Sprite speed every frame UP.
//      - Increase speed by: current_fps / target_fps
//      - Up to a max of 10?
//  - FAST: (current_fps > target_fps), use target_fps
//      - Two possible states:
//      1. Game is capped at target_fps.
//      2. Game is fast-forwarding -> sprites go fast

/* --- FORWARD DECLARATIONS --- */
struct Settings;

enum SLIDER {
    /* Above this distance squared, snap to target */
    SLIDER_MIN_DISTSQ         = 4,
    // Default ratio makes it geometric at 60FPS:
    // move half distance every frame
    SLIDER_DEFAULT_RATIO    = FPS_DEFAULT_CAP / 2,
};

// XP -> X positive
#define SLIDER_PERIODIC_XN_LIMIT    0.2f
#define SLIDER_PERIODIC_YN_LIMIT    0.2f
#define SLIDER_PERIODIC_XP          0.2f
#define SLIDER_PERIODIC_XN          1.1f
#define SLIDER_PERIODIC_YP          0.8f
#define SLIDER_PERIODIC_YN          1.5f
#define SLIDER_MAX_LAG_FACTOR      10.0f

union Slider_uFactors {
    // Speed in px/s for each axis
    // Note: speed is scalar, abs(velocity) vector
    // Ex:  Screen resolution is 1600 x 800 px^2.
    //      Fast is way more than 1 screen per second.
    i32   speed[TWO_D]; /* [px/s] SLIDETYPE_VELOCITY */

    // ratio = fps / rate -> 30 = 60 / 2
    //  - with rate the Geometric per frame rate
    // - e.g. if you want the slider to move half distance
    //   EVERY FRAME, ratio should be half of fps
    // - To keep same speed at different FPS: same ratio
    i32   ratio[TWO_D]; /* [spf] SLIDETYPE_GEOMETRIC */
};

/* --- COMPONENTS --- */
typedef struct Slider {
    /* Ultimate target of Slider movement */
    struct Point target;

    /* SLIDETYPE_EASYINEASYOUT only */
    struct Point start;
    struct Point midpoint;

    /* Slidetype exclusive factors */
    union Slider_uFactors ufactors;

    i32     active;
    i32     slidetype;
    f32     fps_target;
} Slider;
extern const struct Slider Slider_default;

typedef struct SliderOffscreen {
    struct Settings             *settings;
    // Point for periodic movement
    struct Point target;
    struct Point midpoint;

    // If true, slider goes to target in reverse, offscreen
    // and teleports to other screen side (periodic bounds).
    // If false, slider goes from pos to target directly.
    b32   reverse;
} SliderOffscreen;
extern const struct SliderOffscreen SliderOffscreen_default;

typedef struct SliderInput {
    // Input to Slider functions
    Slider              *slider;
    SliderOffscreen     *offscreen;
    Point               *pos;
    Point   target;
    Point   midpoint;
    b32     reverse;
    f32     fps_instant;
} SliderInput;
extern const struct SliderInput SliderInput_default;


/* --- Setters and Getters --- */
i32* Slider_Ratio(Slider *s);
i32* Slider_Speed(Slider *s);
Point Slider_Sign(Point p, b32 r);

void Slider_Ratio_Set(Slider *s, i32 rx, i32 ry);
void Slider_Speed_Set(Slider *s, i32 vx, i32 vy);

/* --- Slider --- */
void Slider_Init(struct Slider *s, struct Point *p, struct Point *t);
void Slider_Init_tnecs(void *voidslider);

f32 Slider_FPS_Effective(Slider *slider,
                         f32 fps_instant);

/* --- Slide: position at next frame --- */
typedef struct SliderSlideInput {
    Point   sign;
    Point   dist;
    Point   target;
    Point   pos;
    f32     fps_eff;
} SliderSlideInput;

typedef Point (*slide_f)(Slider   *, SliderSlideInput);
extern const slide_f slider_slides[SLIDETYPE_END];

Point Slide_VELOCITY(       Slider   *slider,
                            SliderSlideInput input);
Point Slide_GEOMETRIC(      Slider   *slider,
                            SliderSlideInput input);
Point Slide_EASYINEASYOUT(  Slider   *slider,
                            SliderSlideInput input);

void Slider_Compute_Next(           SliderInput input);
void SliderOffscreen_Compute_Next(  SliderInput input);

/* --- SliderOffscreen --- */
void Slider_Offscreen_Midpoint(struct Slider *s, struct SliderOffscreen *o);

// Just offscreen
void Slider_Target_Offscreen(struct Slider *s, struct SliderOffscreen *o, SDL_Rect *p);

// Far offscreen to make sliding look better?
void Slider_Target_Offscreen_Far(struct Slider *s, struct SliderOffscreen *o, SDL_Rect *p);

#endif /* SLIDER_H */
