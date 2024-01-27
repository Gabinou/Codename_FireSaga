#ifndef TOPOFF_BAR_H
#define TOPOFF_BAR_H

#include <math.h>
#include "bars/simple.h"
#include "utilities.h"
#include "SDL2/SDL.h"

/* -- TopOffbar -- */
// Bar with top off, for map combat
// Drawn directly on stats menu, without saving it in memory

enum TOPOFF_BAR {
    TOPOFF_BAR_HEIGHT   =  4,
    TOPOFF_BAR_SHADOW   =  1,
    TOPOFF_BAR_FG_LIGHT = 15,
    TOPOFF_BAR_FG_DARK  = 14,
    TOPOFF_BAR_BG_LIGHT = 53,
    TOPOFF_BAR_BG_DARK  = 52,
};

/* -- Drawing -- */
void TopoffBar_Draw(struct SimpleBar *sb, SDL_Renderer *r);

#endif /* TOPOFF_BAR_H */
