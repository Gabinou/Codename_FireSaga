#ifndef MAP_HP_BAR_H
#define MAP_HP_BAR_H

#include "structs.h"
#include "SDL.h"

#define MAP_HPBAR_THRESHOLD_LOW 0.35f
#define MAP_HPBAR_THRESHOLD_MID 0.7f
#define MAP_HPBAR_SQUISH_FACTOR 1.0f

enum MAP_HPBAR {
    MAP_HPBAR_HEIGHT     = 3,
    MAP_HPBAR_GREEN      = SOTA_GREEN,
    MAP_HPBAR_YELLOW     = SOTA_YELLOW,
    MAP_HPBAR_RED        = SOTA_RED,
    MAP_HPBAR_BACKGROUND = 2,
};

/* --- MapHPBar --- */
// Component. HP bar for units on the map, if they are below 100%.
// Drawn into texture, kept in memory.

typedef struct MapHPBar {
    struct Point tilemap_pos;
    SDL_Texture *texture;
    tnecs_entity unit_ent;
    size_t len; /* [pixels] as filled */
    b32 update;
    b32 visible;
} MapHPBar;
extern const struct MapHPBar MapHPBar_default;

void MapHPBar_Init(struct MapHPBar *maphpbar);
void MapHPBar_Init_tnecs(void *voidmaphpbar);

/* --- Drawing --- */
void MapHPBar_Draw(struct MapHPBar *b, struct Camera *c, i32 t[TWO_D],
                   SDL_Texture *rt, SDL_Renderer *r, tnecs_world *w);
void MapHPBar_Update(struct MapHPBar *b, struct Camera *c, i32 t[TWO_D],
                     SDL_Texture *rt, SDL_Renderer *r, tnecs_world *w);

#endif /* MAP_HP_H */
