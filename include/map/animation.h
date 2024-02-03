#ifndef MAP_ANIMATION_H
#define MAP_ANIMATION_H

#include "structs.h"
#include "enums.h"
#include "n9patch.h"
#include "game/game.h"
#include "stb_sprintf.h"

/* --- FORWARD DECLARATIONS --- */
struct CombatAnimation;

// Map_animation Systems:
//     - Unit movement: entity with Unit, MapAnimation components
//         - Movement to do is encoded in Arrow sota->map->arrow
//     - Turn transition: entity with Text, Position, MapAnimation components
//
//     - Unit Combat: entity with Timer,  CombatAnimation
//         - Units to fight are encoded in sota->aggressor, sota->defendant

typedef struct CombatAnimation { /* on Map */
    int attack_ind;
    int pause_after_ms;
    int pause_before_ms;
    int frame_count; /* highest frame index played in current animation */
} CombatAnimation;
extern struct CombatAnimation CombatAnimation_default;


/* --- MapAnimation --- */
// Blocks control (except to go faster)
typedef struct MapAnimation {
    i64 time_ns;
} MapAnimation;
extern struct MapAnimation MapAnimation_default;

/* --- ConstConstructors/Destructors --- */

/* --- Play --- */
// TODO put fps_fsm combat animation functionality here
void Map_UnitMove_Animate(struct Game *s, tnecs_entity, struct Timer *);
void Map_TurnTransition_Animate(struct Game *, tnecs_entity,
                                struct MapAnimation *, struct Timer *);

void Map_Combat_Animate(struct Game *s, tnecs_entity,
                        struct CombatAnimation *, struct Timer *);

#endif /* MAP_ANIMATION_H */
