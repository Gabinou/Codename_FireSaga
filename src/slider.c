
#include "slider.h"
#include "nmath.h"

const struct Slider Slider_default = {
    .slidetype      = SLIDETYPE_GEOMETRIC,
    .ufactors.ratio = {SLIDER_DEFAULT_RATIO, SLIDER_DEFAULT_RATIO},
    .fps            = FPS_DEFAULT_CAP,
};

const struct SliderOffscreen SliderOffscreen_default = {0};

void Slider_Start(struct Slider *slider, struct Point *pos,
                  struct Point  *target) {
    // Define start Slider point depending on slidetype
    switch (slider->slidetype) {
        case SLIDETYPE_EASYINEASYOUT:
            // Compute sequence of distances to add to slider
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
    return (slider->ufactors.rate);
}

void Slider_Target_Offscreen(struct Slider *slider,
                             struct SliderOffscreen *offscreen,
                             struct Point *pos) {
    // Set target of Slider to offscreen, on the CLOSEST edge to pos

    struct Point res = offscreen->settings->res;
    if (slider->target.x != pos->x) {
        // Always move the slider the same distance to offscreen: res
        offscreen->target.x = pos->x > (res.x / 2) ? pos->x + res.x : pos->x - res.x;
    } else {
        offscreen->target.x = slider->target.x; /* just in case (of what?) */
    }

    if (slider->target.y != pos->y) {
        // Always move the slider the same distance to offscreen: res
        offscreen->target.y = pos->y > (res.y / 2) ? pos->y + res.y : pos->y - res.y;
    } else {
        offscreen->target.y = slider->target.y; /* just in case (of what?) */
    }

    offscreen->go_offscreen = true;
}

void SliderOffscreen_Compute_Next(struct Slider *slider,
                                  struct SliderOffscreen *offscreen,
                                  struct Point *pos) {
    // Slider goes offscreen and reappears on the other side
    SDL_assert(slider   != NULL);
    SDL_assert(pos      != NULL);

    // Skip if not going offscreen
    if (!offscreen->go_offscreen) {
        Slider_Compute_Next(slider, pos, &slider->target, offscreen->go_offscreen);
        return;
    }

    struct Point res = offscreen->settings->res;

    // check if need to teleport to the other side
    /* -- x periodic -- */
    if ((pos->x > res.x) && (offscreen->target.x > res.x)) {
        /* - x positive movement - */
        pos->x = (int)(-res.x * SLIDER_PERIODIC_XP);
        Slider_Start(slider, pos, &slider->target);
        offscreen->go_offscreen = false;
    } else if ((pos->x < (-res.x * SLIDER_PERIODIC_XN_LIMIT)) && (offscreen->target.x < 0)) {
        /* - x negative movement - */
        pos->x = (int)(res.x * SLIDER_PERIODIC_XN);
        Slider_Start(slider, pos, &slider->target);
        offscreen->go_offscreen = false;
    }
    /* -- y periodic -- */
    if ((pos->y > res.y) && (offscreen->target.y > res.y)) {
        /* - y positive movement - */
        pos->y = -res.y * SLIDER_PERIODIC_YP;
        Slider_Start(slider, pos, &slider->target);
        offscreen->go_offscreen = false;
    } else if ((pos->y < (-res.y * SLIDER_PERIODIC_YN_LIMIT)) && (offscreen->target.y < 0)) {
        /* - y negative movement - */
        pos->y = res.y * SLIDER_PERIODIC_YN;
        Slider_Start(slider, pos, &slider->target);
        offscreen->go_offscreen = false;
    }

    // If going offscreen, use offscreen target (which is always offscreen)
    struct Point *current_target = offscreen->go_offscreen ? &offscreen->target : &slider->target;
    Slider_Compute_Next(slider, pos, &slider->target, offscreen->go_offscreen);
}

void Slider_Compute_Next(struct Slider *slider, struct Point *pos,
                         struct Point *target, b32 go_offscreen) {
    // Compute the next position of the slider on way to target

    SDL_assert(slider   != NULL);
    SDL_assert(pos      != NULL);
    SDL_assert(target   != NULL);

    const struct Point dist = {target->x - pos->x, target->y - pos->y};

    /* If Slider close enough to target -> move to target */
    if ((dist.x * dist.x + dist.y * dist.y) < SLIDER_MIN_DIST) {
        pos->x = target->x;
        pos->y = target->y;
        return;
    }

    const float *rate = slider->ufactors.rate;
    SDL_assert(rate[DIMENSION_X] > 1.0f);
    SDL_assert(rate[DIMENSION_Y] > 1.0f);

    struct Point slide      = {0};

    switch (slider->slidetype) {
        case SLIDETYPE_EASYINEASYOUT:
            SDL_assert(false);
            // TODO
            break;
        case SLIDETYPE_VELOCITY:
            SDL_assert(false);
            // TODO
            break;
        case SLIDETYPE_GEOMETRIC:
            // velocity / fps is inverse of rate
            // For geometric with rate 2 on every FRAME, velocity should be half of fps
            slide.x = dist.x * slider->ratio[DIMENSION_X] / slider->fps; 
            slide.y = dist.y * slider->ratio[DIMENSION_Y] / slider->fps; 
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
