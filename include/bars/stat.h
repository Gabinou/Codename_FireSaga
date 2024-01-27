#ifndef STAT_BAR_H
#define STAT_BAR_H

#include "bars/simple.h"
#include "SDL2/SDL.h"

/* -- Stat bar and HP bar -- */
// Normal stat bar for Stats menu. Slanted, with highlights.
// Drawn directly on stats menu, without saving it in texture

enum STATBAR {
    STATBAR_HIGHLIGHT_NUM   =  8,
    STATBAR_LEN_FACTOR      =  2,
    STATBAR_MAX_LEN         = 38,
    STATBAR_MAX_CAP         = 40,
    STATBAR_HEIGHT          =  4,
    STATBAR_MAXMIDTILES     = 10, /* ->max filled up stats 60 (10 mid tiles +2 edges) */
    STATBAR_EDGETILES       =  2,
    STATBAR_STATSPERTILE    =  5,
    STATBAR_TEXTURE_ROWLEN  =  8,
    STATBAR_TILESIZE        =  8, /* [pixels] */
    STATBAR_POS_X0          =  8, /* [pixels] */
    STATBAR_POS_Y0          =  8, /* [pixels] */
};
#define STATBAR_MAX_OVERFILL 1.25f
#define STATBAR_STATSPERTILEF 5.0f

extern u8 statbar_highlights[STATBAR_HIGHLIGHT_NUM]; /* < STATBAR_MAX_LEN */

/* --- STAT BAR --- */
/* -- Constructors/Destructors -- */
void StatBar_Init(struct SimpleBar *b, u8 s, u8 c, int x, int y);

/* -- Internals -- */
u8 StatBar_Len(int cap);
void StatBar_Colors_NES(struct SimpleBar *b, int bd, int bl, int fd, int fl);

/* -- Drawing -- */
void StatBar_Draw(struct SimpleBar *b, SDL_Renderer *r);

/* --- HP BAR --- */
/* -- Constructors/Destructors -- */
void HPBar_Init(struct SimpleBar *b, u8 s, u8 c, int x, int y);

/* -- Drawing -- */
void HPBar_Draw(struct SimpleBar *b, SDL_Renderer *r);

#endif /* STAT_BAR_H */
