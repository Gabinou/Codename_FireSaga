#ifndef SIMPLE_BAR_H
#define SIMPLE_BAR_H

#include "structs.h"
#include "SDL.h"

/* --- SimpleBar --- */
/* Bar drawn without slants or textures,
** mainly for stats.
** Fill with 2 colors: foreground and background
** - other colors used by StatBar only
*/

enum {
    SIMPLEBAR_HEIGHT            = 4,
    SIMPLEBAR_FG_DARK_RECT_NUM  = 4,
    SIMPLEBAR_BLACK_RECT_NUM    = 5,
};

typedef struct SimpleBar StatBar;
typedef struct SimpleBar SimpleBar;

/* -- Drawing -- */
extern void SimpleBar_Draw(SimpleBar    *SimpleBar,
                           SDL_Renderer *renderer);

#endif /* SIMPLE_BARS_H */
