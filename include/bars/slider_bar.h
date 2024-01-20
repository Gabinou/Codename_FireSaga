#ifndef SLIDER_BAR_H
#define SLIDER_BAR_H

#include "utilities.h"
#include "SDL.h"

/* --- SLIDER_BAR --- */
/* Used by user to control settings over a range e.g. volume */

enum SLIDER_BAR {
    SLIDER_BAR_MIN_LEN  =  2,
    SLIDER_BAR_WHITE    = 55,
    SLIDER_BAR_LIGHT    = 53,
    SLIDER_BAR_DARK     =  2,
    SLIDER_BAR_BLACK    =  1,
};

typedef struct SliderBar {
    int fill;
    int len;
    struct Point pos;
} SliderBar;
extern SliderBar SliderBar_default;

/* --- Drawing --- */
void SliderBar_Draw(SliderBar *cb, SDL_Renderer *r);

void _SliderBar_Draw_Bar(   SliderBar *cb, SDL_Renderer *r);
void _SliderBar_Draw_Slider(SliderBar *cb, SDL_Renderer *r);

#endif /* SLIDER_BAR_H */
