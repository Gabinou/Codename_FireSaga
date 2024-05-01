
#include "slider.h"

struct Slider Slider_default = {
    .slidetype =       SLIDETYPE_NONE,
    .slide_num =       10,
    .update_wait_ns =  0, // for whole sprite
    .timer_ns =        0, // for whole sprite
    .slidefactors =    {2.0f, 2.0f}, // for SLIDETYPE_GEOMETRIC
    .target =          {0, 0},
};

struct SliderOffscreen SliderOffscreen_default = {
    .target = {0, 0},
    .go_offscreen = false,
};

void Slider_Start(struct Slider *slider, struct Point *pos,
                  struct Point *target) {
    switch (slider->slidetype) {
        case SLIDETYPE_LINEARXY:
        case SLIDETYPE_LINEARYX:
            slider->point.x = (target->x - pos->x) / slider->slide_num * 2;
            slider->point.y = (target->y - pos->y) / slider->slide_num * 2;
            break;
        case SLIDETYPE_LINEAR:
            slider->point.x = (target->x - pos->x) / slider->slide_num;
            slider->point.y = (target->y - pos->y) / slider->slide_num;
            break;
        case SLIDETYPE_EASYINEASYOUT:
            /* midpoint */
            slider->point.x = pos->x + (target->x - pos->x) / 2;
            slider->point.y = pos->y + (target->y - pos->y) / 2;
            break;
    }
}

void Slider_Target_Offscreen(struct Slider *slider, struct SliderOffscreen *offscreen,
                             struct Point *pos) {
    struct Point res = offscreen->settings->res;
    if (slider->target.x != pos->x)
        offscreen->target.x = pos->x > (res.x / 2) ? pos->x + res.x : pos->x - res.x;
    else
        offscreen->target.x = slider->target.x; /* just in case */

    if (slider->target.y != pos->y)
        offscreen->target.y = pos->y > (res.y / 2) ? pos->y + res.y : pos->y -  res.y;
    else
        offscreen->target.y = slider->target.y; /* just in case */

    offscreen->go_offscreen = true;
}

void SliderOffscreen_Compute_Next(struct Slider *slider, struct SliderOffscreen *offscreen,
                                  struct Point *pos) {
    // Slider goes offscreen and reappears on the other side
    SDL_assert(slider != NULL);
    SDL_assert(pos != NULL);

    struct Point res = offscreen->settings->res;
    struct Point *current_target;

    /* - When object goes offscreen, teleport to the other side - */
    if (offscreen->go_offscreen) {
        /* x periodic */
        if ((pos->x > res.x) && (offscreen->target.x > res.x)) {
            pos->x = -res.x * 0.6f;
            Slider_Start(slider, pos, &slider->target);
            offscreen->go_offscreen = false;
        }
        if ((pos->x < (-res.x * 0.2f)) && (offscreen->target.x < 0)) {
            pos->x = res.x * 1.5f;
            Slider_Start(slider, pos, &slider->target);
            offscreen->go_offscreen = false;
        }
        /* y periodic */
        if ((pos->y > res.y) && (offscreen->target.y > res.y)) {
            pos->y = -res.y * 0.8f;
            Slider_Start(slider, pos, &slider->target);
            offscreen->go_offscreen = false;
        }
        if ((pos->y < (-res.y * 0.2f)) && (offscreen->target.y < 0)) {
            pos->y = res.y * 1.5f;
            Slider_Start(slider, pos, &slider->target);
            offscreen->go_offscreen = false;
        }
    }
    current_target = offscreen->go_offscreen ? &offscreen->target : &slider->target;
    Slider_Compute_Next(slider, pos, current_target, offscreen->go_offscreen);
}

void Slider_Compute_Next(struct Slider *slider, struct Point *pos,
                         struct Point *target, b32 go_offscreen) {
    // This function is used by Slidepopup. Should not be changed.
    // Extract what other sprites need

    SDL_assert(slider != NULL);
    SDL_assert(pos != NULL);
    SDL_assert(target != NULL);
    SDL_assert(slider->slidefactors[DIMENSION_X] > 0.0f);
    SDL_assert(slider->slidefactors[DIMENSION_Y] > 0.0f);

    float *sf = slider->slidefactors;
    struct Point dist = {target->x - pos->x, target->y - pos->y};
    struct Point sign; // 0 if dist is 0
    sign.x = -1 * (dist.x < 0) + 1 * (dist.x > 0);
    sign.y = -1 * (dist.y < 0) + 1 * (dist.y > 0);
    struct Point slide = {dist.x, dist.y};
    switch (slider->slidetype) {
        case SLIDETYPE_GEOMETRIC: // Cursor mvt on map
            slide.x = q_sequence_fgeometric_int32_t(target->x, pos->x, sf[DIMENSION_X]);
            slide.y = q_sequence_fgeometric_int32_t(target->y, pos->y, sf[DIMENSION_Y]);
            break;
        case SLIDETYPE_LINEARYX: // for units?
            slide.y = slider->point.y;
            if (pos->y == target->y)
                slide.x = slider->point.x;
            break;
        case SLIDETYPE_LINEARXY: // for units?
            slide.x = slider->point.x;
            if (pos->x == target->x)
                slide.y = slider->point.y;
            break;
        case SLIDETYPE_LINEAR:
            slide.x = slider->point.x;
            slide.y = slider->point.y;
            break;
        case SLIDETYPE_EASYINEASYOUT:; // faster, then slower
            // NOT EASYINEASY OUT -> for slider offscreen type
            b32 going_offscreen = (((pos->x < slider->point.x) && (pos->x < target->x))
                                   || ((pos->y < slider->point.y) && (pos->y < target->y))
                                   || ((pos->x > slider->point.x) && (pos->x > target->x))
                                   || ((pos->y > slider->point.y) && (pos->y > target->y))
                                  );
            if (go_offscreen && going_offscreen) {
                // BORKED for popup_unit
                // Makes popup go out of screen reasonably fast/slow.
                // Geometric is either TOO FAST/TOO SLOW
                i32 seq = q_sequence_fgeometric_int32_t(slider->point.x, pos->x, sf[DIMENSION_X]);
                slide.x = SLIDETYPE_EASYINEASYOUT_FACTOR_X * abs(target->x - pos->x) / seq;
                seq = q_sequence_fgeometric_int32_t(slider->point.y, pos->y, sf[DIMENSION_Y]);
                slide.y = SLIDETYPE_EASYINEASYOUT_FACTOR_Y * abs(target->y - pos->y) / seq;
            } else {
                // Goes back in SUPER fast
                slide.x = q_sequence_fgeometric_int32_t(target->x, pos->x, sf[DIMENSION_X]);
                slide.y = q_sequence_fgeometric_int32_t(target->y, pos->y, sf[DIMENSION_Y]);
            }
            break;
    }
    /* - Anti overshoot. - */
    // sign is 0 if dist is 0, adding 0 to pos
    pos->x += sign.x * NMATH_MIN(abs(dist.x), abs(slide.x));
    pos->y += sign.y * NMATH_MIN(abs(dist.y), abs(slide.y));
}
