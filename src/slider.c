
#include "slider.h"
#include "nmath.h"
#include "utilities.h"

const struct Slider Slider_default = {
    .slidetype      = SLIDETYPE_GEOMETRIC,
    .ufactors.ratio = {SLIDER_DEFAULT_RATIO, SLIDER_DEFAULT_RATIO},
    .fps_target     = FPS_DEFAULT_CAP,
};
const slide_f slider_slides[SLIDETYPE_END] = {
    Slide_VELOCITY,
    Slide_GEOMETRIC,
    Slide_EASYINEASYOUT
};

const struct SliderOffscreen SliderOffscreen_default    = {0};
const struct SliderInput SliderInput_default            = {0};

void Slider_Init(struct Slider *slider, struct Point *pos,
                 struct Point  *target) {

    slider->start   = *pos;
    slider->target  = *target;

    switch (slider->slidetype) {
        case SLIDETYPE_EASYINEASYOUT:
            slider->midpoint.x = pos->x + (target->x - pos->x) / 2;
            slider->midpoint.y = pos->y + (target->y - pos->y) / 2;
            break;
    }
}

void Slider_Init_tnecs(void *voidslider) {
    struct Slider *slider = voidslider;
    *slider = Slider_default;
}

void Slider_Ratio_Set(Slider *slider, i32 ratiox, i32 ratioy) {
    slider->ufactors.ratio[DIMENSION_X] = ratiox;
    slider->ufactors.ratio[DIMENSION_Y] = ratioy;
}

i32* Slider_Ratio(Slider *slider) {
    return (slider->ufactors.ratio);
}

void Slider_Speed_Set(Slider *slider, i32 velocityx, i32 velocityy) {
    slider->ufactors.speed[DIMENSION_X] = velocityx;
    slider->ufactors.speed[DIMENSION_Y] = velocityy;
}

i32* Slider_Speed(Slider *slider) {
    return (slider->ufactors.speed);
}

void Slider_Offscreen_Midpoint(Slider *slider, SliderOffscreen *offscreen) {
    // Compute midpoint to offscreen target
    offscreen->midpoint.x = slider->start.x + (offscreen->target.x - slider->start.x) / 2;
    offscreen->midpoint.y = slider->start.y + (offscreen->target.y - slider->start.y) / 2;
}

void Slider_Target_Offscreen(struct Slider *slider,
                             struct SliderOffscreen *offscreen,
                             SDL_Rect *rect) {
    // Set target of Slider to offscreen, on the CLOSEST edge to pos
    // Go JUST OUTSIDE of screen

    SDL_assert(rect                 != NULL);
    SDL_assert(slider               != NULL);
    SDL_assert(offscreen            != NULL);
    SDL_assert(offscreen->settings  != NULL);

    struct Point res = offscreen->settings->res;

    // Only consider slider size IF it might poke through
    offscreen->target.x = (rect->x > (res.x / 2)) ? res.x : -rect->w;
    offscreen->target.y = (rect->y > (res.y / 2)) ? res.y : -rect->h;

    offscreen->reverse = true;
}

void Slider_Target_Offscreen_Far(struct Slider *slider,
                                 struct SliderOffscreen *offscreen,
                                 SDL_Rect *rect) {
    // Set target of Slider to offscreen, on the CLOSEST edge to pos
    // Go FAR -> a fraction of the screen resolution

    SDL_assert(rect                 != NULL);
    SDL_assert(slider               != NULL);
    SDL_assert(offscreen            != NULL);
    SDL_assert(offscreen->settings  != NULL);

    struct Point res = offscreen->settings->res;

    // Only consider slider size IF it might poke through
    offscreen->target.x = (rect->x > (res.x / 2)) ? (rect->x + res.x / 3) :
                          (rect->x - res.x / 3 - rect->w);
    offscreen->target.y = (rect->y > (res.y / 2)) ? (rect->y + res.y / 3) :
                          (rect->y - res.y / 3 - rect->h);

    offscreen->reverse = true;
}

