#ifndef TOPOFF_BAR_H
#define TOPOFF_BAR_H

#include "SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct SimpleBar;

/* -- TopOffbar -- */
// Bar with top off, for map combat
// Drawn directly on stats menu, without saving it in memory

enum TOPOFF_BAR_NES {
    TOPOFF_BAR_NES_HEIGHT   =  4,
    TOPOFF_BAR_NES_SHADOW   =  1,
    TOPOFF_BAR_NES_FG_LIGHT = 15,
    TOPOFF_BAR_NES_FG_DARK  = 14,
    TOPOFF_BAR_NES_BG_LIGHT = 53,
    TOPOFF_BAR_NES_BG_DARK  = 52,
};

enum TOPOFF_BAR_SOTA {
    TOPOFF_BAR_HEIGHT   =  4,
    TOPOFF_BAR_SHADOW   =  1,
    TOPOFF_BAR_FG_LIGHT = 39,
    TOPOFF_BAR_FG_DARK  = 38,
    TOPOFF_BAR_BG_LIGHT =  4,
    TOPOFF_BAR_BG_DARK  =  3,
};

/* -- Drawing -- */
void TopoffBar_Draw(struct SimpleBar *sb, SDL_Renderer *r);

#endif /* TOPOFF_BAR_H */
