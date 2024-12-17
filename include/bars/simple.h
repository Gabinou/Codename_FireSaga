#ifndef SIMPLE_BAR_H
#define SIMPLE_BAR_H

#include "structs.h"
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

typedef struct SimpleBar StatBar;

/* -- Drawing -- */
extern const void SimpleBar_Draw(struct SimpleBar *SimpleBar, SDL_Renderer *renderer);

#endif /* SIMPLE_BARS_H */
