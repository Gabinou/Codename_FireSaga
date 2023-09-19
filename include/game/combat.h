#ifndef GAME_COMBAT_H
#define GAME_COMBAT_H

#include "game/game.h"
#include "combat.h"

/* --- Combat --- */
extern void Game_Combat_Outcome( struct Game *sota);
extern void Game_Combat_Animate( struct Game *sota);
extern void Game_Combat_Resolves(struct Game *sota);

#endif /* GAME_COMBAT_H */