void SliderOffscreen_Compute_Next(SliderInput input) {
    // Slider goes offscreen and reappears on the other side

    Slider          *slider     = input.slider;
    SliderOffscreen *offscreen  = input.offscreen;
    Point           *pos        = input.pos;

    SDL_assert(slider   != NULL);
    SDL_assert(pos      != NULL);

    // Skip if not going offscreen
    if (!offscreen->reverse) {
        SliderInput input2      = input;
        input.target            = slider->target;
        input.reverse           = offscreen->reverse;
        Slider_Compute_Next(input2);
        return;
    }

    struct Point res = offscreen->settings->res;

    // check if need to teleport to the other side
    // TODO: use slider widcth to compute teleport position
    /* -- x periodic -- */
    if ((pos->x > res.x) && (offscreen->target.x > res.x)) {
        /* - x positive movement - */
        pos->x = (int)(-res.x * SLIDER_PERIODIC_XP);
        Slider_Init(slider, pos, &slider->target);
        offscreen->reverse = false;
    } else if ((pos->x < (-res.x * SLIDER_PERIODIC_XN_LIMIT)) && (offscreen->target.x < 0)) {
        /* - x negative movement - */
        pos->x = (int)(res.x * SLIDER_PERIODIC_XN);
        Slider_Init(slider, pos, &slider->target);
        offscreen->reverse = false;
    }
    /* -- y periodic -- */
    if ((pos->y > res.y) && (offscreen->target.y > res.y)) {
        /* - y positive movement - */
        pos->y = -res.y * SLIDER_PERIODIC_YP;
        Slider_Init(slider, pos, &slider->target);
        offscreen->reverse = false;
    } else if ((pos->y < (-res.y * SLIDER_PERIODIC_YN_LIMIT)) && (offscreen->target.y < 0)) {
        /* - y negative movement - */
        pos->y = res.y * SLIDER_PERIODIC_YN;
        Slider_Init(slider, pos, &slider->target);
        offscreen->reverse = false;
    }

    // If going offscreen, use offscreen target (which is always offscreen)
    struct Point *current_target = offscreen->reverse ? &offscreen->target : &slider->target;

    SliderInput input2          = input;
    input.target                = *current_target;
    input.reverse               = offscreen->reverse;

    Slider_Compute_Next(input2);
}

Point Slider_Sign(Point      dist,
                  b32        reverse) {
    // Comput sign vector, considering reverse
    const Point torev_sign = Point_Sign(dist);

    // Reversing sign, if element should move offscreen
    const i32 rev_flag = SIGN(!reverse);
    SDL_assert((rev_flag == 1) || (rev_flag == -1));

    Point sign = {
        .x = rev_flag * torev_sign.x,
        .y = rev_flag * torev_sign.y
    };
    SDL_assert((sign.x == 1) || (sign.x == -1) || (sign.x == 0));
    SDL_assert((sign.y == 1) || (sign.y == -1) || (sign.y == 0));
    return (sign);
}

Point Slide_VELOCITY(Slider *slider,
                     SliderSlideInput input) {
    const i32 *speed = slider->ufactors.speed;
    SDL_assert(speed[DIMENSION_X] > 0);
    SDL_assert(speed[DIMENSION_Y] > 0);

    struct Point slide = {
        .x = input.sign.x * speed[DIMENSION_X] / input.fps_eff,
        .y = input.sign.y * speed[DIMENSION_Y] / input.fps_eff
    };
    return (slide);
}

Point Slide_GEOMETRIC(Slider * slider,
                      SliderSlideInput input) {
    // velocity / fps is inverse of rate
    // For geometric with rate 2 on every FRAME, velocity should be half of fps
    const i32 *ratio = slider->ufactors.ratio;
    SDL_assert(ratio[DIMENSION_X] > 0);
    SDL_assert(ratio[DIMENSION_Y] > 0);

    struct Point slide = {
        .x = input.dist.x * ratio[DIMENSION_X] / input.fps_eff,
        .y = input.dist.y * ratio[DIMENSION_Y] / input.fps_eff,
    };
    return (slide);
}

