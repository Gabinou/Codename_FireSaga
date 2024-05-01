#ifndef MAP_HP_BAR_H
#define MAP_HP_BAR_H

#include "utilities.h"
#include "SDL2/SDL.h"

#define MAP_HPBAR_THRESHOLD_LOW 0.35f
#define MAP_HPBAR_THRESHOLD_MID 0.7f
#define MAP_HPBAR_SQUISH_FACTOR 1.0f

enum MAP_HPBAR {
    MAP_HPBAR_HEIGHT     =  3,
    MAP_HPBAR_GREEN      = 14,
    MAP_HPBAR_YELLOW     = 26,
    MAP_HPBAR_RED        = 32,
    MAP_HPBAR_BACKGROUND = 55,
};

/* --- MapHPBar --- */
// Component. HP bar for units on the map, if they are below 100%.
// Drawn into texture, kept in memory.

typedef struct MapHPBar {
    size_t len; /* [pixels] as filled */
    tnecs_entity unit_ent;
    struct Point tilemap_pos;
    SDL_Texture *texture;
    b32 update  : 1;
    b32 visible : 1;
} MapHPBar;
extern struct MapHPBar MapHPBar_default;

/* --- Drawing --- */
void MapHPBar_Draw(struct MapHPBar *b, struct Camera *c, i32 t[TWO_D],
                   SDL_Texture *rt, SDL_Renderer *r, tnecs_world *w);
void MapHPBar_Update(struct MapHPBar *b, struct Camera *c, i32 t[TWO_D],
                     SDL_Texture *rt, SDL_Renderer *r, tnecs_world *w);

#endif /* MAP_HP_H */
