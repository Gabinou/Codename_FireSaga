#ifndef UNIT_BOSS_H
#define UNIT_BOSS_H

#include "types.h"
#include "enums.h"
#include "structs.h"
#include "filesystem.h"
#include "weapon.h"
#include "supports.h"
#include "unit/mount.h"
#include "nmath.h"
#include "jsonio.h"
#include "RNG.h"
#include "physfs.h"
#include "equations.h"
#include "platform.h"
#include "utilities.h"
#include "combat.h"

/* --- FORWARD DECLARATIONS --- */
struct Map;
struct Camera;
struct Position;

/* --- Boss --- */
typedef struct Boss {
    i8 icon;
    SDL_Rect srcrect; /* x,y,w,h */
    SDL_Rect dstrect; /* x,y,w,h */
    SDL_Texture *texture;  /* pixels */
} Boss;
extern struct Boss Boss_default;

void Boss_Free(     struct Boss     *boss);
void Boss_Icon_Pos( struct Boss     *boss, struct Camera    *camera,
                    struct Position *pos,  struct Map       *map);
void Boss_Icon_Load(struct Boss     *boss, SDL_Renderer     *renderer);
void Boss_Icon_Draw(struct Boss     *boss, struct Position  *pos,
                    SDL_Renderer *r);

/* -- Get default boss icon from army -- */
extern int army_icons[ARMY_NUM];

#endif /* UNIT_BOSS_H */
