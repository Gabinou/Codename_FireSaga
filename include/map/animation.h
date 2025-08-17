#ifndef MAP_ANIMATION_H
#define MAP_ANIMATION_H

#include "enums.h"
#include "structs.h"

/* --- FORWARD DECLARATIONS --- */
struct Game;
struct CombatAnimation;
struct MapAnimation;
struct Graph;

// Map_animation Systems:
//     - Unit movement: entity with Unit, MapAnimation components
//         - Movement to do is encoded in Arrow sota->map->arrow
//     - Turn transition: entity with Text, Position, MapAnimation components
//
//     - Unit Combat: entity with Timer,  CombatAnimation
//         - Units to fight are encoded in sota->combat.aggressor, sota->combat.defendant

typedef struct CombatAnimation { /* on Map */
    i32 attack_ind;
    i32 pause_after_ms;
    i32 pause_before_ms;
    i32 frame_count; /* highest frame index played in current animation */
} CombatAnimation;
extern const struct CombatAnimation CombatAnimation_default;

/* --- MapAnimation --- */
/* Blocks control (except to go faster) */
typedef void (*anim_func_t) (struct Game *, tnecs_E,
                             struct MapAnimation *, struct Timer *);

typedef struct MapAnimation {
    i64 time_ns;
    anim_func_t anim;
} MapAnimation;
extern const struct MapAnimation MapAnimation_default;

/* --- Constructors/Destructors --- */
void MapAnimation_Init(struct MapAnimation *manim);
void MapAnimation_Init_tnecs(void *voidmanim);
void CombatAnimation_Init(struct CombatAnimation *canim);
void CombatAnimation_Init_tnecs(void *voidcanim);

/* --- Play --- */
// TODO put fps_fsm combat animation functionality here
void Map_UnitMove_Animate(struct Game *s, tnecs_E, struct Timer *);
void Map_GameOver_Animate(struct Game *s, tnecs_E e,
                          struct MapAnimation *m, struct Timer *t);
void Map_TurnTransition_Animate(struct Game *s, tnecs_E e,
                                struct MapAnimation *m, struct Timer *t);

void Map_Combat_Animate(struct Game *s, tnecs_E,
                        struct CombatAnimation *, struct Timer *);

#endif /* MAP_ANIMATION_H */