Point Slide_EASYINEASYOUT(Slider * slider,
                          SliderSlideInput input) {
    // TODO move slowly when going offscreen
    // Need to compute periodic  midpoint distance
    struct Point slide = {0};

    const i32 *ratio = slider->ufactors.ratio;

    const struct Point midpoint_dist = {
        .x = input.target.x - slider->midpoint.x,
        .y = input.target.y - slider->midpoint.y
    };

    const struct Point start_dist = {
        .x = slider->start.x - input.pos.x,
        .y = slider->start.y - input.pos.y
    };

    if ((midpoint_dist.x * midpoint_dist.x) <=
        (input.dist.x * input.dist.x)
       ) {
        // Before midpoint:
        i32 min_speed_x =  input.fps_eff / ratio[DIMENSION_X] * 4;
        i32 speed = NMATH_MAX(abs(start_dist.x), min_speed_x);
        slide.x = input.sign.x * speed * ratio[DIMENSION_X] / input.fps_eff;
    } else {
        // After midpoint:
        slide.x = input.dist.x * ratio[DIMENSION_X] / input.fps_eff;
    }

    if ((midpoint_dist.y * midpoint_dist.y) <=
        (input.dist.y * input.dist.y)
       ) {
        // Before midpoint:
        i32 min_speed_y =  input.fps_eff / ratio[DIMENSION_Y] * 4;
        i32 speed = NMATH_MAX(abs(start_dist.y), min_speed_y);
        slide.y = input.sign.y * speed * ratio[DIMENSION_Y] / input.fps_eff;
    } else {
        // After midpoint:
        slide.y = input.dist.y * ratio[DIMENSION_Y] / input.fps_eff;
    }
    return (slide);
}

void Slider_Compute_Next(SliderInput input) {
    /* --- Mext slider position on way to target --- */
    /* TODO: Clean this */

    /* -- Preliminaries -- */
    Slider  *slider     = input.slider;
    SDL_assert(slider   != NULL);
    Point   *pos        = input.pos;
    SDL_assert(pos      != NULL);
    const Point target  = input.target;
    const b32 reverse   = input.reverse;
    SDL_assert((reverse == 0) || (reverse == 1));
    // Note: input.fps_instant is for the previous frame.
    //          Timers get synchronized at end of frame,
    //          input.fps_instant is calculated then.
    f32 fps_eff = Slider_FPS_Effective(slider,
                                       input.fps_instant);

    /* -- Distance to target -- */
    const struct Point dist = {
        .x = target.x - pos->x,
        .y = target.y - pos->y
    };

    /* -- Move to target if Slider close enough -- */
    i32 distsq = (dist.x * dist.x + dist.y * dist.y);
    if (distsq < SLIDER_MIN_DISTSQ) {
        pos->x = target.x;
        pos->y = target.y;
        return;
    }

    /* -- Compute slide value -- */
    const Point sign = Slider_Sign(dist, reverse);
    SDL_assert(slider->slidetype > 0 && slider->slidetype < SLIDETYPE_END);
    SliderSlideInput sliderslideinput = {
        .sign       = sign,
        .dist       = dist,
        .target     = target,
        .pos        = *pos,
        .fps_eff    = fps_eff,
    };
    struct Point slide = slider_slides[slider->slidetype](slider, sliderslideinput);

    /* -- Apply slide i.e. pos += slide -- */
    Slider_Apply_Slide(&pos->x, slide.x, sign.x,
                       target.x, dist.x);
    Slider_Apply_Slide(&pos->y, slide.y, sign.y,
                       target.y, dist.y);
}

void Slider_Apply_Slide(i32 *pos, i32 slide, i32 sign,
                        i32 target, i32 dist) {
    /* Refuse 0 speed.
    ** Target is not reached here, UNLESS sign is 0. */
    i32 nonzero_slide = (slide == 0) ? sign : slide;

    if ((nonzero_slide * nonzero_slide) >= (dist * dist)) {
        /* If overshooting -> move to target instead */
        *pos = target;
    } else {
        *pos += sign * abs(nonzero_slide);
    }
}

f32 Slider_FPS_Effective(Slider * slider,
                         f32 fps_instant) {
    if (fps_instant >= slider->fps_target) {
        return (slider->fps_target);
    }
    /* Default to 0.0, gets clamped to 1.0.
    ** Slow movement in case fps_current is not set. */
    f32 ratio = 0.0f;
    if (fps_instant > 0.0f) {
        ratio = slider->fps_target / fps_instant;
    }
    /* Clamp ratio between reasonable values */
    if (ratio < 1.0f) {
        ratio = 1.0f;
    }
    if (ratio > SLIDER_MAX_LAG_FACTOR) {
        ratio = SLIDER_MAX_LAG_FACTOR;
    }

    return (slider->fps_target * ratio);
}
