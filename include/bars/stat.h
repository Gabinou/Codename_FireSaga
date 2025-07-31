#ifndef STAT_BAR_H
#define STAT_BAR_H

#include "bars/simple.h"
#include "SDL.h"

/* -- Stat bar and HP bar --
** Normal stat bar for Stats menu.
** Slanted, with highlights.
** Drawn directly, without saving in texture
*/

enum STATBAR {
    STATBAR_HIGHLIGHT_NUM   =  8,
    STATBAR_LEN_FACTOR      =  2,
    STATBAR_MAX_LEN         = 38,
    STATBAR_MAX_CAP         = 40,
    STATBAR_HEIGHT          =  4,
    /* STATBAR_MAXMIDTILES:
    **  - max filled up stats 60:
    **  - (10 mid tiles +2 edges) */
    STATBAR_MAXMIDTILES     = 10,
    STATBAR_EDGETILES       =  2,
    STATBAR_STATSPERTILE    =  5,
    STATBAR_TEXTURE_ROWLEN  =  8,
    STATBAR_TILESIZE        =  8, /* [px] */
    STATBAR_POS_X0          =  8, /* [px] */
    STATBAR_POS_Y0          =  8, /* [px] */
};
#define STATBAR_MAX_OVERFILL 1.25f
#define STATBAR_STATSPERTILEF 5.0f

extern const int statbar_highlights[STATBAR_HIGHLIGHT_NUM]; /* < STATBAR_MAX_LEN */


typedef struct SimpleBar StatBar;

/* --- STAT BAR --- */
/* -- Constructors/Destructors -- */
void StatBar_Init(SimpleBar *b,
                  int s, int c,
                  int x, int y);

/* -- Internals -- */
int StatBar_Len(int cap);
void StatBar_Colors(SimpleBar *statbar,
                    int BG_dark, int BG_light,
                    int FG_dark, int FG_light);
/* -- Drawing -- */
void StatBar_Draw(SimpleBar *b,
                  SDL_Renderer *r);

/* --- HP BAR --- */
/* -- Constructors/Destructors -- */
void HPBar_Init(SimpleBar *b,
                int s, int c,
                int x, int y);

/* -- Drawing -- */
void HPBar_Draw(SimpleBar *b, SDL_Renderer *r);

#endif /* STAT_BAR_H */
