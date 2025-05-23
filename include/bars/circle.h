#ifndef CIRCLE_BAR_H
#define CIRCLE_BAR_H

#include "SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct CircleBar;

/* --- CIRCLE_BAR --- */
// Circle bar used to show the number of attacks: 1 for single 2 for double.
// Fills slowly if brave effect/astra effects
// Drawn directly on stats menu, without saving it in memory

enum CIRCLE_BAR {
    CIRCLE_BAR_HEIGHT        =  8,
    CIRCLE_BAR_WIDTH         =  8,
    CIRCLE_BAR_FILL_LEN      =  5,
    CIRCLE_BAR_BORDER_LIGHT  = 53,
    CIRCLE_BAR_BORDER_DARK   = 52,
    CIRCLE_BAR_FILL          = 18,
    CIRCLE_BAR_FILL_DARK     = 17,
    CIRCLE_BAR_FILL_LIGHT    = 15,
    CIRCLE_BAR_SHADOW        =  1,
};

/* --- Drawing --- */
void CircleBar_Draw(struct CircleBar *cb, SDL_Renderer *r);
void CircleBar_Fill(struct CircleBar *cb, int attack_i, int attack_num);

#endif /* CIRCLE_BAR_H */
