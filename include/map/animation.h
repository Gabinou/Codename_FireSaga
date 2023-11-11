#ifndef MAP_ANIMATION_H
#define MAP_ANIMATION_H

#include "enums.h"
#include "n9patch.h"
#include "bars.h"
#include "game/game.h"
#include "stb_sprintf.h"

/* --- FORWARD DECLARATIONS --- */

// Map_animation Systems: 
//     - Unit movement: entity with Unit, MapAnimation components
//         - Movement to do is encoded in Arrow sota->map->arrow
//     - Turn transition: entity with Text, Position, MapAnimation components
//     - Unit Combat: entity with Timer,  MapAnimation
//         - Units to fight are encoded in sota->aggressor, sota->defendant


/* --- MapAnimation --- */
// Blocks control (except to go faster)
typedef struct MapAnimation { /* on Map */
    
}MapAnimation;

    // TODO change to MapAnimation component
typedef struct CombatAnimation { /* on Map */

} CombatAnimation;

/* --- Constructors/Destructors --- */

/* --- Play --- */
// TODO put fps_fsm combat animation functionality here
void CombatAnimation_Play();

#endif /* MAP_ANIMATION_H */
