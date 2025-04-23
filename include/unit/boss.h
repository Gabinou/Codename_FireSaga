#ifndef UNIT_BOSS_H
#define UNIT_BOSS_H

#include "types.h"
#include "enums.h"
#include "structs.h"
#include "SDL.h"

/* --- FORWARD DECLARATIONS --- */
struct Map;
struct Camera;
struct Position;

/* --- Boss --- */
// For friendlies: Friendly is a main character
//      - Dieing leads to loss
// For enemies: Enemy is a boss
//      - Killing may lead to win
typedef struct Boss {
    i8 icon;
    SDL_Rect srcrect; /* x,y,w,h */
    SDL_Rect dstrect; /* x,y,w,h */
    SDL_Texture *texture;  /* pixels */
} Boss;
extern const struct Boss Boss_default;

void Boss_Free(     struct Boss     *boss);
void Boss_Icon_Pos( struct Boss     *boss, struct Camera    *camera,
                    struct Position *pos,  struct Map       *map);
int Boss_Icon_Load(struct Boss     *boss, SDL_Renderer     *renderer);
void Boss_Icon_Draw(struct Boss     *boss, struct Position  *pos,
                    SDL_Renderer *r);

/* -- Get default boss icon from army -- */
extern const int army_icons[ARMY_MALLOC];

#endif /* UNIT_BOSS_H */
