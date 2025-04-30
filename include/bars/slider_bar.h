#ifndef SLIDER_BAR_H
#define SLIDER_BAR_H

#include "structs.h"
#include "SDL.h"

/* --- SLIDER_BAR --- */
/* Used by user to control settings over a range e.g. volume */
/* Slider never goes out of sliderbar.len */

enum SLIDER_BAR {
    SLIDER_BAR_MIN_LEN      =  2,
    SLIDER_BAR_WHITE        = 55,
    SLIDER_BAR_LIGHT        = 53,
    SLIDER_BAR_DARK         =  2,
    SLIDER_BAR_BLACK        =  1,
    SLIDER_BAR_INCREMENTS   = 16,
};

typedef struct SliderBar {
    struct Point pos;
    int fill;
    int len;
    int increment;
} SliderBar;
extern const SliderBar SliderBar_default;

/* --- Moving Cursor --- */
void SliderBar_Increment(SliderBar *sb);
void SliderBar_Decrement(SliderBar *sb);

/* --- Drawing --- */
void SliderBar_Draw(SliderBar *sb, SDL_Renderer *r);

void _SliderBar_Draw_Bar(   SliderBar *sb, SDL_Renderer *r);
void _SliderBar_Draw_Slider(SliderBar *sb, SDL_Renderer *r);

#endif /* SLIDER_BAR_H */
