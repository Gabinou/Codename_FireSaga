#ifndef DEBUG_H
#define DEBUG_H

#include "macros.h"

/* -- Release switches -- */
// #define SOTA_NO_LOGS

/* -- Various defines -- */
#define SOTA_RNG SOTA_RN_DOUBLE

#define RENDER2WINDOW /* VERY FAST */

#define SOTA_DEBUG_MENU_BOXES

#define DEBUG_POPUP_TILE_OFFSCREEN
#define DEBUG_POPUP_UNIT_OFFSCREEN

#define DEBUG_CURSOR_SHOW_wDISABLED

/* -- MUSIC & SOUNDS -- */
// #define DEBUG_NO_MUSIC
// #define DEBUG_NO_SOUNDFX

// #define SOTA_PLAYER_CONTROLS_ENEMY
// #define SOTA_PLAYER_CONTROLS_NEUTRAL

// #define DEBUG_LOADOUT_STATS_SHOW_ARROWS

/* Flags for debugging. 0 disabled, 1 enabled. */

/* Flags for debugging. 0 disabled, 1 enabled. */
#define INVINCIBILITY_ALL
#define INVINCIBILITY_FRIENDLY
#define INVINCIBILITY_ENEMY

#define RNG_SEQUENCE_BREAKER_HIT
#define RNG_SEQUENCE_BREAKER_MISS
#define RNG_SEQUENCE_BREAKER_CRIT
#define RNG_SEQUENCE_BREAKER_NOCRIT
// #define RNG_SEQUENCE_BREAKER_HIT_GROWTH /* should never be used, if players gets long sequences of nice growths, its good.*/
/* TODO: GROWTHS MISS. */
#define RNG_SEQUENCE_BREAKER_MISS_GROWTH

#define PASSTHROUGH_ALL
#define PASSTHROUGH_FRIENDLY /* FRIENDLIES can passthrough ENEMIES*/
#define PASSTHROUGH_ENEMY /* ENEMIES can passthrough FRIENDLIES*/

/* UNIT */
// #define DEBUG_UNIT_MOVEAFTERWAIT

#define TWOHANDING_DOUBLES_DAMAGE

// #define INFINITE_MOVE_ALL
#define INFINITE_MOVE_FRIENDLY
#define INFINITE_MOVE_ENEMY

#define INFINITE_RANGE_ALL
#define INFINITE_RANGE_FRIENDLY
#define INFINITE_RANGE_ENEMY

/* Disable Map functions that update it every X frame / Y us */
#define MAP_NO_ANIMATION
#define MAP_TILEMAP_NO_ANIMATION
#define MAP_SHADOW_NO_ANIMATION

/* Set hp to 1 basically */
#define INSTANT_DEATH_ALL
#define INSTANT_DEATH_FRIENDLY /* FRIENDLIES dies instantly */
#define INSTANT_DEATH_ENEMY /* ENEMIES dies instantly */

// #define UNITS_IGNORE_TERRAIN /* effectively makes costmap all ones */

/* Puts all usable ultimate weapons in equipment of units*/
#define ULTIMATE_WEAPONS_ALL
#define ULTIMATE_WEAPONS_FRIENDLY
#define ULTIMATE_WEAPONS_ENEMY

#define CAP_ALLSTATS_ALL
#define CAP_ALLSTATS_FRIENDLYa
#define CAP_ALLSTATS_ENEMY

#define CAP_HP_ALL
#define CAP_HP_FRIENDLY
#define CAP_HP_ENEMY

#define CAP_STR_ALL
#define CAP_STR_FRIENDLY
#define CAP_STR_ENEMY

#define CAP_MAG_ALL
#define CAP_MAG_FRIENDLY
#define CAP_MAG_ENEMY

#define CAP_DEX_ALL
#define CAP_DEX_FRIENDLY
#define CAP_DEX_ENEMY

#define CAP_AGI_ALL
#define CAP_AGI_FRIENDLY
#define CAP_AGI_ENEMY

#define CAP_LUCK_ALL
#define CAP_LUCK_FRIENDLY
#define CAP_LUCK_ENEMY

#define CAP_DEF_ALL
#define CAP_DEF_FRIENDLY
#define CAP_DEF_ENEMY

#define CAP_RES_ALL
#define CAP_RES_FRIENDLY
#define CAP_RES_ENEMY

#define CAP_CON_ALL
#define CAP_CON_FRIENDLY
#define CAP_CON_ENEMY

#define CAP_PROF_ALL
#define CAP_PROF_FRIENDLY
#define CAP_PROF_ENEMY

#define SKIP_TALKSCENES
#define SKIP_CUTSCENES
#define SKIP_PREPARATION
#define SKIP_ENEMYTURN
#define SKIP_PLAYERTURN
#define SKIP_FRIENDLYTURN

#endif /* DEBUG_H */
