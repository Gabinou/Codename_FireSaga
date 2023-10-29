#ifndef SIMPLE_BAR_H
#define SIMPLE_BAR_H

#include "utilities.h"
#include "SDL.h"

/* --- SimpleBar --- */
// Bar drawn without slants or textures, mainly for stats.
// Fill with 2 colors: foreground and background dark
//  - other colors used by StatBar

enum {
    SIMPLEBAR_HEIGHT            = 4,
    SIMPLEBAR_FG_DARK_RECT_NUM  = 4,
    SIMPLEBAR_BLACK_RECT_NUM    = 5,
};

struct SimpleBar {
    float fill;
    size_t len; /* [pixels] as overfilled */
    size_t height; /* [pixels] */
    struct Point pos;
    struct Point scale;
    SDL_Color BG_dark;
    SDL_Color BG_light;
    SDL_Color FG_dark;
    SDL_Color FG_light;
    SDL_RendererFlip flip;
};

extern struct SimpleBar SimpleBar_default;
typedef struct SimpleBar StatBar;

/* -- Drawing -- */
extern void SimpleBar_Draw(struct SimpleBar *SimpleBar, SDL_Renderer *renderer);

#endif /* SIMPLE_BARS_H */
