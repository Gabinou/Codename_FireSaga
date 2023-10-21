#ifndef MAP_ANIMATION_H
#define MAP_ANIMATION_H

#include "enums.h"
#include "globals.h"
#include "n9patch.h"
#include "bars.h"
#include "game/game.h"
#include "stb_sprintf.h"

/* --- FORWARD DECLARATIONS --- */

// What should animations and cutscene do?
// FOR COMBAT:
//      Input combat Forecast and Attacks
//      Get the correct animation frames
//          To get the frames, need -> SPRITESHEET
//      Play the animation

struct CombatCutscene { /* on Map */

};

struct CombatAnimation { /* on Map */

};

/* --- Constructors/Destructors --- */

/* --- Play --- */
void CombatAnimation_Play(struct CombatAnimation *ca);

#endif /* MAP_ANIMATION_H */
