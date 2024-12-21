
#include "slider.h"
#include "nmath.h"

const struct Slider Slider_default = { /* SLIDETYPE_GEOMETRIC */
    .slidetype      = SLIDETYPE_NONE,
    .ufactors.rate  = {2.0f, 2.0f},
};

const struct Slider Slider_Linear_default = {
    .slidetype          = SLIDETYPE_NONE,
    .ufactors.slide_num = 10,
};

const struct SliderOffscreen SliderOffscreen_default = {0};

void Slider_Start(struct Slider *slider, struct Point *pos,
                  struct Point  *target) {
    // Define start Slider point depending on slidetype
    switch (slider->slidetype) {
        case SLIDETYPE_LINEARXY:
        case SLIDETYPE_LINEARYX:
            SDL_assert(false);
            // TODO: FIX
            // Distance to move in one step
            // slider->upoint.distance.x = (target->x - pos->x) / slider->slide_num * 2;
            // slider->upoint.distance.y = (target->y - pos->y) / slider->slide_num * 2;
            break;
        case SLIDETYPE_LINEAR:
            SDL_assert(false);
            // TODO: FIX
            // Distance to move in one step
            // slider->upoint.distance.x = (target->x - pos->x) / slider->slide_num;
            // slider->upoint.distance.y = (target->y - pos->y) / slider->slide_num;
            break;
        case SLIDETYPE_EASYINEASYOUT:
            slider->upoint.midpoint.x = pos->x + (target->x - pos->x) / 2;
            slider->upoint.midpoint.y = pos->y + (target->y - pos->y) / 2;
            break;
    }
}

void Slider_Rate_Set(Slider *slider, float rate0, float rate1) {
    slider->ufactors.rate[0] = rate0;
    slider->ufactors.rate[1] = rate1;
}

float* Slider_Rate(Slider *slider) {
    return(slider->ufactors.rate);
}

i32 Slider_Slide_Num(Slider *slider) {
    return(slider->ufactors.slide_num);
}


void Slider_Target_Offscreen(struct Slider *slider, struct SliderOffscreen *offscreen,
                             struct Point *pos) {
    // Set target of Slider to offscreen, on the CLOSEST edge to pos

    struct Point res = offscreen->settings->res;
    if (slider->target.x != pos->x)
        offscreen->target.x = pos->x > (res.x / 2) ? pos->x + res.x : pos->x - res.x;
    else
        offscreen->target.x = slider->target.x; /* just in case */

    if (slider->target.y != pos->y)
        offscreen->target.y = pos->y > (res.y / 2) ? pos->y + res.y : pos->y - res.y;
    else
        offscreen->target.y = slider->target.y; /* just in case */

    offscreen->go_offscreen = true;
}

void SliderOffscreen_Compute_Next(struct Slider *slider, struct SliderOffscreen *offscreen,
                                  struct Point *pos) {
    // Slider goes offscreen and reappears on the other side
    SDL_assert(slider   != NULL);
    SDL_assert(pos      != NULL);

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
    // Compute the next position of the slider.

    SDL_assert(slider   != NULL);
    SDL_assert(pos      != NULL);
    SDL_assert(target   != NULL);
    SDL_assert(slider->ufactors.rate[DIMENSION_X] > 1.0f);
    SDL_assert(slider->ufactors.rate[DIMENSION_Y] > 1.0f);

    const struct Point dist = {target->x - pos->x, target->y - pos->y};
    /* If Slider close enough to target -> move to target */
    if ((dist.x * dist.x + dist.y * dist.y) < SLIDER_MIN_DIST) {
        pos->x = target->x;
        pos->y = target->y;
        return;
    }

    float *rate = slider->ufactors.rate;

    struct Point slide      = {0};

    switch (slider->slidetype) {
        case SLIDETYPE_GEOMETRIC: // Cursor mvt on map
            slide.x = q_sequence_fgeometric_int32_t(target->x, pos->x, rate[DIMENSION_X]);
            slide.y = q_sequence_fgeometric_int32_t(target->y, pos->y, rate[DIMENSION_Y]);
            break;
        case SLIDETYPE_LINEARYX:
            SDL_assert(false);
            // TODO: FIX
            // slide.y = slider->point.y;
            // if (pos->y == target->y)
            // slide.x = slider->point.x;
            break;
        case SLIDETYPE_LINEARXY: // for units?   
            SDL_assert(false);
            // TODO: FIX
            // slide.x = slider->point.x;
            // if (pos->x == target->x)
            //     slide.y = slider->point.y;
            break;
        case SLIDETYPE_LINEAR:
            SDL_assert(false);
            // TODO: FIX
            // slide.x = slider->point.x;
            // slide.y = slider->point.y;
            break;
        case SLIDETYPE_EASYINEASYOUT:
            ;
            b32 going_offscreen_xn = (pos->x < slider->upoint.midpoint.x) && (pos->x < target->x);
            b32 going_offscreen_xp = (pos->x > slider->upoint.midpoint.x) && (pos->x > target->x);
            b32 going_offscreen_yn = (pos->y < slider->upoint.midpoint.y) && (pos->y < target->y);
            b32 going_offscreen_yp = (pos->y > slider->upoint.midpoint.y) && (pos->y > target->y);
                                                                                     
            b32 going_offscreen = ( going_offscreen_xn
                                    || going_offscreen_xp
                                    || going_offscreen_yn
                                    || going_offscreen_yp);

            if (go_offscreen && going_offscreen) {
                // BORKED for popup_unit
                // Makes popup go out of screen reasonably fast/slow.
                // Geometric is either TOO FAST/TOO SLOW
                i32 seq = q_sequence_fgeometric_int32_t(slider->upoint.midpoint.x, pos->x, rate[DIMENSION_X]);
                slide.x = SLIDETYPE_EASYINEASYOUT_FACTOR_X * abs(target->x - pos->x) / seq;
                seq = q_sequence_fgeometric_int32_t(slider->upoint.midpoint.y, pos->y, rate[DIMENSION_Y]);
                slide.y = SLIDETYPE_EASYINEASYOUT_FACTOR_Y * abs(target->y - pos->y) / seq;
            } else {
                // Goes back in SUPER fast
                slide.x = q_sequence_fgeometric_int32_t(target->x, pos->x, rate[DIMENSION_X]);
                slide.y = q_sequence_fgeometric_int32_t(target->y, pos->y, rate[DIMENSION_Y]);
            }
            break;
    }

    if (abs(slide.x) >= abs(dist.x)) {
        /* If Slider overshoots to target -> move to target */
        pos->x = target->x;
    } else {
        pos->x += slide.x;

    }
    if (abs(slide.y) >= abs(dist.y)) {
        /* If Slider overshoots to target -> move to target */
        pos->y = target->y;
    } else {
        pos->y += slide.y;
    }
}
